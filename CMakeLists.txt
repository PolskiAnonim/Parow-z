cmake_minimum_required (VERSION 3.8)

project ("Grafika")

 #SET ( CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO ) # jebać manifesty, komunistów, demony i windowsa

file(GLOB SOURCES CONFIGURE_DEPENDS 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"     
)

add_executable (Grafika "${SOURCES}")

find_package(GLEW REQUIRED)
target_link_libraries(Grafika GLEW::glew)

find_package(glfw3 REQUIRED)
target_link_libraries(Grafika glfw)

find_package(assimp REQUIRED)
target_link_libraries(Grafika assimp::assimp)

find_package(glm REQUIRED)
target_link_libraries(Grafika glm::glm)

file(REMOVE "${EXECUTABLE_OUTPUT_PATH}/models/")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/models" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/") # Copy models

file(REMOVE "${EXECUTABLE_OUTPUT_PATH}/shaders/")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/") # Copy models

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include") # stb_image

set_property(TARGET Grafika PROPERTY CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()