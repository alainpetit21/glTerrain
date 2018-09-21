#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "3DTypes.h"
#include "2DTypes.h"

#define MOD_GL_RENDERER		1
#define MOD_DX_RENDERER		2
#define MOD_TERRAIN				3

class CMatrix;
class __declspec(novtable) IIndexBuffer2
{
public:
	virtual void				Release()	=0;
	virtual TRIANGLES2*	Lock()		=0;
	virtual void				Unlock()	=0;
};

class __declspec(novtable) IIndexBuffer3
{
public:
	virtual void				Release()	=0;
	virtual TRIANGLES3*	Lock()		=0;
	virtual void				Unlock()	=0;
};

class __declspec(novtable) IVertexBuffer
{
public:
	virtual void			Release()	=0;
	virtual VERTEX*		Lock()		=0;
	virtual void			Unlock()	=0;
};

class __declspec(novtable) IVertexBuffer2
{
public:
	virtual void			Release()	=0;
	virtual VERTEX2*	Lock()		=0;
	virtual void			Unlock()	=0;
};

class __declspec(novtable) IVertexBuffer3
{
public:
	virtual void			Release()	=0;
	virtual VERTEX3*	Lock()		=0;
	virtual void			Unlock()	=0;
};

class __declspec(novtable) ITextureCoordonateBuffer2
{
public:
	virtual void				Release()	=0;
	virtual TEXCOORD2*	Lock()		=0;
	virtual void				Unlock()	=0;
};

class __declspec(novtable) ITextureCoordonateBuffer3
{
public:
	virtual void				Release()	=0;
	virtual TEXCOORD3*	Lock()		=0;
	virtual void				Unlock()	=0;
};

class __declspec(novtable) IMeshStatic
{
public:
	virtual void						Release()										=0;
	virtual IVertexBuffer*	GetVB()											=0;
	virtual void						SetVB(IVertexBuffer* p_vb)	=0;
	virtual void						SetTexture(TEXTURE*	p_tex)	=0;
	virtual TEXTURE*				GetTexture()								=0;
};

class __declspec(novtable) IMeshAnimated
{
public:
	virtual void						Release()										=0;
	virtual	IVertexBuffer2*	GetVB()											=0;
	virtual	void						SetVB(IVertexBuffer2* p_vb)	=0;
	virtual	IIndexBuffer2*	GetIB()											=0;
	virtual	void						SetIB(IIndexBuffer2* p_vb)	=0;
	virtual	void						SetTexture(TEXTURE*	p_tex)	=0;
	virtual	TEXTURE*				GetTexture()								=0;
	virtual	void						SetFPS(int p_nFps)					=0;
};

class __declspec(novtable) ITerrain
{
public:
	virtual void						AddRef		()										=0;
	virtual void						Release		()										=0;
	virtual	IVertexBuffer3*	GetVB			()										=0;
	virtual	void						SetVB			(IVertexBuffer3* p_vb)=0;
	virtual IIndexBuffer3*	GetIB			()										=0;
	virtual void						SetIB			(IIndexBuffer3* p_ib)	=0;
	virtual ITextureCoordonateBuffer3*	
													GetTB			()										=0;
	virtual void						SetTB			(ITextureCoordonateBuffer3* p_tb)
																													=0;
	virtual	void						SetTexture(TEXTURE*	p_tex)			=0;
	virtual	TEXTURE*				GetTexture()										=0;
	virtual	void						SetInitHM(char* pInitFile)			=0;
};

class __declspec(novtable) IRenderer
{
public:
	enum{
		CLR_ZERO	= 0x0,
		CLR_COLOR	= 0x1,
		CLR_BUFFER= 0x2,
		CLR_BLUR	= 0x4,
	};
	enum{
		RST_POINT		= 0x0,
		RST_LINES		= 0x1,
		RST_FLAT		= 0x2,
		RST_TEXT		= 0x3,
	};
	enum{
		ANI_NEAREST	= 0x0,
		ANI_LINEAR	= 0x1,
		ANI_COSINE	= 0x2,
		ANI_CUBIC		= 0x3,
	};
	enum{
		TEX_NEAREST	= 0x1,
		TEX_LINEAR	= 0x2,
		TEX_MIPMAP	= 0x4,
	};
	//IUnknow
	virtual void						AddRef()																									=0;
	virtual void						Release()																									=0;
	//Init																																					
	virtual int							Init						(unsigned long hinstance, unsigned int p_nScreenX, unsigned int p_nScreenY, unsigned int p_bits, unsigned int p_fs)
																																										=0;
	virtual int							Kill						()																				=0;
	virtual	int							Resize					(unsigned int w, unsigned int h)					=0;
	virtual IVertexBuffer*	CreateVB				(int nbVertex)														=0;
	virtual IVertexBuffer2*	CreateVB2				(int nbVertex)														=0;
	virtual IVertexBuffer3*	CreateVB3				(int nbVertex)														=0;
	virtual IIndexBuffer2*	CreateIB2				(int nbIndex)															=0;
	virtual IIndexBuffer3*	CreateIB3				(int nbIndex)															=0;
	virtual	ITextureCoordonateBuffer2*		
													CreateTB2(int nbTC)																				=0;
	virtual	ITextureCoordonateBuffer3*		
													CreateTB3(int nbTC)																				=0;
	virtual IMeshStatic*		CreateMesh3DS		(char* pInitFile)													=0;
	virtual IMeshAnimated*	CreateMeshMD2		(char* pInitFile)													=0;
	virtual TEXTURE*				CreateTexture		(char* pInitFile, int option)							=0;
	virtual	void						BufferPoly1			(int pnbV)																=0;
	virtual	void						BufferPoly2			(int pnbV2, int pnbT2, int pnbI2)					=0;
	virtual	void						BufferPoly3			(int pnbV3, int pnbT3, int pnbI3)					=0;
	virtual void						SetRendererStage(int wichOne)															=0;
	virtual void						SetAnimatorStage(int wichOne)															=0;
	virtual void						SetEraseColor		(unsigned char red, unsigned char green, unsigned char blue)														
																																										=0;
	virtual void						SetEraseBuffer	(TEXTURE* pBitmap)												=0;
	//Run-Time							
	virtual void						Clear						(int param)																									=0;
	virtual void						ProcessVertex		(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb)					=0;
	virtual void						ProcessVB				(CMatrix& p_mat, IVertexBuffer* p_vb)												=0;
	virtual void						ProcessMesh			(CMatrix& p_mat, IMeshStatic* p_mesh)												=0;
	virtual	void						ProcessMesh			(CMatrix& p_mat, IMeshAnimated* p_mesh)											=0;
	virtual void						DrawPrimitive		(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb)					=0;
	virtual	void						DrawPrimitiveIB	(IIndexBuffer2* p_ib, IVertexBuffer2* p_vb, TEXTURE* p_tex)	=0;
	virtual	void						DrawPrimitiveIB	(IIndexBuffer3* p_ib, IVertexBuffer3* p_vb, TEXTURE* p_tex)	=0;
	virtual	void						DrawPrimitiveIB	(IIndexBuffer3* p_ib, IVertexBuffer3* p_vb, 
																					ITextureCoordonateBuffer3* p_tb, TEXTURE* p_tex)						=0;
	virtual void						DrawPrimitiveVB	(IVertexBuffer* p_vb, TEXTURE* p_tex)												=0;
	virtual void						DrawMesh				(IMeshStatic* p_mesh)																				=0;
	virtual void						DrawMesh				(IMeshAnimated* p_mesh)																			=0;
	virtual void						Flip						()																													=0;
};

void* CreateInstance(int what);
#endif // _INTERFACES_H_