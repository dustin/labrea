#define DEFINE1(rv, n, a1) {0, #n, NULL, 1, {sizeof(a1)}, false, false, false},
#define DEFINE2(rv, n, a1, a2) {0, #n, NULL, 2,  \
            {sizeof(a1), sizeof(a2)},        \
            false, false, false},
#define DEFINE3(rv, n, a1, a2, a3) {0, #n, NULL, 3,  \
            {sizeof(a1), sizeof(a2), sizeof(a3)},   \
            false, false, false},
#define DEFINE4(rv, n, a1, a2, a3, a4) {0, #n, NULL, 4,          \
            {sizeof(a1), sizeof(a2), sizeof(a3), sizeof(a4)},   \
            false, false, false},
#define DEFINE5(rv, n, a1, a2, a3, a4, a5) {0, #n, NULL, 5,              \
            {sizeof(a1), sizeof(a2), sizeof(a3), sizeof(a4), sizeof(a5)}, \
            false, false, false},

#include "calls.defs"

#undef DEFINE1
#undef DEFINE2
#undef DEFINE3
#undef DEFINE4
#undef DEFINE5
