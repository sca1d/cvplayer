#pragma once

#include "deflate.h"
#include "defstrct.h"

namespace cvp {

	class cvplayer {

	protected:

		Mat	src, dst;

		String	bef_win_text = "no encode.",
				aft_win_text = "encoded.",
				enc_win_text = "encoding now...";

		int vals_count = 0;
		slidervals vals[CVP_MAX_TRACKBAR];

		/*
		* TRUE	= 1
		* FALSE	= 0
		*/
		int play = 1, update = 0;

		eventdata edata = { &vals[vals_count].value, &play, &update };

		void InitVals(void);
		void MatCheck(Mat* _src);

		void EncodingLog(int now, int length);

		static void TrackbarEvent(int val, void* userdata);
		static void MouseEvent(int e, int x, int y, int flag, void* userdata);

	public:

		cvplayer(void);
		cvplayer(const char* filepath);
		cvplayer(Mat* _src);

		void AddSlider(sliderdata data);

		int GetSliderValue(int num);

		bool Encode(effectFunc effect, String filename, int type, keydomain* valueKey, double fps, int frameLength);

		void MainLoop(FrameCallback framecb, void* data = 0, KeyCallBack keycb = nullptr);

	};

}
