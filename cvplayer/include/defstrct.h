#pragma once

#include "deflate.h"
#include "cvplayer.h"

namespace cvp {

	typedef void (*FrameCallback)(Mat src, Mat* dst, void* player, void* data);

	typedef struct {

		String	name;
		int		def;
		int		max;

	}sliderdata;

}
