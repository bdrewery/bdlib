/* BinaryTreeTest.h
 *
 * $Id$
 */
#ifndef _BINARYTREETEST_H
#define _BINARYTREETEST_H 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "String.h"
#include "BinaryTree.h"
using namespace BDLIB_NS;

class BinaryTreeTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (BinaryTreeTest);
    CPPUNIT_TEST (insertTest);
    CPPUNIT_TEST (containsTest);
    CPPUNIT_TEST (removeTest);
    CPPUNIT_TEST (clearTest);
    CPPUNIT_TEST (copyTest);
//    CPPUNIT_TEST (getKeyTest);
    CPPUNIT_TEST (getValueTest);
    CPPUNIT_TEST (iterateTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void insertTest(void);
        void containsTest(void);
        void clearTest(void);
        void copyTest(void);
//        void getKeyTest(void);
        void getValueTest(void);
        void removeTest(void);
        void iterateTest(void);
    private:
        BinaryTree<int, String> *a, *b, *c, *d, *e, *f, *g, *h;
	int k1, k2, k3, k4;
	String v1, v2, v3, v4;
};
#endif /* !_BINARYTREETEST_H */
