project(svmplayer)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(svmplayer src/svmwindow.cpp src/sopcast.cpp src/svmplayer.cpp main.cpp)
target_link_libraries (svmplayer vlc X11 pthread)
install(TARGETS svmplayer RUNTIME DESTINATION bin)

include_directories(include)
add_subdirectory(include)
add_subdirectory(src)