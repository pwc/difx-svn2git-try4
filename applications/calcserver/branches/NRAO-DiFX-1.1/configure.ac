AC_INIT()

AM_INIT_AUTOMAKE(CalcServer, 1.1, no-define)

AC_PROG_CC
AM_SANITY_CHECK

AC_LANG_C

AC_PROG_F77
AM_PROG_LIBTOOL

AC_CHECK_LIB(g2c, getenv_,,)

AC_OUTPUT(Makefile \
	startCalcServer \
	src/Makefile \
	calc9.1/Makefile \
	data/Makefile
)
