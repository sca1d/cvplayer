#pragma once

#include "deflate.h"
#include "cvplayer.h"

namespace cvp {

	typedef void (*FrameCallback)(Mat src, Mat* dst, void* player, void* data);
	typedef void (*KeyCallBack)(int keycode);
	typedef void (*CudaFrameCallback)(cv::cuda::GpuMat src, cv::cuda::GpuMat* dst, void* player, void* data);

	typedef struct _eventdata {

		int* val;
		int* play;
		int* update;

	}eventdata;

	typedef struct _sliderdata{

		String	name;
		int		def;
		int		max;

	}sliderdata;

}
