//==============================================================================================================
//CubicHermiteCurve.hpp
//by Albert Chen Aug-22-2016.
//==============================================================================================================

#pragma once


#ifndef _included_CubicHermiteCurve__
#define _included_CubicHermiteCurve__

#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/XML/XMLUtils.hpp"


//===========================================================================================================

struct CubicHermitePoint {
	//vars
	Vector2 pos;
	Vector2 vel;

	CubicHermitePoint* next = NULL;
	//methods
	CubicHermitePoint(const Vector2& newPos, const Vector2& newVel, CubicHermitePoint* myNext = NULL) :
		pos(newPos),
		vel(newVel),
		next(myNext)
	{

	}
	CubicHermitePoint(const XMLNode& node){
		pos = ReadXMLAttributeAsVec2(node, "pos");
		vel = ReadXMLAttributeAsVec2(node, "vel");
	}

	//accessors
	CubicHermitePoint* GetNextPoint() { return next; }
	void SetNextPoint(CubicHermitePoint* p) { next = p; }


};

typedef std::vector<CubicHermitePoint> CubicHermitePoints;

//===========================================================================================================

struct CubicHermiteCurve {
	//vars
	CubicHermitePoint startPoint;
	CubicHermitePoint endPoint;

	//methods
	CubicHermiteCurve();
	CubicHermiteCurve(const CubicHermitePoint& start, const CubicHermitePoint& end) :
		startPoint(start),
		endPoint(end)
	{

	}

	
	Vector2 CalcPositionAtParametric(float t);

	Vector2 CalcPositionAtParametric(float t, const Vector2& offset);
};

typedef std::vector<CubicHermiteCurve> CubicHermiteCurves;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline CubicHermiteCurve::CubicHermiteCurve() :
	startPoint(CubicHermitePoint(Vector2::ZERO, Vector2::ZERO)),
	endPoint(CubicHermitePoint(Vector2::ZERO, Vector2::ZERO))
{

}

//===========================================================================================================

#endif //__includedCubicHermiteCurve__

