#ifndef _3D_ENGINE_H_
#define _3D_ENGINE_H_

#include "interfaces.h"

class CVertexBuffer:	public IVertexBuffer
{
public:
	explicit	CVertexBuffer(){};

	void			Release(){delete this;};
	VERTEX*		Lock()
						{return m_start;};
	void			Unlock(){};

	VERTEX*	m_start;
	int			m_nb;
};

class CVertexBuffer2:	public IVertexBuffer2
{
public:
	explicit		CVertexBuffer2(){};

	void				Release(){delete this;};
	VERTEX2*		Lock()
							{return m_start;};
	void				Unlock(){};

	VERTEX2*	m_start;
	int				m_nb;
};

class CVertexBuffer3:	public IVertexBuffer3
{
public:
	explicit		CVertexBuffer3(){};

	void				Release(){delete this;};
	VERTEX3*		Lock()
							{return m_start;};
	void				Unlock(){};

	VERTEX3*	m_start;
	int				m_nb;
};

class CIndexBuffer2:	public IIndexBuffer2
{
public:
	explicit		CIndexBuffer2(){};

	void				Release(){delete this;};
	TRIANGLES2*	Lock()
							{return m_start;};
	void				Unlock(){};

	TRIANGLES2*	m_start;
	int					m_nb;
};

class CIndexBuffer3:	public IIndexBuffer3
{
public:
	explicit		CIndexBuffer3(){};

	void				Release(){delete this;};
	TRIANGLES3*	Lock()
							{return m_start;};
	void				Unlock(){};

	TRIANGLES3*	m_start;
	int					m_nb;
};

class CTextureCoordonateBuffer2:	public ITextureCoordonateBuffer2
{
public:
	explicit		CTextureCoordonateBuffer2(){};

	void				Release(){delete this;};
	TEXCOORD2*	Lock()
							{return m_start;};
	void				Unlock(){};

	TEXCOORD2*	m_start;
	int					m_nb;
};

class CTextureCoordonateBuffer3:	public ITextureCoordonateBuffer3
{
public:
	explicit		CTextureCoordonateBuffer3(){};

	void				Release(){delete this;};
	TEXCOORD3*	Lock()
							{return m_start;};
	void				Unlock(){};

	TEXCOORD3*	m_start;
	int					m_nb;
};

class CMesh3DS	:	public IMeshStatic
{
public:
	explicit				CMesh3DS	(){};

	void						Release(){delete this;};
	IVertexBuffer*	GetVB			()
									{return (IVertexBuffer*)&m_vb;};
	void						SetVB			(IVertexBuffer* p_vb)
									{
										m_vb.m_start= ((CVertexBuffer*)p_vb)->m_start;
										m_vb.m_nb		= ((CVertexBuffer*)p_vb)->m_nb;
									};
	void						SetTexture(TEXTURE*	p_tex)
									{m_tex= p_tex;};
	TEXTURE*				GetTexture()
									{return m_tex;};

	CVertexBuffer		m_vb;
	TEXTURE*				m_tex;	
};

class CMeshMD2	:	public IMeshAnimated
{
public:
	explicit				CMeshMD2():
									m_tex(0),m_nNbFrame(0){};

	void						Release(){delete this;};
	IVertexBuffer2*	GetVB			()
									{return (IVertexBuffer2*)&m_vb;};
	void						SetVB			(IVertexBuffer2* p_vb)
									{
										m_vb.m_start= ((CVertexBuffer2*)p_vb)->m_start;
										m_vb.m_nb		= ((CVertexBuffer2*)p_vb)->m_nb;
									};
	IIndexBuffer2*	GetIB			()
									{return (IIndexBuffer2*)&m_ib;};
	void						SetIB			(IIndexBuffer2* p_ib)
									{
										m_ib.m_start= ((CIndexBuffer2*)p_ib)->m_start;
										m_ib.m_nb		= ((CIndexBuffer2*)p_ib)->m_nb;
									};
	void						SetTexture(TEXTURE*	p_tex)
									{m_tex= p_tex;};
	TEXTURE*				GetTexture()
									{return m_tex;};
	void						SetFPS(int p_nFps)
									{m_nFps= p_nFps;}

	CVertexBuffer2	m_vb;
	CIndexBuffer2		m_ib;
	TEXTURE*				m_tex;
	int							m_cptFrame;
	int							m_nNbFrame;
	int							m_nFps;
};

#endif // _3D_ENGINE_H_