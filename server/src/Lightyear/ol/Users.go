package ol

import (
	"fmt"
	"io/ioutil"
	"lightyear/codec"
	"lightyear/db"
	"lightyear/proto/core"
	"lightyear/proto/im"
	"lightyear/proto/p2p"
	"log"
	"math/rand"
	"net"
	"sync"
	"time"
)

type Users struct {
	Entities map[string][]Entity // 一个ID对应多个可以登录的设备，一个登录的设备是一个entity
	mutex    sync.RWMutex
}

func (this *Users) AllocateResource(bareJid core.Jid) string {
	// 该用户登录的所有设备
	// entities := this.Entities[bareJid.GetId()]
	rand.New(rand.NewSource(time.Now().UnixNano()))
	return fmt.Sprintf("%s_%d", bareJid.GetResource(), rand.Int31())
}

// after authenticate success
func (this *Users) Connect(conn net.Conn, jid core.Jid) Entity {
	this.mutex.Lock()
	// defer this.mutex.Unlock()
	if _, existing := this.Entities[jid.GetId()]; !existing {
		this.Entities[jid.GetId()] = []Entity{}
	}
	entity := NewEntity(conn, jid, this)
	this.Entities[jid.GetId()] = append(this.Entities[jid.GetId()], entity)
	this.mutex.Unlock()
	return entity
}

// after client close the connection
func (this *Users) Disconnect(e Entity) bool {

	var jid core.Jid

	this.mutex.Lock()
	for userId, entities := range this.Entities {
		for index, entity := range entities {
			if entity.Address == e.Address {
				jid = entity.Jid
				log.Println(jid.GetId(), " is going to disconnect.")
				newEntities := append(entities[:index], entities[index+1:]...)
				this.Entities[userId] = newEntities
				break
			}
		}
	}
	this.mutex.Unlock()

	// other device still there
	if this.IsUserOnline(jid.GetId()) {
		return true
	}

	var presence_show im.Presence_Show
	presence_show = im.Presence_OFFLINE
	presence := &im.Presence{
		FromJid: &jid,
		Show:    &presence_show,
	}
	this.BroadcastPresence(e, presence)
	return true
}

func generateReadableChatMessage(message *im.ChatMessage) string {
	// this is strange, timestamp here.
	fromId := message.GetFromJid().GetId()
	html := fmt.Sprintf(`<h2><span class="fromId">%s</span> <span class="timestamp">%s</span></h2>`, fromId, time.Now().Format("2006-01-02 15:04:05"))
	for _, me := range message.Body {
		switch me.GetKind() {
		case im.EnumMessageElement_TEXT:
			html += string(me.Data)
		case im.EnumMessageElement_SMILEY:
			html += fmt.Sprintf(`<img src="emotion/%s" />`, string(me.Data))
		case im.EnumMessageElement_BITMAP:
			// save the bitmap, unique file name
			fileName := fmt.Sprintf("screenshot/%d_%d.bmp", time.Now().UnixNano(), rand.Int63())
			ioutil.WriteFile(fileName, me.Data, 0644)
			html += fmt.Sprintf(`<img src="%s" />`, fileName)
		}
	}
	return html
}

func (this *Users) DistributeChatMessage(fromEntity Entity, message *im.ChatMessage) {
	if !this.IsUserOnline(*message.ToJid.Id) {
		log.Println("Trying to send message to user not online:", *message.ToJid.Id)
		return
	}

	// suppose the toJid is online

	// TODO: redudant info save to the ChatMessage database, only messageElement is needed
	//       What actully needed is proto.Marshal(message.GetBody()) but []MessageElement is not a proto.message
	// messageContent, _ := proto.Marshal(message)
	db.SaveChatMessage(message.GetFromJid().GetId(), message.GetToJid().GetId(), []byte(generateReadableChatMessage(message)))

	this.mutex.RLock()
	defer this.mutex.RUnlock()

	// 首先分发到该用户登录的所有设备，比如用户登录了手机端和PC端，则同时分发到两个
	// send to all binded resources，  range on nil ?
	for _, entity := range this.Entities[*message.ToJid.Id] {
		entity.MessageToPeerChan <- codec.ToMessage{"im.ChatMessage", message}
	}

	// send to other binded resources
	for _, entity := range this.Entities[*message.FromJid.Id] {
		if entity.Address != fromEntity.Address {
			entity.MessageToPeerChan <- codec.ToMessage{"im.ChatMessage", message}
		}
	}
}

func (this *Users) ForwardJingleMessage(fromEntity Entity, message *p2p.JingleMessage) error {
	if !this.IsUserOnline(*message.ToJid.Id) {
		log.Println("Trying to send message to user not online:", *message.ToJid.Id)
		return nil
	}

	// suppose the toJid is online
	// db.SaveChatMessage(message.GetFromJid().GetId(), message.GetToJid().GetId(), message.GetBody())

	this.mutex.RLock()
	defer this.mutex.RUnlock()

	// 建立p2p连接时，只发送到指定的设备，比如PC或移动端，这里为简化只发送到用户的第一个登录设备
	// send to all binded resources，  range on nil ?
	for _, entity := range this.Entities[*message.ToJid.Id] {
		entity.MessageToPeerChan <- codec.ToMessage{"p2p.JingleMessage", message}
		break
	}
	return nil
}

func (this *Users) BroadcastPresence(fromEntity Entity, message *im.Presence) error {

	this.mutex.RLock()
	defer this.mutex.RUnlock()

	for _, entities := range this.Entities {
		for _, entity := range entities {
			if entity.Address != fromEntity.Address {
				entity.MessageToPeerChan <- codec.ToMessage{"im.Presence", message}
			}
		}
	}
	return nil
}

func (this *Users) IsUserOnline(id string) bool {
	this.mutex.RLock()
	defer this.mutex.RUnlock()

	if _, okay := this.Entities[id]; !okay {
		return false
	}
	if len(this.Entities[id]) == 0 {
		return false
	}
	return true
}

func (this *Users) GetOnlineEntities() ([]core.Jid, error) {
	return nil, nil
}
