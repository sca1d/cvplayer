#include "../include/cvplayer.h"

namespace cvp {

	slider_info::slider_info(void) {

		vals = (sliderdata*)malloc(sizeof(sliderdata) * CVP_MAX_TRACKBAR);

	}

	slider_info::~slider_info(void) {

		free(vals);

	}

	inline void cvplayer::InitVals(void) {

		#pragma omp parallel for
		for (int i = 0; i < CVP_MAX_TRACKBAR; i++) {
			vals[i].value	= 0;
			vals[i].def		= 0;
			vals[i].max		= 0;
		}

	}

	inline void cvplayer::MatCheck(Mat* _src) {

		if (_src->empty()) {
			printf("mat is null.\n");
			throw;
		}

	}

	int cvplayer::WaitFunc(int time) {

		int keycode = waitKey(time);

		// 0x20 = SPACE KEY
		if (keycode == 0x20) PlayReverse(&this->edata);

		if (keyCallBack != nullptr) {
			keyCallBack(keycode, this);
		}

		return keycode;

	}

	void cvplayer::PlayModeLog(void) {

		CVP_FIX_CHANNEL(dst);

		if (play == 1) {
			
			const char* text = "Play";

			cv::putText(
				dst,
				text,
				cv::Point(10, 15),
				cv::FONT_HERSHEY_PLAIN,
				1.0,
				cv::Scalar(0, 255, 0),
				1
			);

		}
		else {

			const char* text = "Stop";

			cv::putText(
				dst,
				text,
				cv::Point(10, 15),
				cv::FONT_HERSHEY_PLAIN,
				1.0,
				cv::Scalar(0, 0, 255),
				1
			);

		}

	}

	void cvplayer::EncodingLog(int now, int length) {

		CVP_FIX_CHANNEL(dst);

		char text[32];
		sprintf_s(text, 32, "encoding : %df / %df", now, length);

		cv::putText(
			dst,
			text,
			cv::Point(30, 30),
			cv::FONT_HERSHEY_PLAIN,
			1.0,
			cv::Scalar(0, 0, 255),
			1
		);

	}

	void cvplayer::PlayReverse(eventdata* data) {

		*data->play = *data->play > 0 ? 0 : 1;
		*data->update = 1;

	}

	void cvplayer::TrackbarEvent(int val, void* userdata) {

		eventdata* edata = reinterpret_cast<eventdata*>(userdata);
		*edata->val = val;
		*edata->update = 1;
		//*(int*)(userdata) = val;

	}

	void cvplayer::MouseEvent(int e, int x, int y, int flag, void* userdata) {

		if (e == EVENT_LBUTTONDOWN) {
			eventdata* data = reinterpret_cast<eventdata*>(userdata);
			PlayReverse(data);
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
	cvplayer::~cvplayer(void) {

		src.release();
		dst.release();

	}

	void cvplayer::AddKeyEvent(KeyCallBack keycb) {

		if (keycb != nullptr) {
			keyCallBack = keycb;
		}

	}

	int cvplayer::AddSlider(sliderdata data) {

		if (vals_count == CVP_MAX_TRACKBAR - 1) {
			printf("trackbar is max over.\n");
			return -1;
		}

		vals[vals_count].value	= data.def;
		vals[vals_count].def	= data.def;
		createTrackbar(data.name, aft_win_text, &(vals[vals_count].value), data.max, TrackbarEvent, (void*)(&edata));
		vals_count++;

		return vals_count - 1;

	}

	int cvplayer::GetSliderValue(int num) const {

		if (vals_count < num || CVP_MAX_TRACKBAR <= num) return 0;
		return vals[num].value;

	}
	int cvplayer::GetSliderValue(char* name) const {

		for (int i = 0; i < vals_count; i++) {
			if (strcmp(vals[i].name.c_str(), name)) {
				return vals[i].value;
			}
		}

		return 0;

	}

	bool cvplayer::GetPlayMode(void) const {
		return play;
	}
	void cvplayer::Play(void) {
		play = 1;
		update = 1;
	}
	void cvplayer::Stop(void) {
		play = 0;
		update = 1;
	}

	bool cvplayer::Encode(effectFunc effect, String filename, encode_type type, keydomain* valueKey, double fps, int frameLength) {
		
		if (nowEncode) {

			int isStop = MessageBox(
				NULL,
				L"Cancel the encoding ?",
				L"cvplayer",
				MB_YESNO | MB_ICONQUESTION
			);

			if (isStop != IDYES) {
				return -1;
			}

		}

		namedWindow(enc_win_text);

		int width	= src.cols,
			height	= src.rows,
			color	= src.channels();

		nowEncode	= true;

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
			
			input_data input;
			input.current_frame = 0;
			input.slider_data = (slider_info*)this;
			input.data = effect.data;

			for (int f = 0; f <= frameLength; f++) {

				dst = src.clone();

				#pragma omp parallel for
				for (int i = 0; i < vals_count; i++) {
					vals[i].value = f > 0 ? ((valueKey[i].end - valueKey[i].start) * (1.0 / ((double)frameLength / (double)f)) + valueKey[i].start) : valueKey[i].start;
				}

				input.current_frame = f;
				effect.callBack(src, &dst, &input);
				CVP_FIX_CHANNEL(dst);
				output << dst;

				EncodingLog(f, frameLength);

				imshow(enc_win_text, dst);

				switch (WaitFunc(1)) {

				case 27:
				case 'q':

					int isStop = MessageBox(
						NULL,
						L"Cancel the encoding ?",
						L"cvplayer",
						MB_YESNO | MB_ICONQUESTION
					);

					if (isStop == IDYES) {

						destroyWindow(enc_win_text);
						output.release();
						remove(filename.c_str());

						return -1;
					}
					else break;

				}
			}

			#pragma omp parallel for
			for (int i = 0; i < vals_count; i++) {
				vals[i].value = vals[i].def;
			}

			MessageBox(NULL, L"finished encode!", L"cvplayer", NULL);
			destroyWindow(enc_win_text);

			bool ret = output.isOpened();
			output.release();

			nowEncode = false;

			return ret;

		}
		/*
		else if (type == ENC_GIF) {



		}
		*/

	}

	void cvplayer::MainLoop(FrameCallback framecb, bool copy_src, void* data) {

		int brk = 0;
		int frame = 0;

		int count = 0;
		float fps;
		int st = 0,
			et = 0;

		MatCheck(&src);

		dst = src.clone();

		imshow(bef_win_text, src);
		namedWindow(aft_win_text);
		setMouseCallback(aft_win_text, MouseEvent, (void*)(&edata));

		input_data input;
		input.current_frame = frame;
		input.slider_data = (slider_info*)this;
		input.data = data;

		while (1) {

			/*
			if (count == 0) {
				st = clock();
			}
			else if (count == 60) {
				et = clock();
				fps = 1000.f / ((et - st) / 60.f);
				printf("fps:%f\n", fps);
				count = 0;
				st = clock();
			}
			*/

			if (copy_src) dst = src.clone();

			if (play || update) {

				framecb(src, &dst, &input);

				PlayModeLog();

				imshow(aft_win_text, dst);

				if (play) {
					frame++;
					count++;
					input.current_frame = frame;
				}
			
			}

			update > 0 ? update = 0 : 0;

			switch (WaitFunc(30)) {

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
