//==============================================================================================================
//StringTable.cpp
//by Albert Chen Sep-17-2015.
//==============================================================================================================

#include "StringTable.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///gonna be done usually during runtime
const unsigned int GetStringID(const std::string& s){

	//find the string and return it
	for (StringTableMapIterator it = s_stringTable.begin(); it != s_stringTable.end(); ++it){
		StringTableEntry& myStringTableEntry = (it->second);

		if (myStringTableEntry == s){

			return myStringTableEntry.id;

		}

	}

	s_newID++;
	//add to table
	s_stringTable[s_newID] = StringTableEntry(s_newID, s);

	return s_newID;
}

//-----------------------------------------------------------------------------------------------------------

const char* GetStringFromID(const unsigned int& i){
	StringTableMapIterator foundIter = s_stringTable.find(i);

	//found the string already
	if (foundIter != s_stringTable.end()){
		return s_stringTable[i].s.c_str();
	}

	return "";
}