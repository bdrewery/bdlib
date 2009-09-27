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

int Stream::loadFile(const char* file)
{
  clear();
#ifdef HAVE_MMAP
  int fd = open(file, O_RDONLY);
  if (fd == -1)
    return 1;
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
  close(fd);
#else
  FILE *f = NULL;
  f = fopen(file, "rb");
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

  fclose(f);
#endif
  seek(0, SEEK_SET);
  loading = 0;
  return 0;
}

BDLIB_NS_END
