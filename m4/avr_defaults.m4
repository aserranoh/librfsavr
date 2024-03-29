dnl Our default CPU fequency
AC_DEFUN([avr_DEFAULT_CPUFREQ], [-DF_CPU=16000000UL])

dnl This is where you set your LOCAL root AVR directory
AC_DEFUN([avr_LOCAL], [/usr/local/avr])

dnl This sets the prefix (default install root) for any package that calls avr_PREFIX_DEFAULT in
dnl configure.ac to avr_LOCAL.  You can override this default prefix pretty much anywhere
dnl in the configure/make/install process, this is just a default if nothing is supplied
AC_DEFUN([avr_DEFAULT_PREFIX], [AC_PREFIX_DEFAULT([avr_LOCAL])])

AC_DEFUN([avr_DEFAULT_DEV], [/dev/ttyACM0])

AC_DEFUN([avr_DEFAULT_PROGRAMMING_BAUDS], [115200])