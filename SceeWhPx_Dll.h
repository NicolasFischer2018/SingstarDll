#include "PAKio.h"

#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information

#else
   #define DLLDIR  __declspec(dllimport)   // import DLL information

#endif

DLLDIR	int	SceeWhPx_TOC_Read_Dll(char *, char *);
DLLDIR  int	SceeWhPx_Create_Dll(char *, char *, int);
DLLDIR	int SceeWhPx_File_Extract_Filename_Dll(char *, char *, char *, int);
DLLDIR	int SceeWhPx_File_Extract_All_Dll(char *, char *);
