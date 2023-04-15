# Check for the existence of an AVR program (hex) loader
AC_DEFUN([AVR_CHECK_LOADER], [AC_CHECK_PROG([AVR_LOADER], [avrdude], [$as_dir/$ac_word$ac_exec_ext]) 
if test "x" = "x$AVR_LOADER" ;
then
  AC_MSG_ERROR([No AVR program loader found, try setting the AVR_LOADER variable])
fi]
)

# Check for the existence of an objcopy program begining with our host prefix.
AC_DEFUN([AVR_CHECK_OBJCOPY],
[AC_CHECK_TOOL([OBJCOPY], [objcopy], []) 
if test "x" = "x$OBJCOPY" ;
then
  AC_MSG_ERROR([objcopy not found, try setting the OBJCOPY variable])
fi]
)