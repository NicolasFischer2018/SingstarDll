#include "PAKio.h"


int		SceeWhPx_CreateTXT(char *txtfile, _Whp_struct WhPx_TOC)
{
	FILE	*handle;
	int		i;

	if(fopen_s(&handle, txtfile, "w"))
		return -1;
	printf("TXT File Open for writing OK!\n");
	
	fprintf(handle, WhPx_TOC.PAK_file);
	fprintf(handle, "\n");
	fprintf(handle, "%d\n%d\n", WhPx_TOC.error, WhPx_TOC.readed_TOC);
	fprintf(handle, WhPx_TOC.format);
	fprintf(handle, "\n");
	fprintf(handle, WhPx_TOC.date);
	fprintf(handle, "\n");
	fprintf(handle, WhPx_TOC.time);
	fprintf(handle, "\n");
	fprintf(handle, "%ld\n%ld\n", WhPx_TOC.hdrsize, WhPx_TOC.nbrfile);
	fprintf(handle, WhPx_TOC.StrCompilePath);
	fprintf(handle, "\n");
	
	for (i = 1; WhPx_TOC.extlist[i][0] != 0; i++){
		fprintf(handle, WhPx_TOC.extlist[i]);
		fprintf(handle, "\n");}
	fprintf(handle, "Extension\n");
	for (i = 0; i != WhPx_TOC.nbrfile; i++){
		fprintf(handle, "%ld\n", WhPx_TOC.filelist[i].offset);
		fprintf(handle, WhPx_TOC.filelist[i].filename);
		fprintf(handle, "\n");
		fprintf(handle, "%d\n", WhPx_TOC.filelist[i].filename_len);
		fprintf(handle, WhPx_TOC.filelist[i].reduced_filename);
		fprintf(handle, "\n");
		fprintf(handle, "%d\n", WhPx_TOC.filelist[i].reduced_filename_len);
		fprintf(handle, "%d\n", WhPx_TOC.filelist[i].previous_letters);
		fprintf(handle, "%d\n", WhPx_TOC.filelist[i].header_len);
		fprintf(handle, "%d\n", WhPx_TOC.filelist[i].unknown);
		fprintf(handle, "%ld\n", WhPx_TOC.filelist[i].size);
		fprintf(handle, "%ld\n", WhPx_TOC.filelist[i].checksum);
		fprintf(handle, "%d\n", WhPx_TOC.filelist[i].extension);
		fprintf(handle, "%ld\n", WhPx_TOC.filelist[i].increased_header_len);
	}
	
	if (fclose(handle))
		return -1;
	return 0;
}