/*
 * @Description:
 * @Author: yyun
 * @Date: 2023-09-21 16:39:42
 * @LastEditTime: 2023-09-22 14:15:42
 * @LastEditors: yyun
 */
#pragma once
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "types.h"

// need this typing because cpp mindlessly promotes
// bool to int. so it thinks statements like
// { (bool)foo < 0 } are sensical even though it will never happen
enum class read_t { OK, ERR };
inline bool is_ok(read_t const retcode) { return read_t::OK == retcode; }
inline read_t operator!(read_t const code) {
  return code;  // will throw compiler error if you try to use it like boolean!
}

/** Struct for buffered reading. Everything is public to give low level access
 * if needed */
typedef struct buf {
  buf(unsigned int len) : ptr(new char[len]), len(len), limit(0), pos(0) {}
  ~buf() { delete[] ptr; }
  char *const ptr;
  /** Length of buf */
  unsigned int const len;  // could be private
  /** Position of last byte read into buf */
  unsigned int limit;  // could be private 最后一个数据的位置 数据子pos 和limit之间  limit - pos就是有效数据
  unsigned int pos;
  size_t bytesread;  // 一共读了多少数据
  fd_t fd = -1;
  /** Returns a pointer to ptr + pos + idx */
  char const *get(unsigned int idx) const {
#if 0  // optimization
		assert (pos + idx <= limit);
#endif
    return ptr + pos + idx;
  }
  // maybe this could be useful ?
  // char head(void) const { return *(ptr + pos); }

  /** Returns available bytes in buf */
  unsigned available(void) const { return limit - pos; }
  bool available(unsigned n) const {
    return pos + n <= limit;  // faster than available() >= n;
  }
  /** Returns available space to read bytes into */
  unsigned free_space(void) const { return len - limit; }
  void advance(unsigned bytes) {  // 指针往后移 消费两个字节
    pos += bytes;
    assert(pos <= limit);
  }
  void discard_to_pos(void) {
    if (pos && pos < limit) memmove((void *)ptr, ptr + pos,
                                    limit - pos);  // shift to beginning of buf
    limit -= pos;
    pos = 0;
  }
  void cleanup(void) {
    limit = pos = 0;
    fd = -1;
  }
  /* blocking read. blocks until 1 or more (until available()) bytes are
   * available.
   */
  ssize_t read(void);
  /* blocking read. blocks until at least n bytes are available
   * returns number of bytes read. 0 in case of EOF and -1 for other read error.
   */
  ssize_t read(unsigned n);
  /* Essentially a wrapper around read(n). If at least n bytes are
   * available in the buffer returns true immediately. Otherwise
   * it tries to read as many bytes as necessary to get n bytes
   * in the buffer. Returns false if read(n) returns <= 0.
   */
  read_t ensure(unsigned n);
  /* non blocking read for use in a select / multiplexing event loop */
  ssize_t nb_read(void);
} buf_t;
