math.randomseed(os.time())

function around_read(f, fd, buf, size)
   if math.random(1, 100) < 25 then
      io.stderr:write(string.format("Erroring.\n"))
      labrea.set_errno(5) -- EIO
      return -1
   else
      return labrea.invoke(f, fd, buf, size)
   end
end