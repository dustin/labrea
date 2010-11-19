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

pthread_once_t finit_once = PTHREAD_ONCE_INIT;

START_DEFS
#include "definecalls.h"
END_DEFS

#include "buildfunctions.h"

namespace labrea {
static void doInit() {
    for (size_t n = 0; functions[n].name; ++n) {
        functions[n].orig = dlsym(RTLD_NEXT, functions[n].name);
        assert(functions[n].orig);
    }
}

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

void initFunctions() {
    if (pthread_once(&finit_once, labrea::doInit) != 0) {
        perror("pthread_once");
        abort();
    }
}
}
