#ifndef GEN_SCRIPTING_HH
#define GEN_SCRIPTING_HH 1

#include "labreatypes.h"
#include "script_state.hh"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

namespace labrea {

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

template <typename Rv, typename Arg1>
bool around_call(struct ftype *f, Arg1 a1, Rv &out) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "around_%s", f->name);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, f->pos);
        add_arg(lsh.state, a1);
        if (lua_pcall(lsh.state, 2, 1, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
        assert(lua_gettop(lsh.state) > 0);
        assert(lua_isnumber(lsh.state, 1));
        out = lua_tonumber(lsh.state, 1);
        return true;
    } else {
        return false;
    }
}

template <typename Rv, typename Arg1>
void after_call(const char *call, Rv rv,  Arg1 a1) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "after_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, rv);
        add_arg(lsh.state, a1);
        if (lua_pcall(lsh.state, 2, 0, 0)) {
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

template <typename Rv, typename Arg1, typename Arg2>
bool around_call(struct ftype *f, Arg1 a1, Arg2 a2, Rv &out) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "around_%s", f->name);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, f->pos);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        if (lua_pcall(lsh.state, 3, 1, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
        assert(lua_gettop(lsh.state) > 0);
        assert(lua_isnumber(lsh.state, 1));
        out = lua_tonumber(lsh.state, 1);
        return true;
    } else {
        return false;
    }
}

template <typename Rv, typename Arg1, typename Arg2>
void after_call(const char *call, Rv rv,  Arg1 a1, Arg2 a2) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "after_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, rv);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        if (lua_pcall(lsh.state, 3, 0, 0)) {
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

template <typename Rv, typename Arg1, typename Arg2, typename Arg3>
bool around_call(struct ftype *f, Arg1 a1, Arg2 a2, Arg3 a3, Rv &out) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "around_%s", f->name);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, f->pos);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        if (lua_pcall(lsh.state, 4, 1, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
        assert(lua_gettop(lsh.state) > 0);
        assert(lua_isnumber(lsh.state, 1));
        out = lua_tonumber(lsh.state, 1);
        return true;
    } else {
        return false;
    }
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3>
void after_call(const char *call, Rv rv,  Arg1 a1, Arg2 a2, Arg3 a3) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "after_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, rv);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        if (lua_pcall(lsh.state, 4, 0, 0)) {
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

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
bool around_call(struct ftype *f, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Rv &out) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "around_%s", f->name);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, f->pos);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        add_arg(lsh.state, a4);
        if (lua_pcall(lsh.state, 5, 1, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
        assert(lua_gettop(lsh.state) > 0);
        assert(lua_isnumber(lsh.state, 1));
        out = lua_tonumber(lsh.state, 1);
        return true;
    } else {
        return false;
    }
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void after_call(const char *call, Rv rv,  Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "after_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, rv);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        add_arg(lsh.state, a4);
        if (lua_pcall(lsh.state, 5, 0, 0)) {
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

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
bool around_call(struct ftype *f, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Rv &out) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "around_%s", f->name);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, f->pos);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        add_arg(lsh.state, a4);
        add_arg(lsh.state, a5);
        if (lua_pcall(lsh.state, 6, 1, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
        assert(lua_gettop(lsh.state) > 0);
        assert(lua_isnumber(lsh.state, 1));
        out = lua_tonumber(lsh.state, 1);
        return true;
    } else {
        return false;
    }
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void after_call(const char *call, Rv rv,  Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "after_%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, rv);
        add_arg(lsh.state, a1);
        add_arg(lsh.state, a2);
        add_arg(lsh.state, a3);
        add_arg(lsh.state, a4);
        add_arg(lsh.state, a5);
        if (lua_pcall(lsh.state, 6, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}
}

#endif /* GEN_SCRIPTING_HH */
