#pragma once

#include <string>
#include <cctype>
#include <ut/type_traits.hxx>


namespace cl
{
	namespace internal
	{
		// Ignore case. Used by enum_argument<E>.
		struct ignore_case_t
		{
		};

		// Flags argument as "required". If it is not supplied, an exception is thrown.
		struct required_t
		{
		};

		// Flags argument as "optional". This has no effect since all arguments are optional
		// by default. This is used to make intented meaning clear.
		struct optional_t
		{
		};

		// Flags argument as "inverse". This is used in flags that are "false" when they are present,
		// for example --NoCopy and --Copy sharing a reference.
		struct inverse_t
		{

		};

		// Instructs the argument to clamp to range instead of throwing an exception.
		// This is intended to work with min<T>/max<T>.
		struct clamp_t // TODO put these in to NS internal
		{

		};


		// Add description to argument
		class description_t; // TODO
		class category_t; // TODO


		class id_t
		{
			public:
				id_t(std::size_t p_val)
					: m_Value(p_val)
				{

				}

			public:
				std::size_t value() const
				{
					return m_Value;
				}

			private:
				std::size_t		m_Value;
		};


		// Add reference to argument that is automatically filled with read value
		template< typename T >
		class reference_t
		{
			public:
				reference_t(T& p_ref) // IN/OUT 
					: m_Ptr(std::addressof(p_ref))
				{
				}

			public:
				T* ptr() const
				{
					return m_Ptr;
				}

			private:
				T*	m_Ptr;
		};


		// Sets the "long name" of given argument. This is always needed! TODO maybe make this a fixed first constructor argument
		class long_name_t
		{
			public:
				long_name_t(const std::string& p_str)
					: m_Value(std::move(p_str))
				{

				}

			public:
				const std::string& value() const
				{
					return m_Value;
				}

			private:
				std::string		m_Value;
		};


		// Sets the "short name" of given argument. (optional)
		class short_name_t
		{
			public:
				short_name_t(char p_str)
					: m_Value(p_str)
				{

				}

			public:
				char value() const
				{
					return m_Value;
				}

			private:
				char		m_Value;
		};

		class exclusive_t
		{
			public:
				exclusive_t(const std::string& p_str)
					: m_Value(p_str)
				{

				}
				
			public:
				const std::string& value() const
				{
					return m_Value;
				}

			private:
				std::string		m_Value;
		};


		// Sets the default value of given argument.
		template< typename T >
		class default_value_t
		{
			public:
				default_value_t(const T& p_val) /* We are doing this by reference, since T could be some complex type */
					: m_Value(p_val)
				{

				}

			public:
				const T& value() const
				{
					return m_Value;
				}

			private:
				T	m_Value;
		};


		template< typename T >
		class max_t
		{
			public:
				max_t(T p_val) // We do not expect complex types here => copy
					: m_Value(p_val)
				{

				}

			public:
				const T& value() const
				{
					return m_Value;
				}

			private:
				T	m_Value;
		};

		template< typename T >
		class min_t
		{
			public:
				min_t(T p_val) 
					: m_Value(p_val)
				{

				}
	
			public:
				const T& value() const
				{
					return m_Value;
				}

			private:
				T	m_Value;
		};

		template< typename E >
		class enum_value_t
		{
			public:
				enum_value_t(const std::string& p_key, E p_val)
					: m_Key(p_key), m_Value(p_val)
				{

				}
	
			public:
				const E& value() const
				{
					return m_Value;
				}

				const std::string& key() const
				{
					return m_Key;
				}

			private:
				std::string	m_Key;
				E			m_Value;
		};
	}


	template< typename T >
	static internal::min_t<T> min(T p_val)
	{
		return internal::min_t<T>(p_val);
	}

	template< typename T >
	static internal::max_t<T> max(T p_val)
	{
		return internal::max_t<T>(p_val);
	}

	template< typename T >
	static internal::reference_t<T> reference(T& p_val)
	{
		return internal::reference_t<T>(p_val);
	}

	template< typename T >
	static internal::default_value_t<T> default_value(const T& p_val)
	{
		return internal::default_value_t<T>(p_val);
	}

	static internal::long_name_t long_name(const std::string& p_str)
	{
		if (p_str.empty())
			throw std::runtime_error("Invalid long name! (empty string not allowed)");
		else return internal::long_name_t(p_str);
	}

	static internal::short_name_t short_name(char p_chr)
	{
		if (!std::isalpha(p_chr))
			throw std::runtime_error("Invalid short name!");  // TODO better message
		else return internal::short_name_t(p_chr);
	}

	static internal::exclusive_t exclusive(const std::string& p_str)
	{
		if (p_str.empty())
			throw std::runtime_error("Invalid exclusive group name! (empty string not allowed)");
		else return internal::exclusive_t(p_str);
	}

	template< typename E >
	static internal::enum_value_t<E> enum_value(const std::string& p_key, E p_val)
	{
		return internal::enum_value_t<E>(p_key, p_val);
	}

	template< typename T >
	static internal::id_t id(T t)
	{
		static_assert(ut::is_static_castable<T, std::size_t>::value,
			"ID type has to be castable to size_t!");

		return internal::id_t(static_cast<std::size_t>(t));
	}

	// Constant instances for simple tags
	static constexpr internal::inverse_t inverse{};
	static constexpr internal::optional_t optional{};
	static constexpr internal::required_t required{};
	static constexpr internal::clamp_t clamp{};
	static constexpr internal::ignore_case_t ignore_case{};
}