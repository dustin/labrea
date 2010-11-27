-- This script is executed to initialize the scripting environment
-- after declaring the labrea object and registering all C functions.

-- If exit handlers are used, the table for them must be created
labrea["exit_handlers"] = {}
labrea["periodic_queue"] = {}
labrea["current_timestamp"] = 0

function labrea.atexit(f)
   table.insert(labrea.exit_handlers, f)
end

function labrea.schedule(t, f)
   local tab = labrea.periodic_queue
   t = t + labrea.current_timestamp
   if not tab[t] then
      tab[t] = {}
   end
   table.insert(tab[t], f)
end

-- This function is called immediately before the process exits.
function labrea_exiting()
   for k,f in pairs(labrea.exit_handlers) do f() end
end

-- This function is called once a second.
function labrea_periodic(t)
   labrea.current_timestamp = t
   for k,funs in pairs(labrea.periodic_queue) do
      if t >= k then
         for x,f in pairs(funs) do
            f()
         end
         labrea.periodic_queue[k] = nil
      end
   end
end

-- Now that our environment is initialized, let's load the user script.
dofile(os.getenv("LABREA_SCRIPT"))