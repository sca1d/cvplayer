#include "cvplayer.h"

using namespace cvp;

void frameCallBack(Mat src, Mat* dst, void* player, void* data) {

	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			for (int c = 0; c < src.channels(); c++) {

				dst->ptr<Vec3b>(y, x)[c] = src.ptr<Vec3b>(y, x)[c];

			}
		}
	}

}

int main(void) {

	cvplayer player("F:\\SS\\画像148.jpg");
	player.AddSlider({ "slider1", 10, 100 });
	player.MainLoop(frameCallBack);

	return 0;

}