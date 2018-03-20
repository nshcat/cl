#pragma once

#include <string>
#include <list>
#include <cctype>
#include <ut/type_traits.hxx>
#include <ut/throwf.hxx>
#include <ut/always_false.hxx>

#include "tags.hxx"


namespace cl
{
	namespace internal
	{
		class argument_base
		{
			public:
				argument_base()
				{
				}

				virtual ~argument_base()
				{

				}
				
			public:
				// Dispatch all given tags.
				template< typename T, typename... Ttags >
				void dispatch_all(Ttags&&... p_tags)
				{
					// Check for at least one long_name_t
					static_assert(ut::contains<internal::long_name_t, std::decay_t<Ttags>...>::value,
						"Long name tag is required for all argument types");
					
					// We know that this ptr is a ptr to T*
					auto t_ptr = static_cast<T*>(this);
					
					// Dispatch all tags
					std::initializer_list<int> tmp = { (t_ptr->dispatch(::std::forward<Ttags>(p_tags)), 0)... };

					// Silence "not used" warning
					(void)tmp;

					// Check if long name was set.
					if (this->long_name().empty())
						throw std::runtime_error("Long name was not set!");
					
				}

			public:
				virtual void read(std::list<std::string>&, bool) = 0;
				
				// TODO create pure virtual function read_end_impl(), and let child classes
				// implement that, while read_end() is a normal method that always does the supplied
				// reference update
				virtual void read_end()
				{
					if(m_HasSuppliedRef)
						*m_SuppliedRef = supplied();
				}

				virtual bool is_switch() const
				{
					return false;
				}

			public:
				bool supplied() const
				{
					return m_Supplied;
				}

				const std::string& long_name() const // TODO better names here
				{
					return m_LongName;
				}

				char short_name() const
				{
					return m_ShortName;
				}

				bool has_short_name() const
				{
					return m_HasShortName;
				}

				bool required() const
				{
					return m_Required;
				}

				bool has_id() const
				{
					return m_HasId;
				}

				std::size_t id() const
				{
					return m_Id;
				}
				
				const std::string& description() const
				{
					return m_Description;
				}
				
				bool has_description() const
				{
					return !m_Description.empty();
				}
				
				const std::string& category() const
				{
					return m_Category;
				}
				
				bool has_category() const
				{
					return !m_Category.empty();
				}

			public:
				template< typename T >
				T* as()
				{
					static_assert(std::is_base_of<argument_base, T>::value,
						"T needs to be an argument type!");

					auto t_arg = dynamic_cast<T*>(this);

					if (t_arg == nullptr)
						throw std::runtime_error("Mismatching type!");
					else return t_arg;
				}
				
				template< typename T >
				const T* as() const
				{
					static_assert(std::is_base_of<argument_base, T>::value,
						"T needs to be an argument type!");

					auto t_arg = dynamic_cast<const T*>(this);

					if (t_arg == nullptr)
						throw std::runtime_error("Mismatching type!");
					else return t_arg;
				}
				
				/*template< typename T >
				T& as() &
				{
					static_assert(std::is_base_of<argument_base, T>::value,
						"T needs to be an argument type!");
				}
				
				template< typename T >
				const T& as() &
				{
					static_assert(std::is_base_of<argument_base, T>::value,
						"T needs to be an argument type!");
				}*/
				
				/*template< typename T >
				T&& as() &&
				{
					static_assert(std::is_base_of<argument_base, T>::value,
						"T needs to be an argument type!");
				}*/
				
				template< typename T >
				bool is() const
				{
					static_assert(std::is_base_of<argument_base, T>::value,
						"T needs to be an argument type!");
						
					return (dynamic_cast<const T*>(this) != nullptr);
				}
				

				// All dispatch overloads in this and all derived argument classes need to
				// be public since they need to be accessible within
				// argument_base::dispatch_all.
			public:
				// Sink for all unimplemented tags
				template< typename T >
				void dispatch(T)
				{
					static_assert(::ut::always_false_v<T>,
						"Invalid tag for this argument type!");
				}

				// Set long name.
				void dispatch(const internal::long_name_t& p_tag)
				{
					const auto& t_str = p_tag.value();
					
					// Empty long names are not allowed
					if(t_str.empty())
						throw ::std::runtime_error("Invalid long name: Empty names not allowed!");
					
					// Unary predicate checking if a character is either
					// alpha-numeric or '-' or '_'.
					const auto t_test = [](const char x) -> bool
					{
						return 	(std::isalnum(x) != 0) ||
								(x == '-') ||
								(x == '_');								
					};
					
					// Additionally to only containing valid characters defined above,
					// a valid long name cannot start with the character '-'.
					if(!::std::all_of(t_str.begin(), t_str.end(), t_test) || t_str[0] == '-')
						ut::throwf<::std::runtime_error>("Invalid long name: \"%s\"", t_str);
					
					m_LongName = t_str;
				}

				// Set short name.
				void dispatch(const internal::short_name_t& p_tag)
				{
					if(std::isalpha(p_tag.value()) == 0)
						ut::throwf<::std::runtime_error>("Invalid short_name: %c", p_tag.value());
					
					m_ShortName = p_tag.value();
					m_HasShortName = true;
				}
				
				// Set description
				void dispatch(const internal::description_t& p_tag)
				{
					m_Description = p_tag.value();
				}
		
				// Set category
				void dispatch(const internal::category_t& p_tag)
				{
					m_Category = p_tag.value();
				}

				// Set required.
				void dispatch(internal::required_t)
				{
					m_Required = true;
				}
				
				// Set reference to flag that gets set when this argument
				// is supplied.
				void dispatch(const internal::supplied_t& p_tag)
				{
					m_HasSuppliedRef = true;
					m_SuppliedRef = p_tag.value();
				}

				// Set optional.
				void dispatch(internal::optional_t)
				{
					m_Required = false;
				}

				// Set id.
				void dispatch(internal::id_t p_id)
				{
					m_HasId = true;
					m_Id = p_id.value();
				}


			protected:
				bool			m_Supplied{false};
				bool			m_Required{false};
				bool			m_HasShortName{false};

				::std::string	m_LongName{};
				char			m_ShortName{};
				
				::std::string	m_Description{};
				::std::string	m_Category{};

				bool			m_HasId{false};
				::std::size_t	m_Id{};
				
				bool			m_HasSuppliedRef{false};
				bool*			m_SuppliedRef{nullptr};
		};
	}
}
