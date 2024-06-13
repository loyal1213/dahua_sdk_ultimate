cmake_minimum_required(VERSION 3.25 FATAL_ERROR)# cmake 最低版本需求
project(dahua_sdk_ultimate LANGUAGES C CXX)# 工程名称


# 依赖库
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
#find_package(/usr/local/bin/grpc_cpp_plugin CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(glog REQUIRED)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_REFLECTION grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
set(_GRPC_GRPCPP grpc++)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)

message(STATUS "Using protobuf ${protobuf_VERSION}")
message(STATUS "Using grpc_cpp_plugin ${grpc_cpp_plugin_VERSION}")
message(STATUS "Using gRPC ${gRPC_VERSION}")
message(STATUS "Using cmake ${CMAKE_VERSION}")
message(STATUS "Using build_type: ${CMAKE_BUILD_TYPE}")

message(STATUS "C flags, Debug configuration: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release configuration: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C flags, Release configuration with Debug info: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "C++ flags, Release configuration with Debug info: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")


#设置输出路径
#SET(PROTO_DIR ${CMAKE_SOURCE_DIR}/internal/conf/)
message(STATUS "[CMAKE_SOURCE_DIR]: " ${CMAKE_SOURCE_DIR})
message(STATUS "[CMAKE_BINARY_DIR]: " ${CMAKE_BINARY_DIR})
message(STATUS "[PROTO_SRCS]: " ${PROTO_SRCS})
message(STATUS "[PROTO_HDRS]: " ${PROTO_HDRS})
#message(STATUS "[PROTO_DIR]: " ${PROTO_DIR})
#message(STATUS "[PB_DIR]: " ${PROTO_DIR})
message(STATUS "[PROTOBUF_PROTOC_EXECUTABLE|PROTO_FLAGS]: " ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS})


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

LIST(APPEND PROTO_FLAGS
        -I${CMAKE_CURRENT_SOURCE_DIR}/internal/conf
        -I${CMAKE_CURRENT_SOURCE_DIR}/api/dahua_sdk_ultimate
        #-I${CMAKE_CURRENT_SOURCE_DIR}/api/dahua_sdk_ultimate/v1
)
file(GLOB proto_files
        ${CMAKE_CURRENT_SOURCE_DIR}/internal/conf/*.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/api/dahua_sdk_ultimate/*.proto
        #${CMAKE_CURRENT_SOURCE_DIR}/api/dahua_sdk_ultimate/v1/*.proto
)

execute_process(COMMAND sh grpctool.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/api/dahua_sdk_ultimate/v1)

FOREACH (FIL ${proto_files})
    GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
    string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})
    string(REGEX MATCH "(/internal/conf/.*|/api/dahua_sdk_ultimate/.*)" OUT_PATH ${FILE_PATH})#|/api/dahua_sdk_ultimate/v1/.*
    message("CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
    message("FIL: " ${FIL})
    message("proto_files: " ${proto_files})
    message("FIL_WE: " ${FIL_WE})
    message("FILE_NAME: " ${FILE_NAME})
    message("FIL: " ${FIL})
    message("FILE_PATH: " ${FILE_PATH})
    message("OUT_PATH: " ${OUT_PATH})
    message("FIL_WE: " ${FIL_WE})
    message("PROTOBUF_PROTOC_EXECUTABLE: " ${PROTOBUF_PROTOC_EXECUTABLE})
    message("PROTO_FLAGS: " ${PROTO_FLAGS})
    message("PROTO_META_BASE_DIR: " ${PROTO_META_BASE_DIR})


    set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
    set(PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
    set(PROTO_GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc")
    set(PROTO_GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h")

    #message("grpc_cpp_plugin: " `which ${grpc_cpp_plugin}` )
    EXECUTE_PROCESS(
            # COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS}  --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
            COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} --grpc_out=${PROTO_META_BASE_DIR} --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin ${FIL}
    )

    #protobuf_generate(TARGET protoc LANGUAGE grpc CPP PATH ${CMAKE_BINARY_DIR} PROTO_FILES ${FIL})

    LIST(APPEND TOTAL_PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.h")
    LIST(APPEND TOTAL_PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.cc")
    LIST(APPEND TOTAL_PROTO_GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.h")
    LIST(APPEND TOTAL_PROTO_GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.cc")

    message("TOTAL_PROTO_HDRS: " ${TOTAL_PROTO_HDRS})
    message("TOTAL_PROTO_SRCS: " ${TOTAL_PROTO_SRCS})
    message("PROTO_SRCS: " ${PROTO_SRCS})
    message("PROTO_HDRS: " ${PROTO_HDRS})

    message("Copying " ${PROTO_SRCS} " to " ${FILE_PATH})
    file(COPY ${PROTO_SRCS} DESTINATION ${FILE_PATH})
    file(COPY ${PROTO_HDRS} DESTINATION ${FILE_PATH})
    file(COPY ${PROTO_GRPC_SRCS} DESTINATION ${FILE_PATH})
    file(COPY ${PROTO_GRPC_HDRS} DESTINATION ${FILE_PATH})

ENDFOREACH ()
# add_executable(${BIN_NAME} ${ALL_SRCS} ${TOTAL_PROTO_HDRS} ${TOTAL_PROTO_SRCS})


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/zl_tool_kit/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/general_net_sdk/include
        ${PROTO_HDRS}
        ${PROTO_GRPC_HDRS}
)

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/zl_tool_kit/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/general_net_sdk/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/yaml-cpp/lib
)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/internal/tools DIR_TOOLS_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/internal/server DIR_SERVER_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/internal/conf DIR_SERVER_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/api/dahua_sdk_ultimate DIR_API_SDK_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/internal/conf DIR_CONFIG_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/internal/device DIR_DEVICE_SRCS)

set(DAHUA_LIBS #sqlite3
        gRPC::grpc++_reflection gRPC::grpc++ protobuf::libprotobuf pthread
        ZLToolKit yaml-cpp
        avnetsdk dhconfigsdk dhnetsdk StreamConvertor
        absl::check
        absl::flags
        absl::flags_parse
        absl::log
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
)

add_executable(dahua_sdk_ultimate ${CMAKE_CURRENT_SOURCE_DIR}/cmd/dahua_sdk_ultimate/main.cpp
        ${PROTO_GRPC_SRCS} ${PROTO_GRPC_SRCS} # grpc 相关源码
        ${DIR_TOOLS_SRCS} ${DIR_SERVER_SRCS} ${DIR_DEVICE_SRCS}
        ${DIR_API_SDK_SRCS} ${DIR_CONFIG_SRCS}
)

add_executable(greeter_async_client2 ${CMAKE_CURRENT_SOURCE_DIR}/cmd/dahua_sdk_ultimate/greeter_async_client2.cc
        ${PROTO_GRPC_SRCS} ${PROTO_GRPC_SRCS} # grpc 相关源码
        ${DIR_TOOLS_SRCS} #${DIR_SERVER_SRCS}
        ${DIR_API_SDK_SRCS} ${DIR_CONFIG_SRCS} ${DIR_DEVICE_SRCS}
)

add_executable(greeter_async_client ${CMAKE_CURRENT_SOURCE_DIR}/cmd/dahua_sdk_ultimate/greeter_async_client.cc
        ${PROTO_GRPC_SRCS} ${PROTO_GRPC_SRCS} # grpc 相关源码
        ${DIR_TOOLS_SRCS} ${DIR_SERVER_SRCS} ${DIR_DEVICE_SRCS}
        ${DIR_API_SDK_SRCS} ${DIR_CONFIG_SRCS}
)


target_link_libraries(greeter_async_client2 ${DAHUA_LIBS})

target_link_libraries(greeter_async_client ${DAHUA_LIBS})

target_link_libraries(dahua_sdk_ultimate ${DAHUA_LIBS})
