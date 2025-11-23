#include "MusicLibrary.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <fstream>
#ifdef _WIN32
    #include <windows.h>
    #include <fileapi.h>
#else
    #include <dirent.h>
    #include <sys/stat.h>
#endif

MusicLibrary::MusicLibrary() {}

size_t MusicLibrary::hash_function(const std::string& title) {
    size_t hash = 0;
    for (char c : title) {
        hash += c;
    }
    return hash % TABLE_SIZE;
}

void MusicLibrary::add_song(const Song& song) {
    size_t index = hash_function(song.title);
    table[index].push_back(song);
    std::cout << "Added '" << song.title << "' to the music library." << std::endl;
}

Song* MusicLibrary::find_song(const std::string& title) {
    size_t index = hash_function(title);
    for (Song& song : table[index]) {
        if (song.title == title) {
            return &song;
        }
    }
    return nullptr;
}

int MusicLibrary::get_song_count() const {
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        count += table[i].size();
    }
    return count;
}

Song* MusicLibrary::get_song_by_index(int index) {
    // Convert 1-based index to 0-based
    int zero_based_index = index - 1;
    if (zero_based_index < 0) {
        return nullptr;
    }
    
    int current_index = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (Song& song : table[i]) {
            if (current_index == zero_based_index) {
                return &song;
            }
            current_index++;
        }
    }
    return nullptr;
}

void MusicLibrary::list_all_songs() {
    std::cout << "--- Music Library ---" << std::endl;
    int song_number = 1;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (const Song& song : table[i]) {
            std::cout << song_number << ". " << song.title << " by " << song.artist << std::endl;
            song_number++;
        }
    }
    std::cout << "---------------------" << std::endl;
}

// Helper function to check if a file has an audio extension
static bool is_audio_file(const std::string& filename) {
    std::string lower_filename = filename;
    std::transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    // Check for audio extensions (C++17 compatible - no ends_with)
    const std::vector<std::string> audio_extensions = {".mp3", ".ogg", ".wav", ".flac", ".m4a", ".aac"};
    for (const auto& ext : audio_extensions) {
        if (lower_filename.length() >= ext.length() && 
            lower_filename.substr(lower_filename.length() - ext.length()) == ext) {
            return true;
        }
    }
    return false;
}

// Helper function to extract song title from filename (remove extension and clean up)
static std::string extract_title_from_filename(const std::string& filename) {
    // Find last dot (extension)
    size_t last_dot = filename.find_last_of('.');
    std::string stem = (last_dot != std::string::npos) ? filename.substr(0, last_dot) : filename;
    
    // Replace underscores and hyphens with spaces
    std::replace(stem.begin(), stem.end(), '_', ' ');
    std::replace(stem.begin(), stem.end(), '-', ' ');
    
    return stem;
}

int MusicLibrary::load_songs_from_directory(const std::string& directory_path) {
    int loaded_count = 0;
    
#ifdef _WIN32
    // Use Windows API to avoid filesystem DLL issues
    std::string search_path = directory_path;
    
    // Try multiple path locations
    std::vector<std::string> paths_to_try = {
        directory_path,
        ".\\" + directory_path,
        "..\\" + directory_path,
        "..\\..\\" + directory_path
    };
    
    std::string resolved_path;
    bool found = false;
    
    for (const auto& path : paths_to_try) {
        DWORD attrs = GetFileAttributesA(path.c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
            resolved_path = path;
            found = true;
            break;
        }
    }
    
    if (!found) {
        std::cerr << "Error: Could not find directory: " << directory_path << std::endl;
        return 0;
    }
    
    // Ensure path ends with backslash for search pattern
    std::string search_pattern = resolved_path;
    if (search_pattern.back() != '\\' && search_pattern.back() != '/') {
        search_pattern += "\\";
    }
    search_pattern += "*.*";
    
    std::cout << "Scanning directory: " << resolved_path << std::endl;
    
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(search_pattern.c_str(), &find_data);
    
    if (find_handle == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Could not open directory: " << resolved_path << std::endl;
        return 0;
    }
    
    do {
        // Skip . and ..
        if (find_data.cFileName[0] == '.') {
            continue;
        }
        
        // Check if it's a file (not a directory)
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string filename(find_data.cFileName);
            
            // Check if it's an audio file
            if (is_audio_file(filename)) {
                // Extract title from filename
                std::string title = extract_title_from_filename(filename);
                
                // Create a Song object
                Song new_song;
                new_song.title = title;
                new_song.artist = "Unknown Artist";
                new_song.album = "Unknown Album";
                new_song.duration_seconds = 0;
                
                // Use relative path
                std::string relative_path = directory_path + "/" + filename;
                std::replace(relative_path.begin(), relative_path.end(), '\\', '/');
                new_song.file_path = relative_path;
                
                // Add to library
                add_song(new_song);
                loaded_count++;
            }
        }
    } while (FindNextFileA(find_handle, &find_data));
    
    FindClose(find_handle);
    
    if (loaded_count > 0) {
        std::cout << "Successfully loaded " << loaded_count << " song(s) from " << resolved_path << std::endl;
    } else {
        std::cout << "No audio files found in " << resolved_path << std::endl;
        std::cout << "Supported formats: .mp3, .ogg, .wav, .flac, .m4a, .aac" << std::endl;
    }
#else
    // POSIX implementation would go here
    std::cerr << "Non-Windows platform not yet supported" << std::endl;
    return 0;
#endif
    
    return loaded_count;
}
