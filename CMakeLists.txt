cmake_minimum_required(VERSION 3.8)
project(javamosc)

set(CMAKE_C_STANDARD 99)
set(MSC_SOURCE_FILES)
set(MSC_INCLUDE_DIRS)

file(GLOB SOURCE_FILES_CR src/*.c)
file(GLOB SOURCE_FILES_HR src/*.h)
file(GLOB SOURCE_FILES_INCR src/*.inc)
file(GLOB SOURCE_FILES_C src/**/*.c)
file(GLOB SOURCE_FILES_H src/**/*.h)
file(GLOB SOURCE_FILES_INC src/**/*.inc)

LIST(APPEND MSC_SOURCE_FILES ${SOURCE_FILES_C} ${SOURCE_FILES_H} ${SOURCE_FILES_CR} ${SOURCE_FILES_HR} ${SOURCE_FILES_INC} ${SOURCE_FILES_INCR})

LIST(APPEND MSC_INCLUDE_DIRS
        src
        src/vendors
        )


#include mosc dependancy
LIST(APPEND MSC_INCLUDE_DIRS
        deps/mosc/api
        deps/mosc/src/builtin
        deps/mosc/src/common
        deps/mosc/src/compiler
        deps/mosc/src/helpers
        deps/mosc/src/memory
        deps/mosc/src/meta
        deps/mosc/src/runtime
        )


file(GLOB SOURCE_FILES_C deps/mosc/src/**/*.c)
file(GLOB SOURCE_FILES_H deps/mosc/src/**/*.h)
file(GLOB SOURCE_FILES_INC deps/mosc/src/**/*.inc)
LIST(APPEND MSC_SOURCE_FILES ${SOURCE_FILES_C} ${SOURCE_FILES_H} ${SOURCE_FILES_INC})
LIST(APPEND MSC_SOURCE_FILES deps/mosc/api/msc.h)

LIST(APPEND MSC_INCLUDE_DIRS
        deps/nafamaw
        deps/nafamaw/vendors
        deps/nafamaw/packages
        )

file(GLOB SOURCE_FILES_C deps/nafamaw/**/*.c)
file(GLOB SOURCE_FILES_H deps/nafamaw/**/*.h)
file(GLOB SOURCE_FILES_INC deps/nafamaw/**/*.inc)

LIST(APPEND MSC_SOURCE_FILES ${SOURCE_FILES_C} ${SOURCE_FILES_H} ${SOURCE_FILES_INC})
LIST(APPEND MSC_SOURCE_FILES deps/nafamaw/nafamaw.c deps/nafamaw/nafamaw.h)



add_library(javamosc SHARED ${MSC_SOURCE_FILES})
TARGET_INCLUDE_DIRECTORIES(javamosc PRIVATE ${MSC_INCLUDE_DIRS})

set(SOURCE_FILES test/main.c)
add_executable(jmosctest ${SOURCE_FILES})
TARGET_INCLUDE_DIRECTORIES(jmosctest PRIVATE ${MSC_INCLUDE_DIRS})

link_directories(${PROJECT_BINARY_DIR})

target_link_libraries(jmosctest javamosc)