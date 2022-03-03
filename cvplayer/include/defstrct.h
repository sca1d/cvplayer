#pragma once

#include "deflate.h"
#include "cvplayer.h"

namespace cvp {

	typedef void (*FrameCallback)(Mat src, Mat* dst, void* player, void* data);
	typedef void (*KeyCallBack)(int keycode, void* player);
	typedef void (*CudaFrameCallback)(cv::cuda::GpuMat src, cv::cuda::GpuMat* dst, void* player, void* data);

	typedef struct _effectFunc {
	
		FrameCallback callBack;
		void* data;

		_effectFunc(FrameCallback cb, void* d = nullptr) {
			callBack = cb;
			data = d;
		}

	}effectFunc;

	typedef struct _eventdata {

		int* val;
		int* play;
		int* update;

	}eventdata;

	typedef struct _sliderdata {

		String	name;
		int		value;
		int		def;
		int		max;

		_sliderdata(void) {

		}
		_sliderdata(String _name, int _def, int _max) {
			name	= _name;
			value	= _def;
			def		= _def;
			max		= _max;
		}

	}sliderdata;

	typedef struct _keydomain {

		int start;
		int end;

	}keydomain;

	enum {

		ENC_AVI = 0,
		ENC_MOV,
		//ENC_GIF,

	};

}
