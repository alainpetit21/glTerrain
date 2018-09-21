#ifndef _MACROS_H_
#define _MACROS_H_
#pragma message("_MACROS_H_")

//////////////////////////////////////////////////////////////////////
// Commun use macros

#undef	NULL		
#define NULL					0

#undef	SAFE_DELETE
#define SAFE_DELETE(a)			if (a){delete a; a= NULL;}

#undef	SAFE_RELEASE
#define SAFE_RELEASE(a)			if (a){a->Release(); a= NULL;}

#undef	SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a)	if (a){delete []a; a= NULL;}

#undef	ARRAY_SIZE
#define ARRAY_SIZE(a)			(sizeof(a) / sizeof(a[0]))

#undef _T
#ifndef _UNICODE
	#define _T
#else
	#define _T	L
#endif

#undef ASSERT
#ifndef _WIN32
	#ifdef _DEBUG
		#define ASSERT(a, s)	if (!a)	{9/a;}	// Will crash!
	#else
		#define ASSERT(a, s)
	#endif	// DEBUG
#elif defined(_POCKET_PC)
	#ifdef _DEBUG
		#ifdef _BMI_BUILD_DLL
			#define ASSERT(a, s)	if(!a)	{::MessageBox((HWND)::ghWnd, s, 0, 0);exit(-1);}
		#elif defined(_USE_BMI_DLL)
			#define ASSERT(a, s)	if(!a)	{::MessageBox((HWND)BMI_GetHWND(), s, 0, 0);exit(-1);}
		#endif
	#else
		#define ASSERT(a, s)
	#endif	// DEBUG
#else
	#ifdef _DEBUG
		#define ASSERT(a, s)	if(!a)	{::MessageBox(NULL, s, 0, 0);}
	#else
		#define ASSERT(a, s)
	#endif	// DEBUG
#endif	// _WIN32

#undef	VERIFY
#ifndef _WIN32
	#ifdef _DEBUG
		#define VERIFY(a, s)	if (!a)	{9/a;}	// Will crash!
	#else
		#define VERIFY(a, s)	if(!a)	{::MessageBox(NULL, s, 0, 0);}
	#endif	// DEBUG
#elif defined(_POCKET_PC)
	#ifdef _DEBUG
		#define VERIFY(a, s)	if(!a)	{::MessageBox(NULL, s, 0, 0);}
	#else
		#define VERIFY(a, s)	a;
	#endif	// DEBUG
#else
	#ifdef _DEBUG
		#define VERIFY(a, s)	if(!a)	{::MessageBox(NULL, s, 0, 0);}
	#else
		#define VERIFY(a, s)	a;
	#endif	// DEBUG
#endif	// _WIN32

#undef TRAP_MSG
#if defined(_WIN32)||defined(_POCKET_PC)
	#define TRAP_MSG(a)												\
		if(PeekMessage(&a,NULL,0,0,PM_REMOVE)){	\
			if(a.message == WM_QUIT)	break;			\
			TranslateMessage(&a);									\
			DispatchMessage(&a);									\
		}
#elif defined(_DOS)
	typedef int	MSG;
	#define TRAP_WINDOWS(a)	;
#elif defined(_OS9)
	typedef int	MSG;
	#define TRAP_WINDOWS(a)	;
#elif defined(_TES)
	typedef int	MSG;
	#define TRAP_WINDOWS(a)	;
#elif defined(_LINUX)
	typedef int	MSG;
	#define TRAP_WINDOWS(a)	;
#endif


//////////////////////////////////////////////////////////////////////
// FIXMEs / TODOs / NOTE macros

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE( x )  message( x )
#define FILE_LINE  message( __FILE__LINE__ )

#define TODO( x )  message( __FILE__LINE__"\n"           \
        " ------------------------------------------------\n" \
        "|  TODO :   " #x "\n" \
        " -------------------------------------------------\n" )
#define FIXME( x )  message(  __FILE__LINE__"\n"           \
        " ------------------------------------------------\n" \
        "|  FIXME :  " #x "\n" \
        " -------------------------------------------------\n" )
#define todo( x )  message( __FILE__LINE__" TODO :   " #x "\n" ) 
#define fixme( x )  message( __FILE__LINE__" FIXME:   " #x "\n" ) 

//////////////////////////////////////////////////////////////////////
// FIXMEs / TODOs / NOTE macros
#undef ENTER_FUNCTION
#if defined(_TRACE_FUNCTION_ENTRY) && (defined(_WIN32) || defined(_POCKET_PC))
#define ENTER_FUNCTION(x) OutputDebugString(_T"\n");OutputDebugString(_T#x);
#define si(x) if(x){																			\
								OutputDebugString(_T"\n\t");								\
								OutputDebugString(_T"Enter Condition : ");	\
								OutputDebugString(L#x);
#else
#define ENTER_FUNCTION(x)
#define si(x) if(x){
#endif
//////////////////////////////////////////////////////////////////////
// Profiling

#undef PROFILER_INC
#undef PROFILER_DEC
#undef PROFILER_INIT
#undef PROFILER_BEFORE
#undef PROFILER_AFTER
#ifdef _PROFILE
	#if defined(_POCKET_PC)
		#define PROFILER_INC
		#define PROFILER_DEC															\
			__int64 freq;																\
			__int64 timePrev;															\
			__int64 time;																\
			wchar_t  buf[64];														

		#define PROFILER_INIT															\
			::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);							\
			buf[0]='\0';

		#define PROFILER_BEFORE															\
			::QueryPerformanceCounter((LARGE_INTEGER*)&time);							\
			timePrev= time;
		#define PROFILER_AFTER(string, returnLine)										\
			::QueryPerformanceCounter((LARGE_INTEGER*)&time);							\
			swprintf(buf,_T"%d\t%s%s",(unsigned int)(time-timePrev),string, returnLine);			\
			OutputDebugString(buf);
	#elif defined(_WIN32)
		#define PROFILER_INC
		#define PROFILER_DEC															\
			__int64 freq;																\
			__int64 timePrev;															\
			__int64 time;																\
			char  buf[64];														

		#define PROFILER_INIT															\
			::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);							\
			buf[0]='\0';

		#define PROFILER_BEFORE															\
			::QueryPerformanceCounter((LARGE_INTEGER*)&time);							\
			timePrev= time;
		#define PROFILER_AFTER(string, returnLine)										\
			::QueryPerformanceCounter((LARGE_INTEGER*)&time);							\
			sprintf(buf,"%d\t%s%s",(unsigned int)(time-timePrev),string, returnLine);\
			OutputDebugString(buf);
	#elif defined(_LINUX)
		#define PROFILER_INC
			__int64 timePrev;															\
			__int64 time;																\
		#define PROFILER_DEC
		#define PROFILER_INIT
		#define PROFILER_BEFORE
		#define PROFILER_AFTER(string, returnLine)
	#else
		#error Profilling tools unsupported yet!
	#endif
#else
	#define PROFILER_INC						;
	#define PROFILER_DEC						;
	#define PROFILER_INIT						;
	#define PROFILER_BEFORE						;
	#define PROFILER_AFTER(string, returnLine)	;
#endif	//	_PROFILE

//////////////////////////////////////////////////////////////////////
// Conversions

#undef CHAR_TO_DIGIT
#define CHAR_TO_DIGIT(a)		((a) - '0')

#undef DIGIT_TO_CHAR
#define DIGIT_TO_CHAR(a)		((a) + '0')

#undef	SWAP32
#define SWAP32(a)				(((((long)a) & 0xff000000)	>> 24)	|	\
								((( (long)a) & 0x00ff0000)	>> 8)	|	\
								((  (long)a) & 0x0000ff00)	<< 8)	|	\
								((( (long)a) & 0x000000ff)	<< 24))

#undef	SWAP16
#define SWAP16(a)				(((((short)a) & 0xff00) >> 8)		|	\
								((( (short)a) & 0x00ff) << 8))

//////////////////////////////////////////////////////////////////////
// Maths

#undef	PI
#define PI						3.1415926535f

#undef	IS_ODD
#define IS_ODD(a)				(((unsigned char)a)&1)

#undef	IS_EVEN
#define IS_EVEN(a)				(!(((unsigned char)a)&1))

#undef	NUM_SIGN
#define NUM_SIGN(a)				((p<0)?-1:1)

#undef	DEG_TO_RAD
#define DEG_TO_RAD(a)			(p*PI/180.0f)

#undef	RAD_TO_DEG
#define RAD_TO_DEG(a)			(p*180.0f/PI)

#undef	RAND_RANGE
#define RAND_RANGE(a,b)			(a + (rand()%(b-a+1)))

#undef	BUILD_TRIGO_TABLES_DEG
#define BUILD_TRIGO_TABLES_DEG(s,c,t)					\
	for (uint ang= 0; ang < 360; ang++)					\
	{													\
		float theta= DEG_TO_RAD((float)ang);			\
		if(s)	(s)[ang]= sin(s);						\
		if(c)	(c)[ang]= cos(c);						\
		if(t)	(t)[ang]= tan(t);						\
	}

#undef	MIN
#define MIN(a,b)				((a<b)?a:b)

#undef	MAX
#define MAX(a,b)				((a>b)?a:b)

#undef	ABS
#define ABS(a)					((a>0)?a:-a)

#undef	SWAP
#define SWAP(a,b,t)				t=a;a=b;a=t;

#undef	IS_DIGIT
#define IS_DIGIT(a)				((unsigned)CHAR_TO_DIGIT(a)<=9)

#undef	IS_DIGIT_BIN
#define IS_DIGIT_BIN(a)			((unsigned)CHAR_TO_DIGIT(a)<=1)

#undef	IS_DIGIT_OCT
#define IS_DIGIT_OCT(a)			((unsigned)CHAR_TO_DIGIT(a)<=7)

#undef	IS_DIGIT_HEX
#define IS_DIGIT_HEX(a)			(((unsigned)CHAR_TO_DIGIT(a) <= 8)	||	\
								(('a' <= (a)) && ((a) <= 'f'))		||	\
								(('A' <= (a)) && ((a) <= 'F')))	

/*#undef	LOWEST_BIT_MASK
#define	LOWEST_BIT_MASK(b)		((b & -b))

#undef	LOWEST_BIT_MASK_NO_2_COMPLEMENT
#define LOWEST_BIT_MASK_NO_2_COMPLEMENT(a)\
								(a & ((~a)+1))

#undef	IS_POW_2
#define IS_POW_2(a)				((LOWEST_BIT_MASK(a) == a)? 1:0)

#undef	CEIL_POW_2
#define CEIL_POW_2(a)				\	
	int res= LOWEST_BIT_MASK(a);	\
	while(res < a)					\
		res<<= 1;					\
*/
 //////////////////////////////////////////////////////////////////////
// Display

#undef	RGB_16BITS_555
#define RGB_16BITS_555(r,g,b)	((b&0x1F)		+ \
								((g&0x1F)<<5)	+ \
								((r&0x1F)<<10))


#undef	RGB_16BITS_565
#define RGB_16BITS_565(r,g,b)	((b&0x1F)		+ \
								((g&0x3F)<<6)	+ \
								((r&0x1F)<<11))

//To implement
#undef RGB_555_TO_565
#undef RGB_565_TO_555
#undef RGB_24_TO_555
#undef RGB_24_TO_565
#undef RGB_32_TO_555
#undef RGB_32_TO_565
#undef RGB_555_TO_32
#undef RGB_565_TO_24
#undef RGB_555_TO_32
#undef RGB_565_TO_32

#endif	// _MACROS_H_
