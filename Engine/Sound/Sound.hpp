
//==============================================================================================================
//Sound.hpp
//by Albert Chen Feb-10-2015.
//==============================================================================================================

#ifndef _included_Sound__
#define _included_Sound__

#include <vector>
#include <string>
#include <map>

//#pragma comment(lib, "ck.lib")

//===========================================================================================================

typedef int SoundID;

struct SoundImpl;

class Sound{
public:
	 Sound(const std::string& soundPath );
	~Sound();
	//queries
	bool IsValid() const;
	bool Play(int loopCount, float volumeLevel );
	bool IsPlaying();
	bool IsReady();
	bool NeedsReset(){ return m_doesNeedReset; };
	void Reset();
	void Stop();
private:
	bool m_isValid;
	bool m_doesNeedReset;
	SoundImpl* m_soundImpl;

};
//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline Sound::~Sound(){
	//do nothing
}

typedef std::vector<Sound*> Sounds;

typedef std::map<SoundID, Sounds> SoundMap;

//===========================================================================================================

#endif