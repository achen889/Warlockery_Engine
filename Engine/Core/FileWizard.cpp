//==============================================================================================================
//FileWizard.cpp
//by Albert Chen Sep-10-2015.
//==============================================================================================================

#include <io.h>
#include "FileWizard.hpp"
#include "Engine/Math/MathUtils.hpp"

//#include "Engine/Multithreading/JobManager.hpp"
//#include "Engine/Multithreading/CriticalSection.hpp"

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

//-----------------------------------------------------------------------------------------------------------

//win32 version of method, search string ext is just the extension to look for (.ext)
bool FindAllFilesOfType(const std::string& directory, const std::string& searchStringExt, Strings& out_filePathsFound, bool recurseFolders){
	//PROFILE_SECTION();
	if (recurseFolders) {
		//get current dir
		FindAllFilesOfTypeInDirectory(directory, searchStringExt, out_filePathsFound);

		//get all sub dirs and search those too
		Strings directoryPathsFound;
		//this stuff doesn't seem to work right
		if (FindAllDirectories(directory, directoryPathsFound)) {
			for (std::string& s : directoryPathsFound) {
				//recurse into sub dirs
				if (FindAllFilesOfTypeInDirectory(s, searchStringExt, out_filePathsFound) ){
					//stuff found :D
				}
			}//end of for
		}
		return true;
	
	}
	else {

		if (FindAllFilesOfTypeInDirectory(directory, searchStringExt, out_filePathsFound)) {
			//stuff found :D
			return true;
		}

	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool FireEventForFilesOfType(const std::string& directory, const std::string& searchStringExt, const std::string& eventNameToFire, bool recurseFolders ) {
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
	if (searchHandle == -1) {
		std::string cannotFindErrorStr = "ERROR: Cannot Find ";

		cannotFindErrorStr += searchPath + "\n";

		ERROR_RECOVERABLE("=====DEBUG FILE WIZARD=====" + cannotFindErrorStr);

		return false;
	}

	//fire event for file
	FireEventForFile(directory, fd.name, searchWildCardExt, eventNameToFire);
	while (_findnext(searchHandle, &fd) == 0) {
		if (CompareCStr(fd.name, ".") != 0 && CompareCStr(fd.name, "..") != 0) {
			//fire event for file
			FireEventForFile(directory, fd.name, searchWildCardExt, eventNameToFire);

			if (recurseFolders) {
				if (BIT_AND(fd.attrib, _A_SUBDIR) != 0) {
					//ConsolePrintf("\ndir<%s>", fd.name);
					FireEventForFilesOfType(directory + fd.name + "/", searchStringExt, eventNameToFire, true);
				}
			}
		}//end of nested if
	}
	//_findnext();

	_findclose(searchHandle);

	return true;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///file helpers

const std::string PushFilePathFromFileName(const std::string& directory, const char* fileName,
	const std::string& searchStringExt, std::vector<std::string>& out_filePathsFound) {

	Strings fileNameTokens;

	std::string fileExtStr;
	int numTokens = SplitString(std::string(fileName), ".", fileNameTokens);
	if (numTokens > 1) {
		for (int i = 1; i < numTokens; i++) {
			fileExtStr += "." + fileNameTokens[i];
		}

	}
	else {
		//these are directories
		//std::string filePathString = "dir: "+std::string(fileName) + "\n";
		//ConsolePrintf(filePathString.c_str());

		std::string pathToFile = fileName;
		//pathToFile += searchWildCardExt;
		pathToFile = directory + pathToFile +"/";

		out_filePathsFound.push_back(pathToFile);

		return pathToFile;
	}
	
	if (fileExtStr == searchStringExt) {
		std::string filePathString;

		std::string pathToFile = fileName;
		//pathToFile += searchWildCardExt;
		pathToFile = directory + pathToFile;

		out_filePathsFound.push_back(pathToFile);

		//print files loaded to output
		filePathString = pathToFile + "\n";
		ConsoleLogPrintf(filePathString.c_str());

		return pathToFile;
	}
	return "";
	//UNUSED(searchStringExt);

}

//-----------------------------------------------------------------------------------------------------------

const std::string FireEventForFile(const std::string& directory, const char*  fileName, const std::string& searchWildCardExt,
	const std::string& eventNameToFire) {

	UNUSED(searchWildCardExt);

	std::string fileExtStr = "";

	Strings fileNameTokens;

	int numTokens = SplitString(std::string(fileName), ".", fileNameTokens);
	if (numTokens > 1) {
		fileExtStr = fileNameTokens[1];
	}

	char fullPathCStr[MAX_PATH];
	_fullpath(fullPathCStr, directory.c_str(), MAX_PATH);

	std::string fullPathString = fullPathCStr;

	FileDefinition fileDef(directory, fileNameTokens[0].c_str(), fileExtStr);

	char fwdSlash = '\\';
	char backSlash = '/';
	ConvertThisCharToThatCharInString(fullPathString, fwdSlash, backSlash);
	fileDef.SetFullPath(fullPathString);

	NamedProperties np;
	np.Set("FileDefinition", fileDef);

	FireEvent(eventNameToFire, np);

	return fileDef.ToString();
}

//-----------------------------------------------------------------------------------------------------------

bool FindAllDirectories(const std::string& directory, std::vector<std::string>& out_filePathsFound) {
	std::string searchWildCard = "*";

	//#ifdef(WIN32) //put this here later for platform stuff
	std::string searchPath = directory + searchWildCard;

	struct _finddata_t fd; // find data struct does fancy stuff
						   //finds first file in director
	intptr_t searchHandle = _findfirst(searchPath.c_str(), &fd);

	//error handling
	if (searchHandle == -1) {
		std::string cannotFindErrorStr = "ERROR: Cannot Find <";

		cannotFindErrorStr += searchPath + ">\n";

		ERROR_RECOVERABLE("=====DEBUG FILE WIZARD=====" + cannotFindErrorStr);

		return false;
	}

	PushFilePathFromFileName(directory, fd.name, "*", out_filePathsFound);

	while ((_findnext(searchHandle, &fd) == 0)) {
		if (CompareCStr(fd.name, ".") != 0 && CompareCStr(fd.name, "..") != 0) {

			PushFilePathFromFileName(directory, fd.name, "*", out_filePathsFound);

			if (BIT_AND(fd.attrib, _A_SUBDIR) != 0) {
				//ConsolePrintf("\ndir<%s>", fd.name);
				FindAllDirectories(directory + fd.name + "/", out_filePathsFound);
			}
		}//end of nested if
	}

	_findclose(searchHandle);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

bool FindAllFilesOfTypeInDirectory(const std::string& directory, const std::string& searchStringExt, std::vector<std::string>& out_filePathsFound) {
	std::string searchWildCard = "*";
	std::string searchWildCardExt;

	searchWildCardExt = searchWildCard + searchStringExt;

	//#ifdef(WIN32) //put this here later for platform stuff

	//method 1 cd directory, search for search string.cstr // changes working directory for the whole app

	//method 2: combine direction and search string, using method 2

	std::string searchPath = directory + searchWildCardExt;

	struct _finddata_t fd; // find data struct does fancy stuff
						   //finds first file in director
	intptr_t searchHandle = _findfirst(searchPath.c_str(), &fd);


	//error handling
	if (searchHandle == -1) {
		std::string cannotFindErrorStr = "ERROR: Cannot Find <";

		cannotFindErrorStr += searchPath + ">\n";

		ConsolePrintString(cannotFindErrorStr);
		
		//ERROR_RECOVERABLE("=====DEBUG FILE WIZARD=====" + cannotFindErrorStr);

		return false;
	}

	PushFilePathFromFileName(directory, fd.name, searchStringExt, out_filePathsFound);

	while ((_findnext(searchHandle, &fd) == 0)) {
		if (CompareCStr(fd.name, ".") != 0 && CompareCStr(fd.name, "..") != 0) {

			PushFilePathFromFileName(directory, fd.name, searchStringExt, out_filePathsFound);
			
// 			CRITICAL_SECTION_SCOPE(pushFileCritSec);
// 			PushFilePathFromFileNameJob* newJob = new PushFilePathFromFileNameJob(directory, fd.name, searchStringExt, out_filePathsFound);
// 			theJobManager->AddPendingJob(newJob, PRIORITY_HIGH);

		}//end of nested if
	}

	//CRITICAL_SECTION_SCOPE(pushFileCritSec);

	_findclose(searchHandle);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================
