cmake_minimum_required(VERSION 3.1.0)

project(ed)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

file(GLOB HDRS "src/*.h")
file(GLOB SRCS "src/*.cpp")

include_directories(${PROJECT_NAME}
	thirdparty/glm/
	thirdparty/stb/
)

add_executable(${PROJECT_NAME} ${HDRS} ${SRCS})
