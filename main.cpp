#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include "interfaces.h"
#include "mathFP.h"
#define _PROFILE
#include "macros.h"
PROFILER_INC

#define KEY_UP		0x01
#define KEY_DOWN	0x02
#define KEY_LEFT	0x04
#define KEY_RIGHT	0x08

char		kb=0;
LRESULT CALLBACK WindowProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam) 
{
	switch(msg)
	{	
		case WM_CREATE: 
			return(0);
		case WM_PAINT:
			return(0);
		case WM_TIMER:
			break;
		case WM_KEYDOWN:
			switch(wparam)
			{
				case VK_RIGHT:
					kb|= KEY_RIGHT;
					break;
				case VK_LEFT:
					kb|= KEY_LEFT;
					break;
				case VK_UP:
					kb|= KEY_UP;
					break;
				case VK_DOWN:
					kb|= KEY_DOWN;
					break;
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			return(0);
		case WM_KEYUP:
			switch(wparam)
			{
				case VK_RIGHT:
					kb&= ~KEY_RIGHT;
					break;
				case VK_LEFT:
					kb&= ~KEY_LEFT;
					break;
				case VK_UP:
					kb&= ~KEY_UP;
					break;
				case VK_DOWN:
					kb&= ~KEY_DOWN;
					break;
			}
			return(0);
		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);
	}
	
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

IRenderer			*gRenderer;
IMeshAnimated	*gMyMesh2;
ITerrain			*gTerrain;
int rotX=192;	int posX=0;
int rotY=0;	int posY=0;
int rotZ=64;	int posZ=-100;

int rotX2=192;	int posX2=0;
int rotY2=0;		int posY2=0;
int rotZ2=64;		int posZ2=-100;
int incX= 1;		int incY=1;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) 
{
	PROFILER_DEC;
	PROFILER_INIT;
	CGlMatrix		mat;
	CMatrix			dummy;
	MSG					msg;
	
	gRenderer	= (IRenderer*)CreateInstance(MOD_GL_RENDERER);																			
	gTerrain	=	(ITerrain*)CreateInstance(MOD_TERRAIN);																						
	gRenderer->Init((unsigned long)hinstance, 640, 480, 32, 0);																		
	gRenderer->BufferPoly2(100000, 512, 1024);																										
	gRenderer->BufferPoly3(32*32, 32*32*2, 31*31*2);																							
	gRenderer->SetRendererStage(IRenderer::RST_TEXT);																							
	gMyMesh2= gRenderer->CreateMeshMD2("../Data/tris.MD2");																				
	gMyMesh2->SetTexture(gRenderer->CreateTexture("../Data/cloud2.bmp", IRenderer::TEX_NEAREST));	

	//temp
	//Create the Terrain
	gTerrain->SetVB(gRenderer->CreateVB3(32*32));																									
	gTerrain->SetTB(gRenderer->CreateTB3(32*32));																									
	gTerrain->SetIB(gRenderer->CreateIB3(31*31*2));																								
	gTerrain->SetTexture(gRenderer->CreateTexture("../Data/grass3.bmp", IRenderer::TEX_LINEAR));	
		
	//Init the terrain
	int x, y;
	VERTEX3* pv= gTerrain->GetVB()->Lock();																												
	for(x= 0; x<32; ++x)	for(y= 0; y<32; ++y){																										
		pv->orig[0]= (x-16.0f)*32;																																	
		pv->orig[1]= (y-16.0f)*32;																																	
		pv->orig[2]= (-200.0f);																																			
		++pv;
	}																																															

	TRIANGLES3* pt= gTerrain->GetIB()->Lock();																										
	for(x= 0; x<31; ++x)	for(y= 0; y<31; ++y){																										
		pt->idxVertex[0]= (y*32)+x;																																	
		pt->idxVertex[1]= ((y+1)*32)+(x+1);																													
		pt->idxVertex[2]= ((y+1)*32)+x;																															
		++pt;																																												
		pt->idxVertex[0]= (y*32)+x;																																	
		pt->idxVertex[1]= (y*32)+(x+1);																															
		pt->idxVertex[2]= ((y+1)*32)+(x+1);																													
		++pt;
	}																																															

	TEXCOORD3* ptb= gTerrain->GetTB()->Lock();																										
	for(x= 0; x<32; ++x)	for(y= 0; y<32; ++y){																									
		ptb->m_U1= x/32.0f;																																					
		ptb->m_V1= y/32.0f;																																					
		++ptb;
	}																																															
	gTerrain->SetInitHM("../Data/hmap32.raw");																										

	InitTables();																																									
	while(1){																																											
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if (msg.message == WM_QUIT)				break;																									
			TranslateMessage(&msg);																																		
			DispatchMessage(&msg);																																		
		}
		rotZ+= 1; if (rotZ > 255) rotZ= 0;																													

		gRenderer->Clear(IRenderer::CLR_ZERO);																											

//		gRenderer->SetRendererStage(IRenderer::RST_LINES);																				
//		for(int i= 0; i < 1; i++){for(int j= 0; j < 1; j++){
			mat.Identity();																																							
			mat.Translate(IFP(posX), IFP(posY), IFP(posZ));
			mat.Rotate(rotX, rotY, rotZ);																																
		PROFILER_BEFORE
			gRenderer->ProcessMesh(dummy, gMyMesh2);																										
			gRenderer->ProcessMesh(dummy, gMyMesh2);																										
			gRenderer->ProcessMesh(dummy, gMyMesh2);																										
			gRenderer->DrawMesh(gMyMesh2);																															
			gRenderer->DrawMesh(gMyMesh2);																															
			gRenderer->DrawMesh(gMyMesh2);																															
		PROFILER_AFTER("OneLoop:", "\r\n")
//		}}

//		gRenderer->SetRendererStage(IRenderer::RST_LINES);
		if(kb & KEY_RIGHT){
			if((++rotZ2)>255)rotZ2= 0;
			incX= sin(rotZ2);
			incY= cos(rotZ2);
		}
		if(kb & KEY_LEFT){
			if((--rotZ2)<0)rotZ2= 255;
			incX= sin(rotZ2);
			incY= cos(rotZ2);
		}
		if(kb & KEY_UP){
			posX2+= incX>>2;
			posZ2+= incY>>2;
		}
		if(kb & KEY_DOWN){
			posX2-= incX>>2;
			posZ2-= incY>>2;
		}
		mat.Identity();																																							
		gluLookAt(((float)posX2/128), ((float)posY2/128), ((float)posZ2/128), 
							((float)posX2/128)+incX, ((float)posY2/128)+incY, 0, 1, 0, 1 );
		gRenderer->DrawPrimitiveIB(gTerrain->GetIB(), gTerrain->GetVB(), gTerrain->GetTB(), 
			gTerrain->GetTexture());																																	
		
		gRenderer->Flip();																																					
	}
	gRenderer->Release();																																					
	gTerrain->Release();																																					
	return(msg.wParam);
}