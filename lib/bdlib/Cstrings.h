/* Cstrings.h
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
 * $Id$
 */
#ifndef _W_CSTRINGS_H
#define _W_CSTRINGS_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif


#define strlcpy my_strlcpy
#define strlcat my_strlcat
#define str_redup my_str_redup
#define strdup my_strdup
#undef calloc
#undef realloc

size_t strlcpy(char *, const char *, size_t);
size_t strlcat(char *, const char *, size_t);
void str_redup(char **, const char *);
char *strdup(const char *);
char *strldup(const char *, size_t);
void *my_calloc(size_t, size_t);
void *my_realloc(void *, size_t);

#endif /* !_W_CSTRINGS_H */ 
