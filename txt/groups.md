Let groups be trigger sinks, and tags be trigger sources.
This decouples the trigger logic from the argument classes.
Only the parser handles the logic, because it always is "allowed/required/forbidden".
This is done after **parsing**. 


```c++
enum class operation
{
	add,
	remove
};

enum class trigger
{
	trigger_add,
	trigger_remove
};



// Mode
operation g_Operation{ };


cl::handler
{
	cl::application_title("test_app"),
	cl::application_version("0.0"),
	cl::use_exceptions(true),
	
	cl::group
	{
		// Description and title
		cl::title("Mode of operation"),
		cl::description("Switches that set the mode of operation."),
	
		// Allow only one of the arguments in this group
		cl::exclusive,			
		
		// A switch that sets mode to "add".
		cl::switch<operation>
		{
			cl::long_name("add"),
			cl::value(operation::add),
			cl::reference(g_Operation),
			
			// This is a trigger source for trigger "trigger_add"
			cl::trigger_src(trigger::trigger_add)
		},
		
		// A switch that sets mode to "remove".
		cl::switch<operation>
		{
			cl::long_name("remove"),
			cl::value(operation::remove),
			cl::reference(g_Operation),
			
			// This is a trigger source for trigger "trigger_remove"
			cl::trigger_src(trigger::trigger_remove)
		}
	}
	
	
	
	cl::group
	{
		cl::trigger_sink(trigger::trigger_add),
		cl::required,
		
		
		cl::string_argument
		{
			cl::long_name("path")
		}
	}
}
```