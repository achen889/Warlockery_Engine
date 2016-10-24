//==============================================================================================================
//AnimatedProperty.hpp
//by Albert Chen Apr-18-2016.
//==============================================================================================================

#pragma once
#include "KeyFrameSequence.hpp"

#ifndef _included_AnimatedProperty__
#define _included_AnimatedProperty__

//===========================================================================================================

struct BaseAnimatedProperty {

	//methods

	//constructor
	BaseAnimatedProperty() {
		//do nothing
	}

	BaseAnimatedProperty(const XMLNode& node);

	virtual ~BaseAnimatedProperty() {
		//do nothing
	}

	//accessors
	virtual float GetCurrentAnimTime() {
		return m_animCurrentSeconds;
	}
	virtual void SetDuration(float durationSeconds) {
		m_animDurationSeconds = durationSeconds;
		m_oneOverDurationSeconds = 1.0f / m_animDurationSeconds;
	}
	virtual float GetParametric() {

		return GetLoopedParametric(m_animCurrentSeconds * m_oneOverDurationSeconds);
	}

	//upkeep
	virtual void Update(double deltaSeconds) = 0;
	
protected:
	//vars
	float m_animDurationSeconds;
	float m_oneOverDurationSeconds;
	float m_animCurrentSeconds;
};

typedef std::vector<BaseAnimatedProperty*> AnimatedProperties;

///----------------------------------------------------------------------------------------------------------
///inline methods

inline BaseAnimatedProperty::BaseAnimatedProperty(const XMLNode& node) :
	m_animCurrentSeconds(0.0f)
{
	float durationSeconds = ReadXMLAttributeAsFloat(node, "durationSeconds", 0.1f);
	SetDuration(durationSeconds);



}

//===========================================================================================================

template<typename T>
struct AnimatedProperty : public BaseAnimatedProperty {

	//methods
	AnimatedProperty(){
		//do nothing
	}
	~AnimatedProperty() {
		//do nothing
	}

	AnimatedProperty(const XMLNode& node) : BaseAnimatedProperty(node),
		m_keyFrameSequence(KeyFrameSequence<T>(node)) {

		m_propertyName = ReadXMLAttributeAsString(node, "property");

	}

	AnimatedProperty(const T& inProperty):
		m_currentPropertyValue(inProperty)
	{
		//do nothing
	}
	
	//accessors
	
	const T GetCurrentValue() { return m_currentPropertyValue; }

	void AddKeyFrame(const T& value, float parametric);

	//upkeep
	void Update(double deltaSeconds);

//vars
	std::string m_propertyName;
	T m_currentPropertyValue;
	KeyFrameSequence<T> m_keyFrameSequence;

};

///----------------------------------------------------------------------------------------------------------
///inline methods

template<typename T>
void AnimatedProperty<T>::AddKeyFrame(const T& value, float parametric) {

	m_keyFrameSequence.AddKeyFrame(value, parametric);

}


//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline void AnimatedProperty<T>::Update(double deltaSeconds) {
	
	if (m_keyFrameSequence.GetNumKeyFrames() == 1) {
		m_currentPropertyValue = (T)m_keyFrameSequence.GetFirstKeyFrameValue();

		return;
	}

	if (m_animCurrentSeconds < m_animDurationSeconds) {
		m_animCurrentSeconds += (float)deltaSeconds;
		float animLerpParamtric = GetParametric();

		T outLerpValue = m_keyFrameSequence.GetValueAtParametric(animLerpParamtric);
		//set current value
		m_currentPropertyValue = outLerpValue;
	}
	else {
		//if looping anim
		m_animCurrentSeconds = 0.0f;

		T outLerpValue = m_keyFrameSequence.GetValueAtParametric(0.0f);
		//set current value
		m_currentPropertyValue = outLerpValue;
	}

}

//===========================================================================================================

#endif //__includedAnimatedProperty__

