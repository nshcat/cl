#pragma once

#include <string>
#include <list>
#include <ut/type_traits.hxx>
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
					:	m_Supplied(false),
						m_Required(false),
						m_HasShortName(false),
						m_ShortName(' '),
						m_IsExclusive(false),
						m_HasId(false)
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
					m_LongName = p_tag.value();
				}

				// Set long name.
				void dispatch(const internal::short_name_t& p_tag)
				{
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
				bool			m_Supplied;
				bool			m_Required;
				bool			m_HasShortName;

				bool			m_IsExclusive;
				std::string		m_ExclusiveGroup;

				std::string		m_LongName;
				char			m_ShortName;
				
				std::string		m_Description;

				bool			m_HasId;
				std::size_t		m_Id;
		};
	}
}
