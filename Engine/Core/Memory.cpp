//==============================================================================================================
//Memory.cpp
//by Albert Chen Nov-8-2015.
//==============================================================================================================

#include "Memory.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Time.hpp"
#include "Engine\Input\GUI\UIProgressBar.hpp"

#include "SystemClockWin32.hpp"

//===========================================================================================================

ByteTrackerMap* g_byteTrackerMap = NULL;

MemoryUse g_memoryUse;

CriticalSection* memoryCritSec = NULL;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///new/delete helpers

void TrackMemoryAlloc(size_t numBytes){

	//g_memoryUse.g_allocCount++;

	g_memoryUse.CollectAllocMemUse(numBytes);

}

//-----------------------------------------------------------------------------------------------------------

void TrackMemoryDealloc(void* ptr){

	g_memoryUse.g_deallocCount++;

	//erase entry in map
	if (g_byteTrackerMap){
		if (g_byteTrackerMap->size() > 0){
			EraseEntryInByteTrackerMap(ptr);
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

void LogMemoryAllocation(size_t numBytes, const char* file, const size_t& line){
	
	ConsolePrintf("%s(%d): memory new:%d\n", file, line, numBytes );

}

//-----------------------------------------------------------------------------------------------------------

void LogMemoryDeallocation(size_t numBytes, const char* file, const size_t& line){

	ConsolePrintf("%s(%d): memory delete:%d\n", file, line, numBytes );

}

//-----------------------------------------------------------------------------------------------------------

void AddBytesToTrackerMap(void* ptrKey, const size_t& numBytes ){
	memoryCritSec->Enter();

	size_t keyVal = (size_t)ptrKey;
	ByteTrackerMapIterator mapIt = g_byteTrackerMap->find(keyVal);
	if (mapIt != g_byteTrackerMap->end()){
		//if in map, append it
		ByteTrackerData& existingEntry = (mapIt->second);

		existingEntry.bytesAllocated += numBytes;
	}
	else{
		g_byteTrackerMap->insert(ByteTrackerMapEntry((size_t)ptrKey, ByteTrackerData(FILE_NAME, LINE_NUM, numBytes)));
	}
	memoryCritSec->Exit();
}

//-----------------------------------------------------------------------------------------------------------

void AddBytesToTrackerMap(void* ptrKey, const size_t& numBytes, const char* file, const size_t& line){
	if (!memoryCritSec || !g_byteTrackerMap){
		return;
	}

	memoryCritSec->Enter();

	size_t keyVal = (size_t)ptrKey;

	g_memoryUse.g_allocBytesThisFrame += numBytes;

	g_memoryUse.g_bytesAllocated += numBytes;
	
	ByteTrackerMapIterator mapIt = g_byteTrackerMap->find(keyVal);

	//LogMemoryAllocation(numBytes, file, line);
	
	if (mapIt != g_byteTrackerMap->end()){
		//if in map, append it
		ByteTrackerData& existingEntry = (mapIt->second);

		existingEntry.bytesAllocated += numBytes;
	}
	else{
		g_byteTrackerMap->insert(ByteTrackerMapEntry((size_t)ptrKey, ByteTrackerData(file, line, numBytes)));
	}

	memoryCritSec->Exit();
}

//-----------------------------------------------------------------------------------------------------------

void EraseEntryInByteTrackerMap(void* ptr){
	memoryCritSec->Enter();

	size_t keyVal = (size_t)ptr;

	ByteTrackerMapIterator byteIt = g_byteTrackerMap->find(keyVal);
	if (byteIt != g_byteTrackerMap->end()){
		size_t bytesDeallocated = byteIt->second.bytesAllocated;

		g_memoryUse.g_deallocBytesThisFrame += bytesDeallocated;
	
		g_memoryUse.g_bytesDeallocated += bytesDeallocated;

		//LogMemoryDeallocation(bytesDeallocated, byteIt->second.fileName.c_str(), byteIt->second.lineNum);

		g_byteTrackerMap->erase(byteIt);
	}

	memoryCritSec->Exit();
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///debug helpers

void ConsolePrintMemUseInformation(){

	std::string byteUnits = " bytes";
	
	SetLogFileName("Warlockery_Memory_Usage.txt");
	
	std::string byteTrackingString = "\n//===========================================================================================================";
	byteTrackingString += "\nMem Use Log created on " + GetSystem32TimeString();

	byteTrackingString += "\n";
	byteTrackingString += "alloc count: " + IntToString(g_memoryUse.g_allocCount) + "\n";
	byteTrackingString += "dealloc count: " + IntToString(g_memoryUse.g_deallocCount) + "\n";

	byteTrackingString += "total bytes alloc: " + IntToString(g_memoryUse.g_bytesAllocated) + byteUnits + "\n";
	byteTrackingString += "total bytes dealloc: " + IntToString(g_memoryUse.g_bytesDeallocated) + byteUnits + "\n";

	byteTrackingString += "largest alloc: " + IntToString(g_memoryUse.g_largestAllocRequested) + byteUnits + "\n";
	byteTrackingString += "average alloc: " + IntToString(g_memoryUse.g_averageAllocRequested) + byteUnits + "\n";
	
	g_memoryUse.CalcMemLeaks();

	byteTrackingString += "num memory leaks: " + IntToString(g_memoryUse.g_numMemLeaks) + "\n";
	byteTrackingString += "total bytes of memory leak: " + IntToString(g_memoryUse.g_bytesLeaked) + byteUnits + "\n";

	byteTrackingString += "num memory leaks in map: " + IntToString(g_byteTrackerMap->size()) + "\n";

	ConsolePrintString(byteTrackingString);
	LogFilePrintString(byteTrackingString);
	unsigned int memUseBytesLeak = 0;

	std::string byteTrackerMapString = "";

	if (g_byteTrackerMap){
		for (ByteTrackerMapIterator it = g_byteTrackerMap->begin(); it != g_byteTrackerMap->end(); ++it){
			ByteTrackerData& byteTracker = (it->second);

			byteTrackerMapString += "" + byteTracker.fileName + "(" + IntToString(byteTracker.lineNum);
			byteTrackerMapString += "):memory alloc: " + IntToString(byteTracker.bytesAllocated) + "\n";
			memUseBytesLeak += byteTracker.bytesAllocated;

		}

		byteTrackerMapString += "\nnum bytes of memory leak in map: " + IntToString(memUseBytesLeak) + byteUnits + "\n";

		g_byteTrackerMap->clear();
	}

	

	LogFilePrintString(byteTrackerMapString);
	ConsolePrintString(byteTrackerMapString);

	
}

//-----------------------------------------------------------------------------------------------------------

//this doesn't work
void RenderDebugMemUsePerFrame(OpenGLRenderer* renderer){
	size_t allocBytesThisFrame = g_memoryUse.g_allocBytesThisFrame;
	g_memoryUse.g_allocBytesThisFrame = 0;
	size_t deallocBytesThisFrame = g_memoryUse.g_deallocBytesThisFrame;
	g_memoryUse.g_deallocBytesThisFrame = 0;

	if (allocBytesThisFrame != 0){
		//size_t diffOfAllocDealloc = allocBytesThisFrame - deallocBytesThisFrame;

		float percentFixed = (float)deallocBytesThisFrame / (float)allocBytesThisFrame;

		UIProgressBar memUseBar = UIProgressBar(Vector2(50, 50), Vector2(512.0f, 32.0f), Vector2::ONE, Rgba::RED);
		memUseBar.SetElementColor1(Rgba::GREEN);
		//memUseBar.SetScaleAlongY();
		
		memUseBar.Update(GetDeltaSeconds(), percentFixed);

		memUseBar.RenderMesh(renderer);
	}

}

//===========================================================================================================

