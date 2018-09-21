#ifndef _DEBUG_H_
#define _DEBUG_H_
#ifdef _DEBUGUAGE
#define ASSERT_FATAL(x, msg) if(!(x)) {MessageBox(NULL,msg,"ERROR",MB_OK|MB_ICONEXCLAMATION);::PostQuitMessage(0);}
#define VERIFY_FATAL(x, msg) if(!(x)) {MessageBox(NULL,msg,"ERROR",MB_OK|MB_ICONEXCLAMATION);::PostQuitMessage(0);}
#define SAFE_DELETE_ARRAY(x) if(x) {delete[]x;x=NULL;}
#define SAFE_DELETE(x) if(x) {delete x;x=NULL;}
#else
#define ASSERT_FATAL(x, msg) __asm{nop};
#define VERIFY_FATAL(x, msg) __asm{nop};
#define SAFE_DELETE_ARRAY(x) if(x) {delete[]x;x=NULL;}
#define SAFE_DELETE(x) if(x) {delete x;x=NULL;}
#endif
#endif