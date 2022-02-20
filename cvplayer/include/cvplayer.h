#pragma once

#include "deflate.h"
#include "defstrct.h"

namespace cvp {

	class cvplayer {

	private:
		Mat	src, dst;

		String	bef_win_text = "no encode.",
				aft_win_text = "encoded.";

		int vals_count = 0;
		int vals[CVP_MAX_TRACKBAR];

		void MatCheck(Mat* _src) {

			if (_src->empty()) {
				printf("mat is null.\n");
				throw;
			}

		}

		static void TrackbarEvent(int val, void* userdata)
		{
			*(int*)(userdata) = val;
		}

	public:

		cvplayer(const char* filepath) {

			src = imread(filepath);
			MatCheck(&src);

		}
		cvplayer(Mat* _src) {

			MatCheck(_src);
			src = _src->clone();

		}

		void AddSlider(sliderdata data) {

			if (vals_count == CVP_MAX_TRACKBAR - 1) {
				printf("trackbar is max over.\n");
				return;
			}

			vals[vals_count] = data.def;
			createTrackbar(data.name, aft_win_text, &vals[vals_count], data.max, TrackbarEvent, (void*)(&vals[vals_count]));
			vals_count++;

		}

		int GetSliderValue(int num) {

			if (vals_count < num || CVP_MAX_TRACKBAR <= num) return 0;
			return vals[num];

		}

		void MainLoop(FrameCallback framecb, void* data = 0) {

			dst = src.clone();

			imshow(bef_win_text, src);
			namedWindow(aft_win_text);

			while (1) {

				framecb(src, &dst, (void*)this, data);
				imshow(aft_win_text, dst);

				if (waitKey(30) == 27) break;

			}

			src.release();
			dst.release();
			destroyAllWindows();

		}

	};

}
