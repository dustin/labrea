#include <iostream>
#include <queue>

#include <unistd.h>

#include <assert.h>

#include "labrea.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace labrea {

pthread_mutex_t luamutex = PTHREAD_MUTEX_INITIALIZER;
static lua_State *luaStateProto(NULL);
std::queue<lua_State*> stateThreads;
static bool initialized(false);

static int do_usleep(lua_State *ls) {
    int howlong = lua_tointeger(ls, -1);
    usleep(howlong);
    return 0;
}

// Invoked with lock.
static void initLuaState() {
    if (initialized) {
        return;
    }
    luaStateProto = luaL_newstate();
    luaL_openlibs(luaStateProto);
    lua_register (luaStateProto, "usleep", do_usleep);
    const char *path = getenv("LABREA_SCRIPT");
    if (path == NULL) {
        std::cerr << "No LABREA_SCRIPT set, taking it from stdin." << std::endl;
    }
    int status = luaL_dofile(luaStateProto, path);
    if (status) {
        std::cerr << "Error processing labrea script from "
                  << (path == NULL ? "stdin" : path)
                  << ": " << lua_tostring(luaStateProto, -1) << std::endl;
        exit(1);
    }
    initialized = true;
}

lua_State* getLuaState() {
    LockHolder lh(&luamutex);
    initLuaState();
    if (stateThreads.empty()) {
        stateThreads.push(lua_newthread(luaStateProto));
    }
    lua_State *rv = stateThreads.front();
    stateThreads.pop();
    return rv;
}

void add_arg(lua_State *state, const int val) {
    lua_pushinteger(state, val);
}

void add_arg(lua_State *state, const long val) {
    lua_pushinteger(state, static_cast<int64_t>(val));
}

void add_arg(lua_State *state, const long unsigned val) {
    lua_pushinteger(state, static_cast<uint64_t>(val));
}

void add_arg(lua_State *state, const long long val) {
    lua_pushinteger(state, static_cast<int64_t>(val));
}

void add_arg(lua_State *state, const unsigned long long val) {
    lua_pushinteger(state, static_cast<uint64_t>(val));
}

void add_arg(lua_State *state, const void* val) {
    union {
        const void* p;
        ptrdiff_t i;
    } hack;
    hack.p = val;
    // loss
    add_arg(state, hack.i);
}

}
