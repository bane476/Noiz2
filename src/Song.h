#ifndef SONG_H
#define SONG_H

#include <string>

struct Song {
    std::string title;
    std::string artist;
    std::string album;
    int duration_seconds;
    std::string file_path;
};

#endif // SONG_H
