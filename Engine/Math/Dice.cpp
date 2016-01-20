//==============================================================================================================
//Dice.cpp
//by Albert Chen Sep-2-2015.
//==============================================================================================================

#include "Dice.hpp"

//===========================================================================================================

int RollDice(const Dice& dice){

	int diceTotal = 0;
	for (int i = 0; i < dice.x; i++){
		diceTotal += GetRandomIntInRange(1, dice.y);
	}

	diceTotal += dice.z;

	return diceTotal;
}

//-----------------------------------------------------------------------------------------------------------

//must be in the form "3d6+7"
int RollDice(const std::string& diceString){
	Dice diceFromString = ParseStringToDice(diceString);

	return RollDice(diceFromString);
}

//-----------------------------------------------------------------------------------------------------------

std::string DiceToString(const Dice& dice){
	std::string diceString = "";

	diceString += IntToString(dice.x) + "d";
	diceString += IntToString(dice.y) + "+-";
	diceString += IntToString(dice.z);

	return diceString;
}

//-----------------------------------------------------------------------------------------------------------


//assume 3d6+7 format, no spaces
Dice ParseStringToDice(const std::string& diceString){

	char* diceCStr = StringToWritableCString(diceString);

	char* currentValue;
	char* tokenInBuffer;

	Dice parsedDice;

	//use this with a char* buffer, parse dice
	currentValue = strtok_s(diceCStr, "d", &tokenInBuffer);
	parsedDice.x = CStringToInt(currentValue);
	if (tokenInBuffer[1] == '+'){
	 	currentValue = strtok_s(NULL, "+", &tokenInBuffer);
	 	parsedDice.y = CStringToInt(currentValue);

		currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
		parsedDice.z = CStringToInt(currentValue);
	}
	else if (tokenInBuffer[1] == '-'){
	 	currentValue = strtok_s(NULL, "-", &tokenInBuffer);
		parsedDice.y = CStringToInt(currentValue);

		currentValue = strtok_s(NULL, "\n", &tokenInBuffer);
		parsedDice.z = -1 * CStringToInt(currentValue);
	}

	//debug
	//std::string getString = "x = " + IntToString(parsedDice.x) + "y = " + IntToString(parsedDice.y) + "z = " + IntToString(parsedDice.z);
	//ConsoleGenericMessageBox(getString, diceString);

	return parsedDice;
}

//===========================================================================================================