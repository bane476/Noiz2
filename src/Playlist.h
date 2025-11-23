#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Song.h"
#include <string>
#include <vector>
#include <list>
#include <memory>
// Forward declaration to avoid including SFML in header (prevents static init issues)
namespace sf { class Music; }
#include "MusicLibrary.h" // Include MusicLibrary

class Playlist {
public:
    Playlist(const std::string& name);
    ~Playlist(); // Need destructor for unique_ptr
    Playlist(const Playlist&) = delete; // Disable copy
    Playlist& operator=(const Playlist&) = delete; // Disable assignment
    void add_song(const Song& song);
    void show_playlist() const;
    void play(); // Modified to handle playback
    void pause();
    void stop();
    void next_song();
    void prev_song();
    bool save_to_file(const std::string& filename) const;
    bool load_from_file(const std::string& filename, MusicLibrary& library);

private:
    std::string name;
    std::vector<Song> songs;
    int current_song_index; // To keep track of the current song
    std::unique_ptr<sf::Music> current_music; // SFML music object for playback (pointer to delay init)
};

#endif // PLAYLIST_H
