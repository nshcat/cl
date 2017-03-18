// handler_base: Base for all handler classes. Encapsulates common handler operations:
// Manages handler data and global option dispatch, aswell as error handling.

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
				auto print_help()
					-> void;
				
			protected:
				handler_data m_Data;
		};
	}
}