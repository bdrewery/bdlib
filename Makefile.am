noinst_LIBRARIES=	libbdlib.a
AM_CPPFLAGS=		@TCL_INCLUDE_SPEC@
AM_CXXFLAGS=		-Wall -W -Wno-unused-parameter -Wdisabled-optimization \
			-Wmissing-format-attribute -fstrict-aliasing \
			-Weffc++ -Woverloaded-virtual \
			@AM_CXXFLAGS@
libbdlib_a_SOURCES= \
			src/Cstrings.cpp \
			src/base64.cpp \
			src/Array.cpp \
			src/AtomicFile.cpp \
			src/BinaryTree.cpp \
			src/HashTable.cpp \
			src/List.cpp \
			src/Queue.cpp \
			src/ReferenceCountedArray.cpp \
			src/Script.cpp \
			src/ScriptInterp.cpp \
			src/ScriptInterpTCL.cpp \
			src/SortedList.cpp \
			src/Stack.cpp \
			src/Stream.cpp \
			src/String.cpp
libbdlib_a_CXXFLAGS=	$(AM_CXXFLAGS) @WDOCUMENTATION@

TESTS=			UnitRunner
check_PROGRAMS=		UnitRunner
UnitRunner_SOURCES=	src/tests/UnitRunner.cpp \
			src/tests/ArrayTest.cpp \
			src/tests/AtomicFileTest.cpp \
			src/tests/BinaryTreeTest.cpp \
			src/tests/HashTableTest.cpp \
			src/tests/ListTest.cpp \
			src/tests/SortedListTest.cpp \
			src/tests/ScriptInterpTCLTest.cpp \
			src/tests/StackTest.cpp \
			src/tests/StringTest.cpp \
			src/tests/StreamTest.cpp \
			src/tests/QueueTest.cpp

UnitRunner_CXXFLAGS=	$(AM_CXXFLAGS) @CPPUNIT_CFLAGS@
UnitRunner_LDFLAGS=	@CPPUNIT_LIBS@ @TCL_LIB_SPEC@
#UnitRunner_LDADD=	libbdlib.a
UnitRunner_LDADD=	$(libbdlib_a_OBJECTS)

.PHONY: test
test: check

# Move all-am dependency to check so that source and unit test objects
# can be built in parallel
#check-am:
#	$(MAKE) $(AM_MAKEFLAGS) $(check_PROGRAMS)
#	$(MAKE) $(AM_MAKEFLAGS) check-TESTS
#check: all-am check-am
