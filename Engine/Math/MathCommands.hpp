//==============================================================================================================
//MathCommands.hpp
//by Albert Chen Jun-21-2015.
//==============================================================================================================

#pragma once

#ifndef _included_MathCommands__
#define _included_MathCommands__

#include "Engine/Math/Math2D.hpp"
#include "Engine/Math/Math3D.hpp"

//===========================================================================================================

class MathCommands{
public:
	 MathCommands();

	 void RegisterDefaultMathCommands();

	 void RegisterVectorCommands();

	 ~MathCommands();
};



//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods
inline MathCommands::~MathCommands(){
	//do nothing
}

#endif