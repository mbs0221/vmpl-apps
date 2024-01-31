#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([vmpl-apps], [1.0], [your-email@example.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Enable static linking
AC_ENABLE_STATIC

AC_PROG_CC([musl-gcc])

# Specify the desired archiver program with options
# ... 其他配置选项 ...
AR=ar

# Checks for programs.
AC_PROG_CC
AM_PROG_AR
AM_PROG_AS

# ... 其他配置选项 ...

LT_INIT([static, shared])
# ... 其他配置选项 ...

# Set installation prefix
AC_PREFIX_DEFAULT([/usr/local/musl])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for libraries.
# TODO: Add library checks if needed.

# Checks for typedefs, structures, and compiler characteristics.
# TODO: Add additional checks if needed.

# Checks for library functions.
# TODO: Add additional checks if needed.

AC_CONFIG_FILES([Makefile
                 sandbox/Makefile
                 ufirewall/Makefile
                 wedge/Makefile])
AC_OUTPUT
