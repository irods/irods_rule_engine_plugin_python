#ifndef RE_PYTHON_IRODS_ERRORS_HPP
#define RE_PYTHON_IRODS_ERRORS_HPP

// include this first to fix macro redef warnings
#include <pyconfig.h>

#include <patchlevel.h>

#if PY_VERSION_HEX < 0x03000000
extern "C" void initirods_errors();
#else
#  include <Python.h>
extern "C" PyObject* PyInit_irods_errors();
#endif

#endif // RE_PYTHON_IRODS_ERRORS_HPP
