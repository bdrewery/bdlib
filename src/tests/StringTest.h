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

/* StringTest.h
 *
 */
#ifndef STRINGTEST_H
#define STRINGTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
using namespace BDLIB_NS;

class StringTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (StringTest);
    CPPUNIT_TEST (lengthTest);
    CPPUNIT_TEST (clearTest);
    CPPUNIT_TEST (capacityTest);
    CPPUNIT_TEST (refTest);
    CPPUNIT_TEST (swapTest);
    CPPUNIT_TEST (compareTest);
    CPPUNIT_TEST (equalTest);
    CPPUNIT_TEST (setTest);
    CPPUNIT_TEST (c_strTest);
    CPPUNIT_TEST (hasIndexTest);
    CPPUNIT_TEST (charAtTest);
    CPPUNIT_TEST (indexTest);
    CPPUNIT_TEST (iteratorTest);
    CPPUNIT_TEST (stl_iteratorTest);
    CPPUNIT_TEST (stl_reverse_iteratorTest);
    CPPUNIT_TEST (stl_const_iteratorTest);
    CPPUNIT_TEST (appendTest);
    CPPUNIT_TEST (insertTest);
    CPPUNIT_TEST (replaceTest);
    CPPUNIT_TEST (chompTest);
    CPPUNIT_TEST (trimTest);
    CPPUNIT_TEST (findTest);
    CPPUNIT_TEST (rfindTest);
    CPPUNIT_TEST (ifindTest);
    CPPUNIT_TEST (rifindTest);
    CPPUNIT_TEST (newsplitTest);
    CPPUNIT_TEST (concatTest);
    CPPUNIT_TEST (incDecTest);
    CPPUNIT_TEST (incDecEqualTest);
    CPPUNIT_TEST (initializerTest);
    CPPUNIT_TEST (operatorStarTest);
    CPPUNIT_TEST (resizeTest);
    CPPUNIT_TEST (copyTest);
	CPPUNIT_TEST (printfTest);
//    CPPUNIT_TEST (encryptTest);
    CPPUNIT_TEST (base64Test);
//    CPPUNIT_TEST (encryptBase64Test);
    CPPUNIT_TEST (substringTest);
    CPPUNIT_TEST (substringOutOfRangeResizeTest);
    CPPUNIT_TEST (substringOutOfRangeResizeSubTest);
    CPPUNIT_TEST (substringOutOfRangeInsertTest);
    CPPUNIT_TEST (splitTest);
    CPPUNIT_TEST (hashTest);
    CPPUNIT_TEST (substTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void lengthTest(void);
        void clearTest(void);
        void capacityTest(void);
        void compareTest (void);
        void refTest(void);
        void swapTest(void);
        void equalTest (void);
        void setTest (void);
        void c_strTest (void);
        void hasIndexTest(void);
        void charAtTest(void);
        void indexTest(void);
        void iteratorTest(void);
        void stl_iteratorTest(void);
        void stl_reverse_iteratorTest(void);
        void stl_const_iteratorTest(void);
        void appendTest(void);
        void insertTest(void);
        void replaceTest(void);
        void chompTest(void);
        void trimTest(void);
        void rfindTest(void);
        void findTest(void);
        void rifindTest(void);
        void ifindTest(void);
        void newsplitTest(void);
        void resizeTest(void);
        void concatTest(void);
        void incDecTest(void);
        void incDecEqualTest(void);
        void initializerTest(void);
        void operatorStarTest(void);
        void copyTest(void);
        void printfTest(void);
//        void encryptTest(void);
        void base64Test(void);
//        void encryptBase64Test(void);
        void substringTest(void);
        void substringOutOfRangeResizeTest(void);
        void substringOutOfRangeResizeSubTest(void);
        void substringOutOfRangeInsertTest(void);
        void splitTest(void);
        void hashTest(void);
        void substTest(void);
    private:
        String *a = nullptr, *b = nullptr, *c = nullptr, *d = nullptr, *e = nullptr, *f = nullptr, *g = nullptr, *h = nullptr;
	char cstring[50];
};
#endif
/* vim: set sts=2 sw=2 ts=8 et: */
