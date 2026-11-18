cmake_minimum_required(VERSION 3.20)
project(HoMM3Recreate VERSION 0.1.0 LANGUAGES C)

if(NOT WIN32)
    message(STATUS "The first native Modding Studio target is Windows-only.")
endif()

add_executable(RecreateModdingStudio WIN32
    src/modding_studio/main.c
)

target_compile_definitions(RecreateModdingStudio PRIVATE
    UNICODE
    _UNICODE
    _WIN32_WINNT=0x0601
    WINVER=0x0601
)

target_link_libraries(RecreateModdingStudio PRIVATE
    comdlg32
    shell32
    user32
    gdi32
    comctl32
)

if(MINGW)
    target_link_options(RecreateModdingStudio PRIVATE -municode)
endif()

if(MSVC)
    target_compile_options(RecreateModdingStudio PRIVATE /W4 /permissive-)
else()
    target_compile_options(RecreateModdingStudio PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(RecreateModdingStudio PROPERTIES
    OUTPUT_NAME "HoMM3-Recreate-ModdingStudio"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS RecreateModdingStudio RUNTIME DESTINATION .)
