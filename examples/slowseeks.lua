-- Slow down seek times by 10ms 3% of the time.

function before_lseek(...)
   if math.random(1, 100) <= 3 then
      labrea.usleep(10000)
   end
end
