/***************************************************************************
 *   Copyright (C) 2007 by Walter Brisken                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdlib.h>
#include "../mark5access/mark5_stream.h"

const char program[] = "m5d";
const char author[]  = "Walter Brisken";
const char version[] = "1.0";
const char verdate[] = "2007 Oct 6";

int usage(const char *pgm)
{
	printf("\n");

	printf("%s ver. %s   %s  %s\n\n", program, version, author, verdate);
	printf("A Mark5 decoder.  Can decode VLBA, Mark3/4, and Mark5B "
		"formats using the\nmark5access library.\n\n");
	printf("Usage : %s <file> <dataformat> <n> [<offset>]\n\n", pgm);
	printf("  <file> is the name of the input file\n\n");
	printf("  <dataformat> should be of the form: "
		"<FORMAT>-<Mbps>-<nchan>-<nbit>, e.g.:\n");
	printf("    VLBA1_2-256-8-2\n");
	printf("    MKIV1_4-128-2-1\n");
	printf("    Mark5B-512-16-2\n\n");
	printf("  <n> is the number of samples per channel to decode\n\n");
	printf("  <offset> is number of bytes into file to start decoding\n\n");

	return 0;
}

int decode(const char *filename, const char *formatname, const char *f,
	long long offset, long long n)
{
	struct mark5_stream *ms;
	float **data;
	int i, j, k, status;
	long long chunk = 1024;
	long long total, unpacked;

	total = unpacked = 0;

	ms = new_mark5_stream(
		new_mark5_stream_file(filename, offset),
		new_mark5_format_generic_from_string(formatname) );

	if(!ms)
	{
		printf("problem opening %s\n", filename);
		return 0;
	}

	data = (float **)malloc(ms->nchan*sizeof(float *));
	for(i = 0; i < ms->nchan; i++)
	{
		data[i] = (float *)malloc(chunk*sizeof(float ));
	}

	mark5_stream_print(ms);

	if(n % ms->samplegranularity > 0)
	{
		n -= (n % ms->samplegranularity);
		printf("Warning -- reducing read size to %Ld\n", n);
	}

	for(; n > 0; n -= chunk)
	{
		if(n < chunk)
		{
			chunk = n;
		}
		status = mark5_stream_decode(ms, chunk, data);
		
		if(status < 0)
		{
			printf("<EOF> status=%d\n", status);
			//break;
		}
		else
		{
			total += chunk;
			unpacked += status;
		}

		for(j = 0; j < chunk; j++)
		{
			for(k = 0; k < ms->nchan; k++)
			{
				printf(f, data[k][j]);
			}
			printf("\n");
		}
	}

	fprintf(stderr, "%Ld / %Ld samples unpacked\n", unpacked, total);

	for(i = 0; i < ms->nchan; i++)
	{
		free(data[i]);
	}
	free(data);

	delete_mark5_stream(ms);

	return 0;
}

int main(int argc, char **argv)
{
	long long offset = 0;
	long long n;

	if(argc == 2)
	{
		struct mark5_format *mf;
		int bufferlen = 1<<11;
		char *buffer;
		FILE *in;

		buffer = malloc(bufferlen);
		
		in = fopen(argv[1], "r");
		fread(buffer, bufferlen, 1, in);
		
		mf = new_mark5_format_from_stream(
			new_mark5_stream_memory(buffer, bufferlen/2));

		print_mark5_format(mf);
		delete_mark5_format(mf);

		mf = new_mark5_format_from_stream(
			new_mark5_stream_memory(buffer, bufferlen/2));

		print_mark5_format(mf);
		delete_mark5_format(mf);

		free(buffer);

		return 0;
	}

	else if(argc < 4)
	{
		return usage(argv[0]);
	}

	n = atol(argv[3]);

	if(argc > 4)
	{
		offset=atoll(argv[4]);
	}

	decode(argv[1], argv[2], "%2.0f ", offset, n);

	return 0;
}

