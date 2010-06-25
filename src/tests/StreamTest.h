/* StreamTest.h
 *
 */
#ifndef _STREAMTEST_H
#define _STREAMTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Stream.h"
using namespace BDLIB_NS;

class StreamTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (StreamTest);
    CPPUNIT_TEST (reserveTest);
    CPPUNIT_TEST (tellTest);
    CPPUNIT_TEST (seekTest);
    CPPUNIT_TEST (putsTest);
    CPPUNIT_TEST (truncateTest);
    CPPUNIT_TEST (getsTest);
    CPPUNIT_TEST (getlineTest);
    CPPUNIT_TEST (loadFileTest);
    CPPUNIT_TEST (writeFileFDTest);
    CPPUNIT_TEST (writeFileTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void reserveTest(void);
        void tellTest(void);
        void seekTest(void);
        void putsTest(void);
        void truncateTest(void);
        void getsTest(void);
        void getlineTest(void);
        void loadFileTest(void);
        void writeFileFDTest(void);
        void writeFileTest(void);
    private:
        Stream *a, *b, *c, *d, *e, *F, *g, *h;
	char cstring[50];
};
#endif

