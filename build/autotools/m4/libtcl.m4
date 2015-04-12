AC_DEFUN([SCRIPT_TCL], [
    MY_ARG_DISABLE([script_tcl], [TCL Script])
    if  test "$enable_script_tcl" = "yes"; then
        SC_PATH_TCLCONFIG
        if test "$TCL_BIN_DIR" = "# no Tcl configs found"; then
            AC_MSG_CHECKING([for Tcl version])
      cat << EOF >&2
configure: WARNING:

  Tcl cannot be found on this system.

  Tcl is not required. ${PACKAGE_NAME} will be compiled without Tcl support. If you
  already have Tcl installed on this system, please specify the path by
  rerunning ./configure specifying --with-tcl or use --disable-script-tcl
  to hide this warning.

EOF
            enable_script_tcl="no"
        else
            SC_LOAD_TCLCONFIG
            AC_MSG_CHECKING([for Tcl version])
            AC_MSG_RESULT($TCL_VERSION)
            AC_MSG_CHECKING([for Tcl library])
            AC_MSG_RESULT($TCL_LIB_SPEC)
            AC_MSG_CHECKING([for Tcl include path])
            AC_MSG_RESULT($TCL_INCLUDE_SPEC)
	    AC_DEFINE(HAVE_LIBTCL, 1, [Define if you have support for libtcl])
        fi
    fi
])

#------------------------------------------------------------------------
# SC_PATH_TCLCONFIG --
#
#	Locate the tclConfig.sh file and perform a sanity check on
#	the Tcl compile flags
#
# Arguments:
#	none
#
# Results:
#
#	Adds the following arguments to configure:
#		--with-tcl=...
#
#	Defines the following vars:
#		TCL_BIN_DIR	Full path to the directory containing
#				the tclConfig.sh file
#------------------------------------------------------------------------

AC_DEFUN([SC_PATH_TCLCONFIG], [
    #
    # Ok, lets find the tcl configuration
    # First, look for one uninstalled.
    # the alternative search directory is invoked by --with-tcl
    #

    if test x"${no_tcl}" = x ; then
	# we reset no_tcl in case something fails here
	no_tcl=true
	AC_ARG_WITH(tcl, [  --with-tcl              directory containing tcl configuration (tclConfig.sh)], with_tclconfig=${withval})
	AC_MSG_CHECKING([for Tcl configuration])
	AC_CACHE_VAL(ac_cv_c_tclconfig,[

	    # First check to see if --with-tcl was specified.
	    if test x"${with_tclconfig}" != x ; then
		case "${with_tclconfig}" in
		    */tclConfig.sh )
			if test -f "${with_tclconfig}"; then
			    AC_MSG_WARN([--with-tcl argument should refer to directory containing tclConfig.sh, not to tclConfig.sh itself])
			    with_tclconfig="`echo "${with_tclconfig}" | sed 's!/tclConfig\.sh$!!'`"
			fi ;;
		esac
		if test -f "${with_tclconfig}/tclConfig.sh" ; then
		    ac_cv_c_tclconfig="`(cd "${with_tclconfig}"; pwd)`"
		else
		    AC_MSG_ERROR([${with_tclconfig} directory doesn't contain tclConfig.sh])
		fi
	    fi

	    # then check for a private Tcl installation
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in \
			../tcl \
			`ls -dr ../tcl[[8-9]].[[4-9]].[[0-9]]* 2>/dev/null` \
			`ls -dr ../tcl[[8-9]].[[4-9]] 2>/dev/null` \
			`ls -dr ../tcl[[8-9]].[[4-9]]* 2>/dev/null` \
			../../tcl \
			`ls -dr ../../tcl[[8-9]].[[4-9]].[[0-9]]* 2>/dev/null` \
			`ls -dr ../../tcl[[8-9]].[[4-9]] 2>/dev/null` \
			`ls -dr ../../tcl[[8-9]].[[4-9]]* 2>/dev/null` \
			../../../tcl \
			`ls -dr ../../../tcl[[8-9]].[[4-9]].[[0-9]]* 2>/dev/null` \
			`ls -dr ../../../tcl[[8-9]].[[4-9]] 2>/dev/null` \
			`ls -dr ../../../tcl[[8-9]].[[4-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tclConfig.sh" ; then
			ac_cv_c_tclconfig="`(cd $i/unix; pwd)`"
			break
		    fi
		done
	    fi

	    # on Darwin, check in Framework installation locations
	    if test "`uname -s`" = "Darwin" -a x"${ac_cv_c_tclconfig}" = x ; then
		for i in `ls -d ~/Library/Frameworks 2>/dev/null` \
			`ls -d /Library/Frameworks 2>/dev/null` \
			`ls -d /Network/Library/Frameworks 2>/dev/null` \
			`ls -d /System/Library/Frameworks 2>/dev/null` \
			; do
		    if test -f "$i/Tcl.framework/tclConfig.sh" ; then
			ac_cv_c_tclconfig="`(cd $i/Tcl.framework; pwd)`"
			break
		    fi
		done
	    fi

	    # check in a few common install locations
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in `ls -d ${libdir} 2>/dev/null` \
			`ls -d ${exec_prefix}/lib 2>/dev/null` \
			`ls -d ${prefix}/lib 2>/dev/null` \
			`ls -d /usr/local/lib 2>/dev/null` \
			`ls -d /usr/local/lib/tcl[[8-9]].[[4-9]] 2>/dev/null` \
			`ls -d /usr/contrib/lib 2>/dev/null` \
			`ls -d /usr/share/tcltk/tcl8.4 2>/dev/null` \
			`ls -d /usr/lib 2>/dev/null` \
			`ls -d /usr/lib64 2>/dev/null` \
			; do
		    if test -f "$i/tclConfig.sh" ; then
			ac_cv_c_tclconfig="`(cd $i; pwd)`"
			break
		    fi
		done
	    fi

	    # check in a few other private locations
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in \
			${srcdir}/../tcl \
			`ls -dr ${srcdir}/../tcl[[8-9]].[[4-9]].[[0-9]]* 2>/dev/null` \
			`ls -dr ${srcdir}/../tcl[[8-9]].[[4-9]] 2>/dev/null` \
			`ls -dr ${srcdir}/../tcl[[8-9]].[[4-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tclConfig.sh" ; then
		    ac_cv_c_tclconfig="`(cd $i/unix; pwd)`"
		    break
		fi
		done
	    fi
	])

	if test x"${ac_cv_c_tclconfig}" = x ; then
	    TCL_BIN_DIR="# no Tcl configs found"
	    AC_MSG_WARN([Can't find Tcl configuration definitions. Use --with-tcl to specify a directory containing tclConfig.sh])
            enable_script_tcl=no
	else
	    no_tcl=
	    TCL_BIN_DIR="${ac_cv_c_tclconfig}"
	    AC_MSG_RESULT([found ${TCL_BIN_DIR}/tclConfig.sh])
	fi
    fi
])

#------------------------------------------------------------------------
# SC_LOAD_TCLCONFIG --
#
#	Load the tclConfig.sh file
#
# Arguments:
#
#	Requires the following vars to be set:
#		TCL_BIN_DIR
#
# Results:
#
#	Subst the following vars:
#		TCL_BIN_DIR
#		TCL_SRC_DIR
#		TCL_LIB_FILE
#
#------------------------------------------------------------------------

AC_DEFUN([SC_LOAD_TCLCONFIG], [
    AC_MSG_CHECKING([for existence of ${TCL_BIN_DIR}/tclConfig.sh])

    if test -f "${TCL_BIN_DIR}/tclConfig.sh" ; then
        AC_MSG_RESULT([loading])
	. "${TCL_BIN_DIR}/tclConfig.sh"
    else
        AC_MSG_RESULT([could not find ${TCL_BIN_DIR}/tclConfig.sh])
    fi

    # eval is required to do the TCL_DBGX substitution
    eval "TCL_LIB_FILE=\"${TCL_LIB_FILE}\""
    eval "TCL_STUB_LIB_FILE=\"${TCL_STUB_LIB_FILE}\""

    # If the TCL_BIN_DIR is the build directory (not the install directory),
    # then set the common variable name to the value of the build variables.
    # For example, the variable TCL_LIB_SPEC will be set to the value
    # of TCL_BUILD_LIB_SPEC. An extension should make use of TCL_LIB_SPEC
    # instead of TCL_BUILD_LIB_SPEC since it will work with both an
    # installed and uninstalled version of Tcl.
    if test -f "${TCL_BIN_DIR}/Makefile" ; then
        TCL_LIB_SPEC="${TCL_BUILD_LIB_SPEC}"
        TCL_STUB_LIB_SPEC="${TCL_BUILD_STUB_LIB_SPEC}"
        TCL_STUB_LIB_PATH="${TCL_BUILD_STUB_LIB_PATH}"
    elif test "`uname -s`" = "Darwin"; then
	# If Tcl was built as a framework, attempt to use the libraries
	# from the framework at the given location so that linking works
	# against Tcl.framework installed in an arbitrary location.
	case ${TCL_DEFS} in
	    *TCL_FRAMEWORK*)
		if test -f "${TCL_BIN_DIR}/${TCL_LIB_FILE}"; then
		    for i in "`cd "${TCL_BIN_DIR}"; pwd`" \
			     "`cd "${TCL_BIN_DIR}"/../..; pwd`"; do
			if test "`basename "$i"`" = "${TCL_LIB_FILE}.framework"; then
			    TCL_LIB_SPEC="-F`dirname "$i" | sed -e 's/ /\\\\ /g'` -framework ${TCL_LIB_FILE}"
			    break
			fi
		    done
		fi
		if test -f "${TCL_BIN_DIR}/${TCL_STUB_LIB_FILE}"; then
		    TCL_STUB_LIB_SPEC="-L`echo "${TCL_BIN_DIR}"  | sed -e 's/ /\\\\ /g'` ${TCL_STUB_LIB_FLAG}"
		    TCL_STUB_LIB_PATH="${TCL_BIN_DIR}/${TCL_STUB_LIB_FILE}"
		fi
		;;
	esac
    fi

    # eval is required to do the TCL_DBGX substitution
    eval "TCL_LIB_FLAG=\"${TCL_LIB_FLAG}\""
    eval "TCL_LIB_SPEC=\"${TCL_LIB_SPEC}\""
    eval "TCL_STUB_LIB_FLAG=\"${TCL_STUB_LIB_FLAG}\""
    eval "TCL_STUB_LIB_SPEC=\"${TCL_STUB_LIB_SPEC}\""

    AC_SUBST(TCL_VERSION)
    AC_SUBST(TCL_PATCH_LEVEL)
    AC_SUBST(TCL_BIN_DIR)
    AC_SUBST(TCL_SRC_DIR)
    AC_SUBST(TCL_INCLUDE_SPEC)

    AC_SUBST(TCL_LIB_FILE)
    AC_SUBST(TCL_LIB_FLAG)
    AC_SUBST(TCL_LIB_SPEC)

    AC_SUBST(TCL_STUB_LIB_FILE)
    AC_SUBST(TCL_STUB_LIB_FLAG)
    AC_SUBST(TCL_STUB_LIB_SPEC)
])
