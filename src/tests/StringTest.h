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
    CPPUNIT_TEST (newsplitTest);
    CPPUNIT_TEST (concatTest);
    CPPUNIT_TEST (incDecTest);
    CPPUNIT_TEST (incDecEqualTest);
    CPPUNIT_TEST (operatorStarTest);
    CPPUNIT_TEST (resizeTest);
	CPPUNIT_TEST (printfTest);
//    CPPUNIT_TEST (encryptTest);
    CPPUNIT_TEST (base64Test);
//    CPPUNIT_TEST (encryptBase64Test);
    CPPUNIT_TEST (substringTest);
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
        void newsplitTest(void);
        void resizeTest(void);
        void concatTest(void);
        void incDecTest(void);
        void incDecEqualTest(void);
        void operatorStarTest(void);
        void printfTest(void);
//        void encryptTest(void);
        void base64Test(void);
//        void encryptBase64Test(void);
        void substringTest(void);
        void splitTest(void);
        void hashTest(void);
        void substTest(void);
    private:
        String *a, *b, *c, *d, *e, *f, *g, *h;
	char cstring[50];
};
#endif
