cmake_minimum_required(VERSION 3.10)
set(LIBRARY_NAME ProtoComponents)
project(${LIBRARY_NAME} VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
# Set the include files
set(
	INCLUDES
	AppendArgumentTypes.h
	AppendArgumentTypesTypeContainer.h
	Reference.h
	ReferenceTypeContainer.h
	ExtendStruct.h
	Runnable.h
	Threadpool.h
)
set(
	WINDOWS_INCLUDES
	Windows/CompletionPortRunnable.h
)
set(
	APPLE_INCLUDES
)
set(
	UNIX_INCLUDES
)
# Set the source files
set(
	SOURCES
	Runnable.cpp
	Threadpool.cpp
)
set(
	WINDOWS_SOURCES
	Windows/CompletionPortRunnable.cpp
)
set(
	APPLE_SOURCES
)
set(
	UNIX_SOURCES
)
if (WIN32)
	list(APPEND INCLUDES ${WINDOWS_INCLUDES})
	list(APPEND SOURCES ${WINDOWS_SOURCES})
elseif(APPLE)
	list(APPEND INCLUDES ${APPLE_INCLUDES})
	list(APPEND SOURCES ${APPLE_SOURCES})
elseif (UNIX)
	list(APPEND INCLUDES ${UNIX_INCLUDES})
	list(APPEND SOURCES ${UNIX_SOURCES})
endif()

list(TRANSFORM INCLUDES PREPEND "Proto/Components/")
list(TRANSFORM SOURCES PREPEND "Src/Proto/Components/")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_sources (${PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
	$<INSTALL_INTERFACE:Include>
)