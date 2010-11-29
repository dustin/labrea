#!/usr/bin/env python

import os
import sys

def gen(out, arity):
    typenames = ', '.join('typename Arg%d' % (i+1) for i in range(arity))
    types = ', '.join('Arg%d' % (i+1) for i in range(arity))
    callsig = ', '.join('Arg%d a%d' % (i+1, i+1) for i in range(arity))
    args = ', '.join('a%d' % (i+1) for i in range(arity))

    out.write("""template <typename Rv, %(typenames)s>
Rv invoke(struct ftype *ft, %(callsig)s) {
    assert(ft);
    assert(ft->num_args == %(arity)d);
    void *forig = ft->orig ? ft->orig : dlsym(RTLD_NEXT, ft->name);
    assert(forig);
    Rv (*x)(%(callsig)s) = function_cast<Rv (*)(%(callsig)s)>(forig);
    return x(%(args)s);
}

template <typename Rv, %(typenames)s>
Rv wrapper(const char *name, %(callsig)s) {
    static struct ftype *func;
    if (func == NULL) {
        func = findFunc(name);
    }

    LuaInvocation li;
    if (!li.ready) {
        return invoke<Rv, %(types)s>(func, %(args)s);
    }

    if (func->has_before) {
        before_call<%(types)s>(name, %(args)s);
    }

    Rv rv;
    if (func->has_around) {
        around_call(func, %(args)s, rv);
    } else {
        rv = invoke<Rv, %(types)s>(func, %(args)s);
    }

    if (func->has_after) {
        after_call<Rv, %(types)s>(name, rv, %(args)s);
    }
    return rv;
}

""" % {'typenames': typenames, 'types': types, 'callsig': callsig,
       'args': args, 'arity': arity})

if __name__ == '__main__':
    out = file('gen_wrapperfuns.hh.tmp', 'w')

    out.write("""#ifndef GEN_WRAPPERFUNS_HH
#define GEN_WRAPPERFUNS_HH 1

#include "labreatypes.h"
#include "scripting.hh"

namespace labrea {

struct ftype *findFunc(const char *name);

""")

    for i in range(1, 6):
        gen(out, i)

    out.write("""}
#endif /* GEN_WRAPPERFUNS_HH */
""")

    out.close()
    os.rename('gen_wrapperfuns.hh.tmp', 'gen_wrapperfuns.hh')
