cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(serverFileManager LANGUAGES CXX)
add_executable(serverFileManager utils.h utils.cpp serverFileManager.cpp filemanager.cpp filemanager.h fileManager_imp.h operaciones.h)
target_link_libraries(serverFileManager pthread)

project(clientFileManager LANGUAGES CXX)
add_executable(clientFileManager utils.h utils.cpp clientFileManager.cpp fileManager_stub.h operaciones.h)
target_link_libraries(clientFileManager pthread)