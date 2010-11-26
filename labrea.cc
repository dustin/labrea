#include <iostream>
#include <queue>

#include <stdio.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "labrea.h"

START_DEFS
#include "definecalls.h"
END_DEFS

#include "buildfunctions.h"

namespace labrea {

void (*labreafree)(void*);
void* (*labrearealloc)(void*, size_t);

#define acceptable_arg_size(x) ( (x == 4) || (x == 8) )

static int computeOffset(struct ftype *fun) {
    assert(fun);
    int offset(0);
    for (size_t b = 0; b < fun->num_args; ++b) {
        assert(acceptable_arg_size(fun->arg_widths[b]));
        offset = (offset << 1) | (fun->arg_widths[b] == 4 ? 0 : 1);
    }
    assert(acceptable_arg_size(fun->rv_size));
    offset = (offset << 1) | (fun->rv_size == 4 ? 0 : 1);
    return offset;
}

static class LabreaInit {
public:
    LabreaInit() {
        LuaInvocation invocation;
        labreafree = reinterpret_cast<typeof(labreafree)>(dlsym(RTLD_NEXT, "free"));
        labrearealloc = reinterpret_cast<typeof(labrearealloc)>(dlsym(RTLD_NEXT, "realloc"));
        assert(labreafree);
        assert(labrearealloc);

        for (size_t n = 0; functions[n].name; ++n) {
            functions[n].pos = static_cast<int>(n);
            functions[n].offset = computeOffset(&functions[n]);
            functions[n].orig = dlsym(RTLD_NEXT, functions[n].name);
            assert(functions[n].orig);
        }

        initScriptingState();
    }

    ~LabreaInit() {
        LuaInvocation invocation;
        destroyScriptingState();
    }
} initializer;

struct ftype *findFunc(const char *name) {
    struct ftype *rv(NULL);
    for (size_t i = 0; rv == NULL && functions[i].name; ++i) {
        if (strcmp(functions[i].name, name) == 0) {
            rv = &functions[i];
        }
    }
    if (rv == NULL) {
        std::cerr << "Failed to find " << name << std::endl;
        abort();
    }
    return rv;
}

}
