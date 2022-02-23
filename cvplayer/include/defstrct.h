#pragma once

#include "deflate.h"
#include "cvplayer.h"

namespace cvp {

	typedef void (*FrameCallback)(Mat src, Mat* dst, void* player, void* data);
	typedef void (*CudaFrameCallback)(cv::cuda::GpuMat src, cv::cuda::GpuMat* dst, void* player, void* data);

	typedef struct _sliderdata{

		String	name;
		int		def;
		int		max;

	}sliderdata;

}
