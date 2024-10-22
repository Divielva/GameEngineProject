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


# Include sub-projects.
include_directories(deps/includes)

file(GLOB_RECURSE SOURCES "GameEngineProject/*.cpp")
file(GLOB_RECURSE HEADERS "GameEngineProject/*.h")
file(GLOB_RECURSE IMGUI_SOURCES "deps/includes/imgui/*.cpp")
file(GLOB_RECURSE IMGUI_HEADERS "deps/includes/imgui/*.h")
add_executable (GameEngineProject "GameEngineProject/glad.c" ${SOURCES} ${HEADERS} ${IMGUI_SOURCES} ${IMGUI_HEADERS})

set_target_properties(GameEngineProject PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:GameEngineProject>)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GameEngineProject PROPERTY CXX_STANDARD 20)
endif()

target_link_directories(GameEngineProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/libs)
target_link_libraries(GameEngineProject PRIVATE "glfw3.lib")
target_link_libraries(GameEngineProject PRIVATE "opengl32.lib")
target_link_options(GameEngineProject PRIVATE "/NODEFAULTLIB:library")

add_custom_command(TARGET GameEngineProject POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:GameEngineProject>/shaders)
add_custom_command(TARGET GameEngineProject POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud $<TARGET_FILE_DIR:GameEngineProject>/pointcloud)
