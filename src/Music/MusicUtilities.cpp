#include "MusicUtilities.h"

std::map<std::string, FMOD::Sound*> MusicUtilities::soundCache;
std::mutex MusicUtilities::soundCacheLock;
