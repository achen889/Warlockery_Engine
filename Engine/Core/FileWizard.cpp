//==============================================================================================================
//FileWizard.cpp
//by Albert Chen Sep-10-2015.
//==============================================================================================================

#include <io.h>
#include "FileWizard.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///file magic helpers

FileDefinition StringToFileDefinition(const std::string& stringFileDef){

	FileDefinition outFileDef;

	//get file path
	std::string fullfilePath = "";
	Strings fileDefTokens = SplitString(stringFileDef, "/");

	for (StringsIterator it = fileDefTokens.begin(); it != fileDefTokens.end() - 1; ++it){
		std::string& filePath = (*it);
		fullfilePath += filePath;
		fullfilePath += "/";
	}

	//get name and ext
	StringsIterator strIt = fileDefTokens.end() - 1;

	std::string& strFileName = (*strIt);
	Strings fileNameTokens = SplitString(strFileName, ".");

	std::string fileName = fileNameTokens[0];
	std::string fileExt = fileNameTokens[1];

	//set file def
	outFileDef.m_filePath = fullfilePath;
	outFileDef.m_fileName = fileName;
	outFileDef.m_fileExtension = fileExt;

	return outFileDef;

}


//win32 version of method, search string ext is just the extension to look for (.ext)
bool FindAllFilesOfType(const std::string& directory, const std::string& searchStringExt, Strings& out_filePathsFound){
	std::string searchWildCardExt = "*";

	searchWildCardExt = searchWildCardExt + searchStringExt;

//#ifdef(WIN32) //put this here later for platform stuff

	//method 1 cd directory, search for search string.cstr // changes working directory for the whole app

	//method 2: combine direction and search string, using method 2

	std::string searchPath = directory + searchWildCardExt;

	struct _finddata_t fd; // find data struct does fancy stuff
	
	//finds first file in director
	intptr_t searchHandle = _findfirst(searchPath.c_str(), &fd);
	//error handling
	if (searchHandle == -1){
		std::string cannotFindErrorStr = "ERROR: Cannot Find ";

		cannotFindErrorStr += searchPath + "\n";

		ERROR_RECOVERABLE("=====DEBUG FILE WIZARD====="+cannotFindErrorStr);

		return false;
	}
	

	PushFilePathFromFileName(directory, fd.name, searchWildCardExt, out_filePathsFound);
	
	while (_findnext(searchHandle, &fd) == 0){

		PushFilePathFromFileName(directory, fd.name, searchWildCardExt, out_filePathsFound);

	}
	//_findnext();

	_findclose(searchHandle);
	

	return true;
}

//-----------------------------------------------------------------------------------------------------------

const std::string PushFilePathFromFileName(const std::string& directory, const char* fileName, const std::string& searchWildCardExt, std::vector<std::string>& out_filePathsFound){
	UNUSED(searchWildCardExt);
	std::string pathToFile = fileName;
	//pathToFile += searchWildCardExt;
	pathToFile = directory + pathToFile;

	out_filePathsFound.push_back(pathToFile);

	//print files loaded to output
	std::string filePathString = pathToFile + "\n";
	ConsolePrintf(filePathString.c_str());

	return pathToFile;
	
}
