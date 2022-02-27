#pragma once

#include "deflate.h"
#include "defstrct.h"

namespace cvp {

	class cvplayer {

	protected:

		Mat	src, dst;

		String	bef_win_text = "no encode.",
				aft_win_text = "encoded.";

		int vals_count = 0;
		int vals[CVP_MAX_TRACKBAR];

		void InitVals(void);
		void MatCheck(Mat* _src);

		static void TrackbarEvent(int val, void* userdata);

	public:

		cvplayer(void);
		cvplayer(const char* filepath);
		cvplayer(Mat* _src);

		void AddSlider(sliderdata data);

		int GetSliderValue(int num);

		void MainLoop(FrameCallback framecb, void* data = 0);

	};

}
