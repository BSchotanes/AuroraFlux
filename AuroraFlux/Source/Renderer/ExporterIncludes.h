#ifndef	CEXPORTERINCLUDES
#define CEXPORTERINCLUDES

#include "../StdAfx.h"
// Structure for vertexes with multiple ways to access them.
struct tVertex
{
	union
	{
		float fPos[3];
		struct
		{
			float m_fX, m_fY, m_fZ;
		};
	};
	union
	{
		float fNormal[3];
		struct  
		{
			float m_fNormX,m_fNormY,m_fNormZ;
		};
	};
	union
	{
		float fBinormal[3];
		struct  
		{
			float m_fBNormX,m_fBNormY,m_fBNormZ;
		};
	};
	union
	{
		float fTangent[3];
		struct  
		{
			float m_fTX,m_fTY,m_fTZ;
		};
	};
	union
	{
		float fTexCoord[2];
		struct  
		{
			float m_fU,m_fV;
		};
	};
	bool operator ==(tVertex v);
};

// Structure for triangles 
struct tTriangle
{
	union
	{
		unsigned int uIndices[3];
		struct 
		{
			unsigned int m_uIndex0, m_uIndex1, m_uIndex2;
		};
	};
};

class CMesh
{
public:
	std::string m_strName; 		// This is the name of the mesh.
	std::vector<std::string> m_vTextureNames;	// These are the textures that are used in this mesh.
	std::vector<tVertex>  m_vUniqueVerts;	// These are all the unique vertices in this mesh.
	std::vector<tTriangle> m_vTriangles;		// These are the triangles that make up the mesh.    

};

#endif