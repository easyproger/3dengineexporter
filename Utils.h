#pragma once
#include <iostream>
#include <ostream>
#include <sstream>
#include <cstring>
#include "strclass.h"
#include "Max.h"
#include "decomp.h"
#include "IGame.h"
#include "IGameModifier.h"
#include "iskin.h"


using namespace std;

// String Converter Functions


struct XVector3 {
	float v[3];
};
struct XVector4 {
	float v[4];
};

class Utils
{
public:
	Utils(void);
	~Utils(void);
};

Matrix3 Transpose(Matrix3& matrix);
Point3 VertexTransform( Point3 p, XVector4 quat,XVector3 pos);
Matrix3 GetNodeTM(INode *iNode, float time);
Matrix3 ConvertMax2Ogl(const Matrix3& _mat);
void GetTranslationAndRotationInverseWorldSpace(INode *iNode, float time,bool  inverse, XVector3& translation, XVector4& rotation);
void GetTranslationAndRotation(INode *iNode, INode *iParentNode, float time, XVector3& translation, XVector4& rotation);
std::string ToStdStr(const TSTR& str);
TSTR ToTStr(const std::string& str);
TSTR ToTStr(const unsigned char* str, size_t length);
