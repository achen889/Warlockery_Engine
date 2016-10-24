//==============================================================================================================
//AudioSystem.hpp
//by Albert Chen Jun-23-2016.
//==============================================================================================================

#pragma once

#ifndef _included_AudioSystem__
#define _included_AudioSystem__

//===========================================================================================================

#include <string>
#include <vector>
#include <map>
#include "Engine/Console/Console.hpp"
#include "Engine/Core/Memory.hpp"
#include "../Core/Event/EventSystem.hpp"

//===========================================================================================================

typedef int SoundID;
typedef void* AudioChannelHandle;
const int MISSING_SOUND_ID = 0xffffffff;

struct SoundInstance {
	//vars
	std::string m_name;
	SoundID m_id;
	//methods

	virtual bool IsValid() const = 0;

	
};

typedef std::vector<SoundInstance*> SoundInstances;
typedef std::map< std::string, SoundInstance* > SoundInstanceRegistry;
typedef std::map< std::string, SoundInstance* >::iterator SoundInstanceRegistryIterator;

//===========================================================================================================

//acts as an overarching interface and system for the game
class AudioSystem : EventSubscriberObject{
public:

	AudioSystem();
	~AudioSystem();

	//system helpers
	virtual void StartUp() = 0;
	virtual void ShutDown() = 0;

	//upkeep
	virtual void Update() = 0;

	virtual SoundInstance* CreateOrGetSound(const std::string& soundFileName, unsigned int maxNumberOfCopies, bool allowNullSound = false) = 0;
	virtual SoundInstance* PlayASound(const std::string& soundFileName, 
		int loopCount = 0, float volumeLevel = 1.0f) = 0;
	virtual void StopSoundByName(const std::string& soundFileName) = 0;
	virtual SoundInstance* PlayStreamingSound(SoundID idOfSoundToPlay, int loopCount = 0, float volumeLevel = 1.0f) = 0;

	virtual void EventPlaySound(NamedProperties& params);

	virtual void EventStopSound(NamedProperties& params);
	//virtual void EventCreateOrGetSound(NamedProperties& params);

protected:
	SoundInstanceRegistry m_soundRegistry;
};

extern AudioSystem* theSoundSystem;


//===========================================================================================================

#endif //__includedAudioSystem__

