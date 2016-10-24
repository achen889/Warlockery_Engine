//==============================================================================================================
//Memory.hpp
//by Albert Chen Nov-8-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Memory__
#define _included_Memory__

#include "Engine\Console\Console.hpp"
#include "Engine\Multithreading\CriticalSection.hpp"
#include <map>

//===========================================================================================================

//#define FAST_CALL _fastcall //function args are passed to registers

#define KB = 1024;

#define MB = 1024 * 1024;

#define LOG_MEM_ALLOC(numBytes) LogMemAlloc(numBytes, FILE_NAME, LINE_NUM )

extern bool doDebugMemory;

struct MemoryUse{
	//vars
	size_t g_allocCount = 0;
	size_t g_deallocCount = 0;
	size_t g_numMemLeaks = 0;
	size_t g_bytesLeaked = 0;
	size_t g_bytesAllocated = 0;
	size_t g_bytesDeallocated = 0;
	size_t g_largestAllocRequested = 0;
	size_t g_averageAllocRequested = 0;
	//frame data
	size_t g_allocBytesThisFrame = 0;
	size_t g_deallocBytesThisFrame = 0;
	//helpers
	void CalcMemLeaks(){
		g_numMemLeaks = g_allocCount - g_deallocCount;
		g_bytesLeaked = g_bytesAllocated - g_bytesDeallocated;
	}

	void CollectAllocMemUse(size_t numBytes){
		g_allocCount++;
		g_allocBytesThisFrame += numBytes;

		g_bytesAllocated += numBytes;
		CalcAverageAlloc();
		if (g_largestAllocRequested < numBytes){
			g_largestAllocRequested = numBytes;
		}
		//CalcMemLeaks();
	}

	void CollectDeallocMemUse(size_t numBytes){
		g_deallocCount++;
		g_bytesDeallocated += numBytes;
	}

	void CalcAverageAlloc(){
		g_averageAllocRequested = g_bytesAllocated / g_allocCount;
	}

};

extern MemoryUse g_memoryUse;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///new/delete overload helpers

void TrackMemoryAlloc(size_t numBytes);

void TrackMemoryDealloc(void* ptr);

void LogMemoryAllocation(size_t numBytes, const char* file, const size_t& line);

void LogMemoryDeallocation(size_t numBytes, const char* file, const size_t& line);
//===========================================================================================================

extern CriticalSection* memoryCritSec;

inline void InitializeMemoryCritSec(){
	if (!memoryCritSec){
		memoryCritSec = new CriticalSection();
	}
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///byte tracking

struct ByteTrackerData{

	ByteTrackerData(){
		//do nothing
	}

	ByteTrackerData(const char* file, const size_t& line, const size_t& bytes) :
		fileName(file),
		lineNum(line),
		bytesAllocated(bytes)
	{

	}

	//vars
	std::string fileName;
	size_t lineNum;
	size_t bytesAllocated;

};

typedef std::map< size_t, ByteTrackerData> ByteTrackerMap;
typedef std::map< size_t, ByteTrackerData>::iterator ByteTrackerMapIterator;
typedef std::pair<size_t, ByteTrackerData> ByteTrackerMapEntry;

extern ByteTrackerMap* g_byteTrackerMap;

inline void InitializeByteTrackerMap(){

	InitializeMemoryCritSec();

	if (!g_byteTrackerMap){

		memoryCritSec->Enter();
		g_byteTrackerMap = new ByteTrackerMap();
		memoryCritSec->Exit();
	}
}

void AddBytesToTrackerMap(void* ptrKey, const size_t& numBytes, const char* file, const size_t& line);

void AddBytesToTrackerMap(void* ptrKey, const size_t& numBytes);

void EraseEntryInByteTrackerMap(void* ptr);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///basic new/delete pair

inline void* operator new(size_t numBytes){
	//this is my new
	void* allocPtr = malloc(numBytes);
	
	if (doDebugMemory) {
	//InitializeByteTrackerMap();

		TrackMemoryAlloc(numBytes);

		//LogMemoryAllocation(numBytes, FILE_NAME, LINE_NUM);

	//AddBytesToTrackerMap(allocPtr, numBytes, FILE_NAME, LINE_NUM);
	}


	return allocPtr;
}

 
//-----------------------------------------------------------------------------------------------------------

// somehow this can be paired with the file, line one
inline void operator delete(void* ptr){
	//this is my delete, is paired with just about all of them
	
	if (doDebugMemory) {
		TrackMemoryDealloc(ptr);
	}
	
	
	free(ptr);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///new/delete pair with file, line

inline void* operator new(size_t numBytes, const char* file, const size_t& line){

	void* allocPtr = malloc(numBytes);
	
	if (doDebugMemory) {
		InitializeByteTrackerMap();

		TrackMemoryAlloc(numBytes);

		AddBytesToTrackerMap(allocPtr, numBytes, file, line);
	}
	
	return allocPtr;

}

//-----------------------------------------------------------------------------------------------------------

inline void operator delete(void* ptr, const char* file, const size_t& line){

	//do nothing
	UNUSED(file);
	UNUSED(line);
	
 	free(ptr);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///new/delete pair with array

inline void* operator new[](std::size_t numBytes ){

	void* allocPtr = malloc(numBytes);

	if (doDebugMemory) {
		TrackMemoryAlloc(numBytes);
	}
	
	return allocPtr;
}

//-----------------------------------------------------------------------------------------------------------

inline void operator delete[](void* ptr){

	if (doDebugMemory) {
		TrackMemoryDealloc(ptr);
	}

	free(ptr);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///new/delete pair with array

inline void* operator new[](std::size_t numBytes, const char* file, const size_t& line){

	void* allocPtr = malloc(numBytes);

	if (doDebugMemory) {
		InitializeByteTrackerMap();

		TrackMemoryAlloc(numBytes);

		AddBytesToTrackerMap(allocPtr, numBytes, file, line);
	}

	return allocPtr;

}

//-----------------------------------------------------------------------------------------------------------

inline void operator delete[](void* ptr, const char* file, const size_t& line){
	//do nothing
	UNUSED(file);
	UNUSED(line);

	free(ptr);
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///new define macros

#define new new( FILE_NAME, LINE_NUM)

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///debug helpers

class OpenGLRenderer;

void ConsolePrintMemUseInformation();

void RenderDebugMemUsePerFrame(OpenGLRenderer* renderer);

//===========================================================================================================

#endif //__includedMemory__

