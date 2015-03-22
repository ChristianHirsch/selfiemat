cmake_minimum_required(VERSION 2.6)
project(selfiemat)
find_package(Qt4 REQUIRED)

enable_testing()

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(selfiemat_SRCS selfiemat.cpp main.cpp)

qt4_automoc(${selfiemat_SRCS})
add_executable(selfiemat ${selfiemat_SRCS})
target_link_libraries(selfiemat ${QT_QTCORE_LIBRARY})

add_subdirectory(tests)

install(TARGETS selfiemat RUNTIME DESTINATION bin)
