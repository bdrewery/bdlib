noinst_LIBRARIES=	libbdlib.a
AM_CPPFLAGS=		@TCL_INCLUDE_SPEC@
AM_CXXFLAGS=		-Wall -pipe -Wno-narrowing @AM_CXXFLAGS@ @DEBUG_CXXFLAGS@ \
			-include src/stdio-gets-hack.h
libbdlib_a_SOURCES= \
			src/Cstrings.cpp \
			src/base64.cpp \
			src/AtomicFile.cpp \
			src/ReferenceCountedArray.cpp \
			src/ScriptInterpTCL.cpp \
			src/Stream.cpp \
			src/String.cpp
libbdlib_a_CXXFLAGS=	$(AM_CXXFLAGS)

TESTS=			UnitRunner
check_PROGRAMS=		UnitRunner
UnitRunner_SOURCES=	src/tests/UnitRunner.cpp \
			src/tests/ArrayTest.cpp \
			src/tests/AtomicFileTest.cpp \
			src/tests/HashTableTest.cpp \
			src/tests/ScriptInterpTCLTest.cpp \
			src/tests/StringTest.cpp \
			src/tests/StreamTest.cpp

UnitRunner_CXXFLAGS=	$(AM_CXXFLAGS) -Wno-effc++ -Wno-overloaded-virtual \
			-Wno-unused-value \
			@CPPUNIT_CFLAGS@ @CPPUNIT_CXXFLAGS@
#UnitRunner_LDADD=	libbdlib.a
UnitRunner_LDADD=	$(libbdlib_a_OBJECTS) @CPPUNIT_LIBS@ @TCL_LIB_SPEC@

clean-local:
	-find $(top_builddir) \( -name "*.gcda" -o -name "*.gcno" -o \
	  -name "*.gcov" \) -delete

.PHONY: fix-gcov-names
fix-gcov-names:
	@find $(top_builddir) -name '*.gcda' -o -name '*.gcno' | \
	  while read file; do \
	    dir=$${file%/*}; \
	    fname=$${file##*/}; \
	    fname=$${fname##*-}; \
	    mv -v "$${file}" "$${dir}/$${fname%.*}.$${fname##*.}"; \
	  done

.PHONY: test
test: check

# Move all-am dependency to check so that source and unit test objects
# can be built in parallel
#check-am:
#	$(MAKE) $(AM_MAKEFLAGS) $(check_PROGRAMS)
#	$(MAKE) $(AM_MAKEFLAGS) check-TESTS
#check: all-am check-am

DISTCLEANFILES= $(top_builddir)/build/config.h
