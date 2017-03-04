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
				virtual void read_end() = 0;

				virtual bool is_switch()
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

				bool exclusive() const
				{
					return m_IsExclusive;
				}

				const std::string& exclusive_group() const
				{
					return m_ExclusiveGroup;
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
					
					if(!::std::all_of(t_str.begin(), t_str.end(), t_test))
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

				// Set required.
				void dispatch(internal::required_t)
				{
					m_Required = true;
				}

				// Set optional.
				void dispatch(internal::optional_t)
				{
					m_Required = false;
				}

				// Set exclusive group.
				void dispatch(const internal::exclusive_t& p_tag)
				{
					m_IsExclusive = true;
					m_ExclusiveGroup = p_tag.value();
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

				bool			m_IsExclusive{false};
				::std::string	m_ExclusiveGroup{};

				::std::string	m_LongName{};
				char			m_ShortName{};
				
				::std::string	m_Description{};
				::std::string	m_Category{};

				bool			m_HasId{false};
				::std::size_t	m_Id{};
		};
	}
}
