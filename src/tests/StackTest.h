/* StackTest.h
 *
 * $Id$
 */
#ifndef _STACKTEST_H
#define _STACKTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "Stack.h"
using namespace BDLIB_NS;

class StackTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (StackTest);
    CPPUNIT_TEST (pushTest);
    CPPUNIT_TEST (popTest);
    CPPUNIT_TEST (iterateTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void pushTest(void);
        void popTest(void);
        void iterateTest(void);
    private:
        Stack<String> *a, *b, *c, *d;
        Stack<String*> *e, *f, *g, *h;
};
#endif /* !_STACKTEST_H */
