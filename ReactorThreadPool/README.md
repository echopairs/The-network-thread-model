# Reactor
Simple impl Reactor with epoll

## Introduction
```
Reactor/

  |- .idea/             IDE生成文件(排除在版本控制库之外)
  |- example/           Demo例子
  |- build/             项目构建的工作目录 （排除在版本控制库之外）  
  |- include/           发布需要的头文件目录  
  |   |-reactor/        对外接口文件 
  |   |-unit/           辅助头文件
  |- release/           使用发布脚本发布的二进制文件保存目录 （排除在版本控制库之外） 
  |- test/              单元测试目录
  |- src/               项目源代码目录 
  |   |- reactor/ 		  核心模块源码
  |       |- reactor_impl/ pimpl手法具体实现
  |       |- reactor.cpp
  |   |- third_parties/   第三方依赖库(排除咋控制库之外) 
  |- tools/             构建/发布/测试等相关脚本 
  |- CMakeLists.txt     编译文件
```

## Requirement
### Tools
  1. cmake >= 3.5
  2. g++5.3.1
### Build
1. 安装依赖
```./tools/lib_update_posix.sh```
2. 构建
 ```./tools/build_without_lib_update.sh```

### Tips
  1. event_handler.h 为事件接口，需自己实现相应的event handle可仿照example中的事件
  
  2. reactor.h 提供注册和注销事件的接口
 
