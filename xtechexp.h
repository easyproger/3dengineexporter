#pragma once

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
//SIMPLE TYPE


#include <impexp.h>
#define OBTYPE_MESH 0
#define OBTYPE_CAMERA 1
#define OBTYPE_OMNILIGHT 2
#define OBTYPE_SPOTLIGHT 3
#define OBTYPE_DUMMY 5
#define OBTYPE_CTARGET 6
#define OBTYPE_LTARGET 7

#include <sstream>
#include <fstream>
#include <vector>


#include "Utils.h"


#define DEG_TO_RAD	0.01745329251994329576f
#define RAD_TO_DEG	57.2957795130823209071f

typedef unsigned int uint;
#define MAX_BONES_ON_VERTEX 4
#define MAX_SIZE_BITMAP 250
#define ATLAS_SIZE 1024



class VNormal
{
   public:
     Point3 norm;
     DWORD smooth;
     VNormal *next;
     BOOL init;
 
     VNormal() {smooth=0;next=NULL;init=FALSE;norm=Point3(0,0,0);}
     VNormal(Point3 &n,DWORD s) {next=NULL;init=TRUE;norm=n;smooth=s;}
     ~VNormal() {delete next;}
     void AddNormal(Point3 &n,DWORD s);
     Point3 &GetNormal(DWORD s);
     void Normalize();
};


class SceneEntry {
	public:

		

		TSTR name;
		INode *node,*tnode;
		Object *obj;
		int type;		// See above
		int id;
		SceneEntry *next;
		SceneEntry(INode *n, Object *o, int t);
		void SetID(int id) { this->id = id; }
};
class ObjectEntry {
	public:
		TriObject *tri;
		SceneEntry *entry;
		ObjectEntry *next;
		ObjectEntry(SceneEntry *e) { entry = e; next = NULL;  tri = NULL; }
};



struct XVectorB {
	float v[MAX_BONES_ON_VERTEX];
};

struct Vert3ds {
    Point3 pt;
    UVVert tv;
	Point3 clr;
	Point3 n;
	XVectorB bonesWeights;
	XVectorB bonesIDs;
};

struct Face3ds {
    int vNum[3];
    int flags;
};

struct XMESHFlags {
	char exportTX;
    char exportT;
    char exportB;
    char exportN;
    char exportC;
	char generateA;
	char autoFillTXC;
	char exportAnimation;
	char exportObjSpaceN;
	char exportSkin;
	std::wstring nameMesh;
};


class VertexWeight
{
public:

	VertexWeight() : m_Weight(0), m_BoneID(-1) {}
	VertexWeight( float w, int b ) : m_Weight(w), m_BoneID(b) {}

	void SetWeight( float f )
	{
		m_Weight = f;
	}
	void SetBoneID( int i )
	{
		m_BoneID = i;
	}

	float GetWeight( void ) const
	{
		return m_Weight;
	}
	int GetBoneID( void ) const
	{
		return m_BoneID;
	}

	VertexWeight& operator = ( const VertexWeight& other )
	{
		m_Weight = other.m_Weight;
		m_BoneID = other.m_BoneID;
		return (*this);
	}

protected:

	float	m_Weight;
	int		m_BoneID;
};


class VertexMorph
{
public:

	VertexMorph() : delta(0,0,0) {}
	VertexMorph( Point3 _delta ) : delta(_delta.x,_delta.y,_delta.z) {}

	void setdelta(  Point3 _delta )
	{
		delta.x = _delta.x;
		delta.y = _delta.y;
		delta.z = _delta.z;
	}
	
	Point3 getdelta( void ) const
	{
		return delta;
	}
	

	bool operator == ( const VertexMorph& other ) const
	{
		return ( delta.x == other.delta.x && delta.y == other.delta.y && delta.z == other.delta.z);
	}

	VertexMorph& operator = ( const VertexMorph& other )
	{
		delta.x = other.delta.x;
		delta.y = other.delta.y;
		delta.z = other.delta.z;
		return (*this);
	}

protected:
	Point3 delta;
};



class MeshVertex
{
public:

	MeshVertex() {}

	MeshVertex( float x, float y, float z )
	{
		m_Pos		= Point3( x, y, z );
		m_Normal	= Point3( 0.0f, 0.0f, 0.0f );
		m_TexCoord	= Point2( 0.0f, 0.0f );
		m_ID		= -1;
		m_Morphed   = false;
	}


	void setPos(float x, float y, float z) {
		m_Pos = Point3(x, y, z);
	}

	void SetNormal( float x, float y, float z )
	{
		m_Normal = Point3( x, y, z );
	}


	void addMorphing(const VertexMorph & w) {
		m_Morph.push_back(w);
	}

	std::vector<VertexMorph>& getMorphing() {
		return m_Morph;
	}


	void setMorphed(bool f) {
		m_Morphed = f;
	}
	bool getMorphed() {
		return m_Morphed;
	}

	void setColor(float x, float y, float z) {
		m_color = Point3( x, y, z );
	}
	void SetTexCoord( float u, float v )
	{
		m_TexCoord = Point2( u, v );
	}
	void SetID( int id )
	{
		m_ID = id;
	}
	void AddWeight( const VertexWeight& w )
	{
		if (m_Weights.size() >=4 ) {
			for (int i = 0; i < m_Weights.size();i++) {
				VertexWeight w_m = m_Weights[i];
				m_Weights[i].SetWeight(m_Weights[i].GetWeight()+(w.GetWeight()/4.0));
			}
		}else {
			m_Weights.push_back( w );
		}
		
	}

	const Point3& GetColor( void ) const
	{
		return m_color;
	}
	const Point3& GetPosition( void ) const
	{
		return m_Pos;
	}
	const Point3& GetNormal( void ) const
	{
		return m_Normal;
	}
	const Point2& GetTexCoord( void ) const
	{
		return m_TexCoord;
	}
	int GetID( void ) const
	{
		return m_ID;
	}

	const int GetNumWeights( void ) const
	{
		return (int)m_Weights.size();
	}

	const VertexWeight& GetWeight( int i ) const
	{
		return m_Weights[ i ];
	}

	
	MeshVertex& operator = ( const MeshVertex& other )
	{
		m_Pos		= other.m_Pos;
		m_Normal	= other.m_Normal;
		m_TexCoord	= other.m_TexCoord;
		m_ID		= other.m_ID;
		m_Weights	= other.m_Weights;
		m_color     = other.m_color;
		m_Morphed   = other.m_Morphed;
		m_Morph     = other.m_Morph;
		return (*this);
	}

private:

	typedef	std::vector<VertexWeight>	weightsVec;

	Point3          m_color;
	Point3			m_Pos;
	Point3			m_Normal;
	Point2			m_TexCoord;
	int				m_ID;
	weightsVec		m_Weights;
	bool            m_Morphed;

	std::vector<VertexMorph> m_Morph;
};




class MeshVertexList
{
public:

	int Add( const MeshVertex& v )
	{
		int idx = 0;
		vertexVec::const_iterator it;
		for ( it = m_VertexList.begin(); it != m_VertexList.end(); ++it, ++idx )
		{
			//if ( *it == v )
			//	break;
		}

		if ( it == m_VertexList.end() )
			m_VertexList.push_back( v );

		return idx;
	}
	int Count( void ) const
	{
		return (int)m_VertexList.size();
	}
	MeshVertex& operator [] ( int i )
	{
		return m_VertexList[ i ];
	}

	void Clear( void )
	{
		m_VertexList.clear();
	}

private:

	typedef std::vector<MeshVertex>		vertexVec;

	vertexVec		m_VertexList;
};




extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;
