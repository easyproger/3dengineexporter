#include "Skeleton.h"
#include <algorithm>
#include <cctype>

static std::string FixBoneName( const std::string& boneName )
{
	std::string fixedName = boneName;
	if ( !fixedName.empty() )
	{
		std::replace( fixedName.begin(), fixedName.end(), ' ', '_' );
		std::transform( fixedName.begin(), fixedName.end(), fixedName.begin(), tolower );
	}
	return fixedName;
}


Bone::Bone()
	: m_MaxNode(NULL)
	, m_MaxBoneID(-1)
	, m_ID(-1)
	, m_ParentID(-1)
	, m_Parent(NULL)
	, m_Next(NULL)
	, m_Child(NULL)
{
}
Bone::~Bone()
{
}




void Bone::SetMaxNode( IGameNode* maxNode )
{
	m_MaxNode = maxNode;
}

void Bone::SetMaxBoneID( int ID )
{
	m_MaxBoneID = ID;
}

void Bone::SetName( const std::string& name )
{
	m_Name = FixBoneName( name );
}

void Bone::SetID( int ID )
{
	m_ID = ID;
}

void Bone::SetParentID( int parentID )
{
	m_ParentID = parentID;
}


IGameNode* Bone::GetMaxNode( void ) const
{
	return m_MaxNode;
}

int Bone::GetMaxBoneID( void ) const
{
	return m_MaxBoneID;
}

const std::string& Bone::GetName( void ) const
{
	return m_Name;
}

int Bone::GetID( void ) const
{
	return m_ID;
}

int Bone::GetParentID( void ) const
{
	return m_ParentID;
}

void Bone::LinkChildBone( Bone* childBone )
{
	Bone* oldChild = m_Child;
	m_Child = childBone;
	m_Child->m_Next = oldChild;
	m_Child->m_Parent = this;

	m_Child->SetParentID( this->GetID() );
}

Bone* Bone::GetParent( void ) const
{
	return m_Parent;
}

Bone* Bone::GetChild( void ) const
{
	return m_Child;
}

Bone* Bone::GetNext( void ) const
{
	return m_Next;
}






int Skeleton::Build( IGameObject* obj )
{
	IGameSkin* skin = obj->GetIGameSkin();
	if ( !skin )
		return FALSE;
	if ( IGameSkin::IGAME_SKIN != skin->GetSkinType() )
		return FALSE;

	int i, j, type, bone_id;
	const int numSkinnedVerts = skin->GetNumOfSkinnedVerts();

	for ( i = 0; i < numSkinnedVerts; ++i )
	{
		try {
			
			type = skin->GetVertexType( i );

			if ( IGameSkin::IGAME_RIGID == type )
			{
				bone_id = skin->GetBoneID( i, 0 );
				if (bone_id == -1) continue; // not binded vertex
				this->AddMaxBone( skin->GetIGameBone( i, 0 ), bone_id );
			}
			else	// blended
			{
				for ( j = 0; j < skin->GetNumberOfBones( i ); ++j )
				{
					bone_id = skin->GetBoneID( i, j );
					if (bone_id == -1) continue; // not binded vertex
					this->AddMaxBone( skin->GetIGameBone( i, j ), bone_id );
				}
			}
		}catch(...) {
			printf("hi here");
		}
		
	}

	this->LinkBones();

	return TRUE;
}

int Skeleton::GetNumBones( void ) const
{
	return (int)m_Bones.size();
}

Bone* Skeleton::GetRawBone( int i ) const
{
	return m_Bones[i];
}

Bone* Skeleton::GetRootBone( void ) const
{
	return m_RootBone;
}

int Skeleton::GetBoneIDByMaxID( int maxBoneID ) const
{
	Bone* bone = this->FindBoneByMaxID( maxBoneID );
	if ( !bone )
		return -1;
	return bone->GetID();
}

void Skeleton::WriteToStream( std::stringstream& ss )
{
	ss << this->GetNumBones() << std::endl;
	this->WriteBone( ss, this->GetRootBone() );
}


#include "Utils.h"

void Skeleton::AddMaxBone( IGameNode* boneNode, int maxBoneID )
{
	if ( NULL != this->FindBoneByMaxID( maxBoneID ) )
		return;

	Bone* newBone = new Bone();
	newBone->SetMaxNode( boneNode );
	newBone->SetMaxBoneID( maxBoneID );
	newBone->SetName(ToStdStr( boneNode->GetName() ));
	newBone->SetID( this->GetNumBones() );

	m_Bones.push_back( newBone );
}

Bone* Skeleton::FindBoneByMaxID( int maxBoneID ) const
{
	for ( bonesVec::const_iterator it = m_Bones.begin(); it != m_Bones.end(); ++it )
	{
		if ( (*it)->GetMaxBoneID() == maxBoneID )
			return (*it);
	}

	return NULL;
}

void Skeleton::LinkBones( void )
{
	for ( bonesVec::const_iterator it = m_Bones.begin(); it != m_Bones.end(); ++it )
		this->LinkBone( *it );
}

void Skeleton::LinkBone( Bone* bone )
{
	IGameNode* node = bone->GetMaxNode();
	IGameNode* parent = node->GetNodeParent();
	Bone* parentBone = NULL;

	if ( parent )
		parentBone = this->FindBoneByMaxID( parent->GetNodeID() );

	if ( !parentBone )
		m_RootBone = bone;
	else
		parentBone->LinkChildBone( bone );
}

void Skeleton::WriteBone( std::stringstream& ss, Bone* bone )
{
	ss << bone->GetName() << " " << bone->GetID() << " " << bone->GetParentID() << std::endl;

	bone = bone->GetChild();
	while ( bone )
	{
		this->WriteBone( ss, bone );
		bone = bone->GetNext();
	}
}

