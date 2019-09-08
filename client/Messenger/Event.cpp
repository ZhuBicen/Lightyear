#include "Event.hpp"
#include <google/protobuf/message_lite.h>
Event::Event(MessageType type, google::protobuf::MessageLite* data):
type_(type), data_(data){
}
MessageType Event::Type() const { 
    return type_;
}
google::protobuf::MessageLite* Event::Data(){ 
    return data_;
}
Event::~Event() {
    if (data_) {
        delete data_;
    }
}