#include "PAKio.h"

WIN32_FIND_DATA		*fd;
size_t				pathlen = 0;
unsigned int		nbrofiles = 0;



_Whp_struct	SceeWhPx_Tools_WhpStruct_Init(_Whp_struct WhPx_TOC, int mode)
{
	int i;

	if (mode != FILE_RESET){
		WhPx_TOC.error = 0;
		WhPx_TOC.readed_TOC = 0;
		WhPx_TOC.hdrsize = 0;
		WhPx_TOC.nbrfile = 0;
		WhPx_TOC.PAK_file = malloc(1 * sizeof(char));
		memset(WhPx_TOC.PAK_file, 0, 1 * sizeof(char));
		memset(WhPx_TOC.format, 0, 9 * sizeof(char));
		memset(WhPx_TOC.date, 0, 9 * sizeof(char));
		memset(WhPx_TOC.time, 0, 9 * sizeof(char));
		memset(WhPx_TOC.StrCompilePath, 0, 128 * sizeof(char));
		for (i = 0; i != 31; i++)
			memset(WhPx_TOC.extlist[i], 0, 4 * sizeof(char));}

	WhPx_TOC.filelist = malloc(sizeof(*WhPx_TOC.filelist) * 1);

	if (mode != BASE_RESET){
		if (WhPx_TOC.nbrfile != 0){
			WhPx_TOC.filelist = malloc(sizeof(*WhPx_TOC.filelist) * WhPx_TOC.nbrfile); 
			for (i = 0; i != WhPx_TOC.nbrfile; i++){
				WhPx_TOC.filelist[i].filename = malloc(1 * sizeof(char));
				memset(WhPx_TOC.filelist[i].filename, 0, 1 * sizeof(char));
				WhPx_TOC.filelist[i].reduced_filename = malloc(1 * sizeof(char));
				memset(WhPx_TOC.filelist[i].reduced_filename, 0, 1 * sizeof(char));
				WhPx_TOC.filelist[i].offset = 0;
				WhPx_TOC.filelist[i].filename_len = 0;
				WhPx_TOC.filelist[i].reduced_filename_len = 0;
				WhPx_TOC.filelist[i].previous_letters = 0;
				WhPx_TOC.filelist[i].header_len = 0;
				WhPx_TOC.filelist[i].unknown = 0;
				WhPx_TOC.filelist[i].size = 0;
				WhPx_TOC.filelist[i].checksum = 0;
				WhPx_TOC.filelist[i].extension = 0;
				WhPx_TOC.filelist[i].increased_header_len = 0;
				if (i == 1000)
					i = i;
			}}}

	return WhPx_TOC;
}

_Whp_struct		SceeWhPx_Tools_FileList(_Whp_struct	WhPx_Temp, char *_path)
{
	HANDLE				fh;
	size_t				cnt;
	char				path[MAX_PATH];
	char				tmppath[MAX_PATH];
	char				tempfilename[MAX_PATH];
	char				tempdir[MAX_PATH];
	
	fd = malloc(sizeof(WIN32_FIND_DATA));

	SceeWhPx_Tools_FixPath(_path, path);
	if (pathlen == 0)
		pathlen = strlen(path);
	strcpy_s(tempdir, strlen(path) + 1, path);
	
	//Changing folder to destination and create needed folders
	SceeWhPx_Tools_CreateDirectory(tempdir);

	fh = FindFirstFile((LPCWSTR)"*", fd);
  
	if(fh != INVALID_HANDLE_VALUE)
	{
		do
		{
			//transformation from whar string to normal string
			wcstombs_s(&cnt, tempfilename, 255, fd->cFileName, wcslen(fd->cFileName));
			if(fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//Directory found
				if((strcmp(tempfilename, ".")) && (strcmp(tempfilename, "..")))
				{
					SceeWhPx_Tools_FixPath(_path,tmppath);
					strcat_s(tmppath, strlen(tmppath) + strlen(tempfilename) + 1, tempfilename);
					SceeWhPx_Tools_FixPath(tmppath,tmppath);
					//restart function for recursive mode
					SceeWhPx_Tools_FileList(WhPx_Temp, tmppath);
				}
			}
			else
			{
				//File found
				_getdcwd(_getdrive(), tempdir, _MAX_PATH);
				SceeWhPx_Tools_FixPath(tempdir, tempdir);
				if (strlen(tempdir) > pathlen)
				{
					WhPx_Temp.filelist[nbrofiles].filename = malloc((strlen(tempfilename) + (strlen(tempdir) - pathlen) + 1) * sizeof(char));
					strcpy_s(WhPx_Temp.filelist[nbrofiles].filename, strlen(tempdir) - pathlen + 1, tempdir + pathlen);
					strcat_s(WhPx_Temp.filelist[nbrofiles].filename, (strlen(tempfilename) + (strlen(tempdir) - pathlen) + 1) * sizeof(char), tempfilename);
				}
				else
				{
					WhPx_Temp.filelist[nbrofiles].filename = malloc(strlen(tempfilename) * sizeof(char));
					strcpy_s(WhPx_Temp.filelist[nbrofiles].filename, strlen(tempfilename) + 1, tempfilename);
				}
				WhPx_Temp.filelist[nbrofiles].size = (fd->nFileSizeHigh * MAXDWORD) + fd->nFileSizeLow;
				
				*(path + pathlen) = 0;
				strcpy_s(tempfilename, strlen(path) + 1, path);
				strcat_s(tempfilename, (strlen(WhPx_Temp.filelist[nbrofiles].filename) + (strlen(path)) + 1) * sizeof(char), WhPx_Temp.filelist[nbrofiles].filename);
				WhPx_Temp.filelist[nbrofiles].checksum = SceeWhPx_Tools_CalcChecksum(tempfilename, WhPx_Temp.filelist[nbrofiles].size);
				printf("%s\n", WhPx_Temp.filelist[nbrofiles].filename);
				nbrofiles++;
			}
		}
		while(FindNextFile(fh,fd));
	}
	
	FindClose(fh);
	_chdir("..");
	
	return WhPx_Temp;
}

int			SceeWhPx_Tools_FileList_Dummy(char *_path)
{
	HANDLE				fh;
	size_t				cnt;
	char				path[MAX_PATH];
	char				tmppath[MAX_PATH];
	char				tempfilename[MAX_PATH];
	char				tempdir[MAX_PATH];
	
	fd = malloc(sizeof(WIN32_FIND_DATA));

	SceeWhPx_Tools_FixPath(_path, path);
	if (pathlen == 0)
		pathlen = strlen(path);
	strcpy_s(tempdir, strlen(path) + 1, path);
	
	//Changing folder to destination and create needed folders
	SceeWhPx_Tools_CreateDirectory(tempdir);

	fh = FindFirstFile((LPCWSTR)"*", fd);
  
	if(fh != INVALID_HANDLE_VALUE)
	{
		do
		{
			//transformation from whar string to normal string
			wcstombs_s(&cnt, tempfilename, 255, fd->cFileName, wcslen(fd->cFileName));
			if(fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//Directory found
				if((strcmp(tempfilename, ".")) && (strcmp(tempfilename, "..")))
				{
					SceeWhPx_Tools_FixPath(_path,tmppath);
					strcat_s(tmppath, strlen(tmppath) + strlen(tempfilename) + 1, tempfilename);
					SceeWhPx_Tools_FixPath(tmppath,tmppath);
					//restart function for recursive mode
					SceeWhPx_Tools_FileList_Dummy(tmppath);
				}
			}
			else
			{
				//File found
				_getdcwd(_getdrive(), tempdir, _MAX_PATH);
				SceeWhPx_Tools_FixPath(tempdir, tempdir);
				nbrofiles++;
			}
		}
		while(FindNextFile(fh,fd));
	}
	
	FindClose(fh);
	_chdir("..");
	
	return 0;
}

int			SceeWhPx_Tools_CreateDirectory(char *path)
{
	char	tempdir[MAX_PATH];
	char	tempfixdir[MAX_PATH];
	int		i, j = 0;

	if (strlen(path) >= MAX_PATH)
		return -1;
	//Changing folder to destination and create needed folders
	//printf("Dir was %s ", _getdcwd(_getdrive(), tempdir, MAX_PATH ));
	memset(tempdir, 0, MAX_PATH * sizeof(char));
	memset(tempfixdir, 0, MAX_PATH * sizeof(char));
	SceeWhPx_Tools_FixPath(path, tempfixdir);
	for (i = 0; tempfixdir[i] != 0; i++){
		tempdir[j] = tempfixdir[i];
		if (tempdir[j] == '\\'){
			if (_chdir(tempdir) == -1){
				if(_mkdir(tempdir) == -1)
					return -1;
				else
					_chdir(tempdir);}
			memset(tempdir, 0, MAX_PATH * sizeof(char));
			j = -1;}
		j++;}
	//printf("and now is %s\n", _getdcwd(_getdrive(), tempdir, MAX_PATH ));

	return 0;
}

int			SceeWhPx_Tools_ExtractFilename(char *inpath, char *outpath)
{
	char	*addr;

	//Extract filename string
	if (strlen(inpath) >= MAX_PATH)
		return -1;
	addr = inpath;
	while(*inpath++);
	while(inpath != addr){
		if (*inpath == '\\'){
			inpath++;
			break;}
		inpath--;}
	
	strcpy_s(outpath, strlen(inpath) + 1, inpath);

	return 0;
}

int			SceeWhPx_Tools_ExtractPath(char *inpath, char *outpath)
{
	char	*addr;
	int		count;

	//Extract folder string from filename
	if (strlen(inpath) >= MAX_PATH)
		return -1;
	addr = inpath;
	count = 0;
	while(*inpath++);
	while(*inpath-- != '\\')
		count++;
	count = count - 2;
	inpath = addr;
	strncpy_s(outpath, strlen(inpath) - count + 1, inpath, strlen(inpath) - count);
	SceeWhPx_Tools_FixPath(outpath, outpath);
	
	return 0;
}

int			SceeWhPx_Tools_FixPath(char *inpath, char *outpath)
{
  int   n=0;

  strcpy_s(outpath,strlen(inpath) + 1, inpath);

  while(inpath[n]) n++;

  if(inpath[n-1] != '\\')
  {
    strcat_s(outpath, strlen(outpath) + 2, "\\");
    return 1;
  }

  return 0;
}

_Whp_struct	SceeWhPx_Tools_SortList(_Whp_struct	WhPx_Temp)
{	
	int				i, j, count;
	char			*addr;
	unsigned long	size;
	
	if (WhPx_Temp.nbrfile == 0)
		return WhPx_Temp;

	count = 1;
	while(count){
		count = 0;
		for (i = 0; i != WhPx_Temp.nbrfile - 1; i++){
			for (j = 0; j != strlen(WhPx_Temp.filelist[i].filename) || j != strlen(WhPx_Temp.filelist[i + 1].filename); j++){
				if (*(WhPx_Temp.filelist[i].filename + j) < *(WhPx_Temp.filelist[i + 1].filename + j))
					break;
				if (*(WhPx_Temp.filelist[i].filename + j) > *(WhPx_Temp.filelist[i + 1].filename + j)){
					//printf("%s is higher than %s\n", WhPx_Temp.filelist[i].filename, WhPx_Temp.filelist[i + 1].filename);
					addr = WhPx_Temp.filelist[i].filename;
					size = WhPx_Temp.filelist[i].size;
					WhPx_Temp.filelist[i].filename = WhPx_Temp.filelist[i + 1].filename;
					WhPx_Temp.filelist[i].size = WhPx_Temp.filelist[i + 1].size;
					WhPx_Temp.filelist[i + 1].filename = addr;
					WhPx_Temp.filelist[i + 1].size = size;
					count++;
					break;}}}}
	return WhPx_Temp;
}

_Whp_struct	SceeWhPx_Tools_RecursiveList(char *path, _Whp_struct WhPx_TOC)
{
	nbrofiles = 0;
	pathlen = 0;

	SceeWhPx_Tools_FileList_Dummy(path);
	
	if (nbrofiles == 0){
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
	else {
		//Allocating memory for filename structure
		WhPx_TOC.nbrfile = nbrofiles;
		WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FILE_RESET);

		nbrofiles = 0;
		WhPx_TOC = SceeWhPx_Tools_FileList(WhPx_TOC, path);}


	WhPx_TOC.nbrfile = nbrofiles;
	WhPx_TOC = SceeWhPx_Tools_SortList(WhPx_TOC);
	
	if (WhPx_TOC.nbrfile == 0)
		WhPx_TOC.error = 1;

	printf("Total number of file found: %d\n", WhPx_TOC.nbrfile);
	
	return WhPx_TOC;
}

_Whp_struct	SceeWhPx_Tools_ExtList_Filtering(_Whp_struct WhPx_TOC)
{
	int		i, j, k, count, exist;
	char	tempfilename[MAX_PATH];
	char	extension[255];

	count = 1;
	for (i = 0; i != WhPx_TOC.nbrfile; i++)
	{
		exist = 0;
		k = -1;
		strcpy_s(tempfilename, strlen(WhPx_TOC.filelist[i].filename) + 1, WhPx_TOC.filelist[i].filename);
		
		for (j = (int) strlen(tempfilename); tempfilename[j] != '.'; j--)
			k++;
		
		for (j = 0; j != k && j != 3; j++)
			extension[j] = tempfilename[(strlen(tempfilename) - k) + j];

		extension[j] = 0;

		for (j = 0; j != 31; j++)
			if (strcmp(extension, WhPx_TOC.extlist[j]) == 0)
			{
				exist = 1;
				break;
			}

		if (exist == 0)
		{
			strcpy_s(WhPx_TOC.extlist[count], strlen(extension) + 1, extension);
			count++;
		}
	}

	return WhPx_TOC;
}

char	*SceeWhPx_Tools_DateTime_String()
{
	__time32_t		ltime;
	struct tm		ctime;
	char			*result;
	char			value[4];
	int				year;

	result = malloc(21 * sizeof(char));
	memset(result, 0, 21 * sizeof(char));

	_time32(&ltime);
	_localtime32_s(&ctime, &ltime);
	
	//Writing day
	memset(value, 0, 4 * sizeof(char));
	_itoa_s(ctime.tm_mday, value, 3, 10);
	if (strlen(value) < 2){
		value[1] = value[0];
		value[0] = '0';
		value[2] = 0;}
	
	strcpy_s(result, 2, "[");
	strcat_s(result,  strlen(result) + strlen(value) + 1, value);
	strcat_s(result,  strlen(result) + 2, "/");
	
	//Writing Month
	memset(value, 0, 4 * sizeof(char));
	_itoa_s(ctime.tm_mon + 1, value, 3, 10);
	if (strlen(value) < 2){
		value[1] = value[0];
		value[0] = '0';
		value[2] = 0;}

	strcat_s(result,  strlen(result) + strlen(value) + 1, value);
	strcat_s(result,  strlen(result) + 2, "/");

	//Writing Year
	memset(value, 0, 4 * sizeof(char));
	if (ctime.tm_year >= 100)
		year = ctime.tm_year - 100;
	else
		year = ctime.tm_year;
	
	_itoa_s(year, value, 3, 10);
	if (strlen(value) < 2){
		value[1] = value[0];
		value[0] = '0';
		value[2] = 0;}

	strcat_s(result,  strlen(result) + strlen(value) + 1, value);
	strcat_s(result,  strlen(result) + 2, ",");

	//Writing Hour
	memset(value, 0, 4 * sizeof(char));
	_itoa_s(ctime.tm_hour, value, 3, 10);
	if (strlen(value) < 2){
		value[1] = value[0];
		value[0] = '0';
		value[2] = 0;}

	strcat_s(result,  strlen(result) + strlen(value) + 1, value);
	strcat_s(result,  strlen(result) + 2, ":");

	//Writing Minute
	memset(value, 0, 4 * sizeof(char));
	_itoa_s(ctime.tm_min, value, 3, 10);
	if (strlen(value) < 2){
		value[1] = value[0];
		value[0] = '0';
		value[2] = 0;}

	strcat_s(result,  strlen(result) + strlen(value) + 1, value);
	strcat_s(result,  strlen(result) + 2, ":");
	
	//Writing Seconds
	memset(value, 0, 4 * sizeof(char));
	_itoa_s(ctime.tm_sec, value, 3, 10);
	if (strlen(value) < 2){
		value[1] = value[0];
		value[0] = '0';
		value[2] = 0;}

	strcat_s(result,  strlen(result) + strlen(value) + 1, value);
	strcat_s(result,  strlen(result) + 2, "]");
	
	return result;
}

_Whp_struct		SceeWhPx_Tools_FillChecksumZero(char *PAK, _Whp_struct WhPx_TOC)
{
	FILE			*handle;
	
	int				i;
	unsigned int	letters;
		
	//Initialize TOC structure
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);

	//Open File for Binary Access
	if(fopen_s(&handle, PAK, "rb+")){
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
		
	//Filename of PAK file
	WhPx_TOC.PAK_file = malloc((strlen(PAK)  + 1) * sizeof (char));
	strcpy_s(WhPx_TOC.PAK_file, (strlen(PAK)  + 1) * sizeof (char), PAK);

	//Read File format
	if (fread(WhPx_TOC.format, sizeof(char), 8, handle) != 8){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}

	//Check for valid data
	if ((strncmp(WhPx_TOC.format, "SceeWhPk", 8) != 0) && (strncmp(WhPx_TOC.format, "SceeWhPC", 8) != 0)){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}

	//Skip Unecessary Data
	_fseeki64(handle, 24, SEEK_CUR);
	
	//Read number of files
	if (fread(&WhPx_TOC.nbrfile, sizeof(long), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}	

	//Seek to beginning of header
	_fseeki64(handle, 408 , SEEK_SET);

	//Allocating memory for filename structure
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FILE_RESET);

	for (i = 0; i != WhPx_TOC.nbrfile; i++)
	{
		_fseeki64(handle,  4, SEEK_CUR);
				
		//Read number of character to take
		memset(&letters, 0, sizeof(unsigned int));
		if (fread(&letters, sizeof(unsigned char), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}	
		
		_fseeki64(handle,  6, SEEK_CUR);
		
		//Write Zero checksum
		WhPx_TOC.filelist[i].checksum = 0;
		if (strncmp(WhPx_TOC.format, "SceeWhPk", 8)){
			if (fwrite(&WhPx_TOC.filelist[i].checksum, sizeof(unsigned long), 1, handle) != 1){
			fclose(handle);
			WhPx_TOC.error = -1;
			return WhPx_TOC;}
		}
		
		_fseeki64(handle,  letters, SEEK_CUR);
	}

	if (fclose(handle)){
		WhPx_TOC.error = -1;
		return WhPx_TOC;}

	WhPx_TOC.readed_TOC = 1;

	return WhPx_TOC;
}

unsigned long		SceeWhPx_Tools_CalcChecksum(char *filename, unsigned long size)
{
	FILE			*handle;
	unsigned long	checksum, tmpvalue;
	int				i, div, dif;

	
		
	checksum = 0;
	tmpvalue = 0;

	//Open File for Binary Access
	if(fopen_s(&handle, filename, "rb"))
		return 0;
		
	
	div = size / sizeof(unsigned long);
	dif = size % sizeof(unsigned long); 

	for (i = 0; i != div; i++){
	if (fread(&tmpvalue, sizeof(unsigned long), 1, handle) != 1){
		fclose(handle);
		return 0;}
	checksum = checksum ^ tmpvalue;}
	
	tmpvalue = 0;
	if (fread(&tmpvalue, sizeof(unsigned char), dif, handle) != dif){
		fclose(handle);
		return 0;}
	checksum = checksum ^ tmpvalue;

	checksum = checksum ^ size;

	printf("Checksum: 0x%08.8X ", checksum);

	if (fclose(handle))
		return 0;

	

	return checksum;
}