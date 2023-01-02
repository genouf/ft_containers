#ifndef UTILITY_HPP
# define UTILITY_HPP

namespace ft
{
	template <bool Condition, typename T = void>
	struct enable_if {};

	template <typename T>
	struct enable_if<true, T>
	{
		typedef T type;
	};
}

#endif