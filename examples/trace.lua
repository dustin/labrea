-- Dynamically generate intercepts for all hook points and print entry
-- and exit for each.

for k,v in pairs(labrea.available_hooks) do
   local x = getfenv()
   x["before_" .. v] = function(...)
                          io.stderr:write(string.format("Entering %s\n", v))
                       end
   x["after_" .. v] = function(...)
                         io.stderr:write(string.format("Leaving %s\n", v))
                       end
end
