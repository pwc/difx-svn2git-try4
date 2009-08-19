/***************************************************************************
 *   Copyright (C) 2009 by Walter Brisken                                  *
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
/*===========================================================================
 * SVN properties (DO NOT CHANGE)
 *
 * $Id$
 * $HeadURL$
 * $LastChangedRevision$
 * $Author$
 * $LastChangedDate$
 *
 *==========================================================================*/

#ifndef __CORRPARAM_H__
#define __CORRPARAM_H__

#include <string>
#include <set>
#include <vector>
#include <map>
#include <list>

#include "vextables.h"

using namespace std;

// see http://cira.ivec.org/dokuwiki/doku.php/difx/configuration

class PhaseCentre
{
public:
	//constructors
	PhaseCentre(double ra, double dec, string name);
	PhaseCentre();

	//methods
	void initialise(double ra, double dec, string name);

	//variables
	double ra;	  //radians
	double dec;	  //radians
	string difxname;
        char calCode;
	int qualifier;
	// ephemeris
	string ephemObject;     // name of the object in the ephemeris
	string ephemFile;       // file containing a JPL ephemeris
	string naifFile;        // file containing naif time data
	double ephemDeltaT;     // tabulated ephem. interval (seconds, default 60)
};

class SourceSetup
{
public:
	SourceSetup(const string &name);
	void setkv(const string &key, const string &value);
	void setkv(const string &key, const string &value, PhaseCentre * pc);

	bool doPointingCentre;       	  // Whether or not to correlate the pointing centre
	string vexName;		     	  // Source name as appears in vex file
	PhaseCentre pointingCentre;  	  // The source which is at the pointing centre
	vector<PhaseCentre> phaseCentres; // Additional phase centres to be correlated
};

class AntennaSetup
{
public:
	AntennaSetup(const string &name);
	void setkv(const string &key, const string &value);

	string vexName;		// Antenna name as it appears in vex file
	string difxName;	// Antenna name (if different) to appear in difx
	double X, Y, Z;		// Station coordinates to override vex
	VexClock clock;
	// flag
	// media
	bool polSwap;		// If true, swap polarizations
};

class CorrSetup
{
public:
	CorrSetup(const string &name = "setup_default");
	void setkv(const string &key, const string &value);
	bool correlateFreqId(int freqId) const;
	double bytesPerSecPerBLPerBand() const;

	string corrSetupName;

	double tInt;		// integration time
	bool doPolar;		// false for no cross pol, true for full pol
	bool doAuto;		// write autocorrelations
	int subintNS;		// Duration of a subintegration in nanoseconds
	int guardNS;		// Number of "guard" ns tacked on to end of a send
	int nChan;		// For the narrowest band 
				// (all others will have same spectral resolution)
	int specAvg;
	int maxNSBetweenUVShifts; //for multi-phase centres
	int fringeRotOrder;	// 0, 1, or 2
	int strideLength;	// The number of channels to do at a time
				// when fringeRotOrder > 0
	string binConfigFile;
	set<int> freqIds;	// which bands to correlate
private:
	void addFreqId(int freqId);
};


class CorrRule
{
public:
	CorrRule(const string &name = "rule_default");

	void setkv(const string &key, const string &value);
	bool match(const string &scan, const string &source, const string &mode, char cal, int qual) const;

	string ruleName;

	list<string> scanName;
	list<string> sourceName;
	list<string> modeName;
	list<char> calCode;
	list<int> qualifier;

	string corrSetupName;	/* pointer to CorrSetup */
};

class CorrParams
{
public:
	CorrParams();
	CorrParams(const string& fileName);

	void loadShelves(const string& fileName);
	const char *getShelf(const string& vsn) const;

	void setkv(const string &key, const string &value);
	void load(const string& fileName);
	void defaults();
	void defaultSetup();
	void example();
	void addSourceSetup(SourceSetup toadd);

	bool useAntenna(const string &antName) const;
	bool useBaseline(const string &ant1, const string &ant2) const;
	bool swapPol(const string &antName) const;
	const CorrSetup *getCorrSetup(const string &name) const;
	const SourceSetup *getSourceSetup(const string &name) const;
	const PhaseCentre *getPhaseCentre(const string &difxname) const;
	const AntennaSetup *getAntennaSetup(const string &name) const;
	const VexClock *getAntennaClock(const string &antName) const;

	const string &findSetup(const string &scan, const string &source, const string &mode, char cal, int qual) const;
	
	/* global parameters */
	string vexFile;
	double mjdStart;
	double mjdStop;
	unsigned int minSubarraySize;
	double maxGap;		// days
	bool singleScan;
	bool singleSetup;
	bool allowOverlap;
	bool mediaSplit;	// split jobs on media change
	bool padScans;
	bool simFXCORR;		// set integration and start times to match VLBA HW correlator
	double maxLength;	// [days]
	double maxSize;		// [bytes] -- break jobs for output filesize
	string jobSeries;	// prefix name to job files
	int startSeries;	// start job series at this number
	int dataBufferFactor;
	int nDataSegments;
	double sendLength;	// (s) amount of data to send from datastream to core at a time
	unsigned int invalidMask;
	int visBufferLength;

	list<string> antennaList;
	list<pair<string,string> > baselineList;

	/* setups to apply */
	vector<CorrSetup> corrSetups;

	/* source setups to apply */
	vector<SourceSetup> sourceSetups;

	/* antenna setups to apply */
	vector<AntennaSetup> antennaSetups;

	/* rules to determine which setups to apply */
	vector<CorrRule> rules;

private:
	void addAntenna(const string& antName);
	void addBaseline(const string& baselineName);
	map<string,string> shelves;
};

ostream& operator << (ostream& os, const CorrSetup& x);
ostream& operator << (ostream& os, const CorrRule& x);
ostream& operator << (ostream& os, const CorrParams& x);

bool areCorrSetupsCompatible(const CorrSetup *A, const CorrSetup *B, const CorrParams *C);

#endif
