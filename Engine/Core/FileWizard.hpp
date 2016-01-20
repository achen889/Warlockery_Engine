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
	std::string m_fileName;
	std::string m_fileExtension;

	FileDefinition(){

	}

	std::string GetDefinition(){
		return m_filePath + m_fileName + +"."+m_fileExtension;
	}

};

typedef std::vector<FileDefinition> FileDefinitions;
typedef std::vector<FileDefinition>::iterator FileDefinitionIterator;

//===========================================================================================================

FileDefinition StringToFileDefinition(const std::string& stringFileDef);

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///file magic methods

//searchStringExt is .ext / returns the fileName of each thing
bool FindAllFilesOfType(const std::string& directory, const std::string& searchStringExt, std::vector<std::string>& out_filePathsFound);

///----------------------------------------------------------------------------------------------------------
///file helpers

const std::string PushFilePathFromFileName(const std::string& directory, const char*  fileName, const std::string& searchWildCardExt , std::vector<std::string>& out_filePathsFound);

//===========================================================================================================

#endif //__includedFileWizard__