#ifndef _3D_TYPES_H_
#define _3D_TYPES_H_

typedef struct tagVERTEX
{
	int		orig[3]								//Origin	//3D Point Before Transformation calc
		,		rot[3]								//Rotated //3D Point After Transformation	calc
		,		nor[3]								//Normal Orientation Vector
		,		sX										//Screen Coordonate //2D After Perspective calc
		,		sY										//Screen Coordonate //2D After Perspective calc
		,		m_U1,	m_V1						//Texture Coordonate 
		,		m_R, m_G,	m_B;				//Material color
}VERTEX;

typedef struct tagVERTEX2
{
	int		orig[3]								//Origin	//3D Point Before Transformation calc
		,		rot[3]								//Origin	//3D Point Before Transformation calc
		,		nor[3];								//Normal Orientation Vector
}VERTEX2;

typedef struct tagVERTEX3
{
	float	orig[3];							//Origin	//3D Point Before Transformation calc
}VERTEX3;

typedef struct tagTEXCOORD2
{
	int		m_U1,	m_V1;						//Texture Coordonate 
}TEXCOORD2;

typedef struct tagTEXCOORD3
{
	float		m_U1,	m_V1;						//Texture Coordonate 
}TEXCOORD3;

typedef struct tagTRIANGLES2
{
	int idxVertex[3]						//Index Coordonate
		, idxTexCoor[3];					//Texture Coordonate  
}TRIANGLES2;

typedef struct tagTRIANGLES3
{
	unsigned int idxVertex[3];	//Index Coordonate
}TRIANGLES3;

#endif _3D_TYPES_H_