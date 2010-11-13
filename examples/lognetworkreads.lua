-- Log anything read from connected clients.

network = {}

-- Track accept and close to know when connections come and go.
function after_accept(rv, ...)
   if rv >= 0 then
      network[rv] = 1
   end
end

function before_close(fd)
   network[fd] = nil
end

-- Now we can handle reads and know it was from a client.
function after_read(rv, fd, ...)
   if network[fd] then
      io.write(string.format("Read %d bytes from network fd:  %d\n", rv, fd))
   end
end