#ifndef RE_PYTHON_TYPES_RAW_CONSTRUCTOR_HPP
#define RE_PYTHON_TYPES_RAW_CONSTRUCTOR_HPP

#include <memory>
#include <functional>
#include <initializer_list>

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
#include <boost/python/make_constructor.hpp>
#include <boost/python/object.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/refcount.hpp>
#include <boost/python/detail/api_placeholder.hpp>
#pragma GCC diagnostic pop

namespace boost::python
{
	namespace detail
	{
		template <class F>
		struct raw_constructor_dispatcher
		{
			raw_constructor_dispatcher(F f)
				: f(make_constructor(f))
			{
			}

			PyObject* operator()(PyObject* args, PyObject* keywords)
			{
				borrowed_reference_t* ra = borrowed_reference(args);
				object a(ra);
				// clang-format off
				return incref(
					object(
						f(
							object(a[0]),
							object(a.slice(1, len(a))),
							keywords ? dict(borrowed_reference(keywords)) : dict()
						)
					).ptr()
				);
				// clang-format on
			}

		  private:
			object f;
		};
	} // namespace detail

	template <class F>
	object raw_constructor(F f, std::size_t min_args = 0)
	{
		// clang-format off
		return detail::make_raw_function(
			objects::py_function(
				detail::raw_constructor_dispatcher<F>(f),
				mpl::vector2<void, object>(),
				min_args + 1,
				(std::numeric_limits<unsigned>::max)()
			)
		);
		// clang-format on
	}
} //namespace boost::python

#endif // RE_PYTHON_TYPES_RAW_CONSTRUCTOR_HPP
