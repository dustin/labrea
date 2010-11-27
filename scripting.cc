#include <iostream>
#include <sstream>
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
pthread_t timer_thread;

static pthread_key_t lua_thread_key;
static lua_State *luaStateProto(NULL);

static int cstring_tolstring(lua_State *ls) {
    char *addr = reinterpret_cast<char *>(lua_tointeger(ls, -1));
    lua_pushstring(ls, addr);
    return 1;
}

static int do_fileno(lua_State *ls) {
    FILE *f = reinterpret_cast<FILE *>(lua_tointeger(ls, -1));
    lua_pushinteger(ls, fileno(f));
    return 1;
}

static int do_usleep(lua_State *ls) {
    int howlong = lua_tointeger(ls, -1);
    // This is mildly gross, but we want to release the lock to sleep.
    // The lock has automatic cleanup in the caller.
    AntiLockHolder alh(&luamutex);
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

    // Similar to sleep, invoking from around_advice should be done
    // without a lock.
    lua_Integer rv;
    {
        AntiLockHolder alh(&luamutex);
        rv = abstractInvoke(&fun, args);
    }

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
    {"fileno", do_fileno},
    {"invoke", do_invoke},
    {"reinit", do_reinit},
    {"set_errno", set_errno},
    {"tostring", cstring_tolstring},
    {"usleep", do_usleep},
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

static void* timerLoop(void* a) {
    for (int i = 0;; ++i) {
        sleep(1);
        LockHolder lh(&luamutex);
        lua_getglobal(luaStateProto, "labrea_periodic");
        lua_pushinteger(luaStateProto, i);
        if (lua_pcall(luaStateProto, 1, 0, 0) != 0) {
            const char *errmsg(lua_tostring(luaStateProto, -1));
            std::cerr << "Error running exit function: " << errmsg << std::endl;
        }
    }
}

static void startTimerThread() {
    pthread_attr_t attr;

    if (pthread_attr_init(&attr) != 0 ||
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("pthread attr init");
        exit(1);
    }

    if (pthread_create(&timer_thread, &attr, timerLoop, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
}

void initPthreadKeys() {
    if (pthread_key_create(&lua_thread_key, NULL) != 0) {
        perror("pthread_key_create");
        abort();
    }

    if (pthread_key_create(&invocation_state_key, labreafree) != 0) {
        perror("pthread_key_create");
        abort();
    }
}

void initScriptingState() {
    luaStateProto = lua_newstate(l_alloc, NULL);
    luaL_openlibs(luaStateProto);

    luaL_register(luaStateProto, "labrea", labrea_funcs);

    const char *path = getenv("LABREA_INIT");
    if (path != NULL) {
        int status = luaL_dofile(luaStateProto, path);
        if (status) {
            std::cerr << "Error processing labrea script from "
                      << (path == NULL ? "stdin" : path)
                      << ": " << lua_tostring(luaStateProto, -1) << std::endl;
            exit(1);
        }
    }

    initFunctions(luaStateProto);

    startTimerThread();
}

void destroyScriptingState() {
    LockHolder lh(&luamutex);
    lua_getglobal(luaStateProto, "labrea_exiting");
    if (lua_pcall(luaStateProto, 0, 0, 0) != 0) {
        const char *errmsg(lua_tostring(luaStateProto, -1));
        std::cerr << "Error running exit function: " << errmsg << std::endl;
    }
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

void add_arg(lua_State *state, const unsigned int val) {
    lua_pushinteger(state, val);
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
