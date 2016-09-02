It should be configurable how libcl handles errors:

- `cl::handler::handler` and `cl::parse` both throw exceptions
- `cl::handler::parse` will return false. (Errors in the constructor are delay until parse call)
- `cl::handler::parse` will print `help()` message and then call `std::exit(EXIT_FAILURE)` to terminate the program with clean-up.
- Execute custom error handler on error.


```c++
cl::handler
{
	// Or other way of setting an option. Not yet sure how.
	cl::error_mode<cl::custom_handler>(),
	
	cl::custom_handler(
		[](const std::string& p_msg)
		{
			LOG_F() << "Parsing of command line failed: "
					<< p_msg;

			std::exit(EXIT_FAILURE);
		}
	)
}
```






Implementation ideas:

This would require all arguments to have an instance pointer/reference to the handler. This could be done by supplying it to the read() method (since the parser has a reference to it).
This is a big refactor though!

```c++
template< typename... T >
auto errorf(ut::string_view p_fmt, T&& p_args)
	-> void
{
	return error(ut::sprintf(p_fmt, ::std::forward<T>(p_args)...);
}

auto error(const std::string& p_error)
	-> void
{
	if(m_ErrorMode == error_mode::exception)
		throw parser_exception(p_error);
	else
	{
		set_bad(p_error);
		return;
	}
}
```

`set_bad` will tank all errors other than the first one. At the end of parsing, it will be queried. If something `is_bad()`, action depending on the error mode will be taken.
(For example, return `false` or call the custom error handler with `get_bad()` as argument.)


A different Approach would be to just continue using exceptions internally and handle them at top level by taking the appropiate action. This would of course add some overhead, but would be a solution that (for the time) will not require a big refactor.