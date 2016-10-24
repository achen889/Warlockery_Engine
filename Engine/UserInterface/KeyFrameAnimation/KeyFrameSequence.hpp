//==============================================================================================================
//KeyFrameSequence.hpp
//by Albert Chen Apr-11-2016.
//==============================================================================================================

#pragma once

#ifndef _included_KeyFrameSequence__
#define _included_KeyFrameSequence__

//===========================================================================================================

template<typename T>
struct KeyFrame {
	T value;
	float parametric;

	KeyFrame(){
		//do nothing
	}
	~KeyFrame() {
		//do nothing
	}

	KeyFrame(const XMLNode& node) {
		parametric = ReadXMLAttributeAsFloat(node, "parametric", 0.0f);

		ReadXMLAttribute(node, "value", value);
	}


	KeyFrame(const T& newVal, float newParametric):
		value(newVal),
		parametric(newParametric){
		//do stuff
	}

// 	const std::string ToString(const T& val) {
// 		return ToString(val);
// 	}
	
	const std::string ToString() {
		return "val = " + ToString(value) + " | " + FloatToString(parametric);
	}

};

//-----------------------------------------------------------------------------------------------------------

class KeyFrameSequenceBase {
public:
	virtual ~KeyFrameSequenceBase() {
		//do nothing
	}
	

};

//-----------------------------------------------------------------------------------------------------------

template<typename T>
class KeyFrameSequence : public KeyFrameSequenceBase {

public:

	typedef std::vector<KeyFrame<T>> KeyFrames;
	//typedef std::vector<KeyFrame<T>>::iterator KeyFramesIterator;

	KeyFrameSequence() {
		//do nothing
	}
	~KeyFrameSequence() {
		//do nothing
	}

	KeyFrameSequence(const XMLNode& node) {
		
		int num = node.nChildNode("KeyFrame");
		for (int i = 0; i < num; i++){
			XMLNode tempNode = node.getChildNode("KeyFrame", i);
			m_keyFrames.push_back(KeyFrame<T>(tempNode));
		}

	}
	
	void AddKeyFrame(const T& value, float parametric) {
		m_keyFrames.push_back(KeyFrame<T>(value, parametric));
	}

	T GetValueAtParametric(float parametric) {
		//do stuff
		ASSERT_RECOVERABLE(!m_keyFrames.empty(), "KeyFrameSequence Uninitialized!!");

		KeyFrames framePair = FindClosestKeyframePairForParametricValue(parametric);

		float frameLerpParametric = Interpolate(framePair[0].parametric, framePair[1].parametric, parametric);

		T lerpedValue = Interpolate(framePair[0].value, framePair[1].value, frameLerpParametric);

		return lerpedValue;
	}

	size_t GetNumKeyFrames() { return m_keyFrames.size(); }

	const T GetFirstKeyFrameValue() {
		return m_keyFrames[0].value;
	}

	const T GetLastKeyFrameValue() {
		size_t lastIndex = GetNumKeyFrames()-1;
		return m_keyFrames[lastIndex].value;
	}

	const KeyFrames FindClosestKeyframePairForParametricValue(float parametric);

	//void SortKeyFrames(); //do this??

//private:

	KeyFrames m_keyFrames;

};

///----------------------------------------------------------------------------------------------------------
///inline methods

template<typename T>
inline const std::vector<KeyFrame<T>> KeyFrameSequence<T>::FindClosestKeyframePairForParametricValue(float parametric) {

	std::vector<KeyFrame<T>> framePair;
	framePair.reserve(2);
	framePair.push_back(KeyFrame<T>());
	framePair.push_back(KeyFrame<T>());

	int index = 0;
	bool foundEqual = false;
	for (KeyFrame<T>& frame : m_keyFrames ) {
		if (frame.parametric == parametric) {
			framePair[0] = frame;
			framePair[1] = frame;
			foundEqual = true;
			break;
		}
		else if (frame.parametric  < parametric) {
			framePair[0] = frame;
		}
	
		index++;
	}
	
	if (!foundEqual) {
		for (std::vector<KeyFrame<T>>::reverse_iterator rit = m_keyFrames.rbegin(); rit != m_keyFrames.rend(); ++rit) {
			KeyFrame<T>& frame = (*rit);

			if (frame.parametric > parametric) {
				framePair[1] = frame;
			}

		}//for end to begin
	}



	//framePair[1] = m_keyFrames[index];

	return framePair;

}



//===========================================================================================================

#endif //__includedKeyFrameSequence__

