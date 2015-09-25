#include "Scene.h"
#include "common/Math.h"
#include "common/AxisAlignedBox.h"  
#include <algorithm>
using namespace std;

#define PI 3.1415
#define EPSILON 0.000001 // for comparing double with zero
float all_scale = 1.0 ; // scale and translate all objects of scene


	// global varible for mesh
Mesh * p_mesh;



object::object() { blink=true; }

object::object(object*o){}


	// building scene
	// l3ds loader from 3 task
CScene::CScene()
{
		// varibles 
	L3DS scene; 
	object * o;

		// loading 2-polygonal model ( more 2 - easy "for"-c )
	if (!scene.LoadFile("highpolygonal/table.3ds")) { cout<<" bad .3ds file \r\n"; return;}	
	//cout<<scene.GetMeshCount(); // number of polygones
	LMesh &mesh = scene.GetMesh(0);
	LMesh &mesh1 = scene.GetMesh(1);
	cout<<mesh.GetTriangleCount()<<"+"<<mesh1.GetTriangleCount()<<" triangles in 3ds. \r\n";
	
	p_mesh = new Mesh();

	p_mesh->dataCount = mesh.GetTriangleCount()*3+mesh1.GetTriangleCount()*3; 
	unsigned int nTriangles = mesh.GetTriangleCount()+mesh1.GetTriangleCount();
	p_mesh->indicesCount = (mesh.GetTriangleCount()+mesh1.GetTriangleCount())*3;
	p_mesh->pData = new VertexData [p_mesh->dataCount];
	p_mesh->pIndices = new unsigned int [p_mesh->indicesCount];
	
		// scale unuse!!! 
	float x_scale = 1;
	float y_scale = 1;
	float z_scale = 1;

	p_mesh->box.setMinimum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	p_mesh->box.setMaximum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	for (int i = 0; i< mesh.GetTriangleCount(); i++)
	{
			p_mesh->pData[i*3].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[0].x*x_scale,mesh.GetTriangle2(i).vertices[0].y,mesh.GetTriangle2(i).vertices[0].z*z_scale);
			p_mesh->pData[i*3].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[0].x*x_scale,mesh.GetTriangle2(i).vertexNormals[0].y*y_scale,mesh.GetTriangle2(i).vertexNormals[0].z*z_scale);
			p_mesh->pData[i*3].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[0].x,mesh.GetTriangle2(i).textureCoords[0].y);		
			p_mesh->pIndices[i*3]=i*3;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);

			p_mesh->pData[i*3+1].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[1].x*x_scale,mesh.GetTriangle2(i).vertices[1].y,mesh.GetTriangle2(i).vertices[1].z*z_scale);
			p_mesh->pData[i*3+1].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[1].x*x_scale,mesh.GetTriangle2(i).vertexNormals[1].y*y_scale,mesh.GetTriangle2(i).vertexNormals[1].z*z_scale);
			p_mesh->pData[i*3+1].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[1].x,mesh.GetTriangle2(i).textureCoords[1].y);		
			p_mesh->pIndices[i*3+1]=i*3+1;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);

			p_mesh->pData[i*3+2].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[2].x*x_scale,mesh.GetTriangle2(i).vertices[2].y*y_scale,mesh.GetTriangle2(i).vertices[2].z*z_scale);
			p_mesh->pData[i*3+2].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[2].x*x_scale,mesh.GetTriangle2(i).vertexNormals[2].y*y_scale,mesh.GetTriangle2(i).vertexNormals[2].z*z_scale);
			p_mesh->pData[i*3+2].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[2].x,mesh.GetTriangle2(i).textureCoords[2].y);		
			p_mesh->pIndices[i*3+2]=i*3+2;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
	}
	for (int i = 0; i< mesh1.GetTriangleCount(); i++)
	{
			p_mesh->pData[mesh.GetTriangleCount()+i*3].pos = Common::Vector3(mesh1.GetTriangle2(i).vertices[0].x*x_scale,mesh1.GetTriangle2(i).vertices[0].y,mesh1.GetTriangle2(i).vertices[0].z*z_scale);
			p_mesh->pData[mesh.GetTriangleCount()+i*3].nor = Common::Vector3(mesh1.GetTriangle2(i).vertexNormals[0].x*x_scale,mesh1.GetTriangle2(i).vertexNormals[0].y*y_scale,mesh1.GetTriangle2(i).vertexNormals[0].z*z_scale);	
			p_mesh->pIndices[mesh.GetTriangleCount()+i*3]=mesh.GetTriangleCount()+i*3;
			if (p_mesh->box.getMinimum().x>mesh1.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMinimumX(mesh1.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh1.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMinimumY(mesh1.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh1.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMinimumZ(mesh1.GetTriangle2(i).vertices[0].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh1.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMaximumX(mesh1.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh1.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMaximumY(mesh1.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh1.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMaximumZ(mesh1.GetTriangle2(i).vertices[0].z*z_scale);

			p_mesh->pData[mesh.GetTriangleCount()+i*3+1].pos = Common::Vector3(mesh1.GetTriangle2(i).vertices[1].x*x_scale,mesh1.GetTriangle2(i).vertices[1].y,mesh1.GetTriangle2(i).vertices[1].z*z_scale);
			p_mesh->pData[mesh.GetTriangleCount()+i*3+1].nor = Common::Vector3(mesh1.GetTriangle2(i).vertexNormals[1].x*x_scale,mesh1.GetTriangle2(i).vertexNormals[1].y*y_scale,mesh1.GetTriangle2(i).vertexNormals[1].z*z_scale);	
			p_mesh->pIndices[mesh.GetTriangleCount()+i*3+1]=mesh.GetTriangleCount()+i*3+1;

			if (p_mesh->box.getMinimum().x>mesh1.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMinimumX(mesh1.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh1.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMinimumY(mesh1.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh1.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMinimumZ(mesh1.GetTriangle2(i).vertices[1].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh1.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMaximumX(mesh1.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh1.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMaximumY(mesh1.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh1.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMaximumZ(mesh1.GetTriangle2(i).vertices[1].z*z_scale);

			p_mesh->pData[mesh.GetTriangleCount()+i*3+2].pos = Common::Vector3(mesh1.GetTriangle2(i).vertices[2].x*x_scale,mesh1.GetTriangle2(i).vertices[2].y*y_scale,mesh1.GetTriangle2(i).vertices[2].z*z_scale);
			p_mesh->pData[mesh.GetTriangleCount()+i*3+2].nor = Common::Vector3(mesh1.GetTriangle2(i).vertexNormals[2].x*x_scale,mesh1.GetTriangle2(i).vertexNormals[2].y*y_scale,mesh1.GetTriangle2(i).vertexNormals[2].z*z_scale);
			p_mesh->pIndices[mesh.GetTriangleCount()+i*3+2]=mesh.GetTriangleCount()+i*3+2;

			if (p_mesh->box.getMinimum().x>mesh1.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMinimumX(mesh1.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh1.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMinimumY(mesh1.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh1.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMinimumZ(mesh1.GetTriangle2(i).vertices[2].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh1.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMaximumX(mesh1.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh1.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMaximumY(mesh1.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh1.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMaximumZ(mesh1.GetTriangle2(i).vertices[2].z*z_scale);
	}
		// debug standart size of model
	/*cout<<p_mesh->box.getMinimum().x<<" "<<p_mesh->box.getMinimum().y<<" "<<p_mesh->box.getMinimum().z<<" ";
	cout<<p_mesh->box.getMaximum().x<<" "<<p_mesh->box.getMaximum().y<<" "<<p_mesh->box.getMaximum().z<<" \r\n";*/
	cout<<"tables:\r\n";
	
	//tabels
	//left
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(-200+24,0.1248-119.855,90-19)*all_scale;
	o->scale_mesh = Common::Vector3(10,15,10)*all_scale;
	o->rotate = 3*PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(100/100,75/100,0);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);
	
	//right
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(200-24,0.1248-119.855,90-19)*all_scale;
	o->scale_mesh = Common::Vector3(10,15,10)*all_scale;
	o->rotate = 3*PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(100/100,75/100,0);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);


	//middle
	o = new object();
	o->reflection=0.5;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(0,0.1248-119.866,90-5)*all_scale;
	o->scale_mesh = Common::Vector3(15,7,15)*all_scale;
	o->rotate = 3*PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(10/100,95/100,1);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);
	
	//nearest
	o = new object();
	o->reflection=0.5;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(0,0.1248-119.866,70-5)*all_scale;
	o->scale_mesh = Common::Vector3(8,3.6,8)*all_scale;
	o->rotate = 3*PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(0.6,0.6,0);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);


	//chair-l
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(-15,0.1248-119.866,54.5)*all_scale;
	o->scale_mesh = Common::Vector3(8.0,3.6,8.0)/2*all_scale;
	o->rotate = 3*PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(0.9,0.4,0);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);

	//chair-r
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(15,0.1248-119.866,54.5)*all_scale;
	o->scale_mesh = Common::Vector3(8.0,3.6,8.0)/2*all_scale;
	o->rotate = 3*PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(0.9,0.4,0);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);


	

	
	
	//room
		// its second part of table, xD =)
	mesh = scene.GetMesh(1);
	cout<<mesh.GetTriangleCount()<<" triangles in 3ds. \r\n";
	p_mesh = new Mesh();
	p_mesh->dataCount = mesh.GetTriangleCount()*3; 
	//nTriangles = mesh.GetTriangleCount();
	p_mesh->indicesCount = mesh.GetTriangleCount()*3;
	p_mesh->pData = new VertexData [p_mesh->dataCount];
	p_mesh->pIndices = new unsigned int [p_mesh->indicesCount];
	
	p_mesh->box.setMinimum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	p_mesh->box.setMaximum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	for (int i = 0; i< mesh.GetTriangleCount(); i++)
	{
			p_mesh->pData[i*3].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[0].x*x_scale,mesh.GetTriangle2(i).vertices[0].y,mesh.GetTriangle2(i).vertices[0].z*z_scale);
			p_mesh->pData[i*3].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[0].x*x_scale,mesh.GetTriangle2(i).vertexNormals[0].y*y_scale,mesh.GetTriangle2(i).vertexNormals[0].z*z_scale);
			p_mesh->pData[i*3].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[0].x,mesh.GetTriangle2(i).textureCoords[0].y);		
			p_mesh->pIndices[i*3]=i*3;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);

			p_mesh->pData[i*3+1].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[1].x*x_scale,mesh.GetTriangle2(i).vertices[1].y,mesh.GetTriangle2(i).vertices[1].z*z_scale);
			p_mesh->pData[i*3+1].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[1].x*x_scale,mesh.GetTriangle2(i).vertexNormals[1].y*y_scale,mesh.GetTriangle2(i).vertexNormals[1].z*z_scale);
			p_mesh->pData[i*3+1].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[1].x,mesh.GetTriangle2(i).textureCoords[1].y);		
			p_mesh->pIndices[i*3+1]=i*3+1;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);

			p_mesh->pData[i*3+2].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[2].x*x_scale,mesh.GetTriangle2(i).vertices[2].y*y_scale,mesh.GetTriangle2(i).vertices[2].z*z_scale);
			p_mesh->pData[i*3+2].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[2].x*x_scale,mesh.GetTriangle2(i).vertexNormals[2].y*y_scale,mesh.GetTriangle2(i).vertexNormals[2].z*z_scale);
			p_mesh->pData[i*3+2].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[2].x,mesh.GetTriangle2(i).textureCoords[2].y);		
			p_mesh->pIndices[i*3+2]=i*3+2;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
	}
	cout<<"room:\r\n";


	//дальняя стена
	o = new object();
	o->reflection=0.25;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(0,0.861-0.79,94.2366-85.6167)*all_scale;
	o->scale_mesh = Common::Vector3(125*212.438/221.29, 136 *(119.866+119.861)/(122.923+123.074) , 50)*all_scale;
	o->rotate = 0;
	o->shadow = false;
	o->translate_color = Common::Vector3(1,1,1)*1.15;
	o->axis = Common::Vector3(0,1,0);
	o->blink = false;
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);

	//невидимая стена
	o = new object();
	o->reflection=0.6;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(0,0.861-0.79,94.2366-85.6167-14.2248-99.2355)*all_scale;
	o->scale_mesh = Common::Vector3(125*212.438/221.29, 136 *(119.866+119.861)/(122.923+123.074) , 50)*all_scale;
	o->rotate = 0;
	o->shadow = false;
	o->translate_color = Common::Vector3(1,1,1)*1.5;
	o->axis = Common::Vector3(0,1,0);
	o->blink = false;
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);

	
	//потолок
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(0,0,40)*all_scale;
	o->scale_mesh = Common::Vector3(120,60,70)*all_scale;
	o->rotate = -PI/2;
	o->shadow = false;
	o->translate_color = Common::Vector3(1,1,1)*0.9;
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);
	
	//пол
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(0,0,40)*all_scale;
	o->scale_mesh = Common::Vector3(120,60,70)*all_scale;
	o->rotate = PI/2;
	o->shadow = false;
	o->translate_color = Common::Vector3(1,1,1)*0.85;
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);


	//левая стена
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(-212.438-126.867+0.007,0,-52+94.2366-2.26993)*all_scale;
	o->scale_mesh = Common::Vector3(60*(14.2918+94.2366)/(54.2248+158.213),80*119.866/72.3964,70)*1.002*all_scale;
	o->rotate = PI/2;
	o->shadow = false;
	o->translate_color = Common::Vector3(1,1,1)*0.8;
	o->axis = Common::Vector3(0,1,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);


	//правая стена
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(-212.438-126.867+219.444+212.436-0.0017-0.35+0.114,0,-52+94.2366-2.26993)*all_scale;
	o->scale_mesh = Common::Vector3(60*(14.2918+94.2366)/(54.2248+158.213)*1.01,80*119.866/72.39641*1.01,70)*1.002*all_scale;
	o->rotate = PI/2;
	o->shadow = false;
	o->translate_color = Common::Vector3(1,1,1)*0.8;
	o->axis = Common::Vector3(0,1,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);
	
	
	
	
	if (!scene.LoadFile("lowpolygonal/monkey.3ds")) { cout<<" bad .3ds file \r\n"; return;}	
	//cout<<scene.GetMeshCount();
	mesh = scene.GetMesh(0);
	cout<<mesh.GetTriangleCount()<<" triangles in 3ds. \r\n";
	
	p_mesh = new Mesh();

	p_mesh->dataCount = mesh.GetTriangleCount()*3; 
	nTriangles = mesh.GetTriangleCount();
	p_mesh->indicesCount = mesh.GetTriangleCount()*3;
	p_mesh->pData = new VertexData [p_mesh->dataCount];
	p_mesh->pIndices = new unsigned int [p_mesh->indicesCount];
	
	p_mesh->box.setMinimum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	p_mesh->box.setMaximum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	for (int i = 0; i< mesh.GetTriangleCount(); i++)
	{
			p_mesh->pData[i*3].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[0].x*x_scale,mesh.GetTriangle2(i).vertices[0].y,mesh.GetTriangle2(i).vertices[0].z*z_scale);
			p_mesh->pData[i*3].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[0].x*x_scale,mesh.GetTriangle2(i).vertexNormals[0].y*y_scale,mesh.GetTriangle2(i).vertexNormals[0].z*z_scale);
			p_mesh->pData[i*3].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[0].x,mesh.GetTriangle2(i).textureCoords[0].y);		
			p_mesh->pIndices[i*3]=i*3;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);

			p_mesh->pData[i*3+1].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[1].x*x_scale,mesh.GetTriangle2(i).vertices[1].y,mesh.GetTriangle2(i).vertices[1].z*z_scale);
			p_mesh->pData[i*3+1].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[1].x*x_scale,mesh.GetTriangle2(i).vertexNormals[1].y*y_scale,mesh.GetTriangle2(i).vertexNormals[1].z*z_scale);
			p_mesh->pData[i*3+1].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[1].x,mesh.GetTriangle2(i).textureCoords[1].y);		
			p_mesh->pIndices[i*3+1]=i*3+1;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);

			p_mesh->pData[i*3+2].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[2].x*x_scale,mesh.GetTriangle2(i).vertices[2].y*y_scale,mesh.GetTriangle2(i).vertices[2].z*z_scale);
			p_mesh->pData[i*3+2].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[2].x*x_scale,mesh.GetTriangle2(i).vertexNormals[2].y*y_scale,mesh.GetTriangle2(i).vertexNormals[2].z*z_scale);
			p_mesh->pData[i*3+2].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[2].x,mesh.GetTriangle2(i).textureCoords[2].y);		
			p_mesh->pIndices[i*3+2]=i*3+2;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
	}
	cout<<"monkey:\r\n";

	//right monkey
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(100,-50+79.5313-119.866,80+8)*all_scale;
	o->scale_mesh = Common::Vector3(30,30,10)*all_scale;
	o->rotate = PI;
	o->shadow = true;
	o->translate_color = Common::Vector3(1,1,1);
	o->axis = Common::Vector3(0,1,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);


	//left monkey
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(-100,-50+79.5313-119.866,80+8)*all_scale;
	o->scale_mesh = Common::Vector3(30,30,10)*all_scale;
	o->rotate = PI;
	o->shadow = true;
	o->translate_color = Common::Vector3(1,1,1);
	o->axis = Common::Vector3(0,1,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);
	


	//teapot
	if (!scene.LoadFile("lowpolygonal/Teapot.3ds")) { cout<<" bad .3ds file \r\n"; return;}	
	//cout<<scene.GetMeshCount();
	mesh = scene.GetMesh(0);
	cout<<mesh.GetTriangleCount()<<" triangles in 3ds. \r\n";
	
	p_mesh = new Mesh();

	p_mesh->dataCount = mesh.GetTriangleCount()*3; 
	nTriangles = mesh.GetTriangleCount();
	p_mesh->indicesCount = mesh.GetTriangleCount()*3;
	p_mesh->pData = new VertexData [p_mesh->dataCount];
	p_mesh->pIndices = new unsigned int [p_mesh->indicesCount];
	
	p_mesh->box.setMinimum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	p_mesh->box.setMaximum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	for (int i = 0; i< mesh.GetTriangleCount(); i++)
	{
			p_mesh->pData[i*3].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[0].x*x_scale,mesh.GetTriangle2(i).vertices[0].y,mesh.GetTriangle2(i).vertices[0].z*z_scale);
			p_mesh->pData[i*3].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[0].x*x_scale,mesh.GetTriangle2(i).vertexNormals[0].y*y_scale,mesh.GetTriangle2(i).vertexNormals[0].z*z_scale);
			p_mesh->pData[i*3].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[0].x,mesh.GetTriangle2(i).textureCoords[0].y);		
			p_mesh->pIndices[i*3]=i*3;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);

			p_mesh->pData[i*3+1].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[1].x*x_scale,mesh.GetTriangle2(i).vertices[1].y,mesh.GetTriangle2(i).vertices[1].z*z_scale);
			p_mesh->pData[i*3+1].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[1].x*x_scale,mesh.GetTriangle2(i).vertexNormals[1].y*y_scale,mesh.GetTriangle2(i).vertexNormals[1].z*z_scale);
			p_mesh->pData[i*3+1].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[1].x,mesh.GetTriangle2(i).textureCoords[1].y);		
			p_mesh->pIndices[i*3+1]=i*3+1;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);

			p_mesh->pData[i*3+2].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[2].x*x_scale,mesh.GetTriangle2(i).vertices[2].y*y_scale,mesh.GetTriangle2(i).vertices[2].z*z_scale);
			p_mesh->pData[i*3+2].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[2].x*x_scale,mesh.GetTriangle2(i).vertexNormals[2].y*y_scale,mesh.GetTriangle2(i).vertexNormals[2].z*z_scale);
			p_mesh->pData[i*3+2].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[2].x,mesh.GetTriangle2(i).textureCoords[2].y);		
			p_mesh->pIndices[i*3+2]=i*3+2;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
	}
	cout<<"teapot:\r\n";

	//first teapot
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(-12,-1.37-92.5573,30+92.5-42.9388)*all_scale;
	o->scale_mesh = Common::Vector3(10,10,10)*all_scale;
	o->rotate = -PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(0.5,1,0.5);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);

	//second teapot
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(12,-1.37-92.5573,30+92.5-42.9388)*all_scale;
	o->scale_mesh = Common::Vector3(10,10,10)*all_scale;
	o->rotate = -PI/2;
	o->shadow = true;
	o->translate_color = Common::Vector3(0.5,1,0.5);
	o->axis = Common::Vector3(1,0,0);
	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);

	
	//bunny
	if (!scene.LoadFile("lowpolygonal/bunny.3ds")) { cout<<" bad .3ds file \r\n"; return;}	
	//cout<<scene.GetMeshCount()<<"!";
	mesh = scene.GetMesh(0);
	cout<<mesh.GetTriangleCount()<<" triangles in 3ds. \r\n";
	
	p_mesh = new Mesh();

	p_mesh->dataCount = mesh.GetTriangleCount()*3; 
	nTriangles = mesh.GetTriangleCount();
	p_mesh->indicesCount = mesh.GetTriangleCount()*3;
	p_mesh->pData = new VertexData [p_mesh->dataCount];
	p_mesh->pIndices = new unsigned int [p_mesh->indicesCount];
	
	p_mesh->box.setMinimum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	p_mesh->box.setMaximum(mesh.GetTriangle2(0).vertices[0].x*x_scale,mesh.GetTriangle2(0).vertices[0].y*y_scale,mesh.GetTriangle2(0).vertices[0].z*z_scale);
	for (int i = 0; i< mesh.GetTriangleCount(); i++)
	{
			p_mesh->pData[i*3].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[0].x*x_scale,mesh.GetTriangle2(i).vertices[0].y,mesh.GetTriangle2(i).vertices[0].z*z_scale);
			p_mesh->pData[i*3].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[0].x*x_scale,mesh.GetTriangle2(i).vertexNormals[0].y*y_scale,mesh.GetTriangle2(i).vertexNormals[0].z*z_scale);
			p_mesh->pData[i*3].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[0].x,mesh.GetTriangle2(i).textureCoords[0].y);		
			p_mesh->pIndices[i*3]=i*3;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[0].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[0].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[0].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[0].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[0].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[0].z*z_scale);

			p_mesh->pData[i*3+1].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[1].x*x_scale,mesh.GetTriangle2(i).vertices[1].y,mesh.GetTriangle2(i).vertices[1].z*z_scale);
			p_mesh->pData[i*3+1].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[1].x*x_scale,mesh.GetTriangle2(i).vertexNormals[1].y*y_scale,mesh.GetTriangle2(i).vertexNormals[1].z*z_scale);
			p_mesh->pData[i*3+1].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[1].x,mesh.GetTriangle2(i).textureCoords[1].y);		
			p_mesh->pIndices[i*3+1]=i*3+1;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[1].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[1].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[1].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[1].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[1].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[1].z*z_scale);

			p_mesh->pData[i*3+2].pos = Common::Vector3(mesh.GetTriangle2(i).vertices[2].x*x_scale,mesh.GetTriangle2(i).vertices[2].y*y_scale,mesh.GetTriangle2(i).vertices[2].z*z_scale);
			p_mesh->pData[i*3+2].nor = Common::Vector3(mesh.GetTriangle2(i).vertexNormals[2].x*x_scale,mesh.GetTriangle2(i).vertexNormals[2].y*y_scale,mesh.GetTriangle2(i).vertexNormals[2].z*z_scale);
			p_mesh->pData[i*3+2].tex = Common::Vector2(mesh.GetTriangle2(i).textureCoords[2].x,mesh.GetTriangle2(i).textureCoords[2].y);		
			p_mesh->pIndices[i*3+2]=i*3+2;
			if (p_mesh->box.getMinimum().x>mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMinimumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMinimum().y>mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMinimumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMinimum().z>mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMinimumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
			if (p_mesh->box.getMaximum().x<mesh.GetTriangle2(i).vertices[2].x*x_scale) p_mesh->box.setMaximumX(mesh.GetTriangle2(i).vertices[2].x*x_scale);
			if (p_mesh->box.getMaximum().y<mesh.GetTriangle2(i).vertices[2].y*y_scale) p_mesh->box.setMaximumY(mesh.GetTriangle2(i).vertices[2].y*y_scale);
			if (p_mesh->box.getMaximum().z<mesh.GetTriangle2(i).vertices[2].z*z_scale) p_mesh->box.setMaximumZ(mesh.GetTriangle2(i).vertices[2].z*z_scale);
	}
	cout<<"bunny:\r\n";

	//first bunny
	o = new object();
	o->reflection=0;
	o->own_mesh = p_mesh;
	o->translate_mesh = Common::Vector3(0,0,20)*all_scale;
	o->scale_mesh = Common::Vector3(25,25,25)*all_scale;
	// some rotatings
		Common::Matrix4 a1,a2;// = Common::Matrix4::IDENTITY; 
		a1.makeTransform( Common::Vector3(0,0,0),  Common::Vector3(1,1,1), Common::Quaternion(o->rotate=3*PI/2, Common::Vector3(1,0,0)));
		a2.makeTransform(Common::Vector3(0,0,0),  Common::Vector3(1,1,1), Common::Quaternion(o->rotate=PI, Common::Vector3(0,0,1)));
		a1=a1*a2;
		Common::Quaternion b;
		a1.decomposition(Common::Vector3(),Common::Vector3(),b);
		b.ToAngleAxis(o->rotate, o->axis);
	o->translate_mesh = Common::Vector3(-45,-119,20+62-20);
	o->shadow = true;
	o->translate_color = Common::Vector3(1,0.5,1);

	{ // matrixes translating
		Common::Matrix4 a1 = Common::Matrix4::IDENTITY; 
		a1.makeTransform(o->translate_mesh, o->scale_mesh, Common::Quaternion(o->rotate, o->axis));
		o->box.setMinimum( a1 * o->own_mesh->box.getMinimum());
		o->box.setMaximum( a1 * o->own_mesh->box.getMaximum());
		if (o->box.getMinimum().x > o->box.getMaximum().x) { auto tmp = o->box.getMinimum().x; o->box.setMinimumX( o->box.getMaximum().x ); o->box.setMaximumX( tmp );  }
		if (o->box.getMinimum().y > o->box.getMaximum().y) { auto tmp = o->box.getMinimum().y; o->box.setMinimumY( o->box.getMaximum().y ); o->box.setMaximumY( tmp );  }
		if (o->box.getMinimum().z > o->box.getMaximum().z) { auto tmp = o->box.getMinimum().z; o->box.setMinimumZ( o->box.getMaximum().z ); o->box.setMaximumZ( tmp );  }
		cout<<o->box.getMinimum().x<<" "<<o->box.getMinimum().y<<" "<<o->box.getMinimum().z<<" ";
		cout<<o->box.getMaximum().x<<" "<<o->box.getMaximum().y<<" "<<o->box.getMaximum().z<<"\r\n";
	}
	obj.push_back(o);
	
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*																		func-s																						  */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// checking for hit in box
bool object::check3( SRay ray, double &t, VertexData * ug_1, VertexData * ug_2, VertexData * ug_3, Common::Vector3 & n)
{
	auto copy_norm_ray = ray.m_dir.length();
		// translating matrix, rays to local coordinates
	Common::Matrix4 a = Common::Matrix4::IDENTITY; 
	a.makeInverseTransform(Common::Vector3::ZERO, scale_mesh,  Common::Quaternion(rotate, axis));
	ray.m_dir = a * ray.m_dir;
	a = Common::Matrix4::IDENTITY; 
	a.makeInverseTransform(translate_mesh, scale_mesh,  Common::Quaternion(rotate, axis));
	ray.m_start = a*ray.m_start;
	
			// find nearest hitting
		// start varible eq 
	float t_min = -1;
	int num_min=-1;
	double u_m, v_m, u, v; 
	Common::Vector3 d;
		// across our mesh
		// algorithm as in ray-tracing.ru , but very optimized, as at wiki.en, but bit changed and added own functions
	for ( int i = 0; i <= own_mesh -> dataCount; i+=3 )
	{
		auto e1 = own_mesh -> pData[i+1].pos - own_mesh -> pData[i].pos;
		auto e2 = own_mesh -> pData[i+2].pos - own_mesh -> pData[i].pos;
		auto T = ray.m_start - own_mesh -> pData[i].pos;		
		auto p = ray.m_dir.crossProduct( e2 );
		d = ray.m_dir;

		double det = p.dotProduct( e1 );
		if( det > -EPSILON && det < EPSILON ) continue;
		double inv_det = 1.0 / det;

		u = inv_det*T.dotProduct(p);
		if( u < 0.f || u > 1.f ) continue;

		auto q = T.crossProduct( e1 );
		v = inv_det * d.dotProduct( q );
		if( v < 0.f || u + v  > 1.f ) continue;

		t = inv_det * e2.dotProduct( q );

			if ( ( t < t_min && t > EPSILON || num_min == -1 ) && ( t == t ) )
			{
				t_min = t;
				u_m = u;
				v_m = v;
				num_min = i;
			}
	}
		// nearest hitting
	t = t_min;
		// there is hit
	if ( num_min != -1 )
	{
		a = Common::Matrix4::IDENTITY; 
		a.makeTransform( translate_mesh, scale_mesh,  Common::Quaternion( rotate, axis ) );

			// angles point of hitting triangle
		ug_1 -> pos = a * ( own_mesh -> pData[num_min].pos );
		ug_2 -> pos = a * ( own_mesh -> pData[num_min+1].pos );
		ug_3 -> pos = a * ( own_mesh -> pData[num_min+2].pos );

		a=a.inverse();
		a=a.transpose();
			
			// angles normales of hitting triangle
		ug_1 -> nor = a * ( own_mesh -> pData[num_min].nor );
		ug_2 -> nor = a * ( own_mesh -> pData[num_min+1].nor );
		ug_3 -> nor = a * ( own_mesh -> pData[num_min+2].nor );

			// smooth normales
		n = u_m * ug_2->nor + v_m * ug_3->nor + ( 1 - u_m - v_m ) * ug_1 -> nor;
			// there is hit 
		return true;
	}
		// no hit
	return false;
}