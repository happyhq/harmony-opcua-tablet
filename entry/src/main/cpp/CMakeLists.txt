cmake_minimum_required(VERSION 3.4.1)
project(opcua_napi)

add_library(opcua_napi SHARED
  napi_init.cpp
  opcua_client.cpp
  third_party/open62541/open62541.c
)

target_include_directories(opcua_napi PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  third_party/open62541
)

target_compile_definitions(opcua_napi PRIVATE UA_ENABLE_AMALGAMATION)
