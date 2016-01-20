//==============================================================================================================
//SoundSystem.hpp
//by Albert Chen Feb-10-2015.
//==============================================================================================================

#ifndef _included_SoundSystem__
#define _included_SoundSystem__

#include "Engine/Sound/Sound.hpp"
#include <string>

//===========================================================================================================

class SoundSystem{
public:
	 SoundSystem();
	~SoundSystem();
	
	void StartUp();
	void Update();
	void ShutDown();

	SoundID LoadStreamingSound(const std::string& streamingSoundPath,  unsigned int maxNumberOfCopies);

	Sound* PlayStreamingSound(SoundID idOfSoundToPlay, int loopCount = 0 , float volumeLevel = 1.0f );

	void ReformatFilePath(std::string& pathToFormat );

private:
	std::string m_workingDirectory;
	SoundID m_nextSoundID;
	SoundMap m_soundMap;

};

extern SoundSystem* theSoundSystem;


//===========================================================================================================
#endif

//cricket only really works with ogg vorbis, .ogg
//xAudio 2.7 for it to work on some machines