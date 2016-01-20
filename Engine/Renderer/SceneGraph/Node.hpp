//==============================================================================================================
//Node.hpp
//by Albert Chen Apr-25-2015.
//==============================================================================================================
#define STATIC

#pragma once

#ifndef _included_Node__
#define _included_Node__

#include "Engine\Math\Matrix4.hpp"
#include <assert.h>
#include "Engine/Math/Math3D.hpp"
#include <iostream>

class Node {
public:
	
	typedef std::vector<Node*> Children;
	typedef Children::const_iterator const_iterator_child;
	struct Traversal {
		Matrix4              _view;
		Matrix4              _invView; //could also be handy for other algorithms
		Matrices _stack;

		Vector3 lookAtTarget;

		void    initToViewMatrix(const Matrix4& view)
		{
			_view = view;
			
			_stack.erase(_stack.begin(), _stack.end());
			_stack.push_back(_view);

		}

		void    PushMatrixStack(const Matrix4& modelMatrix4)
		{
			Matrix4 mv = _stack.back();
			//mv.Transpose();
			_stack.push_back(mv);
			
			//_stack.back().Transform(modelMatrix4);
			_stack.back() =  modelMatrix4 * mv;
			//_stack.back() =  mv * modelMatrix4;
			//mult(modelMatrix4, mv, _stack.back());
		}

		void PopMatrixStack()
		{
			_stack.pop_back();
		}

		const Matrix4& topMatrixStack() const { return _stack.back(); }
	};

	Node(const char* name) : _name(name) , _parent(nullptr)
	{ 
		isTransform = false;
		isBillboard = false;
		m_BSphere.center = Vector3::ZERO;
		m_BSphere.radius = 0.0f;
		//s_nodeCount++; //bookkeeping
		drawBoundingSphere = false;
	}

	//NOTE: Virtual Destructor for the base class.
	//See Scott Meyers, Effective C++, Item 7
	virtual ~Node()
	{
		//assert(s_nodeCount >= 1);  s_nodeCount--;//bookkeeping
	}

	void addChild(Node* child) 
	{
		child->setParent(this);
		_children.push_back(child); 
	}

	virtual void Update() //not const-correct, changes scene graph.
	{
		Children::const_iterator it = _children.begin(), ite = _children.end();
		for (; it != ite; ++it)
			(*it)->Update();

	}

	virtual void Cull(Traversal* trav)  //<- Note const-correctness!
	{
		//if(isVisible){
			//ConsolePrintf("Now culling Visible Node named %s\n", _name );

		//m_BSphere.center = ToVector3(trav->topMatrixStack().m_translation);
		//std::cout << "Node::cull " << _name << std::endl;
		if(!_children.empty()){
			Children::const_iterator it = _children.begin(), ite = _children.end();
			for (; it != ite; ++it)
				(*it)->Cull(trav);
		}


		//}
		
	}

	const std::string& name() const { return _name; }

	const_iterator_child   begin_child() const { return _children.begin();  }

	const_iterator_child   end_child()  const { return _children.end();  }

	//For bookkeeping purposes, to show that shared pointers cleanup automatically!
	static size_t getNodeCount();

	static size_t getNodeVisibleCount();
	
	Node* GetParent(){return _parent; }

	bool isTransform;
	bool isBillboard;

	virtual Sphere3 ComputeBSphere(){
		//m_BSphere.radius = 0.0f;
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

	bool CalcNearFrustumPlaneClipping (Vector3 boundingSphereLocation, float sphereRadius );
	bool CalcFarFrustumPlaneClipping  (Vector3 boundingSphereLocation, float sphereRadius  );
	bool CalcLeftFrustumPlaneClipping (Vector3 boundingSphereLocation, float sphereRadius  );
	bool CalcRightFrustumPlaneClipping(Vector3 boundingSphereLocation, float sphereRadius  );
	bool CalcViewFrustrumCull(Traversal* trav);

	bool CalcFrustumPlaneVisible(float dotOfPlaneNP, float sphereRadius ){
		if(dotOfPlaneNP <= sphereRadius ){
			return true;
		}else{
			return false;
		}
	}

	bool isVisible;



protected:
	void    setParent(Node* parent) { _parent = parent; }
	

	Node*           _parent;
	Children        _children;
	Sphere3 m_BSphere;
	bool drawBoundingSphere;
	std::string     _name;
	//static variables are BAD for mulithreading, btw.
// 	static size_t s_nodeCount;
// 	static size_t s_nodesVisibleCount;
};

// STATIC size_t Node::s_nodeCount = 0;
// STATIC size_t Node::s_nodesVisibleCount = 0;

inline bool Node::CalcNearFrustumPlaneClipping(Vector3 boundingSphereLocation, float sphereRadius ){
	const float nearPlane = 0.1f;
	const Vector3 nearPlaneNDirection = Vector3(0.0f, 0.0f, 1.0f);
	// Distance to the plane: [cx,cy,cz] = sphere center in camera space
	// dist = nx * cx + ny *cy + nz * cz + D
	// plane equation is: nx * x + ny * y + nz * z + D = 0
	// for near plane: n = [0,0,1]. D = nearPlane
	//Notice that p = [0,0,-nearPlane] is on the plane: 0+0+1*(-nearPlane) + nearPlane = 0
	float dotOfNearNP = DotProduct(boundingSphereLocation, nearPlaneNDirection) + nearPlane;

	return CalcFrustumPlaneVisible(dotOfNearNP, sphereRadius);
}

inline bool Node::CalcFarFrustumPlaneClipping(Vector3 boundingSphereLocation, float sphereRadius  ){
	const float farPlane = 100.0f; //sqrt(21.0f*21.0f+20*20);
	const Vector3 farPlaneNDirection = Vector3(0.0f, 0.0f, -1.0f);
	//0*x+0*y+-1*z - farPlane = 0;
	float dotOfFarNP = DotProduct(boundingSphereLocation, farPlaneNDirection ) - farPlane;

	return CalcFrustumPlaneVisible(dotOfFarNP, sphereRadius);

}

inline bool Node::CalcLeftFrustumPlaneClipping(Vector3 boundingSphereLocation, float sphereRadius  ){
	const float halfoffieldOfView = ConvertDegreesToRadians(45.0f * 0.5f);
	const Vector3 leftPlaneNDirection = Vector3(-cos( halfoffieldOfView ), 0.0f, sin(halfoffieldOfView ));

	float dotOfLeftNP = DotProduct(leftPlaneNDirection, boundingSphereLocation);

	return CalcFrustumPlaneVisible(dotOfLeftNP, sphereRadius);
}

inline bool Node::CalcRightFrustumPlaneClipping(Vector3 boundingSphereLocation, float sphereRadius  ){
	const float halfoffieldOfView = ConvertDegreesToRadians(45.0f * 0.5f);
	const Vector3 rightPlaneNDirection = Vector3(cos( halfoffieldOfView ), 0.0f, sin( halfoffieldOfView ));

	float dotOfRightNP = DotProduct(rightPlaneNDirection, boundingSphereLocation);

	return CalcFrustumPlaneVisible(dotOfRightNP, sphereRadius);

}

inline bool Node::CalcViewFrustrumCull(Traversal* trav){

	if( //this works with && but doesn't look perfect at the moment
	   CalcNearFrustumPlaneClipping(m_BSphere.center, m_BSphere.radius ) &&
	   CalcFarFrustumPlaneClipping(m_BSphere.center, m_BSphere.radius)  &&
	   CalcLeftFrustumPlaneClipping(m_BSphere.center, m_BSphere.radius ) &&
	   CalcRightFrustumPlaneClipping(m_BSphere.center, m_BSphere.radius )
	   ){
		isVisible = true;
	}else{

		isVisible = false;
	}

	return isVisible;
}

// inline size_t Node::getNodeCount()
// {
// 	//return 0;
// 	return Node::s_nodeCount;
// }
// 
// inline size_t Node::getNodeVisibleCount()
// {
// 	//return 0;
// 	return Node::s_nodesVisibleCount;
// }

#endif

