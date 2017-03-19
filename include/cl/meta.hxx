// meta: Template meta programming utilities

#pragma once

#include <ut/type_traits.hxx>
#include <ut/meta.hxx>

#include "argument_base.hxx"
#include "option_base.hxx"


namespace cl
{
	namespace internal
	{
		// Metaprogramming utilities used by commandline handlers
		struct option_tag_t{};
		struct argument_tag_t{};
		struct invalid_tag_t{};
		
		template< typename T >
		using is_option_tag = ::std::is_base_of<option_base, T>;
		
		template< typename T >
		using is_argument_tag = ::std::is_base_of<argument_base, T>;
		
		template< typename T >
		using tag_category_of_t =
			ut::category_of_t<
				T,
				ut::category<option_tag_t, is_option_tag>,
				ut::category<argument_tag_t, is_argument_tag>,
				ut::category<invalid_tag_t>
			>;
		//
	}
}