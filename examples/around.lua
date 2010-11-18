truncateto = 8

function around_read(fname, d, buf, size)
   io.write(string.format("Doing an around read with %s(%d, %x, %d).\n", fname, d, buf, size))
   if size > truncateto
   then
      io.write(string.format("Truncating read of %d to %d\n", size, truncateto))
      size = truncateto
   end
   rv = invoke(fname, d, buf, size)
   io.write(string.format("...and returning %d bytes read\n", rv))
   return rv
end