#include "Utils.h"


Utils::Utils(void)
{
}


Utils::~Utils(void)
{
}
Matrix3 Transpose(Matrix3& matrix)
{
  float transpose[4][3];

  int row;
  for(row = 0; row < 3; row++)
  {
    int column;
    for(column = 0; column < 3; column++)
    {
      transpose[column][row] = matrix.GetAddr()[row][column];
    }
  }

  int column;
  for(column = 0; column < 3; column++)
  {
    transpose[3][column] = 0;
  }

  Matrix3 transposeMatrix(transpose);
  transposeMatrix.NoTrans();

  return transposeMatrix;
}
Matrix3 GetNodeTM(INode *iNode, float time)
{
  // initialize matrix with the identity
  Matrix3 tm, tmParent;
  tm.IdentityMatrix();

  // only do this for valid nodes
  if(iNode != 0)
  {
    bool isMirrored = false;
    // get the node transformation
	
	
    tm = iNode->GetNodeTM(time*GetTicksPerFrame());
    tm.NoScale();
    
    AffineParts parts;
    decomp_affine(tm, &parts);
    parts.q.MakeMatrix(tm);

    tm.SetRow(3, parts.t);
  }
  return tm;
}


Matrix3 ConvertMax2Ogl(const Matrix3& _mat)
{
  //return _mat;

  //T = R x _mat x R-1
  Matrix3 tm,tmogl,tmoglInv;
  tmogl.IdentityMatrix();
  tmogl.RotateX(PI*0.5f);
  tmoglInv=tmogl;
  tmoglInv.Invert();
  tm=tmogl*_mat*tmoglInv;
  return tm;
}

void GetTranslationAndRotation(INode *iNode, INode *iParentNode, float time, XVector3& translation, XVector4& rotation)
{
  // check for invalid nodes
  if(iNode == 0) return;

  // the initial node pose (time == -1.0) is at (time == 0.0) for 3ds max
  if(time < 0.0f) time = 0.0f;

  // downcast the node to a max node

  // calculate the relative transformation
  Matrix3 tm;
  tm = GetNodeTM(iNode, time) * Inverse(GetNodeTM(iParentNode, time));
  tm = ConvertMax2Ogl(tm);

  // calculate the translation component
  Point3 p;
  p = tm.GetTrans();

  translation.v[0] = p[0];
  translation.v[1] = p[1];
  translation.v[2] = p[2];

  // calculate the rotation component
  Quat q(tm);

  rotation.v[0] = q[0];
  rotation.v[1] = q[1];
  rotation.v[2] = q[2];
  rotation.v[3] = q[3];
}



void GetTranslationAndRotationInverseWorldSpace(INode *iNode, float time,bool inverse, XVector3& translation, XVector4& rotation)
{
  // check for invalid nodes
  if(iNode == 0) return;

  // the initial node pose (time == -1.0) is at (time == 0.0) for 3ds max
  if(time < 0.0f) time = 0.0f;


  // calculate the inverse transformation
  Matrix3 tm,tm2,tm3;
  tm = inverse?Inverse(GetNodeTM(iNode, time)):GetNodeTM(iNode, time);
  tm = ConvertMax2Ogl(tm);

  Point3 p;
  p = tm.GetTrans();
  translation.v[0] = p[0];
  translation.v[1] = p[1];
  translation.v[2] = p[2];

  // calculate the rotation component
  Quat q(tm);

  rotation.v[0] = q[0];
  rotation.v[1] = q[1];
  rotation.v[2] = q[2];
  rotation.v[3] = q[3];
}





     
Point3 VertexTransform( Point3 p, XVector4 quat,XVector3 pos) {
 
 
 
      float x = quat.v[0];
      float y = quat.v[1];
      float z = quat.v[2];
      float w = quat.v[3];
     
     
      float tx = pos.v[0];
      float ty = pos.v[1];
      float tz = pos.v[2];
     
      float xxzz = x*x - z*z;
      float wwyy = w*w - y*y;
      float xw2 = x*w*2.0;
      float xy2 = x*y*2.0;
      float xz2 = x*z*2.0;
      float yw2 = y*w*2.0;
      float yz2 = y*z*2.0;
      float zw2 = z*w*2.0;

	  float vec1 = (xxzz + wwyy)*p.x + (xy2 + zw2)*p.y       + (xz2 - yw2)*p.z;
	  float vec2 = (xy2 - zw2)*p.x   + (y*y+w*w-x*x-z*z)*p.y + (yz2 + xw2)*p.z;
	  float vec3 = (xz2 + yw2)*p.x   + (yz2 - xw2)*p.y       + (wwyy - xxzz)*p.z;

      Point3 ret;
	  ret.x = vec1+tx;
	  ret.y = vec2+ty;
	  ret.z = vec3+tz;
     
     return ret;
}
     



std::string ToStdStr(const TSTR& str)
{
	std::string result;

	int length = str.Length();
	if(length > 0)
	{
		char* buffer = new char[length+1];
		buffer[length] = 0;
		for(int i = 0; i < length; ++i)
		{
			buffer[i] = char(str[i]);
		}
		result = buffer;
		delete [] buffer;
	}

	return result;
}

TSTR ToTStr(const std::string& str)
{
	TSTR result;

	int length = str.length();
	if(length > 0)
	{
		TCHAR* buffer = new TCHAR[length+1];
		buffer[length] = 0;
		for(int i = 0; i < length; ++i)
		{
			buffer[i] = TCHAR(str[i]);
		}
		result = buffer;
		delete [] buffer;
	}

	return result;
}

TSTR ToTStr(const unsigned char* str, size_t length)
{
	TSTR result;

	if(length > 0)
	{
		TCHAR* buffer = new TCHAR[length+1];
		buffer[length] = 0;
		for(size_t i = 0; i < length; ++i)
		{
			buffer[i] = TCHAR(str[i]);
		}
		result = buffer;
		delete [] buffer;
	}

	return result;
}