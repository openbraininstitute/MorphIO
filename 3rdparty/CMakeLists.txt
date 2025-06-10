find_package(HDF5 REQUIRED)

if (NOT EXTERNAL_HIGHFIVE)
  set(HIGHFIVE_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(HIGHFIVE_UNIT_TESTS OFF CACHE BOOL "" FORCE)
  set(HIGHFIVE_USE_BOOST OFF CACHE BOOL "" FORCE)
  set(HIGHFIVE_USE_INSTALL_DEPS ON CACHE BOOL "" FORCE)

  morphio_variable_push(CMAKE_POLICY_VERSION_MINIMUM)
  set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
  add_subdirectory(HighFive)
  morphio_variable_pop(CMAKE_POLICY_VERSION_MINIMUM)

  target_include_directories(HighFive SYSTEM INTERFACE)
endif()

add_subdirectory(GSL_LITE)
target_include_directories(gsl-lite SYSTEM INTERFACE)

add_library(lexertl INTERFACE)
target_include_directories(lexertl SYSTEM INTERFACE lexertl14/include)

add_subdirectory(ghc_filesystem)

