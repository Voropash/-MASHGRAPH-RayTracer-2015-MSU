#pragma once
#include "Types.h"
#include "Math.h"
#include "l3ds.h"
#include <iostream>
#include "common/Math.h"
#include "common/AxisAlignedBox.h"
#include "common/Vector2.h"
using namespace std;

	// from 3 task shader
struct VertexData
{
	 Common::Vector3 pos;
	 Common::Vector3 nor;
	 Common::Vector2 tex; // not used, for additional task
};
	// from 3 task MyObject
struct Mesh
{
	VertexData* pData;	//pointer to object's internal data
	unsigned int dataCount;
	unsigned int* pIndices;	//pointer to indexes (list of vetrices) 
	unsigned int indicesCount;
	Common::AxisAlignedBox box;
};


struct object
{
	Mesh * own_mesh;
		// for instancing
	Common::Vector3 translate_mesh;
	Common::Vector3 scale_mesh;
	Common::Radian rotate;
	Common::Vector3 axis;
		// round box
	Common::AxisAlignedBox box;
		// material
	float reflection;
	bool shadow;
	bool blink;
		// translate rgb
		// color of material
	Common::Vector3 translate_color;  
		// checking for hit
	bool check3( SRay ray, double &t, VertexData * ug_1, VertexData * ug_2, VertexData * ug_3, Common::Vector3 & n );

	object(object * o);
	object();
};

struct CScene
{
		// Set of objects
	std::vector <object *> obj;
	CScene();
};