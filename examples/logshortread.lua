-- Log every time a read read less than it was willing to take.

function after_read(rv, fd, buf, nb)
   if rv < nb then
      io.write(string.format("read of %d bytes from %d returned %d\n",
                             nb, fd, rv))
   end
end
