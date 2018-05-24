#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gnu-hello-world], [0.7], [48721262935pl@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for gettext
AM_GNU_GETTEXT_VERSION([0.19.8])
AM_GNU_GETTEXT([external])
GETTEXT_PACKAGE=gnu_hello_world
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Defining of GETTEXT_PACKAGE])
AC_SUBST(GETTEXT_PACKAGE)

# Checks for pkg-config libs
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([GTK], [gtk+-3.0])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile po/Makefile.in src/Makefile icons/Makefile data/Makefile])
AC_OUTPUT
