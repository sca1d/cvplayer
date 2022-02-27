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

	void cvplayer::TrackbarEvent(int val, void* userdata) {
		eventdata* edata = reinterpret_cast<eventdata*>(userdata);
		*edata->val = val;
		*edata->update = 1;
		//*(int*)(userdata) = val;
	}

	void cvplayer::MouseEvent(int e, int x, int y, int flag, void* userdata) {

		if (e == EVENT_LBUTTONDOWN) {
			*reinterpret_cast<eventdata*>(userdata)->play = *reinterpret_cast<eventdata*>(userdata)->play > 0 ? 0 : 1;
			//*(int*)(userdata) = (*(int*)(userdata)) > 0 ? 0 : 1;
		}

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
		createTrackbar(data.name, aft_win_text, &(vals[vals_count]), data.max, TrackbarEvent, (void*)(&edata));
		vals_count++;

	}

	int cvplayer::GetSliderValue(int num) {

		if (vals_count < num || CVP_MAX_TRACKBAR <= num) return 0;
		return vals[num];

	}

	void cvplayer::MainLoop(FrameCallback framecb, void* data, KeyCallBack keycb) {

		int brk = 0;

		MatCheck(&src);

		dst = src.clone();

		imshow(bef_win_text, src);
		namedWindow(aft_win_text);
		setMouseCallback(aft_win_text, MouseEvent, (void*)(&edata));

		while (1) {

			if (play || update) {
				framecb(src, &dst, (void*)this, data);
				imshow(aft_win_text, dst);
			}

			update > 0 ? update = 0 : 0 ;

			int keycode = waitKey(30);

			if (keycb != nullptr) {
				keycb(keycode);
			}

			switch (keycode) {

			case 27:
			case 'q':
				brk = 1;
				break;

			}

			if (brk) break;

		}

		src.release();
		dst.release();
		destroyAllWindows();

	}

}
