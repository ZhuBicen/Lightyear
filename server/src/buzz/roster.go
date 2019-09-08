package main

import (
	"code.google.com/p/goprotobuf/proto"
	"lightyear/db"
	"lightyear/proto/core"
	"lightyear/proto/im"
)

func GetRoster(id string) []*im.RosterItem {

	roster := make([]*im.RosterItem, 0)

	for _, user := range db.Users() {

		var onlineStatus int32
		if g_ol.IsUserOnline(user.Id) {
			onlineStatus = 1
		}

		roster = append(roster,
			&im.RosterItem{
				Jid: &core.Jid{
					Id:     proto.String(user.Id),
					Domain: proto.String("wanwei"),
					// Resource should be empty here
				},
				OnlineStatus: &onlineStatus,
				Name:         proto.String(user.Name),
				Position:     proto.String(user.Position),
				Department:   proto.String(user.Department),
			})
	}
	return roster
}
