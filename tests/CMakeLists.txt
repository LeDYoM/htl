cmake_minimum_required (VERSION 3.14)
project(htl_utests CXX)

testing_init()

set(target ${PROJECT_NAME})

set(SOURCE_TESTS
    array
    basic_dictionary
    dictionary
    function
    hash
    lockable_vector
    menum
    pair
    shared_ptr
    span
    stack
    str_functions
    str_literal
    str_view
    str
    unique_ptr
    vector
    weak_ptr
)

add_test_executable(${target} SOURCE_TESTS ${SOURCE_TESTS})
set_my_standards_cxx_options(${target} 23)

target_link_libraries(${target} PRIVATE htypes)
