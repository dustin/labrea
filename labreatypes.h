#ifndef LABREA_TYPES_H
#define LABREA_TYPES_H 1

#include <inttypes.h>

// Stolen from http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName)      \
    TypeName(const TypeName&);                  \
    void operator=(const TypeName&)

#define MAX_ARGS 16

namespace labrea {
struct ftype {
    int          pos;    // Position in the array
    int          offset; // Arg encoding.
    const char  *name;
    void        *orig;
    size_t       num_args;
    size_t       rv_size;
    size_t       arg_widths[MAX_ARGS];
    bool         has_before;
    bool         has_around;
    bool         has_after;
};

union farg {
    uint32_t i32;
    uint64_t i64;
};

template <typename F>
F function_cast(const void *p) {
    union {
        const void *vp;
        F fp;
    } hack;
    hack.vp = p;
    return hack.fp;
}

}
#endif /* LABREA_TYPES_H */
