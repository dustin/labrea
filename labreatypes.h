#ifndef LABREA_TYPES_H
#define LABREA_TYPES_H 1

#define MAX_ARGS 16

namespace labrea {
struct ftype {
    int          pos;
    const char  *name;
    void        *orig;
    size_t       num_args;
    size_t       arg_widths[MAX_ARGS];
    bool         has_before;
    bool         has_around;
    bool         has_after;
};

}
#endif /* LABREA_TYPES_H */
