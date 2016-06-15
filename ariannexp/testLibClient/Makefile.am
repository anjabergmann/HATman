INCLUDES = \
	-I$(top_srcdir)/libClient/

AM_CXXFLAGS = $(SDL_CFLAGS)	

bin_PROGRAMS=testLibClient

testLibClient_SOURCES= \
	Test_Attributes.cpp \
	Test_Attributes.h \
	Test_Messages.cpp \
	Test_Messages.h \
	Test_NetworkClientManager.cpp \
	Test_NetworkClientManager.h \
	Test_RPObject.cpp \
	Test_RPObject.h \
	Test_RPSlot.cpp \
	Test_RPSlot.h \
	Test_Serializer.cpp \
	Test_Serializer.h \
	main.cpp

testLibClient_LDADD= \
	$(top_builddir)/libClient/libClient.la \
	-lcppunit \
	-lSDL_net \
	-lz
