#ifndef INIT_STRUCT_HPP
#define INIT_STRUCT_HPP

#include <memory>
#include "irods/private/re/python/types/raw_constructor.hpp"
#include "irods/private/re/python/types/type_sequence.hpp"

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

template <class T, typename = std::enable_if_t<std::is_pointer<T>::value>>
T populate_helper(boost::python::tuple& args,
                  int& index,
                  boost::python::stl_input_iterator<boost::python::object>&,
                  int&)
{
	if (index < boost::python::len(args)) {
		if (!boost::python::object{args[index++]}.is_none()) {
			PyErr_SetString(PyExc_RuntimeError,
			                "Pointers may not be reliably set from within the constructor at this time; instead, "
			                "substitute 'None' and assign to the member after construction");
			boost::python::throw_error_already_set();
		}
	}
	return {};
}

template <class T, typename = std::enable_if_t<std::is_array<T>::value>>
std::remove_all_extents_t<T> populate_helper(boost::python::tuple& args,
                                             int& index,
                                             boost::python::stl_input_iterator<boost::python::object>& iter,
                                             int& index_into_array)
{
	if (index < boost::python::len(args)) {
		if (index_into_array == 0) {
			iter = boost::python::stl_input_iterator<boost::python::object>{args[index]};
		}
		if (index_into_array < NumElements<T> - 1) {
			++index_into_array;
		}
		else {
			index++;
			index_into_array = 0;
		}
		if (iter != boost::python::stl_input_iterator<boost::python::object>{}) {
			return boost::python::extract<std::remove_all_extents_t<T>>{*(iter++)};
		}
	}
	return {};
}

template <class T,
          typename = std::enable_if_t<!std::is_array<T>::value>,
          typename = std::enable_if_t<!std::is_pointer<T>::value>>
T populate_helper(boost::python::tuple& args,
                  int& index,
                  boost::python::stl_input_iterator<boost::python::object>&,
                  int&)
{
	if (index < boost::python::len(args)) {
		return boost::python::extract<T>{args[index++]};
	}
	return {};
}

template <class T, typename... Ts>
struct init_c_struct;

template <class T, typename... MemberTypes, typename... ExpandedTypes>
struct init_c_struct<T, type_sequence<MemberTypes...>, type_sequence<ExpandedTypes...>>
{
	static std::shared_ptr<T> fn(boost::python::tuple args, boost::python::dict)
	{
		int index = 0;
		int index_into_array = 0;
		boost::python::stl_input_iterator<boost::python::object> iter{};
		return std::shared_ptr<T>{new T{populate_helper<ExpandedTypes>(args, index, iter, index_into_array)...}};
	}
};

template <class T, typename... MemberTypes>
boost::python::object make_init_function([[maybe_unused]] MemberTypes T::*... members)
{
	return raw_constructor(
		&init_c_struct<T, type_sequence<MemberTypes...>, make_expanded_sequence_for<MemberTypes...>>::fn, 0);
}

#endif // INIT_STRUCT_HPP
