#include <cstdio>
#include <amp.h>
//#include <amp_graphics.h>
#include <vector>
#include "cvplayer.h"

#define IMG	"F:\\SS\\画像148.jpg"

using namespace concurrency;
//using namespace concurrency::graphics;
using namespace cvp;

void frameCallBack(Mat src, Mat* dst, input_data* input) {

	int xx, yy;

	//Concurrency::parallel_for(0, src.rows, [&](int y) {
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			for (int c = 0; c < src.channels(); c++) {

				xx = x + input->slider_data->GetSliderValue(0);
				yy = y;

				if (0 <= xx && xx < src.cols && 0 <= yy && yy < src.rows) {
					dst->ptr<Vec3b>(y, x)[c] = src.ptr<Vec3b>(yy, xx)[c];
				}
				//*dst->ptr<Vec3b>(y, x)[c].val = *src.ptr<Vec3b>(y, x)[c].val * rand();

			}
		}
	}
	//});

}

void keyCallBack(int keycode, cvplayer* player) {

	switch (keycode) {

	case 'r':
		keydomain kd[1] = { { 5, 90 } };

		int ret = player->Encode((frameCallBack), "video.mov", ENC_MOV, kd, 30.0, 30 * 5);
		printf("ret:%d\n", ret);

	}

}

void amp_test(void) {

	const int num = 1920;

	int aCPP[num] = {10};
	int bCPP[num];
	int sumCPP[num];

	array_view<int, 1> a(num, aCPP);
	array_view<int, 1> b(num, bCPP);
	array_view<int, 1> sum(num, sumCPP);

	parallel_for_each(
		//作成されたスレッド集合の演算領域を定義する
		sum.extent,
		[=](index<1> idx) restrict(amp) {
			sum[idx] = (int)a[0]++;
	}
	);

	for (int i = 0; i < num; i++) {
		printf("[%d] %d\n", i, sum[i]);
	}

}

int main(void) {

	//amp_test();

	///*
	cvplayer* player = new cvplayer(IMG);
	player->AddKeyEvent(keyCallBack);
	player->AddSlider(sliderdata("slider1", 10, 100));

	delete player;

	//player.MainLoop(frameCallBack, 0);
	//*/

	return 0;

}