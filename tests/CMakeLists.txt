include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(selfiematTest_SRCS selfiematTest.cpp)

qt4_automoc(${selfiematTest_SRCS})
add_executable(selfiematTest ${selfiematTest_SRCS})
add_test(selfiematTest selfiematTest)
target_link_libraries(selfiematTest ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})
