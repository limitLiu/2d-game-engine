cmake_minimum_required(VERSION 3.20.0)

if (APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")
endif ()

project(2d-game-engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (APPLE AND EXISTS "/opt/homebrew/include/lua")
    set(LUA_INCLUDE_DIR "/opt/homebrew/opt/lua/include/lua")
    set(LUA_LIBRARY "/opt/homebrew/opt/lua/lib/liblua.dylib")
else ()
    find_package(Lua REQUIRED)
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.17.0
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
)

FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG main
)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)

FetchContent_MakeAvailable(spdlog glm sol2 imgui)

if (imgui_POPULATED)
    set(IMGUI_SOURCES
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp

            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    add_library(imgui_lib STATIC ${IMGUI_SOURCES})

    target_link_libraries(imgui_lib PUBLIC SDL2::SDL2)

    target_include_directories(imgui_lib PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
    )

    if (APPLE)
        target_link_libraries(imgui_lib PUBLIC "-framework OpenGL")
    else ()
        target_link_libraries(imgui_lib PUBLIC GL)
    endif ()

    message(STATUS "ImGui setup complete with SDL2 and OpenGL3 backend.")
endif ()

add_executable(${PROJECT_NAME}
    src/main.cc
    src/ECS/Game.cc
    src/ECS/System.cc
    src/ECS/Entity.cc
    src/ECS/Registry.cc
    src/ECS/Component.cc
    src/AssetStore/AssetStore.cc
)

include_directories(${PROJECT_SOURCE_DIR}/include)

target_include_directories(
        ${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        spdlog::spdlog
        glm::glm
        sol2::sol2
        ${LUA_LIBRARIES}
        SDL2::SDL2
        SDL2_image::SDL2_image
        imgui_lib
)

if (NOT APPLE AND NOT MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE
            "-static-libstdc++"
            "-static-libgcc"
    )
endif ()

set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/assets")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    "${ASSETS_DIR}"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Symlinking assets to build directory..."
    VERBATIM
)

