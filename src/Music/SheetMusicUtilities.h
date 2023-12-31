#pragma once

#include "MusicUtilities.h"

class SheetMusicUtilities
{
private:

	SheetMusicUtilities();

	static std::unique_ptr<SheetMusicUtilities> instance;

	static std::mutex mutex;
	static std::map<std::string, sf::Texture> textures;

public:
	SheetMusicUtilities(const SheetMusicUtilities&) = delete;

	static SheetMusicUtilities* GetInstance() {
		std::lock_guard<std::mutex> lock(mutex);
		if (instance == nullptr) {
			instance = std::unique_ptr<SheetMusicUtilities>(new SheetMusicUtilities());
		}
		return instance.get();
	}


	static sf::Texture& getSignatureTexture(int value)
	{
		
		switch (value)
		{
		case 2:
			if (textures.find("timeSignature2") == textures.end())
			{
				if (!textures["timeSignature2"].loadFromFile("resources/images/sheet_music/time_signatures/2.png")) {
					// Handle error
					std::cout << "Note Texture unable to load\n";
				}
			}
			return (textures["timeSignature2"]);
		case 3:
			if (textures.find("timeSignature3") == textures.end())
			{
				if (!textures["timeSignature3"].loadFromFile("resources/images/sheet_music/time_signatures/3.png")) {
					// Handle error
					std::cout << "Note Texture unable to load\n";
				}
			}
			return (textures["timeSignature3"]);
		case 4:
			if (textures.find("timeSignature4") == textures.end())
			{
				if (!textures["timeSignature4"].loadFromFile("resources/images/sheet_music/time_signatures/4.png")) {
					// Handle error
					std::cout << "Note Texture unable to load\n";
				}
			}
			return (textures["timeSignature4"]);
		case 6:
			if (textures.find("timeSignature6") == textures.end())
			{
				if (!textures["timeSignature6"].loadFromFile("resources/images/sheet_music/time_signatures/6.png")) {
					// Handle error
					std::cout << "Note Texture unable to load\n";
				}
			}
			return (textures["timeSignature6"]);
		case 8:
			if (textures.find("timeSignature8") == textures.end())
			{
				if (!textures["timeSignature8"].loadFromFile("resources/images/sheet_music/time_signatures/8.png")) {
					// Handle error
					std::cout << "Note Texture unable to load\n";
				}
			}
			return (textures["timeSignature8"]);
		}

		if (textures.find("timeSignature4") == textures.end())
		{
			if (!textures["timeSignature4"].loadFromFile("resources/images/sheet_music/time_signatures/4.png")) {
				// Handle error
				std::cout << "Note Texture unable to load\n";
			}
		}
		return (textures["timeSignature4"]);
	}

	std::map<std::string, sf::Texture> getTextures() { return textures; }


};

