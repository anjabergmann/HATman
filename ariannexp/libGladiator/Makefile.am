noinst_LTLIBRARIES = libGladiator.la
AM_CXXFLAGS= $(SDL_CFLAGS)
INCLUDES = \
	-I$(top_srcdir)/libClient

libGladiator_la_SOURCES = \
	libGladiator.cpp \
	libGladiator.h

libGladiator_la_LIBADD = \
	$(top_builddir)/libClient/libClient.la
