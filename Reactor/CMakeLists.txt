cmake_minimum_required(VERSION 3.1)
project(reactor)
set(CMAKE_CXX_FLAGS "-std=c++11")
include_directories(${reactor_SOURCE_DIR}/include)

set(REACTOR_SRC
        include/unit/global.h
        include/unit/singleton.h
        include/reactor/reactor.h
        include/reactor/event_handler.h
        src/reactor/reactor.cpp
        src/reactor/reactor_impl/reactor_impl.cpp
        src/reactor/reactor_impl/reactor_impl.h
        src/reactor/reactor_impl/event_demultiplexer/event_demultiplexer.h
        src/reactor/reactor_impl/event_demultiplexer/impl/epoll_demultiplexer.h
        src/reactor/reactor_impl/event_demultiplexer/impl/epoll_demultiplexer.cpp)

add_library(reactor ${REACTOR_SRC})

set(EXAMPLE_SER_SRC example/example_server.cpp
        example/request_handler.h
        include/unit/socket_help.h)
add_executable(example_server ${EXAMPLE_SER_SRC})
target_link_libraries(example_server ${reactor_SOURCE_DIR}/build/libreactor.a)

set(EXAMPLE_CLT_SRC
        example/example_client.cpp
        include/unit/socket_help.h)
add_executable(example_client ${EXAMPLE_CLT_SRC})
target_link_libraries(example_client ${reactor_SOURCE_DIR}/build/libreactor.a)



ADD_SUBDIRECTORY(test)
