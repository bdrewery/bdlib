/* BinaryTree.h
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
#ifndef _BD_STREAM_H
#define _BD_STREAM_H 1

#include <iostream>
#include "bdlib.h"
#include "String.h"

BDLIB_NS_BEGIN

#define STREAM_BLOCKSIZE	1024
/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET        0       /* Seek from beginning of file.  */
#define SEEK_CUR        1       /* Seek from current position.  */
#define SEEK_END        2       /* Seek from end of file.  */

class Stream {
  protected:
        String str;
        unsigned int pos;

  public:
        Stream() : str(), pos(0) {};
        Stream(Stream &stream) : str(stream.str), pos(stream.pos) {};
        Stream(const char* string) : str(string), pos(0) {};
        Stream(const char* string, size_t len) : str(string, len), pos(0) {};
        Stream(const char ch) : str(ch), pos(0) {};
        Stream(const int newSize) : str(), pos(0) { if (newSize > 0) Reserve(newSize); };
        virtual ~Stream() {};

#ifdef __GNUC__
        /* GNU GCC DOC:
           Since non-static C++ methods have an implicit this argument, the arguments of such methods
           should be counted from two, not one, when giving values for string-index and first-to-check.
         */
        virtual void printf(const char*, ...) __attribute__ ((format(printf, 2, 3)));
#else
        virtual void printf(const char*, ...);
#endif
        virtual void Reserve(const size_t) const;

        /**
         * @brief Returns the position of the Stream.
         * @return Position of the Stream.
        */
        size_t tell() const { return pos; };

        /**
         * @brief Truncates the stream at the current position.
        */
        void truncate() { str = str(0, pos); };

//        operator void*() { return tell() <= length(); };

        int seek(int, int);
        void clear() { str.clear(); pos = 0; }
        inline void puts (const String& string) { puts(string.data(), string.length()); }
        inline void puts (const char* string, size_t len) {
          str.replace(tell(), string, len);
          pos += len;
          /* WTF **/
          //Ref->size = max(tell(), capacity());
        }

        virtual int gets(char *, size_t);
        int loadFile(const char*);

        inline const char* data() const { return str.data(); };
        inline operator String() const { return str; };
        inline size_t length() const { return str.length(); };
        inline size_t capacity() const { return str.capacity(); };
        inline bool operator ! () const { return str.isEmpty(); };
};

BDLIB_NS_END
#endif /* !_BD_STREAM_H */
