#include "PAKio.h"

_Whp_struct	SceeWhPx_TOC_Read(char *PAK, _Whp_struct WhPx_TOC)
{
	FILE			*handle;
	FILE			*output;

	int				i, j;
	unsigned int	letters, prevletters, extension;
	char			*filename = "F:\\write.csv";
	char			tempfilename[MAX_PATH];
	char			tempdir[MAX_PATH];
	char			tempdata[BUFFER_SIZE];
	unsigned long	first_byte, second_byte, third_byte;
	unsigned char	singlechar[3];
	
	//Initialize TOC structure
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FULL_RESET);

	//Open File for Binary Access
	if(fopen_s(&handle, PAK, "rb")){
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
	//printf("\nPAK file open for read-binary access\n");

	memset(tempfilename, 0, MAX_PATH * sizeof(char));
	memset(tempdir, 0, MAX_PATH * sizeof(char));
	memset(tempdata, 0, BUFFER_SIZE * sizeof(char));
	
	//Filename of PAK file
	WhPx_TOC.PAK_file = malloc((strlen(PAK)  + 1) * sizeof (char));
	strcpy_s(WhPx_TOC.PAK_file, (strlen(PAK)  + 1) * sizeof (char), PAK);

	//Read File format
	if (fread(WhPx_TOC.format, sizeof(char), 8, handle) != 8){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
	_fseeki64(handle, 1 , SEEK_CUR);

	//Read Date
	if (fread(WhPx_TOC.date, sizeof(char), 8, handle) != 8){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
	_fseeki64(handle, 1 , SEEK_CUR);

	//Read Time
	if (fread(WhPx_TOC.time, sizeof(char), 8, handle) != 8){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
	_fseeki64(handle, 2 , SEEK_CUR);

	//Check for valid data
	if ((strncmp(WhPx_TOC.format, "SceeWhPk", 8) != 0) && (strncmp(WhPx_TOC.format, "SceeWhPC", 8) != 0)){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
	
	//Read Header size
	if (fread(&WhPx_TOC.hdrsize, sizeof(long), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}	
	
	//Read number of files
	if (fread(&WhPx_TOC.nbrfile, sizeof(long), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}	

	//Read Compile Path
	if (fread(WhPx_TOC.StrCompilePath, sizeof(char), 128, handle) != 128){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}

	//Skip Unecessary Data
	_fseeki64(handle, 112 , SEEK_CUR);
	
	//Read extension list
	for (i = 1; i != 32; i++)
		if (fread(WhPx_TOC.extlist[i], sizeof(char), 4, handle) != 4){
			fclose(handle);
			WhPx_TOC.error = -1;
			return WhPx_TOC;}
	
	//Seek to beginning of header
	_fseeki64(handle, 400 , SEEK_SET);

	//Skip Unecessary Data
	_fseeki64(handle, 8 , SEEK_CUR);
	
	//printf("Format %s detected\n", WhPx_TOC.format);
	//printf("Created the %s at %s\n", WhPx_TOC.date, WhPx_TOC.time);
	//printf("There's %d files and header size is %d bytes\n", WhPx_TOC.nbrfile, WhPx_TOC.hdrsize); 
	//printf("Compiled from: %s\n", WhPx_TOC.StrCompilePath);
	
	//Allocating memory for filename structure
	WhPx_TOC = SceeWhPx_Tools_WhpStruct_Init(WhPx_TOC, FILE_RESET);

	for (i = 0; i != WhPx_TOC.nbrfile; i++)
	{
		//Read offset of i file
		for (j = 0; j != 3; j++)
			if (fread(&singlechar[j], sizeof(unsigned char), 1, handle) != 1){
			fclose(handle);
			WhPx_TOC.error = -1;
			return WhPx_TOC;}
		third_byte = singlechar[2] << 16;
		second_byte = singlechar[1] << 8;
		first_byte = singlechar[0];
		WhPx_TOC.filelist[i].offset = first_byte + second_byte + third_byte;
		
		//Read number of character to take from previous filename
		memset(&prevletters, 0, sizeof(unsigned int));
		memset(&letters, 0, sizeof(unsigned int));
		if (fread(&prevletters, sizeof(unsigned char), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
		WhPx_TOC.filelist[i].previous_letters = prevletters;

		//Read number of character to take
		if (fread(&letters, sizeof(unsigned char), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}	

		//Allocating filename memory + copying previous letters
		WhPx_TOC.filelist[i].filename = malloc((prevletters + letters + 5) * sizeof (char));
		memset(WhPx_TOC.filelist[i].filename, 0, (prevletters + letters + 5) * sizeof(char));

		if (prevletters)
			strncat_s(WhPx_TOC.filelist[i].filename, prevletters + 1, WhPx_TOC.filelist[i-1].filename, prevletters);
		
		//Read Unknown value for the moment (dir?)
		if (fread(&WhPx_TOC.filelist[i].unknown, sizeof(unsigned short), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}	

		//Read size of file
		if (fread(&WhPx_TOC.filelist[i].size, sizeof(unsigned long), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}

		//Read checksum of file if SceeWhPC format
		WhPx_TOC.filelist[i].checksum = 0;
		if (strncmp(WhPx_TOC.format, "SceeWhPk", 8))
			if (fread(&WhPx_TOC.filelist[i].checksum, sizeof(unsigned long), 1, handle) != 1){
			fclose(handle);
			WhPx_TOC.error = -1;
			return WhPx_TOC;}
		
		//Read filename
		memset(tempfilename, 0, MAX_PATH * sizeof(char));
		if (fread(tempfilename, sizeof(char), (letters - 1), handle) != (letters - 1)){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
		strncat_s(WhPx_TOC.filelist[i].filename, prevletters + letters, tempfilename, (letters - 1));
		strncat_s(WhPx_TOC.filelist[i].filename, prevletters + letters + 1, ".", 1);

		//Read Extension for filename
		memset(&extension, 0, sizeof(unsigned int));
		if (fread(&extension, sizeof(unsigned char), 1, handle) != 1){
		fclose(handle);
		WhPx_TOC.error = -1;
		return WhPx_TOC;}	
		if (extension <= 31)
			strncat_s(WhPx_TOC.filelist[i].filename, prevletters + letters + 1 + strlen(WhPx_TOC.extlist[extension]), WhPx_TOC.extlist[extension], strlen(WhPx_TOC.extlist[extension]));
		
		//print offset byte:
		/*printf("%d:", WhPx_TOC.filelist[i].offset);
		for (j = 0; j != 3; j++)
			printf("%d,", singlechar[j]);
		printf("%s\n", WhPx_TOC.filelist[i].filename);*/

		//Print value found
		//printf("0x%x  :  0x%x Bytes  %s\n", WhPx_TOC.filelist[i].offset * 2048, WhPx_TOC.filelist[i].size, WhPx_TOC.filelist[i].filename);
	}

	////Write Debug Log
	memset(tempfilename, 0, MAX_PATH * sizeof(char));
	strcpy_s(tempfilename, strlen(filename) + 1, filename);
	for (i = (unsigned int) strlen(filename) - 1; tempfilename[i] != '\\'; i--);
	tempfilename[i + 1] = 0;
	strcat_s(tempfilename, (strlen(tempfilename) + strlen("result-read.csv") + 1) * sizeof(char), "result-read.csv");
	if(fopen_s(&output, tempfilename, "w")){
		WhPx_TOC.error = -1;
		return WhPx_TOC;}
	fprintf(output, "offset,header_len,filename,reduced_filename,previous_letters,filename_len,reduced_filename_len,unknown,size,checksum,extension\n");
	for (i = 0; i != WhPx_TOC.nbrfile; i++)
		fprintf(output, "%d,%d,%s,%s,%d,%d,%d,%d,%d,0x%08.8X,%d\n", WhPx_TOC.filelist[i].offset, WhPx_TOC.filelist[i].header_len, WhPx_TOC.filelist[i].filename, 
		//fprintf(output, "%d,%d,%s,%s,%d,%d,%d,%d,%d,%d,%d\n", WhPx_TOC.filelist[i].offset, WhPx_TOC.filelist[i].header_len, WhPx_TOC.filelist[i].filename, 
		WhPx_TOC.filelist[i].reduced_filename, WhPx_TOC.filelist[i].previous_letters, WhPx_TOC.filelist[i].filename_len,
		WhPx_TOC.filelist[i].reduced_filename_len, WhPx_TOC.filelist[i].unknown, WhPx_TOC.filelist[i].size, WhPx_TOC.filelist[i].checksum, WhPx_TOC.filelist[i].extension);

	if (fclose(output)){
		WhPx_TOC.error = -1;
		return WhPx_TOC;}

	if (fclose(handle)){
		WhPx_TOC.error = -1;
		return WhPx_TOC;}

	WhPx_TOC.readed_TOC = 1;

	return WhPx_TOC;
}