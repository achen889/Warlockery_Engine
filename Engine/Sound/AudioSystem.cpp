//==============================================================================================================
//AudioSystem.cpp
//by Albert Chen Jun-23-2016.
//==============================================================================================================

#include "AudioSystem.hpp"
#include "Engine\Core\Utilities.hpp"

//===========================================================================================================

AudioSystem* theSoundSystem = NULL;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

AudioSystem::AudioSystem(){
	if (!theSoundSystem) {
		 theSoundSystem = this;
	}
	//register events
	if (IsEventSystemCreated()) {
		RegisterEventMethodCallback("PlaySound", &AudioSystem::EventPlaySound, *theSoundSystem);
		RegisterEventMethodCallback("StopSound", &AudioSystem::EventStopSound, *theSoundSystem);
	}	
}

AudioSystem::~AudioSystem(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

void AudioSystem::EventPlaySound(NamedProperties& params) {
	std::string soundFileNameStr;
	float volumeParam = 1.0f;
	int loopCountParam = 0;
	params.Get("soundFileName", soundFileNameStr);
	params.Get("soundLoopCount", loopCountParam);
	params.Get("soundVolume", volumeParam);

	PlayASound(soundFileNameStr, loopCountParam, volumeParam);
}

//-----------------------------------------------------------------------------------------------------------

void AudioSystem::EventStopSound(NamedProperties& params) {
	std::string soundToStopName = "";
	params.Get("soundFileName", soundToStopName);

	StopSoundByName(soundToStopName);
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

