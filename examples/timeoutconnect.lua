-- Simulate network latencies by returning ETIMEDOUT for every 10th connect

math.randomseed(os.time())

function around_connect(...)
   if math.random(1,100) <= 10 then
       labrea.set_errno(60) -- ETIMEDOUT
       return -1
   else
       return labrea.invoke(...)
   end
end
