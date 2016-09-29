# libcl TODO list

###### General
- [ ] Help printer. Requires cl::description etc to be implemented
- [ ] Use static analyzer (cppcheck f.e.) to test the code
- [ ] Check if there is a better way for tags to take strings as parameter. Maybe supply both r-value and l-value overloads.
- [ ] Rework to use `ut::string_view` where possible. Data referenced by `pegtl::input` is guarantueed to stay valid until end of parsing, so `std::string` only needs to be used where things need to be saved permanently (for example in the argument tree) 
- [ ] Add interface using `ut::optional`.
- [ ] Consider adding a get-Interface based on `ut::string_view`.
- [ ] The name of global options may be not that perfect, but they can be changed later. Mainly, there is a choice regarding the style of them: Should `libcl` supply global settings that expect a value as choice (for example `cl::error_mode(exception/returnvalue)` vs `cl::use_exceptions(true/false)`  vs `cl::use_exceptions`). First has the disadvantage that we can't name an enum the same way (=> solve that)

###### Implementation
- [ ] Think about making all options compile-time by supplying enum values as template arguments (see `txt/options.md` for ideas)
- [ ] Provide `cl::boolean_style(...)`  for `cl::boolean_argument` to allow numerical, boolalpha or both styles.
- [ ] Handle argument tags with perfect forwarding
- [ ] Provide both getters that return `ut::optional` and those that expect a default value.
- [ ] Add a way to set a boolean value if the argument was supplied at least once. Naming candidates are `cl::supplied` and `cl::reference<bool>`. This would not clash with `cl::flag_argument` since it simply overrides the `dispatch` member function.
- [ ] Add `cl::switch` (or other name: `cl::mode`), an argument that does not accept a value and simply sets the reference to a constant enum value.
- [ ] Use `ut::string_view` wherever applicable. Maybe `pegtl::input` can be used to create string view. This would replace a lot of unneeded copies since the input is guarantueed to be valid until parsing is done.

// Possible options:
cl::title() // or application_name
cl::version // or application_version  // maybe provide alias
cl::terminate // terminate on failure
cl::print_help // print help on failure
cl::ignore_case
cl::posix, cl::windows // <- Command line style. Windows needs different grammar.
cl::universal // <- Maybe. Try posix and then windows.

```


