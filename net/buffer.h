#pragma once

#include <string>
#include <string_view>
#include <deque>

#include <cassert>
#include <cstring>
#include <unistd.h>

namespace tinyserver 
{
namespace net 
{

class Buffer {
 public:
  const char* peek() const { return begin(); }

  void append(const std::string_view &data) {
    buffer_.insert(buffer_.end(), data.begin(), data.end());
  }

  void append(const char *data, size_t len) {
    buffer_.insert(buffer_.end(), data, data + len);
  }

  void append(const void *data, size_t len) {
    append(static_cast<const char*>(data), len);
  }

  void retrieve(size_t len) {
    assert(len <= size());
    if (len < size()) {
      buffer_.erase(buffer_.begin(), buffer_.begin() + len);
    } else {
      retrieveAll();
    }
  }

  void retrieveInt64() {
    retrieve(sizeof(int64_t));
  }

  void retrieveInt32() {
    retrieve(sizeof(int32_t));
  }

  void retrieveInt16() {
    retrieve(sizeof(int16_t));
  }

  void retrieveInt8() {
    retrieve(sizeof(int8_t));
  }

  void retrieveAll() { buffer_.clear(); }

  std::string retrieveAllAsString() {
    return retrieveAsString(size());
  }

  std::string retrieveAsString(size_t len) {
    assert(len <= size());
    if (len > size()) {
      len = size();
    }
    std::string result(peek(), len);
    retrieve(len);
    return result;
  }

  std::string_view toStringView() const {
    return std::string_view(peek(), size());
  }

  ///
  /// Peek int64_t from network endian
  ///
  /// Require: buf->size() >= sizeof(int64_t)
  int64_t peekInt64() const
  {
    assert(size() >= sizeof(int64_t));
    int64_t be64 = 0;
    ::memcpy(&be64, peek(), sizeof be64);
    return be64;
  }

  ///
  /// Peek int32_t from network endian
  ///
  /// Require: buf->size() >= sizeof(int32_t)
  int32_t peekInt32() const
  {
    assert(size() >= sizeof(int32_t));
    int32_t be32 = 0;
    ::memcpy(&be32, peek(), sizeof be32);
    return be32;
  }

  int16_t peekInt16() const
  {
    assert(size() >= sizeof(int16_t));
    int16_t be16 = 0;
    ::memcpy(&be16, peek(), sizeof be16);
    return be16;
  }

  int8_t peekInt8() const
  {
    assert(size() >= sizeof(int8_t));
    int8_t x = *peek();
    return x;
  }

  size_t size() const { return buffer_.size(); }

 private:
  
  char* begin() { return &*buffer_.begin(); }
  const char* begin() const { return &*buffer_.begin(); }

  std::deque<char> buffer_;
};

}
}