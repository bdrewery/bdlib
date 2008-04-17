/* OrderedListTest.h
 *
 * $Id$
 */
#ifndef _OrderedListTEST_H
#define _OrderedListTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "OrderedList.h"
using namespace BDLIB_NS;

class OrderedListTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (OrderedListTest);
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
        OrderedList<String> *a, *b, *c, *d;
        OrderedList<String*> *e, *f, *g, *h;
};
#endif /* !_OrderedListTEST_H */
