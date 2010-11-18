#include <iostream>
#include <ios>
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

static void* pointer_cast(lua_Integer n) {
    union {
        lua_Integer n;
        void* p;
    } hack;
    hack.n = n;
    return hack.p;
}

static int do_invoke(lua_State *ls) {
    int n_args = lua_gettop(ls) - 1; // function is an argument.
    assert(n_args == 3); // only supported value currently.
    const char *name = lua_tostring(ls, 1);
    int (*f)(int, void*, int) = reinterpret_cast<typeof(f)>(dlsym(RTLD_NEXT, name));

    int a1 = lua_tointeger(ls, 2);
    void *a2 = pointer_cast(lua_tointeger(ls, 3));
    int a3 = lua_tointeger(ls, 4);
    lua_settop(ls, 0);
    int rv = f(a1, a2, a3);
    lua_settop(ls, 0);
    lua_pushinteger(ls, rv);
    return 1;
}

// Invoked with lock.
static void initLuaState() {
    if (initialized) {
        return;
    }
    luaStateProto = luaL_newstate();
    luaL_openlibs(luaStateProto);
    lua_register (luaStateProto, "usleep", do_usleep);
    lua_register (luaStateProto, "invoke", do_invoke);
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
        uint64_t i;
    } hack;
    hack.p = val;
    lua_pushinteger(state, hack.i);
}

}
