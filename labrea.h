#ifndef LABREA_H
#define LABREA_H 1

#include <dlfcn.h>
#include <stdexcept>

#include "labreatypes.h"
#include "scripting.hh"

#define START_DEFS struct labrea::ftype labrea::functions[] = {

#define END_DEFS {0, 0, NULL, NULL, 0, 0, {0}, false, false, false}};

namespace labrea {

extern ftype functions[];

struct ftype *findFunc(const char *name);

template <typename Rv, typename Arg1>
Rv invoke(struct ftype *ft, Arg1 a1) {
    assert(ft);
    assert(ft->orig);
    assert(ft->num_args == 1);
    Rv (*x)(Arg1 a1) = reinterpret_cast<Rv (*)(Arg1)>(ft->orig);
    return x(a1);
}

template <typename Rv, typename Arg1, typename Arg2>
Rv invoke(struct ftype *ft, Arg1 a1, Arg2 a2) {
    assert(ft);
    assert(ft->orig);
    assert(ft->num_args == 2);
    Rv (*x)(Arg1 a1, Arg2 a2) = reinterpret_cast<Rv (*)(Arg1, Arg2)>(ft->orig);
    return x(a1, a2);
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3>
Rv invoke(struct ftype *ft, Arg1 a1, Arg2 a2, Arg3 a3) {
    assert(ft);
    assert(ft->orig);
    assert(ft->num_args == 3);
    Rv (*x)(Arg1 a1, Arg2 a2, Arg3 a3) = reinterpret_cast<Rv (*)(Arg1, Arg2, Arg3)>(ft->orig);
    return x(a1, a2, a3);
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Rv invoke(struct ftype *ft, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) {
    assert(ft);
    assert(ft->orig);
    assert(ft->num_args == 4);
    Rv (*x)(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) = reinterpret_cast<Rv (*)(Arg1, Arg2, Arg3, Arg4)>(ft->orig);
    return x(a1, a2, a3, a4);
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
Rv invoke(struct ftype *ft, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) {
    assert(ft);
    assert(ft->orig);
    assert(ft->num_args == 5);
    Rv (*x)(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) = reinterpret_cast<Rv (*)(Arg1, Arg2, Arg3, Arg4, Arg5)>(ft->orig);
    return x(a1, a2, a3, a4, a5);
}

template <typename Rv, typename Arg1>
Rv wf1(const char *name, Arg1 a1) {
    static struct ftype *func;
    if (func == NULL) {
        func = findFunc(name);
    }
    before_call<Arg1>(name, a1);
    Rv rv = invoke<Rv, Arg1>(func, a1);
    after_call<Rv, Arg1>(name, rv, a1);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2>
Rv wf2(const char *name, Arg1 a1, Arg2 a2) {
    static struct ftype *func;
    if (func == NULL) {
        func = findFunc(name);
    }
    before_call<Arg1, Arg2>(name, a1, a2);
    Rv rv = invoke<Rv, Arg1, Arg2>(func, a1, a2);
    after_call<Rv, Arg1, Arg2>(name, rv, a1, a2);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3>
Rv wf3(const char *name, Arg1 a1, Arg2 a2, Arg3 a3) {
    static struct ftype *func;
    if (func == NULL) {
        func = findFunc(name);
    }
    before_call<Arg1, Arg2, Arg3>(name, a1, a2, a3);
    Rv rv;
    if (!around_call(func, a1, a2, a3, rv)) {
        rv = invoke<Rv, Arg1, Arg2, Arg3>(func, a1, a2, a3);
    }
    after_call<Rv, Arg1, Arg2, Arg3>(name, rv, a1, a2, a3);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Rv wf4(const char *name, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) {
    static struct ftype *func;
    if (func == NULL) {
        func = findFunc(name);
    }
    before_call<Arg1, Arg2, Arg3, Arg4>(name, a1, a2, a3, a4);
    Rv rv = invoke<Rv, Arg1, Arg2, Arg3, Arg4>(func, a1, a2, a3, a4);
    after_call<Rv, Arg1, Arg2, Arg3, Arg4>(name, rv, a1, a2, a3, a4);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3,
              typename Arg4, typename Arg5>
Rv wf5(const char *name, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) {
    static struct ftype *func;
    if (func == NULL) {
        func = findFunc(name);
    }
    before_call<Arg1, Arg2, Arg3, Arg4, Arg5>(name, a1, a2, a3, a4, a5);
    Rv rv = invoke<Rv, Arg1, Arg2, Arg3, Arg4, Arg5>(func, a1, a2, a3, a4, a5);
    after_call<Rv, Arg1, Arg2, Arg3, Arg4>(name, rv, a1, a2, a3, a4);
    return rv;
}

}

#endif /* LABREA_H */
