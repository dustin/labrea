* Slow Down!

La Brea allows you to slow down parts of an application
programatically.  It's great for simulating a slow network, disk, or
other types of things within a process.

** Usage

First, write a script to slow stuff down.  For example, if you'd like
to slow random seeks down, you can create a script that looks like the
following:

    function before_lseek(fd, offset, whence)
       if math.random(1, 100) == 13 then
          io.write(string.format("Slowing down a seek on fd=%d to %d (%d)\n",
                                 fd, offset, whence))
          usleep(1000000)
       end
    end

Save that in a file, say, `/tmp/slowseek.lua` and then use the
`labrea` as follows:

    labrea /tmp/slowseek.lua myprogram -and -its -arguments

