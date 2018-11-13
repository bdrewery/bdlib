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

/* ArrayTest.h
 *
 */
#ifndef _ARRAYTEST_H
#define _ARRAYTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Array.h"
#include "String.h"
using namespace BDLIB_NS;

class ArrayTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (ArrayTest);
    CPPUNIT_TEST (sizeTest);
    CPPUNIT_TEST (indexTest);
    CPPUNIT_TEST (push_popTest);
    CPPUNIT_TEST (arrayConsTest);
    CPPUNIT_TEST (clearTest);
    CPPUNIT_TEST (refTest);
    CPPUNIT_TEST (joinTest);
    CPPUNIT_TEST (compareTest);
//    CPPUNIT_TEST (sortTest);
//    CPPUNIT_TEST (queueTest);
    CPPUNIT_TEST (findTest);
    CPPUNIT_TEST (subArrayTest);
    CPPUNIT_TEST (hashTest);
    CPPUNIT_TEST (resizeTest);
    CPPUNIT_TEST (operatorsTest);
    CPPUNIT_TEST (initializerTest);
    CPPUNIT_TEST (iteratorTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void sizeTest(void);
        void indexTest(void);
        void push_popTest(void);
        void arrayConsTest(void);
        void clearTest(void);
        void refTest(void);
        void joinTest(void);
        void compareTest(void);
//        void sortTest(void);
//        void queueTest(void);
        void findTest(void);
        void subArrayTest(void);
        void hashTest(void);
        void resizeTest(void);
        void operatorsTest(void);
        void initializerTest(void);
        void iteratorTest(void);
    private:
        Array<String> *str_a = nullptr, *str_b = nullptr, *str_c = nullptr;
        Array<char> *char_a = nullptr, *char_b = nullptr;
        Array<int> *int_a = nullptr, *int_b = nullptr, *int_c = nullptr, *int_d = nullptr;
};
#endif

/* vim: set sts=2 sw=2 ts=8 et: */
