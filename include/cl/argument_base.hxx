#pragma once

#include <string>
#include <list>
#include <ut/type_traits.hxx>

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

			protected:
				// Sink for all unimplemented tags
				template< typename T >
				void dispatch(T)
				{
					static_assert(sizeof(T) == 0,
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

				bool			m_HasId;
				std::size_t		m_Id;
		};
	}
}
