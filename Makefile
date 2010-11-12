LINUX_LDFLAGS=-shared -Wl,-soname,labrea.so -ldl
OSX_LDFLAGS=-MD -MP -Wl,-undefined -Wl,dynamic_lookup -dynamiclib

# LDFLAGS=$(OSX_LDFLAGS)
LDFLAGS=$(LINUX_LDFLAGS)

.PHONY: lua

LUA=lua-5.1.4

CXXFLAGS=-fPIC -Wall -g -I$(LUA)/src

labrea.so: lua labrea.o scripting.o
	$(CXX) $(LDFLAGS) -o labrea.so labrea.o scripting.o $(LUA)/src/*.o

lua:
	cd $(LUA) && make MYCFLAGS=-fPIC posix
	rm $(LUA)/src/lua.o $(LUA)/src/luac.o

clean:
	rm labrea.so labrea.o

labrea.o: labrea.cc labrea.h locks.hh scripting.hh
scripting.o: scripting.cc labrea.h locks.hh scripting.hh

