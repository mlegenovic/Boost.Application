if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wno-unused-private-field)
endif ()

add_executable(tutorial1 main_stage1.cpp)
add_executable(tutorial2 main_stage2.cpp)
add_executable(tutorial3 main_stage3.cpp)
add_executable(tutorial4 main_stage4.cpp)
