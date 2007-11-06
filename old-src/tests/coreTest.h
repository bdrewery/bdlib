/* coreTest.h
 *
 * $Id$
 */
#ifndef CORETEST_H
#define CORETEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <core.h>

class coreTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (coreTest);
    CPPUNIT_TEST (getBinnameTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void getBinnameTest(void);
    private:
	Core *a, *b, *c;
};
#endif
