cmake_minimum_required(VERSION 2.8)

project(LokiD CXX C)

set(EXECUTABLE_OUTPUT_PATH bin)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall ") # 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall ")  # 设置 cxx编译选项

set(SRC 
	src/call.c
	src/connectpool.cc
	src/export.cc
	src/log.cc
	src/main.cc
	src/mysql_part.cc
	src/export_lua.cc
)

include_directories(./inc /usr/local/mysql/include /home/AustinChen/lib/include)
link_directories(/usr/local/mysql/lib /home/AustinChen/lib /home/AustinChen/tools/luajit-2.0/src)

add_executable(LokiD ${SRC})

target_link_libraries(LokiD dl m pthread mysqlclient z rt libtolua++.a libluajit-2.1.a)