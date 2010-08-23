/* ScriptInterpTest.h
 *
 */
#ifndef _SCRIPTINTERPTEST_H
#define _SCRIPTINTERPTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "ScriptInterp.h"
#include "ScriptInterpTCL.h"
using namespace BDLIB_NS;

class ScriptInterpTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (ScriptInterpTest);
    CPPUNIT_TEST (evalTest);
    CPPUNIT_TEST (operatorEqualsTest);
    CPPUNIT_TEST (linkVarTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void evalTest(void);
        void operatorEqualsTest(void);
        void linkVarTest(void);
    private:
};
#endif /* !_SCRIPTINTERPTEST_H */
