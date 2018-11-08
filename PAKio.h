#include <windows.h>
#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h>
#include <string.h>
#include <direct.h>
#include <time.h>

#define	SceeWhPk	0
#define	SceeWhPC	1
#define	LOG_DISABLE 0
#define	LOG_ENABLE	1
#define BUFFER_SIZE 2048
#define FIRST_OFFSET 16
#define OFFSET_SIZE 2048
#define	FILE_RESET 2
#define	BASE_RESET 1
#define	FULL_RESET 0


struct	__Whp_struct {
	char			*PAK_file;
	int				error;
	int				readed_TOC;
	char			format[9];
	char			date[9];
	char			time[9];
	unsigned long	hdrsize;
	unsigned long	nbrfile;
	char			StrCompilePath[128];
	char			extlist[31][4];
	struct			{
					unsigned long	offset;
					char			*filename;
					unsigned int	filename_len;
					char			*reduced_filename;
					unsigned int	reduced_filename_len;
					unsigned int	previous_letters;
					unsigned int	header_len;
					unsigned short	unknown;
					unsigned long	size;
					unsigned long	checksum;
					unsigned int	extension;
					unsigned long	increased_header_len;
	} *filelist;
};

typedef	struct	__Whp_struct _Whp_struct;


_Whp_struct		SceeWhPx_TOC_Read(char *, _Whp_struct);
int				SceeWhPx_File_Extract(_Whp_struct, char *, char *, int);

int				SceeWhPx_Create(char *,  _Whp_struct, char *, int);

int				SceeWhPx_CreateTXT(char *, _Whp_struct);

_Whp_struct		SceeWhPx_Tools_RecursiveList(char *, _Whp_struct);
_Whp_struct		SceeWhPx_Tools_ExtList_Filtering(_Whp_struct);
_Whp_struct		SceeWhPx_Tools_WhpStruct_Init(_Whp_struct, int);
_Whp_struct		SceeWhPx_Tools_SortList(_Whp_struct);
_Whp_struct		SceeWhPx_Tools_Filelist(_Whp_struct, char *);
_Whp_struct		SceeWhPx_Tools_FillChecksumZero(char *, _Whp_struct);
int				SceeWhPx_Tools_Filelist_Dummy(char *);
int				SceeWhPx_Tools_CreateDirectory(char *);
int				SceeWhPx_Tools_ExtractPath(char *, char *);
int				SceeWhPx_Tools_ExtractFilename(char *, char *);
int				SceeWhPx_Tools_FixPath(char *, char *);
char			*SceeWhPx_Tools_DateTime_String();
unsigned long	SceeWhPx_Tools_CalcChecksum(char *, unsigned long);






