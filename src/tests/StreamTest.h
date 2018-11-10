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

/* StreamTest.h
 *
 */
#ifndef _STREAMTEST_H
#define _STREAMTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Stream.h"
using namespace BDLIB_NS;

class StreamTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (StreamTest);
    CPPUNIT_TEST (reserveTest);
    CPPUNIT_TEST (tellTest);
    CPPUNIT_TEST (seekTest);
    CPPUNIT_TEST (putsTest);
    CPPUNIT_TEST (truncateTest);
    CPPUNIT_TEST (readTest);
    CPPUNIT_TEST (getlineTest);
    CPPUNIT_TEST (loadFileTest);
    CPPUNIT_TEST (writeFileFDTest);
    CPPUNIT_TEST (writeFileTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void reserveTest(void);
        void tellTest(void);
        void seekTest(void);
        void putsTest(void);
        void truncateTest(void);
        void readTest(void);
        void getlineTest(void);
        void loadFileTest(void);
        void writeFileFDTest(void);
        void writeFileTest(void);
    private:
        Stream *a = nullptr, *b = nullptr, *c = nullptr, *d = nullptr, *e = nullptr, *F = nullptr, *g = nullptr, *h = nullptr;
	char cstring[50];
};
#endif

/* vim: set sts=2 sw=2 ts=8 et: */
