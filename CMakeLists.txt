cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(CrossGuid
		VERSION 0.2.4
		DESCRIPTION "A cross platform GUID library - forked from Graeme Hill, maintained by Matt Jones and NovelRT contributors."
		HOMEPAGE_URL "https://github.com/novelrt/crossguid"
		LANGUAGES CXX
		)

add_subdirectory(src)
