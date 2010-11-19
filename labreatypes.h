#ifndef LABREA_TYPES_H
#define LABREA_TYPES_H 1

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

}
#endif /* LABREA_TYPES_H */
