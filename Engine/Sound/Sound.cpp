//==============================================================================================================
//Sound.cpp
//by Albert Chen Feb-10-2015.
//==============================================================================================================

#include "Sound.hpp"
#include "ck/sound.h"
#include "Engine/Console/Console.hpp"

//===========================================================================================================

struct SoundImpl{
	CkSound* m_sound;

};

///----------------------------------------------------------------------------------------------------------
///constructor
Sound::Sound(const std::string& absoluteSoundPath )
	:m_soundImpl(NULL),
	 m_isValid(false){

	  //make a sound implementation
	  m_soundImpl = new SoundImpl();
	  //m_soundImpl->m_sound = CkSound::newStreamSound(absoluteSoundPath.c_str(), kCkPathType_FileSystem );
	  if(m_soundImpl->m_sound == NULL){
		  std::string errorMessageText = "ERROR: Sound File failed to Load! FilePath -> "+absoluteSoundPath;
		  std::string errorMessageTitle = "DEBUG: Sound is missing or broken. ";
		  ConsoleGenericMessageBox(errorMessageText.c_str(), errorMessageTitle.c_str());
	  }
	  //if(m_soundImpl->m_sound )
		// m_isValid = true;

	  m_doesNeedReset = false;
	
}

//-----------------------------------------------------------------------------------------------------------

bool Sound::Play(int loopCount, float volumeLevel ){
	if(IsValid() ){
		m_soundImpl->m_sound->setLoopCount(loopCount );
		m_soundImpl->m_sound->setVolume(volumeLevel );
		m_soundImpl->m_sound->play();

		m_doesNeedReset = true;

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool Sound::IsValid() const{ 
	return (bool)( m_soundImpl && m_soundImpl->m_sound );
}

//-----------------------------------------------------------------------------------------------------------

bool Sound::IsPlaying(){
	if(IsValid() ){
		return m_soundImpl->m_sound->isPlaying();
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool Sound::IsReady(){
	if(IsValid() ){
		return m_soundImpl->m_sound->isReady();
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

void Sound::Reset(){
	m_soundImpl->m_sound->stop();
	m_soundImpl->m_sound->setPlayPosition(0);
	m_doesNeedReset = false;
}

//-----------------------------------------------------------------------------------------------------------

void Sound::Stop(){
	m_soundImpl->m_sound->stop();
}

//===========================================================================================================