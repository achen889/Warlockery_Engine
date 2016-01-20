//==============================================================================================================
//MathCommands.cpp
//by Albert Chen Jun-21-2015.
//==============================================================================================================

#include "MathCommands.hpp"
#include "Engine/Console/DevConsole.hpp"

//===========================================================================================================
//basic math commands

CONSOLE_COMMAND(add){
	const unsigned int sumTextLineSkipValue = 500;
	std::string sumText;
	if (COMMAND_HAS_ARGS){
		float* numsToAdd = new float[NUMBER_OF_ARG_TOKENS];

		float sumOfArgs = 0.0f;

		sumText = "Sum of Args: ";

		for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
			numsToAdd[i] = CStringToFloat(ARG_TOKENS[i]);
			sumText += FloatToString(numsToAdd[i]);
			if (i < NUMBER_OF_ARG_TOKENS - 1)sumText +=" + ";
		}

		for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
			sumOfArgs += numsToAdd[i];
		}

		sumText += " = " + FloatToString(sumOfArgs);
		sumText += "\n";
		//ConsoleGenericMessageBox(sumText);
		OUTPUT_STRING_TO_CONSOLE(sumText, sumTextLineSkipValue);
	}
}

CONSOLE_COMMAND(mult){
	const unsigned int productTextLineSkipValue = 500;
	std::string productText;
	if (COMMAND_HAS_ARGS){
		float* numsToMult = new float[NUMBER_OF_ARG_TOKENS];

		float productOfArgs = 1.0f;

		productText = "Product of Args: ";

		for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
			numsToMult[i] = CStringToFloat(ARG_TOKENS[i]);
			productText += FloatToString(numsToMult[i]);
			if (i < NUMBER_OF_ARG_TOKENS - 1)productText +=" * ";
		}

		for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
			productOfArgs *= numsToMult[i];
		}

		productText += " = " + FloatToString(productOfArgs);
		productText += "\n";
		//ConsoleGenericMessageBox(productText);
		OUTPUT_STRING_TO_CONSOLE(productText, productTextLineSkipValue);
		//theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, productText, DEV_CONSOLE_OUTPUT_COLOR, productTextLineSkipValue);
	}
}

CONSOLE_COMMAND(sub){
	const unsigned int diffTextLineSkipValue = 500;

	if (COMMAND_HAS_ARGS){
		float* numsToSub = new float[NUMBER_OF_ARG_TOKENS];

		float diffOfArgs;

		std::string diffText = "======Difference of Args=====\n";

		for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
			numsToSub[i] = CStringToFloat(ARG_TOKENS[i]);
			diffText += FloatToString(numsToSub[i]);
			if(i < NUMBER_OF_ARG_TOKENS-1)diffText+=" - ";
		}

		diffOfArgs = numsToSub[0];

		for (int i = 1; i < NUMBER_OF_ARG_TOKENS; i++){
			diffOfArgs -= numsToSub[i];
		}
		
		diffText += " = " + FloatToString(diffOfArgs);
		diffText += "\n";
		//ConsoleGenericMessageBox(sumText);
		//theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, diffText, DEV_CONSOLE_OUTPUT_COLOR, diffTextLineSkipValue);
		OUTPUT_STRING_TO_CONSOLE(diffText, diffTextLineSkipValue);
	}
}

CONSOLE_COMMAND(pow){
	const unsigned int powTextLineSkipValue = 500;

	if (COMMAND_HAS_ARGS){
		float baseVal;
		float powerVal;

		std::string powText = "=====POW=====\n";
		if (NUMBER_OF_ARG_TOKENS == 2){
			baseVal = ARG_TOKEN_F(0);
			powerVal = ARG_TOKEN_F(1);

			float powTotalVal = 1.0f;
			for (int i = 0; i < powerVal; i++){
				powTotalVal *= baseVal;
			}
			powText += FloatToString(baseVal) + " ^ " + FloatToString(powerVal) + " = " + FloatToString(powTotalVal);
		}

		theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, powText, DEV_CONSOLE_OUTPUT_COLOR, powTextLineSkipValue);
	}
}

//-----------------------------------------------------------------------------------------------------------
//bin/hex methods
CONSOLE_COMMAND(isPow2){

	if (COMMAND_HAS_ARGS){
		unsigned int numToCheck = 1;
		if (NUMBER_OF_ARG_TOKENS > 0){
			numToCheck = CStringToInt(ARG_TOKENS[0]);
		}

		if (IsPowerOfTwo(numToCheck)){
			std::string toCheckText = IntToString(numToCheck);
			toCheckText += " is a power of 2";

			ConsoleGenericMessageBox(toCheckText);
		}
		else{
			std::string toCheckText = IntToString(numToCheck);
			toCheckText += " is not a power of 2";
		}
	}	
}

CONSOLE_COMMAND(toBin){
	const unsigned int binTextLineSkipValue = 500;
	if (COMMAND_HAS_ARGS){
		int* numsToBin;
		if (NUMBER_OF_ARG_TOKENS > 0){
			numsToBin = new int[NUMBER_OF_ARG_TOKENS];
			std::string binText = "====To Binary=====\n";
			for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
				numsToBin[i] = ARG_TOKEN_I(i);
				binText += IntToString(numsToBin[i]) + "-> To Binary ->" + IntToString(numsToBin[i], 2) + "\n";
			}
			OUTPUT_STRING_TO_CONSOLE(binText, binTextLineSkipValue);
			//theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, binText, DEV_CONSOLE_OUTPUT_COLOR, binTextLineSkipValue);
		}
	}
	
}

CONSOLE_COMMAND(toHex){
	const unsigned int hexTextLineSkipValue = 500;
	if (COMMAND_HAS_ARGS){
		int* numsToHex;
		if (NUMBER_OF_ARG_TOKENS > 0){
			numsToHex = new int[NUMBER_OF_ARG_TOKENS];
			std::string hexText = "=====To Hexadecimal=====\n";
			for (int i = 0; i < NUMBER_OF_ARG_TOKENS; i++){
				numsToHex[i] = ARG_TOKEN_I(i);
				hexText += IntToString(numsToHex[i]) + "-> To Hexadecimal ->" + IntToString(numsToHex[i], 16) + "\n";
			}
			//theTextSystem->SetVAOStringTextureCoords(theTextSystem->m_textIn, DEV_CONSOLE_OUTPUT_POSITION, hexText, DEV_CONSOLE_OUTPUT_COLOR, hexTextLineSkipValue);
			OUTPUT_STRING_TO_CONSOLE(hexText, hexTextLineSkipValue);
		}
	}
	
}

//vector math commands
CONSOLE_COMMAND(VECLENGTH){
	Vector2 argVec2;
	Vector3 argVec3;
	if (NUMBER_OF_ARG_TOKENS == 2){
		argVec2.x = ARG_TOKEN_F(0);
		argVec2.y = ARG_TOKEN_F(1);

		std::string argVecLengthText = "=====Vector2 Length=====\n";
		argVecLengthText += argVec2.ToString() + " -> CalcLength = ";
		float argLength = argVec2.CalcLength();
		argVecLengthText += FloatToString(argLength) + "\n";

		ConsoleGenericMessageBox(argVecLengthText);
	}

	else if (NUMBER_OF_ARG_TOKENS == 3){
		argVec3.x = ARG_TOKEN_F(0);
		argVec3.y = ARG_TOKEN_F(1);
		argVec3.z = ARG_TOKEN_F(2);
		std::string argVecLengthText = "=====Vector3 Length=====\n";
		argVecLengthText += argVec3.ToString() + " -> CalcLength = ";
		float argLength = argVec3.CalcLength();
		argVecLengthText += FloatToString(argLength) + "\n";

		ConsoleGenericMessageBox(argVecLengthText);
	}
}

CONSOLE_COMMAND(rolldice) {
	const unsigned int sumTextLineSkipValue = 500;
	std::string diceText;
	if (COMMAND_HAS_ARGS) {
		int diceMax = ARG_TOKEN_I(0);

		diceText += "\nRoll Dice 1d" + IntToString(diceMax)+"";
		
		int rollNum = GetRandomIntInRange(1, diceMax);

		diceText += " = " + IntToString(rollNum);
		diceText += "\n";

		printf("%s", diceText.c_str());
		//ConsoleGenericMessageBox(diceText);
		//OUTPUT_STRING_TO_CONSOLE(diceText, sumTextLineSkipValue);
	}
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///methods

MathCommands::MathCommands(){
	RegisterDefaultMathCommands();

	RegisterVectorCommands();
}

void MathCommands::RegisterDefaultMathCommands(){
	//math funcs
	REGISTER_CONSOLE_COMMAND(isPow2, "Returns whether the integer arg is a power of two.");
	REGISTER_CONSOLE_COMMAND(add, "Add the arguments.");
	REGISTER_CONSOLE_COMMAND(sub, "Subtract the arguments.");
	REGISTER_CONSOLE_COMMAND(pow, "Arg 1 to the power of Arg2.");
	REGISTER_CONSOLE_COMMAND(mult, "Multiply the arguments.");

	//dec/hex/bin
	REGISTER_CONSOLE_COMMAND(toBin, "Displays binary value of integer arg.");
	REGISTER_CONSOLE_COMMAND(toHex, "Displays hex value of integer arg.");

	//random funcs
	REGISTER_CONSOLE_COMMAND(rolldice, "rolldice X: rolls a die of 1dX.");

}

void MathCommands::RegisterVectorCommands(){
	//vector commands
	REGISTER_CONSOLE_COMMAND(VECLENGTH, "Displays the length of the vector.");
}