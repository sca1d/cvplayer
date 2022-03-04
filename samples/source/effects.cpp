#include "effects.h"

int Binary_valueNum;

void Binary_effect(Mat src, Mat* dst, void* player, void* data) {

	cvplayer* p = reinterpret_cast<cvplayer*>(player);

	cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
	cv::threshold(src, *dst, p->GetSliderValue(Binary_valueNum), 0xFF, cv::THRESH_BINARY);

}

void Binary_keyCallBack(int keycode, void* _player) {

	cvplayer* player = reinterpret_cast<cvplayer*>(_player);

	switch (keycode) {

	case 'r':
		keydomain kd[1] = { keydomain( 0, 0xFF ) };

		int ret = player->Encode((Binary_effect), "video.mov", ENC_MOV, kd, 30.0, 30 * 5);
		printf("ret:%d\n", ret);

	}

}

void Binary(void) {

	cvplayer player(IMG);
	Binary_valueNum = player.AddSlider(sliderdata("binary", 100, 0xFF));

	player.AddKeyEvent(Binary_keyCallBack);
	
	player.MainLoop(Binary_effect);

	return;

}
