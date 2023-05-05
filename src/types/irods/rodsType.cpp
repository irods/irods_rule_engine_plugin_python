// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/rodsType.hpp"

#include <irods/rodsType.h>

#include "irods/private/re/python/types/array_ref.hpp"
#include "irods/private/re/python/types/init_struct.hpp"

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
#include <boost/python/enum.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace irods::re::python::types
{
	__attribute__((visibility("hidden"))) void export_objType()
	{
		// clang-format off
		bp::enum_<objType_t>("objType")
			.value("UNKNOWN_OBJ", objType_t::UNKNOWN_OBJ_T)
			.value("DATA_OBJ", objType_t::DATA_OBJ_T)
			.value("COLL_OBJ", objType_t::COLL_OBJ_T)
			.value("UNKNOWN_FILE", objType_t::UNKNOWN_FILE_T)
			.value("LOCAL_FILE", objType_t::LOCAL_FILE_T)
			.value("LOCAL_DIR", objType_t::LOCAL_DIR_T)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_RodsStat()
	{
		// clang-format off
		bp::class_<rodsStat_t>("RodsStat", bp::no_init)
			.def("__init__", make_init_function<rodsStat_t>(
					&rodsStat_t::st_size,
					&rodsStat_t::st_dev,
					&rodsStat_t::st_ino,
					&rodsStat_t::st_mode,
					&rodsStat_t::st_nlink,
					&rodsStat_t::st_uid,
					&rodsStat_t::st_gid,
					&rodsStat_t::st_rdev,
					&rodsStat_t::st_atim,
					&rodsStat_t::st_mtim,
					&rodsStat_t::st_ctim,
					&rodsStat_t::st_blksize,
					&rodsStat_t::st_blocks))
			.add_property("st_size", &rodsStat_t::st_size)
			.add_property("st_dev", &rodsStat_t::st_dev)
			.add_property("st_ino", &rodsStat_t::st_ino)
			.add_property("st_mode", &rodsStat_t::st_mode)
			.add_property("st_nlink", &rodsStat_t::st_nlink)
			.add_property("st_uid", &rodsStat_t::st_uid)
			.add_property("st_gid", &rodsStat_t::st_gid)
			.add_property("st_rdev", &rodsStat_t::st_rdev)
			.add_property("st_atim", &rodsStat_t::st_atim)
			.add_property("st_mtim", &rodsStat_t::st_mtim)
			.add_property("st_ctim", &rodsStat_t::st_ctim)
			.add_property("st_blksize", &rodsStat_t::st_blksize)
			.add_property("st_blocks", &rodsStat_t::st_blocks)
			;
		// clang-format on
	}
	__attribute__((visibility("hidden"))) void export_RodsDirent()
	{
		// clang-format off
		bp::class_<rodsDirent_t>("RodsDirent", bp::no_init)
			.def("__init__", make_init_function<rodsDirent_t>(
					&rodsDirent_t::d_offset,
					&rodsDirent_t::d_ino,
					&rodsDirent_t::d_reclen,
					&rodsDirent_t::d_namlen,
					&rodsDirent_t::d_name))
			.add_property("d_offset", &rodsDirent_t::d_offset)
			.add_property("d_ino", &rodsDirent_t::d_ino)
			.add_property("d_reclen", &rodsDirent_t::d_reclen)
			.add_property("d_namlen", &rodsDirent_t::d_namlen)
			.add_property("d_name", +[](rodsDirent_t *s) { return array_ref<char>{s->d_name}; })
			;
		// clang-format on
	}
}
