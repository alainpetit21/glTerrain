#ifndef _FILE_HELPER_H_
#define _FILE_HELPER_H_
inline unsigned char prend_c(HANDLE file)
{
	unsigned long read;
	unsigned char	data;
	ReadFile(file, &data, 1, &read, 0);
	ASSERT_FATAL((read == 1), "Error Reading File");
	return data;
}
inline float prend_f(HANDLE file)
{
	unsigned long read;
	float	data;
	ReadFile(file, &data, 4, &read, 0);
	ASSERT_FATAL((read == 4), "Error Reading File");
	return data;
}
inline unsigned short prend_w(HANDLE file)
{
	unsigned long		read;
	unsigned short	data;
	ReadFile(file, &data, 2, &read, 0);
	ASSERT_FATAL((read == 2), "Error Reading File");
	return data;
}
inline unsigned long prend_d(HANDLE file)
{
	unsigned long read;
	unsigned long	data;
	ReadFile(file, &data, 4, &read, 0);
	ASSERT_FATAL((read == 4), "Error Reading File");
	return data;
}
#endif // _FILE_HELPER_H_
