//==============================================================================================================
//SoundSystemFMOD.cpp
//by Albert Chen Mar-7-2016.
//==============================================================================================================

#include "Engine/Sound/SoundSystemFMOD.hpp"
//#include "Engine/Core/Utilities.hpp"
//#include "Engine/Console/Console.hpp"

//===========================================================================================================

//SoundSystemFMOD* theSoundSystemFMOD = NULL;

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///sound instance

SoundFMOD::SoundFMOD(const std::string& soundPath) {

// 	SoundFMOD* newSoundFMOD;
// 	newSoundFMOD = this;
	m_name = soundPath;
	//m_fmodSound = NULL;

	SoundSystemFMOD* soundSystemFMOD = (SoundSystemFMOD*)theSoundSystem;
	soundSystemFMOD->CreateSound(soundPath, m_fmodSound);
	
	if (m_fmodSound) {
		m_id = s_curSoundID;
		s_curSoundID++;
	}
}

//-----------------------------------------------------------------------------------------------------------

bool SoundFMOD::Play(float volumeLevel ) {
	SoundSystemFMOD* soundSystemFMOD = (SoundSystemFMOD*)theSoundSystem;
	if (soundSystemFMOD && IsValid()) {
		soundSystemFMOD->PlayFModSound(*this, volumeLevel);
		m_isPlaying = true;
		return m_isPlaying;
	}
	return m_isPlaying;
}

//===========================================================================================================

//---------------------------------------------------------------------------
void SoundSystemFMOD::StartUp(){
	const int MAX_AUDIO_DEVICE_NAME_LEN = 256;
	FMOD_RESULT result;
	unsigned int fmodVersion;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS deviceCapabilities;
	char audioDeviceName[ MAX_AUDIO_DEVICE_NAME_LEN ];

	// Create a System object and initialize.
	result = FMOD::System_Create( &m_fmodSystem );
	ValidateResult( result );

	result = m_fmodSystem->getVersion( &fmodVersion );
	ValidateResult( result );

	if( fmodVersion < FMOD_VERSION ){
		ERROR_RECOVERABLE( Stringf("Engine/Audio SYSTEM ERROR!  Your FMOD .dll is of an older version (0x%08x == %d) than that the .lib used to compile this code (0x%08x == %d).\n", fmodVersion, fmodVersion, FMOD_VERSION, FMOD_VERSION ) );
	}

	result = m_fmodSystem->getNumDrivers( &numDrivers );
	ValidateResult( result );

	if( numDrivers == 0 )
	{
		result = m_fmodSystem->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
		ValidateResult( result );
	}
	else
	{
		result = m_fmodSystem->getDriverCaps( 0, &deviceCapabilities, 0, &speakerMode );
		ValidateResult( result );

		// Set the user selected speaker mode.
		result = m_fmodSystem->setSpeakerMode( speakerMode );
		ValidateResult( result );

		if( deviceCapabilities & FMOD_CAPS_HARDWARE_EMULATED )
		{
			// The user has the 'Acceleration' slider set to off! This is really bad
			// for latency! You might want to warn the user about this.
			result = m_fmodSystem->setDSPBufferSize( 1024, 10 );
			ValidateResult( result );
		}

		result = m_fmodSystem->getDriverInfo( 0, audioDeviceName, MAX_AUDIO_DEVICE_NAME_LEN, 0 );
		ValidateResult( result );

		if( strstr( audioDeviceName, "SigmaTel" ) )
		{
			// Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			// PCM floating point output seems to solve it.
			result = m_fmodSystem->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR );
			ValidateResult( result );
		}
	}

	result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
	if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
	{
		// Ok, the speaker mode selected isn't supported by this sound card. Switch it
		// back to stereo...
		result = m_fmodSystem->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
		ValidateResult( result );

		// ... and re-init.
		result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
		ValidateResult( result );
	}

}

//-----------------------------------------------------------------------------------------------
void SoundSystemFMOD::ShutDown()
{
//	FMOD_RESULT result = FMOD_OK;
//	result = FMOD_System_Close( m_fmodSystem );
//	result = FMOD_System_Release( m_fmodSystem );
	m_fmodSystem = nullptr;
}

//---------------------------------------------------------------------------
SoundInstance* SoundSystemFMOD::CreateOrGetSound( const std::string& soundFilePath, unsigned int maxNumberOfCopies, bool allowNullSound){
	UNUSED(maxNumberOfCopies);

	SoundFMODRegistry::iterator found = m_soundFMODRegistry.find( soundFilePath );
	if( found != m_soundFMODRegistry.end() ){
		return (SoundInstance*)found->second;
	}
	else{
		if (allowNullSound) {
			return NULL;
		}
		SoundFMOD* newSoundFMOD;
		newSoundFMOD = new SoundFMOD(soundFilePath);

		AddSoundFMODToRegistry(soundFilePath, newSoundFMOD);

		return (SoundInstance*)newSoundFMOD;
	}
}

//---------------------------------------------------------------------------

AudioChannelHandle SoundSystemFMOD::PlayFModSound(SoundFMOD& soundFMOD, float volumeLevel) {
	FMOD::Channel* channelAssignedToSound = nullptr;

	m_fmodSystem->playSound(FMOD_CHANNEL_FREE, soundFMOD.m_fmodSound, false, &channelAssignedToSound);
	if (channelAssignedToSound){
		channelAssignedToSound->setVolume(volumeLevel);
	}
	soundFMOD.SetChannelAssignedToSound(channelAssignedToSound);

	soundFMOD.m_isPlaying = true;

	return (AudioChannelHandle)channelAssignedToSound;
}

//-----------------------------------------------------------------------------------------------------------

SoundInstance* SoundSystemFMOD::PlayStreamingSound(SoundID idOfSoundToPlay, int loopCount , float volumeLevel ) {
	UNUSED(idOfSoundToPlay);
	UNUSED(loopCount);
	UNUSED(volumeLevel);
	//figure out how to play sound by id in fmod

	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

AudioChannelHandle SoundSystemFMOD::PlayFModSound(const std::string& soundFileName, float volumeLevel ){
	SoundFMOD* sound = (SoundFMOD*)CreateOrGetSound(soundFileName, 1);
	if (sound->IsValid()) {
		return PlayFModSound(*sound, volumeLevel);
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

SoundInstance* SoundSystemFMOD::PlayASound(const std::string& soundFileName, int loopCount , float volumeLevel ) {
	UNUSED(loopCount);
	
	PlayGameSound(soundFileName, volumeLevel);

	return CreateOrGetSound(soundFileName, 1);
}

//-----------------------------------------------------------------------------------------------------------

void SoundSystemFMOD::PlayGameSound(const std::string& soundFileName, float volumeLevel ) {

	AudioChannelHandle handle = PlayFModSound(soundFileName, volumeLevel);
	//FMODAudioChannel* channel = GetFMODAudioChannelHandle(handle);
	UNUSED(handle);
}

//-----------------------------------------------------------------------------------------------------------

void SoundSystemFMOD::StopChannel(AudioChannelHandle channel){
	
	FMODAudioChannel* fModChannelHandle = GetFMODAudioChannelHandle(channel);
	if (fModChannelHandle) {
		fModChannelHandle->stop();
	}
	else {
		ConsolePrintf("ERROR: fmod audio channel invalid!");
	}
}

//-----------------------------------------------------------------------------------------------------------

FMODAudioChannel* SoundSystemFMOD::GetFMODAudioChannelHandle(AudioChannelHandle channel) {
	FMODAudioChannel* fModChannelHandle = (FMOD::Channel*)channel;
	if (fModChannelHandle) {
		return fModChannelHandle;
	}
	return NULL;
}

//--------------------------------------------------------------------------------------------------------
void SoundSystemFMOD::Update(){
	FMOD_RESULT result = m_fmodSystem->update();
	ValidateResult( result );
}

//===========================================================================================================




