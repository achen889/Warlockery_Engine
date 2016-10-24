//==============================================================================================================
//CubicHermiteSpline.cpp
//by Albert Chen Aug-22-2016.
//==============================================================================================================

#include "CubicHermiteSpline.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

CubicHermiteSpline::CubicHermiteSpline(){
	//do nothing
}

CubicHermiteSpline::CubicHermiteSpline(const XMLNode& node) {
	
	int num = node.nChildNode("Point");
	for (int i = 0; i < num; i++) {
		XMLNode tempNode = node.getChildNode("Point", i);
		CubicHermitePoint tempPoint = CubicHermitePoint(tempNode);
		AddPoint(&tempPoint);
	}
	GenerateCurvesFromPoints(true);

}

CubicHermiteSpline::~CubicHermiteSpline(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

int CubicHermiteSpline::GenerateCurvesFromPoints(bool isLoop) {
	
	CubicHermiteCurve curve;
	for (CubicHermitePoint& pt : m_points) {
		CubicHermitePoint* nextPt = pt.GetNextPoint();
		if (nextPt) {
			//create new curve
			CubicHermiteCurve newCurve = CubicHermiteCurve(CubicHermitePoint(pt), CubicHermitePoint(*nextPt));
			m_curves.push_back(newCurve);
		}//end of if
	}//end of for

	if (isLoop) {
		CubicHermitePoint startPoint = m_points[m_points.size() - 1];
		CubicHermitePoint endPoint = m_points[0];
		m_curves.push_back(CubicHermiteCurve(startPoint, endPoint));
	}

	return m_curves.size();
}

//-----------------------------------------------------------------------------------------------------------

Vector2 CubicHermiteSpline::CalcPositionAtParametric(float t){
	t = GetLoopedParametric(t, (float)m_curves.size());
	//if (t > m_curves.size())t -= 1.0f;
	int curveSegment = (int)(t);
	return m_curves[curveSegment].CalcPositionAtParametric(t - (float)curveSegment);
}

//-----------------------------------------------------------------------------------------------------------

Vector2 CubicHermiteSpline::CalcPositionAtParametric(float t, const Vector2& offset){
	t = GetLoopedParametric(t, (float)m_curves.size());
	//if (t > m_curves.size())t -= 1.0f;
	int curveSegment = (int)(t);
	return m_curves[curveSegment].CalcPositionAtParametric(t - (float)curveSegment, offset);
}

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

