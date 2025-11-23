#include "MusicLibrary.h"
#include <iostream>

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

void MusicLibrary::list_all_songs() {
    std::cout << "--- Music Library ---" << std::endl;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (const Song& song : table[i]) {
            std::cout << "- " << song.title << " by " << song.artist << std::endl;
        }
    }
    std::cout << "---------------------" << std::endl;
}
