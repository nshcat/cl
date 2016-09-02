# libcl TODO list

###### General
- [ ] Use static analyzer (cppcheck f.e.) to test the code
- [ ] Check if there is a better way for tags to take strings as parameter. Maybe supply both r-value and l-value overloads.
- [ ] Rework to use `ut::string_view` where possible. Data referenced by `pegtl::input` is guarantueed to stay valid until end of parsing, so `std::string` only needs to be used where things need to be saved permanently (for example in the argument tree) 
- [ ] Add interface using `ut::optional`.
- [ ] Consider adding a get-Interface based on `ut::string_view`.
- [ ] The name of global options may be not that perfect, but they can be changed later. Mainly, there is a choice regarding the style of them: Should `libcl` supply global settings that expect a value as choice (for example `cl::error_mode(exception/returnvalue)` vs `cl::use_exceptions(true/false)`  vs `cl::use_exceptions`). First has the disadvantage that we can't name an enum the same way (=> solve that)

###### Implementation
- [X] Create `cl::arithmetic_base<T>`, a new base class deriving from `cl::value_base<T>`. It supplies `min`, `max`, `clamp` and `range`.
- [ ] Think about making all options compile-time by supplying enum values as template arguments (see `txt/options.md` for ideas)
- [X] Give `integer_argument` a tag called `cl::range`, that is the same as `min/max` combined.
- [X] Give `argument_base::dispatch_all` a flag to indicate whether to check for long_name or not. (Template argument!) This avoids the code duplication in `ut::free_argument`.
- [ ] Provide `cl::boolean_style(...)`  for `cl::boolean_argument` to allow numerical, boolalpha or both styles.
- [ ] Handle argument tags with perfect forwarding
- [ ] Extend handler constructor so that it can determine the type of an argument (as tag type) and dispatch that
- [ ] Provide both getters that return `ut::optional` and those that expect a default value.

- [ ] Let `cl::use_exceptions(true/false)` whether an error will cause an exception or just return false. For errors originating from argument constructors (for example, empty `long_name`) there should be an internal flag that gets set and causes `handler::parse` to instantly return false. Of course, error handling settings are not retroactive, so error happening before they are set will still cause an exception, as this is the default behaviour.


- [X] Give better error messages when invalid `reference_t<U>` is supplied when `reference_t<T>` is required.
```c++
template< typename U >
void dispatch(reference_t<U>)
{
    static_assert( ut::always_false<U>::value,
        "Reference token with mismatching type supplied!" );
}

void dispatch(reference_t<T>)
{
    // Use normally...
}
```
- [ ] Add a way to set a boolean value if the argument was supplied at least once. Naming candidates are `cl::supplied` and `cl::reference<bool>`. This would not clash with `cl::flag_argument` since it simply overrides the `dispatch` member function.
- [ ] Add `cl::switch` (or other name: `cl::mode`), an argument that does not accept a value and simply sets the reference to a constant enum value.
- [ ] Use `ut::string_view` wherever applicable. Maybe `pegtl::input` can be used to create string view. This would replace a lot of unneeded copies since the input is guarantueed to be valid until parsing is done.
- [ ] Allow user to pass global options along side with argument descriptors. For example:
```c++
cl::handler handlr
{
    cl::use_exceptions(true),
    cl::integer_argument<size_t>
    {
        cl::long_name("test"),
        cl::short_name("t"),
        cl::min<size_t>(0),
        cl::max<size_t>(32),
        cl::id(1)
    }
};

// Possible options:
cl::title() // or application_name
cl::version // or application_version  // maybe provide alias
cl::terminate // terminate on failure
cl::print_help // print help on failure
cl::ignore_case
cl::posix, cl::windows // <- Command line style. Windows needs different grammar.
cl::universal // <- Maybe. Try posix and then windows.

```


