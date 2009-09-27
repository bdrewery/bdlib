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
        bool loading;

  public:
        Stream() : str(), pos(0), loading(0) {};
        Stream(Stream &stream) : str(stream.str), pos(stream.pos), loading(0) {};
        Stream(const char* string) : str(string), pos(0), loading(0) {};
        Stream(const char* string, size_t len) : str(string, len), pos(0), loading(0) {};
        Stream(const char ch) : str(ch), pos(0), loading(0) {};
        Stream(const int newSize) : str(), pos(0), loading(0) { if (newSize > 0) Reserve(newSize); };
        virtual ~Stream() {};

#ifdef __GNUC__
        /* GNU GCC DOC:
           Since non-static C++ methods have an implicit this argument, the arguments of such methods
           should be counted from two, not one, when giving values for string-index and first-to-check.
         */
        void printf(const char*, ...) __attribute__ ((format(printf, 2, 3)));
#else
        void printf(const char*, ...);
#endif
        void Reserve(const size_t) const;

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

        /*
         * @brief Insert a null-terminated character array into the stream.
         * The stream pointer is advanced as well
         */
        void puts (const char* string, size_t len) { puts(String(string, len)); }

        /*
         * @brief Insert a string into the stream.
         * @note The stream pointer is advanced as well
         */
        virtual void puts (const String& string) {
          str.replace(tell(), string);
          pos += string.length();
          /* WTF **/
          //Ref->size = max(tell(), capacity());
        }

        /*
         * @brief Reads 1 line from the stream into a null-terminated character array
         * @note The stream pointer is advanced as well
         * @sa gets
         * @returns Size of the line
         */
        virtual int getline (char *_data, size_t maxSize) { return gets(_data, maxSize, '\n'); }

        /*
         * @brief Reads specified number of bytes from the stream into a null-terminated character array
         * @param maxSize How many bytes to read
         * @param delim What to split the read on. For example: '\n' will return 1 line.
         * @note The stream pointer is advanced as well
         * @returns Size of the data read
         */
        virtual int gets (char *_data, size_t maxSize, char delim = 0) {
          size_t toRead, read = 0;
          char c = 0;

          toRead = (maxSize <= (capacity() - tell())) ? maxSize : (capacity() - tell());

          while ((read < toRead) && (!delim || (c != '\n'))) {
            c = str[pos++];
            *_data++ = c;
            ++read;
          }

          if ( (read < toRead) || (toRead < maxSize))
            *_data = 0;

          return read;
        }

        /*
         * @brief Load a file into the stream
         * @returns 1 on error, 0 on success.
         * @note this.loading is set to 1 during this process.
         */
        int loadFile(const char*);

        inline const char* data() const { return str.data(); };
        inline operator String() const { return str; };
        inline size_t length() const { return str.length(); };
        inline size_t capacity() const { return str.capacity(); };
        inline bool operator ! () const { return str.isEmpty(); };

        friend Stream& operator << (Stream&, const String&);
};

inline Stream& operator << (Stream& stream, const String& string) { stream.puts(string); return stream; }

BDLIB_NS_END
#endif /* !_BD_STREAM_H */
