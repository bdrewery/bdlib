/* @FILE@Test.h
 *
 * $Id: $
 */
#ifndef _@FILECAP@TEST_H
#define _@FILECAP@TEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "@FILE@.h"
using namespace BDLIB_NS;

class @FILE@Test : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (@FILE@Test);
//    CPPUNIT_TEST (insertTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
//        void insertTest(void);
    private:
        @FILE@<String> *a, *b, *c, *d;
        @FILE@<String*> *e, *f, *g, *h;
};
#endif /* !_@FILECAP@TEST_H */
