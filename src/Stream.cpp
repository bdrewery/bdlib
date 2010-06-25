/* Stream.c
 *
 */
#include "Stream.h"
#include <stdarg.h>
#include <algorithm> // min() / max()
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef HAVE_MMAP
#  include <sys/mman.h>
#  include <unistd.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <cstring>
#endif


BDLIB_NS_BEGIN

void Stream::Reserve (const size_t newSize) const {
  if (newSize < capacity())
    return;
  str.Reserve(STREAM_BLOCKSIZE * ((newSize + STREAM_BLOCKSIZE -1) / STREAM_BLOCKSIZE));
}

int Stream::seek (int offset, int whence) {
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
  else if ((unsigned) newpos > capacity())
    newpos = capacity();
  pos = newpos;

  return newpos;
}

String Stream::gets (size_t maxSize, char delim) {
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
  unsigned char* map = (unsigned char*) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
  if ((void*)map == MAP_FAILED) {
    close(fd);
    return 1;
  }
  loading = 1;

  puts(String((const char*)map, size));

  munmap(map, size);
#else
  FILE *f = fdopen(fd, "rb");
  if (f == NULL)
    return 1;

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

  unsigned char* map = (unsigned char*) mmap(0, length(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  if ((void*)map == MAP_FAILED) return 1;

  std::memcpy(map, str.data(), length());

  if (munmap(map, length()) == -1) return 1;
#else
  FILE *f = fdopen(fd, "wb");
  if (f == NULL)
    return 1;

  if ((fwrite(str.data(), 1, length(), f) != length()) || (fflush(f)))
    return 1;
#endif
  return 0;
}

BDLIB_NS_END
