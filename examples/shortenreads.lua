read_max = 64

-- Ensure we never actually read more than 64 bytes.  This will
-- confuse applications that assume they always get what they ask for.
function around_read(f, d, buf, size)
   return labrea.invoke(f, d, buf, math.min(read_max, size))
end