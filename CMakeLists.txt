cmake_minimum_required(VERSION 3.10)
set(LIBRARY_NAME ProtoComponents)
project(${LIBRARY_NAME} VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
# Set the include files
set(
	INCLUDES
	Runnable.h
)
list(TRANSFORM INCLUDES PREPEND "Proto/")
# Set the source files
set(
	SOURCES
	Runnable.cpp
)
list(TRANSFORM SOURCES PREPEND "Src/")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_sources (${PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
	$<INSTALL_INTERFACE:Include>
)