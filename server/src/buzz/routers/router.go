package routers

import (
	"buzz/controllers"
	"github.com/astaxie/beego"
)

func init() {
	beego.Router("/", &controllers.MainController{})
	beego.Router("/messages", &controllers.MessagesController{})
}
