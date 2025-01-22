cmake_minimum_required(VERSION 3.28)
project(
        geocache
        VERSION 0.0.1
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
# define a function for adding git dependencies
function(include_dependency libName gitURL gitTag)
    # setup the declare
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )

    FetchContent_MakeAvailable(${libName})
endfunction()

include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.0)

add_subdirectory(
        ../firmware/components/common common
)

add_executable(geocache
        main/main.cc
        main/led_matrix.cc
)

target_compile_definitions(geocache PRIVATE SDL_MAIN_USE_CALLBACKS)

target_link_libraries(geocache PRIVATE
        GC::COMMON
        SDL3::SDL3
)
