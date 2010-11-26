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

void initScriptingState();
void destroyScriptingState();

}

#endif /* SCRIPTING_HH */
