#ifndef SCRIPTING_HH
#define SCRIPTING_HH 1

#include <queue>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "locks.hh"
#include "labreatypes.h"
#include "script_state.hh"
#include "gen_scripting.hh"

namespace labrea {

template <typename Rv, typename Arg1, typename Arg2, typename Arg3>
void* func_cast(Rv (*f)(Arg1 a1, Arg2 a2, Arg3 a3)) {
    union {
        typeof(f) thefunc;
        void *p;
    } hack;
    hack.thefunc = f;
    return hack.p;
}

}

#endif /* SCRIPTING_HH */
