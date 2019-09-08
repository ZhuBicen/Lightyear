package db

import (
	"database/sql"
	"fmt"
	_ "github.com/Go-SQL-Driver/MySQL"
	"github.com/golang/glog"
)

func UserDatabaseConnectionString() string {
	return fmt.Sprintf("root:root@tcp(%s:3306)/%s", "127.0.0.1", "dy_enterprise_stock")
}

type User struct {
	Id         string
	Department string
	Password   string
	Name       string
	Telephone  string
	Position   string
}

var g_users []User

// 有一个缺陷，新增加的用户不会被服务器识别到，因为IM服务器只有启动的时候
// 才去读一次数据库。因此这种情况下，只有重启IM服务器，才会识别到新增加的用户。

func init() {
	var err error
	g_users, err = readSsoUsers()
	if err != nil {
		panic(err)
	}
}

func Users() []User {
	return g_users
}

func VerifyPassword(id, pw string) bool {
	for _, user := range g_users {
		if id == user.Id && pw == id {
			return true
		}
	}
	return false
}

// Deprecated
func readUsers() ([]User, error) {
	db, err := sql.Open("mysql", UserDatabaseConnectionString())
	if err != nil {
		glog.Error("Database connection failure:", err)
		return nil, err
	}

	defer db.Close()
	rows, err := db.Query(fmt.Sprintf(`select adminCode, department, password, realName, telephone from xt_admin`))
	if err != nil {
		return nil, err
	}

	users := make([]User, 0)

	for rows.Next() {
		var user User
		err = rows.Scan(&user.Id, &user.Department, &user.Password, &user.Name, &user.Telephone)
		if err != nil {
			return nil, err
		}
		user.Position = "职位" //用户的user数据库中没有职位信息
		users = append(users, user)
	}
	return users, nil
}

func SsoConnectionString() string {
	return fmt.Sprintf("root:password@tcp(%s:3306)/%s", "your.db.ip", "passport")
}
func readSsoUsers() ([]User, error) {
	db, err := sql.Open("mysql", SsoConnectionString())
	if err != nil {
		glog.Error("Database connection failure:", err)
		return nil, err
	}

	defer db.Close()
	rows, err := db.Query(fmt.Sprintf(`select sso_email, sso_username from sso_user`))
	if err != nil {
		return nil, err
	}

	users := make([]User, 0)

	for rows.Next() {
		var user User
		err = rows.Scan(&user.Id, &user.Name)
		if err != nil {
			return nil, err
		}
		user.Position = "职位" //用户的user数据库中没有职位信息
		users = append(users, user)
	}
	return users, nil

}
