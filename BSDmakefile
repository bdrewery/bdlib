.PHONY: all check_gmake debug static dynamic clean distclean test test.quick test.verbose

all: check_gmake
	@gmake
check_gmake:
	@which gmake > /dev/null 2>&1 || (echo "Please install gmake" && exit 0)
debug: check_gmake
	@gmake debug
static: check_gmake
	@gmake static
dynamic: check_gmake
	@gmake dynamic
clean: check_gmake
	@gmake clean
distclean: check_gmake
	@gmake distclean
test:
	@gmake test
test.quick:
	@gmake test.quick
test.verbose:
	@gmake test.verbose
