#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include "config.h"
#include "difx2fits.h"

const DifxInput *DifxInput2FitsSU(const DifxInput *D,
	struct fits_keywords *p_fits_keys, struct fitsPrivate *out)
{
	char bandFormDouble[4];
	char bandFormFloat[4];

	struct fitsBinTableColumn columns[] =
	{
		{"ID_NO.", "1J", "source id number", 0},
		{"SOURCE", "16A", "source name", 0},
		{"QUAL", "1J", "source qualifier", 0},
		{"CALCODE", "4A", "calibrator code", 0},
		{"FREQID", "1J", "freq id number in frequency tbl", 0},
		{"IFLUX", bandFormFloat, "ipol flux density at ref freq", "JY"},
		{"QFLUX", bandFormFloat, "qpol flux density at ref freq", "JY"},
		{"UFLUX", bandFormFloat, "upol flux density at ref freq", "JY"},
		{"VFLUX", bandFormFloat, "vpol flux density at ref freq", "JY"},
		{"ALPHA", bandFormFloat, "spectral index", 0},
		{"FREQOFF", bandFormDouble, "freq. offset from ref freq.","HZ"},
		{"RAEPO", "1D", "Right Ascension at EPOCH", "DEGREES"},
		{"DECEPO", "1D", "Declination at EPOCH", "DEGREES"},
		{"EPOCH", "1D", "epoch 1950.0B or J2000", "YEARS"},
		{"RAAPP", "1D", "apparent RA at 0 IAT ref day", "DEGREES"},
		{"DECAPP", "1D", "apparent Dec at 0 IAT ref day", "DEGREES"},
		{"SYSVEL", bandFormDouble, "systemic velocity at ref pixel", 
			"M/SEC"},
		{"VELTYP", "8A", "velocity type", 0},
		{"VELDEF", "8A", "velocity def: radio, optical", 0},
		{"RESTFREQ", bandFormDouble, "line rest frequency", "HZ"},
		{"PMRA", "1D", "proper motion in RA", "DEG/DAY"},
		{"PMDEC", "1D", "proper motion in Dec", "DEG/DAY"},
		{"PARALLAX", "1E", "parallax of source", "ARCSEC"}
	};

	int nColumn;
	int nRowBytes;
	int nBand;
	int b, sourceId;
	char *fitsbuf;
	char *p_fitsbuf;
	char sourceName[16];
	int configId;
	int qual;
	char calCode[4];
	float fluxI[array_MAX_BANDS];
	float fluxQ[array_MAX_BANDS];
	float fluxU[array_MAX_BANDS];
	float fluxV[array_MAX_BANDS];
	float alpha[array_MAX_BANDS];
	double freqOffset[array_MAX_BANDS];
	double RAEpoch, decEpoch;	/* position of Epoch */
	double RAApp, decApp;		/* apparent position */
	double epoch;
	double sysVel[array_MAX_BANDS];
	double restFreq[array_MAX_BANDS];
	char velType[8];
	char velDef[8];
	double muRA;			/* proper motion */
	double muDec;
	float parallax;
	/* 1-based indices for FITS file */
	int32_t sourceId1, freqId1;
	int *fitsSource;
	int i, nFitsSource;
	const DifxSource *source;
	const DifxConfig *config;

	if(D == 0)
	{
		return 0;
	}

	nBand = D->nIF;
	sprintf(bandFormFloat, "%1dE", nBand);
	sprintf(bandFormDouble, "%1dD", nBand); 

	nColumn = NELEMENTS(columns);
	nRowBytes = FitsBinTableSize(columns, nColumn);

	
	fitsWriteBinTable(out, nColumn, columns, nRowBytes, "SOURCE");
	arrayWriteKeys(p_fits_keys, out);
	fitsWriteInteger(out, "TABREV", 1, "");
	fitsWriteEnd(out);

	for(i = 0; i < array_MAX_BANDS; i++)
	{
		freqOffset[i] = 0.0;
	}
	
	/* no knowledge of these from inputs */
	for(b = 0; b < nBand; b++)
	{
		sysVel[b] = 0.0;
		fluxI[b] = 0.0;
		fluxQ[b] = 0.0;
		fluxU[b] = 0.0;
		fluxV[b] = 0.0;
		alpha[b] = 0.0;
		restFreq[b] = p_fits_keys->ref_freq;
	}

	strcpypad(velType, "GEOCENTR", 8);
	strcpypad(velDef, "OPTICAL", 8);

	fitsbuf = (char *)calloc(nRowBytes, 1);
	if(fitsbuf == 0)
	{
		return 0;
	}
	
	fitsSource = (int *)malloc(D->nSource*sizeof(int));
	for(sourceId = 0; sourceId < D->nSource; sourceId++)
	{
		fitsSource[sourceId] = -1;
	}
	nFitsSource = -1;
	for(sourceId = 0; sourceId < D->nSource; sourceId++)
	{
		i = D->source[sourceId].fitsSourceId;
		if(fitsSource[i] < 0)
		{
			fitsSource[i] = sourceId;
			if(i > nFitsSource)
			{
				nFitsSource = i;
			}
		}
	}
	nFitsSource++;

	for(i = 0; i < nFitsSource; i++)
	{
		sourceId = fitsSource[i];
		if(sourceId < 0)
		{
			fprintf(stderr, "Error: sourceId = -1\n");
			continue;
		}

		source = D->source + sourceId;
		configId = source->configId;
		if(configId < 0 || configId >= D->nConfig)
		{
			fprintf(stderr, "Error: configId out of range = %d\n",
				configId);
			continue;
		}

		config = D->config + configId;

		muRA = 0.0;
		muDec = 0.0;
		parallax = 0.0;
		epoch = 2000.0;
		sourceId1 = sourceId + 1;	/* FITS sourceId1 is 1-based */
		qual = source->qual;
		strcpypad(calCode, source->calCode, 4);
		RAEpoch = source->ra * 180.0 / M_PI;
		decEpoch = source->dec * 180.0 / M_PI;
		RAApp = RAEpoch;
		decApp = decEpoch;
		freqId1 = config->freqId + 1;  /* FITS 1-based */
		strcpypad(sourceName, source->name, 16);

		p_fitsbuf = fitsbuf;

		FITS_WRITE_ITEM (sourceId1, p_fitsbuf);
		FITS_WRITE_ITEM (sourceName, p_fitsbuf);
		FITS_WRITE_ITEM (qual, p_fitsbuf);
		FITS_WRITE_ITEM (calCode, p_fitsbuf);
		FITS_WRITE_ITEM (freqId1, p_fitsbuf);
		FITS_WRITE_ARRAY(fluxI, p_fitsbuf, nBand);
		FITS_WRITE_ARRAY(fluxQ, p_fitsbuf, nBand);
		FITS_WRITE_ARRAY(fluxU, p_fitsbuf, nBand);
		FITS_WRITE_ARRAY(fluxV, p_fitsbuf, nBand);
		FITS_WRITE_ARRAY(alpha, p_fitsbuf, nBand);
		FITS_WRITE_ARRAY(freqOffset, p_fitsbuf, nBand);
		FITS_WRITE_ITEM (RAEpoch, p_fitsbuf);
		FITS_WRITE_ITEM (decEpoch, p_fitsbuf);
		FITS_WRITE_ITEM (epoch, p_fitsbuf);
		FITS_WRITE_ITEM (RAApp, p_fitsbuf);
		FITS_WRITE_ITEM (decApp, p_fitsbuf);
		FITS_WRITE_ARRAY(sysVel, p_fitsbuf, nBand);
		FITS_WRITE_ITEM (velType, p_fitsbuf);
		FITS_WRITE_ITEM (velDef, p_fitsbuf);
		FITS_WRITE_ARRAY(restFreq, p_fitsbuf, nBand);
		FITS_WRITE_ITEM (muRA, p_fitsbuf);
		FITS_WRITE_ITEM (muDec, p_fitsbuf);
		FITS_WRITE_ITEM (parallax, p_fitsbuf);

		testFitsBufBytes(p_fitsbuf - fitsbuf, nRowBytes, "SU");

#ifndef WORDS_BIGENDIAN
		FitsBinRowByteSwap(columns, nColumn, fitsbuf);
#endif
		fitsWriteBinRow(out, fitsbuf);
	}

	free(fitsbuf);
	free(fitsSource);

	return D;
}	
