cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(simpleSDL C CXX)

#
# Set some helper variables.
#
string(TOLOWER "${CMAKE_SYSTEM_NAME}" targetSystem)
set(projectDir  "${CMAKE_CURRENT_LIST_DIR}")
set(sourceDir   "${projectDir}/source")
set(targetName  "main")
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

## bzip2
set (ZLIB_LOCATION ${DISTRIBUTION_DIR}/${ANDROID_BUILD_DIR}/obj/local/${ANDROID_ABI}/zlib.so)
add_library(ZLIB SHARED IMPORTED )
set_target_properties(ZLIB PROPERTIES IMPORTED_LOCATION ${ZLIB_LOCATION})

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

set(SOURCES
        source/engine/basics/camera.cxx
        source/engine/basics/isoMath.cxx
        source/engine/basics/mapEdit.cxx
        source/engine/basics/settings.cxx
        source/engine/basics/timer.cxx
        source/engine/basics/utils.cxx
        source/engine/GameObjects/mapNode.cxx
        source/engine/ui/basics/uiElement.cxx
        source/engine/ui/basics/buttonGroup.cxx
        source/engine/ui/basics/Layout.cxx
        source/engine/ui/widgets/bar.cxx
        source/engine/ui/widgets/button.cxx
        source/engine/ui/widgets/combobox.cxx
        source/engine/ui/widgets/checkbox.cxx
        source/engine/ui/widgets/frame.cxx
        source/engine/ui/widgets/Image.cxx
        source/engine/ui/widgets/text.cxx
        source/engine/ui/widgets/textField.cxx
        source/engine/ui/widgets/tooltip.cxx
        source/engine/ui/menuGroups/MenuGroupBuild.cxx
        source/engine/engine.cxx
        source/engine/eventManager.cxx
        source/engine/map.cxx
        source/engine/sprite.cxx
        source/engine/resourcesManager.cxx
        source/engine/tileManager.cxx
        source/engine/uiManager.cxx
        source/engine/windowManager.cxx
        source/game.cxx
        source/main.cxx
        source/engine/audio/soundEffect.cxx
        source/engine/audio/soundEffect.cxx
        source/engine/audio/music.cxx
        source/engine/audioMixer.cxx
        ${CMAKE_SOURCE_DIR}/contrib/SDL2/src/main/android/SDL_android_main.c 
        )


# Include dirs.
set(projectIncludeDirs ${projectIncludeDirs}
  "${projectDir}/contrib/SDL2/include"
  "${projectDir}/contrib/SDL2_image"
  "${projectDir}/contrib/SDL2_ttf"
  "${projectDir}/contrib/SDL2_mixer"
  "${projectDir}/externLibs/libnoise/src/noise"
  "${projectDir}/externLibs/zlib"
)


message("     Sources: ${SOURCES}\n")
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
  add_library(${targetName} SHARED ${SOURCES})
else()
  add_executable(${targetName} ${SOURCES})
endif()

target_link_libraries(${targetName}
  SDL2
  SDL2_IMAGE
  SDL2_TTF
  SDL2_MIXER
#   ZLIB
  ${OPENGL_LIBRARY}
)
include_directories(${projectIncludeDirs})

# target_include_directories(${targetName}
#   PUBLIC ${projectIncludeDirs}
# )


