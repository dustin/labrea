#ifndef SCRIPTING_HH
#define SCRIPTING_HH 1

#include <queue>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "locks.hh"

extern pthread_mutex_t luamutex;
extern std::queue<lua_State*> stateThreads;

lua_State* getLuaState();

class LuaStateHolder {
public:
    LuaStateHolder(lua_State *ls) : state(ls) {
        assert(state);
    }
    ~LuaStateHolder() {
        LockHolder lh(&luamutex);
        stateThreads.push(state);
    }

    lua_State *state;
};

void add_arg(lua_State *state, const int val);
void add_arg(lua_State *state, const long val);
void add_arg(lua_State *state, const long unsigned val);
void add_arg(lua_State *state, const long long val);
void add_arg(lua_State *state, const unsigned long long val);
void add_arg(lua_State *state, const void* val);

template <typename Arg1>
void before_call(const char *call, Arg1 a1) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "before_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, a1);
        if (lua_pcall(lsh.state, 1, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}

template <typename Arg1, typename Arg2>
void before_call(const char *call, Arg1 a1, Arg2 a2) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "before_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        if (lua_pcall(lsh.state, 2, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}

template <typename Arg1, typename Arg2, typename Arg3>
void before_call(const char *call, Arg1 a1, Arg2 a2, Arg3 a3) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "before_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        if (lua_pcall(lsh.state, 3, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}

template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void before_call(const char *call, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "before_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        add_arg(lsh.state, a4);
        if (lua_pcall(lsh.state, 4, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}

template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void before_call(const char *call, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "before_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        add_arg(lsh.state, a4);
        add_arg(lsh.state, a5);
        if (lua_pcall(lsh.state, 5, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}

#endif /* SCRIPTING_HH */
