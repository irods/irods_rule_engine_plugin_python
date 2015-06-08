cmake_minimum_required(VERSION 3.1)
project(irods_rule_engine_plugin_python)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

find_package(Boost REQUIRED COMPONENTS python date_time)
find_package(PythonInterp 2.7 REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)

add_library(${CMAKE_PROJECT_NAME} MODULE ${CMAKE_PROJECT_NAME}.cxx)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
