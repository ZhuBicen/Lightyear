// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: p2p.proto

#include "p2p.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_core_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Jid_core_2eproto;
namespace p2p {
class JingleMessageDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<JingleMessage> _instance;
} _JingleMessage_default_instance_;
}  // namespace p2p
static void InitDefaultsscc_info_JingleMessage_p2p_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::p2p::_JingleMessage_default_instance_;
    new (ptr) ::p2p::JingleMessage();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::p2p::JingleMessage::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_JingleMessage_p2p_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_JingleMessage_p2p_2eproto}, {
      &scc_info_Jid_core_2eproto.base,}};

namespace p2p {

// ===================================================================

void JingleMessage::InitAsDefaultInstance() {
  ::p2p::_JingleMessage_default_instance_._instance.get_mutable()->fromjid_ = const_cast< ::core::Jid*>(
      ::core::Jid::internal_default_instance());
  ::p2p::_JingleMessage_default_instance_._instance.get_mutable()->tojid_ = const_cast< ::core::Jid*>(
      ::core::Jid::internal_default_instance());
}
class JingleMessage::_Internal {
 public:
  using HasBits = decltype(std::declval<JingleMessage>()._has_bits_);
  static const ::core::Jid& fromjid(const JingleMessage* msg);
  static void set_has_fromjid(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static const ::core::Jid& tojid(const JingleMessage* msg);
  static void set_has_tojid(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_content(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

const ::core::Jid&
JingleMessage::_Internal::fromjid(const JingleMessage* msg) {
  return *msg->fromjid_;
}
const ::core::Jid&
JingleMessage::_Internal::tojid(const JingleMessage* msg) {
  return *msg->tojid_;
}
void JingleMessage::clear_fromjid() {
  if (fromjid_ != nullptr) fromjid_->Clear();
  _has_bits_[0] &= ~0x00000002u;
}
void JingleMessage::clear_tojid() {
  if (tojid_ != nullptr) tojid_->Clear();
  _has_bits_[0] &= ~0x00000004u;
}
JingleMessage::JingleMessage()
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:p2p.JingleMessage)
}
JingleMessage::JingleMessage(const JingleMessage& from)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  content_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_content()) {
    content_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.content_);
  }
  if (from._internal_has_fromjid()) {
    fromjid_ = new ::core::Jid(*from.fromjid_);
  } else {
    fromjid_ = nullptr;
  }
  if (from._internal_has_tojid()) {
    tojid_ = new ::core::Jid(*from.tojid_);
  } else {
    tojid_ = nullptr;
  }
  // @@protoc_insertion_point(copy_constructor:p2p.JingleMessage)
}

void JingleMessage::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_JingleMessage_p2p_2eproto.base);
  content_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&fromjid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&tojid_) -
      reinterpret_cast<char*>(&fromjid_)) + sizeof(tojid_));
}

JingleMessage::~JingleMessage() {
  // @@protoc_insertion_point(destructor:p2p.JingleMessage)
  SharedDtor();
}

void JingleMessage::SharedDtor() {
  content_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete fromjid_;
  if (this != internal_default_instance()) delete tojid_;
}

void JingleMessage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const JingleMessage& JingleMessage::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_JingleMessage_p2p_2eproto.base);
  return *internal_default_instance();
}


void JingleMessage::Clear() {
// @@protoc_insertion_point(message_clear_start:p2p.JingleMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      content_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      GOOGLE_DCHECK(fromjid_ != nullptr);
      fromjid_->Clear();
    }
    if (cached_has_bits & 0x00000004u) {
      GOOGLE_DCHECK(tojid_ != nullptr);
      tojid_->Clear();
    }
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* JingleMessage::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required .core.Jid fromJid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_fromjid(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required .core.Jid toJid = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_tojid(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string content = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(_internal_mutable_content(), ptr, ctx);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* JingleMessage::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:p2p.JingleMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required .core.Jid fromJid = 1;
  if (cached_has_bits & 0x00000002u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessageToArray(
        1, _Internal::fromjid(this), target, stream);
  }

  // required .core.Jid toJid = 2;
  if (cached_has_bits & 0x00000004u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessageToArray(
        2, _Internal::tojid(this), target, stream);
  }

  // required string content = 3;
  if (cached_has_bits & 0x00000001u) {
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_content(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = stream->WriteRaw(_internal_metadata_.unknown_fields().data(),
        static_cast<int>(_internal_metadata_.unknown_fields().size()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:p2p.JingleMessage)
  return target;
}

size_t JingleMessage::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:p2p.JingleMessage)
  size_t total_size = 0;

  if (has_content()) {
    // required string content = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_content());
  }

  if (has_fromjid()) {
    // required .core.Jid fromJid = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *fromjid_);
  }

  if (has_tojid()) {
    // required .core.Jid toJid = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *tojid_);
  }

  return total_size;
}
size_t JingleMessage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:p2p.JingleMessage)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x00000007) ^ 0x00000007) == 0) {  // All required fields are present.
    // required string content = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_content());

    // required .core.Jid fromJid = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *fromjid_);

    // required .core.Jid toJid = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *tojid_);

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    total_size += _internal_metadata_.unknown_fields().size();
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void JingleMessage::CheckTypeAndMergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::MessageLite& from) {
  MergeFrom(*::PROTOBUF_NAMESPACE_ID::internal::DownCast<const JingleMessage*>(
      &from));
}

void JingleMessage::MergeFrom(const JingleMessage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:p2p.JingleMessage)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      content_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.content_);
    }
    if (cached_has_bits & 0x00000002u) {
      _internal_mutable_fromjid()->::core::Jid::MergeFrom(from._internal_fromjid());
    }
    if (cached_has_bits & 0x00000004u) {
      _internal_mutable_tojid()->::core::Jid::MergeFrom(from._internal_tojid());
    }
  }
}

void JingleMessage::CopyFrom(const JingleMessage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:p2p.JingleMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool JingleMessage::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;
  if (has_fromjid()) {
    if (!this->fromjid_->IsInitialized()) return false;
  }
  if (has_tojid()) {
    if (!this->tojid_->IsInitialized()) return false;
  }
  return true;
}

void JingleMessage::InternalSwap(JingleMessage* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  content_.Swap(&other->content_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(fromjid_, other->fromjid_);
  swap(tojid_, other->tojid_);
}

std::string JingleMessage::GetTypeName() const {
  return "p2p.JingleMessage";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace p2p
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::p2p::JingleMessage* Arena::CreateMaybeMessage< ::p2p::JingleMessage >(Arena* arena) {
  return Arena::CreateInternal< ::p2p::JingleMessage >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
