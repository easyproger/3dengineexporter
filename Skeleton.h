#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "IGame/IGame.h"
#include "IGame/IGameModifier.h"



class Bone
{
public:
	Bone();
	virtual ~Bone();

	void SetMaxNode( IGameNode* maxNode );
	void SetMaxBoneID( int ID );
	void SetName( const std::string& name );
	void SetID( int ID );
	void SetParentID( int parentID );

	IGameNode*         GetMaxNode( void ) const;
	int                GetMaxBoneID( void ) const;
	const std::string& GetName( void ) const;
	int                GetID( void ) const;
	int                GetParentID( void ) const;

	void  LinkChildBone( Bone* childBone );
	Bone* GetParent( void ) const;
	Bone* GetChild( void ) const;
	Bone* GetNext( void ) const;

protected:

	IGameNode*		m_MaxNode;
	int				m_MaxBoneID;
	std::string		m_Name;
	int				m_ID;
	int				m_ParentID;

	Bone*			m_Parent;	
	Bone*			m_Next;		
	Bone*			m_Child;	
};


class Skeleton
{
public:
	Skeleton() {}
	virtual ~Skeleton() {}

	int   Build( IGameObject* obj );
	int   GetNumBones( void ) const;
	Bone* GetRawBone( int i ) const;
	Bone* GetRootBone( void ) const;
	int   GetBoneIDByMaxID( int maxBoneID ) const;

	void WriteToStream( std::stringstream& ss );

	typedef std::vector<Bone*>	bonesVec;
	bonesVec		m_Bones;
	Bone*			m_RootBone;

protected:

	void  AddMaxBone( IGameNode* boneNode, int maxBoneID );
	Bone* FindBoneByMaxID( int maxBoneID ) const;
	void  LinkBones( void );
	void  LinkBone( Bone* bone );

	void WriteBone( std::stringstream& ss, Bone* bone );

	

	
};

#endif	// __SKELETON_H__

