//==============================================================================================================
//GameplayMetric.hpp
//by Albert Chen May-2-2016.
//==============================================================================================================

#pragma once

#ifndef _included_GameplayMetric__
#define _included_GameplayMetric__

#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Utilities.hpp"

#include <typeinfo> //must include for type info

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///consider two metric gathering modes

//-get from start of App
//-get from start of Game

//===========================================================================================================

struct BaseGameplayMetric {

	//vars
	double timestamp;

	//constructors
	BaseGameplayMetric() :
		timestamp(GetCurrentSeconds())
	{
		//do nothing
	}
	BaseGameplayMetric(const double& my_timestamp) :
		timestamp(my_timestamp)
	{

	}

	virtual ~BaseGameplayMetric() {
		//do nothing
	}

	//methods
	virtual double GetDurationSinceTimeSeconds(double timeSeconds) {
		return timeSeconds - timestamp;
	}

};

//-----------------------------------------------------------------------------------------------------------

//each metric type T must have an interp method and basic math operator
template<typename T>
struct GameplayMetric : public BaseGameplayMetric {

	//vars
	T value;
	
	//methods
	GameplayMetric(const T& property) : 
		BaseGameplayMetric(),
		value(property)
	{
		//do nothing
	}

	GameplayMetric(const T& property, const double& my_timestamp) :
		BaseGameplayMetric(my_timestamp),
		value(property)
	{
		//do nothing
	}
	
	//accessors
	T GetValue() { return value; }
	void SetValue(const T& newValue) { value = newValue; }
	
};

//===========================================================================================================

#endif //__includedGameplayMetric__