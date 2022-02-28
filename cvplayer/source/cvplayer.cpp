#include "../include/cvplayer.h"

namespace cvp {

	void cvplayer::InitVals(void) {

		#pragma omp parallel for
		for (int i = 0; i < CVP_MAX_TRACKBAR; i++) {
			vals[i].value	= 0;
			vals[i].def		= 0;
			vals[i].max		= 0;
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

		vals[vals_count].value	= data.def;
		vals[vals_count].def	= data.def;
		createTrackbar(data.name, aft_win_text, &(vals[vals_count].value), data.max, TrackbarEvent, (void*)(&edata));
		vals_count++;

	}

	int cvplayer::GetSliderValue(int num) {

		if (vals_count < num || CVP_MAX_TRACKBAR <= num) return 0;
		return vals[num].value;

	}

	bool cvplayer::Encode(effectFunc effect, String filename, int type, keydomain* valueKey, double fps, int frameLength) {
		
		namedWindow(aft_win_text);

		int width	= src.cols,
			height	= src.rows,
			color	= src.channels();

		if (type == ENC_MOV || type == ENC_AVI) {

			int fourcc;
			switch (type) {

			case ENC_MOV:
				fourcc = cv::VideoWriter::fourcc('M', 'P', '4', 'V');
				break;

			case ENC_AVI:
				fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
				break;

			}

			cv::VideoWriter output(filename, fourcc, fps, cv::Size(width, height), src.channels() > 1 ? true : false);

			for (int f = 0; f < frameLength; f++) {

				dst = src.clone();

				#pragma omp parallel for
				for (int i = 0; i < vals_count; i++) {
					vals[i].value = f > 0 ? ((valueKey[i].end - valueKey[i].start) * (1.0 / ((double)frameLength / (double)f)) + valueKey[i].start) : valueKey[i].start;
				}

				effect.callBack(src, &dst, this, effect.data);
				imshow(aft_win_text, dst);

				output << dst;

			}

			#pragma omp parallel for
			for (int i = 0; i < vals_count; i++) {
				vals[i].value = vals[i].def;
			}

			return output.isOpened();

		}
		else if (type == ENC_GIF) {

			BITMAPINFO bitInfo;
			bitInfo.bmiHeader.biBitCount		= color * 8;
			bitInfo.bmiHeader.biWidth			= width;
			bitInfo.bmiHeader.biWidth			= -height;
			bitInfo.bmiHeader.biPlanes			= 1;
			bitInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
			bitInfo.bmiHeader.biCompression		= BI_RGB;
			bitInfo.bmiHeader.biClrImportant	= 0;
			bitInfo.bmiHeader.biClrUsed			= 0;
			bitInfo.bmiHeader.biSizeImage		= 0;
			bitInfo.bmiHeader.biXPelsPerMeter	= 0;
			bitInfo.bmiHeader.biYPelsPerMeter	= 0;

			LPDWORD lpPixel;
			lpPixel = (LPDWORD)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, width * height * color);

			for (int f = 0; f < frameLength; f++) {

				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {

						lpPixel[x + y * width] = dst.data[y * width + x * dst.elemSize()];

					}
				}

			}

		}

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
				framecb(src, &dst, this, data);
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
