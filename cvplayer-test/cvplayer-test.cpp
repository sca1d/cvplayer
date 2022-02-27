#include <ppl.h>

#include "cvplayer.h"

#define IMG	"F:\\SS\\画像148.jpg"

using namespace cvp;

void frameCallBack(Mat src, Mat* dst, void* player, void* data) {

	Concurrency::parallel_for(0, src.rows, [&](int y) {
		for (int x = 0; x < src.cols; x++) {
			for (int c = 0; c < src.channels(); c++) {

				*dst->ptr<Vec3b>(y, x)[c].val = *src.ptr<Vec3b>(y, x)[c].val + rand();

			}
		}
	});

}

int main(void) {

	cvplayer player(IMG);
	player.AddSlider({ "slider1", 10, 100 });
	player.MainLoop(frameCallBack);

	return 0;

}