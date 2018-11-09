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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTIRBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Stream.h
 */

#ifndef _BD_STREAM_H
#define _BD_STREAM_H 1

#include <iostream>
#include <algorithm>
#include "bdlib.h"
#include "String.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

BDLIB_NS_BEGIN

#define STREAM_BLOCKSIZE	1024
/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET        0       /* Seek from beginning of file.  */
#define SEEK_CUR        1       /* Seek from current position.  */
#define SEEK_END        2       /* Seek from end of file.  */

class Stream {
  protected:
        String str{};
        size_t pos = 0;
        bool loading = false;

  public:
        Stream() noexcept = default;
        Stream(const Stream& stream) noexcept = default;
        Stream(Stream&& stream) noexcept = default;
        Stream(const int newSize) {
          if (newSize > 0) reserve(newSize);
        }
        virtual ~Stream() {};

        Stream(const String& string) noexcept : str(string) {};
        Stream(String&& string) noexcept : str(std::move(string)) {};

        friend void swap(Stream& a, Stream& b) noexcept {
          using std::swap;

          swap(a.str, b.str);
          swap(a.pos, b.pos);
          swap(a.loading, b.loading);
        }

        Stream& operator=(Stream stream) & {
          swap(*this, stream);
          return *this;
        }

        Stream& operator=(Stream&& stream) & noexcept = default;

        void reserve(const size_t) const;

        /**
         * @brief Returns the position of the Stream.
         * @return Position of the Stream.
        */
        size_t tell() const __attribute__((pure)) {
          return pos;
        }

        /**
         * @brief Truncates the stream at the current position.
        */
        void truncate() { str = str(0, pos); };

        String peek() { return str(pos); };

//        operator void*() { return tell() <= length(); };

        int seek(int, int);
        void clear() { str.clear(); pos = 0; }

        /**
         * @brief Insert a string into the stream.
         * @note The stream pointer is advanced as well
         */
        virtual void puts (const String& string) {
          str.replace(tell(), string);
          pos += string.length();
        }

        /**
         * @brief Insert a string into the stream.
         * @note The stream pointer is advanced as well
         */
        virtual void puts (String&& string) {
          auto len = string.length();
          str.replace(tell(), std::move(string));
          pos += len;
        }

        /**
         * @brief Reads 1 line from the stream
         * @note The stream pointer is advanced as well
         * @param maxSize Optional param which specifies max data to pull
         * @sa read
         */
        virtual String getline (size_t maxSize = 99999999) {
          return read(maxSize == 99999999 ? (length() - pos) : maxSize, '\n');
        }

        /**
         * @brief Reads specified number of bytes from the stream
         * @param maxSize How many bytes to read
         * @param delim What to split the read on. For example: '\n' will return 1 line.
         * @note The stream pointer is advanced as well
         */
        virtual String read (size_t maxSize, char delim = 0);

        /**
         * @brief Load a file into the stream
         * @returns 1 on error, 0 on success.
         * @param fd File descriptor to read from
         * @note this.loading is set to 1 during this process.
         * @note Uses puts() for populating the stream.
         * @note The fd is not closed.
         * @sa puts
         */
        virtual int loadFile(const int fd);

        /**
         * @brief Load a file into the stream
         * @returns 1 on error, 0 on success.
         * @param fname File to read from
         * @note this.loading is set to 1 during this process.
         * @note Uses puts() for populating the stream.
         * @sa puts
         */
        virtual int loadFile(const String& fname);

        /**
         * @brief Write stream out to a file
         * @returns 1 on error, 0 on success
         * @param fd File descriptor to write to
         * @note The fd is not closed.
         */
        virtual int writeFile(const int fd) const;

        /**
         * @brief Write stream out to a file
         * @returns 1 on error, 0 on success
         * @param fname Filename to write to
         * @param mode Optional param to specify mode for new file
         */
        virtual int writeFile(const String& fname,
            mode_t mode = (S_IRUSR|S_IWUSR)) const;

        inline operator String() const __attribute__((pure)) {
          return str;
        }
        inline size_t length() const __attribute__((pure)) {
          return str.length();
        }
        inline size_t capacity() const __attribute__((pure)) {
          return str.capacity();
        }
        inline bool operator!() const __attribute__((pure)) {
          return str.isEmpty();
        }

        friend Stream& operator<<(Stream&, const String&);
        friend Stream& operator<<(Stream&, String&&);
};

inline Stream& operator<<(Stream& stream, const String& string) {
  stream.puts(string);
  return stream;
}

inline Stream& operator<<(Stream& stream, String&& string) {
  stream.puts(std::move(string));
  return stream;
}

BDLIB_NS_END
#endif /* _BD_STREAM_H */
/* vim: set sts=2 sw=2 ts=8 et: */
