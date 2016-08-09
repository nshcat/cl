#pragma once

#include <list>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <ut/throwf.hxx>

#include "argument_base.hxx"


namespace cl
{
	namespace internal
	{
		// Struct holding information about the current state of the parser.
		class parser_state final
		{
			public:
				using Targptr = std::unique_ptr<argument_base>;

			public:
				parser_state()
					: m_IsShort(false)
				{
				}

			public:
				bool& is_short()
				{
					return m_IsShort;
				}

				auto values() -> std::list<std::string>&
				{
					return m_Values;
				}

				auto names() -> std::vector<std::string>&
				{
					return m_Names;
				}			

				std::string& name()
				{
					return m_Names.front();
				}

				char short_name()
				{
					// Since empty identifiers are forbidden this will never fail
					return m_Names.front().at(0);
				}

				void cleanup()
				{
					m_Values.clear();
					m_Names.clear();
				}

			private:
				bool						m_IsShort;
				std::vector<std::string>	m_Names;
				std::list<std::string>		m_Values;
		};
	}
}