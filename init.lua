-- This script is executed to initialize the scripting environment
-- after declaring the labrea object and registering all C functions.

-- If exit handlers are used, the table for them must be created
labrea["exit_handlers"] = {}

labrea['atexit'] = function(f)
                      table.insert(labrea['exit_handlers'], f)
                   end

dofile(os.getenv("LABREA_SCRIPT"))
