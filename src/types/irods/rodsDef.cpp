// include this first for defines and pyconfig
#include "irods/private/re/python/types/config.hpp"

#include "irods/private/re/python/types/irods/rodsDef.hpp"

#include <irods/rcMisc.h>
#include <irods/rodsDef.h>

#include "irods/private/re/python/types/init_struct.hpp"
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
#include <boost/python/enum.hpp>
#include <boost/python/list.hpp>
#pragma GCC diagnostic pop

namespace bp = boost::python;

namespace irods::re::python::types
{
	namespace
	{
		static inline void set_buffer_2argument_form_(bytesBuf_t* self, PyObject *c, int L)
		{
			// The PyObject*c will be "borrowed" in the context of this function, meaning
			// we increment its reference count until the handle goes out of scope.
			bp::handle<> handle{bp::borrowed(c)};
			bp::object obj {handle};
			if (L < 0) {
				L = bp::len(obj);
			}
			self->buf = realloc(self->buf, L);
			if (self->buf) {
				self->len = L;
				auto *buf_dest = reinterpret_cast<unsigned char *>(self->buf);
				for (int i=0; i<L; i++) {
					*buf_dest++ = bp::extract<unsigned char>{ obj[i] };
				}
			}
		}

		static inline void set_buffer( bytesBuf_t* self, PyObject *c) {
			set_buffer_2argument_form_(self, c, -1);
		}
	}

	__attribute__((visibility("hidden"))) void export_irodsProt()
	{
		// clang-format off
		bp::enum_<irodsProt_t>("irodsProt")
			.value("NATIVE_PROT", irodsProt_t::NATIVE_PROT)
			.value("XML_PROT", irodsProt_t::XML_PROT)
			.export_values();
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_BytesBuf()
	{
		// clang-format off
		bp::class_<bytesBuf_t>("BytesBuf", bp::no_init)
			.def("__init__", make_init_function<bytesBuf_t>(
					&bytesBuf_t::len,
					&bytesBuf_t::buf))
			.def("get_byte", +[]( bytesBuf_t* s, std::size_t n) -> unsigned char {
					if (n >= s->len) {
						PyErr_SetString(PyExc_IndexError, "BytesBuf out-of-range access");
						throw bp::error_already_set();
					}
					return static_cast<char*>(s->buf)[n];
				})
			.def("get_bytes", +[]( bytesBuf_t* s) {
					bp::object None;
					bp::list a{};
					a.append(None);   // construct a list of length 1 and expand it...
					a *= s->len;      // ... to the desired length.
					auto buf_src = static_cast<unsigned char*>(s->buf);
					for (int j=0; j<s->len; j++) {
						a[j]=*buf_src++;
					}
					return a;
				})
			.def("clear_buffer", clearBBuf)
			.def("set_buffer",set_buffer)
			.def("set_buffer",set_buffer_2argument_form_)
			.add_property("len", &bytesBuf_t::len)
			.add_property("buf", +[](bytesBuf_t *s) { return s->buf ? bp::object{array_ref<char>{static_cast<char*>(s->buf), static_cast<std::size_t>(s->len)}} : bp::object{}; })
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_MsgHeader()
	{
		// clang-format off
		bp::class_<msgHeader_t>("MsgHeader", bp::no_init)
			.def("__init__", make_init_function<msgHeader_t>(
					&msgHeader_t::type,
					&msgHeader_t::msgLen,
					&msgHeader_t::errorLen,
					&msgHeader_t::bsLen,
					&msgHeader_t::intInfo))
			.add_property("type", +[](msgHeader_t *s) { return array_ref<char>{s->type}; })
			.add_property("msgLen", &msgHeader_t::msgLen)
			.add_property("errorLen", &msgHeader_t::errorLen)
			.add_property("bsLen", &msgHeader_t::bsLen)
			.add_property("intInfo", &msgHeader_t::intInfo)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_Version()
	{
		// clang-format off
		bp::class_<version_t>("Version", bp::no_init)
			.def("__init__", make_init_function<version_t>(
					&version_t::status,
					&version_t::relVersion,
					&version_t::apiVersion,
					&version_t::reconnPort,
					&version_t::reconnAddr,
					&version_t::cookie))
			.add_property("status", &version_t::status)
			.add_property("relVersion", +[](version_t *s) { return array_ref<char>{s->relVersion}; })
			.add_property("apiVersion", +[](version_t *s) { return array_ref<char>{s->apiVersion}; })
			.add_property("reconnPort", &version_t::reconnPort)
			.add_property("reconnAddr", +[](version_t *s) { return array_ref<char>{s->reconnAddr}; })
			.add_property("cookie", &version_t::cookie)
			;
		// clang-format on
	}

	__attribute__((visibility("hidden"))) void export_RodsHostAddr()
	{
		// clang-format off
		bp::class_<rodsHostAddr_t>("RodsHostAddr", bp::no_init)
			.def("__init__", make_init_function<rodsHostAddr_t>(
					&rodsHostAddr_t::hostAddr,
					&rodsHostAddr_t::zoneName,
					&rodsHostAddr_t::portNum,
					&rodsHostAddr_t::dummyInt))
			.add_property("hostAddr", +[](rodsHostAddr_t *s) { return array_ref<char>{s->hostAddr}; })
			.add_property("zoneName", +[](rodsHostAddr_t *s) { return array_ref<char>{s->zoneName}; })
			.add_property("portNum", &rodsHostAddr_t::portNum)
			.add_property("dummyInt", &rodsHostAddr_t::dummyInt)
			;
		// clang-format on
	}
}
