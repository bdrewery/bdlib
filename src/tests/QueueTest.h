/* QueueTest.h
 *
 * $Id$
 */
#ifndef _QUEUETEST_H
#define _QUEUETEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "Queue.h"
using namespace BDLIB_NS;

class QueueTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (QueueTest);
    CPPUNIT_TEST (enqueueTest);
    CPPUNIT_TEST (dequeueTest);
    CPPUNIT_TEST (iterateTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void enqueueTest(void);
        void dequeueTest(void);
        void iterateTest(void);
    private:
        Queue<String> *a, *b, *c, *d;
        Queue<String*> *e, *f, *g, *h;
};
#endif /* !_QUEUETEST_H */
