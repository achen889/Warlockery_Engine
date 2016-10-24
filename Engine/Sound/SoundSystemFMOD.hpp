//==============================================================================================================
//SoundSystemFMOD.hpp
//by Albert Chen Mar-7-2016.
//==============================================================================================================

#pragma once

#ifndef _included_SoundSystemFMOD__
#define _included_SoundSystemFMOD__

//---------------------------------------------------------------------------

#pragma comment( lib, "Engine/Sound/fmod/fmodex_vc.lib" ) // Link in the fmodex_vc.lib static library

//---------------------------------------------------------------------------
#include "fmod/fmod.hpp"
#include <string>
#include <vector>
#include <map>
//#include "Engine/Console/Console.hpp"
//#include "Engine/Core/Utilities.hpp"

#include "AudioSystem.hpp"

//---------------------------------------------------------------------------
//typedef unsigned int SoundID;
typedef void* AudioChannelHandle;
//const unsigned int MISSING_SOUND_ID = 0xffffffff;

typedef std::map< std::string, SoundID > SoundIDRegistry;

static SoundID s_curSoundID = 0;

struct SoundFMOD : public SoundInstance{

	SoundFMOD() {
		//do nothing
	}
	SoundFMOD(const std::string& soundPath);

	void SetChannelAssignedToSound(FMOD::Channel* channel) {
		m_audioChannelHandle = (AudioChannelHandle)channel;
	}

	//sound controls
	bool Play(float volumeLevel = 1.0f);

	void Stop() {
		//stop playing sound channel
		if (m_audioChannelHandle != NULL && IsPlaying()) {
			FMOD::Channel* fmodChannel = (FMOD::Channel*) m_audioChannelHandle;
			fmodChannel->stop();
		}
	}

	//queries
	virtual bool IsValid() const override{
		return (m_fmodSound != NULL);
	}

	bool IsPlaying() {
		return m_isPlaying;
	}

	//vars
	AudioChannelHandle m_audioChannelHandle;
	FMOD::Sound* m_fmodSound;
	bool m_isPlaying = false;
};

typedef std::map<std::string, SoundFMOD*> SoundFMODRegistry;

typedef FMOD::Channel FMODAudioChannel;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///fmod sound system

class SoundSystemFMOD : public AudioSystem{
public:
	SoundSystemFMOD();
	virtual ~SoundSystemFMOD();

	virtual SoundInstance* CreateOrGetSound( const std::string& soundFileName, unsigned int maxNumberOfCopies, bool allowNullSound = false);
	AudioChannelHandle PlayFModSound(SoundFMOD& soundFMOD, float volumeLevel = 1.0f);
	AudioChannelHandle PlayFModSound(const std::string& soundFileName, float volumeLevel = 1.0f);

	virtual SoundInstance* PlayASound(const std::string& soundFileName, 
		int loopCount = 0, float volumeLevel = 1.0f) override;

	virtual SoundInstance* PlayStreamingSound(SoundID idOfSoundToPlay, int loopCount = 0, float volumeLevel = 1.0f);

	void PlayGameSound(const std::string& soundFileName, float volumeLevel = 1.0f);

	virtual void StopSoundByName(const std::string& soundFileName) override{
		SoundFMOD* soundToStop = (SoundFMOD*)CreateOrGetSound(soundFileName, 1, true);
		if (soundToStop) {
			soundToStop->Stop();
		}
	}

	void StopChannel(AudioChannelHandle channel);
	FMODAudioChannel* GetFMODAudioChannelHandle(AudioChannelHandle channel);

	void Update();

	void StartUp();
	void ShutDown();

	void ValidateResult( FMOD_RESULT result );

	FMOD::System* GetFMODSystem() { return m_fmodSystem; }

	void CreateSound(const std::string& soundFilePath, FMOD::Sound*& soundHandle, 
		FMOD_MODE mode = FMOD_DEFAULT, FMOD_CREATESOUNDEXINFO* info = NULL ) {

		m_fmodSystem->createSound(soundFilePath.c_str(), mode, info, &soundHandle);
	}

	void AddSoundFMODToRegistry(const std::string& soundPath, SoundFMOD* soundFMOD) {
		m_soundFMODRegistry[soundPath] = soundFMOD;
		m_soundRegistry[soundPath] = (SoundInstance*)soundFMOD;
	}


protected:
	FMOD::System* m_fmodSystem;
	SoundFMODRegistry m_soundFMODRegistry;
};

//extern SoundSystemFMOD* theSoundSystemFMOD;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

//---------------------------------------------------------------------------
// FMOD startup code based on "GETTING STARTED With FMOD Ex Programmer’s API for Windows" document
//	from the FMOD programming API at http://www.fmod.org/download/
//
inline SoundSystemFMOD::SoundSystemFMOD() : AudioSystem(),
	m_fmodSystem(nullptr)
{
// 	if (!theSoundSystemFMOD) {
// 		theSoundSystemFMOD = this;
// 	}
}

//-----------------------------------------------------------------------------------------------
inline SoundSystemFMOD::~SoundSystemFMOD()
{
	//do nothing
}

//---------------------------------------------------------------------------
inline void SoundSystemFMOD::ValidateResult(FMOD_RESULT result) {
	ASSERT_RECOVERABLE(result == FMOD_OK, Stringf("Engine/Audio SYSTEM ERROR: Got error result code %d.\n", result));

}

//===========================================================================================================





//===========================================================================================================

#endif