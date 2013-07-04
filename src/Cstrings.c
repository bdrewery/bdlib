/*
 * Copyright (c) 2006-2013, Bryan Drewery <bryan@shatow.net>
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

/* Cstrings.c
 */

#include "Cstrings.h"

BDLIB_NS_BEGIN
/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t
strlcpy(char *dst, const char *src, size_t siz)
{
  register char *d = dst;
  register const char *s = src;
  register size_t n = siz;

  /* Copy as many bytes as will fit */
  if (n != 0 && --n != 0) {
    do {
      if ((*d++ = *s++) == 0)
        break;
    } while (--n != 0);
  }

  /* Not enough room in dst, add NUL and traverse rest of src */
  if (n == 0) {
    if (siz != 0)
      *d = '\0';                /* NUL-terminate dst */
    while (*s++) ;
  }

  return (s - src - 1);         /* count does not include NUL */
}

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t
strlcat(char *dst, const char *src, size_t siz)
{
  register char *d = dst;
  register const char *s = src;
  register size_t n = siz;
  size_t dlen;

  /* Find the end of dst and adjust bytes left but don't go past end */
  while (*d != '\0' && n-- != 0)
    d++;
  dlen = d - dst;
  n = siz - dlen;

  if (n == 0)
    return (dlen + strlen(s));
  while (*s != '\0') {
    if (n != 1) {
      *d++ = *s;
      n--;
    }
    s++;
  }
  *d = '\0';

  return (dlen + (s - src));    /* count does not include NUL */
}

void
str_redup(char **str, const char *newstr)
{
        size_t len;

        if (!newstr) {
                if (*str) free(*str);
                *str = NULL;
                return;
        }
        len = strlen(newstr) + 1;
        *str = static_cast<char*>(realloc(*str, len));
        memcpy(*str, newstr, len);
}

char *
strdup(const char *entry)
{
  size_t len = strlen(entry);
  char *target = static_cast<char*>(calloc(1, len + 1));
  if (target == NULL) return NULL;
  target[len] = 0;
  return static_cast<char*>(memcpy(target, entry, len));
}

char *
strldup(const char *entry, size_t maxlen)
{
  size_t slen = strlen(entry);
  size_t len = slen < maxlen ? slen : maxlen;
  char *target = static_cast<char*>(calloc(1, len + 1));
  if (target == NULL) return NULL;
  target[len] = 0;
  return static_cast<char*>(memcpy(target, entry, len));
}

void *calloc(size_t nmemb, size_t size)
{
  void *ptr = ::calloc(nmemb, size);

  if (ptr == NULL)
    exit(5);

  return ptr;
}

void *realloc(void *ptr, size_t size)
{
  void *x = ::realloc(ptr, size);

  if (x == NULL && size > 0)
    exit(5);

  return x;
}
BDLIB_NS_END
