//==============================================================================================================
//CriticalSection.hpp
//by Albert Chen Nov-3-2015.
//==============================================================================================================

#pragma once

#ifndef _included_CriticalSection__
#define _included_CriticalSection__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Engine/Console/Console.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///critical section macros

//===========================================================================================================

struct CriticalSection{
	//vars
	CRITICAL_SECTION m_criticalSection;

	//methods
	CriticalSection(){
		InitializeCriticalSection(&m_criticalSection);
	}

	void Enter(){
		EnterCriticalSection(&m_criticalSection);
	}

	void Exit(){
		LeaveCriticalSection(&m_criticalSection);
	}

};

struct CriticalSectionScope{
	//vars
	CriticalSection* critSec;

	//methods
	CriticalSectionScope(CriticalSection* cs):
		critSec(cs)
	{
		critSec->Enter();
	}

	~CriticalSectionScope(){
		critSec->Exit();
	}
};

#define CRITICAL_SECTION_SCOPE(name) CriticalSectionScope cs_##name(&name)


//===========================================================================================================

#endif //__includedCriticalSection__

