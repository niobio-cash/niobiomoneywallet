cmake_minimum_required(VERSION 3.11)

include(macros/settings.cmake)
include(macros/qrencode.cmake)

project(${CN_PROJECT_NAME})

execute_process(COMMAND git log -1 --pretty=format:%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REVISION)

set(NODEQUAN_LIB nodequan)

include_directories(${CMAKE_BINARY_DIR}
        src
        "external/quan-node/external"
        "external/quan-node/include"
        "external/quan-node/src"
        "external/libqrencode")

if (WIN32)
    if (NOT MSVC)
        message(FATAL_ERROR "Only MSVC is supported on this platform")
    endif ()
    set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.0\\5.9\\msvc2017_64")
endif()

find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
if(WIN32)
    set(Boost_USE_STATIC_RUNTIME OFF)
else(WIN32)
    set(Boost_USE_STATIC_RUNTIME ON)
endif(WIN32)
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.58 REQUIRED COMPONENTS date_time filesystem program_options regex serialization system thread chrono)
if ((${Boost_MAJOR_VERSION} EQUAL 1) AND (${Boost_MINOR_VERSION} EQUAL 54))
    message(SEND_ERROR "Boost version 1.54 is unsupported, more details are available here http://goo.gl/RrCFmA")
endif ()

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(VERSION "")
configure_file("external/quan-node/src/version.h.in" "version.h")
configure_file("src/CryptoNoteWalletConfig.h.in" "CryptoNoteWalletConfig.h")

add_definitions(-DVERSION=\"${CN_VERSION}\")
add_definitions(-DGIT_REVISION=\"${GIT_REVISION}\")

set(CMAKE_AUTOMOC ON)

include(macros/node.cmake)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE HEADERS src/*.hpp)
file(GLOB_RECURSE FORMS src/gui/ui/*.ui)

set(QRC src/resources.qrc)

qt5_wrap_ui(UIS ${FORMS})
qt5_add_resources(RCC ${QRC})

if (WIN32)
    add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x0600 /DSTATICLIB)
    include_directories("external/quan-node/src/platform/msc")

    set(PLATFORM_DIR Windows)
    set(BUILD_PLATFORM WIN32)
    set(BUILD_RESOURCES src/cryptonotewallet.rc)
    set(QTMAIN Qt5::WinMain)

elseif (UNIX)
    set(NODEQUAN_SOURCES ${NODEQUAN_SOURCES} "external/quan-node/external/miniupnpc/minissdpc.c")
    if (APPLE)
        enable_language(ASM)
        file(GLOB_RECURSE OBJC_SOURCES src/*.mm)
        set(SOURCES ${SOURCES} ${OBJC_SOURCES})
        set(PLATFORM_DIR OSX)
        set(MACOSX_BUNDLE_INFO_STRING "${CN_VERSION}")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")
        set(MACOSX_BUNDLE_BUNDLE_NAME "${CN_CURRENCY_DISPLAY_NAME}")
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")
        set(MACOSX_BUNDLE_BUNDLE_VERSION "$${VERSION_VERSION}.${VERSION_MINOR}.${VERSION_PATCH}")

        find_package(Qt5PrintSupport REQUIRED)

        include_directories(/usr/include/malloc)

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -std=c++11 -stdlib=libc++")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -maes -D_DARWIN_C_SOURCE")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework CoreFoundation -framework Carbon -framework IOKit -L/usr/lib")

        set(MACOSX_BUNDLE_ICON_FILE q1v-wallet.icns)
        set(APPLICATION_ICON src/images/q1v-wallet.icns)
        set_source_files_properties(${APPLICATION_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

        set(BUILD_PLATFORM MACOSX_BUNDLE)
        set(BUILD_RESOURCES ${APPLICATION_ICON})

        GET_TARGET_PROPERTY(QT_LIB_DIR "${Qt5Widgets_LIBRARIES}" LOCATION)
        GET_FILENAME_COMPONENT(QT_LIB_DIR "${QT_LIB_DIR}" PATH)
    else(APPLE)
        set(PLATFORM_DIR Linux)
        list(REMOVE_ITEM HEADERS ${CMAKE_SOURCE_DIR}/src/gui/macdockiconhandler.h)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -std=c++11")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes -std=c11")
    endif (APPLE)
endif ()

include_directories("external/quan-node/src/Platform/${PLATFORM_DIR}")
include_directories("external/quan-node/src/Platform/Posix")
file(GLOB PLATFORM_SOURCES "external/quan-node/src/Platform/${PLATFORM_DIR}/System/*")
set(NODEQUAN_SOURCES ${NODEQUAN_SOURCES} ${PLATFORM_SOURCES})

add_library(${NODEQUAN_LIB} STATIC ${NODEQUAN_SOURCES})
set_target_properties(${NODEQUAN_LIB} PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)

if (APPLE OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(${NODEQUAN_LIB} ${Boost_LIBRARIES} -lresolv)
else ()
    target_link_libraries(${NODEQUAN_LIB} ${Boost_LIBRARIES})
endif ()

add_executable(${PROJECT_NAME}-wallet ${BUILD_PLATFORM} ${BUILD_RESOURCES} ${SOURCES} ${HEADERS} ${UIS} ${RCC})
set_target_properties(${PROJECT_NAME}-wallet PROPERTIES COMPILE_DEFINITIONS _GNU_SOURCE)
target_link_libraries(${PROJECT_NAME}-wallet ${Boost_LIBRARIES} ${QTMAIN} ${NODEQUAN_LIB} ${QRENCODE_LIB})

if (APPLE)
    qt5_use_modules(${PROJECT_NAME}-wallet PrintSupport)
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME}-wallet -lpthread)
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME}-wallet Imm32 Iphlpapi Winmm)
endif (APPLE)

qt5_use_modules(${PROJECT_NAME}-wallet Widgets Gui Network)

# Installation

set(CPACK_PACKAGE_NAME ${CN_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CN_VERSION})
set(CPACK_PACKAGE_VENDOR ${CN_PROJECT_VENDOR})
set(CPACK_PACKAGE_CONTACT ${CN_PROJECT_URL})
set(CPACK_STRIP_FILES ON)
if (APPLE)
    set(CPACK_GENERATOR DragNDrop)
    install(TARGETS ${PROJECT_NAME}-wallet BUNDLE DESTINATION .)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${VERSION_RELEASE}")
elseif (UNIX)
    find_program(RPMBUILD rpmbuild)

    install(PROGRAMS build/${PROJECT_NAME}-wallet DESTINATION bin
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    install(FILES src/quanwallet.desktop DESTINATION share/applications)
    install(FILES src/images/logo5.png DESTINATION share/pixmaps RENAME logo5.png)
    install(FILES copyright DESTINATION share/doc/${CN_PROJECT_NAME})

    if (NOT RPMBUILD)
        set(CPACK_GENERATOR DEB)
        set(CPACK_DEBIAN_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
        set(CPACK_SYSTEM_NAME 64-bit)
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR} <${CN_PROJECT_MAIL}>")
        set(CPACK_DEBIAN_PACKAGE_SECTION Office)
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
        set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CN_CURRENCY_DISPLAY_NAME} wallet
 ${CN_CURRENCY_DISPLAY_NAME} is a Brazilian decentralized, privacy oriented peer-to-peer
 cryptocurrency. It is open-source, nobody owns or controls ${CN_CURRENCY_DISPLAY_NAME}
 and everyone can take part of it.")
        set(CPACK_PACKAGE_FILE_NAME "${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${VERSION_RELEASE}${CPACK_SYSTEM_NAME}")
    else()
        set(CPACK_GENERATOR RPM)
        set(CPACK_SYSTEM_NAME x86_64)
        set(CPACK_RPM_PACKAGE_RELEASE ${VERSION_RELEASE})
        set(CPACK_RPM_PACKAGE_LICENSE "MIT")
        set(CPACK_RPM_PACKAGE_GROUP Office)
        set(CPACK_RPM_PACKAGE_REQUIRES "qt5-qtbase >= 5.3.2, qt5-qtbase-gui >= 5.3.2")
        set(CPACK_RPM_PACKAGE_SUMMARY "${CN_CURRENCY_DISPLAY_NAME} wallet")
        set(CPACK_RPM_PACKAGE_DESCRIPTION "${CN_CURRENCY_DISPLAY_NAME} wallet
 ${CN_CURRENCY_DISPLAY_NAME} is a Brazilian decentralized, privacy oriented peer-to-peer
 cryptocurrency. It is open-source, nobody owns or controls ${CN_CURRENCY_DISPLAY_NAME}
 and everyone can take part of it.")
        set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${VERSION_RELEASE}${CPACK_SYSTEM_NAME}")
    endif ()
elseif (WIN32)
    install(TARGETS ${PROJECT_NAME}-wallet RUNTIME DESTINATION .)
    set(CPACK_GENERATOR ZIP)
endif (APPLE)

include(CPack)
