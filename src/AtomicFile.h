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

/* AtomicFile.h
 */
#ifndef _BD_ATOMICFILE_H
#define _BD_ATOMICFILE_H 1

#include "bdlib.h"
#include "String.h"

BDLIB_NS_BEGIN

/**
 * @class AtomicFile
 * @brief Write out files atomically via tmpfile/rename.
 */
class AtomicFile {
  private:
    String _fname;
    String _tmpname;
    int _fd;
    mode_t _mode;
  public:
    AtomicFile() : _fname(), _tmpname(), _fd(-1), _mode(-1) {};
    AtomicFile(AtomicFile& f) : _fname(f._fname), _tmpname(), _fd(f._fd),
                                _mode(f._mode) {};
    /**
     * @brief Calling the destructor will abort the file if it has not already been written.
     * @sa abort
     */
    virtual ~AtomicFile() {
      if (this->is_open()) {
        this->abort();
      }
    }

    /**
     * @brief Open a file for writing.
     */
    bool open(const String& fname, mode_t mode = -1);

    /**
     * @brief Abort creating a file and cleanup.
     */
    bool abort();

    /**
     * @brief Check if a file was successfully opened.
     */
    inline bool is_open() const {
      return _fd != -1;
    }

    /**
     * @brief Return an fd for the open file.
     * @return Returns -1 if not open.
     */
    inline int fd() {
      return (this->is_open() ? _fd : -1);
    }

    /**
     * @brief Write out the file and close the descriptor.
     */
    bool commit();
};
BDLIB_NS_END
#endif /* _BD_ATOMICFILE_H */ 
/* vim: set sts=2 sw=2 ts=8 et: */
