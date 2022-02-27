#include "../include/cvplayer.h"

namespace cvp {

	void cvplayer::InitVals(void) {

		#pragma omp parallel for
		for (int i = 0; i < CVP_MAX_TRACKBAR; i++) {
			vals[i] = 0;
		}

	}

	void cvplayer::MatCheck(Mat* _src) {

		if (_src->empty()) {
			printf("mat is null.\n");
			throw;
		}

	}

	void cvplayer::TrackbarEvent(int val, void* userdata)
	{
		*(int*)(userdata) = val;
	}

	cvplayer::cvplayer(void) {
		InitVals();
		cv::namedWindow(aft_win_text);
	}
	cvplayer::cvplayer(const char* filepath) {

		src = imread(filepath);
		MatCheck(&src);
		InitVals();
		cv::namedWindow(aft_win_text);

	}
	cvplayer::cvplayer(Mat* _src) {

		MatCheck(_src);
		src = _src->clone();
		InitVals();
		cv::namedWindow(aft_win_text);

	}

	void cvplayer::AddSlider(sliderdata data) {

		if (vals_count == CVP_MAX_TRACKBAR - 1) {
			printf("trackbar is max over.\n");
			return;
		}

		vals[vals_count] = data.def;
		createTrackbar(data.name, aft_win_text, &(vals[vals_count]), data.max, TrackbarEvent, (void*)(&vals[vals_count]));
		vals_count++;

	}

	int cvplayer::GetSliderValue(int num) {

		if (vals_count < num || CVP_MAX_TRACKBAR <= num) return 0;
		return vals[num];

	}

	void cvplayer::MainLoop(FrameCallback framecb, void* data) {

		MatCheck(&src);

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

}
