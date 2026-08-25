#ifndef RE_PYTHON_TYPES_INIT_STRUCT_HPP
#define RE_PYTHON_TYPES_INIT_STRUCT_HPP

#include <memory>
#include <tuple>
#include <type_traits>
#include "irods/private/re/python/types/raw_constructor.hpp"

#include <patchlevel.h>
#include <boost/version.hpp>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/python/dict.hpp>
#include <boost/python/errors.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/object.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/tuple.hpp>
#pragma GCC diagnostic pop

template <class T, std::enable_if_t<std::is_pointer<T>::value, int> = 0>
void populate_member(T& member, boost::python::tuple& args, int& index)
{
	if (index < boost::python::len(args)) {
		if (!boost::python::object{args[index++]}.is_none()) {
			PyErr_SetString(PyExc_RuntimeError,
			                "Pointers may not be reliably set from within the constructor at this time; instead, "
			                "substitute 'None' and assign to the member after construction");
			boost::python::throw_error_already_set();
		}
	}
	member = {};
}

template <class T, std::enable_if_t<!std::is_array<T>::value, int> = 0>
void populate_array_element(T& element, boost::python::stl_input_iterator<boost::python::object>& iter)
{
	if (iter != boost::python::stl_input_iterator<boost::python::object>{}) {
		element = boost::python::extract<T>{*(iter++)};
	}
}

template <class T, std::enable_if_t<std::is_array<T>::value, int> = 0>
void populate_array_element(T& array, boost::python::stl_input_iterator<boost::python::object>& iter)
{
	for (auto& element : array) {
		populate_array_element(element, iter);
	}
}

template <class T, std::enable_if_t<std::is_array<T>::value, int> = 0>
void populate_member(T& member, boost::python::tuple& args, int& index)
{
	if (index < boost::python::len(args)) {
		boost::python::stl_input_iterator<boost::python::object> iter{args[index++]};
		populate_array_element(member, iter);
	}
}

template <class T,
          std::enable_if_t<!std::is_array<T>::value, int> = 0,
          std::enable_if_t<!std::is_pointer<T>::value, int> = 0>
void populate_member(T& member, boost::python::tuple& args, int& index)
{
	if (index < boost::python::len(args)) {
		member = boost::python::extract<T>{args[index++]};
	}
}

template <class T, typename... MemberTypes>
struct init_c_struct
{
	inline static std::tuple<MemberTypes T::*...> members;

	static std::shared_ptr<T> fn(boost::python::tuple args, boost::python::dict)
	{
		auto result = std::shared_ptr<T>{new T{}};
		int index = 0;
		std::apply([&](auto... member) { (populate_member((*result).*member, args, index), ...); }, members);
		return result;
	}
};

template <class T, typename... MemberTypes>
boost::python::object make_init_function(MemberTypes T::*... members)
{
	init_c_struct<T, MemberTypes...>::members = std::tuple<MemberTypes T::*...>{members...};
	return boost::python::raw_constructor(&init_c_struct<T, MemberTypes...>::fn, 0);
}

#endif // RE_PYTHON_TYPES_INIT_STRUCT_HPP
