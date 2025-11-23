#include "Playlist.h"

Playlist::Playlist(const std::string& name) : name(name), head(nullptr), tail(nullptr), current_song(nullptr) {
    std::cout << "Created playlist: " << name << std::endl;
}

Playlist::~Playlist() {
    Node* current = head;
    while (current != nullptr) {
        Node* to_delete = current;
        current = current->next;
        delete to_delete;
    }
}

void Playlist::add_song(const Song& song) {
    Node* new_node = new Node(song);
    if (head == nullptr) {
        head = new_node;
        tail = new_node;
        current_song = head;
    } else {
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }
    std::cout << "Added '" << song.title << "' to playlist '" << name << "'" << std::endl;
}

void Playlist::play() {
    if (current_song != nullptr) {
        std::cout << "Playing: " << current_song->song.title << " by " << current_song->song.artist << std::endl;
    } else {
        std::cout << "Playlist '" << name << "' is empty." << std::endl;
    }
}

void Playlist::next_song() {
    if (current_song != nullptr && current_song->next != nullptr) {
        current_song = current_song->next;
        play();
    } else {
        std::cout << "End of playlist." << std::endl;
    }
}

void Playlist::prev_song() {
    if (current_song != nullptr && current_song->prev != nullptr) {
        current_song = current_song->prev;
        play();
    } else {
        std::cout << "Beginning of playlist." << std::endl;
    }
}

void Playlist::show_playlist() {
    std::cout << "--- Playlist: " << name << " ---" << std::endl;
    if (head == nullptr) {
        std::cout << "(empty)" << std::endl;
        return;
    }
    Node* current = head;
    while (current != nullptr) {
        std::cout << "- " << current->song.title << " by " << current->song.artist;
        if (current == current_song) {
            std::cout << " <-- (current)";
        }
        std::cout << std::endl;
        current = current->next;
    }
    std::cout << "--------------------" << std::endl;
}
