cmake_minimum_required(VERSION 3.6)
project(CURL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpic -frtti -fexceptions")

#release时去除符号表，不然很大，debug时不去除，因为要debug
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
endif()

if(UNIX)
  add_definitions(-fPIC)
elseif(WIN32)
  set(STATIC_FLAGS "-static-libstdc++ -static-libgcc -static")
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${STATIC_FLAGS}")
endif()
if(ANDROID AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-no-integrated-as)
endif()
if(ANDROID AND "${ANDROID_STL}" STREQUAL c++_static)
  set(CMAKE_CXX_CREATE_SHARED_LIBRARY "${CMAKE_CXX_CREATE_SHARED_LIBRARY} -latomic")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src/main/cpp/log.h
    ${CMAKE_SOURCE_DIR}/src/main/cpp/native.h
    ${CMAKE_SOURCE_DIR}/src/main/cpp/thirdparty/include
)



add_library(crypto STATIC IMPORTED)
set_target_properties(crypto
        PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/cpp/thirdparty/lib/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl
        PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/cpp/thirdparty/lib/libssl.a)


add_library(nghttp2 STATIC IMPORTED)
set_target_properties(nghttp2
        PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/cpp/thirdparty/lib/libnghttp2.a)


add_library(curl STATIC IMPORTED)
set_target_properties(curl
        PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/cpp/thirdparty/lib/libcurl.a)


set(CURL_FILES
    ${CMAKE_SOURCE_DIR}/src/main/cpp/native.cpp
    ${CMAKE_SOURCE_DIR}/src/main/cpp/thirdparty/include
)

add_library(
    curl-client
    SHARED
    ${CURL_FILES})

target_link_libraries(
   curl-client
   curl
   nghttp2
   ssl
   crypto
   log
   atomic
   z
)
