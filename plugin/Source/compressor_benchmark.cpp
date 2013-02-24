//-----------------------------------------------------
//
// Code generated with Faust 0.9.46 (http://faust.grame.fr)
//-----------------------------------------------------
/* link with : "" */
#include "typepunning.h"
#include <math.h>
/************************************************************************

	IMPORTANT NOTE : this file contains two clearly delimited sections : 
	the ARCHITECTURE section (in two parts) and the USER section. Each section 
	is governed by its own copyright and license. Please check individually 
	each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
	Copyright (C) 2003-2011 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it 
    and/or modify it under the terms of the GNU General Public License 
	as published by the Free Software Foundation; either version 3 of 
	the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License 
	along with this program; If not, see <http://www.gnu.org/licenses/>.

	EXCEPTION : As a special exception, you may create a larger work 
	that contains this FAUST architecture section and distribute  
	that work under terms of your choice, so long as this FAUST 
	architecture section is not modified. 


 ************************************************************************
 ************************************************************************/


/* link with  */
/* link with : "" */
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/types.h>
#include <assert.h>
#include <pthread.h>
#include <sys/wait.h>
#include <list>

#include <iostream>


using namespace std;

#include <map>

using namespace std;

// On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
// flags to avoid costly denormals
#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS 
#endif

struct Meta : map<const char*, const char*>
{
    void declare (const char* key, const char* value) { (*this)[key]=value; }
};



float*          gBuffer = 0;        // a buffer of NV*VSize samples

unsigned int    COUNT   = 2000;     // number of measures
unsigned int    NV      = 4096;     // number of vectors in BIG buffer (should exceed cache)
unsigned int    ITER    = 10;       // number of iterations per measure
unsigned int    VSIZE   = 4096;     // size of a vector in samples
unsigned int    IDX     = 0;        // current vector number (0 <= VIdx < NV)


//inline void *aligned_calloc(size_t nmemb, size_t size) { return (void*)((unsigned)(calloc((nmemb*size)+15,sizeof(char)))+15 & 0xfffffff0); }
//inline void *aligned_calloc(size_t nmemb, size_t size) { return (void*)((size_t)(calloc((nmemb*size)+15,sizeof(char)))+15 & ~15); }



// g++ -Wall -O3 -lm -lpthread -lasound `gtk-config --cflags --libs` test.cpp -o test


//-------------------------------------------------------------------
// Generic min and max using c++ inline
//-------------------------------------------------------------------

inline int 		max (unsigned int a, unsigned int b) { return (a>b) ? a : b; }
inline int 		max (int a, int b) 			{ return (a>b) ? a : b; }

inline long 	max (long a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (int a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (long a, int b) 		{ return (a>b) ? a : b; }

inline float 	max (float a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (int a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, int b) 		{ return (a>b) ? a : b; }
inline float 	max (long a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, long b) 		{ return (a>b) ? a : b; }

inline double 	max (double a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (int a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, int b) 		{ return (a>b) ? a : b; }
inline double 	max (long a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, long b) 		{ return (a>b) ? a : b; }
inline double 	max (float a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (double a, float b) 	{ return (a>b) ? a : b; }


inline int 		min (int a, int b) 			{ return (a<b) ? a : b; }

inline long 	min (long a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (int a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (long a, int b) 		{ return (a<b) ? a : b; }

inline float 	min (float a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (int a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, int b) 		{ return (a<b) ? a : b; }
inline float 	min (long a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, long b) 		{ return (a<b) ? a : b; }

inline double 	min (double a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (int a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, int b) 		{ return (a<b) ? a : b; }
inline double 	min (long a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, long b) 		{ return (a<b) ? a : b; }
inline double 	min (float a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (double a, float b) 	{ return (a<b) ? a : b; }


inline int		lsr (int x, int n)			{ return int(((unsigned int)x) >> n); }


bool setRealtimePriority ()
{
    struct passwd *         pw;
    int                     err;
    uid_t                   uid;
    int                     policy;
    struct sched_param      param;

    uid = getuid ();
    pw = getpwnam ("root");
    setuid (pw->pw_uid);

    pthread_getschedparam(pthread_self(), &policy, &param);
    policy = SCHED_RR;
    param.sched_priority = 50;
    err = pthread_setschedparam(pthread_self(), policy, &param);

    setuid (uid);
    return (err != -1);
}

#include <sys/time.h>

double mysecond()
{
        struct timeval tp;
        struct timezone tzp;
        int i;

        i = gettimeofday(&tp,&tzp);
        return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

/******************************************************************************
*******************************************************************************

							       VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/






/******************************************************************************
*******************************************************************************

			ABSTRACT USER INTERFACE

*******************************************************************************
*******************************************************************************/

class UI
{
	bool	fStopped;
public:

	UI() : fStopped(false) {}
	virtual ~UI() {}

	virtual void addButton(const char* label, float* zone) = 0;
	virtual void addToggleButton(const char* label, float* zone) = 0;
	virtual void addCheckButton(const char* label, float* zone) = 0;
	virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
	virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
	virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) = 0;

        // -- passive widgets

        virtual void addNumDisplay(const char* label, float* zone, int precision) = 0;
        virtual void addTextDisplay(const char* label, float* zone, char* names[], float min, float max) = 0;
        virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max) = 0;
	virtual void addVerticalBargraph(const char* label, float* zone, float min, float max) = 0;

	virtual void openFrameBox(const char* label) = 0;
        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;

        virtual void closeBox() = 0;

        virtual void run() = 0;

	void stop()	{ fStopped = true; }
	bool stopped() 	{ return fStopped; }

    virtual void declare(float* zone, const char* key, const char* value) {}
};




/******************************************************************************
*******************************************************************************

								DSP

*******************************************************************************
*******************************************************************************/



//----------------------------------------------------------------
//  definition du processeur de signal
//----------------------------------------------------------------

class dsp {
 protected:
	int fSamplingFreq;
 public:
	dsp() {}
	virtual ~dsp() {}
	virtual int getNumInputs() 										= 0;
	virtual int getNumOutputs() 									= 0;
	virtual void buildUserInterface(UI* interface) 					= 0;
	virtual void init(int samplingRate) 							= 0;
 	virtual void compute(int len, float** inputs, float** outputs) 	= 0;
};
		
/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/
		
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

typedef long double quad;

#define FAUSTCLASS mydsp

class mydsp : public dsp {
  private:
	FAUSTFLOAT 	fslider0;
	float 	fRec0_perm[4];
	int 	iConst0;
	float 	fConst1;
	FAUSTFLOAT 	fslider1;
	float 	fRec3_perm[4];
	FAUSTFLOAT 	fslider2;
	float 	fRec2_perm[4];
	float 	fConst2;
	FAUSTFLOAT 	fslider3;
	FAUSTFLOAT 	fbargraph0;
	FAUSTFLOAT 	fslider4;
	FAUSTFLOAT 	fbargraph1;
	float 	fRec1_perm[4];
  public:
	static void metadata(Meta* m) 	{ 
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("filter.lib/copyright", "Julius O. Smith III");
		m->declare("filter.lib/version", "1.29");
		m->declare("filter.lib/license", "STK-4.3");
		m->declare("filter.lib/reference", "https://ccrma.stanford.edu/~jos/filters/");
		m->declare("music.lib/name", "Music Library");
		m->declare("music.lib/author", "GRAME");
		m->declare("music.lib/copyright", "GRAME");
		m->declare("music.lib/version", "1.0");
		m->declare("music.lib/license", "LGPL");
		m->declare("math.lib/name", "Math Library");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/version", "1.0");
		m->declare("math.lib/license", "LGPL");
		m->declare("effect.lib/name", "Faust Audio Effect Library");
		m->declare("effect.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("effect.lib/copyright", "Julius O. Smith III");
		m->declare("effect.lib/version", "1.33");
		m->declare("effect.lib/license", "STK-4.3");
		m->declare("effect.lib/reference", "https://ccrma.stanford.edu/realsimple/faust_strings/");
	}

	virtual int getNumInputs() 	{ return 1; }
	virtual int getNumOutputs() 	{ return 1; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fslider0 = 0.0f;
		for (int i=0; i<4; i++) fRec0_perm[i]=0;
		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = (1.0f / iConst0);
		fslider1 = 100.237f;
		for (int i=0; i<4; i++) fRec3_perm[i]=0;
		fslider2 = 50.148f;
		for (int i=0; i<4; i++) fRec2_perm[i]=0;
		fConst2 = (2.0f / iConst0);
		fslider3 = -2e+01f;
		fslider4 = 2.0f;
		for (int i=0; i<4; i++) fRec1_perm[i]=0;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(UI* interface) {
		interface->openVerticalBox("compressor_mono");
		interface->declare(&fslider4, "0", "");
		interface->declare(&fslider4, "style", "slider");
		interface->declare(&fslider4, "tooltip", "A compression Ratio of N means that for each N dB increase in input signal level above Threshold, the output level goes up 1 dB");
		interface->addHorizontalSlider("Ratio", &fslider4, 2.0f, 1.0f, 2e+01f, 0.1f);
		interface->declare(&fslider3, "1", "");
		interface->declare(&fslider3, "style", "slider");
		interface->declare(&fslider3, "tooltip", "When the signal level exceeds the Threshold (in dB), its level is compressed according to the Ratio");
		interface->declare(&fslider3, "unit", "dB");
		interface->addHorizontalSlider("Threshold", &fslider3, -2e+01f, -2e+01f, 2e+01f, 0.1f);
		interface->declare(&fslider2, "2", "");
		interface->declare(&fslider2, "style", "slider");
		interface->declare(&fslider2, "tooltip", "Time constant in ms (1/e smoothing time) for the compression gain to approach (exponentially) a new lower target level (the compression `kicking in')");
		interface->declare(&fslider2, "unit", "ms");
		interface->addHorizontalSlider("Attack", &fslider2, 50.148f, 0.0f, 5e+02f, 0.1f);
		interface->declare(&fslider1, "3", "");
		interface->declare(&fslider1, "style", "slider");
		interface->declare(&fslider1, "tooltip", "Time constant in ms (1/e smoothing time) for the compression gain to approach (exponentially) a new higher target level (the compression 'releasing')");
		interface->declare(&fslider1, "unit", "ms");
		interface->addHorizontalSlider("Release", &fslider1, 100.237f, 0.0f, 1e+03f, 0.1f);
		interface->declare(&fslider0, "5", "");
		interface->declare(&fslider0, "tooltip", "The compressed-signal output level is increased by this amount (in dB) to make up for the level lost due to compression");
		interface->declare(&fslider0, "unit", "dB");
		interface->addHorizontalSlider("Output Gain", &fslider0, 0.0f, -4e+01f, 4e+01f, 0.1f);
		interface->declare(&fbargraph0, "0", "");
		interface->declare(&fbargraph0, "7", "");
		interface->addHorizontalBargraph("Gakk", &fbargraph0, -5e+01f, 1e+01f);
		interface->declare(&fbargraph1, "1", "");
		interface->declare(&fbargraph1, "7", "");
		interface->addHorizontalBargraph("Gakk", &fbargraph1, -5e+01f, 1e+01f);
		interface->closeBox();
	}
	virtual void compute (int fullcount, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fRec0_tmp[32+4];
		float 	fZec0[32];
		float 	fRec3_tmp[32+4];
		float 	fRec2_tmp[32+4];
		float 	fZec1[32];
		float 	fZec2[32];
		float 	fRec1_tmp[32+4];
		float 	fSlow0 = (0.0010000000000000009f * powf(10,(0.05f * fslider0)));
		float* 	fRec0 = &fRec0_tmp[4];
		float 	fSlow1 = expf((0 - (fConst1 / max(fConst1, (0.001f * fslider1)))));
		float 	fSlow2 = (1.0f - fSlow1);
		float* 	fRec3 = &fRec3_tmp[4];
		float 	fSlow3 = max(fConst1, (0.001f * fslider2));
		float 	fSlow4 = expf((0 - (fConst1 / fSlow3)));
		float 	fSlow5 = (1.0f - fSlow4);
		float* 	fRec2 = &fRec2_tmp[4];
		float 	fSlow6 = expf((0 - (fConst2 / fSlow3)));
		float 	fSlow7 = fslider3;
		float 	fSlow8 = ((1.0f / float(fslider4)) - 1.0f);
		float 	fSlow9 = (1.0f - fSlow6);
		float* 	fRec1 = &fRec1_tmp[4];
		int index;
		for (index = 0; index <= fullcount - 32; index += 32) {
			// compute by blocks of 32 samples
			const int count = 32;
			FAUSTFLOAT* input0 = &input[0][index];
			FAUSTFLOAT* output0 = &output[0][index];
			// SECTION : 1
			// LOOP 0x2119c50
			// exec code
			for (int i=0; i<count; i++) {
				fZec0[i] = fabsf(fabsf((float)input0[i]));
			}
			
			// SECTION : 2
			// LOOP 0x2119970
			// pre processing
			for (int i=0; i<4; i++) fRec3_tmp[i]=fRec3_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec3[i] = ((fSlow2 * fZec0[i]) + (fSlow1 * max(fZec0[i], fRec3[i-1])));
			}
			// post processing
			for (int i=0; i<4; i++) fRec3_perm[i]=fRec3_tmp[count+i];
			
			// SECTION : 3
			// LOOP 0x21194d0
			// pre processing
			for (int i=0; i<4; i++) fRec2_tmp[i]=fRec2_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec2[i] = ((fSlow5 * fRec3[i]) + (fSlow4 * fRec2[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec2_perm[i]=fRec2_tmp[count+i];
			
			// SECTION : 4
			// LOOP 0x211f540
			// exec code
			for (int i=0; i<count; i++) {
				fZec1[i] = (8.685889638065037f * ((8.262958288192749e-08f * float(pun_float_to_int(fRec2[i]))) - 87.989971088f));
			}
			
			// SECTION : 5
			// LOOP 0x211f000
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph0 = fZec1[i];
				fZec2[i] = (fSlow8 * max((fZec1[i] - fSlow7), 0.0f));
			}
			
			// SECTION : 6
			// LOOP 0x21173b0
			// pre processing
			for (int i=0; i<4; i++) fRec0_tmp[i]=fRec0_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec0[i] = (fSlow0 + (0.999f * fRec0[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec0_perm[i]=fRec0_tmp[count+i];
			
			// LOOP 0x2118fb0
			// pre processing
			for (int i=0; i<4; i++) fRec1_tmp[i]=fRec1_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph1 = fZec2[i];
				fRec1[i] = ((fSlow9 * fZec2[i]) + (fSlow6 * fRec1[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec1_perm[i]=fRec1_tmp[count+i];
			
			// SECTION : 7
			// LOOP 0x21171f0
			// exec code
			for (int i=0; i<count; i++) {
				output0[i] = (FAUSTFLOAT)(((float)input0[i] * fRec0[i]) * pun_int_to_float((8388608 * (126.94269504f + max(-126.0f, (0.16609640464202244f * fRec1[i]))))));
			}
			
		}
		if (index < fullcount) {
			// compute the remaining samples if any
			int count = fullcount-index;
			FAUSTFLOAT* input0 = &input[0][index];
			FAUSTFLOAT* output0 = &output[0][index];
			// SECTION : 1
			// LOOP 0x2119c50
			// exec code
			for (int i=0; i<count; i++) {
				fZec0[i] = fabsf(fabsf((float)input0[i]));
			}
			
			// SECTION : 2
			// LOOP 0x2119970
			// pre processing
			for (int i=0; i<4; i++) fRec3_tmp[i]=fRec3_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec3[i] = ((fSlow2 * fZec0[i]) + (fSlow1 * max(fZec0[i], fRec3[i-1])));
			}
			// post processing
			for (int i=0; i<4; i++) fRec3_perm[i]=fRec3_tmp[count+i];
			
			// SECTION : 3
			// LOOP 0x21194d0
			// pre processing
			for (int i=0; i<4; i++) fRec2_tmp[i]=fRec2_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec2[i] = ((fSlow5 * fRec3[i]) + (fSlow4 * fRec2[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec2_perm[i]=fRec2_tmp[count+i];
			
			// SECTION : 4
			// LOOP 0x211f540
			// exec code
			for (int i=0; i<count; i++) {
				fZec1[i] = (8.685889638065037f * ((8.262958288192749e-08f * float(pun_float_to_int(fRec2[i]))) - 87.989971088f));
			}
			
			// SECTION : 5
			// LOOP 0x211f000
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph0 = fZec1[i];
				fZec2[i] = (fSlow8 * max((fZec1[i] - fSlow7), 0.0f));
			}
			
			// SECTION : 6
			// LOOP 0x21173b0
			// pre processing
			for (int i=0; i<4; i++) fRec0_tmp[i]=fRec0_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec0[i] = (fSlow0 + (0.999f * fRec0[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec0_perm[i]=fRec0_tmp[count+i];
			
			// LOOP 0x2118fb0
			// pre processing
			for (int i=0; i<4; i++) fRec1_tmp[i]=fRec1_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph1 = fZec2[i];
				fRec1[i] = ((fSlow9 * fZec2[i]) + (fSlow6 * fRec1[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec1_perm[i]=fRec1_tmp[count+i];
			
			// SECTION : 7
			// LOOP 0x21171f0
			// exec code
			for (int i=0; i<count; i++) {
				output0[i] = (FAUSTFLOAT)(((float)input0[i] * fRec0[i]) * pun_int_to_float((8388608 * (126.94269504f + max(-126.0f, (0.16609640464202244f * fRec1[i]))))));
			}
			
		}
	}
};



/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/
					
mydsp	DSP;


#if 0

static __inline__ unsigned long long int rdtsc(void)
{
  unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
}

#endif

/**
 * Bench by calling COUNT times the compute() method for
 * the computation of vsize samples
 */

#define MEGABYTE 1048576.0

void statistic(const char* name, double* timing)
{
    double lo, hi, tot;
    double mega =  double(VSIZE*ITER)/MEGABYTE; // mega samples
    // skip first 10 values to avoid cache bias ???
    lo = hi = tot = mega/(timing[11] - timing[10]);
    for (int i = 11; i<COUNT; i++) {
        double delta = mega/(timing[i] - timing[i-1]);
        if (delta < lo) {
            lo = delta;
        } else if (delta > hi) {
            hi = delta;
        }
        tot += delta;
    }

    cout << '\t' << hi
         << '\t' << hi*4*DSP.getNumInputs() << '\t' << "MB/s inputs"
         << '\t' << hi*4*DSP.getNumOutputs() << '\t' << "MB/s outputs"
         << '\t' << tot/(COUNT-11)
         << '\t' << lo
         << endl; 
} 

void allocBuffer()
{
    unsigned int BSIZE = NV * VSIZE;
    gBuffer = (float*) calloc (BSIZE, sizeof(float));
    
    int R0_0 = 0;
    for (int j = 0; j < BSIZE; j++) {
        int R0temp0 = (12345 + (1103515245 * R0_0));
        gBuffer[j] = 4.656613e-10f*R0temp0;
        R0_0 = R0temp0;
    }
}

float* nextVect()
{
    IDX = (1+IDX)%NV;
    return &gBuffer[IDX*VSIZE];
}

void bench(const char* name)
{

    int numInChan = DSP.getNumInputs();
    int numOutChan = DSP.getNumOutputs();

    assert (numInChan < 256);
    assert (numOutChan < 256);

    float*  inChannel[256];
    float*  outChannel[256];

    // allocate input buffers (initialized with white noise)
    allocBuffer();

    // allocate output channels (not initialized)
    for (int i = 0; i < numOutChan; i++) outChannel[i] = (float*) calloc (VSIZE, sizeof(float));

    // init the dsp with a resoneable sampling rate)
    DSP.init(48000);
    double* timing = (double*) calloc (COUNT, sizeof(double));

    for (int i = 0; i<COUNT; i++) {
        timing[i] = mysecond();
        for (int k = 0; k<ITER; k++) {
            // allocate new input buffers to avoid L2 cache
            for (int c=0; c<numInChan; c++) { inChannel[c] = nextVect(); }
            DSP.compute(VSIZE,inChannel,outChannel);
        }
    }

    statistic(name, timing);
}

//-------------------------------------------------------------------------
// 									MAIN
//-------------------------------------------------------------------------

// lopt : Scan Command Line long int Arguments

long lopt (int argc, char *argv[], const char* longname, const char* shortname, long def)
{
	for (int i=2; i<argc; i++)
		if ( strcmp(argv[i-1], shortname) == 0 || strcmp(argv[i-1], longname) == 0 )
			return atoi(argv[i]);
	return def;
}


int main(int argc, char *argv[] )
{
    AVOIDDENORMALS;
    VSIZE = lopt(argc, argv, "--vector-size", "-vec", 4096);
    NV = lopt(argc, argv, "--num-vector", "-n", 20000);
    COUNT = lopt(argc, argv, "--count", "-c", 1000);
    ITER = lopt(argc, argv, "--iteration", "-i", 10);
    //setRealtimePriority();
  	bench(argv[0]);
  	return 0;
}


/********************END ARCHITECTURE SECTION (part 2/2)****************/



