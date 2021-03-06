#include <stdio.h>
#include <stdlib.h>
#include "parsevis.h"

const char program[] = "difxfringe";
const char version[] = "0.0";
const char verdate[] = "20070329";
const char author[]  = "Walter Brisken";

int usage(const char *pgm)
{
	fprintf(stderr, "%s ver. %s   %s %s\n\n", program, version,
		author, verdate);
	fprintf(stderr, "usage : %s <difx file> <nchan> [<baseline>]\n\n", pgm);

	return 0;
}

int fringe(const char *filename, int nchan, int baseline, const char *pol)
{
	DifxVisRecord *vis;
	int i, k;
	int v;

	FILE *out;

	out = fopen("vis.out", "w");

	vis = newDifxVisRecord(filename, nchan);

	if(!vis)
	{
		fprintf(stderr, "fringe : vis = 0\n");
		return -1;
	}

	for(i = 0; i < 3000; i++)
	{
		printf("i = %d\n", i);
		v = DifxVisRecordgetnext(vis);
		//v = DifxVisRecordfindnext(vis, baseline, freqid, pol);
		if(v < 0)
		{
			break;
		}
		printDifxParameters(vis->params);
		printf("%f %f # QQQ\n", 
			creal(vis->visdata[8]),
			cimag(vis->visdata[8]));
		for(k = 0; k < nchan; k++)
		{
			fprintf(out, "%d %d %f %f\n", i, k,
				creal(vis->visdata[k]),
				cimag(vis->visdata[k]) );
		}
	}

	fclose(out);

	deleteDifxVisRecord(vis);

	return 0;
}

int main(int argc, char **argv)
{
	int nchan;
	int baseline = 0;
	const char *pol = 0;

	if(argc < 3)
	{
		return usage(argv[0]);
	}

	nchan = atoi(argv[2]);
	if(nchan < 1 || nchan > 1<<21)
	{
		fprintf(stderr, "nchan out of range\n");
		return 0;
	}

	if(argc > 3)
	{
		baseline = atoi(argv[3]);
		if(baseline < 0)
		{
			fprintf(stderr, "baseline out of range\n");
			return 0;
		}	
	}

	if(argc > 4)
	{
		pol = argv[4];
	}

	fringe(argv[1], nchan, baseline, pol);

	return 0;
}
