cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(game C CXX)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if(NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if(NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()

set(RAYLIB_CPP_VERSION "v5.5.0")
find_package(raylib_cpp QUIET) # QUIET or REQUIRED
if(NOT raylib_cpp_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib_cpp
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/RobLoach/raylib-cpp/archive/refs/tags/${RAYLIB_CPP_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib_cpp)
  if(NOT raylib_cpp_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib_cpp)
  endif()
endif()

# Our Project
add_executable(${PROJECT_NAME})

# Generate header files from assets using xxd
file(GLOB_RECURSE ASSET_FILES ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)

set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(GENERATED_HEADERS "")

foreach(ASSET_FILE ${ASSET_FILES})
  file(RELATIVE_PATH REL_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${ASSET_FILE}
  )

  get_filename_component(FILE_NAME ${ASSET_FILE} NAME)
  get_filename_component(SUB_DIR ${REL_PATH} DIRECTORY)

  set(OUTPUT_DIR ${DATA_DIR}/${SUB_DIR})
  file(MAKE_DIRECTORY ${OUTPUT_DIR})

  set(OUTPUT_FILE ${OUTPUT_DIR}/${FILE_NAME}.h)

  get_filename_component(ASSET_DIR ${ASSET_FILE} DIRECTORY)

  add_custom_command(
    OUTPUT ${OUTPUT_FILE}
    COMMAND xxd -i ${FILE_NAME} ${OUTPUT_FILE}
    DEPENDS ${ASSET_FILE}
    WORKING_DIRECTORY ${ASSET_DIR}
    COMMENT "Embedding ${REL_PATH}"
    VERBATIM
  )

  list(APPEND GENERATED_HEADERS ${OUTPUT_FILE})
endforeach()

# Add source files
file(GLOB_RECURSE PROJECTSOURCES "src/*.c" "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${PROJECTSOURCES})

# Add generated headers
target_sources(${PROJECT_NAME} PRIVATE ${GENERATED_HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${DATA_DIR}
  ${raylib_INCLUDE_DIRS}
  ${raylib_cpp_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} raylib raylib_cpp)

# Web Configurations
if(${PLATFORM} STREQUAL "Web")
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s SINGLE_FILE=1 --shell-file ${CMAKE_SOURCE_DIR}/src/minshell.html")
endif()

# macOS frameworks
if(APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
