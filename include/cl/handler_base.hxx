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

#include "command_base.hxx"
#include "tags.hxx"
#include "global_option.hxx"
#include "local_option.hxx"
#include "handler_data.hxx"
#include "meta.hxx"
#include "argument_base.hxx"

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
				auto dispatch(global_option_tag_t, T&& p_arg)
					-> void
				{
					p_arg.apply(m_Data);
				}
				
			protected:	/* Error handling */
				// Handle help/usage display for given command, aswell as error mode
				auto handle_error(const ::std::exception& p_ex, const command_base& p_cmd)
					-> void;
					
				// Only do error mode stuff. This is used when no command can be supplied, 
				// e.g. when the user supplied a wrong command name
				auto handle_error(const ::std::exception& p_ex)
					-> void;
					
				[[noreturn]] auto rethrow_error()
					-> void;
				
			protected:
				handler_data m_Data;
		};
	}
}
