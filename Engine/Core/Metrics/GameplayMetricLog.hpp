//==============================================================================================================
//GameplayMetricLog.hpp
//by Albert Chen May-22-2016.
//==============================================================================================================

#pragma once

#ifndef _included_GameplayMetricLog__
#define _included_GameplayMetricLog__

#include "GameplayMetric.hpp"

//===========================================================================================================

class BaseGameplayMetricLog {
public:
	//vars
	char* m_name;

	//methods
	BaseGameplayMetricLog() {
		//do nothing
	}
	BaseGameplayMetricLog(const std::string& name) :
		m_name(StringToWritableCStr(name))
	{
		//do nothing
	}
	virtual ~BaseGameplayMetricLog() {
		//do nothing
	}

	//helpers
	virtual void SetName(const std::string& name) { m_name = StringToWritableCStr(name); }
	virtual std::string GetName() { return std::string(m_name); }

	//pure virtual helpers
	virtual bool IsOfType(const type_info& typeToCompare) = 0;

};

//-----------------------------------------------------------------------------------------------------------

template<typename T>
class GameplayMetricLog : public BaseGameplayMetricLog {
public:

	typedef std::vector<GameplayMetric<T>> GameplayMetrics;
	typedef std::vector<GameplayMetric<T>*> GameplayMetricPtrs;

	//vars
	GameplayMetrics m_gameplayMetrics;

	//methods
	GameplayMetricLog() : BaseGameplayMetricLog() {
		//do nothing
	}
	GameplayMetricLog(const std::string& name) :
		BaseGameplayMetricLog(name)
	{
		//do nothing
	}

	//log queries
	virtual bool IsOfType(const type_info& typeToCompare) override {
		const type_info&  myType = typeid(T);
		if (myType == typeToCompare) {
			return true;
		}
		return false;
	}

	//metric mutators
	void AddGameplayMetric(const T& value) {
		GameplayMetric<T> myMetric(value);

		AddGameplayMetric(myMetric);
	}
	void AddGameplayMetric(const T& value, const double& my_timestamp) {
		GameplayMetric<T> myMetric(value, my_timestamp);

		AddGameplayMetric(myMetric);
	}
	void AddGameplayMetric(GameplayMetric<T>& metric) {
		m_gameplayMetrics.push_back(metric);
	}
	
	//metric helpers
	T GetValueAtParametric(double timestamp); //probably cut this function
	size_t GetNumMetrics() { return m_gameplayMetrics.size(); }
	const T GetFirstGameplayMetricValue() {
		return m_gameplayMetrics[0].value;
	}
	const T GetLastGameplayMetricValue() {
		size_t lastIndex = GetNumMetrics() - 1;
		return (T)m_gameplayMetrics[lastIndex].value;
	}
	GameplayMetric<T>* GetLastGameplayMetric() {
		size_t lastIndex = GetNumMetrics() - 1;
		return &m_gameplayMetrics[lastIndex];
	}
	const GameplayMetrics FindClosestMetricPairForParametricValue(double parametric);

	//special metric queries
	int GetAllMetricsOverLastNSeconds(GameplayMetricPtrs& out_metricPtrs, double currentSeconds, double durationSeconds);
	int GetNumEntriesOverLastNSeconds(double currentSeconds, double durationSeconds);
	bool GetAverageValueOverLastNSeconds(T& metricAverage, double currentSeconds, double durationSeconds);
	
	bool GetHighestValueOverLastNSeconds(T& out_highestValue, double currentSeconds, double durationSeconds);
	bool GetLowestValueOverLastNSeconds(T& out_lowestValue, double currentSeconds, double durationSeconds);
	const T GetMostRecentValue();

	//debug metrics

};

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///inline methods

template<typename T>
inline T GameplayMetricLog<T>::GetValueAtParametric(double timestamp) {
	//do stuff
	ASSERT_RECOVERABLE(!m_gameplayMetrics.empty(), "GameplayMetricLog Uninitialized!!");

	GameplayMetrics framePair = FindClosestMetricPairForParametricValue(timestamp);

	double frameLerpParametric = Interpolate(framePair[0].timestamp, framePair[1].timestamp, timestamp);

	T lerpedValue = Interpolate(framePair[0].value, framePair[1].value, frameLerpParametric);

	return lerpedValue;
}

template<typename T>
inline const std::vector<GameplayMetric<T>> GameplayMetricLog<T>::FindClosestMetricPairForParametricValue(double timestamp) {

	std::vector<GameplayMetric<T>> framePair;
	framePair.reserve(2);
	framePair.push_back(GameplayMetric<T>());
	framePair.push_back(GameplayMetric<T>());

	int index = 0;
	bool foundEqual = false;
	for (GameplayMetric<T>& frame : m_gameplayMetrics) {
		if (frame.timestamp == timestamp) {
			framePair[0] = frame;
			framePair[1] = frame;
			foundEqual = true;
			break;
		}
		else if (frame.timestamp < timestamp) {
			framePair[0] = frame;
		}

		index++;
	}

	if (!foundEqual) {
		for (std::vector<GameplayMetric<T>>::reverse_iterator rit = m_gameplayMetrics.rbegin(); rit != m_gameplayMetrics.rend(); ++rit) {
			GameplayMetric<T>& frame = (*rit);

			if (frame.timestamp > timestamp) {
				framePair[1] = frame;
			}

		}//for end to begin
	}

	return framePair;

}

///----------------------------------------------------------------------------------------------------------
///special metric queries

//returns the size of the out_metricPtrs
template<typename T>
inline int GameplayMetricLog<T>::GetAllMetricsOverLastNSeconds(GameplayMetricPtrs& out_metricPtrs, double currentSeconds, double durationSeconds) {

	double currentDuration = 0.0;
	double currentTimeSeconds = currentSeconds;
	GameplayMetrics::reverse_iterator metricReverseIt;

	int outMetricCount = 0;

	for (metricReverseIt = m_gameplayMetrics.rbegin(); metricReverseIt != m_gameplayMetrics.rend(); ++metricReverseIt) {
		GameplayMetric<T>& metric = (*metricReverseIt);

		currentDuration += metric.GetDurationSinceTimeSeconds(currentTimeSeconds);
		if (currentDuration >= durationSeconds) {
			break;
		}
		else{
			currentTimeSeconds = metric.timestamp;
			out_metricPtrs.push_back(&metric);
			outMetricCount++;
		}
	}

	return outMetricCount;

}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline int GameplayMetricLog<T>::GetNumEntriesOverLastNSeconds(double currentSeconds, double durationSeconds) {
	GameplayMetricPtrs tempMetrics;

	return GetAllMetricsOverLastNSeconds(tempMetrics, currentSeconds, durationSeconds);
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline bool GameplayMetricLog<T>::GetAverageValueOverLastNSeconds(T& metricAverage, double currentSeconds, double durationSeconds) {
	
	GameplayMetricPtrs metricsWithinDuration;

	int numMetricsWithinDuration = GetAllMetricsOverLastNSeconds(metricsWithinDuration, currentSeconds, durationSeconds);
	if (numMetricsWithinDuration <= 0 ) {
		return false;
	}
	
	T metricTotal = 0.0f;

	for (GameplayMetric<T>* metricPtr : metricsWithinDuration) {
		GameplayMetric<T>& metric = (*metricPtr);
			
		metricTotal += (T)metric.value;
		
	}//end of for

	T metricAvg = metricTotal / (float)numMetricsWithinDuration;
	metricAverage = metricAvg;

	return true;
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline bool GameplayMetricLog<T>::GetHighestValueOverLastNSeconds(T& out_highestValue, double currentSeconds, double durationSeconds) {
	
	GameplayMetricPtrs metricsWithinDuration;

	int numMetricsWithinDuration = GetAllMetricsOverLastNSeconds(metricsWithinDuration, currentSeconds, durationSeconds);
	if (numMetricsWithinDuration <= 0) {
		return false;
	}
	 
	T highestValue = 0.0f; //has warning

	for (GameplayMetric<T>* metricPtr : metricsWithinDuration) {
		GameplayMetric<T>& metric = (*metricPtr);
		if (metric.value > highestValue) {
			highestValue = metric.value;
		}
	}
	out_highestValue = highestValue;

	return true;
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline bool GameplayMetricLog<T>::GetLowestValueOverLastNSeconds(T& out_lowestValue, double currentSeconds, double durationSeconds) {

	GameplayMetricPtrs metricsWithinDuration;

	int numMetricsWithinDuration = GetAllMetricsOverLastNSeconds(metricsWithinDuration, currentSeconds, durationSeconds);
	if (numMetricsWithinDuration <= 0) {
		return false;
	}

	T lowestValue = 0.0f;

	for (GameplayMetric<T>* metricPtr : metricsWithinDuration) {
		GameplayMetric<T>& metric = (*metricPtr);
		if (metric.value <= lowestValue) {
			lowestValue = metric.value;
		}
	}
	out_lowestValue = lowestValue;

	return true;
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
inline const T GameplayMetricLog<T>::GetMostRecentValue() {
	return (T)GetLastGameplayMetricValue();
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

#endif //__includedGameplayMetricLog__

