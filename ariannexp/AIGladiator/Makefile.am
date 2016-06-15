INCLUDES = \
	-I$(top_srcdir)/libClient/

AM_CXXFLAGS = $(SDL_CFLAGS)	

bin_PROGRAMS=AIGladiator

AIGladiator_SOURCES= \
	AIGladiator.cpp

AIGladiator_LDADD= \
	$(top_builddir)/libClient/libClient.la \
	$(top_builddir)/libGladiator/libGladiator.la \
	$(SDL_LIBS) \
	$(SDLnet_LIBS)
