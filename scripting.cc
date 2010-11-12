#include <iostream>
#include <queue>

#include <unistd.h>

#include <assert.h>

#include "labrea.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static pthread_mutex_t luamutex = PTHREAD_MUTEX_INITIALIZER;
static lua_State *luaStateProto(NULL);
static std::queue<lua_State*> stateThreads;
static bool initialized(false);

static int do_usleep(lua_State *ls) {
    int howlong = lua_tointeger(ls, -1);
    usleep(howlong);
    return 0;
}

// Invoked with lock.
static void initLuaState() {
    if (initialized) {
        return;
    }
    luaStateProto = luaL_newstate();
    luaL_openlibs(luaStateProto);
    lua_register (luaStateProto, "usleep", do_usleep);
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
    initialized = true;
}

static lua_State* getLuaState() {
    LockHolder lh(&luamutex);
    initLuaState();
    if (stateThreads.empty()) {
        std::cerr << "New lua thread." << std::endl;
        stateThreads.push(lua_newthread(luaStateProto));
    }
    lua_State *rv = stateThreads.front();
    stateThreads.pop();
    return rv;
}

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

void handle_call(const char *call) {
    LuaStateHolder lsh(getLuaState());
    assert(lsh.state);
    lua_settop(lsh.state, 0);
    lua_getfield(lsh.state, LUA_GLOBALSINDEX, "handle");
    lua_pushstring(lsh.state, call);
    if (lua_pcall(lsh.state, 1, 0, 0)) {
        std::cerr << "Error initializing registration: "
                  << lua_tostring(lsh.state, -1) << std::endl;
    }
}
