cmake_minimum_required(VERSION 3.10)

project(libx++)

set(CMAKE_CXX_STANDARD 11)  
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED)
if (Boost_FOUND) 
  include_directories(${Boost_INCLUDE_DIRS}) 
else() 
  message(FATAL_ERROR "Could not find Boost libraries!") 
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

set(XPP "xpp_")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(POOL "${XPP}pool")

set(POOL_SRC
  src/free_list.cpp
  src/memory_pool.cpp
  src/thread_pool.cpp
)


add_library("${POOL}" STATIC "${POOL_SRC}")
target_link_libraries("${POOL}" PRIVATE ${Boost_LIBRARIES}) 

add_executable(test main.cpp)

target_link_libraries(test PRIVATE "${POOL}")


