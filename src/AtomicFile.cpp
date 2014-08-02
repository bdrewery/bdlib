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

/* AtomicFile.cpp
 */

#include <sys/stat.h>

#include <unistd.h>

#include "AtomicFile.h"

BDLIB_NS_BEGIN

bool AtomicFile::open(const String& fname, mode_t mode) {
  size_t slash_pos;
  String dir;

  this->_fname = fname;
  this->_mode = mode;

  /*
   * Get a tempfile in the same directory as the target so that rename(2)
   * is not cross-device.
   */
  if ((slash_pos = this->_fname.rfind("/")) == String::npos) {
    slash_pos = 0;
    dir = "./";
  } else {
    dir = this->_fname(0, slash_pos + 1);	/* include the '/' */
  }

  this->_tmpname = String::printf("%s.%s.XXXXXX", dir.c_str(),
      static_cast<String>(this->_fname(slash_pos)).c_str());

  /* XXX: Move nul-writing to String per C++11 */
  this->_tmpname.resize(this->_tmpname.length() + 1);
  this->_fd = mkstemp(this->_tmpname.begin());

  return this->is_open();
}

bool AtomicFile::abort() {
  if (!this->is_open()) {
    return false;
  }
  ::unlink(this->_tmpname.c_str());
  ::close(this->_fd);
  this->_fd = -1;
  return true;
}

bool AtomicFile::commit() {
  bool result, remove_temp;

  result = false;
  remove_temp = true;
  if (this->_mode != mode_t(-1) && fchmod(this->_fd, this->_mode) != 0) {
    goto cleanup;
  }
  if (::fsync(this->_fd) != 0) {
    goto cleanup;
  }
  if (::close(this->_fd) != 0) {
    goto cleanup;
  }
  if (::rename(this->_tmpname.c_str(), this->_fname.c_str()) != 0) {
    goto cleanup;
  }

  remove_temp = false;
  result = true;

cleanup:
  if (remove_temp) {
    ::unlink(this->_tmpname.c_str());
  }
  this->_fd = -1;
  return result;
}
BDLIB_NS_END
/* vim: set sts=2 sw=2 ts=8 et: */
