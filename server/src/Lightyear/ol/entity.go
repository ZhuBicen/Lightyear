package ol

import (
	"code.google.com/p/goprotobuf/proto"
	"lightyear/codec"
	"lightyear/db"
	"lightyear/proto/core"
	"lightyear/proto/im"
	"lightyear/proto/p2p"
	"log"
	"net"
)

// An entity represents a User Equipment
type Entity struct {
	MessageFromPeerChan chan codec.Message
	MessageToPeerChan   chan codec.ToMessage
	EventChan           chan codec.Event
	Address             string
	Jid                 core.Jid
	OnlineUsers         *Users // point to all online users
}

func NewEntity(conn net.Conn, jid core.Jid, users *Users) Entity {
	return Entity{
		MessageFromPeerChan: make(chan codec.Message, 128),
		MessageToPeerChan:   make(chan codec.ToMessage, 128),
		EventChan:           make(chan codec.Event, 128),
		Address:             conn.RemoteAddr().String(),
		Jid:                 jid, // this is full Jid, resource binded when authentication
		OnlineUsers:         users,
	}
}
func (this Entity) OnPresence(ipresence *im.Presence) error {
	this.OnlineUsers.BroadcastPresence(this, ipresence)
	return nil
}

func (this Entity) OnChatMessage(chatMessage *im.ChatMessage) error {
	this.OnlineUsers.DistributeChatMessage(this, chatMessage)
	return nil
}

func (this Entity) OnJingleMessage(jingleMessage *p2p.JingleMessage) error {
	this.OnlineUsers.ForwardJingleMessage(this, jingleMessage)
	return nil
}
func (this Entity) OnImRosterReq(irr *im.RosterReq) error {
	userId := irr.GetJid().GetId()
	// domain := irr.GetJid().GetDomain()
	// resource := irr.GetJid().GetResource()

	resp := &im.RosterResp{
		RosterItems: this.GetRoster(userId),
	}
	this.MessageToPeerChan <- codec.ToMessage{"im.RosterResp", resp}
	return nil
}
func (this Entity) GetRoster(id string) []*im.RosterItem {

	roster := make([]*im.RosterItem, 0)

	for _, user := range db.Users() {

		var onlineStatus int32
		if this.OnlineUsers.IsUserOnline(user.Id) {
			onlineStatus = 1
		}

		ri := &im.RosterItem{
			Jid: &core.Jid{
				Id:     proto.String(user.Id),
				Domain: proto.String("wanwei"),
				// Resource should be empty here
			},
			OnlineStatus: &onlineStatus,
		}
		if user.Name != "" {
			ri.Name = proto.String(user.Name)
		}
		if user.Position != "" {
			ri.Position = proto.String(user.Position)
		}
		if user.Department != "" {
			ri.Department = proto.String(user.Department)
		}
		roster = append(roster, ri)
	}
	return roster
}
func (this Entity) ProcessMessage(message codec.Message) error {
	buff := message.Buffer
	switch message.Name {
	case "im.RosterReq":
		irr := new(im.RosterReq)
		if err := proto.Unmarshal(buff, irr); err != nil {
			return err
		}
		return this.OnImRosterReq(irr)
	case "im.Presence":
		ipresence := new(im.Presence)
		if err := proto.Unmarshal(buff, ipresence); err != nil {
			return err
		}
		return this.OnPresence(ipresence)

	case "im.ChatMessage":
		chatMessage := new(im.ChatMessage)
		if err := proto.Unmarshal(buff, chatMessage); err != nil {
			return err
		}
		return this.OnChatMessage(chatMessage)
	case "p2p.JingleMessage":
		jingleMessage := new(p2p.JingleMessage)
		if err := proto.Unmarshal(buff, jingleMessage); err != nil {
			return err
		}
		return this.OnJingleMessage(jingleMessage)
	case "core.HeartbeatAck":
		// log.Println("HeartbeatAck received from:", this.Address)
		return nil
	default:
		log.Println("Unrecoganized message name: " + message.Name)
		return nil
	}
}
