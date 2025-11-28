function(target_add_warnings target_name)
  # MSVC
  list(APPEND MSVC_FLAGS /DH5_BUILT_AS_DYNAMIC_LIB)
  # Exception handling
  # s: Enables standard C++ stack unwinding.
  # c: [assume] functions declared as extern "C" never throw a C++ exception.
  list(APPEND MSVC_FLAGS /EHsc)
  # get access to M_PI constant
  list(APPEND MSVC_FLAGS /D_USE_MATH_DEFINES)

  # clang
  if(${PROJECT_NAME}_CXX_WARNINGS)
      list(APPEND CLANG_FLAGS -Weverything)
      # Using std=c++11, no need for 98 compat
      list(APPEND CLANG_FLAGS -Wno-c++98-compat)
      list(APPEND CLANG_FLAGS -Wno-c++98-compat-pedantic)

      # allow static objects
      list(APPEND CLANG_FLAGS -Wno-global-constructors)
      list(APPEND CLANG_FLAGS -Wno-exit-time-destructors)

      list(APPEND CLANG_FLAGS -Wno-documentation)
      list(APPEND CLANG_FLAGS -Wno-documentation-unknown-command)

      list(APPEND CLANG_FLAGS -Wno-padded)
      list(APPEND CLANG_FLAGS -Wno-weak-vtables)
      list(APPEND CLANG_FLAGS -Wno-covered-switch-default)
      list(APPEND CLANG_FLAGS -Wno-poison-system-directories)
      if(${PROJECT_NAME}_WERROR)
          list(APPEND CLANG_FLAGS -Werror)
      endif()
  endif()

  # GNU
  if(${PROJECT_NAME}_CXX_WARNINGS)
    list(APPEND GNU_FLAGS -Wall)
    list(APPEND GNU_FLAGS -Wcast-align)
    list(APPEND GNU_FLAGS -Wconversion)
    list(APPEND GNU_FLAGS -Wdouble-promotion)
    list(APPEND GNU_FLAGS -Wextra)
    list(APPEND GNU_FLAGS -Wformat=2)
    list(APPEND GNU_FLAGS -Wnon-virtual-dtor)
    list(APPEND GNU_FLAGS -Wold-style-cast)
    list(APPEND GNU_FLAGS -Woverloaded-virtual)
    list(APPEND GNU_FLAGS -Wshadow)
    list(APPEND GNU_FLAGS -Wsign-conversion)
    list(APPEND GNU_FLAGS -Wunused)
    list(APPEND GNU_FLAGS -Wuseless-cast)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "6.0")
      list(APPEND GNU_FLAGS -Wduplicated-cond)
      list(APPEND GNU_FLAGS -Wmisleading-indentation)
      list(APPEND GNU_FLAGS -Wnull-dereference)
    endif()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0")
      list(APPEND GNU_FLAGS -Wduplicated-branches)
      list(APPEND GNU_FLAGS -Wlogical-op)
      list(APPEND GNU_FLAGS -Wrestrict)
    endif()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "8.0")
      list(APPEND GNU_FLAGS -Wclass-memaccess)
    endif()
    if(${PROJECT_NAME}_WERROR)
      list(APPEND GNU_FLAGS -Werror)
    endif()
  endif()

  target_compile_options(${target_name} PRIVATE
      $<$<CXX_COMPILER_ID:GNU>:${GNU_FLAGS}>
      $<$<CXX_COMPILER_ID:Clang>:${CLANG_FLAGS}>
      $<$<CXX_COMPILER_ID:MSVC>:${MSVC_FLAGS}>
  )
endfunction()
