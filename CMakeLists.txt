cmake_minimum_required(VERSION 3.18)
project(MusicPlayer)

set(CMAKE_CXX_STANDARD 17) # Using C++17 for modern features
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- SFML from MSYS2 ----
set(SFML_ROOT "C:/msys64/ucrt64")
set(CMAKE_PREFIX_PATH "${SFML_ROOT}")

include_directories("${SFML_ROOT}/include")
link_directories("${SFML_ROOT}/lib")

add_executable(MusicPlayer

    src/main.cpp

    src/Song.cpp

    src/Playlist.cpp

    src/MusicLibrary.cpp

)

target_link_libraries(MusicPlayer
    sfml-audio
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
)

# Note: std::filesystem should be available in C++17 standard library
# If linking fails, uncomment the line below:
# target_link_libraries(MusicPlayer stdc++fs)

# --- Copy DLLs to output directory (for Windows) ---
# This command ensures the SFML DLLs are next to the .exe file so it can run.
add_custom_command(TARGET MusicPlayer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${SFML_ROOT}/bin"
    $<TARGET_FILE_DIR:MusicPlayer>
    COMMENT "Copying SFML DLLs to output directory"
)

# --- Copy assets to output directory ---
# This command ensures assets like test.ogg are next to the .exe file.
add_custom_command(TARGET MusicPlayer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    $<TARGET_FILE_DIR:MusicPlayer>
    COMMENT "Copying assets to output directory"
)
