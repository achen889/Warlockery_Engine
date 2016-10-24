//==============================================================================================================
//FileWizard.hpp
//by Albert Chen Sep-10-2015.
//==============================================================================================================

#pragma once

#ifndef _included_FileWizard__
#define _included_FileWizard__

#include "Utilities.hpp"

struct FileDefinition{
	std::string m_filePath;
	std::string m_fullPath;
	std::string m_fileName;
	std::string m_fileExtension;

	FileDefinition(){
		//do nothing
	}

	FileDefinition(const std::string& directory, const char*  fileName, const std::string& searchWildCardExt):
		m_filePath(directory), 
		m_fileName(fileName),
		m_fileExtension(searchWildCardExt)
	{
		//do nothing
	}

	void SetFullPath(const std::string& fullPath) {
		m_fullPath = fullPath;
	}

	const std::string& GetFullPath() { return m_fullPath; }
	const std::string& GetFilePath() { return m_filePath; }
	const std::string& GetFileName() { return m_fileName; }
	const std::string& GetFileExt() { return m_fileExtension; }

	std::string ToString(){
		return m_filePath + m_fileName + +"."+m_fileExtension;
	}

};

typedef std::vector<FileDefinition> FileDefinitions;
typedef std::vector<FileDefinition>::iterator FileDefinitionIterator;

const FileDefinition GetFileDefinition(const std::string& directory, const char*  fileName, const std::string& searchWildCardExt);

///----------------------------------------------------------------------------------------------------------
///inline methods

inline const FileDefinition GetFileDefinition(const std::string& directory, const char*  fileName, const std::string& searchWildCardExt) {
	return FileDefinition(directory, fileName, searchWildCardExt);
}

//===========================================================================================================

FileDefinition StringToFileDefinition(const std::string& stringFileDef);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///file magic methods

//searchStringExt is .ext / returns the fileName of each thing
bool FindAllFilesOfType(const std::string& directory, const std::string& searchStringExt, std::vector<std::string>& out_filePathsFound, bool recurseFolders = false);

bool FireEventForFilesOfType(const std::string& directory, const std::string& searchStringExt, const std::string& eventNameToFire, bool recurseFolders = false);

///----------------------------------------------------------------------------------------------------------
///file helpers

const std::string PushFilePathFromFileName(const std::string& directory, const char*  fileName, const std::string& searchStringExt , std::vector<std::string>& out_filePathsFound);

const std::string FireEventForFile(const std::string& directory, const char*  fileName, 
	const std::string& searchWildCardExt, const std::string& eventNameToFire );

bool FindAllDirectories(const std::string& directory, std::vector<std::string>& out_filePathsFound);

bool FindAllFilesOfTypeInDirectory(const std::string& directory, const std::string& searchStringExt, std::vector<std::string>& out_filePathsFound);


//===========================================================================================================

#endif //__includedFileWizard__