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
    void list_all_songs();

private:
    static const int TABLE_SIZE = 128;
    std::list<Song> table[TABLE_SIZE];

    size_t hash_function(const std::string& title);
};

#endif // MUSIC_LIBRARY_H
