# libcl TODO list

###### General
- [ ] Check if there is a better way for tags to take strings as parameter. Maybe supply both r-value and l-value overloads.
- [ ] Rework to use `ut::string_view` where possible. Data referenced by `pegtl::input` is guarantueed to stay valid until end of parsing, so `std::string` only needs to be used where things need to be saved permanently (for example in the argument tree) 
- [ ] Add interface using `ut::optional`.
- [ ] Consider adding a get-Interface based on `ut::string_view`.

###### Implementation
- [ ] Extend handler constructor so that it can determine the type of an argument (as tag type) and dispatch that
- [ ] Provide both getters that return `ut::optional` and those that expect a default value.
- [ ] Give better error messages when invalid `reference_t<U>` is supplied when `reference_t<T>` is required.
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
cl::ignore_case
cl::posix, cl::windows // <- Command line style. Windows needs different grammar.
cl::universal // <- Maybe. Try posix and then windows.

```


