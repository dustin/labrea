#ifndef SCRIPT_STATE_HH
#define SCRIPT_STATE_HH 1

#include <pthread.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

namespace labrea {

extern pthread_mutex_t luamutex;
extern pthread_key_t invocation_state_key;

lua_State* getLuaState();

class LuaStateHolder {
public:
    LuaStateHolder(lua_State *ls) : state(ls) {
        assert(state);
    }
    ~LuaStateHolder() {
    }

    lua_State *state;
};

class LuaInvocation {
public:
    LuaInvocation() : ready(false), pos(NULL) {
        pos = reinterpret_cast<bool*>(pthread_getspecific(invocation_state_key));
        if (pos == NULL) {
            pos = static_cast<bool*>(calloc(1, sizeof(bool)));
            if (pthread_setspecific(invocation_state_key, pos) != 0) {
                perror("pthread_setspecific");
                abort();
            }
        }
        ready = *pos == false;
        if (ready) {
            *pos = true;
        }
    }

    ~LuaInvocation() {
        if (ready) {
            *pos = false;
        }
    }

    bool ready;
private:
    bool *pos;
};

}

#endif /* SCRIPT_STATE_HH */
