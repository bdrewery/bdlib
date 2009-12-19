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
    private:
        Array<String> *str_a, *str_b, *str_c;
        Array<char> *char_a, *char_b;
        Array<int> *int_a, *int_b, *int_c;
};
#endif

