//-----------------------------------------------------
//
// Code generated with Faust 0.9.46 (http://faust.grame.fr)
//-----------------------------------------------------
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

typedef long double quad;
/* link with : "" */
#include "typepunning.h"
#include <math.h>

#define FAUSTCLASS mydsp

class mydsp : public dsp {
  private:
	FAUSTFLOAT 	fcheckbox0;
	int 	iConst0;
	float 	fConst1;
	FAUSTFLOAT 	fslider0;
	float 	fRec2_perm[4];
	FAUSTFLOAT 	fslider1;
	float 	fRec1_perm[4];
	float 	fConst2;
	FAUSTFLOAT 	fslider2;
	FAUSTFLOAT 	fbargraph0;
	FAUSTFLOAT 	fslider3;
	FAUSTFLOAT 	fbargraph1;
	float 	fRec0_perm[4];
	FAUSTFLOAT 	fslider4;
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

	virtual int getNumInputs() 	{ return 2; }
	virtual int getNumOutputs() 	{ return 2; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fcheckbox0 = 0.0;
		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = (1.0f / iConst0);
		fslider0 = 100.237f;
		for (int i=0; i<4; i++) fRec2_perm[i]=0;
		fslider1 = 50.148f;
		for (int i=0; i<4; i++) fRec1_perm[i]=0;
		fConst2 = (2.0f / iConst0);
		fslider2 = -2e+01f;
		fslider3 = 2.0f;
		for (int i=0; i<4; i++) fRec0_perm[i]=0;
		fslider4 = 0.0f;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(UI* interface) {
		interface->openVerticalBox("system_compressor");
		interface->declare(&fslider3, "0", "");
		interface->declare(&fslider3, "style", "slider");
		interface->declare(&fslider3, "tooltip", "A compression Ratio of N means that for each N dB increase in input signal level above Threshold, the output level goes up 1 dB");
		interface->addHorizontalSlider("Ratio", &fslider3, 2.0f, 1.0f, 2e+01f, 0.1f);
		interface->declare(&fslider2, "1", "");
		interface->declare(&fslider2, "style", "slider");
		interface->declare(&fslider2, "tooltip", "When the signal level exceeds the Threshold (in dB), its level is compressed according to the Ratio");
		interface->declare(&fslider2, "unit", "dB");
		interface->addHorizontalSlider("Threshold", &fslider2, -2e+01f, -2e+01f, 2e+01f, 0.1f);
		interface->declare(&fslider1, "2", "");
		interface->declare(&fslider1, "style", "slider");
		interface->declare(&fslider1, "tooltip", "Time constant in ms (1/e smoothing time) for the compression gain to approach (exponentially) a new lower target level (the compression `kicking in')");
		interface->declare(&fslider1, "unit", "ms");
		interface->addHorizontalSlider("Attack", &fslider1, 50.148f, 0.0f, 5e+02f, 0.1f);
		interface->declare(&fslider0, "3", "");
		interface->declare(&fslider0, "style", "slider");
		interface->declare(&fslider0, "tooltip", "Time constant in ms (1/e smoothing time) for the compression gain to approach (exponentially) a new higher target level (the compression 'releasing')");
		interface->declare(&fslider0, "unit", "ms");
		interface->addHorizontalSlider("Release", &fslider0, 100.237f, 0.0f, 1e+03f, 0.1f);
		interface->declare(&fslider4, "5", "");
		interface->declare(&fslider4, "tooltip", "The compressed-signal output level is increased by this amount (in dB) to make up for the level lost due to compression");
		interface->declare(&fslider4, "unit", "dB");
		interface->addHorizontalSlider("Output Gain", &fslider4, 0.0f, -4e+01f, 4e+01f, 0.1f);
		interface->declare(&fcheckbox0, "6", "");
		interface->addCheckButton("bypass", &fcheckbox0);
		interface->declare(&fbargraph0, "0", "");
		interface->declare(&fbargraph0, "7", "");
		interface->addHorizontalBargraph("Gakk", &fbargraph0, -5e+01f, 1e+01f);
		interface->declare(&fbargraph1, "1", "");
		interface->declare(&fbargraph1, "7", "");
		interface->addHorizontalBargraph("Gakk", &fbargraph1, -5e+01f, 1e+01f);
		interface->closeBox();
	}
	virtual void compute (int fullcount, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fZec0[32];
		float 	fZec1[32];
		float 	fZec2[32];
		float 	fRec2_tmp[32+4];
		float 	fRec1_tmp[32+4];
		float 	fZec3[32];
		float 	fZec4[32];
		float 	fRec0_tmp[32+4];
		float 	fZec5[32];
		int 	iSlow0 = int(fcheckbox0);
		float 	fSlow1 = expf((0 - (fConst1 / max(fConst1, (0.001f * fslider0)))));
		float 	fSlow2 = (1.0f - fSlow1);
		float* 	fRec2 = &fRec2_tmp[4];
		float 	fSlow3 = max(fConst1, (0.001f * fslider1));
		float 	fSlow4 = expf((0 - (fConst1 / fSlow3)));
		float 	fSlow5 = (1.0f - fSlow4);
		float* 	fRec1 = &fRec1_tmp[4];
		float 	fSlow6 = expf((0 - (fConst2 / fSlow3)));
		float 	fSlow7 = fslider2;
		float 	fSlow8 = ((1.0f / float(fslider3)) - 1.0f);
		float 	fSlow9 = (1.0f - fSlow6);
		float* 	fRec0 = &fRec0_tmp[4];
		float 	fSlow10 = powf(10,(0.05f * fslider4));
		int index;
		for (index = 0; index <= fullcount - 32; index += 32) {
			// compute by blocks of 32 samples
			const int count = 32;
			FAUSTFLOAT* input0 = &input[0][index];
			FAUSTFLOAT* input1 = &input[1][index];
			FAUSTFLOAT* output0 = &output[0][index];
			FAUSTFLOAT* output1 = &output[1][index];
			// SECTION : 1
			// LOOP 0x2a67980
			// exec code
			for (int i=0; i<count; i++) {
				fZec0[i] = ((iSlow0)?0:(float)input0[i]);
			}
			
			// LOOP 0x2a68a60
			// exec code
			for (int i=0; i<count; i++) {
				fZec1[i] = ((iSlow0)?0:(float)input1[i]);
			}
			
			// SECTION : 2
			// LOOP 0x2a678a0
			// exec code
			for (int i=0; i<count; i++) {
				fZec2[i] = fabsf((fabsf(fZec1[i]) + fabsf(fZec0[i])));
			}
			
			// SECTION : 3
			// LOOP 0x2a67530
			// pre processing
			for (int i=0; i<4; i++) fRec2_tmp[i]=fRec2_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec2[i] = ((fSlow2 * fZec2[i]) + (fSlow1 * max(fZec2[i], fRec2[i-1])));
			}
			// post processing
			for (int i=0; i<4; i++) fRec2_perm[i]=fRec2_tmp[count+i];
			
			// SECTION : 4
			// LOOP 0x2a67050
			// pre processing
			for (int i=0; i<4; i++) fRec1_tmp[i]=fRec1_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec1[i] = ((fSlow5 * fRec2[i]) + (fSlow4 * fRec1[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec1_perm[i]=fRec1_tmp[count+i];
			
			// SECTION : 5
			// LOOP 0x2a6e8f0
			// exec code
			for (int i=0; i<count; i++) {
				fZec3[i] = (8.685889638065037f * ((8.262958288192749e-08f * float(pun_float_to_int(fRec1[i]))) - 87.989971088f));
			}
			
			// SECTION : 6
			// LOOP 0x2a6e3b0
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph0 = fZec3[i];
				fZec4[i] = (fSlow8 * max((fZec3[i] - fSlow7), 0.0f));
			}
			
			// SECTION : 7
			// LOOP 0x2a66c50
			// pre processing
			for (int i=0; i<4; i++) fRec0_tmp[i]=fRec0_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph1 = fZec4[i];
				fRec0[i] = ((fSlow9 * fZec4[i]) + (fSlow6 * fRec0[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec0_perm[i]=fRec0_tmp[count+i];
			
			// SECTION : 8
			// LOOP 0x2a72a60
			// exec code
			for (int i=0; i<count; i++) {
				fZec5[i] = pun_int_to_float((8388608 * (126.94269504f + max(-126.0f, (0.16609640464202244f * fRec0[i])))));
			}
			
			// SECTION : 9
			// LOOP 0x2a66a20
			// exec code
			for (int i=0; i<count; i++) {
				output0[i] = (FAUSTFLOAT)((iSlow0)?(float)input0[i]:(fSlow10 * (fZec0[i] * fZec5[i])));
			}
			
			// LOOP 0x2a74a00
			// exec code
			for (int i=0; i<count; i++) {
				output1[i] = (FAUSTFLOAT)((iSlow0)?(float)input1[i]:(fSlow10 * (fZec1[i] * fZec5[i])));
			}
			
		}
		if (index < fullcount) {
			// compute the remaining samples if any
			int count = fullcount-index;
			FAUSTFLOAT* input0 = &input[0][index];
			FAUSTFLOAT* input1 = &input[1][index];
			FAUSTFLOAT* output0 = &output[0][index];
			FAUSTFLOAT* output1 = &output[1][index];
			// SECTION : 1
			// LOOP 0x2a67980
			// exec code
			for (int i=0; i<count; i++) {
				fZec0[i] = ((iSlow0)?0:(float)input0[i]);
			}
			
			// LOOP 0x2a68a60
			// exec code
			for (int i=0; i<count; i++) {
				fZec1[i] = ((iSlow0)?0:(float)input1[i]);
			}
			
			// SECTION : 2
			// LOOP 0x2a678a0
			// exec code
			for (int i=0; i<count; i++) {
				fZec2[i] = fabsf((fabsf(fZec1[i]) + fabsf(fZec0[i])));
			}
			
			// SECTION : 3
			// LOOP 0x2a67530
			// pre processing
			for (int i=0; i<4; i++) fRec2_tmp[i]=fRec2_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec2[i] = ((fSlow2 * fZec2[i]) + (fSlow1 * max(fZec2[i], fRec2[i-1])));
			}
			// post processing
			for (int i=0; i<4; i++) fRec2_perm[i]=fRec2_tmp[count+i];
			
			// SECTION : 4
			// LOOP 0x2a67050
			// pre processing
			for (int i=0; i<4; i++) fRec1_tmp[i]=fRec1_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fRec1[i] = ((fSlow5 * fRec2[i]) + (fSlow4 * fRec1[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec1_perm[i]=fRec1_tmp[count+i];
			
			// SECTION : 5
			// LOOP 0x2a6e8f0
			// exec code
			for (int i=0; i<count; i++) {
				fZec3[i] = (8.685889638065037f * ((8.262958288192749e-08f * float(pun_float_to_int(fRec1[i]))) - 87.989971088f));
			}
			
			// SECTION : 6
			// LOOP 0x2a6e3b0
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph0 = fZec3[i];
				fZec4[i] = (fSlow8 * max((fZec3[i] - fSlow7), 0.0f));
			}
			
			// SECTION : 7
			// LOOP 0x2a66c50
			// pre processing
			for (int i=0; i<4; i++) fRec0_tmp[i]=fRec0_perm[i];
			// exec code
			for (int i=0; i<count; i++) {
				fbargraph1 = fZec4[i];
				fRec0[i] = ((fSlow9 * fZec4[i]) + (fSlow6 * fRec0[i-1]));
			}
			// post processing
			for (int i=0; i<4; i++) fRec0_perm[i]=fRec0_tmp[count+i];
			
			// SECTION : 8
			// LOOP 0x2a72a60
			// exec code
			for (int i=0; i<count; i++) {
				fZec5[i] = pun_int_to_float((8388608 * (126.94269504f + max(-126.0f, (0.16609640464202244f * fRec0[i])))));
			}
			
			// SECTION : 9
			// LOOP 0x2a66a20
			// exec code
			for (int i=0; i<count; i++) {
				output0[i] = (FAUSTFLOAT)((iSlow0)?(float)input0[i]:(fSlow10 * (fZec0[i] * fZec5[i])));
			}
			
			// LOOP 0x2a74a00
			// exec code
			for (int i=0; i<count; i++) {
				output1[i] = (FAUSTFLOAT)((iSlow0)?(float)input1[i]:(fSlow10 * (fZec1[i] * fZec5[i])));
			}
			
		}
	}
};


