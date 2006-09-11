/* Cstrings.c
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

#ifndef lint
static const char rcsid[] = "$Id$";
#endif /* lint */

#include "Cstrings.h"

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
        *str = (char *) my_realloc(*str, len);
        memcpy(*str, newstr, len);
}

char *
strdup(const char *entry)
{
  size_t len = strlen(entry);
  char *target = (char *) my_calloc(1, len + 1);
  if (target == NULL) return NULL;
  target[len] = 0;
  return (char *) memcpy(target, entry, len);
}

char *
strldup(const char *entry, size_t maxlen)
{
  size_t slen = strlen(entry);
  size_t len = slen < maxlen ? slen : maxlen;
  char *target = (char *) my_calloc(1, len + 1);
  if (target == NULL) return NULL;
  target[len] = 0;
  return (char *) memcpy(target, entry, len);
}

void *my_calloc(size_t nmemb, size_t size)
{
  void *ptr = calloc(nmemb, size);

  if (ptr == NULL)
    exit(5);

  return ptr;
}

void *my_realloc(void *ptr, size_t size)
{
  void *x = realloc(ptr, size);

  if (x == NULL && size > 0)
    exit(5);

  return x;
}

