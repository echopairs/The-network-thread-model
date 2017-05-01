# Reactor
Simple impl Reactor Thread with epoll

## Introduction
在Reactor基础上增加了线程池来处理请求。实现了半同步半异步网络模式

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
 
