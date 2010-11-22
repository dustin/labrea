# OPP Scripting

![Hackitecture](labrea/wiki/hackitecture.png "This is how it works")

La Brea allows you to inject scripts into an existing application
without recompiling.

Initially, this was built to slow down specific parts of an
application programatically.  It's great for simulating a slow
network, disk, or other types of things within a process.

## Implemented Injection Points

The following patterns are available for injection:

### before_`fname`

Invoked before the function `fname` is called.  The arguments to be
given to *fname* are provided.

These functions should not return values.

### after_`fname`

Invoked after the function `fname` has completed.  Both the return
value and the original arguments are provided (in that order).  This
is invoked *before* returning control back to the application, but you
have little opportuntity to do anything particularly destructive here.

These functions should not return values.

### around_`fname`

Invoked instead of `fname`.  This allows for ridiculous magic.  Since
it's a bit complex, I'll cover it in more detail here.

First, the arguments are as follows:

1. A reference to the original function.
2. Any additional arguments required by that function.

The original function can be invoked using the `invoke` function in
lua and that function can pass along whatever parameters may be
interesting.  For example, if you'd like to truncate all reads so that
the application never reads more than 64 bytes, you can do the
following:

    function around_read(f, d, buf, size)
        if size > 64 then
            size = 64
        end
        return labrea.invoke(f, d, buf, size)
    end

Alternatively, you can just cause an error as shown in the
[random errors][re] example.

## Usage

First, write a script to slow stuff down.  For example, if you'd like
to slow random seeks down, you can create a script that looks like the
following:

    function before_lseek(fd, offset, whence)
       if math.random(1, 100) == 13 then
          io.write(string.format("Slowing down a seek on fd=%d to %d (%d)\n",
                                 fd, offset, whence))
          labrea.usleep(1000000)
       end
    end

Save that in a file, say, `/tmp/slowseek.lua` and then use the
`labrea` as follows:

    labrea /tmp/slowseek.lua myprogram -and -its -arguments

[re]: https://github.com/dustin/labrea/blob/master/examples/randomerrors.lua
