#include "cvplayer.h"

#include "effects.h"

using namespace cvp;

void frameCallBack(Mat src, Mat* dst, input_data* input) {

	int xx, yy;

	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			for (int c = 0; c < src.channels(); c++) {

				xx = x + input->slider_data->GetSliderValue(0);
				yy = y;

				if (0 <= xx && xx < src.cols && 0 <= yy && yy < src.rows) {
					dst->ptr<Vec3b>(y, x)[c] = src.ptr<Vec3b>(yy, xx)[c];
				}

			}
		}
	}

}

void keyCallBack(int keycode, cvplayer* player) {

	switch (keycode) {

	case 'r':
		keydomain kd[1] = { { 5, 90 } };

		int ret = player->Encode((frameCallBack), "video.mov", ENC_MOV, kd, 30.0, 30 * 5);
		printf("ret:%d\n", ret);

	}

}

int main(void) {
	
	/*
	cvplayer player(IMG);
	player.AddKeyEvent(keyCallBack);
	player.AddSlider(sliderdata("slider1", 10, 100));

	player.MainLoop(frameCallBack, 0);
	*/
	
	//Binary();
	ColorBalance();

	return 0;

}
