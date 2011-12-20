/* ScriptInterpTCLTest.h
 *
 */
#ifndef _SCRIPTINTERPTCLTEST_H
#define _SCRIPTINTERPTCLTEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "Array.h"
#include "Stream.h"
#include "ScriptInterp.h"
#include "ScriptInterpTCL.h"
using namespace BDLIB_NS;

class ScriptInterpTCLTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (ScriptInterpTCLTest);
    CPPUNIT_TEST (evalTest);
    CPPUNIT_TEST (loadScriptTest);
//    CPPUNIT_TEST (operatorEqualsTest);
    CPPUNIT_TEST (linkVarTest);
    CPPUNIT_TEST (unlinkVarTest);
    CPPUNIT_TEST (createCommandTest);
    CPPUNIT_TEST (createCommandEventTest);
    CPPUNIT_TEST (deleteCommandTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void evalTest(void);
        void loadScriptTest(void);
//        void operatorEqualsTest(void);
        void linkVarTest(void);
        void unlinkVarTest(void);
        void createCommandTest(void);
        void createCommandEventTest(void);
        void deleteCommandTest(void);
    private:
};
#endif /* !_SCRIPTINTERPTCLTEST_H */
