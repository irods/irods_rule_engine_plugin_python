// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/rodsGenQuery.hpp"

#include <irods/rodsGenQuery.h>

#include "irods/private/re/python/types/init_struct.hpp"
#include "irods/private/re/python/types/array_indexing_suite.hpp"
#include "irods/private/re/python/types/array_ref.hpp"

#include <patchlevel.h>
#include <boost/version.hpp>
#pragma GCC diagnostic push
#if PY_VERSION_HEX < 0x030400A2
#  pragma GCC diagnostic ignored "-Wregister"
#endif
#if PY_VERSION_HEX >= 0x03090000 && BOOST_VERSION < 107500
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/python/class.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;

bool operator==([[maybe_unused]] const sqlResult_t& s1, [[maybe_unused]] const sqlResult_t& s2)
{
	return false;
}

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_GenQueryInp()
	{
		// clang-format off
		bp::class_<genQueryInp_t>("GenQueryInp", bp::no_init)
			.def("__init__", make_init_function<genQueryInp_t>(
					&genQueryInp_t::maxRows,
					&genQueryInp_t::continueInx,
					&genQueryInp_t::rowOffset,
					&genQueryInp_t::options,
					&genQueryInp_t::condInput,
					&genQueryInp_t::selectInp,
					&genQueryInp_t::sqlCondInp))
			.def_readwrite("maxRows", &genQueryInp_t::maxRows)
			.def_readwrite("continueInx", &genQueryInp_t::continueInx)
			.def_readwrite("rowOffset", &genQueryInp_t::rowOffset)
			.def_readwrite("options", &genQueryInp_t::options)
			.def_readwrite("condInput", &genQueryInp_t::condInput)
			.def_readwrite("selectInp", &genQueryInp_t::selectInp)
			.def_readwrite("sqlCondInp", &genQueryInp_t::sqlCondInp)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_SqlResult()
	{
		// clang-format off
		bp::class_<sqlResult_t>("SqlResult", bp::no_init)
			.def("__init__", make_init_function<sqlResult_t>(
					&sqlResult_t::attriInx,
					&sqlResult_t::len,
					&sqlResult_t::value))
			.def_readwrite("attriInx", &sqlResult_t::attriInx)
			.add_property("len", &sqlResult_t::len)
			.def("row", +[](sqlResult_t *s, std::size_t row) { return std::string{&(s->value[row * s->len])}; })
			.add_property("value", +[]([[maybe_unused]] sqlResult_t *s) {
					PyErr_SetString(PyExc_RuntimeError, "Value cannot be directly accessed due to unknown number of rows, access the rows in value using \"row\" property instead.");
					bp::throw_error_already_set();
				})
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_SqlResult_array()
	{
		// clang-format off
		bp::class_<array_ref<sqlResult_t>>("SqlResult_array")
			.def(array_indexing_suite<array_ref<sqlResult_t>>{})
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_GenQueryOut()
	{
		// clang-format off
		bp::class_<genQueryOut_t>("GenQueryOut", bp::no_init)
			.def("__init__", make_init_function<genQueryOut_t>(
					&genQueryOut_t::rowCnt,
					&genQueryOut_t::attriCnt,
					&genQueryOut_t::continueInx,
					&genQueryOut_t::totalRowCount,
					&genQueryOut_t::sqlResult))
			.add_property("rowCnt", &genQueryOut_t::rowCnt)
			.add_property("attriCnt", &genQueryOut_t::attriCnt)
			.add_property("continueInx", &genQueryOut_t::continueInx)
			.add_property("totalRowCount", &genQueryOut_t::totalRowCount)
			.add_property("sqlResult", +[](genQueryOut_t *s) { return array_ref<sqlResult_t>{s->sqlResult}; })
			;
		// clang-format on
	}
} //namespace irods::re::python::types
