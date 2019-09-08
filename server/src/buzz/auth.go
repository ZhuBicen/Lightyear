package main

import (
	"code.google.com/p/goprotobuf/proto"
	"errors"
	"fmt"
	"github.com/golang/glog"
	"lightyear/codec"
	"lightyear/db"
	"lightyear/proto/core"
	"net"
)

func UnmarshalAuthenticationReq(buff []byte) *core.AuthenticationReq {
	car := new(core.AuthenticationReq)
	err := proto.Unmarshal(buff, car)
	if err != nil {
		return nil
	}
	return car
}
func OnCoreAuthenticationReq(conn net.Conn, car *core.AuthenticationReq) error {

	resp := &core.AuthenticationResp{
		Resp: &core.Response{
			Status: proto.Int32(0),
			Cause:  proto.String(""),
		},
	}
	respCode := resp.Resp
	if !db.VerifyPassword(car.GetJid().GetId(), *car.Password) {
		respCode.Status = proto.Int32(1)
		respCode.Cause = proto.String("ID或密码错误!")
	} else {
		resourceId := g_ol.AllocateResource(*car.GetJid())
		fullJid := new(core.Jid)
		fullJid.Resource = proto.String(resourceId)
		fullJid.Domain = car.GetJid().Domain
		fullJid.Id = car.GetJid().Id
		resp.FullJid = fullJid
	}
	if err := codec.SendMessage(conn, "core.AuthenticationResp", resp); err != nil {
		glog.Error(err)
		return err
	}
	if *respCode.Status != 0 {
		return errors.New(fmt.Sprintf("Password or network error, Status: %d", *respCode.Status))
	}
	return nil
}
