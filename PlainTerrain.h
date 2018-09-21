#ifndef _M_PLAIN_TERRAIN_H_
#define _M_PLAIN_TERRAIN_H_

#include "interfaces.h"
#include "3DEngine.h"

class CPlainTerrain  : ITerrain
{
public:
									CPlainTerrain			():	m_nRefCount(0){};
	virtual					~CPlainTerrain		(){};

	//Interface IUnknow
	void						AddRef		(){m_nRefCount++;};
	void						Release		(){if(!(--m_nRefCount)) delete this;};

	//Interface ITerrain
	IVertexBuffer3*	GetVB			()
									{return (IVertexBuffer3*)&m_vb;};
	void						SetVB			(IVertexBuffer3* p_vb)
									{
										m_vb.m_start= ((CVertexBuffer3*)p_vb)->m_start;
										m_vb.m_nb		= ((CVertexBuffer3*)p_vb)->m_nb;
									};
	IIndexBuffer3*	GetIB			()
									{return (IIndexBuffer3*)&m_ib;};
	void						SetIB			(IIndexBuffer3* p_ib)
									{
										m_ib.m_start= ((CIndexBuffer3*)p_ib)->m_start;
										m_ib.m_nb		= ((CIndexBuffer3*)p_ib)->m_nb;
									};
	ITextureCoordonateBuffer3*	
									GetTB			()
									{return (ITextureCoordonateBuffer3*)&m_tb;};
	void						SetTB			(ITextureCoordonateBuffer3* p_tb)
									{
										m_tb.m_start= ((CTextureCoordonateBuffer3*)p_tb)->m_start;
										m_tb.m_nb		= ((CTextureCoordonateBuffer3*)p_tb)->m_nb;
									};
	void						SetTexture(TEXTURE*	p_tex)
									{m_tex= p_tex;};
	TEXTURE*				GetTexture()
									{return m_tex;};
	void						SetInitHM	(char* pInitFile);

	unsigned int							m_nRefCount;
	CVertexBuffer3						m_vb;
	CIndexBuffer3							m_ib;
	CTextureCoordonateBuffer3	m_tb;
	TEXTURE*									m_tex;
};												

#endif // _M_PLAIN_TERRAIN_H_
