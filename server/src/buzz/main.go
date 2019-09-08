package main

import (
	_ "buzz/routers"
	"flag"
	"github.com/astaxie/beego"
	"github.com/golang/glog"
	"lightyear/codec"
	"lightyear/ol"
	"lightyear/proto/core"
	"net"
	"time"
)

var g_ol ol.Users

func main() {
	go tcpServer()
	beego.SetStaticPath("/screenshot", "screenshot")
	beego.SetStaticPath("/emotion", "emotion")
	beego.Run()
}

func tcpServer() {
	//
	// TODO why the glog flag log_dir can't be parsed here?
	//
	flag.Parse()

	glog.Info("Starting TCP server ...")

	//
	g_ol.Entities = make(map[string][]ol.Entity)

	// create listener:
	listener, err := net.Listen("tcp", ":8089")
	if err != nil {
		glog.Fatal("Error listening", err.Error())
		return // terminate program
	}
	// listen and accept connections from clients:
	for {
		conn, err := listener.Accept()
		if err != nil {
			glog.Errorln("Error accepting:", err.Error())
			continue
		}
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	// the remote port was reused, safely disconnect the already connected entity
	// g_ol.Disconnect(conn)

	defer conn.Close()
	messageBuffer, messageName, err := codec.DecodePacket(conn)
	if err != nil {
		glog.Error(err)
		return
	}
	if messageName != "core.AuthenticationReq" {
		glog.Error("first message should be: core.AuthenticationReq")
		return
	}
	var car *core.AuthenticationReq
	if car = UnmarshalAuthenticationReq(messageBuffer); car == nil {
		glog.Error("decode error, ignore connection")
		return
	}
	if err := OnCoreAuthenticationReq(conn, car); err != nil {
		glog.Error("client should re-establish the connection. CODE:", err)
		return
	}

	// Create the entity(represents the online User Equipment)
	entity := g_ol.Connect(conn, *car.GetJid())
	defer g_ol.Disconnect(entity)

	// read data from socket, and pump out
	go func() {
		for {
			messageBuffer, messageName, err = codec.DecodePacket(conn)
			if err != nil {
				glog.Errorln(err)
				close(entity.MessageFromPeerChan)
				return
			}
			entity.MessageFromPeerChan <- codec.Message{messageName, messageBuffer}
		}
	}()
	// send message to peer
	go func() {
		for {
			toSendMessage, ok := <-entity.MessageToPeerChan
			if !ok {
				return
			}
			// TODO check the return value
			err := codec.SendMessage(conn, toSendMessage.Name, toSendMessage.Msg)
			if err != nil {
				glog.Error("Error when sending message:", toSendMessage.Name)
				return
			}
		}
	}()

	go func() {
		// Send heartbeat every 2 seconds.
		for {
			time.Sleep(2 * time.Second)
			hb := new(core.Heartbeat)
			entity.MessageToPeerChan <- codec.ToMessage{"core.Heartbeat", hb}
		}
	}()
	for {
		// read message from messageChan, read event from eventChan
		select {
		case message, ok := <-entity.MessageFromPeerChan:
			if !ok {
				entity.MessageFromPeerChan = nil
				glog.Info("Disconnecting entity", entity.Address)
				return
			}
			entity.ProcessMessage(message)
		// if didn't receive message from peer within N seconds, send a heartbeat
		case <-time.After(8 * time.Second):
			glog.Info("Connection lost entity:", entity.Address)
			return
		}
	}
}
