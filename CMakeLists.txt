# CMakeList.txt : CMake project for mlp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("mlp")

# Add source to this project's executable.
add_executable (mlp "mlp.cpp" "mlp.h" "utils.cpp" "utils.h" "data.cpp" "data.h" "preprocessing.cpp" "preprocessing.h" "matrix.h" "matrix.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mlp PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
