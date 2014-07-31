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

/* BinaryTreeTest.h
 *
 */
#ifndef _BINARYTREETEST_H
#define _BINARYTREETEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "BinaryTree.h"
using namespace BDLIB_NS;

class BinaryTreeTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (BinaryTreeTest);
    CPPUNIT_TEST (insertTest);
    CPPUNIT_TEST (containsTest);
    CPPUNIT_TEST (removeTest);
    CPPUNIT_TEST (clearTest);
    CPPUNIT_TEST (copyTest);
//    CPPUNIT_TEST (getKeyTest);
    CPPUNIT_TEST (getValueTest);
    CPPUNIT_TEST (iterateTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void insertTest(void);
        void containsTest(void);
        void clearTest(void);
        void copyTest(void);
//        void getKeyTest(void);
        void getValueTest(void);
        void removeTest(void);
        void iterateTest(void);
    private:
        BinaryTree<int, String> *a, *b, *c, *d, *e, *f, *g, *h;
};
#endif /* !_BINARYTREETEST_H */
/* vim: set sts=2 sw=2 ts=8 et: */
