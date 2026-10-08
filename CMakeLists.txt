set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake_modules" ${CMAKE_MODULE_PATH})

cmake_minimum_required(VERSION 3.14)
project("HTL"
  VERSION 0.9.2.1
  DESCRIPTION "Hyper Template Library"
  HOMEPAGE_URL https://github.com/LeDYoM/htl
  LANGUAGES CXX
)

# Start common part to get CMake common scripts
include(FetchContent)

# Fetch my cmake utils package
FetchContent_Declare(
  cmake_ledyom_utils
  GIT_REPOSITORY https://github.com/LeDYoM/cmake_ledyom_utils.git
)

FetchContent_MakeAvailable(cmake_ledyom_utils)
list(APPEND CMAKE_MODULE_PATH ${cmake_ledyom_utils_SOURCE_DIR})

include(c_l_u_a)

# Finish common part
setup_common()

set(target htl)
add_library(${target} STATIC)

target_sources(${target}
  PUBLIC
  FILE_SET CXX_MODULES
  FILES
  src/array.cppm
  src/memory/alloc_func.cppm
  src/memory/allocator.cppm
  src/memory/allocatorfunc.cppm
  src/memory/placement.cppm
  src/algoutils.cppm
  src/basic_dictionary.cppm
  src/basic_str.cppm
  src/basic_types.cppm
  src/cast.cppm
  src/dictionary.cppm
  src/function.cppm
  src/growpolicy.cppm
  src/hash_table.cppm
  src/hash.cppm
  src/htypes.cppm
  src/lockable_vector.cppm
  src/menum.cppm
  src/p_impl_pointer.cppm
  src/pair.cppm
  src/raw_memory.cppm
  src/shared_ptr.cppm
  src/span.cppm
  src/stack.cppm
  src/str_functions.cppm
  src/str_literal.cppm
  src/str_view.cppm
  src/str.cppm
  src/unique_ptr.cppm
  src/vector_base.cppm
  src/vector_storage.cppm
  src/vector.cppm
  src/weak_ptr.cppm
)

if(MSVC)
  target_compile_options(${target} INTERFACE
    "/wd4625;"  # Copy constructor generated
    "/wd4820;"  # Padding bytes
    )
endif()

set_my_standards_cxx_options(${target} 23)
add_subdirectory(tests)
