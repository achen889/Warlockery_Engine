//==============================================================================================================
//SoundSystem.cpp
//by Albert Chen Feb-10-2015.
//==============================================================================================================

#include "Engine/Sound/SoundSystem.hpp"
#include "ck/ck.h"
#include <ck/config.h>
#include <direct.h>

SoundSystem* theSoundSystem = NULL;

//===========================================================================================================

SoundSystem::SoundSystem(){

	m_nextSoundID = 1;

	if(!theSoundSystem ){
		theSoundSystem = this;
	}

}

//-----------------------------------------------------------------------------------------------------------

SoundSystem::~SoundSystem(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

void SoundSystem::StartUp(){
	
	static CkConfig ckConfig;
	CkInit(&ckConfig);

	m_workingDirectory = _getcwd(NULL, 0 );
	m_workingDirectory+="/";

	ReformatFilePath(m_workingDirectory );

}

//-----------------------------------------------------------------------------------------------------------

void SoundSystem::Update(){

	CkUpdate();

	
	//for every sound instance loop through sounds and reset what needs to be reset
	for(SoundMap::iterator soundMapIter = m_soundMap.begin(); soundMapIter != m_soundMap.end(); ++soundMapIter){
		Sounds& soundsInstances = soundMapIter->second;
		for(Sounds::iterator soundsInstancesIter = soundsInstances.begin(); soundsInstancesIter != soundsInstances.end(); ++soundsInstancesIter){
			Sound* sound = (*soundsInstancesIter);
			if(sound && sound->NeedsReset() && !sound->IsPlaying() ){
				sound->Reset();
			}
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

void SoundSystem::ShutDown(){

	CkShutdown();

}

//-----------------------------------------------------------------------------------------------------------

//requires absolute path
SoundID SoundSystem::LoadStreamingSound(const std::string& streamingSoundPath,  unsigned int maxNumberOfCopies){
	SoundID newSoundID = m_nextSoundID++;
	std::string fullPath = m_workingDirectory;
	fullPath += streamingSoundPath;
	ReformatFilePath(fullPath);
	Sounds soundsLoaded;

	for (unsigned int i = 0; i < maxNumberOfCopies; i++){
		Sound* newSound = new Sound(fullPath );

		while(!newSound->IsReady() ){
			CkUpdate();
		}

		if(newSound->IsValid()){
			soundsLoaded.push_back(newSound);
		}else{
			newSoundID = -1;
			delete newSound;
			break;
		}//end of if/else


	}//end of for
	if(newSoundID != -1)
		m_soundMap[newSoundID] = soundsLoaded;

	return newSoundID;
}

//-----------------------------------------------------------------------------------------------------------

Sound* SoundSystem::PlayStreamingSound(SoundID IdOfSoundToPlay, int loopCount , float volumeLevel ){
	
	Sound* soundPicked = NULL;

	SoundMap::iterator soundIDIter = m_soundMap.find(IdOfSoundToPlay );

	if(soundIDIter != m_soundMap.end() ){
		//bool playedSound = false;

		Sounds& sounds = soundIDIter->second;

		for(Sounds::iterator soundIter = sounds.begin(); soundIter != sounds.end(); ++soundIter ){
			Sound* sound = (*soundIter );
			if(sound->IsPlaying() )
				continue;

			sound->Play(loopCount, volumeLevel );
			soundPicked = sound;

			break;
		}//end of for
	}//end of if

	return soundPicked;
}

//-----------------------------------------------------------------------------------------------------------

void SoundSystem::ReformatFilePath(std::string& pathToFormat ){
	char forwardSlash = 92;//'\'';
	for(unsigned int i = 0; i < pathToFormat.length(); i++ ){
		if(pathToFormat[i] == forwardSlash ){
			pathToFormat[i] = '/';
		}
	}
}

//===========================================================================================================