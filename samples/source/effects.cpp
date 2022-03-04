#include "effects.h"

int Binary_valueNum;

void Binary_effect(Mat src, Mat* dst, void* player, void* data) {

	cvplayer* p = reinterpret_cast<cvplayer*>(player);

	cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
	cv::threshold(src, *dst, p->GetSliderValue(Binary_valueNum), 0xFF, cv::THRESH_BINARY);

}

void Binary(void) {

	cvplayer player(IMG);
	Binary_valueNum = player.AddSlider(sliderdata("binary", 100, 0xFF));

	//player.MainLoop(frameCallBack, 0);
	player.MainLoop(Binary_effect, 0);

	return;

}
