#ifndef IRODS_RE_PYTHON_HPP
#define IRODS_RE_PYTHON_HPP

#include <cstring>
#include <type_traits>
#include <functional>

#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <boost/core/demangle.hpp>

#include <irods/irods_re_serialization.hpp>
#include <irods/irods_re_plugin.hpp>
#include <irods/irods_exception.hpp>

#include <patchlevel.h>
#include <boost/version.hpp>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if BOOST_VERSION < 108100
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif BOOST_VERSION == 108100
// Newer versions of Python have new members in some types which have not yet been added to the Boost Python
// implementation. Ignore -Wmissing-field-initializers until this has been resolved.
// See https://github.com/boostorg/python/issues/418
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#include <boost/python.hpp>
#pragma GCC diagnostic pop

#include <fmt/format.h>

#include "irods/private/re/python/irods_types.hpp"
#include "irods/private/re/python/irods_errors.hpp"
#include "irods/private/re/python/types/array_ref.hpp"

namespace bp = boost::python;

namespace boost::python
{
	stl_input_iterator<object> begin(const object& o)
	{
		return stl_input_iterator<object>(o);
	}
	stl_input_iterator<object> end(const object&)
	{
		return stl_input_iterator<object>();
	}
} //namespace boost::python

template <typename T>
const char* getMsParamStringFromType();

template <>
const char* getMsParamStringFromType<genQueryInp_t>()
{
	return GenQueryInp_MS_T;
}

template <>
const char* getMsParamStringFromType<genQueryOut_t>()
{
	return GenQueryOut_MS_T;
}

template <>
const char* getMsParamStringFromType<keyValPair_t>()
{
	return KeyValPair_MS_T;
}

template <>
const char* getMsParamStringFromType<bytesBuf_t>()
{
	return BUF_LEN_MS_T;
}

template <>
const char* getMsParamStringFromType<fileLseekOut_t>()
{
	return DataObjLseekOut_MS_T;
}

template <>
const char* getMsParamStringFromType<rodsObjStat_t>()
{
	return RodsObjStat_MS_T;
}

template <>
const char* getMsParamStringFromType<int>()
{
	return INT_MS_T;
}

template <>
const char* getMsParamStringFromType<float>()
{
	return FLOAT_MS_T;
}

namespace
{
	boost::python::object object_from_msParam(msParam_t& msParam)
	{
		if (!msParam.inOutStruct) {
			return {};
		}
		else if (!msParam.type) {
			THROW(SYS_NOT_SUPPORTED, "msParam type is null");
		}
		else if (std::strcmp(msParam.type, GenQueryInp_MS_T) == 0) {
			return boost::python::object{*static_cast<genQueryInp_t*>(msParam.inOutStruct)};
		}
		else if (std::strcmp(msParam.type, GenQueryOut_MS_T) == 0) {
			return boost::python::object{*static_cast<genQueryOut_t*>(msParam.inOutStruct)};
		}
		else if (std::strcmp(msParam.type, KeyValPair_MS_T) == 0) {
			return boost::python::object{*static_cast<keyValPair_t*>(msParam.inOutStruct)};
		}
		else if (std::strcmp(msParam.type, DataObjLseekOut_MS_T) == 0) {
			return boost::python::object{*static_cast<fileLseekOut_t*>(msParam.inOutStruct)};
		}
		else if (std::strcmp(msParam.type, RodsObjStat_MS_T) == 0) {
			return boost::python::object{*static_cast<rodsObjStat_t*>(msParam.inOutStruct)};
		}
		else if (std::strcmp(msParam.type, INT_MS_T) == 0) {
			return boost::python::object{*static_cast<int*>(msParam.inOutStruct)};
		}
		else if (std::strcmp(msParam.type, FLOAT_MS_T) == 0) {
			return boost::python::object{*static_cast<float*>(msParam.inOutStruct)};
		}
		else if (std::strcmp(msParam.type, BUF_LEN_MS_T) == 0) {
			// clang-format off
			return msParam.inpOutBuf
			     ? boost::python::object{bytesBuf_t{msParam.inpOutBuf->len, msParam.inpOutBuf->buf}}
			     : boost::python::object{};
			// clang-format on
		}
		else {
			THROW(SYS_NOT_SUPPORTED, fmt::format("Unknown type in msParam: [{0}]", msParam.type));
		}
	}

	template <class T>
	msParam_t msParam_from_object_impl(boost::python::object& obj)
	{
		boost::python::extract<T&> extr{obj};
		if (extr.check()) {
			return {
				.label = nullptr,
				.type = strdup(getMsParamStringFromType<T>()),
				.inOutStruct = &extr(),
				.inpOutBuf = nullptr
			};
		}
		else {
			THROW(SYS_NOT_SUPPORTED, "Attempted to extract a boost::python::object containing a non-conforming type");
		}
	}

	template <>
	msParam_t msParam_from_object_impl<int>(boost::python::object& obj)
	{
		boost::python::extract<int> extr{obj};
		if (extr.check()) {
			return {
				.label = nullptr,
				.type = strdup(getMsParamStringFromType<int>()),
				.inOutStruct = new int{extr()},
				.inpOutBuf = nullptr
			};
		}
		else {
			THROW(SYS_NOT_SUPPORTED, "Attempted to extract a boost::python::object containing a non-conforming type");
		}
	}

	template <>
	msParam_t msParam_from_object_impl<float>(boost::python::object& obj)
	{
		boost::python::extract<float> extr{obj};
		if (extr.check()) {
			return {
				.label = nullptr,
				.type = strdup(getMsParamStringFromType<float>()),
				.inOutStruct = new float{extr()},
				.inpOutBuf = nullptr
			};
		}
		else {
			THROW(SYS_NOT_SUPPORTED, "Attempted to extract a boost::python::object containing a non-conforming type");
		}
	}

	template <>
	msParam_t msParam_from_object_impl<bytesBuf_t>(boost::python::object& obj)
	{
		boost::python::extract<bytesBuf_t> extr{obj};
		if (extr.check()) {
			bytesBuf_t buf = extr();
			return {
				.label = nullptr,
				.type = strdup(getMsParamStringFromType<bytesBuf_t>()),
				.inOutStruct = new int{buf.len},
				.inpOutBuf = new bytesBuf_t{buf}
			};
		}
		else {
			THROW(SYS_NOT_SUPPORTED, "Attempted to extract a boost::python::object containing a non-conforming type");
		}
	}

	template <>
	msParam_t msParam_from_object_impl<msParam_t>(boost::python::object& obj)
	{
		boost::python::extract<msParam_t&> extr{obj};
		if (extr.check()) {
			return extr();
		}
		else {
			THROW(SYS_NOT_SUPPORTED, "Attempted to extract a boost::python::object containing a non-conforming type");
		}
	}
} //namespace

template <class T>
msParam_t msParam_from_object(boost::python::object& obj)
{
	return msParam_from_object_impl<T>(obj);
}

template <class T1, class T2, class... Ts>
msParam_t msParam_from_object(boost::python::object& obj)
{
	try {
		return msParam_from_object_impl<T1>(obj);
	}
	catch (const irods::exception&) {
		return msParam_from_object<T2, Ts...>(obj);
	}
}

//for value types
template <typename T, typename std::enable_if_t<!std::is_pointer<T>{}>* = nullptr>
void update_argument_impl([[maybe_unused]] boost::any& cpp_arg, [[maybe_unused]] boost::python::object& py_arg)
{
}

//for non-const single-pointer types
template <typename T,
          typename std::enable_if_t<std::is_pointer<T>{}>* = nullptr,
          typename std::enable_if_t<!std::is_const<std::remove_pointer_t<T>>{}>* = nullptr,
          typename std::enable_if_t<!std::is_pointer<std::remove_pointer_t<T>>{}>* = nullptr>
void update_argument_impl(boost::any& cpp_arg, boost::python::object& py_arg)
{
	const boost::python::extract<std::remove_pointer_t<T>> py_arg_extracted{py_arg};
	T cpp_arg_extracted = boost::any_cast<T>(cpp_arg);
	if (cpp_arg_extracted && py_arg_extracted.check()) {
		*cpp_arg_extracted = py_arg_extracted;
	}
}

//for double-pointer types
template <typename T,
          typename std::enable_if_t<std::is_pointer<T>{}>* = nullptr,
          typename std::enable_if_t<std::is_pointer<std::remove_pointer_t<T>>{}>* = nullptr>
void update_argument_impl(boost::any& cpp_arg, boost::python::object& py_arg)
{
	const boost::python::extract<std::remove_pointer_t<std::remove_pointer_t<T>>> py_arg_extracted{py_arg};
	T cpp_arg_extracted = boost::any_cast<T>(cpp_arg);
	if (cpp_arg_extracted && *cpp_arg_extracted && py_arg_extracted.check()) {
		**cpp_arg_extracted = py_arg_extracted;
	}
}

//for const single-pointer types
template <typename T,
          typename std::enable_if_t<std::is_pointer<T>{}>* = nullptr,
          typename std::enable_if_t<std::is_const<std::remove_pointer_t<T>>{}>* = nullptr>
void update_argument_impl([[maybe_unused]] boost::any& cpp_arg, [[maybe_unused]] boost::python::object& py_arg)
{
}

template <>
void update_argument_impl<const char*>([[maybe_unused]] boost::any& cpp_arg,
                                       [[maybe_unused]] boost::python::object& py_arg)
{
}

template <>
void update_argument_impl<char*>(boost::any& cpp_arg, boost::python::object& py_arg)
{
	array_ref<char, true> py_string = boost::python::extract<array_ref<char, true>>{py_arg};
	char* cpp_string = boost::any_cast<char*>(cpp_arg);
	if (cpp_string) {
		for (int i = 0; cpp_string[i]; ++i) {
			cpp_string[i] = py_string[i];
		}
	}
}

template <>
void update_argument_impl<void*>([[maybe_unused]] boost::any& cpp_arg, [[maybe_unused]] boost::python::object& py_arg)
{
}

//for value types
template <typename T, typename std::enable_if_t<!std::is_pointer<T>{}>* = nullptr>
boost::python::object object_from_specific(boost::any& arg)
{
	return boost::python::object{boost::any_cast<T&>(arg)};
}

//for single-pointer types
template <typename T,
          typename std::enable_if_t<std::is_pointer<T>{}>* = nullptr,
          typename std::enable_if_t<!std::is_pointer<std::remove_pointer_t<T>>{}>* = nullptr>
boost::python::object object_from_specific(boost::any& arg)
{
	T arg_extracted = boost::any_cast<T>(arg);
	return arg_extracted ? boost::python::object{*arg_extracted} : boost::python::object{};
}

//for double-pointer types
template <typename T,
          typename std::enable_if_t<std::is_pointer<T>{}>* = nullptr,
          typename std::enable_if_t<std::is_pointer<std::remove_pointer_t<T>>{}>* = nullptr>
boost::python::object object_from_specific(boost::any& arg)
{
	T arg_extracted = boost::any_cast<T>(arg);
	return arg_extracted && *arg_extracted ? boost::python::object{**arg_extracted} : boost::python::object{};
}

template <>
boost::python::object object_from_specific<const char*>(boost::any& arg)
{
	const char* str = boost::any_cast<const char*>(arg);
	return str ? boost::python::object{std::string{str}} : boost::python::object{};
}

template <>
boost::python::object object_from_specific<char*>(boost::any& arg)
{
	char* str = boost::any_cast<char*>(arg);
	return str ? boost::python::object{array_ref<char, true>{str}} : boost::python::object{};
}

template <>
boost::python::object object_from_specific<void*>([[maybe_unused]] boost::any& arg)
{
	return boost::python::object{};
}

template <typename... Ts>
std::map<std::type_index, std::function<void(boost::any&, bp::object&)>> generate_update_argument_function_map(
	const type_sequence<Ts...>&)
{
	return {
		std::pair<std::type_index, std::function<void(boost::any&, bp::object&)>>{
			std::type_index{typeid(Ts)},
			std::function<void(boost::any&, bp::object&)>{&update_argument_impl<Ts>}}...,
		std::pair<std::type_index, std::function<void(boost::any&, bp::object&)>>{
			std::type_index{typeid(Ts*)},
			std::function<void(boost::any&, bp::object&)>{&update_argument_impl<Ts*>}}...,
		std::pair<std::type_index, std::function<void(boost::any&, bp::object&)>>{
			std::type_index{typeid(const Ts*)},
			std::function<void(boost::any&, bp::object&)>{&update_argument_impl<const Ts*>}}...,
		std::pair<std::type_index, std::function<void(boost::any&, bp::object&)>>{
			std::type_index{typeid(Ts**)},
			std::function<void(boost::any&, bp::object&)>{&update_argument_impl<Ts**>}}...,
		std::pair<std::type_index, std::function<void(boost::any&, bp::object&)>>{
			std::type_index{typeid(void*)},
			std::function<void(boost::any&, bp::object&)>{&update_argument_impl<void*>}}};
}

template <typename... Ts>
std::map<std::type_index, std::function<bp::object(boost::any&)>> generate_object_from_any_function_map(
	const type_sequence<Ts...>&)
{
	return {std::pair<std::type_index, std::function<bp::object(boost::any&)>>{
				std::type_index{typeid(Ts)},
				std::function<bp::object(boost::any&)>{&object_from_specific<Ts>}}...,
	        std::pair<std::type_index, std::function<bp::object(boost::any&)>>{
				std::type_index{typeid(Ts*)},
				std::function<bp::object(boost::any&)>{&object_from_specific<Ts*>}}...,
	        std::pair<std::type_index, std::function<bp::object(boost::any&)>>{
				std::type_index{typeid(const Ts*)},
				std::function<bp::object(boost::any&)>{&object_from_specific<const Ts*>}}...,
	        std::pair<std::type_index, std::function<bp::object(boost::any&)>>{
				std::type_index{typeid(Ts**)},
				std::function<bp::object(boost::any&)>{&object_from_specific<Ts**>}}...,
	        std::pair<std::type_index, std::function<bp::object(boost::any&)>>{
				std::type_index{typeid(void*)},
				std::function<bp::object(boost::any&)>{&object_from_specific<void*>}}};
}

void update_argument(boost::any& cpp_arg, boost::python::object& py_arg)
{
	static auto update_argument_function_map = generate_update_argument_function_map(list_of_irods_types{});
	try {
		return update_argument_function_map.at(std::type_index{cpp_arg.type()})(cpp_arg, py_arg);
	}
	catch (const std::out_of_range&) {
		//THROW(SYS_NOT_SUPPORTED,
		//      fmt::format("Attempted to extract from a boost::python::object containing an "
		//                  "unsupported type: {0}", boost::core::scoped_demangled_name{cpp_arg.type().name()}.get()));
	}
	catch (const boost::bad_any_cast&) {
		THROW(SYS_NOT_SUPPORTED, "Failed any_cast when updating boost::any from boost:python::object");
	}
}

boost::python::object object_from_any(boost::any& arg)
{
	static auto object_from_any_function_map = generate_object_from_any_function_map(list_of_irods_types{});
	try {
		return object_from_any_function_map.at(std::type_index{arg.type()})(arg);
	}
	catch (const std::out_of_range&) {
		std::map<std::string, std::string> arg_map{};
		auto err = irods::re_serialization::serialize_parameter(arg, arg_map);
		if (err.ok()) {
			bp::dict arg_dict{};
			for (auto it = arg_map.begin(); it != arg_map.end(); ++it) {
				arg_dict[it->first] = it->second;
			}
			return arg_dict;
		}
		else {
			THROW(SYS_NOT_SUPPORTED,
			      fmt::format("Attempted to create a boost::python::object from a boost::any containing an "
			                  "unsupported type: {0}", boost::core::scoped_demangled_name{arg.type().name()}.get()));
		}
	}
	catch (const boost::bad_any_cast&) {
		THROW(SYS_NOT_SUPPORTED, "Failed any_cast when creating boost:python::object from boost::any");
	}
}

#endif // IRODS_RE_PYTHON_HPP
