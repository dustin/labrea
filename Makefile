CFLAGS=-fpic

OSX_LDFLAGS=-MD -MP -Wl,-undefined -Wl,dynamic_lookup -dynamiclib

LDFLAGS=$(OSX_LDFLAGS)

labrea.so: labrea.o
	$(CXX) $(LDFLAGS) -o labrea.so labrea.o

clean:
	rm labrea.so labrea.o

labrea.o: labrea.cc labrea.h
