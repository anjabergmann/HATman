INCLUDES = \
	-I$(top_srcdir)/libClient/

AM_CXXFLAGS = $(SDL_CFLAGS)	

bin_PROGRAMS=testLibGladiator

testLibGladiator_SOURCES= \
	Test_LibGladiator.cpp \
	Test_LibGladiator.h \
	main.cpp

testLibGladiator_LDADD= \
	$(top_builddir)/libClient/libClient.la \
	$(top_builddir)/libGladiator/libGladiator.la \
	-lcppunit \
	-lSDL_net \
	-lz
