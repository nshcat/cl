// handler_base: Base for all handler classes. Manages handler data and global option dispatch.

#pragma once

#include <ut/meta.hxx>
#include <ut/type_traits.hxx>
#include <ut/observer_ptr.hxx>
#include <ut/throwf.hxx>
#include <ut/always_false.hxx>

#include "tags.hxx"
#include "option_base.hxx"
#include "handler_data.hxx"

namespace cl
{
	namespace internal
	{
		// Metaprogramming utilities used by commandline handlers
		struct option_tag_t{};
		struct argument_tag_t{};
		struct invalid_tag_t{};
		
		template< typename T >
		using is_option_tag = ::std::is_base_of<internal::option_base, T>;
		
		template< typename T >
		using is_argument_tag = ::std::is_base_of<internal::argument_base, T>;
		
		template< typename T >
		using tag_category_of_t =
			ut::category_of_t<
				T,
				ut::category<option_tag_t, is_option_tag>,
				ut::category<argument_tag_t, is_argument_tag>,
				ut::category<invalid_tag_t>
			>;
		//
		
		
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
			
			protected:
				template< typename TType, typename T >
				auto dispatch(TType&&, T&&)
					-> void
				{
					static_assert(ut::always_false_v<T>,
						"Tried to dispatch invalid type with handler_base!"
						"(This should not happen)"
					);
				}
				
				template< typename T >
				auto dispatch(option_tag_t, T&& p_arg)
					-> void
				{
					p_arg.apply(m_Data);
				}
				
			protected:
				handler_data m_Data;
		};
	}
}