// Code generated by protoc-gen-go. DO NOT EDIT.
// source: core.proto

package core

import (
	fmt "fmt"
	proto "github.com/golang/protobuf/proto"
	math "math"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion3 // please upgrade the proto package

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

func (AuthenticationReq_ResourceType) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor_f7e43720d1edc0fe, []int{1, 0}
}

// http://xmpp.org/rfcs/rfc6120.html
type Jid struct {
	Id                   *string  `protobuf:"bytes,1,req,name=id" json:"id,omitempty"`
	Domain               *string  `protobuf:"bytes,2,opt,name=domain" json:"domain,omitempty"`
	Resource             *string  `protobuf:"bytes,3,opt,name=resource" json:"resource,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *Jid) Reset()         { *m = Jid{} }
func (m *Jid) String() string { return proto.CompactTextString(m) }
func (*Jid) ProtoMessage()    {}
func (*Jid) Descriptor() ([]byte, []int) {
	return fileDescriptor_f7e43720d1edc0fe, []int{0}
}

func (m *Jid) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_Jid.Unmarshal(m, b)
}
func (m *Jid) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_Jid.Marshal(b, m, deterministic)
}
func (m *Jid) XXX_Merge(src proto.Message) {
	xxx_messageInfo_Jid.Merge(m, src)
}
func (m *Jid) XXX_Size() int {
	return xxx_messageInfo_Jid.Size(m)
}
func (m *Jid) XXX_DiscardUnknown() {
	xxx_messageInfo_Jid.DiscardUnknown(m)
}

var xxx_messageInfo_Jid proto.InternalMessageInfo

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
	Jid                  *Jid                            `protobuf:"bytes,1,req,name=jid" json:"jid,omitempty"`
	Password             *string                         `protobuf:"bytes,2,opt,name=password" json:"password,omitempty"`
	ResourceType         *AuthenticationReq_ResourceType `protobuf:"varint,3,opt,name=resourceType,enum=core.AuthenticationReq_ResourceType,def=0" json:"resourceType,omitempty"`
	ResourceName         *string                         `protobuf:"bytes,4,opt,name=resourceName" json:"resourceName,omitempty"`
	XXX_NoUnkeyedLiteral struct{}                        `json:"-"`
	XXX_unrecognized     []byte                          `json:"-"`
	XXX_sizecache        int32                           `json:"-"`
}

func (m *AuthenticationReq) Reset()         { *m = AuthenticationReq{} }
func (m *AuthenticationReq) String() string { return proto.CompactTextString(m) }
func (*AuthenticationReq) ProtoMessage()    {}
func (*AuthenticationReq) Descriptor() ([]byte, []int) {
	return fileDescriptor_f7e43720d1edc0fe, []int{1}
}

func (m *AuthenticationReq) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_AuthenticationReq.Unmarshal(m, b)
}
func (m *AuthenticationReq) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_AuthenticationReq.Marshal(b, m, deterministic)
}
func (m *AuthenticationReq) XXX_Merge(src proto.Message) {
	xxx_messageInfo_AuthenticationReq.Merge(m, src)
}
func (m *AuthenticationReq) XXX_Size() int {
	return xxx_messageInfo_AuthenticationReq.Size(m)
}
func (m *AuthenticationReq) XXX_DiscardUnknown() {
	xxx_messageInfo_AuthenticationReq.DiscardUnknown(m)
}

var xxx_messageInfo_AuthenticationReq proto.InternalMessageInfo

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
	Status               *int32   `protobuf:"varint,1,req,name=status" json:"status,omitempty"`
	Cause                *string  `protobuf:"bytes,2,opt,name=cause" json:"cause,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *Response) Reset()         { *m = Response{} }
func (m *Response) String() string { return proto.CompactTextString(m) }
func (*Response) ProtoMessage()    {}
func (*Response) Descriptor() ([]byte, []int) {
	return fileDescriptor_f7e43720d1edc0fe, []int{2}
}

func (m *Response) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_Response.Unmarshal(m, b)
}
func (m *Response) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_Response.Marshal(b, m, deterministic)
}
func (m *Response) XXX_Merge(src proto.Message) {
	xxx_messageInfo_Response.Merge(m, src)
}
func (m *Response) XXX_Size() int {
	return xxx_messageInfo_Response.Size(m)
}
func (m *Response) XXX_DiscardUnknown() {
	xxx_messageInfo_Response.DiscardUnknown(m)
}

var xxx_messageInfo_Response proto.InternalMessageInfo

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
	Resp                 *Response `protobuf:"bytes,1,req,name=resp" json:"resp,omitempty"`
	FullJid              *Jid      `protobuf:"bytes,3,opt,name=fullJid" json:"fullJid,omitempty"`
	XXX_NoUnkeyedLiteral struct{}  `json:"-"`
	XXX_unrecognized     []byte    `json:"-"`
	XXX_sizecache        int32     `json:"-"`
}

func (m *AuthenticationResp) Reset()         { *m = AuthenticationResp{} }
func (m *AuthenticationResp) String() string { return proto.CompactTextString(m) }
func (*AuthenticationResp) ProtoMessage()    {}
func (*AuthenticationResp) Descriptor() ([]byte, []int) {
	return fileDescriptor_f7e43720d1edc0fe, []int{3}
}

func (m *AuthenticationResp) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_AuthenticationResp.Unmarshal(m, b)
}
func (m *AuthenticationResp) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_AuthenticationResp.Marshal(b, m, deterministic)
}
func (m *AuthenticationResp) XXX_Merge(src proto.Message) {
	xxx_messageInfo_AuthenticationResp.Merge(m, src)
}
func (m *AuthenticationResp) XXX_Size() int {
	return xxx_messageInfo_AuthenticationResp.Size(m)
}
func (m *AuthenticationResp) XXX_DiscardUnknown() {
	xxx_messageInfo_AuthenticationResp.DiscardUnknown(m)
}

var xxx_messageInfo_AuthenticationResp proto.InternalMessageInfo

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
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *Heartbeat) Reset()         { *m = Heartbeat{} }
func (m *Heartbeat) String() string { return proto.CompactTextString(m) }
func (*Heartbeat) ProtoMessage()    {}
func (*Heartbeat) Descriptor() ([]byte, []int) {
	return fileDescriptor_f7e43720d1edc0fe, []int{4}
}

func (m *Heartbeat) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_Heartbeat.Unmarshal(m, b)
}
func (m *Heartbeat) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_Heartbeat.Marshal(b, m, deterministic)
}
func (m *Heartbeat) XXX_Merge(src proto.Message) {
	xxx_messageInfo_Heartbeat.Merge(m, src)
}
func (m *Heartbeat) XXX_Size() int {
	return xxx_messageInfo_Heartbeat.Size(m)
}
func (m *Heartbeat) XXX_DiscardUnknown() {
	xxx_messageInfo_Heartbeat.DiscardUnknown(m)
}

var xxx_messageInfo_Heartbeat proto.InternalMessageInfo

type HeartbeatAck struct {
	FromJid              *Jid     `protobuf:"bytes,1,req,name=fromJid" json:"fromJid,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *HeartbeatAck) Reset()         { *m = HeartbeatAck{} }
func (m *HeartbeatAck) String() string { return proto.CompactTextString(m) }
func (*HeartbeatAck) ProtoMessage()    {}
func (*HeartbeatAck) Descriptor() ([]byte, []int) {
	return fileDescriptor_f7e43720d1edc0fe, []int{5}
}

func (m *HeartbeatAck) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_HeartbeatAck.Unmarshal(m, b)
}
func (m *HeartbeatAck) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_HeartbeatAck.Marshal(b, m, deterministic)
}
func (m *HeartbeatAck) XXX_Merge(src proto.Message) {
	xxx_messageInfo_HeartbeatAck.Merge(m, src)
}
func (m *HeartbeatAck) XXX_Size() int {
	return xxx_messageInfo_HeartbeatAck.Size(m)
}
func (m *HeartbeatAck) XXX_DiscardUnknown() {
	xxx_messageInfo_HeartbeatAck.DiscardUnknown(m)
}

var xxx_messageInfo_HeartbeatAck proto.InternalMessageInfo

func (m *HeartbeatAck) GetFromJid() *Jid {
	if m != nil {
		return m.FromJid
	}
	return nil
}

func init() {
	proto.RegisterEnum("core.AuthenticationReq_ResourceType", AuthenticationReq_ResourceType_name, AuthenticationReq_ResourceType_value)
	proto.RegisterType((*Jid)(nil), "core.Jid")
	proto.RegisterType((*AuthenticationReq)(nil), "core.AuthenticationReq")
	proto.RegisterType((*Response)(nil), "core.Response")
	proto.RegisterType((*AuthenticationResp)(nil), "core.AuthenticationResp")
	proto.RegisterType((*Heartbeat)(nil), "core.Heartbeat")
	proto.RegisterType((*HeartbeatAck)(nil), "core.HeartbeatAck")
}

func init() { proto.RegisterFile("core.proto", fileDescriptor_f7e43720d1edc0fe) }

var fileDescriptor_f7e43720d1edc0fe = []byte{
	// 364 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0x6c, 0x92, 0xcf, 0x6b, 0xdb, 0x30,
	0x1c, 0xc5, 0xe7, 0x1f, 0xf9, 0xe1, 0x6f, 0x42, 0xf0, 0x44, 0x18, 0x66, 0xbb, 0x04, 0x6d, 0x87,
	0x9c, 0x12, 0xc8, 0x2e, 0x63, 0xec, 0xe2, 0x2d, 0x83, 0xd8, 0x07, 0x27, 0x88, 0x42, 0xa1, 0xd0,
	0x83, 0x6a, 0xab, 0x8d, 0xda, 0xc4, 0x72, 0x25, 0x99, 0x92, 0xbf, 0xbe, 0xc5, 0xb2, 0x9d, 0x26,
	0x6d, 0x6f, 0x7e, 0x7a, 0xe2, 0xf3, 0xfc, 0x9e, 0x0d, 0x90, 0x0a, 0xc9, 0x66, 0x85, 0x14, 0x5a,
	0x20, 0xb7, 0x7a, 0xc6, 0x11, 0x38, 0x31, 0xcf, 0xd0, 0x08, 0x6c, 0x9e, 0x05, 0xd6, 0xc4, 0x9e,
	0x7a, 0xc4, 0xe6, 0x19, 0xfa, 0x02, 0xdd, 0x4c, 0xec, 0x29, 0xcf, 0x03, 0x7b, 0x62, 0x4d, 0x3d,
	0xd2, 0x28, 0xf4, 0x15, 0xfa, 0x92, 0x29, 0x51, 0xca, 0x94, 0x05, 0x8e, 0x71, 0x8e, 0x1a, 0x3f,
	0x5b, 0xf0, 0x39, 0x2c, 0xf5, 0x96, 0xe5, 0x9a, 0xa7, 0x54, 0x73, 0x91, 0x13, 0xf6, 0x88, 0xbe,
	0x81, 0x73, 0xdf, 0xa0, 0x07, 0x0b, 0x6f, 0x66, 0x5e, 0x20, 0xe6, 0x19, 0xa9, 0x4e, 0x2b, 0x5c,
	0x41, 0x95, 0x7a, 0x12, 0x32, 0x6b, 0x82, 0x8e, 0x1a, 0xad, 0x61, 0xd8, 0xa2, 0x2f, 0x0e, 0x45,
	0x1d, 0x37, 0x5a, 0xfc, 0xa8, 0x09, 0xef, 0x72, 0x66, 0xe4, 0xe4, 0xee, 0xef, 0xce, 0x65, 0x94,
	0x6c, 0xfe, 0x91, 0x33, 0x00, 0xc2, 0xaf, 0xc0, 0x84, 0xee, 0x59, 0xe0, 0x9a, 0xc0, 0xb3, 0x33,
	0xfc, 0x07, 0x86, 0xa7, 0x20, 0xe4, 0x41, 0x8d, 0xf2, 0x3f, 0xa1, 0x01, 0xf4, 0xc2, 0x64, 0x49,
	0xd6, 0xd1, 0xd2, 0xb7, 0x10, 0x40, 0x37, 0xda, 0xac, 0xd6, 0xc9, 0x7f, 0xdf, 0x46, 0x7d, 0x70,
	0xa3, 0x4d, 0xb8, 0xf4, 0x1d, 0xfc, 0x0b, 0xfa, 0x84, 0xa9, 0x42, 0xe4, 0x8a, 0x55, 0x0b, 0x2a,
	0x4d, 0x75, 0xa9, 0x4c, 0xf5, 0x0e, 0x69, 0x14, 0x1a, 0x43, 0x27, 0xa5, 0xa5, 0x62, 0x4d, 0xdf,
	0x5a, 0xe0, 0x6b, 0x40, 0x6f, 0x2b, 0xa9, 0x02, 0x61, 0x70, 0x25, 0x53, 0x45, 0x33, 0xde, 0xa8,
	0xae, 0xde, 0x26, 0x10, 0xe3, 0xa1, 0xef, 0xd0, 0xbb, 0x2d, 0x77, 0xbb, 0x98, 0x67, 0x66, 0xa1,
	0xb3, 0x8d, 0x5b, 0x07, 0x0f, 0xc0, 0x5b, 0x31, 0x2a, 0xf5, 0x0d, 0xa3, 0x1a, 0xff, 0x84, 0xe1,
	0x51, 0x84, 0xe9, 0x83, 0x21, 0x48, 0xb1, 0x8f, 0x3f, 0xfa, 0x4a, 0xad, 0xf3, 0x37, 0x58, 0x39,
	0x57, 0xe3, 0x1d, 0xbf, 0xdb, 0xea, 0x03, 0xa3, 0x72, 0x6e, 0x7e, 0xa1, 0x79, 0x75, 0xed, 0x25,
	0x00, 0x00, 0xff, 0xff, 0x05, 0xe0, 0xf5, 0x1b, 0x54, 0x02, 0x00, 0x00,
}
