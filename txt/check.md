Allow the user to register any number of functions that will be run after parsing of the command line is done.
This allows arbitrary conditions to cause a parser failure, which will be handled like an intern one.
This has the benefit that the user does not have to repeat code that is already present in libcl.
All checks will be done in the order they were specified.

(Other name could be `cl::guard`)

Example:

```c++

std::vector<string> m_Input;

cl::handler
{
	cl::positional_argument
	{
		cl::long_name("miau"),
		cl::reference(m_Input)
	},

	cl::check
	{
		[&m_Input](const cl::handler&)
		{
			return m_Input.size() == 4;
		}
	},
	
	cl::check
	{
		// ...
	}
}
```