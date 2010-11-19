#include <iostream>
#include <ios>

#include <unistd.h>
#include <pthread.h>

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
static pthread_key_t lua_thread_key;
static lua_State *luaStateProto(NULL);

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

    lua_Integer rv = abstractInvoke(&fun, args);

    lua_settop(ls, 0);
    lua_pushinteger(ls, rv);
    return 1;
}

void initScriptingState() {
    if (pthread_key_create(&lua_thread_key, NULL) != 0) {
        perror("pthread_key_create");
        abort();
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
}

lua_State* getLuaState() {
    lua_State *rv = reinterpret_cast<lua_State*>(pthread_getspecific(lua_thread_key));
    if (rv == NULL) {
        rv = lua_newthread(luaStateProto);
        if (pthread_setspecific(lua_thread_key, rv) != 0) {
            perror("pthread_setspecific");
            abort();
        }
    }
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
