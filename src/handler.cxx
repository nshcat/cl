#include "handler.hxx"

namespace cl
{
	bool handler::read(input_type p_in)
	{
		// Just forward input to implementation and handle errors.
		try
		{
			this->command_type::read(p_in);
			
			// If no exception was thrown, no error occured.
			return true;
		}
		catch(const ::std::exception& p_ex)
		{
			this->handle_error(p_ex, *this);
			
			return false;
		}	
	}
				
	bool handler::read(int p_argc, const char** p_argv)
	{
		return read(ut::make_view<const char*>(p_argv+1, p_argc-1));
	}
}

