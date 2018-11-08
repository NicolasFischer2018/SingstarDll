#include "SceeWhPx_Dll.h"

int	SceeWhPx_TOC_Read_Dll(char *PAK, char *WhPx_TOC_TXT)
{
	_Whp_struct		WhPx_TOC;

	//Initialize TOC structure
	WhPx_TOC.filelist = malloc(sizeof(*WhPx_TOC.filelist) * 1);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);

	WhPx_TOC = SceeWhPx_TOC_Read(PAK, WhPx_TOC);

	SceeWhPx_CreateTXT(WhPx_TOC_TXT, WhPx_TOC);

	return 0;
}

int	SceeWhPx_Create_Dll(char *PAK, char *source, int type)
{
	_Whp_struct		WhPx_TOC;

	//Initialize TOC structure
	WhPx_TOC.filelist = malloc(sizeof(*WhPx_TOC.filelist) * 1);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);

	if (SceeWhPx_Create(PAK, WhPx_TOC, source, type) == -1)
		return -1;

	return 0;
}

int	SceeWhPx_File_Extract_Filename_Dll(char *PAK, char *filename, char *dest, int relative)
{
	_Whp_struct		WhPx_TOC;
	
	//Initialize TOC structure
	WhPx_TOC.filelist = malloc(sizeof(*WhPx_TOC.filelist) * 1);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);

	WhPx_TOC = SceeWhPx_TOC_Read(PAK, WhPx_TOC);
	if (SceeWhPx_File_Extract(WhPx_TOC, filename, dest, relative) == -1)
		return -1;
	return 0;
}

int	SceeWhPx_File_Extract_All_Dll(char *PAK, char *dest)
{
	_Whp_struct		WhPx_TOC;
	int				i;

	//Initialize TOC structure
	WhPx_TOC.filelist = malloc(sizeof(*WhPx_TOC.filelist) * 1);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);

	WhPx_TOC = SceeWhPx_TOC_Read(PAK, WhPx_TOC);
	for (i = 0; i != WhPx_TOC.nbrfile; i++)
		if (SceeWhPx_File_Extract(WhPx_TOC, WhPx_TOC.filelist[i].filename, dest, 0) == -1)
			return -1;
	return 0;
}

int main()
{
	_Whp_struct		WhPx_TOC;
	_Whp_struct		WhPx_TOC_W;

	int i;
	
	//Initialize TOC structure
	WhPx_TOC_W.filelist = malloc(sizeof(*WhPx_TOC_W.filelist) * 1);
	WhPx_TOC_W = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC_W, FULL_RESET);

	if (SceeWhPx_Create("E:\\Temp\\PACK_EE.PAK", WhPx_TOC_W, "E:\\Temp\\Temp\\", SceeWhPC) == -1)
		return -1;
	


	//Initialize TOC structure
	WhPx_TOC.filelist = malloc(sizeof(*WhPx_TOC.filelist) * 1);
	
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_TOC_Read("E:\\Temp\\PACK_EE.PAK", WhPx_TOC);
	return 0;
	//PACK_EE
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_TOC_Read("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PACK_EE.PAK", WhPx_TOC);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_Tools_FillChecksumZero("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PACK_EE.PAK", WhPx_TOC);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_TOC_Read("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PACK_EE.PAK", WhPx_TOC);
	
	//PAK_IOP0
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_TOC_Read("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PAK_IOP0.PAK", WhPx_TOC);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_Tools_FillChecksumZero("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PAK_IOP0.PAK", WhPx_TOC);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_TOC_Read("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PAK_IOP0.PAK", WhPx_TOC);
	
	//PAK_IOPM
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_TOC_Read("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PAK_IOPM.PAK", WhPx_TOC);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_Tools_FillChecksumZero("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PAK_IOPM.PAK", WhPx_TOC);
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);
	WhPx_TOC = SceeWhPx_TOC_Read("F:\\Temp\\Singstar\\Library\\ORIGINAL\\PAK_IOPM.PAK", WhPx_TOC);

	//WhPx_TOC = SceeWhPx_TOC_Read("F:\\Temp\\Singstar\\Library\\OUTPUT\\PACK_EE.PAK", WhPx_TOC);
	//WhPx_TOC = SceeWhPx_TOC_Read("I:\\PACK_EE.PAK", WhPx_TOC);
	return 0;
	for (i = 0; i != WhPx_TOC.nbrfile; i++){
		if (SceeWhPx_File_Extract(WhPx_TOC, WhPx_TOC.filelist[i].filename, "E:\\Temp\\Temp\\", 0) == -1)
			return -1;}
	return 0;
	

	//Initialize TOC structure
	WhPx_TOC_W.filelist = malloc(sizeof(*WhPx_TOC_W.filelist) * 1);
	WhPx_TOC_W = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC_W, FULL_RESET);

	if (SceeWhPx_Create("E:\\Temp\\PACK_EE.PAK", WhPx_TOC_W, "E:\\Temp\\Temp\\", 0) == -1)
		return -1;


	return 0;
}

