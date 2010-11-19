#!/usr/bin/env python

import os
import sys

def gen_calls(out, arity):
    typenames = ', '.join(['typename Arg%d' % (i+1) for i in range(arity)])
    callsig = ', '.join(['Arg%d a%d' % (i+1, i+1) for i in range(arity)])
    out.write("""
template <%(typenames)s>
void before_call(const char *call, %(callsig)s) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "before_%%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
""" % {'typenames': typenames,
       'callsig': callsig})
    for i in range(arity):
        out.write("        add_arg(lsh.state, a%d);\n" % (i+1))
    out.write("""        if (lua_pcall(lsh.state, %d, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}
""" % arity)

    out.write("""
template <typename Rv, %(typenames)s>
void around_call(struct ftype *f, %(callsig)s, Rv &out) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "around_%%s", f->name);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    add_arg(lsh.state, f->pos);
""" % {'typenames': typenames, 'callsig': callsig})
    for i in range(arity):
        out.write("    add_arg(lsh.state, a%d);\n" % (i+1))
    out.write("""    if (lua_pcall(lsh.state, %d, 1, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    assert(lua_gettop(lsh.state) > 0);
    assert(lua_isnumber(lsh.state, 1));
    out = lua_tointeger(lsh.state, 1);
}
""" % (arity + 1))

    out.write("""
template <typename Rv, %(typenames)s>
void after_call(const char *call, Rv rv,  %(callsig)s) {
    char fname[32];
    snprintf(fname, sizeof(fname)-1, "after_%%s", call);
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, fname);
    if (!lua_isnil(lsh.state, -1)) {
        add_arg(lsh.state, rv);
""" % {'typenames': typenames, 'callsig': callsig})
    for i in range(arity):
        out.write("        add_arg(lsh.state, a%d);\n" % (i+1))
    out.write("""        if (lua_pcall(lsh.state, %d, 0, 0)) {
            std::cerr << "Error invoking " << fname << ": "
                      << lua_tostring(lsh.state, -1) << std::endl;
        }
    }
}
""" % (arity+1))

if __name__ == '__main__':
    out = file("gen_scripting.hh.tmp", "w")

    out.write("""#ifndef GEN_SCRIPTING_HH
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
""")
    for i in range(1, 6):
        gen_calls(out, i)

    out.write("""}

#endif /* GEN_SCRIPTING_HH */\n""")

    out.close()
    os.rename("gen_scripting.hh.tmp", 'gen_scripting.hh')
