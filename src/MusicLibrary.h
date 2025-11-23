#ifndef MUSIC_LIBRARY_H
#define MUSIC_LIBRARY_H

#include "Song.h"
#include <string>
#include <vector>
#include <list>
#include <functional>

class MusicLibrary {
public:
    MusicLibrary();
    void add_song(const Song& song);
    Song* find_song(const std::string& title);
    Song* get_song_by_index(int index); // Get song by number (1-based)
    int get_song_count() const; // Get total number of songs
    void list_all_songs(); // Lists songs with numbers
    int load_songs_from_directory(const std::string& directory_path); // Returns number of songs loaded

private:
    static const int TABLE_SIZE = 128;
    std::list<Song> table[TABLE_SIZE];

    size_t hash_function(const std::string& title);
};

#endif // MUSIC_LIBRARY_H
