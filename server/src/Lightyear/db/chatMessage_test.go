package db

import (
	"testing"
	"time"
)

func Test_SaveChatMessage(t *testing.T) {
	if err := SaveChatMessage("1", "2", []byte("1->2")); err != nil {
		t.Fatal(err)
	}
	//                           Layout                Value
	fromTime, err := time.Parse("2006-01-02 15:04:05", "2000-06-01 21:40:00")
	if err != nil {
		t.Fatal(err)
	}
	toTime, err := time.Parse("2006-01-02 15:04:05", "2015-06-01 21:40:00")
	if err != nil {
		t.Fatal(err)
	}
	LoadChatMessage(fromTime, toTime, "1", "2")
}
