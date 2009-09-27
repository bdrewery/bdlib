/* Stream.c
 *
 */
#include "Stream.h"
#include <stdarg.h>
#include <algorithm> // min() / max()

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
  seek(0, SEEK_SET);
  loading = 0;
  return 0;
}

BDLIB_NS_END
