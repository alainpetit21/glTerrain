#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_

#include <windows.h>
#include "interfaces.h"
#include "3DEngine.h"

class CGlRenderer : public IRenderer  
{
public:
													CGlRenderer			();
	virtual									~CGlRenderer		();
	//Interface IUnknow
	void						AddRef					(){m_nRefCount++;};
	void						Release					(){if(!(--m_nRefCount)) delete this;};
	//Interface IRenderer
	//Init GL
	int							Init						(	unsigned long hinstance, 
																		unsigned int p_nSX, unsigned int p_nSY, 
																		unsigned int p_bits, unsigned int p_fs);
	int							Kill						();
	int							Resize					(unsigned int w, unsigned int h);
	//Init 3DEngine
	IVertexBuffer*	CreateVB				(int nbVertex);
	IVertexBuffer2*	CreateVB2				(int nbVertex);
	IVertexBuffer3*	CreateVB3				(int nbVertex);
	IIndexBuffer2*	CreateIB2				(int nbIndex);
	IIndexBuffer3*	CreateIB3				(int nbIndex);
	ITextureCoordonateBuffer2*		
									CreateTB2				(int nbTC);
	ITextureCoordonateBuffer3*		
									CreateTB3				(int nbTC);
	IMeshStatic*		CreateMesh3DS		(char* pInitFile);
	IMeshAnimated*	CreateMeshMD2		(char* pInitFile);
	TEXTURE*				CreateTexture		(char* pInitFile, int option);
	void						BufferPoly1			(int pnbV);
	void						BufferPoly2			(int pnbV2, int pnbT2, int pnbI2);
	void						BufferPoly3			(int pnbV3, int pnbT3, int pnbI3);
	void						SetRendererStage(int wichOne)
									{							
										switch(wichOne){
										case RST_POINT:	Drawer= D_Point;Drawer2= D2_Point;Drawer3= D3_Point;break;
										case RST_LINES:	Drawer= D_Lines;Drawer2= D2_Lines;Drawer3= D3_Lines;break;
										case RST_FLAT:	Drawer= D_Flat;	Drawer2= D2_Flat;	Drawer3= D3_Flat;	break;
										case RST_TEXT:	Drawer= D_Text;	Drawer2= D2_Text;	Drawer3= D3_Text;	break;
										}
									};
	void						SetAnimatorStage(int wichOne)
									{							
										switch(wichOne){
										case ANI_NEAREST:	process= PI_Nearest;	break;
										case ANI_LINEAR:	process= PI_Linear;	break;
										case ANI_COSINE:	process= PI_Cosine;	break;
										case ANI_CUBIC:		process= PI_Cubic;	break;
										}
									};
	void						SetEraseColor		(unsigned char red, unsigned char green, unsigned char blue)
									{m_red= red; m_green= green; m_blue= blue;};
	void						SetEraseBuffer	(TEXTURE* pBitmap){}
	//Run-Time 3DEngine
	void						Clear						(int param);
	void						ProcessVertex		(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb){};
	void						ProcessVB				(CMatrix& p_mat, IVertexBuffer* p_vb){};
	void						ProcessMesh			(CMatrix& p_mat, IMeshStatic* p_mesh){};
	void						ProcessMesh			(CMatrix& p_mat, IMeshAnimated* p_mesh);
	void						DrawPrimitive		(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb){};
	void						DrawPrimitiveIB	(IIndexBuffer2* p_ib, IVertexBuffer2* p_vb, TEXTURE* p_tex);
	void						DrawPrimitiveIB	(IIndexBuffer3* p_ib, IVertexBuffer3* p_vb, TEXTURE* p_tex);
	void						DrawPrimitiveIB	(IIndexBuffer3* p_ib, IVertexBuffer3* p_vb, ITextureCoordonateBuffer3* p_tb, TEXTURE* p_tex);
	void						DrawPrimitiveVB	(IVertexBuffer* p_vb, TEXTURE* p_tex){};
	void						DrawMesh				(IMeshStatic* p_mesh){};
	void						DrawMesh				(IMeshAnimated* p_mesh);
	void						Flip						();

	//Internal Fonctions
	int							SetDCPixelFormat(HDC pHDC);
	void						PolyCalcPlane		(VERTEX* p_tri);
	long						eatchunk				(HANDLE file, CVertexBuffer* p_vbToFill);
	void						PI_Nearest			(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter);
	void						PI_Linear				(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter);
	void						PI_Cosine				(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter);
	void						PI_Cubic				(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter);
	void						D_Point					(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb);
	void						D_Lines					(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb);
	void						D_Flat					(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb);
	void						D_Text					(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb);
	void						D2_Point				(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);
	void						D2_Lines				(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);
	void						D2_Flat					(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);
	void						D2_Text					(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);
	void						D3_Point				(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);
	void						D3_Lines				(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);
	void						D3_Flat					(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);
	void						D3_Text					(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI);

	//PtFonctions
	void				(CGlRenderer::*Drawer)	(VERTEX*, TEXTURE*, unsigned int);
	void				(CGlRenderer::*Drawer2) (VERTEX2*, TRIANGLES2*, TEXTURE*, unsigned int, unsigned int);
	void				(CGlRenderer::*Drawer3) (VERTEX3*, TRIANGLES3*, TEXCOORD3* p_tb, TEXTURE*, unsigned int, unsigned int);
	void				(CGlRenderer::*process)	(CMatrix&, VERTEX*, unsigned int, int);

	//GL System				
	unsigned int	m_nRefCount
						,		m_nSX
						,		m_nSY
						,		m_nBpp;
	HWND					m_hWnd;
	HDC						m_hDC;
	HGLRC					m_hRC;
	HINSTANCE			m_hInstance;
	BOOL					m_fs;
	unsigned char	m_red
							,	m_green
							,	m_blue;
	//3D System
	TEXTURE*			m_Dest;
	VERTEX*				m_bufV;
	VERTEX2*			m_bufV2;
	VERTEX3*			m_bufV3;
	TEXCOORD2*		m_bufT2;
	TEXCOORD3*		m_bufT3;
	TRIANGLES2*		m_bufI2;
	TRIANGLES3*		m_bufI3;
	unsigned int	m_cptV;
	unsigned int	m_cptMaxV;
	unsigned int	m_cptV3;
	unsigned int	m_cptMaxV3;
	unsigned int	m_cptV2;
	unsigned int	m_cptMaxV2;
	unsigned int	m_cptT2;
	unsigned int	m_cptMaxT2;
	unsigned int	m_cptT3;
	unsigned int	m_cptMaxT3;
	unsigned int	m_cptI2;
	unsigned int	m_cptMaxI2;
	unsigned int	m_cptI3;
	unsigned int	m_cptMaxI3;
	int *x, *y, *z, nb;
};

#endif // _GL_RENDERER_H_
