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

ADD_SUBDIRECTORY(test)
