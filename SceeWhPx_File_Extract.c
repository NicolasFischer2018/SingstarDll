#include "PAKio.h"

int	SceeWhPx_File_Extract(_Whp_struct WhPx_TOC, char *filename, char *dest, int relative)
{
	FILE			*handle;
	FILE			*output;
	int				i, j, div, dif;
	int				found;
	char			tempfilename[MAX_PATH];
	char			temprelativefilename[MAX_PATH];
	char			tempdir[MAX_PATH];
	char			tempdata[BUFFER_SIZE];
	char			destination[MAX_PATH];

	
	//Check if structure is valid to extract all
	if ((WhPx_TOC.error != 0) | (WhPx_TOC.readed_TOC != 1))
		return -1;

	//Open File for Binary Access
	if(fopen_s(&handle, WhPx_TOC.PAK_file, "rb")){
		return -1;}

	//Change and create necessary folders for this resulting destination path
	memset(destination, 0, MAX_PATH * sizeof(char));
	SceeWhPx_Tools_FixPath(dest, destination);
	SceeWhPx_Tools_CreateDirectory(destination);
	
	found = 0;

	//Begin to write files from PAK file
	for (i = 0; i != WhPx_TOC.nbrfile; i++)
	{
		if (strcmp(WhPx_TOC.filelist[i].filename, filename) == 0){
			memset(tempfilename, 0, MAX_PATH * sizeof(char));
			strcpy_s(tempfilename, strlen(destination) + 1, destination);
			SceeWhPx_Tools_ExtractFilename(WhPx_TOC.filelist[i].filename, temprelativefilename);
			if (relative == 0)
				strcat_s(tempfilename, strlen(WhPx_TOC.filelist[i].filename) + strlen(destination) + 1, WhPx_TOC.filelist[i].filename);
			else
				strcat_s(tempfilename, strlen(temprelativefilename) + strlen(destination) + 1,  temprelativefilename);
			
			//Is there a folder to create?
			memset(tempdir, 0, MAX_PATH * sizeof(char));
			SceeWhPx_Tools_ExtractPath(tempfilename, tempdir);
			SceeWhPx_Tools_CreateDirectory(tempdir);
						
			//Open File for Binary Access
			if(fopen_s(&output, tempfilename, "wb"))
				return -1;
			
			
			//Seek for file begining with offset information from PAK begining
			if(_fseeki64(handle, (unsigned long)(WhPx_TOC.filelist[i].offset * 2048) , SEEK_SET) == -1)
					return -1;
		
			//Calculate value to read from offset with buffer size
			div = WhPx_TOC.filelist[i].size / BUFFER_SIZE;
			dif = WhPx_TOC.filelist[i].size % BUFFER_SIZE;
			
			//Read/Write to file
			for (j = 0; j != div; j++)
			{
				memset(tempdata, 0, BUFFER_SIZE * sizeof(char));
				if (fread(&tempdata, sizeof(char), BUFFER_SIZE, handle) != BUFFER_SIZE){
					fclose(handle);
					return -1;}	
				if (fwrite(&tempdata, sizeof(char), BUFFER_SIZE, output) != BUFFER_SIZE){
					fclose(output);
					return -1;}	
			}

			memset(tempdata, 0, BUFFER_SIZE * sizeof(char));
			if (fread(&tempdata, sizeof(char), dif, handle) != dif){
				fclose(handle);
				return -1;}	
			if (fwrite(&tempdata, sizeof(char), dif, output) != dif){
				fclose(output);
				return -1;}	

			//Close output file
			if (fclose(output))
				return -1;

			found = 1;
		}
	}

	//Close file
	if (fclose(handle))
		return -1;
	//printf("File Close OK!\n");
	if (found == 0){
		WhPx_TOC.error = -1;
		return -1;}
	printf("Extract: %s...OK\n", tempfilename);
	WhPx_TOC.error = 0;
	return 0;
}
