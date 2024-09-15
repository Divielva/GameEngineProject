# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Enable outside build directory libs
if (POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

project (GameEngineProject)

file(GLOB_RECURSE IMGUI_SOURCES "deps/includes/imgui/*.cpp")
file(GLOB_RECURSE IMGUI_HEADERS "deps/includes/imgui/*.h")

# Include sub-projects.
include_directories(deps/includes)
add_subdirectory (GameEngineProject)

add_executable(OpenGLEngineCpp ${IMGUI_SOURCES} ${IMGUI_HEADERS})


target_link_directories(GameEngineProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/libs)
target_link_libraries(GameEngineProject PRIVATE "glfw3.lib")
target_link_libraries(GameEngineProject PRIVATE "opengl32.lib")
