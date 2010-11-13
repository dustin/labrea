#ifndef LABREA_H
#define LABREA_H 1

#include <dlfcn.h>
#include <stdexcept>

#include "scripting.hh"

#define DEFINE1(rv, n, a1)                                              \
    extern "C" rv n(a1 arg1) {                                          \
        return wf1<rv, a1>(#n, arg1);                                   \
    }

#define DEFINE2(rv, n, a1, a2)                                          \
    extern "C" rv n(a1 arg1, a2 arg2) {                                 \
        return wf2<rv, a1, a2>(#n, arg1, arg2);                         \
    }

#define DEFINE3(rv, n, a1, a2, a3)                                      \
    extern "C" rv n(a1 arg1, a2 arg2, a3 arg3) {                        \
        return wf3<rv, a1, a2, a3>(#n, arg1, arg2, arg3);               \
    }

#define DEFINE4(rv, n, a1, a2, a3, a4)                                  \
    extern "C" rv n(a1 arg1, a2 arg2, a3 arg3, a4 arg4) {               \
        return wf4<rv, a1, a2, a3, a4>(#n, arg1, arg2, arg3, arg4);     \
    }

#define DEFINE5(rv, n, a1, a2, a3, a4, a5)                              \
    extern "C" rv n(a1 arg1, a2 arg2, a3 arg3, a4 arg4, a5 arg5) {      \
        return wf5<rv, a1, a2, a3, a4, a5>(#n, arg1, arg2, arg3, arg4, arg5); \
    }

template <typename Rv, typename Arg1>
Rv wf1(const char *name, Arg1 a1) {
    static Rv (*f)(Arg1);
    if (f == NULL) {
        f = reinterpret_cast<typeof(f)>(dlsym(RTLD_NEXT, name));
        assert(f);
    }
    before_call<Arg1>(name, a1);
    Rv rv = f(a1);
    after_call<Rv, Arg1>(name, rv, a1);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2>
Rv wf2(const char *name, Arg1 a1, Arg2 a2) {
    static Rv (*f)(Arg1, Arg2);
    if (f == NULL) {
        f = reinterpret_cast<typeof(f)>(dlsym(RTLD_NEXT, name));
        assert(f);
    }
    before_call<Arg1, Arg2>(name, a1, a2);
    Rv rv = f(a1, a2);
    after_call<Rv, Arg1, Arg2>(name, rv, a1, a2);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3>
Rv wf3(const char *name, Arg1 a1, Arg2 a2, Arg3 a3) {
    static Rv (*f)(Arg1, Arg2, Arg3);
    if (f == NULL) {
        f = reinterpret_cast<typeof(f)>(dlsym(RTLD_NEXT, name));
        assert(f);
    }
    before_call<Arg1, Arg2, Arg3>(name, a1, a2, a3);
    Rv rv = f(a1, a2, a3);
    after_call<Rv, Arg1, Arg2, Arg3>(name, rv, a1, a2, a3);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Rv wf4(const char *name, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) {
    static Rv (*f)(Arg1, Arg2, Arg3, Arg4);
    if (f == NULL) {
        f = reinterpret_cast<typeof(f)>(dlsym(RTLD_NEXT, name));
        assert(f);
    }
    before_call<Arg1, Arg2, Arg3, Arg4>(name, a1, a2, a3, a4);
    Rv rv = f(a1, a2, a3, a4);
    after_call<Rv, Arg1, Arg2, Arg3, Arg4>(name, rv, a1, a2, a3, a4);
    return rv;
}

template <typename Rv, typename Arg1, typename Arg2, typename Arg3,
              typename Arg4, typename Arg5>
Rv wf5(const char *name, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) {
    static Rv (*f)(Arg1, Arg2, Arg3, Arg4, Arg5);
    if (f == NULL) {
        f = reinterpret_cast<typeof(f)>(dlsym(RTLD_NEXT, name));
        assert(f);
    }
    before_call<Arg1, Arg2, Arg3, Arg4, Arg5>(name, a1, a2, a3, a4, a5);
    Rv rv = f(a1, a2, a3, a4, a5);
    after_call<Rv, Arg1, Arg2, Arg3, Arg4>(name, rv, a1, a2, a3, a4);
    return rv;
}

#endif /* LABREA_H */
