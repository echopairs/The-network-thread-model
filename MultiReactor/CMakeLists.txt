cmake_minimum_required(VERSION 2.8.3)
project(mreactor)

INCLUDE_DIRECTORIES(${mreactor_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS "-g -std=c++11")

set(SOURCE
        main.cc
        include/mreactor/conn_task.h
        include/mreactor/event_handler.h
        include/mreactor/listen_handler.h
        include/mreactor/reactor.h
        include/mreactor/reactor_manager.h
        include/mreactor/work_reactor.h
        src/mreactor/reactor_manger.cpp
        src/mreactor/work_reactor.cpp
        src/reactor/reactor.cpp
        src/reactor/reactor_impl/reactor_impl.cpp
        src/reactor/reactor_impl/event_demultiplexer/impl/epoll_demultiplexer.cpp
        )


add_executable(main ${SOURCE})
target_link_libraries(main pthread)

enable_testing()
add_subdirectory(tests)
