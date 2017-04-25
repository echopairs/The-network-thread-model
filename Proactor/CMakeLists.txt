cmake_minimum_required(VERSION 2.8)

project(proactor)
set(CMAKE_CXX_FLAGS "-g -std=c++11")
include_directories(${proactor_SOURCE_DIR}/include)

set(PROACTOR_SRC
        include/utils/singleton.h
        include/utils/thread_pool.h
        include/proactor/event_handle.h
        include/proactor/proactor.h
        src/proactor/proactor.cpp
        src/proactor/proactor_impl/proactor_impl.h
        src/proactor/proactor_impl/proactor_impl.cpp
        src/proactor/proactor_impl/event_demultiplexer/event_demultiplexer.h
        src/proactor/proactor_impl/event_demultiplexer/impl/epoll_demultiplexer.h
        src/proactor/proactor_impl/event_demultiplexer/impl/epoll_demultiplexer.cpp)

add_library(proactor ${PROACTOR_SRC})

set(EXAMPLE_SER_SRC example/example_server.cpp
        include/utils/socket_help.h)

add_executable(example_server ${EXAMPLE_SER_SRC})
target_link_libraries(example_server ${proactor_SOURCE_DIR}/build/libproactor.a)

set(EXAMPLE_CLT_SRC
        example/example_client.cpp
        include/utils/socket_help.h)

add_executable(example_client ${EXAMPLE_CLT_SRC})


enable_testing()
add_subdirectory(tests)
