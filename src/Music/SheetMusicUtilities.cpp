#include "SheetMusicUtilities.h"
#include <mutex>

// Initialize the static unique_ptr instance to nullptr at the start
std::unique_ptr<SheetMusicUtilities> SheetMusicUtilities::instance = nullptr;
std::mutex SheetMusicUtilities::mutex;
std::map<std::string, sf::Texture> SheetMusicUtilities::textures;


// Other member function implementations (if any)

// For example, the constructor (if it has any specific code)
SheetMusicUtilities::SheetMusicUtilities() {
    // Constructor implementation (if needed)
}

