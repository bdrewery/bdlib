BDLIB = $(top_srcdir)/src/libbdlib.a
BDLIB_UNITS = $(top_srcdir)/src/libbdlib_units.a

depcomp = /bin/sh $(top_srcdir)/build/autotools/depcomp

.SUFFIXES:
.SUFFIXES: .c .cpp .h .o

.cpp.o:
	@echo -e "[CC]	$<"
	if [ "$(CCDEPMODE)" = "gcc3" ]; then \
		if $(CXX) $(CPPFLAGS) -MT '$@' -MD -MP -MF '.deps/$*.TPo' $(CXXFLAGS) -c $< -o $@; then \
		mv '.deps/$*.TPo' '.deps/$*.Po'; \
		else rm -f '.deps/$*.Tpo'; exit 1; \
		fi; \
	else \
		source='$<' object='$@' depfile='.deps/$*.Po' tmpdepfile='.deps/$*.TPo' depmode=$(CCDEPMODE) $(depcomp) \
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@; \
	fi;

.c.o:
	@echo -e "[C ]	$<"
	if [ "$(CCDEPMODE)" = "gcc3" ]; then \
		if $(CXX) $(CPPFLAGS) -MT '$@' -MD -MP -MF '.deps/$*.TPo' $(CXXFLAGS) -c $< -o $@; then \
		mv '.deps/$*.TPo' '.deps/$*.Po'; \
		else rm -f '.deps/$*.Tpo'; exit 1; \
		fi; \
	else \
		source='$<' object='$@' depfile='.deps/$*.Po' tmpdepfile='.deps/$*.TPo' depmode=$(CCDEPMODE) $(depcomp) \
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@; \
	fi;
