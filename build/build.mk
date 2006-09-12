WRAITH = $(top_srcdir)/lib/libwraith.a
WRAITH_UNITS = $(top_srcdir)/lib/libwraith_units.a
BDLIB = $(top_srcdir)/lib/libbdlib.a
BDLIB_UNITS = $(top_srcdir)/lib/libbdlib_units.a
CORE_UNITS = $(top_srcdir)/src/tests/core_units.a
OPENSSL = $(top_srcdir)/lib/libopenssl.a

depcomp = /bin/sh $(top_srcdir)/build/autotools/depcomp

%.o : %.cpp
#	@echo -e "[+] Compiling: \033[1m$(basename $< .cpp)\033[0m"
	@echo -e "[CC]	$<"
	source='$<' object='$@' depfile='.deps/$*.Po' tmpdepfile='.deps/$*.TPo' depmode=$(CCDEPMODE) $(depcomp) \
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o : %.c
#	@echo -e "[+] Compiling: \033[1m$(basename $< .c)\033[0m"
	@echo -e "[CC]	$<"
	source='$<' object='$@' depfile='.deps/$*.Po' tmpdepfile='.deps/$*.TPo' depmode=$(CCDEPMODE) $(depcomp) \
	$(CXX) $(CXXFLAGS) -c $< -o $@

