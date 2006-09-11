WRAITH = $(top_srcdir)/lib/libwraith.la
WRAITH_UNITS = $(top_srcdir)/lib/libwraith_units.la
BDLIB = $(top_srcdir)/lib/libbdlib.la
BDLIB_UNITS = $(top_srcdir)/lib/libbdlib_units.la
CORE_UNITS = $(top_srcdir)/src/tests/core_units.la
OPENSSL = $(top_srcdir)/lib/libopenssl.la

depcomp = /bin/sh $(top_srcdir)/build/autotools/depcomp

%.o : %.cpp
#	@echo -e "[+] Compiling: \033[1m$(basename $< .cpp)\033[0m"
	@echo -e "[CC]	$<"
	@source='$<' object='$@' depfile='.deps/$*.Po' tmpdepfile='.deps/$*.TPo' depmode=$(CCDEPMODE) $(depcomp) \
	$(CXX) $(CXXFLAGS) -c $< -o $@

