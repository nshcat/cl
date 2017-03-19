#include <exception>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <stdexcept>

#include "handler_base.hxx"

namespace cl
{
	namespace internal
	{
		auto handler_base::handle_error(const ::std::exception& p_ex)
				-> bool
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
					return false;
					break;
				}
				case internal::error_mode_::terminate:
				{
					// Print out error message
					::std::cout << p_ex.what() << std::endl;

					print_help();
					::std::exit(EXIT_FAILURE);
				}	
				case internal::error_mode_::throw_exception:
				default:
				{
					rethrow_error();
					break;
				}
			}
			
			return false;
		}

		auto handler_base::rethrow_error()
			-> void
		{
			::std::rethrow_exception(::std::current_exception());
		}
		
		auto handler_base::print_help() const
			-> void
		{
			if(m_Data.m_HelpMode != internal::help_mode_::show_none)
			{
				// Print usage first
				if(m_Data.m_HelpMode & internal::help_mode_::show_usage)
					print_usage();
				
				if(m_Data.m_HelpMode & internal::help_mode_::show_summary)
					print_summary();
			}
		}
		
		auto handler_base::print_usage() const
			-> void
		{
			if(!m_Data.m_ApplicationUsage.empty())
				std::cout << m_Data.m_ApplicationUsage << std::endl;
		}
		
		auto handler_base::print_summary() const
			-> void
		{
			
		}
	}
}