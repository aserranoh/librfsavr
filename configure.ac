
AC_PREREQ([2.65])
AC_INIT([librfsavr], [0.1], [toni.serranoh@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIRS([m4])

# Normally the default prefix (root dir for most of the package installation) is "/usr/local".
# We want the default prefix to be whatever is defined in avr_defaults.m4
# From https://github.com/mattwind/avr-autobiome
avr_DEFAULT_PREFIX

AC_PROG_CC
AM_PROG_AS
AM_PROG_AR
LT_INIT
AM_PATH_PYTHON([3.10])

# Check for the avr-ar and avr-objcopy programs
AVR_CHECK_LOADER
AVR_CHECK_OBJCOPY

# Substitute our default avr CFLAGS into AM_CFLAGS.  We do this so that we can
# create a set of default compilation flags in an "avr_defaults.m4" file and
# reuse them in all of our projects.  (See avr_defaults.m4)

if test "x$CPU_FREQ" = "x" ; then
AC_SUBST([CPU_FREQ], [avr_DEFAULT_CPUFREQ])
fi

if test "x$AVR_DEV" = "x" ; then
AC_SUBST([AVR_DEV], [avr_DEFAULT_DEV])
fi

if test "x$AVR_PROGRAMMING_BAUDS" = "x" ; then
AC_SUBST([AVR_PROGRAMMING_BAUDS], [avr_DEFAULT_PROGRAMMING_BAUDS])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 test/Makefile
])
AC_OUTPUT
