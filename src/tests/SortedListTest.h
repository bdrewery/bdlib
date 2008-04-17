/* SortedListTest.h
 *
 * $Id$
 */
#ifndef _SORTEDLISTTEST_H
#define _SORTEDLISTTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "SortedList.h"
using namespace BDLIB_NS;

class SortedListTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (SortedListTest);
    CPPUNIT_TEST (insertTest);
    CPPUNIT_TEST (containsTest);
    CPPUNIT_TEST (copyTest);
    CPPUNIT_TEST (findTest);
    CPPUNIT_TEST (clearTest);
    CPPUNIT_TEST (removeTest);
    CPPUNIT_TEST (iterateTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void insertTest(void);
        void containsTest(void);
        void copyTest(void);
        void findTest(void);
        void clearTest(void);
        void removeTest(void);
        void iterateTest(void);
    private:
        SortedList<String> *a, *b, *c, *d;
        SortedList<String*> *e, *f, *g, *h;
};
#endif /* !_SORTEDLISTTEST_H */
