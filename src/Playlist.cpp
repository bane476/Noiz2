#include "Playlist.h"
#include "MusicLibrary.h" // Needed for loading from file
#include <SFML/Audio.hpp> // Include SFML here in implementation file
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <exception>
#ifdef _WIN32
    #include <windows.h>
    #include <fileapi.h>
#endif

// Helper function to resolve asset file paths using Windows API (avoids filesystem DLL issues)
// Tries multiple possible locations for the file
static std::string resolve_asset_path(const std::string& file_path) {
    // If path is empty, return as-is
    if (file_path.empty()) {
        return file_path;
    }
    
#ifdef _WIN32
    // Try the path as-is first
    DWORD attrs = GetFileAttributesA(file_path.c_str());
    if (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        return file_path;
    }
    
    // Try relative to current working directory
    char cwd[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, cwd)) {
        std::string cwd_path = std::string(cwd) + "\\" + file_path;
        std::replace(cwd_path.begin(), cwd_path.end(), '/', '\\');
        attrs = GetFileAttributesA(cwd_path.c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
            return cwd_path;
        }
    }
    
    // Try parent directory
    if (GetCurrentDirectoryA(MAX_PATH, cwd)) {
        std::string parent_path = std::string(cwd) + "\\..\\" + file_path;
        std::replace(parent_path.begin(), parent_path.end(), '/', '\\');
        attrs = GetFileAttributesA(parent_path.c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
            return parent_path;
        }
    }
    
    // Try grandparent directory
    if (GetCurrentDirectoryA(MAX_PATH, cwd)) {
        std::string grandparent_path = std::string(cwd) + "\\..\\..\\" + file_path;
        std::replace(grandparent_path.begin(), grandparent_path.end(), '/', '\\');
        attrs = GetFileAttributesA(grandparent_path.c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
            return grandparent_path;
        }
    }
#else
    // POSIX implementation would go here
    // For now, just return the original path
#endif
    
    // If none found, return original path (will fail gracefully with better error message)
    return file_path;
}

Playlist::Playlist(const std::string& name) : name(name), current_song_index(-1), current_music(std::make_unique<sf::Music>()) {
    std::cout << "Created playlist: " << name << std::endl;
}

Playlist::~Playlist() = default; // Default destructor is fine with unique_ptr

void Playlist::add_song(const Song& song) {
    // Check if song is already in the playlist (by title and artist to be safe)
    for (const Song& existing_song : songs) {
        if (existing_song.title == song.title && existing_song.artist == song.artist) {
            std::cout << "Song '" << song.title << "' by " << song.artist << " is already in the playlist." << std::endl;
            return;
        }
    }
    
    // Song is not a duplicate, add it
    songs.push_back(song);
    if (current_song_index == -1) { // If playlist was empty, set this as the first song
        current_song_index = 0;
    }
    std::cout << "Added '" << song.title << "' to playlist '" << name << "'" << std::endl;
}

void Playlist::play() {
    if (songs.empty()) {
        std::cout << "Playlist '" << name << "' is empty. No song to play." << std::endl;
        return;
    }

    if (current_song_index < 0 || current_song_index >= songs.size()) {
        std::cout << "Invalid current song index. Resetting to first song." << std::endl;
        current_song_index = 0;
    }

    const Song& song_to_play = songs[current_song_index];
    if (!song_to_play.file_path.empty()) {
        // Stop any currently playing music
        current_music->stop();

        // Resolve the asset path (try multiple locations)
        std::string resolved_path = resolve_asset_path(song_to_play.file_path);
        
        // Load and play the new song
        if (current_music->openFromFile(resolved_path)) {
            current_music->play();
            std::cout << "Playing: " << song_to_play.title << " by " << song_to_play.artist << " from " << resolved_path << std::endl;
        } else {
            std::cerr << "Error: Could not open audio file: " << resolved_path << std::endl;
            std::cerr << "  (Tried original path: " << song_to_play.file_path << ")" << std::endl;
        }
    } else {
        std::cout << "Song '" << song_to_play.title << "' has no file path specified. Cannot play." << std::endl;
    }
}

void Playlist::pause() {
    // Check if we have a valid current song index
    if (current_song_index < 0 || current_song_index >= static_cast<int>(songs.size())) {
        std::cout << "No music playing to pause/resume." << std::endl;
        return;
    }

    if (current_music->getStatus() == sf::Music::Playing) {
        current_music->pause();
        std::cout << "Paused: " << songs[current_song_index].title << std::endl;
    } else if (current_music->getStatus() == sf::Music::Paused) {
        current_music->play();
        std::cout << "Resumed: " << songs[current_song_index].title << std::endl;
    } else {
        std::cout << "No music playing to pause/resume." << std::endl;
    }
}

void Playlist::stop() {
    current_music->stop();
    std::cout << "Stopped playback." << std::endl;
}

void Playlist::next_song() {
    if (songs.empty()) {
        std::cout << "Playlist is empty. Cannot go to next song." << std::endl;
        return;
    }
    current_music->stop(); // Stop current song before changing
    // If no current song is set, start from the beginning
    if (current_song_index < 0 || current_song_index >= static_cast<int>(songs.size())) {
        current_song_index = 0;
    } else {
        current_song_index = (current_song_index + 1) % songs.size();
    }
    play();
}

void Playlist::prev_song() {
    if (songs.empty()) {
        std::cout << "Playlist is empty. Cannot go to previous song." << std::endl;
        return;
    }
    current_music->stop(); // Stop current song before changing
    // If no current song is set, start from the last song
    if (current_song_index < 0 || current_song_index >= static_cast<int>(songs.size())) {
        current_song_index = songs.size() - 1;
    } else {
        current_song_index = (current_song_index - 1 + songs.size()) % songs.size();
    }
    play();
}

void Playlist::show_playlist() const {
    std::cout << "--- Playlist: " << name << " ---" << std::endl;
    if (songs.empty()) {
        std::cout << "(empty)" << std::endl;
        return;
    }
    for (size_t i = 0; i < songs.size(); ++i) {
        std::cout << "- " << songs[i].title << " by " << songs[i].artist;
        if (static_cast<int>(i) == current_song_index) {
            std::cout << " <-- (current)";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------" << std::endl;
}

bool Playlist::save_to_file(const std::string& filename) const {
    if (songs.empty()) {
        std::cout << "Playlist is empty. Nothing to save." << std::endl;
        return false;
    }
    
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        std::cerr << "Make sure you have write permissions in the current directory." << std::endl;
        return false;
    }

    int saved_count = 0;
    for (const Song& song : songs) {
        // Escape any pipe characters in the data (though unlikely)
        outfile << song.title << "|" << song.artist << "|" << song.album << "|" 
                << song.duration_seconds << "|" << song.file_path << std::endl;
        saved_count++;
    }
    
    outfile.close();

    std::cout << "Playlist '" << name << "' saved to " << filename << " (" << saved_count << " song(s))" << std::endl;
    return true;
}

bool Playlist::load_from_file(const std::string& filename, MusicLibrary& library) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return false;
    }

    songs.clear(); // Clear the current playlist before loading
    current_song_index = -1; // Reset index

    std::string line;
    int line_number = 0;
    int songs_loaded = 0;
    
    while (std::getline(infile, line)) {
        line_number++;
        
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\n\r") == std::string::npos) {
            continue;
        }
        
        // Parse the line (title|artist|album|duration|filepath)
        size_t first_delim = line.find('|');
        size_t second_delim = line.find('|', first_delim + 1);
        size_t third_delim = line.find('|', second_delim + 1);
        size_t fourth_delim = line.find('|', third_delim + 1);

        if (first_delim == std::string::npos || second_delim == std::string::npos || 
            third_delim == std::string::npos || fourth_delim == std::string::npos) {
            std::cerr << "Warning: Skipping malformed line " << line_number << " in playlist file: " << line << std::endl;
            continue;
        }

        try {
            std::string title = line.substr(0, first_delim);
            std::string artist = line.substr(first_delim + 1, second_delim - (first_delim + 1));
            std::string album = line.substr(second_delim + 1, third_delim - (second_delim + 1));
            
            // Safely parse duration
            int duration = 0;
            try {
                std::string duration_str = line.substr(third_delim + 1, fourth_delim - (third_delim + 1));
                if (!duration_str.empty()) {
                    duration = std::stoi(duration_str);
                }
            } catch (const std::exception& e) {
                std::cerr << "Warning: Invalid duration on line " << line_number << ", using 0" << std::endl;
            }
            
            std::string file_path = line.substr(fourth_delim + 1);

            // Try to find the song in the library first (by title)
            Song* library_song = library.find_song(title);
            if (library_song) {
                // Use the song from library (has correct file path)
                // Check if already in playlist before adding
                bool is_duplicate = false;
                for (const Song& existing_song : songs) {
                    if (existing_song.title == library_song->title && existing_song.artist == library_song->artist) {
                        is_duplicate = true;
                        break;
                    }
                }
                if (!is_duplicate) {
                    add_song(*library_song);
                    songs_loaded++;
                } else {
                    std::cout << "Skipping duplicate: '" << title << "' by " << artist << std::endl;
                }
            } else {
                // Song not in library, create new one from file
                // Check if already in playlist before adding
                bool is_duplicate = false;
                for (const Song& existing_song : songs) {
                    if (existing_song.title == title && existing_song.artist == artist) {
                        is_duplicate = true;
                        break;
                    }
                }
                if (!is_duplicate) {
                    Song loaded_song = {title, artist, album, duration, file_path};
                    add_song(loaded_song);
                    songs_loaded++;
                } else {
                    std::cout << "Skipping duplicate: '" << title << "' by " << artist << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line " << line_number << ": " << e.what() << std::endl;
            continue;
        }
    }

    if (!songs.empty()) {
        current_song_index = 0; // Set first song as current after loading
    }

    std::cout << "Playlist loaded from " << filename << " (" << songs_loaded << " song(s) loaded)" << std::endl;
    return true;
}