#include <iostream>
#include <string>
#include <limits> // Required for numeric_limits
#include <exception>

#include "Song.h"
#include "Playlist.h"
#include "MusicLibrary.h"

void display_menu() {
    std::cout << "\n--- Music Player Menu ---" << std::endl;
    std::cout << "1. List all songs in library" << std::endl;
    std::cout << "2. Show current playlist" << std::endl;
    std::cout << "3. Play current song" << std::endl;
    std::cout << "4. Pause/Resume current song" << std::endl;
    std::cout << "5. Next song" << std::endl;
    std::cout << "6. Previous song" << std::endl;
    std::cout << "7. Add song to playlist (by number)" << std::endl;
    std::cout << "8. Save playlist to file" << std::endl;
    std::cout << "9. Load playlist from file" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

// REMOVED load_songs_from_assets function

int main() {
    try {
        std::cout << "--- Music Player ---" << std::endl;

        MusicLibrary library;

        // For now, manually add songs (avoiding filesystem DLL issues)
        // You can add your songs here manually, or we can fix the automatic loading later
        std::cout << "Loading songs..." << std::endl;
        
        // Add songs manually - replace these with your actual song files
        library.add_song({"INTERWORLD - METAMORPHOSIS", "INTERWORLD", "Unknown Album", 0, "assets/INTERWORLD - METAMORPHOSIS.mp3"});
        library.add_song({"Joy Crookes - Feet Don't Fail Me Now", "Joy Crookes", "Unknown Album", 0, "assets/Joy Crookes - Feet Don't Fail Me Now (Official Video).mp3"});
        library.add_song({"KALEO - Way Down We Go", "KALEO", "Unknown Album", 0, "assets/KALEO - Way Down We Go (Official Music Video).mp3"});
        library.add_song({"Måneskin - Beggin'", "Måneskin", "Unknown Album", 0, "assets/Måneskin - Beggin' (LyricsTesto).mp3"});
        library.add_song({"MGMT - Little Dark Age", "MGMT", "Unknown Album", 0, "assets/MGMT - Little Dark Age (Official Video).mp3"});
        library.add_song({"The Lost Soul Down", "Unknown Artist", "Unknown Album", 0, "assets/The Lost Soul Down X Lost Soul.mp3"});
        library.add_song({"The Script - Hall of Fame", "The Script", "Unknown Album", 0, "assets/The Script - Hall of Fame (Official Video) ft. will.i.am.mp3"});
        library.add_song({"The Weeknd - Often", "The Weeknd", "Unknown Album", 0, "assets/The Weeknd - Often (NSFW) (Official Video).mp3"});
        library.add_song({"Timbaland - The Way I Are", "Timbaland", "Unknown Album", 0, "assets/Timbaland - The Way I Are (Official Music Video) ft. Keri Hilson, D.O.E., Sebastian.mp3"});
        library.add_song({"test", "Unknown Artist", "Unknown Album", 0, "assets/test.mp3"});
        library.add_song({"test ogg", "Unknown Artist", "Unknown Album", 0, "assets/test.ogg"});
        
        std::cout << "Songs loaded successfully!" << std::endl;
        std::cout << std::endl;

        Playlist my_playlist("My Awesome Playlist");

        // Show what was loaded
        library.list_all_songs();
        std::cout << "\nAll songs have been added to the library. Use menu option 7 to add songs to your playlist." << std::endl;
        
        int choice;
        do {
            display_menu();
            std::cin >> choice;

            // Clear the input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
            case 1:
                library.list_all_songs();
                break;
            case 2:
                my_playlist.show_playlist();
                break;
            case 3:
                my_playlist.play();
                break;
            case 4:
                my_playlist.pause();
                break;
            case 5:
                my_playlist.next_song();
                break;
            case 6:
                my_playlist.prev_song();
                break;
            case 7: {
                // Show numbered list first
                library.list_all_songs();
                std::cout << "Enter the song number to add to playlist: ";
                int song_number;
                std::cin >> song_number;
                
                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                Song* found_song = library.get_song_by_index(song_number);
                if (found_song) {
                    my_playlist.add_song(*found_song);
                    std::cout << "Song #" << song_number << " added to playlist!" << std::endl;
                } else {
                    std::cout << "Invalid song number. Please enter a number between 1 and " << library.get_song_count() << "." << std::endl;
                }
                break;
            }
            case 8: {
                std::cout << "Enter filename to save playlist: ";
                std::string save_filename;
                std::getline(std::cin, save_filename);
                my_playlist.save_to_file(save_filename);
                break;
            }
            case 9: {
                std::cout << "Enter filename to load playlist from: ";
                std::string load_filename;
                std::getline(std::cin, load_filename);
                my_playlist.load_from_file(load_filename, library);
                break;
            }
            case 0:
                std::cout << "Exiting Music Player. Goodbye!" << std::endl;
                my_playlist.stop(); 
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        } while (choice != 0);

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Fatal error: Unknown exception occurred" << std::endl;
        return 1;
    }
}
