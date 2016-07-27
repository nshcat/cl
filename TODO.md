# libcl TODO list

###### Implementation
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

- [ ] Use `ut::string_view` wherever applicable.
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
```
