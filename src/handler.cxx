#include <iterator>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <ut/throwf.hxx>

#include "handler.hxx"
#include "grammar.hxx"
#include "parser_action.hxx"
#include "parser_state.hxx"




namespace cl
{
	void handler::check_required()
	{
		// Check if there ary any arguments that are required but not supplied
		for (auto& arg : m_Arguments)
		{
			auto& ptr = arg.second;

			if (ptr->required() && !ptr->supplied())
				ut::throwf<std::runtime_error>("Required argument \"--%s\" was not supplied!", ptr->long_name().c_str());
		}
	}

	void handler::check_exclusive()
	{
		// Maps group name to argument name
		std::map<std::string, std::string> t_map;

		for (auto& elem : m_Arguments)
		{
			auto& arg = elem.second;

			if (arg->supplied() && arg->exclusive())
			{
				auto it = t_map.find(arg->exclusive_group());

				if (it == t_map.end())
					t_map[arg->exclusive_group()] = arg->long_name();
				else ut::throwf<std::runtime_error>("\"--%s\": Already supplied argument \"--%s\" for exclusive group \"%s\"!",
					arg->long_name().c_str(), it->second.c_str(), it->first.c_str());
			}
		}
	}

	void handler::read(int argc, const char* argv[])
	{
		std::ostringstream ss{};

		for (int ix = 1; ix < argc; ++ix)
		{
			ss << std::quoted(argv[ix]) << ' ';
		}

		internal::parser_state state{};

		//pegtl::string_parser parser{ ss.str(), "" };
		//parser.parse<internal::R_Commandline, internal::parser_action>(*this, state);
		pegtl::parse<internal::R_Commandline, internal::parser_action>(ss.str(), "", *this, state);

		// Call read_end on all arguments to refresh references etc
		for (auto& arg : m_Arguments)
			arg.second->read_end();

		// Check required arguments
		check_required();
		check_exclusive();
	}

	auto handler::argument_long(const std::string& p_name) const -> internal::argument_base*
	{
		// TODO make throw optional using flag argument
		auto it = m_Arguments.find(p_name);

		if (it != m_Arguments.end())
			return it->second.get();
		else ut::throwf<std::runtime_error>("Unknown commandline argument: \"--%s\"", p_name.c_str());
	}

	auto handler::argument_short(char p_name) const -> internal::argument_base*
	{
		auto it = std::find_if(m_Arguments.begin(), m_Arguments.end(),
				[p_name](const auto& elem) -> bool
				{
					auto& t_tmp = elem.second;

					return t_tmp->has_short_name() && (t_tmp->short_name() == p_name);
				}
		);

		if(it != m_Arguments.end())
			return it->second.get();
		else ut::throwf<std::runtime_error>("Unknown commandline argument: \"-%c\"", p_name);
	}

	auto handler::argument_str(const std::string& p_name, bool p_short) const -> internal::argument_base*
	{
		return p_short ? argument_short(p_name.at(0)) : argument_long(p_name);
	}

	auto handler::argument(size_t p_id) const -> internal::argument_base*
	{
		auto it = m_ArgMap.find(p_id);

		if (it != m_ArgMap.end())
			return it->second.get();
		else ut::throwf<std::runtime_error>("Unknown commandline argument ID: \"%u\"", p_id);
	}


	void handler::add_internal(std::unique_ptr<internal::argument_base> p_ptr)
	{
		if (p_ptr->has_id())			
			m_ArgMap[p_ptr->id()] = ut::make_observer<internal::argument_base>(p_ptr.get());

		// Add argument and hand over ownership
		m_Arguments[p_ptr->long_name()] = std::move(p_ptr);
	}
}
