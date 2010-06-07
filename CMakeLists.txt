project(bindingviewer)

cmake_minimum_required(VERSION 2.6)
find_package(Qt4 4.5.0 REQUIRED)
find_package(ApiExtractor REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_BUILD_TYPE Debug)

set(bindingviewer_RCCS bindingviewer.qrc)
set(bindingviewer_MOCS
    bindingviewer.h
    bindingdatamodel.h)

qt4_add_resources(bindingviewer_RCC_SRCS ${bindingviewer_RCCS})
qt4_wrap_cpp(bindingviewer_MOC_SRCS ${bindingviewer_MOCS})

add_executable(bindingviewer
               bindingviewer.cpp
               bindingdatamodel.cpp
               bindingdataitem.cpp
               main.cpp
               ${bindingviewer_MOC_SRCS}
               ${bindingviewer_RCC_SRCS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${APIEXTRACTOR_INCLUDE_DIR}
                    ${QT_INCLUDE_DIR}
                    ${QT_QTCORE_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR}
                    ${QT_QTXML_INCLUDE_DIR}
                    ${QT_QTUITOOLS_INCLUDE_DIR})

target_link_libraries(bindingviewer
                      ${APIEXTRACTOR_LIBRARY}
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTUITOOLS_LIBRARY})

install(TARGETS bindingviewer DESTINATION bin)

# uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}"
                  -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")


