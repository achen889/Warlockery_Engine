//==============================================================================================================
//GeometryNode.hpp
//by Albert Chen Apr-25-2015.
//==============================================================================================================

#pragma once
#include "Node.hpp"
#include "Engine/Core/Rgba.hpp"

#ifndef _included_GeometryNode__
#define _included_GeometryNode__

class GeometryNode : public Node {
public:
//The Strategy Design pattern
	class Strategy {
	public:
		Strategy(){}
		virtual ~Strategy(){}

		virtual void Update(Matrix4& m) = 0;
	};

	GeometryNode(const char* name) : Node(name) {
		//do nothing
	}

	GeometryNode(const char* name, float newRadius, Rgba color ) : Node(name) {
		//do nothing
		radius = newRadius;
		myColor = new Rgba();
		*myColor = color;
	}

	virtual Sphere3 ComputeBSphere(){
		//m_BSphere.center = Vector3::ZERO;
		m_BSphere.radius = radius;
		if(!_children.empty()){
			Sphere3 sp;
			Children::const_iterator it = _children.begin(), ite = _children.end();
			for (; it != ite; ++it){
				sp = (*it)->ComputeBSphere();
			}
			m_BSphere = Union(m_BSphere, sp);
		}

		return m_BSphere;
	}

// 	virtual void Update()
// 	{
// 		if(_strategy)
// 			_strategy->Update();
// 	}

	void SetStrategy(Strategy* s);


	virtual void Cull(Traversal* trav) 
	{
		Matrix4 viewM = trav->topMatrixStack();

// 		float viewGMV[16] = { viewM.m_iBasis.x, viewM.m_jBasis.x, viewM.m_kBasis.x, viewM.m_translation.x,
// 			viewM.m_iBasis.y, viewM.m_jBasis.y, viewM.m_kBasis.y, viewM.m_translation.y,
// 			viewM.m_iBasis.z, viewM.m_jBasis.z, viewM.m_kBasis.z, viewM.m_translation.z,
// 			viewM.m_iBasis.w, viewM.m_jBasis.w, viewM.m_kBasis.w, viewM.m_translation.w };

		float viewGMV[16] = { viewM.m_iBasis.x, viewM.m_iBasis.y, viewM.m_iBasis.z, viewM.m_iBasis.w,
			viewM.m_jBasis.x, viewM.m_jBasis.y, viewM.m_jBasis.z, viewM.m_jBasis.w,
			viewM.m_kBasis.x, viewM.m_kBasis.y, viewM.m_kBasis.z, viewM.m_kBasis.w,
			viewM.m_translation.x, viewM.m_translation.y, viewM.m_translation.z, viewM.m_translation.w };

		
		//glLoadIdentity();
		
		glLoadMatrixf(viewGMV);

		m_BSphere.center = ToVector3(trav->topMatrixStack().m_translation);
		m_BSphere = ComputeBSphere();
		CalcViewFrustrumCull(trav);
		
		if(drawBoundingSphere){
			glPushMatrix();
			glColor3f(1.0f, 0.0f , 1.0f);
			//glTranslatef(m_BSphere.center.x,m_BSphere.center.y, m_BSphere.center.z );
			renderWireSphere(m_BSphere.radius, 10, 10 );
			glPopMatrix();
		}
		
		if(isVisible){

			glColor3f(myColor->fR(), myColor->fG(), myColor->fB());
			renderWireTeapot(radius);
		
			Node::Cull(trav);
			//std::cout << "GeometryNode::cull" << std::endl;
		}
		


	}

	Strategy*  _strategy;
	float radius;
	Rgba* myColor;
};

inline void GeometryNode::SetStrategy(Strategy* s) { 
	_strategy = s;
}



#endif