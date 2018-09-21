#include "interfaces.h"
#include "GlRenderer.h"
#include "3DEngine.h"
#include "PlainTerrain.h"

void* CreateInstance(int what)
{
	void* pRet;
	if(what == MOD_GL_RENDERER)	{pRet= (void*)new CGlRenderer();((IRenderer*)pRet)->AddRef();}
	if(what == MOD_TERRAIN)			{pRet= (void*)new CPlainTerrain();((ITerrain*)pRet)->AddRef();}
	return pRet;
}