package codec

import (
	"bytes"
	"code.google.com/p/goprotobuf/proto"
	"encoding/binary"
	"errors"
	"fmt"
	"github.com/golang/glog"
	"hash/crc32"
	"net"
)

func SendMessage(conn net.Conn, messageName string, message proto.Message) error {
	messageData, err := proto.Marshal(message)
	if err != nil {
		return err
	}
	encodedBuff, err := EncodePacket(messageData, messageName)
	if err != nil {
		return err
	}
	bytesTransfered, err := conn.Write(encodedBuff)
	if err != nil {
		return err
	}
	if bytesTransfered != len(encodedBuff) {
		return errors.New("conn.Write Error not all bytes transfered.")
	}
	if messageName != "core.Heartbeat" {
		glog.V(2).Infoln("Sent message:", messageName, "to:", conn.RemoteAddr())
	}
	return nil
}

func readPacketLength(conn net.Conn) (int32, error) {
	dataLenBuff := make([]byte, 4) // 4 byte data length
	bytesRead, err := conn.Read(dataLenBuff)
	if err != nil {
		return 0, err
	}
	if bytesRead != 4 {
		return 0, errors.New("Error when reading 4 bytes data length")
	}
	var dataLen int32
	err = binary.Read(bytes.NewBuffer(dataLenBuff), binary.BigEndian, &dataLen)
	if err != nil {
		return 0, err
	}
	return dataLen, nil
}

// encode the TransportFormatMessage
func EncodePacket(buff []byte, messageName string) ([]byte, error) {
	messageName = messageName + "\x00"

	var nameLen int32
	nameLen = int32(len(messageName))

	var packetLen int32
	packetLen = int32(4) + int32(4) + nameLen + int32(len(buff)) + int32(4)
	// log.Println("Packet length:", packetLen)

	packetBuff := make([]byte, packetLen)

	binary.BigEndian.PutUint32(packetBuff, uint32(packetLen-4)) // exclude the for bytes
	// log.Println("Packet length bytes:", packetBuff[0:4])
	binary.BigEndian.PutUint32(packetBuff[4:], uint32(nameLen))
	// log.Println("Name length bytes:", packetBuff[4:8])

	copy(packetBuff[8:], messageName)
	// log.Println("Message Name bytes:", packetBuff[8:8+nameLen])
	copy(packetBuff[8+nameLen:], buff)

	h := crc32.NewIEEE()
	h.Write(packetBuff[4 : packetLen-4])
	checksum := h.Sum32()
	binary.BigEndian.PutUint32(packetBuff[packetLen-4:], uint32(checksum))
	// TODO: caculate the checksum
	// if err := binary.Write(bytes.NewBuffer(packetBuff[packetLen-4 : packetLen-4]), binary.BigEndian, &checksum); err != nil {
	// 	return nil, err
	// }
	// log.Println("Packet Buffer length:", len(packetBuff))
	return packetBuff, nil
}

func DecodePacket(conn net.Conn) ([]byte, string, error) {
	packetLen, err := readPacketLength(conn)
	if err != nil {
		return nil, "", errors.New(fmt.Sprintf("Can't read the packet length: %s", err))
	}
	// log.Println("Received packet length:", packetLen)

	buf := make([]byte, packetLen)
	left := packetLen
	for {
		bytesTransfered, err := conn.Read(buf[packetLen-left : packetLen])
		if err != nil {
			glog.Errorln("conn.Read error:", err.Error())
			return nil, "", errors.New("Can't read the necessary bytes.")
		}
		// log.Println("Bytes transfered:", bytesTransfered)
		left = left - int32(bytesTransfered)
		if left == 0 {
			break
		}
	}
	// TODO: verify the checksum
	h := crc32.NewIEEE()
	h.Write(buf[0 : packetLen-4])
	checkSum := h.Sum32()

	var receivedChecksum uint32
	err = binary.Read(bytes.NewBuffer(buf[packetLen-4:]), binary.BigEndian, &receivedChecksum)
	if err != nil {
		return nil, "", errors.New("Can't get the checksum.")
	}
	if checkSum != receivedChecksum {
		return nil, "", errors.New("Checksum error.")
	}
	var nameLen int32
	err = binary.Read(bytes.NewBuffer(buf[0:4]), binary.BigEndian, &nameLen)
	if err != nil {
		return nil, "", errors.New("Can't convert the namelength, invalid packet received.")
	}
	messageName := string(buf[4 : 4+nameLen-1]) // -1 to exclude the last '\0'
	// log.Println("Message received:", messageName)
	return buf[4+nameLen : packetLen-4], messageName, nil
}
