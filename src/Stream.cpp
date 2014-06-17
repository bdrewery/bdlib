/*
 * Copyright (c) 2006-2014, Bryan Drewery <bryan@shatow.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Stream.c
 *
 */
#include "Stream.h"
#include <algorithm> // min() / max()
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef HAVE_MMAP
#  include <sys/mman.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <cstring>
#endif
#include <unistd.h>


BDLIB_NS_BEGIN

void Stream::Reserve(const size_t newSize) const {
  if (newSize < capacity())
    return;
  str.Reserve(STREAM_BLOCKSIZE * ((newSize + STREAM_BLOCKSIZE -1) / STREAM_BLOCKSIZE));
}

int Stream::seek(int offset, int whence) {
  int newpos;

  switch (whence) {
    case SEEK_SET:
      newpos = offset;
      break;
    case SEEK_CUR:
      newpos = tell() + offset;
      break;
    case SEEK_END:
      newpos = capacity() - offset;
      break;
    default:
      newpos = tell();
  }
  if (newpos < 0)
    newpos = 0;
  else if (static_cast<size_t>(newpos) > capacity())
    newpos = capacity();
  pos = newpos;

  return newpos;
}

String Stream::read(size_t maxSize, char delim) {
  size_t toRead = std::min(maxSize, length() - pos);

  /* No need to split the string, return a substring */
  if (!delim) {
    String ret(str(pos, toRead));
    pos += toRead;
    return ret;
  }

  /* Must reconstruct a new string */
  String ret(toRead);

  while ((ret.length() < toRead) && (str[pos] != delim)) {
    ret += str[pos++];
  }
  if (str.hasIndex(pos) && str[pos] == delim) {
    ret += delim;
    ++pos;
  }

  return ret;
}

int Stream::loadFile(const String& file)
{
  int fd = open(file.c_str(), O_RDONLY);
  if (fd == -1)
    return 1;
  int ret = loadFile(fd);
  close(fd);
  return ret;
}

int Stream::loadFile(const int fd)
{
  clear();
#ifdef HAVE_MMAP
  size_t size = lseek(fd, 0, SEEK_END);
  Reserve(size);
  void* map = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED) {
    return 1;
  }
  loading = 1;

  puts(String(static_cast<const char*>(map), size));

  munmap(static_cast<void*>(map), size);
#else
  int my_fd = dup(fd);
  if (my_fd == -1) {
    return 1;
  }
  FILE *f = fdopen(my_fd, "rb");
  if (f == nullptr) {
    close(my_fd);
    return 1;
  }

  loading = 1;
  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);
  Reserve(size);

  size_t len = 0;
  char buf[STREAM_BLOCKSIZE + 1];

  while ((len = fread(buf, 1, sizeof(buf) - 1, f))) {
    puts(String(buf, len));
  }
  fclose(f);
#endif
  seek(0, SEEK_SET);
  loading = 0;
  return 0;
}

int Stream::writeFile(const String& file, mode_t mode) const
{
  int fd = open(file.c_str(), O_CREAT|O_RDWR|O_TRUNC, mode);
  if (fd == -1)
    return 1;
  int ret = writeFile(fd);
  close(fd);
  return ret;
}

int Stream::writeFile(const int fd) const
{
#ifdef HAVE_MMAP
  /* Write to end of file to make its size match */
  /* Seek past wherever it is at now plus our length */
  if (ftruncate(fd, length())) return 1;
  if (!length())
    return 0;

  void* map = mmap(0, length(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  if (map == MAP_FAILED) return 1;

  std::memcpy(map, str.data(), length());

  if (munmap(map, length()) == -1) return 1;
#else
  int my_fd = dup(fd);
  if (my_fd == -1) {
    return 1;
  }
  FILE *f = fdopen(my_fd, "wb");
  if (f == nullptr) {
    close(my_fd);
    return 1;
  }

  if ((fwrite(str.data(), 1, length(), f) != length()) || (fflush(f))) {
    fclose(f);
    return 1;
  }
  fclose(f);
#endif
  return 0;
}

BDLIB_NS_END
