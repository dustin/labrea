#!/usr/bin/env python

import os
import sys

def type(s):
    return {4: 'uint32_t', 8: 'uint64_t'}[int(s)]

def fargtype(s):
    return {4: 'i32', 8: 'i64'}[int(s)]

def encode(r, argseq):
    i = 0
    for b in (int(int(x) == 8) for x in argseq):
        i = (i << 1) | b
    return (i << 1) | int(int(r) == 8)

def decode(offset, length):
    def s(n):
        return int('48'[n])

    r = s(offset & 1)
    offset = offset >> 1
    seq = list(reversed([s((offset >> i) & 1) for i in range(length)]))
    return r, seq

def genfname(argc, r, argwidth):
    return 'invoke_%d_%s_%s' % (argc, r, ''.join(argwidth))

def mk_argseq(l):
    def strdecode(a, b):
        return ''.join([str(x) for x in decode(a, b)[1]])
    return sorted(set(strdecode(x, l) for x in range(int(2 ** (l+1)))))

def gen(out, argc, r, argwidth):
    argtypes = '%s' % ', '.join(type(a) for a in argwidth)
    fname = genfname(argc, r, argwidth)
    fargs = ', '.join('union farg a%d' % i for i in range(len(argwidth)))
    callargs = ', '.join('a%d.%s' % (i, fargtype(t)) for (i, t) in enumerate(argwidth))
    out.write("static lua_Integer %s(const void *f, %s) {\n" % (fname, fargs))
    out.write("    %s (*fc)(%s) = reinterpret_cast<%s (*)(%s)>(f);\n" % (
        type(r), argtypes, type(r), argtypes))
    out.write("    return (lua_Integer)fc(%s);\n" % callargs)
    out.write("}\n\n")

if __name__ == '__main__':

    MAXARGS = 5
    out = file('gen_invoker.cc.tmp', 'w')

    out.write("""#include <cassert>
#include <iostream>

#include "labrea.h"
#include "scripting.hh"
#include "gen_invoker.hh"

namespace labrea {

""")

    for i in range(1, MAXARGS+1):
        for argseq in mk_argseq(i):
            for r in '48':
                gen(out, i, r, argseq)

    for i in range(1, MAXARGS+1):
        fargs = ', '.join('union farg a%d' % x for x in range(i))
        callargs = ', '.join('a%d' % x for x in range(i))
        out.write("static lua_Integer abstractInvoke(struct ftype *fun, %s) {\n" % fargs)
        out.write("    assert(fun->num_args == %d);\n" % i)
        allfuns = dict((encode(4, seq), seq) for seq in mk_argseq(i))
        allfuns.update(dict((encode(8, seq), seq) for seq in mk_argseq(i)))
        out.write("    static lua_Integer (*allfuns[])(const void*, %s) = {\n" % fargs)
        for o in range(len(allfuns)):
            if o in allfuns:
                r = {o: 4}.get(encode(4, allfuns[o]), 8)
                out.write("        %s," % genfname(i, r, allfuns[o]))
            else:
                out.write("        NULL,")
            out.write(" // %d\n" % o)
        out.write("    };\n\n")
        out.write("    return allfuns[fun->offset](fun->orig, %s);\n" % (callargs))
        out.write("}\n\n")

    out.write("lua_Integer abstractInvoke(struct ftype *fun, union farg *args) {\n")
    out.write("    switch (fun->num_args) {");
    for i in range(1, MAXARGS+1):
        out.write("    case %d:\n" % i)
        fa = ', '.join('args[%d]' % x for x in range(i))
        out.write("        return abstractInvoke(fun, %s);\n" % fa)

    out.write("    }\n")
    out.write("    abort(); // UNREACHED\n")
    out.write("}\n")

    out.write("}\n")


    out.close()
    os.rename('gen_invoker.cc.tmp', 'gen_invoker.cc')
