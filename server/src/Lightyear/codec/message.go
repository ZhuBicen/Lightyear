package codec

import (
	"code.google.com/p/goprotobuf/proto"
)

type Message struct {
	Name   string
	Buffer []byte
}

type ToMessage struct {
	Name string
	Msg  proto.Message
}

//func NewToMessage(name string, msg proto.Message) ToMessage {
//	return ToMessage{
//		Name: name,
//		Msg:  msg,
//	}
//}

type Event struct {
	Id int
}
