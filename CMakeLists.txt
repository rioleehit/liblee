cmake_minimum_required (VERSION 2.6)
project (test)

set(CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++")

include_directories("./include/")
IF (WIN32)  
    MESSAGE(STATUS "SeverMain is windows")  
ELSEIF (APPLE)  
    MESSAGE(STATUS "SeverMain is Apple systens.")  
ELSEIF (UNIX)  
    MESSAGE(STATUS "SeverMain is UNIX-like OS's. Including aPPLE os x  and CygWin")  
ENDIF ()  

add_executable(Test ./test/main.cpp)
#target_link_libraries(SeverMain SeverFrame)
#target_link_libraries(SeverMain WebSever)
