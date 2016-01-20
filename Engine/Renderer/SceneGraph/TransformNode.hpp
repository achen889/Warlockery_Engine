
//==============================================================================================================
//TransformNode.hpp
//by Albert Chen Apr-25-2015.
//==============================================================================================================

#include "Node.hpp"
#include "Engine\Math\Matrix4.hpp"
#include <iostream>

#pragma once

#ifndef _included_TransformNode__
#define _included_TransformNode__

class TransformNode : public Node {
public:
//The Strategy Design pattern
class Strategy {
public:
	Strategy(){}
	virtual ~Strategy(){}

	virtual void Update(Matrix4& m) = 0;
};


	TransformNode(const char* name) : Node(name) , _strategy(nullptr) {
		//do nothing
		_transform = Matrix4();
		isTransform = true;
	}

	~TransformNode(){
		//do nothing
	}

	void SetStrategy(Strategy* s);

	virtual void Update()
	{
		if(_strategy)
			_strategy->Update(_transform);
		Node::Update();
	}

	virtual Sphere3 ComputeBSphere(){
		m_BSphere.radius = 0.1f;
		if(!_children.empty()){
			Sphere3 sp;
			Children::const_iterator it = _children.begin(), ite = _children.end();
			for (; it != ite; ++it){
				sp = (*it)->ComputeBSphere();
				
			}
			m_BSphere = Union(sp, m_BSphere); 
		}
		
		return m_BSphere;
	}

	virtual void Cull(Traversal* trav) {
		//std::cout << "TransformNode::cull " << _name << std::endl;
		//view frustum culling
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

		
		trav->PushMatrixStack(_transform);
		
		if(isVisible){

			Node::Cull(trav);
		}

		trav->PopMatrixStack();
	}

	Matrix4& GetTransformMatrix(){
		return _transform;
	}

protected:
	Matrix4  _transform;
	Strategy*  _strategy;

}; //end of transform node class

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void TransformNode::SetStrategy(Strategy* s) { 
	_strategy = s;
}

#endif