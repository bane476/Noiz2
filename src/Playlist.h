#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Song.h"
#include <iostream>

struct Node {
    Song song;
    Node* next;
    Node* prev;

    Node(const Song& s) : song(s), next(nullptr), prev(nullptr) {}
};

class Playlist {
public:
    Playlist(const std::string& name);
    ~Playlist();

    void add_song(const Song& song);
    void play();
    void next_song();
    void prev_song();
    void show_playlist();

private:
    std::string name;
    Node* head;
    Node* tail;
    Node* current_song;
};

#endif // PLAYLIST_H
