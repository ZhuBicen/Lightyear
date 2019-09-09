package controllers

import (
	"github.com/astaxie/beego"
	"lightyear/db"
)

type MessagesController struct {
	beego.Controller
}

func (this *MessagesController) Get() {
	this.Data["Website"] = "beego.me"
	this.Data["Email"] = "astaxie@gmail.com"
	this.Data["messages"] = db.LoadAllChatMessages(this.GetString("id1"), this.GetString("id2"))
	this.TplName = "messages.tpl"
}
