// Code generated by protoc-gen-go.
// source: im.proto
// DO NOT EDIT!

/*
Package im is a generated protocol buffer package.

It is generated from these files:
	im.proto

It has these top-level messages:
	Presence
	RosterReq
	RosterItem
	RosterResp
	PresenceReq
	PresenceResp
	MessageElement
	ChatMessage
	AlertMessage
*/
package im

import proto "code.google.com/p/goprotobuf/proto"
import json "encoding/json"
import math "math"
import core	"lightyear/proto/core" 

// Reference proto, json, and math imports to suppress error if they are not otherwise used.
var _ = proto.Marshal
var _ = &json.SyntaxError{}
var _ = math.Inf

type EnumMessageElement int32

const (
	EnumMessageElement_TEXT   EnumMessageElement = 0
	EnumMessageElement_SMILEY EnumMessageElement = 1
	EnumMessageElement_BITMAP EnumMessageElement = 2
)

var EnumMessageElement_name = map[int32]string{
	0: "TEXT",
	1: "SMILEY",
	2: "BITMAP",
}
var EnumMessageElement_value = map[string]int32{
	"TEXT":   0,
	"SMILEY": 1,
	"BITMAP": 2,
}

func (x EnumMessageElement) Enum() *EnumMessageElement {
	p := new(EnumMessageElement)
	*p = x
	return p
}
func (x EnumMessageElement) String() string {
	return proto.EnumName(EnumMessageElement_name, int32(x))
}
func (x *EnumMessageElement) UnmarshalJSON(data []byte) error {
	value, err := proto.UnmarshalJSONEnum(EnumMessageElement_value, data, "EnumMessageElement")
	if err != nil {
		return err
	}
	*x = EnumMessageElement(value)
	return nil
}

type Presence_Show int32

const (
	Presence_ONLINE  Presence_Show = 0
	Presence_OFFLINE Presence_Show = 1
	Presence_AWAY    Presence_Show = 2
)

var Presence_Show_name = map[int32]string{
	0: "ONLINE",
	1: "OFFLINE",
	2: "AWAY",
}
var Presence_Show_value = map[string]int32{
	"ONLINE":  0,
	"OFFLINE": 1,
	"AWAY":    2,
}

func (x Presence_Show) Enum() *Presence_Show {
	p := new(Presence_Show)
	*p = x
	return p
}
func (x Presence_Show) String() string {
	return proto.EnumName(Presence_Show_name, int32(x))
}
func (x *Presence_Show) UnmarshalJSON(data []byte) error {
	value, err := proto.UnmarshalJSONEnum(Presence_Show_value, data, "Presence_Show")
	if err != nil {
		return err
	}
	*x = Presence_Show(value)
	return nil
}

// presence
type Presence struct {
	FromJid *core.Jid `protobuf:"bytes,1,opt,name=fromJid" json:"fromJid,omitempty"`
	ToJid   *core.Jid `protobuf:"bytes,2,opt,name=toJid" json:"toJid,omitempty"`
	// optional string type = 3;
	Priority         *int32         `protobuf:"varint,4,opt,name=priority" json:"priority,omitempty"`
	Show             *Presence_Show `protobuf:"varint,5,opt,name=show,enum=im.Presence_Show" json:"show,omitempty"`
	Status           *string        `protobuf:"bytes,6,opt,name=status" json:"status,omitempty"`
	XXX_unrecognized []byte         `json:"-"`
}

func (m *Presence) Reset()         { *m = Presence{} }
func (m *Presence) String() string { return proto.CompactTextString(m) }
func (*Presence) ProtoMessage()    {}

func (m *Presence) GetFromJid() *core.Jid {
	if m != nil {
		return m.FromJid
	}
	return nil
}

func (m *Presence) GetToJid() *core.Jid {
	if m != nil {
		return m.ToJid
	}
	return nil
}

func (m *Presence) GetPriority() int32 {
	if m != nil && m.Priority != nil {
		return *m.Priority
	}
	return 0
}

func (m *Presence) GetShow() Presence_Show {
	if m != nil && m.Show != nil {
		return *m.Show
	}
	return Presence_ONLINE
}

func (m *Presence) GetStatus() string {
	if m != nil && m.Status != nil {
		return *m.Status
	}
	return ""
}

// roster get
type RosterReq struct {
	Jid              *core.Jid `protobuf:"bytes,1,req,name=jid" json:"jid,omitempty"`
	XXX_unrecognized []byte    `json:"-"`
}

func (m *RosterReq) Reset()         { *m = RosterReq{} }
func (m *RosterReq) String() string { return proto.CompactTextString(m) }
func (*RosterReq) ProtoMessage()    {}

func (m *RosterReq) GetJid() *core.Jid {
	if m != nil {
		return m.Jid
	}
	return nil
}

type RosterItem struct {
	Jid          *core.Jid `protobuf:"bytes,1,req,name=jid" json:"jid,omitempty"`
	OnlineStatus *int32    `protobuf:"varint,2,req,name=onlineStatus" json:"onlineStatus,omitempty"`
	Name         *string   `protobuf:"bytes,3,opt,name=name" json:"name,omitempty"`
	Nickname     *string   `protobuf:"bytes,4,opt,name=nickname" json:"nickname,omitempty"`
	Description  *string   `protobuf:"bytes,5,opt,name=description" json:"description,omitempty"`
	Position     *string   `protobuf:"bytes,6,opt,name=position" json:"position,omitempty"`
	Department   *string   `protobuf:"bytes,7,opt,name=department" json:"department,omitempty"`
	// optional string subscription = 6;
	GroupTag         []string `protobuf:"bytes,8,rep,name=groupTag" json:"groupTag,omitempty"`
	XXX_unrecognized []byte   `json:"-"`
}

func (m *RosterItem) Reset()         { *m = RosterItem{} }
func (m *RosterItem) String() string { return proto.CompactTextString(m) }
func (*RosterItem) ProtoMessage()    {}

func (m *RosterItem) GetJid() *core.Jid {
	if m != nil {
		return m.Jid
	}
	return nil
}

func (m *RosterItem) GetOnlineStatus() int32 {
	if m != nil && m.OnlineStatus != nil {
		return *m.OnlineStatus
	}
	return 0
}

func (m *RosterItem) GetName() string {
	if m != nil && m.Name != nil {
		return *m.Name
	}
	return ""
}

func (m *RosterItem) GetNickname() string {
	if m != nil && m.Nickname != nil {
		return *m.Nickname
	}
	return ""
}

func (m *RosterItem) GetDescription() string {
	if m != nil && m.Description != nil {
		return *m.Description
	}
	return ""
}

func (m *RosterItem) GetPosition() string {
	if m != nil && m.Position != nil {
		return *m.Position
	}
	return ""
}

func (m *RosterItem) GetDepartment() string {
	if m != nil && m.Department != nil {
		return *m.Department
	}
	return ""
}

func (m *RosterItem) GetGroupTag() []string {
	if m != nil {
		return m.GroupTag
	}
	return nil
}

type RosterResp struct {
	RosterItems      []*RosterItem `protobuf:"bytes,1,rep,name=rosterItems" json:"rosterItems,omitempty"`
	XXX_unrecognized []byte        `json:"-"`
}

func (m *RosterResp) Reset()         { *m = RosterResp{} }
func (m *RosterResp) String() string { return proto.CompactTextString(m) }
func (*RosterResp) ProtoMessage()    {}

func (m *RosterResp) GetRosterItems() []*RosterItem {
	if m != nil {
		return m.RosterItems
	}
	return nil
}

type PresenceReq struct {
	Jid              *core.Jid `protobuf:"bytes,1,req,name=jid" json:"jid,omitempty"`
	XXX_unrecognized []byte    `json:"-"`
}

func (m *PresenceReq) Reset()         { *m = PresenceReq{} }
func (m *PresenceReq) String() string { return proto.CompactTextString(m) }
func (*PresenceReq) ProtoMessage()    {}

func (m *PresenceReq) GetJid() *core.Jid {
	if m != nil {
		return m.Jid
	}
	return nil
}

type PresenceResp struct {
	Jids             []*core.Jid `protobuf:"bytes,1,rep,name=jids" json:"jids,omitempty"`
	XXX_unrecognized []byte      `json:"-"`
}

func (m *PresenceResp) Reset()         { *m = PresenceResp{} }
func (m *PresenceResp) String() string { return proto.CompactTextString(m) }
func (*PresenceResp) ProtoMessage()    {}

func (m *PresenceResp) GetJids() []*core.Jid {
	if m != nil {
		return m.Jids
	}
	return nil
}

type MessageElement struct {
	Kind             *EnumMessageElement `protobuf:"varint,1,req,name=kind,enum=im.EnumMessageElement" json:"kind,omitempty"`
	Data             []byte              `protobuf:"bytes,2,req,name=data" json:"data,omitempty"`
	XXX_unrecognized []byte              `json:"-"`
}

func (m *MessageElement) Reset()         { *m = MessageElement{} }
func (m *MessageElement) String() string { return proto.CompactTextString(m) }
func (*MessageElement) ProtoMessage()    {}

func (m *MessageElement) GetKind() EnumMessageElement {
	if m != nil && m.Kind != nil {
		return *m.Kind
	}
	return EnumMessageElement_TEXT
}

func (m *MessageElement) GetData() []byte {
	if m != nil {
		return m.Data
	}
	return nil
}

// chat message
// 5.2.2 type attribute
// 1. chat
// 2. group chat
// 3. headline
type ChatMessage struct {
	FromJid          *core.Jid         `protobuf:"bytes,1,req,name=fromJid" json:"fromJid,omitempty"`
	ToJid            *core.Jid         `protobuf:"bytes,2,req,name=toJid" json:"toJid,omitempty"`
	Body             []*MessageElement `protobuf:"bytes,3,rep,name=body" json:"body,omitempty"`
	XXX_unrecognized []byte            `json:"-"`
}

func (m *ChatMessage) Reset()         { *m = ChatMessage{} }
func (m *ChatMessage) String() string { return proto.CompactTextString(m) }
func (*ChatMessage) ProtoMessage()    {}

func (m *ChatMessage) GetFromJid() *core.Jid {
	if m != nil {
		return m.FromJid
	}
	return nil
}

func (m *ChatMessage) GetToJid() *core.Jid {
	if m != nil {
		return m.ToJid
	}
	return nil
}

func (m *ChatMessage) GetBody() []*MessageElement {
	if m != nil {
		return m.Body
	}
	return nil
}

// server message push
type AlertMessage struct {
	Body             *string `protobuf:"bytes,3,req,name=body" json:"body,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *AlertMessage) Reset()         { *m = AlertMessage{} }
func (m *AlertMessage) String() string { return proto.CompactTextString(m) }
func (*AlertMessage) ProtoMessage()    {}

func (m *AlertMessage) GetBody() string {
	if m != nil && m.Body != nil {
		return *m.Body
	}
	return ""
}

func init() {
	proto.RegisterEnum("im.EnumMessageElement", EnumMessageElement_name, EnumMessageElement_value)
	proto.RegisterEnum("im.Presence_Show", Presence_Show_name, Presence_Show_value)
}