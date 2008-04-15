/* HashTableTest.h
 *
 * $Id$
 */
#ifndef _HASHTABLETEST_H
#define _HASHTABLETEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "HashTable.h"
using namespace BDLIB_NS;

class HashTableTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (HashTableTest);
    CPPUNIT_TEST (insertTest);
    CPPUNIT_TEST (containsTest);
    CPPUNIT_TEST (clearTest);
//    CPPUNIT_TEST (getKeyTest);
    CPPUNIT_TEST (getValueTest);
    CPPUNIT_TEST (removeTest);
//    CPPUNIT_TEST (iterateTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void insertTest(void);
        void containsTest(void);
        void clearTest(void);
//        void getKeyTest(void);
        void getValueTest(void);
        void removeTest(void);
//        void iterateTest(void);
    private:
        HashTable<int, String> *a, *b, *c, *d, *e, *f, *g, *h;
	int k1, k2, k3, k4;
	String v1, v2, v3, v4;
};
#endif /* !_HASHTABLETEST_H */
