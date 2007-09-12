#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include "difx2fits.h"
#include "byteorder.h"

const DifxInput *DifxInput2FitsFQ(const DifxInput *D,
	struct fits_keywords *p_fits_keys, struct fitsPrivate *out)
{
	char bandFormDouble[4];
	char bandFormFloat[4];
	char bandFormInt[4];

	struct fitsBinTableColumn columns[] =
	{
		{"FREQID", "1J", "FREQID number in uv data"},
		{"BANDFREQ", bandFormDouble, "frequency offset", "HZ"},
		{"CH_WIDTH", bandFormFloat, "spectral channel bandwidth", "HZ"},
		{"TOTAL_BANDWIDTH", bandFormFloat, "total bw of a BAND", "HZ"},
		{"SIDEBAND", bandFormInt, "sideband of each BAND"},
		{"BB_CHAN", bandFormInt, "baseband channel number (1-16)"}
	};

	int nRowBytes;
	int nColumn;
	char *fitsbuf, *p_fitsbuf;
	int row;
	int no_bands;
	int swap;

	if(D == 0)
	{
		return 0;
	}

	swap = (byteorder() == BO_LITTLE_ENDIAN);
	
	no_bands = p_fits_keys->no_band;

	sprintf(bandFormDouble, "%1dD", no_bands);  
	sprintf(bandFormFloat, "%1dE", no_bands);  
	sprintf(bandFormInt, "%1dJ", no_bands);  

	nColumn = NELEMENTS(columns);

	/* determine size of FITS record to be written */
	nRowBytes = FitsBinTableSize(columns, nColumn);

	/* malloc space for storing table in FITS format */
	if((fitsbuf = (char *)malloc(nRowBytes)) == 0)
	{
		return 0;
	}
	
	/* spew out the table header */
	fitsWriteBinTable(out, nColumn, columns, nRowBytes, "FREQUENCY");
	arrayWriteKeys(p_fits_keys, out);
	fitsWriteInteger(out, "TABREV", 2, "");
	fitsWriteEnd(out);

	for(row = 0; row < D->nConfig; row++)
	{
		int i;
		int freqId = row + 1;

		/* pointer to the buffer for FITS records */
		p_fitsbuf = fitsbuf;

		/* FREQ_ID */
		{
			bcopy((char *)&freqId, p_fitsbuf,
			sizeof(freqId));     
			p_fitsbuf += sizeof(freqId);
		}
		
		/* BANDFREQ */
		for (i = 0; i < no_bands; i++)
		{
			double bandfreq = (D->config[row].IF[i].freq - 
				D->refFreq) * 1.0e6;

			bcopy((char *)&bandfreq, p_fitsbuf, sizeof(bandfreq));
			p_fitsbuf += sizeof(bandfreq);
		}

		/* CH_WIDTH */
		for (i = 0; i < no_bands; i++)
		{
			float ch_width = (D->config[row].IF[i].bw / 
				D->nOutChan) * 1.0e6;
			bcopy((char *)&ch_width, p_fitsbuf, sizeof(ch_width));
			p_fitsbuf += sizeof(ch_width);
		}

		/* TOTAL_BANDWIDTH */
		for (i = 0; i < no_bands; i++)
		{
			float total_bw = D->config[row].IF[i].bw * 1.0e6;
			bcopy((char *)&total_bw, p_fitsbuf, sizeof(total_bw));
			p_fitsbuf += sizeof(total_bw);
		}

		/* SIDEBAND */
		for (i = 0; i < no_bands; i++)
		{
			int32_t netside;
			netside = D->config[row].IF[i].sideband == 'U' ? 1 : -1;
			bcopy((char *)&netside, p_fitsbuf, sizeof (netside));
			p_fitsbuf += sizeof(netside);
		}

		/* BB_CHAN */
		for (i = 0; i < no_bands; i++)
		{
			/*
			* set to zero since AIPS doesn't care. To be
			* removed in a future CORR/AIPS update
			*/
			int bb_chan = 0;      

			bcopy((char *)&bb_chan, p_fitsbuf, sizeof(bb_chan));
			p_fitsbuf += sizeof(bb_chan);
		}
		if(swap)
		{
			FitsBinRowByteSwap(columns, nColumn, fitsbuf);
		}
		fitsWriteBinRow(out, fitsbuf);
	}

	/* close the output and return */
	free(fitsbuf);

	return D;
}
