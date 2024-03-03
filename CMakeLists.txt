cmake_minimum_required(VERSION 3.7)

project(opengl)
set(TARGET_NAME exec)
set(CMAKE_CXX_STANDARD 17)

include_directories(./)

set(SOURCE_ALL
    ./source/main.cpp

    ./source/Events.cpp
    ./source/Log.cpp
    ./source/Shader.cpp
    ./source/Window.cpp
)


if (UNIX)
  add_definitions(-DLINUX)

  # debug
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG  "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g" )

  # release
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -fomit-frame-pointer -fno-strict-aliasing -fvisibility=hidden -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -Wall")

  # link options
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections -static-libstdc++ -static-libgcc -lXi")

  add_executable(${TARGET_NAME} ${SOURCE_ALL})

  set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../out/")
  set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_NAME "exec")

  
  find_package(Threads REQUIRED)
  find_package(OpenAL REQUIRED)
  find_package(X11 REQUIRED)
  #find_package(X11_Xi REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(glfw3 3.3 REQUIRED)
  find_package(Freetype REQUIRED)
  
  # look for SDL2
  if(APPLE)
    include_directories("/opt/local/include")
    include_directories("/opt/homebrew/include")
    include_directories("/opt/homebrew/Cellar/freetype/2.13.0_1/include/freetype2/")
    #set(SDL2_INCLUDE_DIR "/opt/homebrew/include")
    #set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2-2.0.0.dylib")

    #SET(SOIL_LIBRARY "/opt/local/lib/libSOIL.dylib")
  endif()

  include_directories("/usr/include/freetype2")

  INCLUDE(FindPkgConfig)
  PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
  INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})


  SET(SOIL_LIBRARY SOIL)
  SET(SDL2_LIBRARY SDL2)

  set(CMAKE_THREAD_LIBS_INIT "-lpthread")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
  set(CMAKE_HAVE_THREADS_LIBRARY 1)
  set(CMAKE_USE_WIN32_THREADS_INIT 0)
  set(CMAKE_USE_PTHREADS_INIT 1)
  set(THREADS_PREFER_PTHREAD_FLAG ON)

  target_link_libraries(${TARGET_NAME} ${CMAKE_DL_LIBS} ${SDL2_LIBRARY} ${SOIL_LIBRARY} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} GLEW::GLEW glfw)
endif()
