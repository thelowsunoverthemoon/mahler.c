cmake_minimum_required(VERSION 3.13.0)

project(musictheory
	VERSION 0.0.0
	DESCRIPTION "A simple library for music theory in pure C, with Interval, Chord, and Scale functions"
	HOMEPAGE_URL "https://github.com/thelowsunoverthemoon/musictheory.c"
	LANGUAGES "C"
)

# Without this, paths are not relative in the sources list
cmake_policy(SET CMP0076 NEW)

include(GNUInstallDirs)
include(FindPkgConfig)
include(CPack)

# The program itself
add_library(${PROJECT_NAME} SHARED)

# Sources
target_sources(${PROJECT_NAME}
	PRIVATE
		musictheory.c
)

# *.so
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Pkg-config
configure_file(${PROJECT_NAME}.pc.in
	${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
	@ONLY
)
install(FILES
	${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
	DESTINATION ${CMAKE_INSTALL_DATADIR}/pkgconfig
)

# Install target
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES
	musictheory.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
