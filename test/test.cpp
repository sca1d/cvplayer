#include "cvplayer.h"

using namespace cvp;

void frameCallBack(Mat src, Mat* dst, void* player, void* data) {



}

int main(void) {

	cvplayer player("F:\\SS\\画像148.jpg");
	player.MainLoop(frameCallBack);

	return 0;

}