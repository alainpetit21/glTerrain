#ifndef _MATHFP_H_
#define _MATHFP_H_

#include <windows.h>
#include "interfaces.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#define FPP 10
#define IFP(x) ((x)<<FPP)
#define FP_ONE IFP(1)
#define FPMUL(x,y) (((x)*(y))>>FPP)
#define FPDIV(x,y) (((x)<<FPP)/(y))
#define FP(x) ((int)((x)*FP_ONE+0.5f))
#define PI 3.141559f

extern void InitTables();
extern int Sin[256];
extern int Cos[256];

#undef sin
#undef cos
#define sin(x) Sin[x]
#define cos(x) Cos[x]

class CMatrix
{
public:
	explicit					CMatrix()
											{Identity();};
					void			Identity();
					void			Rotatex(int a_X);
					void			Rotatey(int a_Y);
					void			Rotatez(int a_Z);
					void			Translate(int a_X, int a_Y, int a_Z);
					void			Concat(const CMatrix& a_M2);
					void			Rotate(int a_RX, int a_RY, int a_RZ);
					int*			Getx(int*	p_vec);
					int*			Gety(int*	p_vec);
					int*			Getz(int*	p_vec);
	union{
		int m[4][4];
		int l[16];
		struct 	{
			int m11,m12,m13,m14
				, m21,m22,m23,m24
				, m31,m32,m33,m34
				, m41,m42,m43,m44;
		};
	};
};

class CGlMatrix
{
public:
	explicit	CGlMatrix()
						{Identity();};
	void			Identity()
						{glLoadIdentity();};
	void			Translate(int a_X, int a_Y, int a_Z)
						{glTranslatef(((float)a_X)/1024, ((float)a_Y)/1024, ((float)a_Z)/1024);};
	void			Rotate(int a_RX, int a_RY, int a_RZ)
						{
							glRotatef((float)(a_RX*360/256),1.0f,0.0f,0.0f);
							glRotatef((float)(a_RY*360/256),0.0f,1.0f,0.0f);
							glRotatef((float)(a_RZ*360/256),0.0f,0.0f,1.0f);
						};
};

#endif	// _B_MATHFP_H_