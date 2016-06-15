EXTRA_DIST = arianne.i "API Description.txt" libFramework.vcproj test.py chatClient.py

SUBDIRS = pyarianne
CLEANFILES = arianne_wrap.cxx arianne_wrap_python.cpp arianne.py pyarianne/_arianne.so pyarianne/arianne.py

all-am: arianne.py arianne.so complete

bin_PROGRAMS = arianne.so 
AM_CXXFLAGS= -shared $(SDL_CFLAGS) $(PYTHON_INCLUDES)

complete: arianne.so
	mv arianne.so pyarianne/_arianne.so
	mv arianne.py pyarianne 

arianne_so_SOURCES = \
	libFramework.cpp \
	libFramework.h \
	arianne_wrap_python.cpp

arianne_wrap_python.cpp: arianne.i
	$(SWIG) -python -c++ -o $@ $<

arianne.py: arianne.i
	$(SWIG) -python -c++ $<

arianne_so_LDADD=\
	$(top_builddir)/libClient/libClient.la \
	$(SDL_LIBS) -lSDL_net
