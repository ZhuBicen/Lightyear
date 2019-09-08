package db

import (
	"database/sql"
	"fmt"
	_ "github.com/Go-SQL-Driver/MySQL"
	"html/template"
	"log"
	"time"
)

func ChatMessageDataSourceName() string {
	return fmt.Sprintf("root:xiaoqiang@tcp(%s:3306)/%s", "125.76.249.13", "lightyear")
}

// serilize the im.ChatMessage and save to DB
// timestamp is the current server time.
func SaveChatMessage(fromId string, toId string, message []byte) error {
	db, err := sql.Open("mysql", ChatMessageDataSourceName())
	if err != nil {
		log.Println("Database connection failure:", err)
		return err
	}
	defer db.Close()

	_, err = db.Exec("INSERT INTO chatmessage (fromId, toId, message, chattime) VALUES(?, ?, ?, ?)",
		fromId, toId, message, time.Now().Format("2006-01-02 15:04:05"))
	return err
}

func LoadChatMessage(fromTime time.Time, toTime time.Time, id1 string, id2 string) {
	log.Println("Query chat message from:", fromTime, " to:", toTime)
	db, err := sql.Open("mysql", ChatMessageDataSourceName())
	if err != nil {
		log.Println("Database connection failure:", err)
		return
	}
	defer db.Close()
	// SELECT fromId, toId, chattime, message FROM chatmessage WHERE (chattime BETWEEN '2000-1-2 1:2:3' AND '2014-6-1 21:41:00') AND (( fromId = "1" and toId = "2"))
	rows, err := db.Query(`SELECT fromId, toId, chattime, message FROM chatmessage WHERE (chattime BETWEEN ? AND ?) AND ((fromId = ? AND toId = ?) OR (toId = ? AND fromId = ?))`,
		fromTime.Format("2006-01-02 15:04:05"), toTime.Format("2006-01-02 15:04:05"),
		id1, id2, id2, id1)

	if err != nil {
		log.Fatal(err)

		return
	}
	log.Println("Query Result:")
	for rows.Next() {
		var fromId, toId string
		// var chatTime time.Time
		chatTime := []byte{}
		message := []byte{}
		if err := rows.Scan(&fromId, &toId, &chatTime, &message); err != nil {
			log.Fatal(err)
		}
		log.Println(fromId, "=>", toId, string(message), string(chatTime))
	}
	if err := rows.Err(); err != nil {
		log.Fatal(err)
	}
}

func LoadAllChatMessages(id1 string, id2 string) []template.HTML {
	messages := []template.HTML{}
	db, err := sql.Open("mysql", ChatMessageDataSourceName())
	if err != nil {
		log.Println("Database connection failure:", err)
		return messages
	}
	defer db.Close()
	// SELECT fromId, toId, chattime, message FROM chatmessage WHERE (chattime BETWEEN '2000-1-2 1:2:3' AND '2014-6-1 21:41:00') AND (( fromId = "1" and toId = "2"))
	rows, err := db.Query(`SELECT fromId, toId, chattime, message FROM chatmessage WHERE ((fromId = ? AND toId = ?) OR (toId = ? AND fromId = ?)) ORDER BY chattime ASC`,
		id1, id2, id1, id2)

	if err != nil {
		log.Println("db.Query error=", err)
		return messages
	}

	log.Println("Query Result:")
	for rows.Next() {
		var fromId, toId string
		// var chatTime time.Time
		chatTime := []byte{}
		message := []byte{}
		if err := rows.Scan(&fromId, &toId, &chatTime, &message); err != nil {
			log.Println("rows.Scan error=", err)
		}
		log.Println(fromId, "=>", toId, string(message), string(chatTime))
		messages = append(messages, template.HTML(string(message)))
	}
	if err := rows.Err(); err != nil {
		log.Println("rows error=", err)
	}
	return messages
}
