// Code generated by protoc-gen-go.
// source: core.proto
// DO NOT EDIT!

package core

import proto "code.google.com/p/goprotobuf/proto"
import json "encoding/json"
import math "math"

// Reference proto, json, and math imports to suppress error if they are not otherwise used.
var _ = proto.Marshal
var _ = &json.SyntaxError{}
var _ = math.Inf

// Resource Binding
type AuthenticationReq_ResourceType int32

const (
	AuthenticationReq_WINPC   AuthenticationReq_ResourceType = 0
	AuthenticationReq_ANDROID AuthenticationReq_ResourceType = 1
	AuthenticationReq_IPHONE  AuthenticationReq_ResourceType = 2
	AuthenticationReq_IPAD    AuthenticationReq_ResourceType = 3
)

var AuthenticationReq_ResourceType_name = map[int32]string{
	0: "WINPC",
	1: "ANDROID",
	2: "IPHONE",
	3: "IPAD",
}
var AuthenticationReq_ResourceType_value = map[string]int32{
	"WINPC":   0,
	"ANDROID": 1,
	"IPHONE":  2,
	"IPAD":    3,
}

func (x AuthenticationReq_ResourceType) Enum() *AuthenticationReq_ResourceType {
	p := new(AuthenticationReq_ResourceType)
	*p = x
	return p
}
func (x AuthenticationReq_ResourceType) String() string {
	return proto.EnumName(AuthenticationReq_ResourceType_name, int32(x))
}
func (x *AuthenticationReq_ResourceType) UnmarshalJSON(data []byte) error {
	value, err := proto.UnmarshalJSONEnum(AuthenticationReq_ResourceType_value, data, "AuthenticationReq_ResourceType")
	if err != nil {
		return err
	}
	*x = AuthenticationReq_ResourceType(value)
	return nil
}

// http://xmpp.org/rfcs/rfc6120.html
type Jid struct {
	Id               *string `protobuf:"bytes,1,req,name=id" json:"id,omitempty"`
	Domain           *string `protobuf:"bytes,2,opt,name=domain" json:"domain,omitempty"`
	Resource         *string `protobuf:"bytes,3,opt,name=resource" json:"resource,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *Jid) Reset()         { *m = Jid{} }
func (m *Jid) String() string { return proto.CompactTextString(m) }
func (*Jid) ProtoMessage()    {}

func (m *Jid) GetId() string {
	if m != nil && m.Id != nil {
		return *m.Id
	}
	return ""
}

func (m *Jid) GetDomain() string {
	if m != nil && m.Domain != nil {
		return *m.Domain
	}
	return ""
}

func (m *Jid) GetResource() string {
	if m != nil && m.Resource != nil {
		return *m.Resource
	}
	return ""
}

type AuthenticationReq struct {
	// The term "bare JID" refers to an XMPP address of the form <localpart@domainpart> (for an account at a server) or of the form <domainpart> (for a server).
	Jid              *Jid                            `protobuf:"bytes,1,req,name=jid" json:"jid,omitempty"`
	Password         *string                         `protobuf:"bytes,2,opt,name=password" json:"password,omitempty"`
	ResourceType     *AuthenticationReq_ResourceType `protobuf:"varint,3,opt,name=resourceType,enum=core.AuthenticationReq_ResourceType,def=0" json:"resourceType,omitempty"`
	ResourceName     *string                         `protobuf:"bytes,4,opt,name=resourceName" json:"resourceName,omitempty"`
	XXX_unrecognized []byte                          `json:"-"`
}

func (m *AuthenticationReq) Reset()         { *m = AuthenticationReq{} }
func (m *AuthenticationReq) String() string { return proto.CompactTextString(m) }
func (*AuthenticationReq) ProtoMessage()    {}

const Default_AuthenticationReq_ResourceType AuthenticationReq_ResourceType = AuthenticationReq_WINPC

func (m *AuthenticationReq) GetJid() *Jid {
	if m != nil {
		return m.Jid
	}
	return nil
}

func (m *AuthenticationReq) GetPassword() string {
	if m != nil && m.Password != nil {
		return *m.Password
	}
	return ""
}

func (m *AuthenticationReq) GetResourceType() AuthenticationReq_ResourceType {
	if m != nil && m.ResourceType != nil {
		return *m.ResourceType
	}
	return Default_AuthenticationReq_ResourceType
}

func (m *AuthenticationReq) GetResourceName() string {
	if m != nil && m.ResourceName != nil {
		return *m.ResourceName
	}
	return ""
}

type Response struct {
	Status           *int32  `protobuf:"varint,1,req,name=status" json:"status,omitempty"`
	Cause            *string `protobuf:"bytes,2,opt,name=cause" json:"cause,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *Response) Reset()         { *m = Response{} }
func (m *Response) String() string { return proto.CompactTextString(m) }
func (*Response) ProtoMessage()    {}

func (m *Response) GetStatus() int32 {
	if m != nil && m.Status != nil {
		return *m.Status
	}
	return 0
}

func (m *Response) GetCause() string {
	if m != nil && m.Cause != nil {
		return *m.Cause
	}
	return ""
}

type AuthenticationResp struct {
	Resp             *Response `protobuf:"bytes,1,req,name=resp" json:"resp,omitempty"`
	FullJid          *Jid      `protobuf:"bytes,3,opt,name=fullJid" json:"fullJid,omitempty"`
	XXX_unrecognized []byte    `json:"-"`
}

func (m *AuthenticationResp) Reset()         { *m = AuthenticationResp{} }
func (m *AuthenticationResp) String() string { return proto.CompactTextString(m) }
func (*AuthenticationResp) ProtoMessage()    {}

func (m *AuthenticationResp) GetResp() *Response {
	if m != nil {
		return m.Resp
	}
	return nil
}

func (m *AuthenticationResp) GetFullJid() *Jid {
	if m != nil {
		return m.FullJid
	}
	return nil
}

type Heartbeat struct {
	XXX_unrecognized []byte `json:"-"`
}

func (m *Heartbeat) Reset()         { *m = Heartbeat{} }
func (m *Heartbeat) String() string { return proto.CompactTextString(m) }
func (*Heartbeat) ProtoMessage()    {}

type HeartbeatAck struct {
	FromJid          *Jid   `protobuf:"bytes,1,req,name=fromJid" json:"fromJid,omitempty"`
	XXX_unrecognized []byte `json:"-"`
}

func (m *HeartbeatAck) Reset()         { *m = HeartbeatAck{} }
func (m *HeartbeatAck) String() string { return proto.CompactTextString(m) }
func (*HeartbeatAck) ProtoMessage()    {}

func (m *HeartbeatAck) GetFromJid() *Jid {
	if m != nil {
		return m.FromJid
	}
	return nil
}

func init() {
	proto.RegisterEnum("core.AuthenticationReq_ResourceType", AuthenticationReq_ResourceType_name, AuthenticationReq_ResourceType_value)
}