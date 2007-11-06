
all: clean aclocal autoheader autoconf clean_again


aclocal:
	@rm -rf aclocal.m4
	@mkdir build
	aclocal -I includes/

autoheader:
	@rm -rf ../config.h.in
	@echo "Autoheader..."
	@autoheader
	@echo "" >> build/config.h.in
#	@echo "/* Defines for pack features */" >> config.h.in
#	@(for define in `gsed -n -e 's/^[\+|\-] \([^ ]*\) .*/\1/ p' ../pack/pack.cfg`; do \
#	echo "#undef S_$$define" >> config.h.in; \
#	done)
	@mv -f build/config.h.in ../config.h.in
	@ls -al ../config.h.in

autoconf: 
	@rm -rf ../../configure
	@echo "Autoconf...."
	@autoconf
	@./fixconfigure.sh configure
	@mv configure ../../configure
	@chmod 700 ../../configure
	@ls -al ../../configure

clean:
	@echo "Cleaning..."
	@rm -rf autom4te.cache/ configure config.h.in aclocal.m4
	@rm -rf build

clean_again:
	@echo "Cleaning..."
	@rm -rf autom4te.cache/ configure config.h.in aclocal.m4
	@rm -rf build
