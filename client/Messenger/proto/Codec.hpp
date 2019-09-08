// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (giantchen at gmail dot com)

#ifndef PROTOBUF_CODEC_H
#define PROTOBUF_CODEC_H
#include <string>
#include <stdint.h>

namespace google {
    namespace protobuf {
        class MessageLite;
    }
}
/// Encode protobuf Message to transport format defined above
/// returns a std::string.
///
/// returns a empty string if message.AppendToString() fails.
///
std::string encode(const google::protobuf::MessageLite& message);
google::protobuf::MessageLite* createMessage(const std::string& type_name);
int32_t asInt32(const char* buf);
///
/// Decode protobuf Message from transport format defined above.
/// returns a Message*
///
/// returns NULL if fails.
///
google::protobuf::MessageLite* decode(const std::string& buf);
#endif  // PROTOBUF_CODEC_H
