
//==============================================================================================================
//Utilities.hpp
//by Albert Chen Feb-10-2015.
//==============================================================================================================

#ifndef _included_Utilities__
#define _included_Utilities__

#include "Engine/Core/EventCallback.hpp"

#include "Engine/Core/StringUtils.hpp"

#include "Engine/Core/Performance.hpp"

//-----------------------------------------------------------------------------------------------------------
//common file paths
const std::string COMMON_TEXTURE_FILE_PATH = "Data/Images/";
const std::string COMMON_MODEL_FILE_PATH = "Data/Models/";
const std::string COMMON_SHADER_FILE_PATH = "Data/Shaders/";

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///i/o save/load

bool WriteExistingBufferToTextFile(const std::string& buffer, const std::string& filePath);
bool WriteExistingBufferToTextFile(const char* buffer, size_t bufferSize, const std::string& filePath);
bool WriteExistingUCharBufferToFile(unsigned char* buffer, size_t bufferSize, const std::string& filePath);
bool WriteExistingByteVectorToFile(std::vector<unsigned char> existingbyteVector, const std::string& filePath);

//existing buffers
bool LoadFileToExistingCStr(const std::string& filePath, char* existingBuffer, size_t existingBufferSize);
bool LoadFileToExistingUCharBuffer(const std::string& filePath, unsigned char* existingBuffer, size_t existingBufferSize );
bool LoadBinaryFileToExistingByteVector(const std::string& filePath, std::vector<unsigned char>& existingByteVector);
bool LoadFileToExistingString(const std::string& filePath, std::string& existingBuffer);
//new buffers
char* LoadTextFileToNewBuffer(const std::string& filePath );
unsigned char* LoadBinaryFileToNewBuffer(const std::string& filePath, size_t& out_bufferSize);

//===========================================================================================================


//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///event system helpers

void InitEventSystem();

int FireEvent(const std::string& event_name, NamedProperties& args = NamedProperties::EMPTY ); //int for how many responders

int GetNumEventsFired();

void RegisterEventCallback(const std::string& event_name, EventCallbackFunction* callbackFunction); //add it to some registry

void UnregisterEventCallback(const std::string& event_name, EventCallbackFunction* callbackFunction); //find the one and remove it

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
void RegisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance); //add it to some registry

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
void UnregisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance); //find the one and remove it

template<typename T_OBJECT_TYPE>
void UnregisterEventObjectInstance(const std::string& event_name, T_OBJECT_TYPE& objectInstance); //find the one and remove it

///----------------------------------------------------------------------------------------------------------
///inline template functions

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
inline void RegisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance) {

	EventSystem& eventSys = EventSystem::GetInstance();

	eventSys.RegisterEventMethodCallback(event_name, callbackMethod, objectInstance);

}

//-----------------------------------------------------------------------------------------------------------

template<typename T_METHOD_PTR_TYPE, typename T_OBJECT_TYPE>
inline void UnregisterEventMethodCallback(const std::string& event_name, T_METHOD_PTR_TYPE callbackMethod, T_OBJECT_TYPE& objectInstance) {
	EventSystem& eventSys = EventSystem::GetInstance();

	eventSys.UnregisterEventMethodCallback(event_name, callbackMethod, objectInstance);

}

//-----------------------------------------------------------------------------------------------------------

template<typename T_OBJECT_TYPE>
void UnregisterEventObjectInstance(const std::string& event_name, T_OBJECT_TYPE& objectInstance) {
	EventSystem& eventSys = EventSystem::GetInstance();

	eventSys.UnregisterEventObjectInstance(event_name, objectInstance);
}


//===========================================================================================================

#endif