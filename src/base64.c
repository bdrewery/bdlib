/* base64.c
 *
 * Copyright (C) Bryan Drewery
 *
 * This program is private and may not be distributed, modified
 * or used without express permission of Bryan Drewery.
 *
 * THIS PROGRAM IS DISTRIBUTED WITHOUT ANY WARRANTY.
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 * DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 * IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
 * NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 */


#include <cstdlib>
#include "base64.h"
#include "String.h"

BDLIB_NS_BEGIN

#define NUM_ASCII_BYTES (3)
#define NUM_ENCODED_BYTES (4)
#define PADDING_CHAR '='

void b64enc_buf(const unsigned char *data, size_t *len, char *dest, const char* charset)
{
  char *buf = dest;

  if (!charset) {
    charset = b64_charset;
  }

  /* Encode 3 bytes at a time. */

  /*
   *
   * |       0       |       1       |       2       |
   *
   * |               |               |               |
   * |       |       |       |       |       |       |
   * |   |   |   |   |   |   |   |   |   |   |   |   |
   * | | | | | | | | | | | | | | | | | | | | | | | | |
   *  7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
   * -------------------------------------------------
   *  5 4 3 2 1 0 5 4 3 2 1 0 5 4 3 2 1 0 5 4 3 2 1 0 
   * | | | | | | | | | | | | | | | | | | | | | | | | |
   * |   | | |   |   | | |   |   | | |   |   | | |   |
   * |     |     |     |     |     |     |     |     |
   * |           |           |           |           |
   *
   * |     0     |     1     |     2     |     3     |
   *
   */


  while (*len >= NUM_ASCII_BYTES) {
    /* buf[0] is the 6 left-most bits of data[0] */
    buf[0] = charset[(data[0] & 0xfc) >> 2];
    /* buf[1] is the right-most 2 bits of data[0] and the left-most 4 bits of data[1] */
    buf[1] = charset[((data[0] & 0x03) << 4) | ((data[1] & 0xf0) >> 4)];
    /* buf[2] is the right-most 4 bits of data[1] and the 2 left-most bits of data[2] */
    buf[2] = charset[((data[1] & 0x0f) << 2) | ((data[2] & 0xc0) >> 6)];
    /* buf[3] is the right-most 6 bits of data[2] */
    buf[3] = charset[data[2] & 0x3f];

    data += NUM_ASCII_BYTES;
    buf += NUM_ENCODED_BYTES;
    *len -= NUM_ASCII_BYTES;
  }

  /* There is either 1 or 2 bytes left to encode (and possibly some padding to add) */
  if (*len > 0) {
    buf[0] = charset[(data[0] & 0xfc) >> 2];
    if (*len == 1) {
      buf[1] = charset[((data[0] & 0x03) << 4)];
      buf[2] = PADDING_CHAR;
    } else if (*len > 1) { /* == 2 .. left as >1 to help optimize with last if */
      /* Tack on to buf[1] the left-most 4 bits of data[1] */
      buf[1] = charset[((data[0] & 0x03) << 4) | ((data[1] & 0xf0) >> 4)];
      buf[2] = charset[(data[1] & 0x0f) << 2];
    }
    buf[3] = PADDING_CHAR;
    buf += NUM_ENCODED_BYTES;
  }

  *len = buf - dest;
}

String base64Encode(const String& string, const char* charset) {
  size_t len = string.length();
  char *p = b64enc(reinterpret_cast<const unsigned char*>(string.data()), &len, charset);
  bd::String encoded(p, len);
  free(p);
  return encoded;
}

char *b64enc(const unsigned char *src, size_t *len, const char* charset)
{
  /* Take the length and round up the next 4-byte boundary */
  size_t dlen = (((*len + (NUM_ASCII_BYTES - 1)) / NUM_ASCII_BYTES) * NUM_ENCODED_BYTES);
  char *dest = static_cast<char*>(malloc(dlen + 1));

  b64enc_buf(src, len, dest, charset);
  dest[*len] = '\0';
  return dest;
}

void b64dec_buf(const unsigned char *data, size_t *len, char *dest, const char* charset_index)
{
  char *buf = dest;
  size_t blocks = (*len / NUM_ENCODED_BYTES) + ((*len % NUM_ENCODED_BYTES) ? 1 : 0);

  if (!charset_index) {
    charset_index = b64_indexes;
  }

  /* Convert the encoded base64 character back into our base255 ASCII character */
  while (blocks > 0) {
    /* buf[0] is the 6 bits from data[0] + left-most 2 bits of data[1] */
    *buf++ = (charset_index[data[0]] << 2) + ((charset_index[data[1]] & 0x30) >> 4);  /* 0x30 not required but to be clear. */
    if (data[2] != PADDING_CHAR) {
      /* buf[1] is the right-most 4 bits of data[1] + left-most 4 bits of data[2] */
      *buf++ = ((charset_index[data[1]] & 0x0f) << 4) + ((charset_index[data[2]] & 0x3c) >> 2);
      if (data[3] != PADDING_CHAR) {
        /* buf[2] is the right-most 2 bits of data[2] + data[3] */
        *buf++ = ((charset_index[data[2]] & 0x03) << 6) + charset_index[data[3]];
      }
    }
    data += NUM_ENCODED_BYTES;
    --blocks;
  }

  *len = (buf - dest);
}

char *b64dec(const unsigned char *data, size_t *len, const char* charset)
{
  size_t dlen = ((*len / NUM_ENCODED_BYTES) + ((*len % NUM_ENCODED_BYTES) ? 1 : 0)) * NUM_ASCII_BYTES;
  char *dest = static_cast<char*>(malloc(dlen + 1));

  if (charset) {
    char charset_index[256];
    memset(charset_index, -1, sizeof(charset_index));
    for (size_t i = 0; i < 64; ++i)
      charset_index[int(charset[i])] = i;
    b64dec_buf(data, len, dest, charset_index);
  }  else
    b64dec_buf(data, len, dest);
  dest[*len] = '\0';
  return dest;
}

String base64Decode(const String& string, const char* charset) {
  size_t len = string.length();
  char *p = b64dec(reinterpret_cast<const unsigned char*>(string.data()), &len, charset);
  bd::String decoded(p, len);
  free(p);
  return decoded;
}

BDLIB_NS_END
