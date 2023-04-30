
cmake_minimum_required(VERSION 3.5)

project (FilesCollectionService)

set (CMAKE_CXX_STANDARD 20)

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