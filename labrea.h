#ifndef LABREA_H
#define LABREA_H 1

#include <dlfcn.h>
#include <stdexcept>

#include "labreatypes.h"
#include "gen_wrapperfuns.hh"

#define START_DEFS struct labrea::ftype labrea::functions[] = {

#define END_DEFS {0, 0, NULL, NULL, 0, 0, {0}, false, false, false}};

namespace labrea {

extern ftype functions[];

}

#endif /* LABREA_H */
