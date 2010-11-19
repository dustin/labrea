#ifndef SCRIPT_STATE_HH
#define SCRIPT_STATE_HH 1

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

namespace labrea {

extern std::queue<lua_State*> stateThreads;

extern pthread_mutex_t luamutex;
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

}

#endif /* SCRIPT_STATE_HH */
