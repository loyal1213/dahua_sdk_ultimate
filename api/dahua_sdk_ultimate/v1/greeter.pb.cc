// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: greeter.proto
// Protobuf C++ Version: 5.26.1

#include "greeter.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/generated_message_tctable_impl.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace helloworld {
namespace v1 {

inline constexpr HelloRequest::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : name_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR HelloRequest::HelloRequest(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct HelloRequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR HelloRequestDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~HelloRequestDefaultTypeInternal() {}
  union {
    HelloRequest _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 HelloRequestDefaultTypeInternal _HelloRequest_default_instance_;

inline constexpr HelloReply::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : message_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR HelloReply::HelloReply(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct HelloReplyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR HelloReplyDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~HelloReplyDefaultTypeInternal() {}
  union {
    HelloReply _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 HelloReplyDefaultTypeInternal _HelloReply_default_instance_;
}  // namespace v1
}  // namespace helloworld
static ::_pb::Metadata file_level_metadata_greeter_2eproto[2];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_greeter_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_greeter_2eproto = nullptr;
const ::uint32_t
    TableStruct_greeter_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::helloworld::v1::HelloRequest, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::helloworld::v1::HelloRequest, _impl_.name_),
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::helloworld::v1::HelloReply, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::helloworld::v1::HelloReply, _impl_.message_),
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, -1, -1, sizeof(::helloworld::v1::HelloRequest)},
        {9, -1, -1, sizeof(::helloworld::v1::HelloReply)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::helloworld::v1::_HelloRequest_default_instance_._instance,
    &::helloworld::v1::_HelloReply_default_instance_._instance,
};
const char descriptor_table_protodef_greeter_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\rgreeter.proto\022\rhelloworld.v1\"\034\n\014HelloR"
    "equest\022\014\n\004name\030\001 \001(\t\"\035\n\nHelloReply\022\017\n\007me"
    "ssage\030\001 \001(\t2O\n\007Greeter\022D\n\010SayHello\022\033.hel"
    "loworld.v1.HelloRequest\032\031.helloworld.v1."
    "HelloReply\"\000Bl\n$dev.kratos.api.dahua_sdk"
    "_ultimate.v1B\021HelloworldProtoV1P\001Z/dahua"
    "_sdk_ultimate/api/dahua_sdk_ultimate/v1;"
    "v1b\006proto3"
};
static ::absl::once_flag descriptor_table_greeter_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_greeter_2eproto = {
    false,
    false,
    290,
    descriptor_table_protodef_greeter_2eproto,
    "greeter.proto",
    &descriptor_table_greeter_2eproto_once,
    nullptr,
    0,
    2,
    schemas,
    file_default_instances,
    TableStruct_greeter_2eproto::offsets,
    file_level_metadata_greeter_2eproto,
    file_level_enum_descriptors_greeter_2eproto,
    file_level_service_descriptors_greeter_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_greeter_2eproto_getter() {
  return &descriptor_table_greeter_2eproto;
}
namespace helloworld {
namespace v1 {
// ===================================================================

class HelloRequest::_Internal {
 public:
};

HelloRequest::HelloRequest(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:helloworld.v1.HelloRequest)
}
inline PROTOBUF_NDEBUG_INLINE HelloRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : name_(arena, from.name_),
        _cached_size_{0} {}

HelloRequest::HelloRequest(
    ::google::protobuf::Arena* arena,
    const HelloRequest& from)
    : ::google::protobuf::Message(arena) {
  HelloRequest* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);

  // @@protoc_insertion_point(copy_constructor:helloworld.v1.HelloRequest)
}
inline PROTOBUF_NDEBUG_INLINE HelloRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : name_(arena),
        _cached_size_{0} {}

inline void HelloRequest::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
HelloRequest::~HelloRequest() {
  // @@protoc_insertion_point(destructor:helloworld.v1.HelloRequest)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void HelloRequest::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.name_.Destroy();
  _impl_.~Impl_();
}

const ::google::protobuf::MessageLite::ClassData*
HelloRequest::GetClassData() const {
  PROTOBUF_CONSTINIT static const ::google::protobuf::MessageLite::
      ClassDataFull _data_ = {
          {
              nullptr,  // OnDemandRegisterArenaDtor
              PROTOBUF_FIELD_OFFSET(HelloRequest, _impl_._cached_size_),
              false,
          },
          &HelloRequest::MergeImpl,
          &HelloRequest::kDescriptorMethods,
      };
  return &_data_;
}
PROTOBUF_NOINLINE void HelloRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:helloworld.v1.HelloRequest)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.name_.ClearToEmpty();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* HelloRequest::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 39, 2> HelloRequest::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_HelloRequest_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::helloworld::v1::HelloRequest>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // string name = 1;
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(HelloRequest, _impl_.name_)}},
  }}, {{
    65535, 65535
  }}, {{
    // string name = 1;
    {PROTOBUF_FIELD_OFFSET(HelloRequest, _impl_.name_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\32\4\0\0\0\0\0\0"
    "helloworld.v1.HelloRequest"
    "name"
  }},
};

::uint8_t* HelloRequest::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:helloworld.v1.HelloRequest)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // string name = 1;
  if (!this->_internal_name().empty()) {
    const std::string& _s = this->_internal_name();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "helloworld.v1.HelloRequest.name");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:helloworld.v1.HelloRequest)
  return target;
}

::size_t HelloRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:helloworld.v1.HelloRequest)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string name = 1;
  if (!this->_internal_name().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_name());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}


void HelloRequest::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<HelloRequest*>(&to_msg);
  auto& from = static_cast<const HelloRequest&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:helloworld.v1.HelloRequest)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_name().empty()) {
    _this->_internal_set_name(from._internal_name());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void HelloRequest::CopyFrom(const HelloRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:helloworld.v1.HelloRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool HelloRequest::IsInitialized() const {
  return true;
}

void HelloRequest::InternalSwap(HelloRequest* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.name_, &other->_impl_.name_, arena);
}

::google::protobuf::Metadata HelloRequest::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_greeter_2eproto_getter,
                                   &descriptor_table_greeter_2eproto_once,
                                   file_level_metadata_greeter_2eproto[0]);
}
// ===================================================================

class HelloReply::_Internal {
 public:
};

HelloReply::HelloReply(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:helloworld.v1.HelloReply)
}
inline PROTOBUF_NDEBUG_INLINE HelloReply::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : message_(arena, from.message_),
        _cached_size_{0} {}

HelloReply::HelloReply(
    ::google::protobuf::Arena* arena,
    const HelloReply& from)
    : ::google::protobuf::Message(arena) {
  HelloReply* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);

  // @@protoc_insertion_point(copy_constructor:helloworld.v1.HelloReply)
}
inline PROTOBUF_NDEBUG_INLINE HelloReply::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : message_(arena),
        _cached_size_{0} {}

inline void HelloReply::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
HelloReply::~HelloReply() {
  // @@protoc_insertion_point(destructor:helloworld.v1.HelloReply)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void HelloReply::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.message_.Destroy();
  _impl_.~Impl_();
}

const ::google::protobuf::MessageLite::ClassData*
HelloReply::GetClassData() const {
  PROTOBUF_CONSTINIT static const ::google::protobuf::MessageLite::
      ClassDataFull _data_ = {
          {
              nullptr,  // OnDemandRegisterArenaDtor
              PROTOBUF_FIELD_OFFSET(HelloReply, _impl_._cached_size_),
              false,
          },
          &HelloReply::MergeImpl,
          &HelloReply::kDescriptorMethods,
      };
  return &_data_;
}
PROTOBUF_NOINLINE void HelloReply::Clear() {
// @@protoc_insertion_point(message_clear_start:helloworld.v1.HelloReply)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.message_.ClearToEmpty();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* HelloReply::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 40, 2> HelloReply::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_HelloReply_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::helloworld::v1::HelloReply>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // string message = 1;
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(HelloReply, _impl_.message_)}},
  }}, {{
    65535, 65535
  }}, {{
    // string message = 1;
    {PROTOBUF_FIELD_OFFSET(HelloReply, _impl_.message_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\30\7\0\0\0\0\0\0"
    "helloworld.v1.HelloReply"
    "message"
  }},
};

::uint8_t* HelloReply::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:helloworld.v1.HelloReply)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // string message = 1;
  if (!this->_internal_message().empty()) {
    const std::string& _s = this->_internal_message();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "helloworld.v1.HelloReply.message");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:helloworld.v1.HelloReply)
  return target;
}

::size_t HelloReply::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:helloworld.v1.HelloReply)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string message = 1;
  if (!this->_internal_message().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_message());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}


void HelloReply::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<HelloReply*>(&to_msg);
  auto& from = static_cast<const HelloReply&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:helloworld.v1.HelloReply)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_message().empty()) {
    _this->_internal_set_message(from._internal_message());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void HelloReply::CopyFrom(const HelloReply& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:helloworld.v1.HelloReply)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool HelloReply::IsInitialized() const {
  return true;
}

void HelloReply::InternalSwap(HelloReply* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.message_, &other->_impl_.message_, arena);
}

::google::protobuf::Metadata HelloReply::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_greeter_2eproto_getter,
                                   &descriptor_table_greeter_2eproto_once,
                                   file_level_metadata_greeter_2eproto[1]);
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace v1
}  // namespace helloworld
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::std::false_type _static_init_ PROTOBUF_UNUSED =
    (::_pbi::AddDescriptors(&descriptor_table_greeter_2eproto),
     ::std::false_type{});
#include "google/protobuf/port_undef.inc"