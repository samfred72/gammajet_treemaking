AC_INIT(timingana,[1.00])
AC_CONFIG_SRCDIR([configure.ac])

AM_INIT_AUTOMAKE

AC_PROG_CXX(CC g++)
LT_INIT([disable-static])

case $CXX in
 clang++)
  CXXFLAGS="$CXXFLAGS -Wall -Wextra"
 ;;
 *g++)
  CXXFLAGS="$CXXFLAGS -Wall -Wno-deprecated-declarations -Wextra"
 ;;
esac

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
