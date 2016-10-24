//==============================================================================================================
//Dice.hpp
//by Albert Chen Sep-2-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Dice__
#define _included_Dice__

#include "IntVec3.hpp"
#include "Engine/Core/Utilities.hpp"
#include "MathUtils.hpp"

//typedef IntVec3 Dice; //x for number of dice, y for size of dice (1-y), z for modifier

//dice utility functions

struct Dice {
	//vars
	IntVec3 diceDef;

	//operator overload

	Dice() {
		//do nothing
	}

	Dice(const std::string& diceString);

	const Dice& operator=(const std::string& diceStrToAssign);

	const int RollDice() const ;

	const std::string ToString() {
		return Stringf("%dd%d+%d", diceDef.x, diceDef.y, diceDef.z);
	}

};


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///globals

int RollDice(const Dice& dice);

int RollDice(const std::string& diceString);

std::string DiceToString(const Dice& dice);

Dice ParseStringToDice(const std::string& diceString);

//===========================================================================================================
	





#endif