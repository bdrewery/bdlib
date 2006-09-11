depcomp = /bin/sh $(top_srcdir)/build/autotools/depcomp

%.o : %.cpp
	@echo -e "[+] Compiling: \033[1m$(basename $< .cpp)\033[0m"
	@source='$<' object='$@' depfile='.deps/$*.Po' tmpdepfile='.deps/$*.TPo' depmode=$(CCDEPMODE) $(depcomp) \
	$(CXX) $(CXXFLAGS) -c $< -o $@

