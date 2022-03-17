#include "effects.h"

int CB_valueNum;
int Binary_valueNum;

void keyEvent(int keycode, cvplayer* player) {

	switch (keycode) {

	case 'r':
		keydomain kd[1] = { keydomain(0, 0xFF) };

		int ret = player->Encode((Binary_effect), "video.mov", ENC_MOV, kd, 30.0, 30 * 5);
		printf("ret:%d\n", ret);

	}

}

void ColorBalance_effect(Mat src, Mat* dst, input_data* input) {

	cvplayer* p = reinterpret_cast<cvplayer*>(input->player);

	cv::cvtColor(src, src, cv::COLOR_BGR2HSV);

	std::vector<cv::Mat> mat_channels;
	cv::split(src, mat_channels);

	int hue_min = 0;
	int hue_max = hue_min + p->GetSliderValue(CB_valueNum);

	int hsv_value;

	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {

			hsv_value = static_cast<int>(*mat_channels[0].ptr<uchar>(y, x));
			if (!(hue_min <= hsv_value) && (hsv_value <= hue_max)) {
				*mat_channels[1].ptr<uchar>(y, x) = 0;
			}

		}
	}

	cv::merge(mat_channels, src);
	cv::cvtColor(src, *dst, cv::COLOR_HSV2BGR);

}

void ColorBalance(void) {

	cvplayer player(IMG);
	CB_valueNum = player.AddSlider(sliderdata("Color Balance", 100, 0xFF));

	player.AddKeyEvent(keyEvent);

	player.MainLoop(ColorBalance_effect);

	return;

}

void Binary_effect(Mat src, Mat* dst, input_data* input) {

	cvplayer* p = reinterpret_cast<cvplayer*>(input->player);

	cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
	cv::threshold(src, *dst, p->GetSliderValue(Binary_valueNum), 0xFF, cv::THRESH_BINARY);

}

void Binary(void) {

	cvplayer player(IMG);
	Binary_valueNum = player.AddSlider(sliderdata("binary", 100, 0xFF));

	player.AddKeyEvent(keyEvent);
	
	player.MainLoop(Binary_effect);

	return;

}
