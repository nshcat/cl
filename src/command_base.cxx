#include <iterator>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <map>
#include <ut/throwf.hxx>

#include "utility.hxx"
#include "command.hxx"
#include "grammar.hxx"
#include "parser_action.hxx"
#include "parser_state.hxx"

using namespace std::literals;

namespace cl
{
	namespace internal
	{
		void command_base::check_required() const
		{
			// Check if there ary any arguments that are required but not supplied
			for (auto& arg : m_ArgMap)
			{
				auto& ptr = arg.second;

				if (ptr->required() && !ptr->supplied())
				{
					ut::throwf<std::runtime_error>(
						"Required argument \"--%s\" was not supplied!",
						ptr->long_name()
					);
				}
			}
		}
		
		void command_base::add(argument_ptr p_ptr)
		{
			if(m_ArgMap.count(p_ptr->long_name()) != 0)
			{
				ut::throwf<::std::runtime_error>(
					"Tried to register multiple arguments with identical long name"
					" \"--%s\"",
					p_ptr->long_name()
				);
			}
		
			if (p_ptr->has_id())			
				m_IdMap[p_ptr->id()] = ut::make_observer<internal::argument_base>(p_ptr.get());

			// Add argument and hand over ownership
			m_ArgMap[p_ptr->long_name()] = ::std::move(p_ptr);
		}
		
		
		auto command_base::get(const ::std::string& p_str) const
			-> const_argument_view
		{
			// Lookup by long name
			const auto t_it = m_ArgMap.find(p_str);
			
			if(t_it != m_ArgMap.end())
			{
				return ut::make_observer<const internal::argument_base>(t_it->second.get());
			}
			else
			{
				ut::throwf<::std::runtime_error>(
					"Unknown commandline argument:"
					" \"--%s\"",
					p_str
				);
			}
		}
		
		auto command_base::get(char p_chr) const
			-> const_argument_view
		{
			// Lookup by short name
			const auto t_it = ::std::find_if(m_ArgMap.cbegin(), m_ArgMap.cend(),
				[p_chr](const auto& p_pair) -> bool
				{
					return p_pair.second->short_name() == p_chr;
				}
			);
			
			if(t_it != m_ArgMap.cend())
			{
				return ut::make_observer<const internal::argument_base>(t_it->second.get());
			}
			else
			{
				ut::throwf<::std::runtime_error>(
					"Unknown commandline argument:"
					" \"-%c\"",
					p_chr
				);
			}
		}
		
		auto command_base::get(::std::size_t p_id) const
			-> const_argument_view
		{
			// Lookup by id
			const auto t_it = m_IdMap.find(p_id);
			
			if(t_it != m_IdMap.cend())
			{
				return ut::make_observer<const internal::argument_base>(t_it->second.get());
			}
			else
			{
				ut::throwf<::std::runtime_error>(
					"Unknown commandline argument id:"
					" \"%u\"",
					p_id
				);
			}
		}
		
		auto command_base::get(const ::std::string& p_str)
			-> argument_view
		{
			const auto t_view = const_cast<const this_type*>(this)->get(p_str);
			
			return ut::make_observer<internal::argument_base>(
				const_cast<internal::argument_base*>(t_view.get())
			);
		}
		
		auto command_base::get(char p_short)
			-> argument_view
		{
			const auto t_view = const_cast<const this_type*>(this)->get(p_short);
			
			return ut::make_observer<internal::argument_base>(
				const_cast<internal::argument_base*>(t_view.get())
			);
		}
		
		auto command_base::get(::std::size_t p_id)
			-> argument_view
		{
			const auto t_view = const_cast<const this_type*>(this)->get(p_id);
			
			return ut::make_observer<internal::argument_base>(
				const_cast<internal::argument_base*>(t_view.get())
			);
		}
		
		
		auto command_base::has(const ::std::string& p_long) const
			-> bool
		{
			return m_ArgMap.count(p_long) != 0;
		}
		
		auto command_base::has(::std::size_t p_id) const
			-> bool
		{
			return m_IdMap.count(p_id) != 0;
		}
		
		auto command_base::has(char p_short) const
			-> bool
		{
			const auto t_it = ::std::find_if(m_ArgMap.begin(), m_ArgMap.end(),
				[p_short](const auto& p_pair) -> bool
				{
					return p_pair.second->short_name() == p_short;
				}
			);
			
			return t_it != m_ArgMap.end();
		}
		
		auto command_base::supplied(const ::std::string& p_long) const
			-> bool
		{
			return get(p_long)->supplied();
		}
		
		auto command_base::supplied(char p_short) const
			-> bool
		{
			return get(p_short)->supplied();
		}
		
		auto command_base::supplied(::std::size_t p_id) const
			-> bool
		{
			return get(p_id)->supplied();
		}
		
		
		auto command_base::read(input_type p_cl)
			-> void
		{
			::std::ostringstream ss{};

			for (const char* t_entry: p_cl)
			{
				ss << std::quoted(t_entry) << ' ';
			}

			internal::parser_state state{};

			pegtl::parse<internal::R_Commandline, internal::parser_action>(ss.str(), "", *this, state);

			// Call read_end on all arguments to refresh references etc
			for (auto& arg : m_ArgMap)
				arg.second->read_end();

			// Perform after-read tasks and checks
			check_required();
		}
		
		auto command_base::local_data() const
			-> const command_data&
		{
			return m_LocalData;
		}
		
		auto command_base::local_data()
			-> command_data&
		{
			return m_LocalData;
		}
		
		auto command_base::global_data() const
			-> const handler_data&
		{
			if(m_GlobalDataPtr)
				return *m_GlobalDataPtr;
			else throw ::std::runtime_error("Tried to access null global data ptr!");
		}
		
		auto command_base::set_global_data(global_data_ptr p_ptr)
			-> void
		{
			m_GlobalDataPtr = p_ptr;
		}
		
		auto command_base::print_help() const
			-> void
		{
			if(global_data().m_HelpMode != internal::help_mode_::show_none)
			{
				// Print usage first
				if(global_data().m_HelpMode & internal::help_mode_::show_usage)
					print_usage();
				
				if(global_data().m_HelpMode & internal::help_mode_::show_summary)
					print_summary();
			}
		}
		
		auto command_base::print_usage() const
			-> void
		{
			if(!m_LocalData.m_Usage.empty())
				std::cout << m_LocalData.m_Usage << std::endl;
		}
		
		
		
		auto command_base::print_summary() const
			-> void
		{
			// Retrieve arguments sorted by category
			const auto t_categories = sort_arguments();
			
			// Handle "general" category first
			const auto t_it = t_categories.find("General"s);
			
			if(t_it != t_categories.end())
				print_category("General"s, t_it->second);
			
			// Handle rest
			for(const auto& t_kvp: t_categories)
			{
				if(t_kvp.first != "General")
					print_category(t_kvp.first, t_kvp.second);
			}
		}
		
		auto command_base::print_category(const ::std::string& p_name, const ::std::vector<argument_view>& p_cat) const
			-> void
		{
			::std::cout << p_name << ":" << ::std::endl;
			
			::std::ostringstream t_ss{ };
			
			// Character used to fill empty space
			const auto t_fillChar = (global_data().m_SummaryStyle == summary_style_::dots ? '.' : ' ');
			
			constexpr const auto t_LengthArgumentSpace = 22U;
			constexpr const auto t_DescriptionLineLength = 47U;
			
			for(const auto& t_entry: p_cat)
			{
				// Ensure string stream is empty
				t_ss.str(""s);
			
				// Build string containing long and (possibly) short name
				t_ss << '[';
				
				if(t_entry->has_short_name())
					t_ss << '-' << t_entry->short_name() << ',';
					
				t_ss << "--" << t_entry->long_name() << "] ";
				//
				
				if(t_entry->has_description())
				{
					// -1 because the last space is set manually
					::std::cout << "    " << ::std::setfill(t_fillChar) << ::std::left << ::std::setw(t_LengthArgumentSpace-1) << t_ss.str() << ' ';
				
					// TODO: real line breaks, aka ones that do not divide words
					::std::cout << insert_line_breaks(t_entry->description(), t_DescriptionLineLength, t_LengthArgumentSpace) << ::std::endl;
				}
				else
				{
					::std::cout << "    " << t_ss.str() << ::std::endl;
				}	
			}
			
			::std::cout << ::std::endl;
		}
		
		auto command_base::sort_arguments() const
			-> category_map_type
		{
			category_map_type t_categories{ };
			
			for(const auto& t_kvp: m_ArgMap)
			{
				const auto& t_ptr = t_kvp.second;
			
				t_categories[(t_ptr->has_category() ? t_ptr->category() : "General"s)].push_back({t_ptr.get()});				
			}	
		
			return t_categories;
		}
	}
}
