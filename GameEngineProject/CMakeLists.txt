# CMakeList.txt : CMake project for GameEngineProject, include source and define
# project specific logic here.
#

# Add source to this project's executable.
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_executable (GameEngineProject "glad.c" ${SOURCES} ${HEADERS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GameEngineProject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
