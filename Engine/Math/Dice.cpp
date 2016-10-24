//==============================================================================================================
//Dice.cpp
//by Albert Chen Sep-2-2015.
//==============================================================================================================

#include "Dice.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///dice methods



Dice::Dice(const std::string& diceString) {
	char* diceCStr = StringToWritableCStr(diceString);

	char* currentValue;
	char* tokenInBuffer;

	//default z to 0
	diceDef.z = 0;

	//use this with a char* buffer, parse dice
	currentValue = strtok_s(diceCStr, "d", &tokenInBuffer);
	diceDef.x = CStringToInt(currentValue);

	if (tokenInBuffer[1] == '+' || tokenInBuffer[2] == '+' || tokenInBuffer[3] == '+') {
		currentValue = strtok_s(NULL, "+", &tokenInBuffer);
		diceDef.y = CStringToInt(currentValue);

		currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
		diceDef.z = CStringToInt(currentValue);
	}
	else if (tokenInBuffer[1] == '-') {
		currentValue = strtok_s(NULL, "-", &tokenInBuffer);
		diceDef.y = CStringToInt(currentValue);

		currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
		diceDef.z = -1 * CStringToInt(currentValue);
	}
	else {
		currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
		diceDef.y = CStringToInt(currentValue);
	}
}

//-----------------------------------------------------------------------------------------------------------

const Dice& Dice::operator=(const std::string& diceStrToAssign) {

	*this = Dice(diceStrToAssign);
	
	return *this;
}

//-----------------------------------------------------------------------------------------------------------

const int Dice::RollDice() const {
	int diceTotal = 0;
	for (int i = 0; i < diceDef.x; i++) {
		diceTotal += GetRandomIntInRange(1, diceDef.y);
	}

	diceTotal += diceDef.z;

	return diceTotal;
}


//===========================================================================================================

int RollDice(const Dice& dice){

	return dice.RollDice();

}

//-----------------------------------------------------------------------------------------------------------

//must be in the form "3d6+7"
int RollDice(const std::string& diceString){
	Dice diceFromString = diceString;

	return RollDice(diceFromString);
}

//-----------------------------------------------------------------------------------------------------------

std::string DiceToString(const Dice& dice){
	std::string diceString = "";

	diceString += IntToString(dice.diceDef.x) + "d";
	diceString += IntToString(dice.diceDef.y) + "+-";
	diceString += IntToString(dice.diceDef.z);

	return diceString;
}

//-----------------------------------------------------------------------------------------------------------


//assume 3d6+7 format, no spaces
Dice ParseStringToDice(const std::string& diceString){

	Dice parsedDice = diceString;

	return parsedDice;
}

//===========================================================================================================