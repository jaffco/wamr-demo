/* ------------------------------------------------------------
author: "Daniel Leonov"
name: "springreverb"
version: "0.1"
Code generated with Faust 2.83.6 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float mydsp_faustpower2_f(float value) {
	return value * value;
}

class mydsp : public dsp {
	
 private:
	
	FAUSTFLOAT fVslider0;
	int IOTA0;
	float fVec0[8192];
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fVslider2;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	FAUSTFLOAT fVslider3;
	float fConst5;
	float fConst6;
	int iConst7;
	int iConst8;
	int iConst9;
	int iConst10;
	int iConst11;
	int iConst12;
	int iConst13;
	int iConst14;
	float fVec1[2048];
	int iConst15;
	float fVec2[4096];
	int iConst16;
	float fVec3[4096];
	int iConst17;
	float fVec4[8192];
	int iConst18;
	float fVec5[4096];
	int iConst19;
	float fVec6[8192];
	int iConst20;
	float fVec7[4096];
	int iConst21;
	float fVec8[8192];
	int iConst22;
	float fVec9[2048];
	int iConst23;
	float fVec10[4096];
	int iConst24;
	float fVec11[4096];
	int iConst25;
	float fVec12[4096];
	int iConst26;
	float fVec13[4096];
	int iConst27;
	float fVec14[4096];
	int iConst28;
	float fVec15[4096];
	int iConst29;
	float fVec16[8192];
	int iConst30;
	float fVec17[2048];
	float fVec18[16384];
	float fConst31;
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fEntry0;
	float fVec19[2];
	float fRec9[2];
	float fRec1[2];
	float fVec20[4096];
	float fVec21[16384];
	float fVec22[2];
	float fRec10[2];
	float fRec2[2];
	float fVec23[4096];
	int iConst32;
	float fVec24[16384];
	float fVec25[2];
	float fRec11[2];
	float fRec3[2];
	float fVec26[4096];
	int iConst33;
	float fVec27[16384];
	float fVec28[2];
	float fRec12[2];
	float fRec4[2];
	float fVec29[4096];
	int iConst34;
	float fVec30[16384];
	float fVec31[2];
	float fRec13[2];
	float fRec5[2];
	float fVec32[8192];
	int iConst35;
	float fVec33[16384];
	float fVec34[2];
	float fRec14[2];
	float fRec6[2];
	float fVec35[8192];
	int iConst36;
	float fVec36[16384];
	float fVec37[2];
	float fRec15[2];
	float fRec7[2];
	float fVec38[8192];
	int iConst37;
	float fVec39[16384];
	float fVec40[2];
	float fRec16[2];
	float fRec8[2];
	float fVec41[2];
	float fRec0[2];
	
 public:
	mydsp() {
	}
	
	mydsp(const mydsp&) = default;
	
	virtual ~mydsp() = default;
	
	mydsp& operator=(const mydsp&) = default;
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/ADAA1:author", "Dario Sanfilippo");
		m->declare("aanl.lib/ADAA1:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/ADAA1:license", "MIT License");
		m->declare("aanl.lib/hardclip:author", "Dario Sanfilippo");
		m->declare("aanl.lib/hardclip:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/hardclip:license", "MIT License");
		m->declare("aanl.lib/name", "Faust Antialiased Nonlinearities");
		m->declare("aanl.lib/version", "1.4.2");
		m->declare("author", "Daniel Leonov");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("demos.lib/name", "Faust Demos Library");
		m->declare("demos.lib/springreverb_demo:author", "Daniel Leonov");
		m->declare("demos.lib/springreverb_demo:licence", "MIT");
		m->declare("demos.lib/version", "1.4.0");
		m->declare("description", "Spring reverb demo application.");
		m->declare("filename", "springreverb.dsp");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "springreverb");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/springreverb:author", "Daniel Leonov");
		m->declare("reverbs.lib/version", "1.5.1");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("version", "0.1");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / std::tan(471.2389f / fConst0);
		fConst2 = 1.0f / (fConst1 + 1.0f);
		fConst3 = 1.0f - fConst1;
		fConst4 = 3.1415927f / fConst0;
		fConst5 = std::round(0.035f * fConst0);
		fConst6 = std::max<float>(0.0f, fConst5);
		iConst7 = static_cast<int>(std::min<float>(std::round(0.05f * fConst0), fConst6));
		iConst8 = static_cast<int>(std::min<float>(fConst5, fConst6));
		iConst9 = static_cast<int>(std::min<float>(std::round(0.0425f * fConst0), fConst6));
		iConst10 = static_cast<int>(std::min<float>(std::round(0.0275f * fConst0), fConst6));
		iConst11 = static_cast<int>(std::min<float>(std::round(0.04625f * fConst0), fConst6));
		iConst12 = static_cast<int>(std::min<float>(std::round(0.03125f * fConst0), fConst6));
		iConst13 = static_cast<int>(std::min<float>(std::round(0.03875f * fConst0), fConst6));
		iConst14 = static_cast<int>(std::min<float>(std::round(0.02375f * fConst0), fConst6));
		iConst15 = static_cast<int>(std::min<float>(std::round(0.009f * fConst0), fConst6));
		iConst16 = static_cast<int>(std::min<float>(std::round(0.0195f * fConst0), fConst6));
		iConst17 = static_cast<int>(std::min<float>(std::round(0.01425f * fConst0), fConst6));
		iConst18 = static_cast<int>(std::min<float>(std::round(0.02475f * fConst0), fConst6));
		iConst19 = static_cast<int>(std::min<float>(std::round(0.011625f * fConst0), fConst6));
		iConst20 = static_cast<int>(std::min<float>(std::round(0.022125f * fConst0), fConst6));
		iConst21 = static_cast<int>(std::min<float>(std::round(0.016875f * fConst0), fConst6));
		iConst22 = static_cast<int>(std::min<float>(std::round(0.027375f * fConst0), fConst6));
		iConst23 = static_cast<int>(std::min<float>(std::round(0.01f * fConst0), fConst6));
		iConst24 = static_cast<int>(std::min<float>(std::round(0.0175f * fConst0), fConst6));
		iConst25 = static_cast<int>(std::min<float>(std::round(0.01375f * fConst0), fConst6));
		iConst26 = static_cast<int>(std::min<float>(std::round(0.02125f * fConst0), fConst6));
		iConst27 = static_cast<int>(std::min<float>(std::round(0.011875f * fConst0), fConst6));
		iConst28 = static_cast<int>(std::min<float>(std::round(0.019375f * fConst0), fConst6));
		iConst29 = static_cast<int>(std::min<float>(std::round(0.015625f * fConst0), fConst6));
		iConst30 = static_cast<int>(std::min<float>(std::round(0.023125f * fConst0), fConst6));
		fConst31 = std::round(0.08f * fConst0);
		iConst32 = static_cast<int>(std::min<float>(std::round(0.01475f * fConst0), fConst6));
		iConst33 = static_cast<int>(std::min<float>(std::round(0.017625f * fConst0), fConst6));
		iConst34 = static_cast<int>(std::min<float>(std::round(0.0205f * fConst0), fConst6));
		iConst35 = static_cast<int>(std::min<float>(std::round(0.023375f * fConst0), fConst6));
		iConst36 = static_cast<int>(std::min<float>(std::round(0.02625f * fConst0), fConst6));
		iConst37 = static_cast<int>(std::min<float>(std::round(0.029125f * fConst0), fConst6));
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = static_cast<FAUSTFLOAT>(1.0f);
		fVslider1 = static_cast<FAUSTFLOAT>(1.0f);
		fVslider2 = static_cast<FAUSTFLOAT>(0.5f);
		fVslider3 = static_cast<FAUSTFLOAT>(0.8f);
		fVslider4 = static_cast<FAUSTFLOAT>(0.5f);
		fEntry0 = static_cast<FAUSTFLOAT>(2.0f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 8192; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2048; l1 = l1 + 1) {
			fVec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 4096; l2 = l2 + 1) {
			fVec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 4096; l3 = l3 + 1) {
			fVec3[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 8192; l4 = l4 + 1) {
			fVec4[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 4096; l5 = l5 + 1) {
			fVec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 8192; l6 = l6 + 1) {
			fVec6[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4096; l7 = l7 + 1) {
			fVec7[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 8192; l8 = l8 + 1) {
			fVec8[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2048; l9 = l9 + 1) {
			fVec9[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 4096; l10 = l10 + 1) {
			fVec10[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4096; l11 = l11 + 1) {
			fVec11[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 4096; l12 = l12 + 1) {
			fVec12[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 4096; l13 = l13 + 1) {
			fVec13[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4096; l14 = l14 + 1) {
			fVec14[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4096; l15 = l15 + 1) {
			fVec15[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 8192; l16 = l16 + 1) {
			fVec16[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2048; l17 = l17 + 1) {
			fVec17[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 16384; l18 = l18 + 1) {
			fVec18[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fVec19[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec9[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec1[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 4096; l22 = l22 + 1) {
			fVec20[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 16384; l23 = l23 + 1) {
			fVec21[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fVec22[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fRec10[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec2[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 4096; l27 = l27 + 1) {
			fVec23[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 16384; l28 = l28 + 1) {
			fVec24[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fVec25[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec11[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec3[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 4096; l32 = l32 + 1) {
			fVec26[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 16384; l33 = l33 + 1) {
			fVec27[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fVec28[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec12[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec4[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 4096; l37 = l37 + 1) {
			fVec29[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 16384; l38 = l38 + 1) {
			fVec30[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fVec31[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec13[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec5[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 8192; l42 = l42 + 1) {
			fVec32[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 16384; l43 = l43 + 1) {
			fVec33[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fVec34[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec14[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec6[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 8192; l47 = l47 + 1) {
			fVec35[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 16384; l48 = l48 + 1) {
			fVec36[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fVec37[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec15[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec7[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 8192; l52 = l52 + 1) {
			fVec38[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 16384; l53 = l53 + 1) {
			fVec39[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fVec40[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec16[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec8[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fVec41[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec0[l58] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openHorizontalBox("Spring Reverb");
		ui_interface->declare(&fVslider3, "0", "");
		ui_interface->declare(&fVslider3, "style", "knob");
		ui_interface->declare(&fVslider3, "tooltip", "Feedback amount; controls decay length (0..1)");
		ui_interface->addVerticalSlider("Dwell", &fVslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider1, "1", "");
		ui_interface->declare(&fVslider1, "style", "knob");
		ui_interface->declare(&fVslider1, "tooltip", "Wet gain scaling; maps to 0..0.8 (0..1)");
		ui_interface->addVerticalSlider("Blend", &fVslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider2, "2", "");
		ui_interface->declare(&fVslider2, "style", "knob");
		ui_interface->declare(&fVslider2, "tooltip", "Lowpass cutoff on wet path; darker to brighter (0..1)");
		ui_interface->addVerticalSlider("Tone", &fVslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider4, "3", "");
		ui_interface->declare(&fVslider4, "style", "knob");
		ui_interface->declare(&fVslider4, "tooltip", "Base delay time; affects predelay and tail diffusion (0..1)");
		ui_interface->addVerticalSlider("Tension", &fVslider4, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fEntry0, "4", "");
		ui_interface->declare(&fEntry0, "style", "menu{'Left':0;'Right':1;'Middle':2}");
		ui_interface->declare(&fEntry0, "tooltip", "Spacing preset between spring delays");
		ui_interface->addNumEntry("Springs", &fEntry0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fVslider0, "5", "");
		ui_interface->declare(&fVslider0, "style", "knob");
		ui_interface->declare(&fVslider0, "tooltip", "Wet/dry mix (0=dry, 1=wet)");
		ui_interface->addVerticalSlider("Wet", &fVslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = static_cast<float>(fVslider0);
		float fSlow1 = 1.0f - fSlow0;
		float fSlow2 = std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * static_cast<float>(fVslider2)));
		float fSlow3 = fSlow2 * (1.9e+02f * fSlow2 + 5e+01f) + 1.5e+03f;
		float fSlow4 = 0.04f * std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * static_cast<float>(fVslider1))) * std::min<float>(5.0f, 2e+03f / fSlow3 + 1.0f);
		float fSlow5 = 1.0f / std::tan(fConst4 * fSlow3);
		float fSlow6 = 1.0f / (fSlow5 + 1.0f);
		float fSlow7 = 1.0f - fSlow5;
		float fSlow8 = std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * static_cast<float>(fVslider3)));
		float fSlow9 = fSlow8 * (0.013f - 0.0006f * fSlow8) + 0.26f;
		float fSlow10 = std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * static_cast<float>(fVslider4)));
		float fSlow11 = fSlow10 * (0.00032f * fSlow10 + -0.0072f);
		float fSlow12 = std::max<float>(0.0f, std::min<float>(2.0f, std::round(static_cast<float>(fEntry0))));
		float fSlow13 = ((fSlow12 == 0.0f) ? 2e-06f : ((fSlow12 == 1.0f) ? 0.0005f : 2.8e-05f));
		int iSlow14 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + fSlow13 + 0.07f)))));
		int iSlow15 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + 2.0f * fSlow13 + 0.07f)))));
		int iSlow16 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + 3.0f * fSlow13 + 0.07f)))));
		int iSlow17 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + 5.0f * fSlow13 + 0.07f)))));
		int iSlow18 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + 7.0f * fSlow13 + 0.07f)))));
		int iSlow19 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + 11.0f * fSlow13 + 0.07f)))));
		int iSlow20 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + 13.0f * fSlow13 + 0.07f)))));
		int iSlow21 = static_cast<int>(std::min<float>(fConst31, std::max<float>(0.0f, std::round(fConst0 * (fSlow11 + 17.0f * fSlow13 + 0.07f)))));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = static_cast<float>(input0[i0]);
			fVec0[IOTA0 & 8191] = fTemp0;
			float fTemp1 = fRec1[1] + fRec5[1];
			float fTemp2 = fRec7[1] + fTemp1 + fRec3[1];
			float fTemp3 = fVec0[(IOTA0 - iConst7) & 8191];
			float fTemp4 = fVec0[(IOTA0 - iConst8) & 8191];
			float fTemp5 = fTemp3 + fTemp4;
			float fTemp6 = fVec0[(IOTA0 - iConst9) & 8191];
			float fTemp7 = fVec0[(IOTA0 - iConst10) & 8191];
			float fTemp8 = fTemp6 + fTemp7;
			float fTemp9 = fTemp5 + fTemp8;
			float fTemp10 = fVec0[(IOTA0 - iConst11) & 8191];
			float fTemp11 = fVec0[(IOTA0 - iConst12) & 8191];
			float fTemp12 = fTemp10 + fTemp11;
			float fTemp13 = fVec0[(IOTA0 - iConst13) & 8191];
			float fTemp14 = fVec0[(IOTA0 - iConst14) & 8191];
			float fTemp15 = fTemp13 + fTemp14;
			float fTemp16 = fTemp12 + fTemp15;
			fVec1[IOTA0 & 2047] = fTemp9 + fTemp16;
			float fTemp17 = fVec1[(IOTA0 - iConst15) & 2047];
			float fTemp18 = fTemp3 - fTemp4;
			float fTemp19 = fTemp6 - fTemp7;
			float fTemp20 = fTemp18 + fTemp19;
			float fTemp21 = fTemp10 - fTemp11;
			float fTemp22 = fTemp13 - fTemp14;
			float fTemp23 = fTemp21 + fTemp22;
			fVec2[IOTA0 & 4095] = fTemp20 + fTemp23;
			float fTemp24 = fVec2[(IOTA0 - iConst16) & 4095];
			float fTemp25 = fTemp17 - fTemp24;
			float fTemp26 = fTemp5 - fTemp8;
			float fTemp27 = fTemp12 - fTemp15;
			fVec3[IOTA0 & 4095] = fTemp26 + fTemp27;
			float fTemp28 = fVec3[(IOTA0 - iConst17) & 4095];
			float fTemp29 = fTemp18 - fTemp19;
			float fTemp30 = fTemp21 - fTemp22;
			fVec4[IOTA0 & 8191] = fTemp29 + fTemp30;
			float fTemp31 = fVec4[(IOTA0 - iConst18) & 8191];
			float fTemp32 = fTemp28 - fTemp31;
			float fTemp33 = fTemp25 + fTemp32;
			fVec5[IOTA0 & 4095] = fTemp9 - fTemp16;
			float fTemp34 = fVec5[(IOTA0 - iConst19) & 4095];
			fVec6[IOTA0 & 8191] = fTemp20 - fTemp23;
			float fTemp35 = fVec6[(IOTA0 - iConst20) & 8191];
			float fTemp36 = fTemp34 - fTemp35;
			fVec7[IOTA0 & 4095] = fTemp26 - fTemp27;
			float fTemp37 = fVec7[(IOTA0 - iConst21) & 4095];
			fVec8[IOTA0 & 8191] = fTemp29 - fTemp30;
			float fTemp38 = fVec8[(IOTA0 - iConst22) & 8191];
			float fTemp39 = fTemp37 - fTemp38;
			float fTemp40 = fTemp36 - fTemp39;
			fVec9[IOTA0 & 2047] = fTemp33 - fTemp40;
			float fTemp41 = fVec9[(IOTA0 - iConst23) & 2047];
			float fTemp42 = fTemp17 + fTemp24;
			float fTemp43 = fTemp28 + fTemp31;
			float fTemp44 = fTemp42 + fTemp43;
			float fTemp45 = fTemp34 + fTemp35;
			float fTemp46 = fTemp37 + fTemp38;
			float fTemp47 = fTemp45 - fTemp46;
			fVec10[IOTA0 & 4095] = fTemp44 - fTemp47;
			float fTemp48 = fVec10[(IOTA0 - iConst24) & 4095];
			float fTemp49 = fTemp41 - fTemp48;
			float fTemp50 = fTemp25 - fTemp32;
			float fTemp51 = fTemp36 + fTemp39;
			fVec11[IOTA0 & 4095] = fTemp50 - fTemp51;
			float fTemp52 = fVec11[(IOTA0 - iConst25) & 4095];
			float fTemp53 = fTemp42 - fTemp43;
			float fTemp54 = fTemp45 + fTemp46;
			fVec12[IOTA0 & 4095] = fTemp53 - fTemp54;
			float fTemp55 = fVec12[(IOTA0 - iConst26) & 4095];
			float fTemp56 = fTemp52 + fTemp55;
			float fTemp57 = fTemp49 - fTemp56;
			fVec13[IOTA0 & 4095] = fTemp33 + fTemp40;
			float fTemp58 = fVec13[(IOTA0 - iConst27) & 4095];
			fVec14[IOTA0 & 4095] = fTemp44 + fTemp47;
			float fTemp59 = fVec14[(IOTA0 - iConst28) & 4095];
			float fTemp60 = fTemp58 + fTemp59;
			fVec15[IOTA0 & 4095] = fTemp50 + fTemp51;
			float fTemp61 = fVec15[(IOTA0 - iConst29) & 4095];
			fVec16[IOTA0 & 8191] = fTemp53 + fTemp54;
			float fTemp62 = fVec16[(IOTA0 - iConst30) & 8191];
			float fTemp63 = fTemp61 + fTemp62;
			float fTemp64 = fTemp60 - fTemp63;
			fVec17[IOTA0 & 2047] = fTemp57 - fTemp64;
			fVec18[IOTA0 & 16383] = fSlow9 * (fRec8[1] + fRec4[1] + fRec6[1] + fTemp2 + fRec2[1]) - 0.01f * fVec17[(IOTA0 - iConst15) & 2047];
			float fTemp65 = fVec18[(IOTA0 - iSlow14) & 16383];
			fVec19[0] = fTemp65;
			fRec9[0] = -(fSlow6 * (fSlow7 * fRec9[1] - (fTemp65 + fVec19[1])));
			float fTemp66 = fRec9[0] - fRec9[1];
			fRec1[0] = ((std::fabs(fTemp66) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec9[0] + fRec9[1]))) : ((((fRec9[0] <= 1.0f) & (fRec9[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec9[0]) : fRec9[0] * static_cast<float>((fRec9[0] > 0.0f) - (fRec9[0] < 0.0f)) + -0.5f) - (((fRec9[1] <= 1.0f) & (fRec9[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec9[1]) : fRec9[1] * static_cast<float>((fRec9[1] > 0.0f) - (fRec9[1] < 0.0f)) + -0.5f)) / fTemp66);
			fVec20[IOTA0 & 4095] = fTemp57 + fTemp64;
			fVec21[IOTA0 & 16383] = fSlow9 * (fTemp2 - (fRec8[1] + fRec2[1] + fRec6[1] + fRec4[1])) + 0.01f * fVec20[(IOTA0 - iConst27) & 4095];
			float fTemp67 = fVec21[(IOTA0 - iSlow15) & 16383];
			fVec22[0] = fTemp67;
			fRec10[0] = -(fSlow6 * (fSlow7 * fRec10[1] - (fTemp67 + fVec22[1])));
			float fTemp68 = fRec10[0] - fRec10[1];
			fRec2[0] = ((std::fabs(fTemp68) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec10[0] + fRec10[1]))) : ((((fRec10[0] <= 1.0f) & (fRec10[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec10[0]) : fRec10[0] * static_cast<float>((fRec10[0] > 0.0f) - (fRec10[0] < 0.0f)) + -0.5f) - (((fRec10[1] <= 1.0f) & (fRec10[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec10[1]) : fRec10[1] * static_cast<float>((fRec10[1] > 0.0f) - (fRec10[1] < 0.0f)) + -0.5f)) / fTemp68);
			float fTemp69 = fRec3[1] + fRec7[1];
			float fTemp70 = fTemp49 + fTemp56;
			float fTemp71 = fTemp60 + fTemp63;
			fVec23[IOTA0 & 4095] = fTemp70 - fTemp71;
			fVec24[IOTA0 & 16383] = fSlow9 * (fRec6[1] + fTemp1 + fRec2[1] - (fRec8[1] + fTemp69 + fRec4[1])) + 0.01f * fVec23[(IOTA0 - iConst32) & 4095];
			float fTemp72 = fVec24[(IOTA0 - iSlow16) & 16383];
			fVec25[0] = fTemp72;
			fRec11[0] = -(fSlow6 * (fSlow7 * fRec11[1] - (fTemp72 + fVec25[1])));
			float fTemp73 = fRec11[0] - fRec11[1];
			fRec3[0] = ((std::fabs(fTemp73) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec11[0] + fRec11[1]))) : ((((fRec11[0] <= 1.0f) & (fRec11[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec11[0]) : fRec11[0] * static_cast<float>((fRec11[0] > 0.0f) - (fRec11[0] < 0.0f)) + -0.5f) - (((fRec11[1] <= 1.0f) & (fRec11[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec11[1]) : fRec11[1] * static_cast<float>((fRec11[1] > 0.0f) - (fRec11[1] < 0.0f)) + -0.5f)) / fTemp73);
			fVec26[IOTA0 & 4095] = fTemp70 + fTemp71;
			fVec27[IOTA0 & 16383] = fSlow9 * (fRec8[1] + fTemp1 + fRec4[1] - (fRec6[1] + fTemp69 + fRec2[1])) - 0.01f * fVec26[(IOTA0 - iConst33) & 4095];
			float fTemp74 = fVec27[(IOTA0 - iSlow17) & 16383];
			fVec28[0] = fTemp74;
			fRec12[0] = -(fSlow6 * (fSlow7 * fRec12[1] - (fTemp74 + fVec28[1])));
			float fTemp75 = fRec12[0] - fRec12[1];
			fRec4[0] = ((std::fabs(fTemp75) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec12[0] + fRec12[1]))) : ((((fRec12[0] <= 1.0f) & (fRec12[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec12[0]) : fRec12[0] * static_cast<float>((fRec12[0] > 0.0f) - (fRec12[0] < 0.0f)) + -0.5f) - (((fRec12[1] <= 1.0f) & (fRec12[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec12[1]) : fRec12[1] * static_cast<float>((fRec12[1] > 0.0f) - (fRec12[1] < 0.0f)) + -0.5f)) / fTemp75);
			float fTemp76 = fRec1[1] + fRec3[1];
			float fTemp77 = fRec5[1] + fRec7[1];
			float fTemp78 = fTemp41 + fTemp48;
			float fTemp79 = fTemp52 - fTemp55;
			float fTemp80 = fTemp78 - fTemp79;
			float fTemp81 = fTemp58 - fTemp59;
			float fTemp82 = fTemp61 - fTemp62;
			float fTemp83 = fTemp81 - fTemp82;
			fVec29[IOTA0 & 4095] = fTemp80 - fTemp83;
			fVec30[IOTA0 & 16383] = fSlow9 * (fRec4[1] + fRec2[1] + fTemp76 - (fRec8[1] + fRec6[1] + fTemp77)) - 0.01f * fVec29[(IOTA0 - iConst34) & 4095];
			float fTemp84 = fVec30[(IOTA0 - iSlow18) & 16383];
			fVec31[0] = fTemp84;
			fRec13[0] = -(fSlow6 * (fSlow7 * fRec13[1] - (fTemp84 + fVec31[1])));
			float fTemp85 = fRec13[0] - fRec13[1];
			fRec5[0] = ((std::fabs(fTemp85) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec13[0] + fRec13[1]))) : ((((fRec13[0] <= 1.0f) & (fRec13[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec13[0]) : fRec13[0] * static_cast<float>((fRec13[0] > 0.0f) - (fRec13[0] < 0.0f)) + -0.5f) - (((fRec13[1] <= 1.0f) & (fRec13[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec13[1]) : fRec13[1] * static_cast<float>((fRec13[1] > 0.0f) - (fRec13[1] < 0.0f)) + -0.5f)) / fTemp85);
			fVec32[IOTA0 & 8191] = fTemp80 + fTemp83;
			fVec33[IOTA0 & 16383] = fSlow9 * (fRec8[1] + fRec6[1] + fTemp76 - (fRec4[1] + fRec2[1] + fTemp77)) - 0.01f * fVec32[(IOTA0 - iConst35) & 8191];
			float fTemp86 = fVec33[(IOTA0 - iSlow19) & 16383];
			fVec34[0] = fTemp86;
			fRec14[0] = -(fSlow6 * (fSlow7 * fRec14[1] - (fTemp86 + fVec34[1])));
			float fTemp87 = fRec14[0] - fRec14[1];
			fRec6[0] = ((std::fabs(fTemp87) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec14[0] + fRec14[1]))) : ((((fRec14[0] <= 1.0f) & (fRec14[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec14[0]) : fRec14[0] * static_cast<float>((fRec14[0] > 0.0f) - (fRec14[0] < 0.0f)) + -0.5f) - (((fRec14[1] <= 1.0f) & (fRec14[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec14[1]) : fRec14[1] * static_cast<float>((fRec14[1] > 0.0f) - (fRec14[1] < 0.0f)) + -0.5f)) / fTemp87);
			float fTemp88 = fRec1[1] + fRec7[1];
			float fTemp89 = fRec5[1] + fRec3[1];
			float fTemp90 = fTemp78 + fTemp79;
			float fTemp91 = fTemp81 + fTemp82;
			fVec35[IOTA0 & 8191] = fTemp90 - fTemp91;
			fVec36[IOTA0 & 16383] = fSlow9 * (fRec8[1] + fRec2[1] + fTemp88 - (fRec4[1] + fRec6[1] + fTemp89)) + 0.01f * fVec35[(IOTA0 - iConst36) & 8191];
			float fTemp92 = fVec36[(IOTA0 - iSlow20) & 16383];
			fVec37[0] = fTemp92;
			fRec15[0] = -(fSlow6 * (fSlow7 * fRec15[1] - (fTemp92 + fVec37[1])));
			float fTemp93 = fRec15[0] - fRec15[1];
			fRec7[0] = ((std::fabs(fTemp93) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec15[0] + fRec15[1]))) : ((((fRec15[0] <= 1.0f) & (fRec15[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec15[0]) : fRec15[0] * static_cast<float>((fRec15[0] > 0.0f) - (fRec15[0] < 0.0f)) + -0.5f) - (((fRec15[1] <= 1.0f) & (fRec15[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec15[1]) : fRec15[1] * static_cast<float>((fRec15[1] > 0.0f) - (fRec15[1] < 0.0f)) + -0.5f)) / fTemp93);
			fVec38[IOTA0 & 8191] = fTemp90 + fTemp91;
			fVec39[IOTA0 & 16383] = fSlow9 * (fRec4[1] + fRec6[1] + fTemp88 - (fRec8[1] + fRec2[1] + fTemp89)) - 0.01f * fVec38[(IOTA0 - iConst37) & 8191];
			float fTemp94 = fVec39[(IOTA0 - iSlow21) & 16383];
			fVec40[0] = fTemp94;
			fRec16[0] = -(fSlow6 * (fSlow7 * fRec16[1] - (fTemp94 + fVec40[1])));
			float fTemp95 = fRec16[0] - fRec16[1];
			fRec8[0] = ((std::fabs(fTemp95) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec16[0] + fRec16[1]))) : ((((fRec16[0] <= 1.0f) & (fRec16[0] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec16[0]) : fRec16[0] * static_cast<float>((fRec16[0] > 0.0f) - (fRec16[0] < 0.0f)) + -0.5f) - (((fRec16[1] <= 1.0f) & (fRec16[1] >= -1.0f)) ? 0.5f * mydsp_faustpower2_f(fRec16[1]) : fRec16[1] * static_cast<float>((fRec16[1] > 0.0f) - (fRec16[1] < 0.0f)) + -0.5f)) / fTemp95);
			float fTemp96 = fRec1[0] + fRec2[0] + fRec3[0] + fRec4[0] + fRec5[0] + fRec6[0] + fRec7[0] + fRec8[0];
			fVec41[0] = fTemp96;
			fRec0[0] = -(fConst2 * (fConst3 * fRec0[1] - fConst1 * (fTemp96 - fVec41[1])));
			output0[i0] = static_cast<FAUSTFLOAT>(fSlow1 * fTemp0 + fSlow0 * (fTemp0 + fSlow4 * fRec0[0]));
			IOTA0 = IOTA0 + 1;
			fVec19[1] = fVec19[0];
			fRec9[1] = fRec9[0];
			fRec1[1] = fRec1[0];
			fVec22[1] = fVec22[0];
			fRec10[1] = fRec10[0];
			fRec2[1] = fRec2[0];
			fVec25[1] = fVec25[0];
			fRec11[1] = fRec11[0];
			fRec3[1] = fRec3[0];
			fVec28[1] = fVec28[0];
			fRec12[1] = fRec12[0];
			fRec4[1] = fRec4[0];
			fVec31[1] = fVec31[0];
			fRec13[1] = fRec13[0];
			fRec5[1] = fRec5[0];
			fVec34[1] = fVec34[0];
			fRec14[1] = fRec14[0];
			fRec6[1] = fRec6[0];
			fVec37[1] = fVec37[0];
			fRec15[1] = fRec15[0];
			fRec7[1] = fRec7[0];
			fVec40[1] = fVec40[0];
			fRec16[1] = fRec16[0];
			fRec8[1] = fRec8[0];
			fVec41[1] = fVec41[0];
			fRec0[1] = fRec0[0];
		}
	}

};

#endif
