cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(simpleSDL C CXX)

#
# Set some helper variables.
#
string(TOLOWER "${CMAKE_SYSTEM_NAME}" targetSystem)
set(projectDir  "${CMAKE_CURRENT_LIST_DIR}")
set(sourceDir   "${projectDir}/source")
set(targetName  "SimpleExample")
set(binDir      "${projectDir}/bin")

# Define executable output dir.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${binDir}/${targetSystem}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${binDir}/${targetSystem}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${binDir}/${targetSystem}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${binDir}/${targetSystem}_debug")

#
# Dependencies:
#   Using SDL as a subproject for simplicity.
#   NOTE: This means that CMAKE_C_FLAGS set anywhere here are also passed to SDL.
#
if(MINGW)
  set(VIDEO_OPENGLES OFF CACHE STRING "")
endif()


## NEW 


set(DISTRIBUTION_DIR ${CMAKE_SOURCE_DIR}/platforms/android/build/SDL2/intermediates/ndkBuild)
string(TOLOWER ${CMAKE_BUILD_TYPE} ANDROID_BUILD_DIR)


message (STATUS "---- SDL2 dir for debug:  ${SDL2_LOCATION}")

set(SDL_LOCATION ${CMAKE_SOURCE_DIR}/contrib/)

##SDL2
set (SDL2_LOCATION ${DISTRIBUTION_DIR}/${ANDROID_BUILD_DIR}/obj/local/${ANDROID_ABI}/libSDL2.so)
add_library(SDL2 SHARED IMPORTED )
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${SDL2_LOCATION})

## add includes 
# include_directories(${CMAKE_SOURCE_DIR}/contrib/SDL/include)

## SDL2_image
set (SDL2_IMAGE_LOCATION ${DISTRIBUTION_DIR}/${ANDROID_BUILD_DIR}/obj/local/${ANDROID_ABI}/libSDL2_image.so)
add_library(SDL2_IMAGE SHARED IMPORTED )
set_target_properties(SDL2_IMAGE PROPERTIES IMPORTED_LOCATION ${SDL2_IMAGE_LOCATION})

## SDL2_ttf
set (SDL2_TTF_LOCATION ${DISTRIBUTION_DIR}/${ANDROID_BUILD_DIR}/obj/local/${ANDROID_ABI}/libSDL2_ttf.so)
add_library(SDL2_TTF SHARED IMPORTED )
set_target_properties(SDL2_TTF PROPERTIES IMPORTED_LOCATION ${SDL2_TTF_LOCATION})

## SDL2_mixer
set (SDL2_MIXER_LOCATION ${DISTRIBUTION_DIR}/${ANDROID_BUILD_DIR}/obj/local/${ANDROID_ABI}/libSDL2_mixer.so)
add_library(SDL2_MIXER SHARED IMPORTED )
set_target_properties(SDL2_MIXER PROPERTIES IMPORTED_LOCATION ${SDL2_MIXER_LOCATION})


# add_library( main SHARED ${SDL_LOCATION}/SDL2/src/main/android/SDL_android_main.c ${SOURCE_FILES} )

## END NEW



# Joysticks were causing trouble on android.
add_definitions(-DSDL_JOYSTICK=0)

#
# Sources (relative to the project root dir).
#
set(projectSources
  source/main.c
  #  ${CMAKE_SOURCE_DIR}/contrib/SDL/src/main/android/SDL_android_main.c 

  # Use this instead for an opengl example
  #source/main_opengl.c
)

# Include dirs.
set(projectIncludeDirs ${projectIncludeDirs}
  "${projectDir}/contrib/SDL2/include"
  "${projectDir}/contrib/SDL2_image"
  "${projectDir}/contrib/SDL2_ttf"
  "${projectDir}/contrib/SDL2_mixer"
)


message("     Sources: ${projectSources}\n")
message("Include dirs: ${projectIncludeDirs}\n")


#
# Platform dependent stuff.
#

if(NOT ANDROID)
  find_package(OpenGL REQUIRED)
endif()

if(MINGW)
  # -Link standard libs statically to reduce dll clutter.
  # -lmingw32 is needed to make WinMain not disappear when linking.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -static-libgcc -lmingw32")

  # Only show the console in debug builds and also strip unused dependencies on MinSizeRel.
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-mconsole")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-mwindows")
  set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "-mwindows -s")
endif()

if(EMSCRIPTEN)
  # This makes emscripten build a html page in addition to the js code.
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  # Embedding and mapping files at from_path@to_path
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ../../assets@/")

  # Emscripten requires static linking.
  set(SDL2_LIBRARY SDL2-static )
endif()


#
# Build the binary.
# -----------------------------------------------------------------------
#
if(ANDROID)
  # On android the final binary is a shared library not an executable.
#   add_library(${targetName} SHARED ${projectSources})
# add_library( SDL2 SHARED IMPORTED  GLOBAL)
  add_library(${targetName} SHARED ${projectSources})
else()
  add_executable(${targetName} ${projectSources})
endif()

target_link_libraries(${targetName}
  SDL2
  SDL2_IMAGE
  SDL2_TTF
  SDL2_MIXER
  ${OPENGL_LIBRARY}
)
include_directories(${projectIncludeDirs})

# target_include_directories(${targetName}
#   PUBLIC ${projectIncludeDirs}
# )


