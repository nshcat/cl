#pragma once

#include <type_traits>
#include <vector>
#include <algorithm>
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
		
		
		enum class multi_mode
		{
			multi,
			positional
		};
		
		
		template< 	typename T,
					typename TReader,
					multi_mode Mode
		>
		class multi_base /* Base class for arguments that handles multiple values */
			: public argument_base
		{		
			using container_type = ::std::vector<T>;
			using view_type = ::ut::array_view<T>;
			using const_view_type = ::ut::array_view<const T>;
			using ref_container_type = ::std::vector<view_type>;
			using cref_container_type = ::std::vector<::std::unique_ptr<type_base<T>>>;
			
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
				multi_base()
					: argument_base{}
				{
				}
				
			public:
				virtual void read_end() override
				{
					// Refresh references only if argument was given
					if(this->supplied())
						refresh_references();
				}
				
				virtual void read(::std::list<::std::string>& p_args, bool) override
				{
					using TIsPositional =
						::std::integral_constant<bool, Mode == multi_mode::positional>;
						
					read_impl(TIsPositional{}, p_args);
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
				
				void read_impl(::std::false_type, ::std::list<::std::string>& p_vals)
				{
					// Only try to read one value
					if(p_vals.empty())
					{
						ut::throwf<std::runtime_error>(
							"No value supplied for given argument \"--%s\"!", this->m_LongName
						);
					}
					else
					{
						// Parse value
						m_Values.push_back(m_Reader.read(p_vals.front()));
			
						// Consume value from list
						p_vals.pop_front();
						this->m_Supplied = true;
					}
				}
				
				void read_impl(::std::true_type, ::std::list<::std::string>& p_vals)
				{
					// Match as many values as possible
					if(p_vals.empty())
					{
						ut::throwf<std::runtime_error>(
							"No value supplied for given argument \"--%s\"!", this->m_LongName
						);
					}
					else
					{
						// Parse values
						for(const auto& t_str: p_vals)
							m_Values.push_back(m_Reader.read(t_str));
			
						// Consume list
						p_vals.clear();
						this->m_Supplied = true;
					}
				}
				
			protected:
				TReader m_Reader;
				container_type m_Values;
				ref_container_type m_Refs;
				cref_container_type m_ContainerRefs;
		};
	}
}