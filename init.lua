-- This script is executed to initialize the scripting environment
-- after declaring the labrea object and registering all C functions.

-- If exit handlers are used, the table for them must be created
labrea["exit_handlers"] = {}

labrea['atexit'] = function(f)
                      table.insert(labrea['exit_handlers'], f)
                   end

-- This function is called immediately before the process exits.
function labrea_exiting()
   for k,f in pairs(labrea['exit_handlers']) do f() end
end

-- Now that our environment is initialized, let's load the user script.
dofile(os.getenv("LABREA_SCRIPT"))
