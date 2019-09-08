#pragma once
#include "UiMessage.hpp"
#include <memory>
namespace google {
    namespace protobuf {
        class MessageLite;
    }
}
class Event {
public:
    Event(MessageType type, google::protobuf::MessageLite* data = nullptr);
    MessageType Type() const;
    google::protobuf::MessageLite* Data();
    ~Event();
		
private:
    MessageType type_;
    google::protobuf::MessageLite* data_;
 };

typedef std::shared_ptr<Event> EventPtr;
