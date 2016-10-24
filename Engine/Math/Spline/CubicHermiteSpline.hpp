//==============================================================================================================
//CubicHermiteSpline.hpp
//by Albert Chen Aug-22-2016.
//==============================================================================================================

#pragma once

#ifndef _included_CubicHermiteSpline__
#define _included_CubicHermiteSpline__

#include "CubicHermiteCurve.hpp"

//===========================================================================================================

//must have even number of points to do stuff
class CubicHermiteSpline {
public:
	
	//methods
	CubicHermiteSpline();
	CubicHermiteSpline(const XMLNode& node);
	~CubicHermiteSpline();

	//helpers
	void AddPoint(CubicHermitePoint* point);
	int GenerateCurvesFromPoints(bool isLoop);

	Vector2 CalcPositionAtParametric(float t);
	Vector2 CalcPositionAtParametric(float t, const Vector2& offset);

	//vars
	int m_numPoints = 0;
	CubicHermitePoints m_points;
	CubicHermiteCurves m_curves;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void CubicHermiteSpline::AddPoint(CubicHermitePoint* point) {
	//no null pts allowed
	if (point) {
		if (!m_points.empty()) {
			m_points.back().next = point;
		}
		m_points.push_back(*point);
		m_points.back().next = NULL;
	}
}


//===========================================================================================================

#endif //__includedCubicHermiteSpline__

