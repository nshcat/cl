There are multiple ways that could be used to supply options:

1: By template, using a tag typename
2: By template, using an enum value
3: By constructor, using an enum value

First method:

cl::group_mode< cl::exclusive >


Can be dispatched like this:

```c++
template< typename T >
void dispatch(group_mode<T>)
{
	dispatch(T{});
}
```

// => Would allow both cl::exclusive and cl::group_mode< cl::exclusive >, depending on what the user wants

Or alternatively, to avoid that:

```c++
template< typename T >
void dispatch(group_mode<T>)
{
	dispatch_mode(T{});
}

void dispatch_mode(exclusive)
{
	// ...
}
```

This could allow reusing tags for different purposes.



The first method has the problem that it lacks the handy namespace that scoped enums introduce. It's easier to mix up different values.
Additionally, it violates the design of the library, because nothing in the library uses templates this way (altough it would be a good change? Compiletime is better than runtime)
But using an enum is not that great, because it is a lot of code duplication:

```c++
cl::error_mode(cl::error_mode::exception)

```