// handler_base: Base for all handler classes.
//
// This class encapsulates functionality common to all types of handlers:
//	It
//	- manages handler data like application name, version, error handling and help settings
//	- dispatches option tags
//	- does error handling
//	- does help handling

#pragma once

#include <type_traits>

#include <ut/meta.hxx>
#include <ut/type_traits.hxx>
#include <ut/observer_ptr.hxx>
#include <ut/throwf.hxx>
#include <ut/always_false.hxx>

#include "tags.hxx"
#include "option_base.hxx"
#include "handler_data.hxx"
#include "meta.hxx"

namespace cl
{
	namespace internal
	{	
		class handler_base
		{		
			public:
				handler_base() = default;
				handler_base(const handler_base&) = default;			
				handler_base& operator=(const handler_base&) = default;
			
			public:
				template< typename T >
				auto dispatch(T&& p_arg)
					-> void
				{
					dispatch(tag_category_of_t<::std::decay_t<T>>{}, ::std::forward<T>(p_arg));
				}
			
			protected:	/* Option dispatching */
				template< typename TType, typename T >
				auto dispatch(TType, T&&)
					-> void
				{
					// Just sink it
				}
				
				template< typename T >
				auto dispatch(option_tag_t, T&& p_arg)
					-> void
				{
					p_arg.apply(m_Data);
				}
				
			protected:	/* Error handling */
				auto handle_error(const ::std::exception& p_ex)
					-> bool;
					
				[[noreturn]] auto rethrow_error()
					-> void;
				
			protected:	/* Help handling */
				// Prints help according to help_mode
				auto print_help() const
					-> void;
					
				// Prints a summary of all registered arguments
				auto print_summary() const
					-> void;
					
				// Prints a usage string, if specified
				auto print_usage() const
					-> void;
				
			protected:
				handler_data m_Data;
		};
	}
}
