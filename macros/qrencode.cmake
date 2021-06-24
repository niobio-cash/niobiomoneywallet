set(QRENCODE_LIB qrencode)

file(GLOB QRENCODE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} external/libqrencode/*.c)
list(REMOVE_ITEM QRENCODE_SOURCES external/libqrencode/qrenc.c)
file(GLOB QRENCODE_HEADERS external/libqrencode/*.h)

list(APPEND QRENCODE_DEFINITIONS "__STATIC=static")
list(APPEND QRENCODE_DEFINITIONS "STATIC_IN_RELEASE=static")
list(APPEND QRENCODE_DEFINITIONS "MAJOR_VERSION=4")
list(APPEND QRENCODE_DEFINITIONS "MINOR_VERSION=1")
list(APPEND QRENCODE_DEFINITIONS "MICRO_VERSION=1")
if(WIN32)
    list(APPEND QRENCODE_DEFINITIONS "inline=__inline")
endif(WIN32)
add_library(${QRENCODE_LIB} ${QRENCODE_SOURCES} ${QRENCODE_HEADERS})
set_target_properties(${QRENCODE_LIB} PROPERTIES COMPILE_DEFINITIONS "${QRENCODE_DEFINITIONS}")

include_directories(${CMAKE_BINARY_DIR} "external/libqrencode")