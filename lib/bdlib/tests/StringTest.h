/* StringTest.h
 *
 * $Id$
 */
#ifndef STRINGTEST_H
#define STRINGTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"

class StringTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (StringTest);
    CPPUNIT_TEST (lengthTest);
    CPPUNIT_TEST (capacityTest);
    CPPUNIT_TEST (compareTest);
    CPPUNIT_TEST (equalTest);
    CPPUNIT_TEST (setTest);
    CPPUNIT_TEST (c_strTest);
    CPPUNIT_TEST (hasIndexTest);
    CPPUNIT_TEST (charAtTest);
    CPPUNIT_TEST (indexTest);
    CPPUNIT_TEST (appendTest);
    CPPUNIT_TEST (insertTest);
    CPPUNIT_TEST (replaceTest);
    CPPUNIT_TEST (concatTest);
    CPPUNIT_TEST (incDecTest);
    CPPUNIT_TEST (incDecEqualTest);
    CPPUNIT_TEST (printfTest);
    CPPUNIT_TEST (encryptTest);
    CPPUNIT_TEST (base64Test);
    CPPUNIT_TEST (encryptBase64Test);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void lengthTest(void);
        void capacityTest(void);
        void compareTest (void);
        void equalTest (void);
        void setTest (void);
        void c_strTest (void);
        void hasIndexTest(void);
        void charAtTest(void);
        void indexTest(void);
        void appendTest(void);
        void insertTest(void);
        void replaceTest(void);
        void concatTest(void);
        void incDecTest(void);
        void incDecEqualTest(void);
        void printfTest(void);
        void encryptTest(void);
        void base64Test(void);
        void encryptBase64Test(void);
    private:
        String *a, *b, *c, *d, *e, *f, *g, *h;
	char cstring[50];
};
#endif
