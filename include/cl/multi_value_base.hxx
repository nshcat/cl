#pragma once

#include <type_traits>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <limits>
#include <ut/always_false.hxx>
#include <ut/small_vector.hxx>
#include <ut/array_view.hxx>
#include <ut/throwf.hxx>
#include "tags.hxx"
#include "argument_base.hxx"

namespace cl
{
	namespace internal
	{
		template< typename T, typename U >
		using is_push_backable_check = decltype( ::std::declval<U>().push_back(::std::declval<T>()) );
		
		template< typename T, typename U >
		using is_push_backable = ut::is_detected< is_push_backable_check, T, U >;
		
		template< typename T, typename U >
		constexpr bool is_push_backable_v = is_push_backable<T, U>::value;
		
		
		template< typename T >
		class type_base
		{
			public:
				// This is needed for STL algorithms to work
				using value_type = T;
				using reference = T&;
				using const_reference = const T&;
			
			public:
				virtual ~type_base()
				{
				}
			
			public:
				virtual void push_back(const T&) = 0;
		};
		
		template< typename T, typename U >
		class type_holder
			: public type_base<T>
		{
			static_assert(is_push_backable_v<T, U>,
				"U needs to be a container that can push_back a value of type T!");
			
			using TThis = type_holder<T, U>;
			using TBase = type_base<T>;
			
			public:
				type_holder(U* p_ref)
				 	: m_Ref{p_ref}
				{
					
				}
				
			public:
				virtual void push_back(const T& p_val) override
				{
					m_Ref->push_back(p_val);
				}
				
			private:
				U* m_Ref;
		};
		
		template< typename T >
		class multi_value_base /* Base class for arguments that handles multiple values. This does not implement parsing logic.
								  The separation of multi_value_base and multi_base was done to allow generic code to only access
								  the stored values (e.g. via view()) without having to known the type of reader or multi_mode
								  used. Generic code can just use multi_value_base<T> (like value_base<T> for singular values)
								  to retrieve the values. */
			: public argument_base
		{		
			public:
				using view_type = ::ut::array_view<T>;
				using const_view_type = ::ut::array_view<const T>;
	
			protected:
				using container_type = ::std::vector<T>;
				using ref_container_type = ::std::vector<view_type>;
				using cref_container_type = ::std::vector<::std::unique_ptr<type_base<T>>>;
				using cb_container_type = ::std::vector<::std::function<void(const T&)>>;
			
			public:
				using iterator = typename container_type::const_iterator;
				using const_iterator = iterator;
				using reverse_iterator = typename container_type::const_reverse_iterator;
				using const_reverse_iterator = reverse_iterator;
				using value_type = T;
				using const_reference = const T&;
				using reference = const_reference;
				using size_type = typename container_type::size_type;
				using difference_type = typename container_type::difference_type;
			
			public:
				multi_value_base()
					: argument_base{}
				{
				}				
			
			public:
				auto view() const
					-> const_view_type
				{
					return { m_Values.cbegin(), m_Values.cend() };
				}
				
				auto size() const
					-> size_type
				{
					return m_Values.size();
				}
				
				auto empty() const
					-> bool
				{
					return m_Values.empty();
				}		
			
			public:
				auto begin() const
					-> const_iterator
				{
					return m_Values.cbegin();
				}
				
				auto end() const
					-> const_iterator
				{
					return m_Values.cend();
				}
				
				auto cbegin() const
					-> const_iterator
				{
					return m_Values.cbegin();
				}
				
				auto cend() const
					-> const_iterator
				{
					return m_Values.cend();
				}
				
				auto rbegin() const
					-> const_reverse_iterator
				{
					return m_Values.crbegin();
				}
				
				auto rend() const
					-> const_reverse_iterator
				{
					return m_Values.crend();
				}
				
				auto crbegin() const
					-> const_reverse_iterator
				{
					return m_Values.crbegin();
				}
				
				auto crend() const
					-> const_reverse_iterator
				{
					return m_Values.crend();
				}
				
			public:
				using argument_base::dispatch;
				
				// Set reference
				void dispatch(internal::reference_t<T> p_tag)
				{
					// Add single element range
					m_Refs.push_back({p_tag.value(), 1});
				}
				
				// Set reference view
				void dispatch(internal::view_t<T> p_tag)
				{
					m_Refs.push_back(p_tag.value());
				}
				
				// Set callback
				void dispatch(internal::callback_t<T> p_tag)
				{
					m_Callbacks.push_back(p_tag.value());
				}
							
				// Set container reference
				template< 	typename C,
							typename = ::std::enable_if_t<is_push_backable_v<T, C>>
				>
				void dispatch(internal::reference_t<C>&& p_tag)
				{
					m_ContainerRefs.push_back(
						::std::make_unique<type_holder<T, C>>(p_tag.value())
					);
				}
						
				// Provide better error message if reference is mismatching
				template< 	typename U,
							typename = void,
							typename = ::std::enable_if_t<! is_push_backable_v<T, U>>
				>
				void dispatch(internal::reference_t<U>&&)
				{
					static_assert( ut::always_false<U>::value,
						"Reference tag with mismatching type supplied!" );
				}
				
			protected:
				void do_callbacks() const
				{
					if(m_Values.size() <= 0)
						return;
					
					for(const auto& t_cb: m_Callbacks)
					{
						t_cb( m_Values.back() );
					}
				}
			
				void refresh_references()
				{
					// Range references
					for(auto t_view: m_Refs)
					{
						// Copy up to t_view.length() values
						::std::copy_n(cbegin(), ::std::min(t_view.length(), size()), t_view.begin());
					}
					
					// Container references
					for(auto& t_ref: m_ContainerRefs)
					{
						// Dereference
						auto& t_holder = *t_ref;
						
						// Push back all elements
						::std::copy(cbegin(), cend(), ::std::back_inserter(t_holder));
					}
				}						
				
			protected:
				container_type m_Values;
				ref_container_type m_Refs;
				cref_container_type m_ContainerRefs;
				cb_container_type m_Callbacks;
		};
	}
}
