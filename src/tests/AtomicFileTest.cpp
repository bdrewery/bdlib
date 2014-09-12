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

/* AtomicFileTest.c
 *
 */
#include "AtomicFileTest.h"
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (AtomicFileTest);

void AtomicFileTest :: setUp (void)
{
  FILE *f;
  size_t source_len;

  chdir("/tmp/");

  /* Copy /etc/services into source */
  f = fopen("/etc/services", "r");
  CPPUNIT_ASSERT(f != NULL);
  fseek(f, 0, SEEK_END);
  source_len = ftell(f);
  CPPUNIT_ASSERT(source_len > 0);
  source = new String();
  source->resize(source_len);
  fseek(f, 0, SEEK_SET);
  CPPUNIT_ASSERT_EQUAL(source_len, fread(source->begin(), 1, source_len, f));
  CPPUNIT_ASSERT_EQUAL(source_len, source->length());
  fclose(f);
}

void AtomicFileTest :: tearDown (void)
{
  delete source;
}

void AtomicFileTest :: basicTest (void)
{
  AtomicFile *a;
  int fd;
  FILE *f, *f2;
  const char *dst_name;
  char buf[4];
  String verification;
  struct stat st;

  dst_name = "test.file";
  unlink(dst_name);

  /* Generate a junk file to replace and keep it open */
  f2 = fopen(dst_name, "w");
  CPPUNIT_ASSERT(f2 != NULL);
  CPPUNIT_ASSERT_EQUAL(size_t(4), fwrite("test", 1, 4, f2));
  fflush(f2);
  /* !! Keep dst_name open so it is busy */

  /* Write out source into the dst */
  a = new AtomicFile;
  a->open(dst_name, S_IRUSR);
  CPPUNIT_ASSERT_EQUAL(true, a->is_open());
  /* Write out the data to the fd through a FILE stream */
  fd = a->fd();
  CPPUNIT_ASSERT(fd != -1);
  /* dup(2) the fd due to lack of fdclose() */
  fd = dup(fd);
  f = fdopen(fd, "w");
  CPPUNIT_ASSERT(f != NULL);
  CPPUNIT_ASSERT_EQUAL(source->length(), fwrite(
        static_cast<const char*>(source->begin()), 1, source->length(), f));
  fclose(f);

  /* Verify the contents of the dst have not yet changed */
  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT_EQUAL(size_t(4), fread(buf, 1, 4, f));
  fclose(f);
  CPPUNIT_ASSERT_STRING_EQUAL("test", String(buf, 4));

  /* Write out the file atomically. */
  CPPUNIT_ASSERT_EQUAL(true, a->commit());
  /* !! a left open, it is expected to flush out / rename at close() */

  /* Now verify the contents of dst against source */

  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT(f != NULL);
  fseek(f, 0, SEEK_END);
  CPPUNIT_ASSERT_EQUAL(source->length(), static_cast<size_t>(ftell(f)));
  verification.resize(source->length());
  fseek(f, 0, SEEK_SET);
  CPPUNIT_ASSERT_EQUAL(source->length(), fread(verification.begin(), 1,
        source->length(), f));
  CPPUNIT_ASSERT_STRING_EQUAL(*source, verification);

  /* Also verify the chmod */
  CPPUNIT_ASSERT_EQUAL(0, fstat(fileno(f), &st));
  CPPUNIT_ASSERT_EQUAL(mode_t(S_IRUSR), mode_t(st.st_mode & ~S_IFMT));

  fclose(f);

  fclose(f2);		/* The junk file kept open */
  unlink(dst_name);
  delete a;
}

void AtomicFileTest :: absoluteTest (void)
{
  AtomicFile *a;
  int fd;
  FILE *f, *f2;
  const char *dst_name;
  char buf[4];
  String verification;
  struct stat st;

  dst_name = "/tmp/test.file";
  unlink(dst_name);

  /* Generate a junk file to replace and keep it open */
  f2 = fopen(dst_name, "w");
  CPPUNIT_ASSERT(f2 != NULL);
  CPPUNIT_ASSERT_EQUAL(size_t(4), fwrite("test", 1, 4, f2));
  fflush(f2);
  /* !! Keep dst_name open so it is busy */

  /* Write out source into the dst */
  a = new AtomicFile;
  a->open(dst_name, S_IRUSR);
  CPPUNIT_ASSERT_EQUAL(true, a->is_open());
  /* Write out the data to the fd through a FILE stream */
  fd = a->fd();
  CPPUNIT_ASSERT(fd != -1);
  /* dup(2) the fd due to lack of fdclose() */
  fd = dup(fd);
  f = fdopen(fd, "w");
  CPPUNIT_ASSERT(f != NULL);
  CPPUNIT_ASSERT_EQUAL(source->length(), fwrite(
        static_cast<const char*>(source->begin()), 1, source->length(), f));
  fclose(f);

  /* Verify the contents of the dst have not yet changed */
  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT_EQUAL(size_t(4), fread(buf, 1, 4, f));
  fclose(f);
  CPPUNIT_ASSERT_STRING_EQUAL("test", String(buf, 4));

  /* Write out the file atomically. */
  CPPUNIT_ASSERT_EQUAL(true, a->commit());
  /* !! a left open, it is expected to flush out / rename at close() */

  /* Now verify the contents of dst against source */

  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT(f != NULL);
  fseek(f, 0, SEEK_END);
  CPPUNIT_ASSERT_EQUAL(source->length(), static_cast<size_t>(ftell(f)));
  verification.resize(source->length());
  fseek(f, 0, SEEK_SET);
  CPPUNIT_ASSERT_EQUAL(source->length(), fread(verification.begin(), 1,
        source->length(), f));
  CPPUNIT_ASSERT_STRING_EQUAL(*source, verification);

  /* Also verify the chmod */
  CPPUNIT_ASSERT_EQUAL(0, fstat(fileno(f), &st));
  CPPUNIT_ASSERT_EQUAL(mode_t(S_IRUSR), mode_t(st.st_mode & ~S_IFMT));

  fclose(f);

  fclose(f2);		/* The junk file kept open */
  unlink(dst_name);
  delete a;
}

void AtomicFileTest :: abortTest (void)
{
  AtomicFile *a;
  int fd;
  FILE *f, *f2;
  const char *dst_name;
  char buf[4];
  String verification;

  dst_name = "test.file";
  unlink(dst_name);

  /* Generate a junk file to replace and keep it open */
  f2 = fopen(dst_name, "w");
  CPPUNIT_ASSERT(f2 != NULL);
  CPPUNIT_ASSERT_EQUAL(size_t(4), fwrite("test", 1, 4, f2));
  fflush(f2);
  /* !! Keep dst_name open so it is busy */

  /* Write out source into the dst */
  a = new AtomicFile;
  a->open(dst_name);
  CPPUNIT_ASSERT_EQUAL(true, a->is_open());
  /* Write out the data to the fd through a FILE stream */
  fd = a->fd();
  CPPUNIT_ASSERT(fd != -1);
  /* dup(2) the fd due to lack of fdclose() */
  fd = dup(fd);
  f = fdopen(fd, "w");
  CPPUNIT_ASSERT(f != NULL);
  CPPUNIT_ASSERT_EQUAL(source->length(), fwrite(
        static_cast<const char*>(source->begin()), 1, source->length(), f));
  fsync(fd);
  fclose(f);

  /* Verify the contents of the dst have not yet changed */
  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT_EQUAL(size_t(4), fread(buf, 1, 4, f));
  fclose(f);
  CPPUNIT_ASSERT_STRING_EQUAL("test", String(buf, 4));

  /* Abort the atomic file writing and verify the contents and file remain. */
  CPPUNIT_ASSERT_EQUAL(true, a->abort());

  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT_EQUAL(size_t(4), fread(buf, 1, 4, f));
  fseek(f, 0, SEEK_END);
  CPPUNIT_ASSERT_EQUAL(size_t(4), static_cast<size_t>(ftell(f)));
  fclose(f);
  CPPUNIT_ASSERT_STRING_EQUAL("test", String(buf, 4));

  fclose(f2);		/* The junk file kept open */
  unlink(dst_name);
  delete a;
}

void AtomicFileTest :: deleteTest (void)
{
  AtomicFile *a;
  int fd;
  FILE *f, *f2;
  const char *dst_name;
  char buf[4];
  String verification;

  dst_name = "test.file";
  unlink(dst_name);

  /* Generate a junk file to replace and keep it open */
  f2 = fopen(dst_name, "w");
  CPPUNIT_ASSERT(f2 != NULL);
  CPPUNIT_ASSERT_EQUAL(size_t(4), fwrite("test", 1, 4, f2));
  fflush(f2);
  /* !! Keep dst_name open so it is busy */

  /* Write out source into the dst */
  a = new AtomicFile;
  a->open(dst_name);
  CPPUNIT_ASSERT_EQUAL(true, a->is_open());
  /* Write out the data to the fd through a FILE stream */
  fd = a->fd();
  CPPUNIT_ASSERT(fd != -1);
  /* dup(2) the fd due to lack of fdclose() */
  fd = dup(fd);
  f = fdopen(fd, "w");
  CPPUNIT_ASSERT(f != NULL);
  CPPUNIT_ASSERT_EQUAL(source->length(), fwrite(
        static_cast<const char*>(source->begin()), 1, source->length(), f));
  fsync(fd);
  fclose(f);

  /* Verify the contents of the dst have not yet changed */
  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT_EQUAL(size_t(4), fread(buf, 1, 4, f));
  fclose(f);
  CPPUNIT_ASSERT_STRING_EQUAL("test", String(buf, 4));

  /* Abort the atomic file writing and verify the contents and file remain. */
  delete a;
  a = NULL;

  f = fopen(dst_name, "r");
  CPPUNIT_ASSERT_EQUAL(size_t(4), fread(buf, 1, 4, f));
  fseek(f, 0, SEEK_END);
  CPPUNIT_ASSERT_EQUAL(size_t(4), static_cast<size_t>(ftell(f)));
  fclose(f);
  CPPUNIT_ASSERT_STRING_EQUAL("test", String(buf, 4));

  fclose(f2);		/* The junk file kept open */
  unlink(dst_name);
}
/* vim: set sts=2 sw=2 ts=8 et: */
