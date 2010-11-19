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

static class LabreaInit {
public:
    LabreaInit() {
        for (size_t n = 0; functions[n].name; ++n) {
            functions[n].orig = dlsym(RTLD_NEXT, functions[n].name);
            assert(functions[n].orig);
        }
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
