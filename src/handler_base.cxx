#include <exception>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <stdexcept>

#include "handler_base.hxx"
#include "diagnostics.hxx"

namespace cl
{
	namespace internal
	{
		auto handler_base::handle_error(const ::std::exception& p_ex)
				-> void
		{
			// Select action based on current error_mode
			switch (m_Data.m_ErrorMode)
			{
				case internal::error_mode_::custom_handler:
				{
					// If no callback is registered rethrow exception
					if(m_Data.m_ErrorHandler)
					{
						m_Data.m_ErrorHandler(p_ex.what());
						break;
					}
					else rethrow_error();
					break;
				}
				case internal::error_mode_::return_value:
				{
					// Just do nothing
					break;
				}
				case internal::error_mode_::terminate:
				{
					// Print out error message
					::std::cout << p_ex.what() << std::endl;
						
					::std::exit(EXIT_FAILURE);
				}	
				case internal::error_mode_::throw_exception:
				default:
				{
					rethrow_error();
					break;
				}
			}
		}
	
		auto handler_base::handle_error(const ::std::exception& p_ex, const command_base& p_cmd)
				-> void
		{
			// Select action based on current error_mode
			switch (m_Data.m_ErrorMode)
			{
				case internal::error_mode_::custom_handler:
				{
					// If no callback is registered rethrow exception
					if(m_Data.m_ErrorHandler)
					{
						m_Data.m_ErrorHandler(p_ex.what());
						break;
					}
					else rethrow_error();
					break;
				}
				case internal::error_mode_::return_value:
				{
					// Do nothing
					break;
				}
				case internal::error_mode_::terminate:
				{
					// Print out error message
					internal::post_diagnostic(
						internal::diagnostics_level::error,
						{ m_Data.m_ApplicationName },
						{ },
						{ p_ex.what() }
					);

					if(m_Data.m_HelpMode & internal::help_mode_::on_error)
						p_cmd.print_help();
						
					::std::exit(EXIT_FAILURE);
				}	
				case internal::error_mode_::throw_exception:
				default:
				{
					rethrow_error();
					break;
				}
			}
		}

		auto handler_base::rethrow_error()
			-> void
		{
			::std::rethrow_exception(::std::current_exception());
		}		
	}
}
