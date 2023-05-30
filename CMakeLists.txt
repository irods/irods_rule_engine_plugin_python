cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
# 3.12: FindPython
# 3.13: target_link_options

find_package(IRODS REQUIRED)
set(IRODS_PLUGIN_REVISION "1")
set(IRODS_PLUGIN_VERSION "${IRODS_VERSION}.${IRODS_PLUGIN_REVISION}")

set(IRODS_PACKAGE_REVISION "1")

include(IrodsCXXCompiler)
set(CMAKE_CXX_STANDARD ${IRODS_CXX_STANDARD})
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-Wl,--enable-new-dtags -Wl,--as-needed -Wl,-z,defs")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-Wl,--enable-new-dtags -Wl,--as-needed -Wl,-z,defs")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE_INIT "-Wl,--gc-sections -Wl,-z,combreloc")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE_INIT "-Wl,--gc-sections -Wl,-z,combreloc")
include(IrodsRunpathDefaults)

project(irods_rule_engine_plugin-python
  VERSION "${IRODS_PLUGIN_VERSION}"
  LANGUAGES CXX)

find_package(Threads REQUIRED)

include(${IRODS_TARGETS_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(GNUInstallDirs)
include(UseLibCXX)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build {Debug, Release}." FORCE)
  message(STATUS "Setting unspecified CMAKE_BUILD_TYPE to '${CMAKE_BUILD_TYPE}'")
endif()

if (NOT IRODS_PYTHON_VERSION)
  set(IRODS_PYTHON_VERSION "3" CACHE STRING "The version of python to link against" FORCE)
  message(STATUS "Setting unspecified IRODS_PYTHON_VERSION to '${IRODS_PYTHON_VERSION}'. This is the correct setting for normal builds.")
endif()

find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)
find_package(nlohmann_json "3.6.1" REQUIRED)
find_package(Python "${IRODS_PYTHON_VERSION}" REQUIRED COMPONENTS Development)

get_target_property(__PYTHON_IMPORTED_LOCATION Python::Python IMPORTED_LOCATION)
if (NOT IRODS_PYTHON_SONAME)
  get_target_property(IRODS_PYTHON_SONAME Python::Python IMPORTED_SONAME)
  if (NOT IRODS_PYTHON_SONAME)
    if (__PYTHON_IMPORTED_LOCATION)
      if (IS_SYMLINK "${__PYTHON_IMPORTED_LOCATION}")
        file(READ_SYMLINK "${__PYTHON_IMPORTED_LOCATION}" __PYTHON_LIBRARY_PATH)
      else()
        set(__PYTHON_LIBRARY_PATH "${__PYTHON_IMPORTED_LOCATION}")
      endif()
      get_filename_component(IRODS_PYTHON_SONAME "${__PYTHON_LIBRARY_PATH}" NAME)
    else()
      # We might be able to use python-config before falling back to guessing like this.
      if (Python_VERSION_MAJOR EQUAL "3" AND Python_VERSION_MINOR LESS "8")
        set(__PYTHON_VERSION_SUFFIX "m")
      else()
        set(__PYTHON_VERSION_SUFFIX "")
      endif()
      set(IRODS_PYTHON_SONAME "${CMAKE_SHARED_LIBRARY_PREFIX}python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}${__PYTHON_VERSION_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}.1.0")
    endif()
  endif()
  message("Python SONAME detected as ${IRODS_PYTHON_SONAME}")
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/irods/private/re/python")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/config.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/irods/private/re/python/config.hpp"
  @ONLY
)

set(
  PLUGIN
  irods_rule_engine_plugin-python
  )

add_library(
  ${PLUGIN}
  MODULE
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/irods_errors.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/irods_types.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/standard/arrays.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/standard/containers.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/standard/pointers.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/system/netinet/in.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/system/sys/stat.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/system/sys/time.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/bulkDataObjPut.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/dataCopy.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/dataObjCopy.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/dataObjInpOut.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/dataObjOpenAndStat.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/execCmd.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/fileCreate.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/fileLseek.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/fileOpen.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/fileRename.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/getRodsEnv.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/guiProgressCallback.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/icatStructs.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/irods_hierarchy_parser.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/irods_plugin_context.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/irods_re_structs.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/irods_threads.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/miscUtil.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/msParam.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/modAccessControl.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/objInfo.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/objStat.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/rcConnect.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/rodsDef.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/rodsError.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/rodsGeneralUpdate.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/rodsGenQuery.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/rodsType.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/rodsUser.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/ruleExecSubmit.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/ruleExecDel.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/specificQuery.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/structFileExtAndReg.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/structFileSync.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/types/irods/subStructFileRead.cpp"
  )

set(
  ${PLUGIN}_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/irods_errors.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/irods_types.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/array_indexing_suite.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/array_ref.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/init_struct.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/raw_constructor.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/type_sequence.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/config.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/standard/arrays.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/standard/containers.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/standard/pointers.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/system/netinet/in.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/system/sys/stat.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/system/sys/time.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/bulkDataObjPut.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/dataCopy.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/dataObjCopy.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/dataObjInpOut.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/dataObjOpenAndStat.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/execCmd.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/fileCreate.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/fileLseek.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/fileOpen.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/fileRename.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/getRodsEnv.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/guiProgressCallback.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/icatStructs.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/irods_hierarchy_parser.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/irods_plugin_context.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/irods_re_structs.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/irods_threads.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/miscUtil.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/msParam.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/modAccessControl.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/objInfo.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/objStat.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/rcConnect.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/rodsDef.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/rodsError.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/rodsGeneralUpdate.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/rodsGenQuery.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/rodsType.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/rodsUser.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/ruleExecDel.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/ruleExecSubmit.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/specificQuery.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/structFileExtAndReg.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/structFileSync.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/irods/private/re/python/types/irods/subStructFileRead.hpp"
  )
target_sources(${PLUGIN} PRIVATE ${${PLUGIN}_HEADERS})

target_include_directories(
  ${PLUGIN}
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  ${IRODS_INCLUDE_DIRS}
  ${IRODS_EXTERNALS_FULLPATH_BOOST}/include
  ${IRODS_EXTERNALS_FULLPATH_FMT}/include
  )

target_link_libraries(
  ${PLUGIN}
  PRIVATE
  ${IRODS_EXTERNALS_FULLPATH_BOOST}/lib/libboost_python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}.so
  ${IRODS_EXTERNALS_FULLPATH_BOOST}/lib/libboost_system.so
  ${IRODS_EXTERNALS_FULLPATH_BOOST}/lib/libboost_date_time.so
  ${IRODS_EXTERNALS_FULLPATH_BOOST}/lib/libboost_filesystem.so
  ${IRODS_EXTERNALS_FULLPATH_FMT}/lib/libfmt.so
  ${CMAKE_DL_LIBS}
  irods_common
  irods_server
  nlohmann_json::nlohmann_json
  )

# Since we are embedding Python rather than just creating extension modules, we must link to
# libpython. However, the build-time linker will silently drop libpython unless we pass
# --no-as-needed just before libpython. Furthermore, Python::Python might specify multiple
# libraries, and other libraries may be passed after libpython. To mitigate this, we toggle off
# --as-needed just for Python::Python's IMPORTED_LOCATION. If we can't fetch IMPORTED_LOCATION, we
# just toggle it for Python::Python.
if (__PYTHON_IMPORTED_LOCATION)
  target_link_libraries(
    ${PLUGIN}
    PRIVATE
    Python::Python
    -Wl,--no-as-needed
    "${__PYTHON_IMPORTED_LOCATION}"
    -Wl,--as-needed
    )
else()
  target_link_libraries(
    ${PLUGIN}
    PRIVATE
    -Wl,--no-as-needed
    Python::Python
    -Wl,--as-needed
    )
endif()

# Disable lazy-loading
target_link_options(
  ${PLUGIN}
  PRIVATE
  -Wl,-z,now
   )

target_compile_definitions(
  ${PLUGIN}
  PRIVATE
  RODS_SERVER
  ENABLE_RE
  ${IRODS_COMPILE_DEFINITIONS}
  ${IRODS_COMPILE_DEFINITIONS_PRIVATE}
  )
  
target_compile_options(${PLUGIN} PRIVATE -Wno-deprecated-volatile -Wmissing-field-initializers)

install(
  TARGETS
  ${PLUGIN}
  LIBRARY
    DESTINATION ${IRODS_PLUGINS_DIRECTORY}/rule_engines
  )

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/python_rules
  DESTINATION ${IRODS_HOME_DIRECTORY}/scripts/irods/test
  )

install(
  FILES ${CMAKE_SOURCE_DIR}/setup_python_rule_engine_as_only_rule_engine.py
  DESTINATION ${IRODS_HOME_DIRECTORY}/scripts
  )

install(
  FILES 
  ${CMAKE_SOURCE_DIR}/core.py.template
  ${CMAKE_SOURCE_DIR}/session_vars.py
  ${CMAKE_SOURCE_DIR}/genquery.py
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/irods
  )

include(IrodsCPackCommon)

list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CPACK_PACKAGING_INSTALL_PREFIX}${IRODS_HOME_DIRECTORY}")
list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CPACK_PACKAGING_INSTALL_PREFIX}${IRODS_HOME_DIRECTORY}/scripts")
list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CPACK_PACKAGING_INSTALL_PREFIX}${IRODS_HOME_DIRECTORY}/scripts/irods")
list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CPACK_PACKAGING_INSTALL_PREFIX}${IRODS_HOME_DIRECTORY}/scripts/irods/test")
list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CPACK_PACKAGING_INSTALL_PREFIX}${IRODS_HOME_DIRECTORY}/scripts/irods/test/python_rules")
list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CPACK_PACKAGING_INSTALL_PREFIX}${CMAKE_INSTALL_SYSCONFDIR}")
list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CPACK_PACKAGING_INSTALL_PREFIX}${CMAKE_INSTALL_SYSCONFDIR}/irods")

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_COMPONENTS_GROUPING IGNORE)
set(CPACK_PACKAGE_VERSION ${IRODS_PLUGIN_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The integrated Rule-Oriented Data System")

set(CPACK_DEB_COMPONENT_INSTALL OFF)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)
set(CPACK_DEBIAN_PACKAGE_RELEASE "${IRODS_PACKAGE_REVISION}")
if (IRODS_LINUX_DISTRIBUTION_VERSION_CODENAME)
  set(CPACK_DEBIAN_PACKAGE_RELEASE "${CPACK_DEBIAN_PACKAGE_RELEASE}~${IRODS_LINUX_DISTRIBUTION_VERSION_CODENAME}")
endif()
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

set(CPACK_RPM_COMPONENT_INSTALL OFF)
set(CPACK_RPM_PACKAGE_LICENSE "BSD-3-Clause")
set(CPACK_RPM_PACKAGE_AUTOREQ 0)
set(CPACK_RPM_PACKAGE_AUTOPROV 0)
set(CPACK_RPM_PACKAGE_RELEASE "${IRODS_PACKAGE_REVISION}")
set(CPACK_RPM_FILE_NAME RPM-DEFAULT)

set(CPACK_ARCHIVE_COMPONENT_INSTALL OFF)

set(CPACK_DEBIAN_PACKAGE_NAME "irods-rule-engine-plugin-python")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "${IRODS_PACKAGE_DEPENDENCIES_STRING}, irods-runtime (= ${IRODS_VERSION}), libc6, libpython${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")

set(CPACK_RPM_PACKAGE_NAME "irods-rule-engine-plugin-python")
if (IRODS_LINUX_DISTRIBUTION_NAME STREQUAL "centos" OR IRODS_LINUX_DISTRIBUTION_NAME STREQUAL "centos linux")
  set(CPACK_RPM_PACKAGE_REQUIRES "${IRODS_PACKAGE_DEPENDENCIES_STRING}, irods-runtime = ${IRODS_VERSION}, openssl, python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}-libs")
elseif (IRODS_LINUX_DISTRIBUTION_NAME STREQUAL "opensuse")
  set(CPACK_RPM_PACKAGE_REQUIRES "${IRODS_PACKAGE_DEPENDENCIES_STRING}, irods-runtime = ${IRODS_VERSION}, libopenssl1_0_0")
endif()

if (NOT CPACK_GENERATOR)
  set(CPACK_GENERATOR ${IRODS_CPACK_GENERATOR} CACHE STRING "CPack generator to use, e.g. {DEB, RPM, TGZ}." FORCE)
  message(STATUS "Setting unspecified CPACK_GENERATOR to ${CPACK_GENERATOR}. This is the correct setting for normal builds.")
endif()

include(CPack)
