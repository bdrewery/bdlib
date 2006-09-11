depcomp = /bin/sh $(top_srcdir)/build/autotools/depcomp

%.o : %.cc
	@echo -e "[+] Compiling: \033[1m$(basename $< .cc)\033[0m"
	@source='$<' object='$@' depfile='.deps/$*.Po' tmpdepfile='.deps/$*.TPo' depmode=$(CCDEPMODE) $(depcomp) \
	$(CXX) $(CXXFLAGS) -c $< -o $@

