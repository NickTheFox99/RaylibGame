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

<<<<<<< HEAD
file(GLOB_RECURSE ASSET_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/*
)

set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(GENERATED_HEADERS "")

foreach(ASSET_FILE ${ASSET_FILES})
  # Get relative path from assets/
  file(RELATIVE_PATH REL_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${ASSET_FILE}
    )

  # Get just the filename
  get_filename_component(FILE_NAME ${ASSET_FILE} NAME)

  # Get the subdirectory path
  get_filename_component(SUB_DIR ${REL_PATH} DIRECTORY)

  # Create output directory structure
  set(OUTPUT_DIR ${DATA_DIR}/${SUB_DIR})
  file(MAKE_DIRECTORY ${OUTPUT_DIR})

  # Output file with full extension + .h
  set(OUTPUT_FILE ${OUTPUT_DIR}/${FILE_NAME}.h)

  # Get the directory of the asset file for WORKING_DIRECTORY
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

file(GLOB_RECURSE PROJECTSOURCES "src/*.c" "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${PROJECTSOURCES})

# CHANGED: Use ${GENERATED_HEADERS} instead of GLOB
# The files don't exist at configure time, so GLOB won't find them!
target_sources(${PROJECT_NAME} PRIVATE ${GENERATED_HEADERS})

# Add data directory to include path
target_include_directories(${PROJECT_NAME} PRIVATE ${DATA_DIR})
=======
<<<<<<< Updated upstream
file(GLOB_RECURSE PROJECTSOURCES "src/*.c" "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${PROJECTSOURCES})

file(GLOB_RECURSE PROJECTDATA data/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECTDATA})
=======
file(GLOB_RECURSE ASSET_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/*
)

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

file(GLOB_RECURSE PROJECTSOURCES "src/*.c" "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${PROJECTSOURCES})

target_sources(${PROJECT_NAME} PRIVATE ${GENERATED_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${DATA_DIR})
>>>>>>> Stashed changes
>>>>>>> 4149042 (resolve conflicts)

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib raylib_cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${raylib_INCLUDE_DIRS} ${raylib_cpp_INCLUDE_DIRS})

# Web Configurations
if(${PLATFORM} STREQUAL "Web")
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s SINGLE_FILE=1 --shell-file ${CMAKE_SOURCE_DIR}/src/minshell.html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if(APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
