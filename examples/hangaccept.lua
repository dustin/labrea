-- Simulate network failures by slowing down accepts by 100ms.
function before_accept(...)
   labrea.usleep(100000)
end
