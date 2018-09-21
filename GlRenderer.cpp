#include <io.h>
#include <math.h>
#include "GlRenderer.h"
#include "debug.h"
#include "mathFP.h"
#include "filehelper.h"

#include <GL/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "GL/glext.h"

#define GLH_EXT_SINGLE_FILE
#include "glh_genext.h"


extern LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam);

CGlRenderer::CGlRenderer():
m_nRefCount(0),
m_fs(0),
m_hInstance(GetModuleHandle(NULL)),
m_nSX(0),
m_nSY(0),
m_nBpp(0),
m_bufV(0),
m_bufV2(0),	m_bufI2(0),	m_bufT2(0),
m_bufV3(0),	m_bufI3(0),	m_bufT3(0)
{
	WNDCLASS	wc;

	wc.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc	= (WNDPROC) WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon				= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground= NULL;
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= "WIN3DCLASS";

	ASSERT_FATAL(RegisterClass(&wc), "Failed To Register The Window Class.")
}

CGlRenderer::~CGlRenderer()
{
	if(m_bufV)		{delete []m_bufV;m_bufV= 0;}
	if(m_bufV2)		{delete []m_bufV2;m_bufV2= 0;}
	if(m_bufI2)		{delete []m_bufI2;m_bufI2= 0;}
	if(m_bufI3)		{delete []m_bufI3;m_bufI3= 0;}
	if(m_bufT2)		{delete []m_bufT2;m_bufT2= 0;}
	if(m_bufV3)		{wglFreeMemoryNV(m_bufV3);m_bufV3= 0;}
	Kill();
}

int		
CGlRenderer::Init(	unsigned long hinstance, unsigned int p_nSX, unsigned int p_nSY, 
					unsigned int p_bits, unsigned int p_fs)
{
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;


	m_fs		= p_fs;
	m_nSX	= p_nSX;
	m_nSY	= p_nSY;
	m_nBpp		= p_bits;

	WindowRect.left		=(long)0;
	WindowRect.right	=(long)m_nSX;
	WindowRect.top		=(long)0;
	WindowRect.bottom	=(long)m_nSY;

	if(m_fs)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= m_nSX;
		dmScreenSettings.dmPelsHeight	= m_nSY;
		dmScreenSettings.dmBitsPerPel	= m_nBpp;
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			m_fs=FALSE;
	}
	if(m_fs)
	{
		dwExStyle	=WS_EX_APPWINDOW;
		dwStyle		=WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle	=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle		=WS_OVERLAPPEDWINDOW;
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	
	VERIFY_FATAL((m_hWnd= CreateWindow("WIN3DCLASS","MyApp",
					dwStyle|WS_CLIPSIBLINGS |WS_CLIPCHILDREN,
					0, 0, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top,
					NULL,	NULL,	m_hInstance,
					NULL)), "Window Creation Error");

	VERIFY_FATAL(m_hDC= GetDC(m_hWnd), "Can't Create A GL Device Context.");
	VERIFY_FATAL(SetDCPixelFormat(m_hDC), "Can't Find A Suitable PixelFormat.");
	VERIFY_FATAL(m_hRC=wglCreateContext(m_hDC), "Can't Create A GL Rendering Context.");
	VERIFY_FATAL(wglMakeCurrent(m_hDC,m_hRC), "Can't Activate The GL Rendering Context.");

	ShowWindow(m_hWnd,SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	Resize(m_nSX, m_nSY);

	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);		// Black Background
	glClearDepth(1.0f);											// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glCullFace (GL_BACK);

	float temp[]= { 0.75f, 0.75f, 0.75f, 1.0f }; 
	float temp2[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	float temp3[]= { 0.0f, 0.0f, 2.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, temp);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, temp2);
	glLightfv(GL_LIGHT1, GL_POSITION,temp3);
	glEnable(GL_LIGHT1);

	VERIFY_FATAL(glh_init_extension("GL_NV_vertex_array_range"), "Failed to initialize GL_NV_vertex_array_range");
	return 1;
}

int		
CGlRenderer::Kill()
{
	if (m_fs)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}
	if (m_hRC)
	{
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(m_hRC);
		m_hRC=NULL;
	}
	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))
		m_hDC=NULL;

	if (m_hWnd && !DestroyWindow(m_hWnd))
		m_hWnd=NULL;

	if (!UnregisterClass("Terrain",m_hInstance))
		m_hInstance=NULL;

	return 0;
}

int		
CGlRenderer::Resize(unsigned int w, unsigned int h)
{
	if (!h)	h=1;

	glViewport(0,0,m_nSX= w, m_nSY= h);
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,10000.0f);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
	return 0;
}

void
CGlRenderer::BufferPoly1(int pnbV)
{
	if(m_bufV)	{delete []m_bufV;m_bufV= 0;}

	if(m_cptMaxV= pnbV)		m_bufV	= new VERTEX[m_cptMaxV];

	m_cptV=0;
}

void
CGlRenderer::BufferPoly2(int pnbV2, int pnbT2, int pnbI2)
{
	if(m_bufV2)	{delete []m_bufV2;m_bufV2= 0;}
	if(m_bufI2)	{delete []m_bufI2;m_bufI2= 0;}
	if(m_bufT2)	{delete []m_bufT2;m_bufT2= 0;}

	if(m_cptMaxV2= pnbV2)	m_bufV2	= new VERTEX2[m_cptMaxV2];
	if(m_cptMaxI2= pnbI2)	m_bufI2	= new TRIANGLES2[m_cptMaxI2];
	if(m_cptMaxT2= pnbT2)	m_bufT2	= new TEXCOORD2[m_cptMaxT2];

	m_cptV2=0;
	m_cptT2=0;
	m_cptI2=0;
}

void
CGlRenderer::BufferPoly3(int pnbV3, int pnbT3, int pnbI3)
{
	if(m_bufI3)	{delete []m_bufI3;m_bufI3= 0;}
	if(m_bufT3)	{delete []m_bufT3;m_bufT3= 0;}
	if(m_bufV3)	{wglFreeMemoryNV(m_bufV3);m_bufV3= 0;}

	if(m_cptMaxV3= pnbV3)	m_bufV3	= (VERTEX3*)wglAllocateMemoryNV(sizeof(VERTEX3)*m_cptMaxV3, 0, 0, 1.0f);
	if(m_cptMaxI3= pnbI3)	m_bufI3	= new TRIANGLES3[m_cptMaxI3];
	if(m_cptMaxT3= pnbT3)	m_bufT3	= new TEXCOORD3[m_cptMaxT3];
	glVertexArrayRangeNV(sizeof(VERTEX3)*m_cptMaxV3, m_bufV3);
	
	m_cptV3=0;
	m_cptI3=0;
	m_cptT3=0;
}
IVertexBuffer*	
CGlRenderer::CreateVB(int nbVertex)
{
	CVertexBuffer*	vb= new CVertexBuffer();
	vb->m_start= m_bufV+m_cptV;
	vb->m_nb= nbVertex;
	m_cptV+= nbVertex;
	return vb;
};

IVertexBuffer2*	
CGlRenderer::CreateVB2(int nbVertex)
{
	CVertexBuffer2*	vb= new CVertexBuffer2();
	vb->m_start= m_bufV2+m_cptV2;
	vb->m_nb= nbVertex;
	m_cptV2+= nbVertex;
	return vb;
};

IVertexBuffer3*	
CGlRenderer::CreateVB3(int nbVertex)
{
	CVertexBuffer3*	vb= new CVertexBuffer3();
	vb->m_start= m_bufV3+m_cptV3;
	vb->m_nb= nbVertex;
	m_cptV3+= nbVertex;
	return vb;
};

IIndexBuffer2*		
CGlRenderer::CreateIB2(int nbIndex)
{
	CIndexBuffer2*	ib= new CIndexBuffer2();
	ib->m_start= m_bufI2+m_cptI2;
	ib->m_nb= nbIndex;
	m_cptI2+= nbIndex;
	return ib;
};

IIndexBuffer3*		
CGlRenderer::CreateIB3(int nbIndex)
{
	CIndexBuffer3*	ib= new CIndexBuffer3();
	ib->m_start= m_bufI3+m_cptI3;
	ib->m_nb= nbIndex;
	m_cptI3+= nbIndex;
	return ib;
};

ITextureCoordonateBuffer2*		
CGlRenderer::CreateTB2(int nbTC)
{
	CTextureCoordonateBuffer2*	tcb= new CTextureCoordonateBuffer2();
	tcb->m_start= m_bufT2+m_cptT2;
	tcb->m_nb= nbTC;
	m_cptT2+= nbTC;
	return tcb;
};

ITextureCoordonateBuffer3*
CGlRenderer::CreateTB3(int nbTC)
{
	CTextureCoordonateBuffer3*	tcb= new CTextureCoordonateBuffer3();
	tcb->m_start= m_bufT3+m_cptT3;
	tcb->m_nb= nbTC;
	m_cptT3+= nbTC;
	return tcb;
};

void 
CGlRenderer::PolyCalcPlane(VERTEX* p_tri)
{
	int* v1 = p_tri[0].orig;
	int* v2 = p_tri[1].orig;
	int* v3 = p_tri[2].orig;
	double rx1 = (double)(v1[0] - v1[0]) / FP_ONE;
	double ry1 = (double)(v2[1] - v1[1]) / FP_ONE;
	double rz1 = (double)(v2[2] - v1[2]) / FP_ONE;
	double rx2 = (double)(v3[0] - v1[0]) / FP_ONE;
	double ry2 = (double)(v3[1] - v1[1]) / FP_ONE;
	double rz2 = (double)(v3[2] - v1[2]) / FP_ONE;
	double A = (ry1 * rz2) - (ry2 * rz1 );
	double B = (rz1 * rx2) - (rz2 * rx1 );
	double C = (rx1 * ry2) - (rx2 * ry1 );
	double OneOnlen = 1/(sqrt( A * A + B * B + C * C));
	p_tri[2].nor[0]= p_tri[1].nor[0]= p_tri[0].nor[0]= (int)(A * FP_ONE * OneOnlen);
	p_tri[2].nor[1]= p_tri[1].nor[1]= p_tri[0].nor[1]= (int)(B * FP_ONE * OneOnlen);
	p_tri[2].nor[2]= p_tri[1].nor[2]= p_tri[0].nor[2]= (int)(C * FP_ONE * OneOnlen);
}

long 
CGlRenderer::eatchunk(HANDLE file, CVertexBuffer* vbToFill)
{
	unsigned short tmep;
	unsigned char temp2;
	VERTEX*	cur;
	short chunkid = prend_w(file);
	long chunklength = prend_d(file);
	int j, i = 6;
	switch (chunkid)
	{
	case 0x4D4D:
		do{
			tmep= prend_w(file);
			i += 2;
		}while ((tmep != 0x3D3D)&&(tmep != 0xB000));
		i -= 2;
		SetFilePointer(file, -2, 0, FILE_CURRENT);
		break;
	case 0x3D3D:
		break;
	case 0x4000:
		do{
			temp2= prend_c(file);
			++i;
		}while(temp2);
		break;
	case 0x4100:
		break;
	case 0x4110:
		nb= prend_w(file); 	
		i+=2;
		x = new int[nb];y = new int[nb];z = new int[nb];
		for (j = 0; j < nb; j++ )
		{
			x[j]= (int)((FP_ONE >> 3) * (prend_f(file)/2));
			y[j]= (int)((FP_ONE >> 3) * (prend_f(file)/2));
			z[j]= (int)((FP_ONE >> 3) * (prend_f(file)/2));
			i+=12;
		}
		break;
	case 0x4120:
		{		
			int cpV = m_cptV;
			vbToFill->m_nb		= prend_w(file);
			vbToFill->m_nb*= 3;
			ASSERT_FATAL(((m_cptV+vbToFill->m_nb)<m_cptMaxV), "Not Enough Polygones Buffered");
			i+=2;
			cur= vbToFill->m_start= (m_bufV+m_cptV);

			for (j = 0; j < vbToFill->m_nb; j+= 3)
			{
				int c1 = prend_w(file);
				int c2 = prend_w(file);
				int c3 = prend_w(file);
				int cS = prend_w(file);
				if ((c1 != c2) && (c2 != c3) && (c1 != c3))
				{
					cur[cpV].orig[0]= x[c1];
					cur[cpV].orig[1]= y[c1];
					cur[cpV].orig[2]= z[c1];
					cur[cpV].m_R= 0xF;
					cur[cpV].m_G= 0xF;
					cur[cpV].m_B= 0xF;
					cpV++;
					cur[cpV].orig[0]= x[c2];
					cur[cpV].orig[1]= y[c2];
					cur[cpV].orig[2]= z[c2];
					cur[cpV].m_R= 0xF;
					cur[cpV].m_G= 0xF;
					cur[cpV].m_B= 0xF;
					cpV++;
					cur[cpV].orig[0]= x[c3];
					cur[cpV].orig[1]= y[c3];
					cur[cpV].orig[2]= z[c3];
					cur[cpV].m_R= 0xF;
					cur[cpV].m_G= 0xF;
					cur[cpV].m_B= 0xF;
					cpV++;
					PolyCalcPlane(cur+(cpV-3));
				}
				i+=8;
			}
			m_cptV= cpV;
			SAFE_DELETE_ARRAY(x);SAFE_DELETE_ARRAY(y);SAFE_DELETE_ARRAY(z);
		}
		break;
	default:
		i = chunklength;
		SetFilePointer(file, i-6, 0, FILE_CURRENT);
	break;
	}
	while (i < chunklength) 
		i+= eatchunk(file, vbToFill);
	return chunklength;
}

IMeshStatic*
CGlRenderer::CreateMesh3DS(char* pInitFile)
{
	x= 0;y= 0;z= 0;nb=0;
	CMesh3DS*				mesh= new CMesh3DS();
	CVertexBuffer	vbToFill;
	HANDLE file;

	VERIFY_FATAL(((file= CreateFile(pInitFile, GENERIC_READ, FILE_SHARE_READ,0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) != INVALID_HANDLE_VALUE), "Error Opening File");

	eatchunk(file, &vbToFill);
	CloseHandle(file);

	int xmax = IFP(-1000), xmin = IFP(1000);
	int ymax = IFP(-1000), ymin = IFP(1000);
	int zmax = IFP(-1000), zmin = IFP(1000);
	for ( int i = 0; i < vbToFill.m_nb; i++ )
	{
		int x = vbToFill.m_start[i].orig[0];
		int y = vbToFill.m_start[i].orig[1];
		int z = vbToFill.m_start[i].orig[2];
		if (x < xmin) xmin = x;
		if (x > xmax) xmax = x;
		if (y < ymin) ymin = y;
		if (y > ymax) ymax = y;
		if (z < zmin) zmin = z;
		if (z > zmax) zmax = z;
	}
	double du = (FP_ONE * 256.0) / (double)(xmax - xmin);
	double dv = (FP_ONE * 256.0) / (double)(zmax - zmin);
	for ( i = 0; i < vbToFill.m_nb; i+=3){
		for ( int j = 0; j < 3; j++ ){
			vbToFill.m_start[i+j].m_U1= (int)((vbToFill.m_start[i+j].orig[0] - xmin) * du);
			vbToFill.m_start[i+j].m_V1= (int)((vbToFill.m_start[i+j].orig[2] - zmin) * dv);
		}
	}
	mesh->m_vb.m_start= vbToFill.m_start;
	mesh->m_vb.m_nb		= vbToFill.m_nb;
	return mesh;
}

IMeshAnimated*
CGlRenderer::CreateMeshMD2(char* pInitFile)
{
struct 
{ 
	int	magic,	version,	skinWidth,	skinHeight,	frameSize
		,	numSkins,numVertices,	numTexCoords,	numTriangles,numGlCommands,	numFrames
		,	offsetSkins,	offsetTexCoords,	offsetTriangles
		,	offsetFrames,	offsetGlCommands,	offsetEnd; 
}								md2_header;
	unsigned long	read;
	CMeshMD2*			mesh= new CMeshMD2();
	VERTEX2*			curV;
	TEXCOORD2*		curT;
	TRIANGLES2*		curI;
	HANDLE				file;
	int						i, j;

	VERIFY_FATAL(((file= CreateFile(pInitFile, GENERIC_READ, FILE_SHARE_READ,0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) != INVALID_HANDLE_VALUE), "Error Opening File");
	ReadFile(file, &md2_header, sizeof(md2_header), &read, NULL);

	curV= mesh->m_vb.m_start=	(m_bufV2+m_cptV2);
				mesh->m_vb.m_nb		=	md2_header.numVertices;
	curT=											(m_bufT2+m_cptT2);
	curI= mesh->m_ib.m_start=	(m_bufI2+m_cptI2);
				mesh->m_ib.m_nb		=	md2_header.numTriangles;
				mesh->m_nNbFrame	=	md2_header.numFrames;

//Read UV
	int startPosUV= m_cptT2;
	SetFilePointer(file, md2_header.offsetTexCoords, 0, FILE_BEGIN);
	for (i = 0; i < md2_header.numTexCoords; ++i){
		curT->m_U1= prend_w(file)<<FPP;
		curT->m_V1= prend_w(file)<<FPP;
		++curT;++m_cptT2;
	}

//Read Coordonnes
	SetFilePointer(file, md2_header.offsetFrames, 0, FILE_BEGIN);
	for(i = 0; i < md2_header.numFrames; ++i){
		float scale[3],transl[3];char name[16];
		scale[0]= (prend_f(file));
		scale[1]= (prend_f(file));
		scale[2]= (prend_f(file));
		transl[0]= (prend_f(file));
		transl[1]= (prend_f(file));
		transl[2]= (prend_f(file));
		ReadFile(file, &name, 16, &read, NULL);
		for(j = 0; j < md2_header.numVertices; ++j){
			curV->orig[0]	= FP(((float)((int)prend_c(file)) * scale[0]) + transl[0]);
			curV->orig[1]	= FP((((float)((int)prend_c(file)) * scale[1]) + transl[1]));
			curV->orig[2]	= FP(((float)((int)prend_c(file)) * scale[2]) + transl[2]);
			name[0]				= prend_c(file);
			++curV;++m_cptV2;
		}
	}

//Read triangles
	SetFilePointer(file, md2_header.offsetTriangles, 0, FILE_BEGIN);
	for(i = 0; i < md2_header.numTriangles; ++i){
		curI->idxVertex[0]	= (int)prend_w(file);
		curI->idxVertex[1]	= (int)prend_w(file);
		curI->idxVertex[2]	= (int)prend_w(file);
		curI->idxTexCoor[0]	= startPosUV+(int)prend_w(file);
		curI->idxTexCoor[1]	= startPosUV+(int)prend_w(file);
		curI->idxTexCoor[2]	= startPosUV+(int)prend_w(file);
		++curI;++m_cptI2;
	}

	CloseHandle(file);
	return mesh;
}

TEXTURE*
CGlRenderer::CreateTexture(char* pInitFile, int option)
{
	int								glModeMag=0;
	int								glModeMin=0;
	TEXTURE*					cntTexture=0;
	AUX_RGBImageRec*	dataTexture=0;
	VERIFY_FATAL(!_access(pInitFile, 0x04), "Can't Acces to texture");
	
	dataTexture= auxDIBImageLoad(pInitFile);
	cntTexture= new TEXTURE;
	cntTexture->nX= dataTexture->sizeX;
	cntTexture->nY= dataTexture->sizeY;

	if(option&IRenderer::TEX_NEAREST){
		glModeMag= glModeMin= GL_NEAREST;
	}else if(option&IRenderer::TEX_LINEAR){
		glModeMag= glModeMin= GL_LINEAR;
	}else if(option&IRenderer::TEX_MIPMAP){
		glModeMag= GL_LINEAR;
		glModeMin= GL_LINEAR_MIPMAP_NEAREST;
	}
	glGenTextures(1, (unsigned int*)&cntTexture->pData);
	glBindTexture(GL_TEXTURE_2D, (unsigned int)cntTexture->pData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,glModeMag);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,glModeMin);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, dataTexture->sizeX, dataTexture->sizeY, 0, 
				 GL_RGB, GL_UNSIGNED_BYTE, dataTexture->data);

	if (dataTexture->data)	free(dataTexture->data);
	free(dataTexture);

	return cntTexture;
}

void			
CGlRenderer::Clear(int param)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void
CGlRenderer::Flip()
{
	glFlush();	
	SwapBuffers(m_hDC);
}

void
CGlRenderer::ProcessMesh(CMatrix& p_mat, IMeshAnimated* p_mesh)
{
/*	static	int previous= IFP(BMI_GetTime());
					int thisFrame= IFP(BMI_GetTime());
					int dif= thisFrame-previous;
	(this->*process)(p_mat, ((CVertexBuffer*)((CMeshMD2*)p_mesh)->GetVB())->m_start, ((CVertexBuffer*)((CMeshMD2*)p_mesh)->GetVB())->m_nb, dif);
	previous= thisFrame;
*/
	static int cpt=0;
	VERTEX2*	p_tri	= (p_mesh->GetVB())->Lock();
	int				p_nb	= ((CVertexBuffer2*)p_mesh->GetVB())->m_nb;
	VERTEX2*	p_tri2= p_tri+(p_nb*cpt);
	for (int i = 0; i < p_nb;i++) 
	{
		p_tri[i].rot[0] = p_tri2[i].orig[0];
		p_tri[i].rot[1] = p_tri2[i].orig[1];
		p_tri[i].rot[2] = p_tri2[i].orig[2];
	}
	if((++cpt)>=((CMeshMD2*)p_mesh)->m_nNbFrame)
		cpt=0;
}

void
CGlRenderer::DrawMesh(IMeshAnimated* p_mesh)
{	
	DrawPrimitiveIB(p_mesh->GetIB(), p_mesh->GetVB(), p_mesh->GetTexture());
}

void			
CGlRenderer::DrawPrimitiveIB(IIndexBuffer2* p_ib, IVertexBuffer2* p_vb, TEXTURE* p_tex)
{
	(this->*Drawer2)(((CVertexBuffer2*)p_vb)->m_start, ((CIndexBuffer2*)p_ib)->m_start, p_tex, ((CVertexBuffer2*)p_vb)->m_nb, ((CIndexBuffer2*)p_ib)->m_nb);
}

void			
CGlRenderer::DrawPrimitiveIB(IIndexBuffer3* p_ib, IVertexBuffer3* p_vb, TEXTURE* p_tex)
{
	(this->*Drawer3)(((CVertexBuffer3*)p_vb)->m_start, ((CIndexBuffer3*)p_ib)->m_start, 0, p_tex, ((CVertexBuffer3*)p_vb)->m_nb, ((CIndexBuffer3*)p_ib)->m_nb);
}

void			
CGlRenderer::DrawPrimitiveIB(IIndexBuffer3* p_ib, IVertexBuffer3* p_vb, ITextureCoordonateBuffer3* p_tb, TEXTURE* p_tex)
{
	(this->*Drawer3)(((CVertexBuffer3*)p_vb)->m_start, ((CIndexBuffer3*)p_ib)->m_start, ((CTextureCoordonateBuffer3*)p_tb)->m_start, p_tex, ((CVertexBuffer3*)p_vb)->m_nb, ((CIndexBuffer3*)p_ib)->m_nb);
}

//Internal Fonctions
int 
CGlRenderer::SetDCPixelFormat(HDC pHDC)
{
	int nPixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA, m_nBpp, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0,									
		16,	0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};

	if (!(nPixelFormat=ChoosePixelFormat(pHDC,&pfd)))
		return FALSE;
	if(!SetPixelFormat(pHDC,nPixelFormat,&pfd))
		return FALSE;
	return TRUE;
}

void			
CGlRenderer::D_Point(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb)
{
}

void			
CGlRenderer::D_Lines(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb)
{
}

void			
CGlRenderer::D_Flat(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb)
{
}

void			
CGlRenderer::D_Text(VERTEX* p_tri, TEXTURE* p_tex, unsigned int p_nb)
{
}

void			
CGlRenderer::D2_Point(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
	unsigned int i;
	glColor4f(1.0f,1.0f,1.0f,0.5f);			// Full Brightness, 50% Alpha ( NEW )
	glBegin(GL_POINTS);
	for(i= 0; i < p_nbV; ++i){
		glVertex3f(((float)p_ver->rot[0])/1024, ((float)p_ver->rot[1])/1024, ((float)p_ver->rot[2])/1024);
		++p_ver;
	}
	glEnd();
}

void			
CGlRenderer::D2_Lines(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
	unsigned int i;
	glColor4f(1.0f,1.0f,1.0f,0.5f);			// Full Brightness, 50% Alpha ( NEW )
	glBegin(GL_TRIANGLES);
	for(i= 0; i < p_nbI; ++i){
		glVertex3f(((float)p_ver[p_tri->idxVertex[0]].rot[0])/1024, ((float)p_ver[p_tri->idxVertex[0]].rot[1])/1024, ((float)p_ver[p_tri->idxVertex[0]].rot[2])/1024);
		glVertex3f(((float)p_ver[p_tri->idxVertex[1]].rot[0])/1024, ((float)p_ver[p_tri->idxVertex[1]].rot[1])/1024, ((float)p_ver[p_tri->idxVertex[1]].rot[2])/1024);
		glVertex3f(((float)p_ver[p_tri->idxVertex[2]].rot[0])/1024, ((float)p_ver[p_tri->idxVertex[2]].rot[1])/1024, ((float)p_ver[p_tri->idxVertex[2]].rot[2])/1024);
		++p_tri;
	}
	glEnd();
}

void			
CGlRenderer::D2_Flat(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
}

//je risque d'avoir un probleme avec plusieur modele et les textre coordonne 
//faire un TextureCoordonateBuffer avec start et nb
void			
CGlRenderer::D2_Text(VERTEX2* p_ver, TRIANGLES2* p_tri, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
	unsigned int i;
	glBindTexture(GL_TEXTURE_2D, (unsigned int)p_tex->pData);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	for(i= 0; i < p_nbI; ++i){
		glTexCoord2f(((float)m_bufT2[p_tri->idxTexCoor[0]].m_U1)/262144,((float)m_bufT2[p_tri->idxTexCoor[0]].m_V1)/262144);
		glVertex3f(((float)p_ver[p_tri->idxVertex[0]].rot[0])/1024, ((float)p_ver[p_tri->idxVertex[0]].rot[1])/1024, ((float)p_ver[p_tri->idxVertex[0]].rot[2])/1024);
		glTexCoord2f(((float)m_bufT2[p_tri->idxTexCoor[1]].m_U1)/262144,((float)m_bufT2[p_tri->idxTexCoor[1]].m_V1)/262144);
		glVertex3f(((float)p_ver[p_tri->idxVertex[1]].rot[0])/1024, ((float)p_ver[p_tri->idxVertex[1]].rot[1])/1024, ((float)p_ver[p_tri->idxVertex[1]].rot[2])/1024);
		glTexCoord2f(((float)m_bufT2[p_tri->idxTexCoor[2]].m_U1)/262144,((float)m_bufT2[p_tri->idxTexCoor[2]].m_V1)/262144);
		glVertex3f(((float)p_ver[p_tri->idxVertex[2]].rot[0])/1024, ((float)p_ver[p_tri->idxVertex[2]].rot[1])/1024, ((float)p_ver[p_tri->idxVertex[2]].rot[2])/1024);
		++p_tri;
	}
	glEnd();
}

void			
CGlRenderer::D3_Point(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
	glEnable(GL_VERTEX_ARRAY);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glVertexPointer(3, GL_FLOAT, 0, p_ver);
  glDrawElements(GL_POINTS, (p_nbI)*3, GL_UNSIGNED_INT, p_tri);

	glDisable(GL_VERTEX_ARRAY);
}

void			
CGlRenderer::D3_Lines(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
	glEnable(GL_VERTEX_ARRAY);
	
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glVertexPointer(3, GL_FLOAT, 0, p_ver);
  glDrawElements(GL_LINES, p_nbI*3, GL_UNSIGNED_INT, p_tri);
	
	glDisable(GL_VERTEX_ARRAY);
}

void			
CGlRenderer::D3_Flat(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
}

void			
CGlRenderer::D3_Text(VERTEX3* p_ver, TRIANGLES3* p_tri, TEXCOORD3* p_tb, TEXTURE* p_tex, unsigned int p_nbV, unsigned int p_nbI)
{
	glEnable(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, (unsigned int)p_tex->pData);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glVertexPointer(3, GL_FLOAT, 0, p_ver);
	glTexCoordPointer(2, GL_FLOAT, 0, p_tb);
  glDrawElements(GL_TRIANGLES, (p_nbI)*3, GL_UNSIGNED_INT, p_tri);
	
	glDisable(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void						
CGlRenderer::PI_Nearest(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter)
{

}

void						
CGlRenderer::PI_Linear(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter)
{

}

void						
CGlRenderer::PI_Cosine(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter)
{

}

void						
CGlRenderer::PI_Cubic(CMatrix& p_mat, VERTEX* p_tri, unsigned int p_nb, int p_inter)
{

}

