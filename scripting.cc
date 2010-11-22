#include <iostream>
#include <ios>

#include <unistd.h>
#include <pthread.h>
#include <errno.h>

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
pthread_key_t invocation_state_key;

static pthread_key_t lua_thread_key;
static lua_State *luaStateProto(NULL);

static int cstring_tolstring(lua_State *ls) {
    char *addr = reinterpret_cast<char *>(lua_tointeger(ls, -1));
    lua_pushstring(ls, addr);
    return 1;
}

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

static int set_errno(lua_State *ls) {
    errno = lua_tointeger(ls, -1);
    return 0;
}

static bool hasFunction(lua_State *state, const char *fname) {
    lua_settop(state, 0);
    lua_getfield(state, LUA_GLOBALSINDEX, fname);
    return !lua_isnil(state, -1);
}

static void initFunctions(lua_State *state) {
    char fname[64];
    for (size_t i = 0; functions[i].name; ++i) {
        snprintf(fname, sizeof(fname)-1, "before_%s", functions[i].name);
        functions[i].has_before = hasFunction(state, fname);
        snprintf(fname, sizeof(fname)-1, "around_%s", functions[i].name);
        functions[i].has_around = hasFunction(state, fname);
        snprintf(fname, sizeof(fname)-1, "after_%s", functions[i].name);
        functions[i].has_after = hasFunction(state, fname);
    }
}

static int do_reinit(lua_State *s) {
    LockHolder lh(&luamutex);
    initFunctions(luaStateProto);
    return 0;
}

static const luaL_Reg labrea_funcs[] = {
    {"usleep", do_usleep},
    {"invoke", do_invoke},
    {"set_errno", set_errno},
    {"reinit", do_reinit},
    {"tostring", cstring_tolstring},
    {NULL, NULL}
};

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)ud; (void)osize;

    if (nsize == 0) {
        labreafree(ptr);
        return NULL;
    } else {
        return labrearealloc(ptr, nsize);
    }
}

void initScriptingState() {
    if (pthread_key_create(&lua_thread_key, NULL) != 0) {
        perror("pthread_key_create");
        abort();
    }

    if (pthread_key_create(&invocation_state_key, labreafree) != 0) {
        perror("pthread_key_create");
        abort();
    }

    luaStateProto = lua_newstate(l_alloc, NULL);
    luaL_openlibs(luaStateProto);

    luaL_register(luaStateProto, "labrea", labrea_funcs);

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

    initFunctions(luaStateProto);
}

lua_State* getLuaState() {
    lua_State *rv = reinterpret_cast<lua_State*>(pthread_getspecific(lua_thread_key));
    if (rv == NULL) {
        LockHolder lh(&luamutex);
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
