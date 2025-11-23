#include <iostream>
#include <SFML/Audio.hpp>
#include "Song.h"
#include "Playlist.h"
#include "MusicLibrary.h"

int main() {
    std::cout << "--- Music Player ---" << std::endl;

    // Create a music library
    MusicLibrary library;

    // Add songs to the library
    library.add_song({"Bohemian Rhapsody", "Queen", "A Night at the Opera", 355});
    library.add_song({"Stairway to Heaven", "Led Zeppelin", "Led Zeppelin IV", 482});
    library.add_song({"Hotel California", "Eagles", "Hotel California", 391});
    library.add_song({"Like a Rolling Stone", "Bob Dylan", "Highway 61 Revisited", 369});
    std::cout << std::endl;

    // List all songs in the library
    library.list_all_songs();
    std::cout << std::endl;

    // Create a playlist
    Playlist my_playlist("Rock Classics");

    // Find songs in the library and add them to the playlist
    Song* found_song = library.find_song("Hotel California");
    if (found_song) {
        my_playlist.add_song(*found_song);
    }

    found_song = library.find_song("Bohemian Rhapsody");
    if (found_song) {
        my_playlist.add_song(*found_song);
    }
    
    found_song = library.find_song("Non-existent song");
    if (found_song == nullptr) {
        std::cout << "Correctly did not find 'Non-existent song'" << std::endl;
    }
    std::cout << std::endl;


    // Show the playlist
    my_playlist.show_playlist();
    std::cout << std::endl;

    // Play and navigate
    my_playlist.play();
    my_playlist.next_song();
    my_playlist.prev_song();
    
    std::cout << "\n--- SFML Playback Test ---" << std::endl;
    sf::Music music;
    // NOTE: This will fail until you provide a valid audio file.
    if (!music.openFromFile("test.ogg")) {
        std::cout << "Error: Could not open music file 'test.ogg'." << std::endl;
        std::cout << "This is expected if the file doesn't exist." << std::endl;
        std::cout << "The important part is that the program compiled and linked with SFML!" << std::endl;
    } else {
        music.play();
        std::cout << "Playback started! Playing for 5 seconds..." << std::endl;
        // Simple loop to keep the program alive for a bit
        sf::sleep(sf::seconds(5));
        music.stop();
    }


    return 0;
}
