#ifndef RE_PYTHON_TYPES_TYPE_SEQUENCE_HPP
#define RE_PYTHON_TYPES_TYPE_SEQUENCE_HPP

#include <cstddef>

namespace
{
	template <typename T>
	struct num_elements
	{
		enum
		{
			value = 1
		};
	};

	template <typename T>
	constexpr std::size_t NumElements = num_elements<T>::value;

	template <typename T, std::size_t N>
	struct num_elements<T[N]>
	{
		enum
		{
			value = N * NumElements<T>
		};
	};

	//adapted from Xeo's implementation of integer sequence here: http://stackoverflow.com/a/17426611
	template <typename S>
	using Invoke = typename S::type;

	template <typename...>
	struct type_sequence
	{
		using type = type_sequence;
	};

	template <typename S1, typename S2>
	struct concat;
	template <typename S1, typename S2>
	using Concat = Invoke<concat<S1, S2>>;

	template <typename... T1, typename... T2>
	struct concat<type_sequence<T1...>, type_sequence<T2...>> : type_sequence<T1..., T2...>
	{
	};

	template <std::size_t N, typename S>
	struct repeat;
	template <std::size_t N, typename S>
	using Repeat = Invoke<repeat<N, S>>;

	template <std::size_t N, typename S>
	struct repeat : Concat<Repeat<N / 2, S>, Repeat<N - N / 2, S>>
	{
	};

	template <typename... T>
	struct repeat<0, type_sequence<T...>> : type_sequence<>
	{
	};

	template <typename... T>
	struct repeat<1, type_sequence<T...>> : type_sequence<T...>
	{
	};

	template <typename S>
	struct expand;
	template <typename S>
	using Expand = Invoke<expand<S>>;

	template <>
	struct expand<type_sequence<>> : type_sequence<>
	{
	};

	template <typename T, typename... Ts>
	struct expand<type_sequence<T, Ts...>>
		: Concat<Repeat<NumElements<T>, type_sequence<T>>, Expand<type_sequence<Ts...>>>
	{
	};

	template <typename... T>
	using make_expanded_sequence_for = Expand<type_sequence<T...>>;

	template <typename T1, typename T2>
	using UseFirstType = T1;
} //namespace

#endif // RE_PYTHON_TYPES_TYPE_SEQUENCE_HPP
