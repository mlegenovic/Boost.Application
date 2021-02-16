find_package(Boost REQUIRED COMPONENTS timer)

add_executable(work_queue work_queue.cpp)

target_link_libraries(work_queue PRIVATE Boost::timer)
