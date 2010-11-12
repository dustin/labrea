LINUX_LDFLAGS=-shared -Wl,-soname,labrea.so -ldl
MAC_LDFLAGS=-MD -MP -Wl,-undefined -Wl,dynamic_lookup -dynamiclib

PLATS=linux mac
.PHONY: lua $(PLATS)

LUA=lua-5.1.4
LUATARGET=posix
LUACFLAGS=-fPIC

CXXFLAGS=-fPIC -Wall -g -I$(LUA)/src

default:
	make `uname -s`

labrea.so: lua labrea.o scripting.o
	$(CXX) $(LDFLAGS) -o labrea.so labrea.o scripting.o $(LUA)/src/*.o

lua:
	cd $(LUA)/src && make MORECFLAGS="$(LUACFLAGS)" $(LUATARGET)
	rm $(LUA)/src/lua.o $(LUA)/src/luac.o

clean:
	rm -f labrea.so labrea.o scripting.o
	cd $(LUA) && make clean

linux:
	make LDFLAGS="$(LINUX_LDFLAGS)" labrea.so

mac:
	make LDFLAGS="$(MAC_LDFLAGS)" labrea.so

# Uname -> target mappings.
Linux: linux
Darwin: mac

labrea.o: labrea.cc labrea.h locks.hh scripting.hh
scripting.o: scripting.cc labrea.h locks.hh scripting.hh

