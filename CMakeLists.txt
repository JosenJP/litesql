cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0048 NEW)

project(litesql)
set(${PROJECT_NAME}_VERSION_MAJOR 4)
set(${PROJECT_NAME}_VERSION_MINOR 0)

enable_language(C)
enable_language(CXX)

OPTION(BUILD_SHARED_LIBS "Build shared libraries." ON)
# Make sure the rpath inside the binaries points to the directory where the
# libraries are installed
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-long-long -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-long-long -pedantic")

# Include all the necessary files for macros
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
include (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
include (${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
include (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
include (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
include (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)

set(PROJECT_NAME "litesql" VERSION 0.3.17.4)

option(LITESQL_WITH_MYSQL "build with mysql Backend" OFF)
option(LITESQL_WITH_SQLITE3 "build with sqlite3 backend" ON)
option(LITESQL_WITH_POSTGRES "build with postgres backend" OFF)
option(LITESQL_WITH_OCILIB "build with ocilib Backend" OFF)
#option(LITESQL_WITH_ODBC "build with ODBC Backend" ON)
option(LITESQL_WITH_DOCS "generate Documentation" OFF)
option(LITESQL_WITH_SAX_PARSER "use new SAX parser" ON)
option(LITESQL_WITH_UI "build also GUI-Version of generator" OFF)
option(LITESQL_WITH_TESTS "enable testing" ON)
option(LITESQL_WITH_EXAMPLES "enable building of examples" OFF)
option(CMAKE_SUPPRESS_REGENERATION "suppress rules to re-run cmake on changes (warning: no dependency checks!)?" OFF)


include(cmake/FindMySQL.cmake)
include(cmake/FindPostgreSQL.cmake)
include(cmake/FindSQLite3.cmake)
include(cmake/FindOCILib.cmake)


if(LITESQL_WITH_TESTS)
  enable_testing()
endif(LITESQL_WITH_TESTS)

if (LITESQL_WITH_SQLITE3 AND SQLITE3_FOUND)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBSQLITE3)
  list(APPEND BACKEND_LIBRARIES litesql_sqlite)
  list(APPEND BACKEND_INCLUDES ${SQLITE3_INCLUDE_DIRS})
  list(APPEND BACKEND_LIBRARIES ${SQLITE3_LIBRARIES})
  list(APPEND BACKEND_DEFINITIONS ${SQLITE3_DEFINITIONS})
else()
  message("can not use sqlite3")
  set(LITESQL_WITH_SQLITE3 OFF)
endif()

if (LITESQL_WITH_MYSQL AND MYSQL_FOUND)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBMYSQLCLIENT)
  list(APPEND BACKEND_INCLUDES ${MYSQL_INCLUDE_DIR})
  list(APPEND BACKEND_LIBRARIES litesql_mysql ${MYSQL_LIBRARIES})
  list(APPEND BACKEND_DEFINITIONS ${MYSQL_DEFINITIONS})
  # the precompiled mysql client libraries are compiled with libcmt - this causes duplicate symbols
else(LITESQL_WITH_MYSQL AND MYSQL_FOUND)
  message("can not use mysql")
  set(LITESQL_WITH_MYSQL OFF)
endif (LITESQL_WITH_MYSQL AND MYSQL_FOUND)

if (LITESQL_WITH_OCILIB AND OCILIB_FOUND)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS HAVE_OCILIB)
  list(APPEND BACKEND_INCLUDES ${OCILIB_INCLUDE_DIR})
  list(APPEND BACKEND_LIBRARIES litesql_ocilib ${OCILIB_LIBRARIES})
  list(APPEND BACKEND_DEFINITIONS ${OCILIB_DEFINITIONS})
else(LITESQL_WITH_OCILIB AND OCILIB_FOUND)
  message("can not use ocilib")
  set(LITESQL_WITH_OCILIB OFF)
endif (LITESQL_WITH_OCILIB AND OCILIB_FOUND)

if (LITESQL_WITH_POSTGRES AND POSTGRESQL_FOUND)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBPQ)
  list(APPEND BACKEND_INCLUDES ${POSTGRESQL_INCLUDE_DIR})
  list(APPEND BACKEND_LIBRARIES litesql_postgresql ${POSTGRESQL_LIBRARIES})
  list(APPEND BACKEND_DEFINITIONS ${POSTGRESQL_DEFINITIONS})
else (LITESQL_WITH_POSTGRES AND POSTGRESQL_FOUND)
  message("can not use postgres")
  set(LITESQL_WITH_POSTGRES OFF)
endif (LITESQL_WITH_POSTGRES AND POSTGRESQL_FOUND)

if (LITESQL_WITH_ODBC)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS HAVE_ODBC)
  message(ODBC is not supported yet)
endif (LITESQL_WITH_ODBC)

if (LITESQL_WITH_DOCS)
  add_subdirectory(docs/doxygen)
endif(LITESQL_WITH_DOCS)

if (LITESQL_WITH_SAX_PARSER)
  include (${CMAKE_ROOT}/Modules/FindEXPAT.cmake)
  mark_as_advanced(EXPAT_INCLUDE_DIRS EXPAT_LIBRARIES EXPAT_FOUND)

  message(EXPAT_LIBRARIES= ${EXPAT_LIBRARIES})
  message(EXPAT_INCLUDE_DIRS=${EXPAT_INCLUDE_DIRS})
  message(EXPAT_DEFINITIONS=${EXPAT_DEFINITIONS})

endif (LITESQL_WITH_SAX_PARSER)

check_include_files(dlfcn.h HAVE_DLFCN_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)


check_function_exists(malloc HAVE_MALLOC)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(atof HAVE_ATOF)
check_function_exists(strtoll HAVE_STRTOLL)
check_function_exists(strtof  HAVE_STRTOF)
check_function_exists(localtime_r HAVE_LOCALTIME_R)


# create config.h
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake 
		${CMAKE_CURRENT_BINARY_DIR}/config.h)

# set include directories
get_filename_component(CONFIG_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/config.h PATH)
include_directories(include ${CONFIG_INCLUDE} ${BACKEND_INCLUDES})

install(DIRECTORY include DESTINATION .
	COMPONENT devel 
	FILES_MATCHING PATTERN "*.hpp"
	PATTERN .svn EXCLUDE 
	)
install(DIRECTORY include DESTINATION .
	COMPONENT devel 
	FILES_MATCHING PATTERN "*.h"
	PATTERN .svn EXCLUDE 
	)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h 
	DESTINATION include/litesql 
	COMPONENT devel)
install(DIRECTORY src/generator/ DESTINATION include/litesql
  COMPONENT devel
  FILES_MATCHING PATTERN "*.hpp"
  PATTERN .svn EXCLUDE)

add_subdirectory(src)

export(EXPORT model)
install(EXPORT model DESTINATION model.cmake)

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR TGZ ZIP)
if(WIN32 AND NOT CYGWIN)
  set(CPACK_GENERATOR NSIS ${CPACK_GENERATOR})
endif(WIN32 AND NOT CYGWIN)
if(APPLE)
  set(CPACK_GENERATOR DragNDrop ${CPACK_GENERATOR})
endif(APPLE)
if (UNIX AND NOT APPLE AND NOT WIN32)
  set(CPACK_GENERATOR DEB ${CPACK_GENERATOR})
endif (UNIX AND NOT APPLE AND NOT WIN32)

set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-src-${PROJECT_VERSION})
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
set(CPACK_PACKAGE_CONTACT gulliver@users.sourceforge.net) 
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "litesql Development Team")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.rst")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_CURRENT_BINARY_DIR}/;/docs/html;/autom4te.cache;/build/;/win32/;/.svn/;/.git/;~$;${CPACK_SOURCE_IGNORE_FILES}")

if(WIN32 AND NOT UNIX)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}\\\\${PROJECT_VERSION}")
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  #  set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${PROJECT_NAME}.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ${PROJECT_NAME}")
  set(CPACK_NSIS_HELP_LINK "http://litesql.sourceforge.net/")
  set(CPACK_NSIS_URL_INFO_ABOUT "http://litesql.sourceforge.net/")
  set(CPACK_NSIS_CONTACT "gulliver@users.sourceforge.net")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_NSIS_MENU_LINKS "doc/html/index.html;Documentation")
else(WIN32 AND NOT UNIX)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}/${PROJECT_VERSION}")
  set(CPACK_STRIP_FILES "bin/${PROJECT_NAME}-gen")
  set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_PACKAGE_EXECUTABLES Visual-Litesql;Visual-Litesql;${PROJECT_NAME}-gen;${PROJECT_NAME}-gen)
include(CPack)

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-src-${PROJECT_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
