//SceeWhPx Creation from scratch

#include "PAKio.h"


int		SceeWhPx_Create(char *PAK, _Whp_struct WhPx_TOC, char *source, int type)
{
	FILE	*handle;
	FILE	*filesource;
	FILE	*output;
	
	int				i, j, div, dif;
	int				k = 0, count = 0, count2 = 0;
	char			buf;
	char			tempfilename[MAX_PATH];
	char			tempdir[MAX_PATH];
	char			tempdata[BUFFER_SIZE];
	char			*addr;
	char			*filename = "f:\\write.csv";
	unsigned char	offchar[3];
	
	//Initialize TOC structure
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);

	//Initialize String data
	memset(tempfilename, 0, MAX_PATH * sizeof(char));
	memset(tempdir, 0, MAX_PATH * sizeof(char));
	memset(tempdata, 0, BUFFER_SIZE * sizeof(char));

	//Extract path from destination filename
	SceeWhPx_Tools_ExtractPath(PAK, tempdir);
	//Change and create necessary folders for this resulting destination path
	SceeWhPx_Tools_CreateDirectory(tempdir);

	//File listing directory we want to PAK
	WhPx_TOC = SceeWhPx_Tools_RecursiveList(source, WhPx_TOC);
	if (WhPx_TOC.error != 0)
		return -1;

	//Printf files found
	//for (i = 0; i != WhPx_TOC.nbrfile; i++){
	//	printf("%s\n", WhPx_TOC.filelist[i].filename);}
	
	//Copy filename of PAK file into TOC struct
	WhPx_TOC.PAK_file = malloc((strlen(PAK)  + 1) * sizeof (char));
	strcpy_s(WhPx_TOC.PAK_file, (strlen(PAK)  + 1) * sizeof (char), PAK);

	//Open Output PAK file for writing
	if(fopen_s(&handle, WhPx_TOC.PAK_file, "wb"))
		return -1;
	printf("File Open OK!\n");
	
	//Write format header
	if (type==SceeWhPk){
		strcpy_s(WhPx_TOC.format, 9 * sizeof(char), "SceeWhPk");
		if (fwrite("SceeWhPk", sizeof(char), 8, handle) != 8){
			fclose(handle);
			return -1;}}
	else {
		strcpy_s(WhPx_TOC.format, 9 * sizeof(char), "SceeWhPC");
		if (fwrite("SceeWhPC", sizeof(char), 8, handle) != 8){
			fclose(handle);
			return -1;}}

	//Write Time and Hour
	addr = SceeWhPx_Tools_DateTime_String();
	if (fwrite(addr, sizeof(char), strlen(addr), handle) != strlen(addr)){
			fclose(handle);
			return -1;}
	
	//Write 0x00
	_fseeki64(handle, 1, SEEK_CUR);

	//We can't write header size yet so write 0x00 for 4 bytes 
	_fseeki64(handle, 4, SEEK_CUR);

	//Write nbr of files
	if (fwrite(&WhPx_TOC.nbrfile, sizeof(long), 1, handle) != 1){
			fclose(handle);
			return -1;}

	//Write Compile Path
	strcpy_s(WhPx_TOC.StrCompilePath, strlen(source) + 1, source);
	SceeWhPx_Tools_FixPath(WhPx_TOC.StrCompilePath, WhPx_TOC.StrCompilePath);
	if (strlen(WhPx_TOC.StrCompilePath) <= 128){
		if (fwrite(WhPx_TOC.StrCompilePath, sizeof(char), strlen(WhPx_TOC.StrCompilePath), handle) != strlen(WhPx_TOC.StrCompilePath)){
				fclose(handle);
				return -1;}
		_fseeki64(handle, (128 - (long) strlen(WhPx_TOC.StrCompilePath)), SEEK_CUR);}
	else {
		if (fwrite(WhPx_TOC.StrCompilePath, sizeof(char), 128, handle) != 128){
				fclose(handle);
				return -1;}}
	
	//Write "Unecessary data" (112 bytes)
	_fseeki64(handle, 112, SEEK_CUR);
	
	////Data
	//
	////0xFFFFFFFF
	//buf = 0xFF;
	//for (i = 0; i != 4; i++)
	//	if (fwrite(&buf, sizeof(char), 1, handle) != 1){
	//				fclose(handle);
	//				return -1;}
	////0x00000000
	//_fseeki64(handle, 4, SEEK_CUR);
	////0xFFFFFFFF
	//buf = 0xFF;
	//for (i = 0; i != 4; i++)
	//	if (fwrite(&buf, sizeof(char), 1, handle) != 1){
	//				fclose(handle);
	//				return -1;}
	////0x1A
	//buf = 0x1A;
	//if (fwrite(&buf, sizeof(char), 1, handle) != 1){
	//			fclose(handle);
	//			return -1;}
	////0x000000
	//_fseeki64(handle, 3, SEEK_CUR);
	////0xFFFFFFFF
	//buf = 0xFF;
	//for (i = 0; i != 4; i++)
	//	if (fwrite(&buf, sizeof(char), 1, handle) != 1){
	//				fclose(handle);
	//				return -1;}
	////0x93
	//buf = 0x93;
	//if (fwrite(&buf, sizeof(char), 1, handle) != 1){
	//			fclose(handle);
	//			return -1;}
	////0x000000
	//_fseeki64(handle, 3, SEEK_CUR);

	////Write "Unecessary data" (88 bytes)
	//_fseeki64(handle, 88, SEEK_CUR);



	//Write extension list
	count = 0;
	WhPx_TOC = SceeWhPx_Tools_ExtList_Filtering(WhPx_TOC);
	for (i = 1; WhPx_TOC.extlist[i][0] != 0; i++)
	{
		if (fwrite(WhPx_TOC.extlist[i], sizeof(char), strlen(WhPx_TOC.extlist[i]), handle) != strlen(WhPx_TOC.extlist[i])){
			fclose(handle);
			return -1;}
		//Write 0x00
		for (j = 0; j != (4 - strlen(WhPx_TOC.extlist[i])); j++)
			_fseeki64(handle, 1, SEEK_CUR);
		count = count + (unsigned int) strlen(WhPx_TOC.extlist[i]) + 1;
	}

	//Skip remaining bytes after writing ext list
	_fseeki64(handle, 124 - count, SEEK_CUR);

	//BIG Loop begining
	for (i = 0; i != WhPx_TOC.nbrfile; i++)
	{
		//Scan files to strip extension
		for (j = 1; WhPx_TOC.extlist[j][0] != 0; j++){
			if (strcmp((WhPx_TOC.filelist[i].filename + (strlen(WhPx_TOC.filelist[i].filename) - 3)), WhPx_TOC.extlist[j]) == 0)
				WhPx_TOC.filelist[i].extension = j;}

		//remove extension from filename
		addr = WhPx_TOC.filelist[i].filename;
		for (j = (unsigned int) strlen(addr); *(addr + j) != '.'; j--);
		*(addr + j) = 0;
		
		//Scan files to reduce filenames
		if (i == 0)
		{
			WhPx_TOC.filelist[0].reduced_filename = malloc(strlen(WhPx_TOC.filelist[0].filename) + 1);
			strncpy_s(WhPx_TOC.filelist[0].reduced_filename, strlen(WhPx_TOC.filelist[0].filename) + 1, WhPx_TOC.filelist[0].filename, strlen(WhPx_TOC.filelist[0].filename));
		}
		else
		{
			if (strcmp(WhPx_TOC.filelist[i].filename, WhPx_TOC.filelist[i - 1].filename) != 0)
			{
				for (j = 0; strncmp(WhPx_TOC.filelist[i].filename, WhPx_TOC.filelist[i - 1].filename, j) == 0; j++);
				j--;
			}
			else
				j = (unsigned int) strlen(WhPx_TOC.filelist[i].filename);
			WhPx_TOC.filelist[i].reduced_filename = malloc(strlen(WhPx_TOC.filelist[i].filename) - j + 1);
			strncpy_s(WhPx_TOC.filelist[i].reduced_filename, strlen(WhPx_TOC.filelist[i].filename) - j + 1, WhPx_TOC.filelist[i].filename + j, strlen(WhPx_TOC.filelist[i].filename) - j);
			WhPx_TOC.filelist[i].previous_letters = j;
		}
		
		//Prepare reduced filename len
		WhPx_TOC.filelist[i].reduced_filename_len = (unsigned int) strlen(WhPx_TOC.filelist[i].reduced_filename) + 1;

		//Calculate and Write header len at beginning of file
		if (type==SceeWhPC)
			WhPx_TOC.filelist[i].header_len = 15 + WhPx_TOC.filelist[i].reduced_filename_len;
		else
			WhPx_TOC.filelist[i].header_len = 11 + WhPx_TOC.filelist[i].reduced_filename_len;

		WhPx_TOC.hdrsize = WhPx_TOC.hdrsize + WhPx_TOC.filelist[i].header_len;
		
		//Write filename len
		WhPx_TOC.filelist[i].filename_len = (unsigned int) strlen(WhPx_TOC.filelist[i].filename) + 1;
	}
	
	//Finalize and write hdrsize
	WhPx_TOC.hdrsize = WhPx_TOC.hdrsize + 15;
	_fseeki64(handle, 28, SEEK_SET);
	if (fwrite(&WhPx_TOC.hdrsize, sizeof(long), 1, handle) != 1){
			fclose(handle);
			return -1;}
	_fseeki64(handle, 400 , SEEK_SET);

	//Scan files to define offset of files in PAK file
	if (WhPx_TOC.hdrsize <= OFFSET_SIZE)
		WhPx_TOC.filelist[0].offset = 1;
	else
		WhPx_TOC.filelist[0].offset = (2 * WhPx_TOC.hdrsize) / OFFSET_SIZE;

	for (i = 1; i != WhPx_TOC.nbrfile; i++){
		if (WhPx_TOC.filelist[i - 1].size >= OFFSET_SIZE){
			if ((WhPx_TOC.filelist[i - 1].size % OFFSET_SIZE) != 0)
				WhPx_TOC.filelist[i].offset = WhPx_TOC.filelist[i - 1].offset + (WhPx_TOC.filelist[i - 1].size / OFFSET_SIZE) + 1;
			else
				WhPx_TOC.filelist[i].offset = WhPx_TOC.filelist[i - 1].offset + (WhPx_TOC.filelist[i - 1].size / OFFSET_SIZE);}	
		else
			WhPx_TOC.filelist[i].offset = WhPx_TOC.filelist[i - 1].offset + 1;}

	//Calculate Unknwown field (header len of filename reduction)
	for (i = 0; i != WhPx_TOC.nbrfile - 1; i++){
		if (WhPx_TOC.filelist[i].previous_letters >= WhPx_TOC.filelist[i + 1].filename_len  + 1)
			k = WhPx_TOC.filelist[i + 1].filename_len - 1;
		else {
			if (WhPx_TOC.filelist[i].previous_letters - 1 != 0)
				k = WhPx_TOC.filelist[i].previous_letters;
			else
				k = 1;}
		if (WhPx_TOC.filelist[i].previous_letters == 0)
			k = 1;
		for (k; k != WhPx_TOC.filelist[i + 1].filename_len + 1; k++){	
			count = 0;
			for (j = i; j != WhPx_TOC.nbrfile; j++){
				if (strncmp(WhPx_TOC.filelist[i].filename, WhPx_TOC.filelist[j].filename, k) != 0)
					break;
				count = count + WhPx_TOC.filelist[j].header_len;
				if (WhPx_TOC.filelist[i].reduced_filename_len == 1 && WhPx_TOC.filelist[i + 1].reduced_filename_len == 1)
					break;
			}
			if (count2 < count)
				count2 = count;	
		}
		count = 0;
		for (j = i; j != WhPx_TOC.nbrfile; j++)
			count = count + WhPx_TOC.filelist[j].header_len;
		if (count2 != count)
			WhPx_TOC.filelist[i].unknown = count2;
		else
			WhPx_TOC.filelist[i].unknown = 0;
		//printf("Calculate unknown value for filename: %s: %d\n", WhPx_TOC.filelist[i].filename, WhPx_TOC.filelist[i].unknown);
		count = 0;
		count2 = 0;
	}

	//Skip 8 bytes
	_fseeki64(handle, 8, SEEK_CUR);

	//Begin to write TOC
	for (i = 0; i != WhPx_TOC.nbrfile; i++){
		//Split offset in 3 bytes
		offchar[2] = (unsigned char)(WhPx_TOC.filelist[i].offset / (1 << 16));
		if (offchar[2] == 0)
			offchar[1] = (unsigned char)(WhPx_TOC.filelist[i].offset / (1 << 8));
		else {
			//offlong = (WhPx_TOC.filelist[i].offset - (offchar[2] * (1 << 16))) / (1 << 8);
			offchar[1] = (unsigned char)((WhPx_TOC.filelist[i].offset - (offchar[2] * (1 << 16))) / (1 << 8));
		}
		offchar[0] = (unsigned char)(WhPx_TOC.filelist[i].offset - ((offchar[2] * (1 << 16)) + (offchar[1] * (1 << 8))));
		
		//Write offset of current file
		//printf("%d:", WhPx_TOC.filelist[i].offset);
		for (j = 0; j != 3; j++){
			if (fwrite(&offchar[j], sizeof(char), 1, handle) != 1){
				fclose(handle);
				return -1;}}
		//printf("%d,", offchar[j]);}
		//printf("%s\n", WhPx_TOC.filelist[i].filename);

		//Write previous letters
		if (fwrite(&WhPx_TOC.filelist[i].previous_letters, sizeof(char), 1, handle) != 1){
			fclose(handle);
			return -1;}
		//Write header filename len
		if (fwrite(&WhPx_TOC.filelist[i].reduced_filename_len, sizeof(char), 1, handle) != 1){
			fclose(handle);
			return -1;}
		//Write horizontal header size for char repetition (skip for the moment)
		if (fwrite(&WhPx_TOC.filelist[i].unknown, sizeof(unsigned short), 1, handle) != 1){
			fclose(handle);
			return -1;}
		//Write size of current file
		if (fwrite(&WhPx_TOC.filelist[i].size, sizeof(long), 1, handle) != 1){
			fclose(handle);
			return -1;}
		//Write checksum if SceeWhPC format
		if (type==SceeWhPC){
			if (fwrite(&WhPx_TOC.filelist[i].checksum, sizeof(long), 1, handle) != 1){
			fclose(handle);
			return -1;}}
		//Write reduced filename
		if (fwrite(WhPx_TOC.filelist[i].reduced_filename, sizeof(char), strlen(WhPx_TOC.filelist[i].reduced_filename), handle) != strlen(WhPx_TOC.filelist[i].reduced_filename)){
			fclose(handle);
			return -1;}
		//Write value for filename extension
		if (fwrite(&WhPx_TOC.filelist[i].extension, sizeof(char), 1, handle) != 1){
			fclose(handle);
			return -1;}
	}
	
	//Add back extension
	for (i = 0; i != WhPx_TOC.nbrfile; i++)
		*(WhPx_TOC.filelist[i].filename + strlen(WhPx_TOC.filelist[i].filename)) = '.';

	//Write Debug Log
	//memset(tempfilename, 0, MAX_PATH * sizeof(char));
	//strcpy_s(tempfilename, strlen(filename) + 1, filename);
	///* for (i = (unsigned int) strlen(filename) - 1; tempfilename[i] != '\\'; i--);
	//tempfilename[i + 1] = 0;
	//strcat_s(tempfilename, (strlen(tempfilename) + strlen("result-write.csv") + 1) * sizeof(char), "result-write.csv");*/
	//if(fopen_s(&output, tempfilename, "w"))
	//	return -1;
	//fprintf(output, "offset,header_len,filename,reduced_filename,previous_letters,filename_len,reduced_filename_len,unknown,size,checksum,extension\n");
	//for (i = 0; i != WhPx_TOC.nbrfile; i++)
	//	fprintf(output, "%d,%d,%s,%s,%d,%d,%d,%d,%d,%d,%d\n", WhPx_TOC.filelist[i].offset, WhPx_TOC.filelist[i].header_len, WhPx_TOC.filelist[i].filename, 
	//	WhPx_TOC.filelist[i].reduced_filename, WhPx_TOC.filelist[i].previous_letters, WhPx_TOC.filelist[i].filename_len,
	//	WhPx_TOC.filelist[i].reduced_filename_len, WhPx_TOC.filelist[i].unknown, WhPx_TOC.filelist[i].size, WhPx_TOC.filelist[i].checksum, WhPx_TOC.filelist[i].extension);

	//if (fclose(output))
	//	return -1;
	

	//Write files
	for (i = 0; i != WhPx_TOC.nbrfile; i++){
		//Set write file position
		_fseeki64(handle, WhPx_TOC.filelist[i].offset * OFFSET_SIZE , SEEK_SET);
		//Construct filename with path
		memset(tempfilename, 0, MAX_PATH * sizeof(char));
		strcpy_s(tempfilename, strlen(WhPx_TOC.StrCompilePath) + 1, WhPx_TOC.StrCompilePath);
		strcat_s(tempfilename, strlen(WhPx_TOC.StrCompilePath) + strlen(WhPx_TOC.filelist[i].filename) + 1, WhPx_TOC.filelist[i].filename);
		printf("Filename to inject: %s\n", tempfilename);
		//Open Source file on disk
		if(fopen_s(&filesource, tempfilename, "rb"))
			return -1;
		//Read data from source file
		div = WhPx_TOC.filelist[i].size / BUFFER_SIZE;
		dif = WhPx_TOC.filelist[i].size % BUFFER_SIZE;
		for (j = 0; j != div; j++)
		{
			memset(tempdata, 0, BUFFER_SIZE * sizeof(char));
			if (fread(tempdata, sizeof(char), BUFFER_SIZE, filesource)!= BUFFER_SIZE){
					fclose(filesource);
					return -1;}	
			if (fwrite(tempdata, sizeof(char), BUFFER_SIZE, handle)!= BUFFER_SIZE){
					fclose(handle);
					return -1;}	
		}
		memset(tempdata, 0, BUFFER_SIZE * sizeof(char));
		if (fread(tempdata, sizeof(char), dif, filesource)!= dif){
				fclose(filesource);
				return -1;}	
		if (fwrite(tempdata, sizeof(char), dif, handle)!= dif){
				fclose(handle);
				return -1;}	
		//Close Source file on disk
		if (fclose(filesource))
			return -1;
	}

	//Close PAK file
	if (fclose(handle))
		return -1;
	printf("File Close OK!\n");
	
	WhPx_TOC.readed_TOC = 1;
	
	return 0;
}