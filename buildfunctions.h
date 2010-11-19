#define DEFINE1(rv, n, a1)                                              \
    rv n(a1 arg1) {                                                     \
        return labrea::wrapper<rv, a1>(#n, arg1);                       \
    }

#define DEFINE2(rv, n, a1, a2)                                          \
    rv n(a1 arg1, a2 arg2) {                                            \
        return labrea::wrapper<rv, a1, a2>(#n, arg1, arg2);             \
    }

#define DEFINE3(rv, n, a1, a2, a3)                                      \
    rv n(a1 arg1, a2 arg2, a3 arg3) {                                   \
        return labrea::wrapper<rv, a1, a2, a3>(#n, arg1, arg2, arg3);   \
    }

#define DEFINE4(rv, n, a1, a2, a3, a4)                                  \
    rv n(a1 arg1, a2 arg2, a3 arg3, a4 arg4) {                          \
        return labrea::wrapper<rv, a1, a2, a3, a4>(#n, arg1, arg2, arg3, arg4); \
    }

#define DEFINE5(rv, n, a1, a2, a3, a4, a5)                              \
    rv n(a1 arg1, a2 arg2, a3 arg3, a4 arg4, a5 arg5) {                 \
        return labrea::wrapper<rv, a1, a2, a3, a4, a5>(#n, arg1, arg2, arg3, arg4, arg5); \
    }

extern "C" {
#include "calls.defs"
}

#undef DEFINE1
#undef DEFINE2
#undef DEFINE3
#undef DEFINE4
#undef DEFINE5
