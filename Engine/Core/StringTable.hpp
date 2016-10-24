//============================================================================================================
//StringTable.hpp
//by Albert Chen Sep-17-2015.
//==============================================================================================================

#pragma once

#ifndef _included_StringTable__
#define _included_StringTable__

#include <map>
#include <string>
#include <vector>

typedef std::vector<std::string> Strings;
typedef std::vector<std::string>::iterator StringsIterator;

static unsigned int s_newID = 0;

//===========================================================================================================

struct StringTableEntry{
	unsigned int id;
	std::string s;
	
	StringTableEntry(){
		//do nothing
	}

	//constructor
	StringTableEntry(const unsigned int& newID, const std::string& newS) :
		id(newID),
		s(newS)
	{

	}

	///----------------------------------------------------------------------------------------------------------
	///Equals Operator
	bool operator==(const StringTableEntry& stringTableEntryToEqual) const {
		if (id == stringTableEntryToEqual.id){
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------------------------------------
	bool operator==(const std::string& stringToEqual) const {
		if (s == stringToEqual){
			return true;
		}
		return false;
	}
	///----------------------------------------------------------------------------------------------------------
	///Not Equals Operator
	bool operator!=(const StringTableEntry& stringTableEntryToNotEqual) const {
		if (id != stringTableEntryToNotEqual.id){
			return true;
		}
		return false;
	}


	

};

typedef unsigned int StringID;

typedef std::map<unsigned int, StringTableEntry> StringTableMap;
typedef std::map<unsigned int, StringTableEntry>::iterator StringTableMapIterator;

static StringTableMap s_stringTable;

//===========================================================================================================

const unsigned int GetStringID(const std::string& s);

const char* GetStringFromID(const unsigned int& i);

//===========================================================================================================

#endif //__includedStringTable__


