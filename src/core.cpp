/* core.cpp
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

#include "core.h"
#include <stdio.h>

Core::Core(int argc, char** argv)
{
  this->argc = argc;
  this->argv = argv;
  getFullBinname();
}

Core::~Core()
{
}

void Core::getFullBinname()
{
  char *bin = bd::strdup(argv[0]), *p = NULL, *p2 = NULL;
  char cwd[PATH_MAX] = "", buf[PATH_MAX] = "";
  size_t len = 0;

  if (bin[0] == '/') {
#ifdef CYGWIN_HACKS
    goto cygwin;
    len = strlen(bin);
#else
    binname = bin;
    free(bin);
    return;
#endif /* CYGWIN_HACKS */
  }

  if (!getcwd(cwd, PATH_MAX))
    exit(1);

  if (cwd[strlen(cwd) - 1] == '/')
    cwd[strlen(cwd) - 1] = 0;

  p = bin;
  p2 = strchr(p, '/');

  while (p) {
    if (p2)
      *p2++ = 0;
    if (!strcmp(p, "..")) {
      p = strrchr(cwd, '/');
      if (p)
        *p = 0;
    } else if (strcmp(p, ".")) {
      strcat(cwd, "/");
      strcat(cwd, p);
    }
    p = p2;
    if (p)
      p2 = strchr(p, '/');
  }
  bd::str_redup(&bin, cwd);
  len = strlen(bin);
#ifdef CYGWIN_HACKS
  /* tack on the .exe */
  cygwin:
  bin = (char *) bd::realloc(bin, len + 4 + 1);
  len += 4;
  strcat(bin, ".exe");
  bin[len] = 0;
#endif /* CYGWIN_HACKS */
  /* Fix for symlinked binaries */
  realpath(bin, buf);
  binname = buf;
  free(bin);
}
