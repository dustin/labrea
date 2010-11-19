#include <iostream>
#include <ios>
#include <queue>

#include <unistd.h>

#include <assert.h>

#include "labrea.h"
#include "gen_invoker.hh"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace labrea {

pthread_mutex_t luamutex = PTHREAD_MUTEX_INITIALIZER;
static lua_State *luaStateProto(NULL);
std::queue<lua_State*> stateThreads;

static int do_usleep(lua_State *ls) {
    int howlong = lua_tointeger(ls, -1);
    usleep(howlong);
    return 0;
}

static int do_invoke(lua_State *ls) {
    int n_args = lua_gettop(ls) - 1; // function is an argument.
    int offset = lua_tointeger(ls, 1);
    struct ftype fun = functions[offset];
    assert(fun.pos == offset);

    // Capture the arguments.
    union farg args[MAX_ARGS];
    for (int i = 0; i < n_args; ++i) {
        args[i].i64 = lua_tointeger(ls, i + 2); // 2 == starting point of args
    }

    lua_Integer rv;
    switch (fun.num_args) {
    case 1:
        rv = abstractInvoke(&fun, args[0]);
        break;
    case 2:
        rv = abstractInvoke(&fun, args[0], args[1]);
        break;
    case 3:
        rv = abstractInvoke(&fun, args[0], args[1], args[2]);
        break;
    case 4:
        rv = abstractInvoke(&fun, args[0], args[1], args[2], args[3]);
        break;
    case 5:
        rv = abstractInvoke(&fun, args[0], args[1], args[2], args[3], args[5]);
        break;
    }

    lua_settop(ls, 0);
    lua_pushinteger(ls, rv);
    return 1;
}

// Invoked with lock.
static class LuaStateInitializer {
public:
    LuaStateInitializer() {
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
    }
} initializer;

lua_State* getLuaState() {
    LockHolder lh(&luamutex);
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
