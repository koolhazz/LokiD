cmake_minimum_required(VERSION 2.8)

project(LokiD CXX C)

set(EXECUTABLE_OUTPUT_PATH bin)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -std=c99") # 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall ")  # 设置 cxx编译选项

set(SRC 
	src/call.c
	src/export.cc
	src/log.cc
	src/main.cc
	src/export_lua.cc
	src/worker.c
)

include_directories(./inc /home/AustinChen/lib/include)
link_directories(/home/AustinChen/lib /home/AustinChen/tools/luajit-2.0/src)

add_executable(LokiD ${SRC})
add_definitions(-DWORKERS=2)

target_link_libraries(LokiD dl m pthread rt libtolua++.a libluajit-2.1.a)
