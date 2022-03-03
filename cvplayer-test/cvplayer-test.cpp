#include <ppl.h>

#include "cvplayer.h"

#define IMG	"F:\\SS\\画像148.jpg"

using namespace cvp;

void frameCallBack(Mat src, Mat* dst, void* player, void* data) {

	int xx, yy;

	//Concurrency::parallel_for(0, src.rows, [&](int y) {
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			for (int c = 0; c < src.channels(); c++) {

				xx = x + reinterpret_cast<cvplayer*>(player)->GetSliderValue(0);
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

void keyCallBack(int keycode, void* _player) {

	cvplayer* player = reinterpret_cast<cvplayer*>(_player);

	switch (keycode) {

	case 'r':
		keydomain kd[1] = { { 5, 90 } };

		int ret = player->Encode((frameCallBack), "video.mov", ENC_MOV, kd, 30.0, 30 * 5);
		printf("ret:%d\n", ret);

	}

}

int main(void) {

	cvplayer player(IMG);
	player.AddSlider({ "slider1", 10, 100 });

	player.MainLoop(frameCallBack, 0, keyCallBack);

	return 0;

}