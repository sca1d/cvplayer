#include "../include/cvplayer.h"

namespace cvp {

	void cvplayer::MatCheck(Mat* _src) {

		if (_src->empty()) {
			printf("mat is null.\n");
			throw;
		}

	}

	void cvplayer::TrackbarEvent(int val, void* userdata)
	{
		*(int*)(userdata) = val;
	}

}
