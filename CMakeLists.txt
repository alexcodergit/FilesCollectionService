
cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 20)

project (FilesCollectionService)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(FilesCollectionService PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(FilesCollectionService "${CMAKE_THREAD_LIBS_INIT}")
endif()

set(SOURCES
    src/collection_service_runner.cpp
    src/copy_service.cpp
    src/scan_service.cpp
    src/service_runner.cpp
    src/utils/files_generator.cpp
    src/utils/program_arguments.cpp
    src/utils/tar_archive.cpp
    src/test/collection_service_test.cpp
)

add_executable(FilesCollectionService ${SOURCES})
