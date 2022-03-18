#pragma once

#ifndef _INC_CVPLAYER

#include "deflate.h"
#include "defstrct.h"

namespace cvp {

	class slider_info {

	protected:
		int vals_count = 0;
		sliderdata vals[CVP_MAX_TRACKBAR];

	public:
		virtual ~slider_info(void);

		virtual int GetSliderValue(int num) const = 0;
		virtual int GetSliderValue(char* name) const = 0;

	};

	class cvplayer : public slider_info {

	protected:

		Mat	src, dst;

		String	bef_win_text = "input (src)",
				aft_win_text = "output (dst)",
				enc_win_text = "encoding now...";

		KeyCallBack keyCallBack = nullptr;

		/*
		* TRUE	= 1
		* FALSE	= 0
		*/
		int play = 1, update = 0;

		bool nowEncode = false;

		eventdata edata = { &vals[vals_count].value, &play, &update };

		inline void InitVals(void);
		inline void MatCheck(Mat* _src);
		int WaitFunc(int time);

		void PlayModeLog(void);
		void EncodingLog(int now, int length);

		static void PlayReverse(eventdata* data);
		static void TrackbarEvent(int val, void* userdata);
		static void MouseEvent(int e, int x, int y, int flag, void* userdata);

	public:

		cvplayer(void);
		cvplayer(const char* filepath);
		cvplayer(Mat* _src);

		void AddKeyEvent(KeyCallBack keycb);

		int AddSlider(sliderdata data);
		int GetSliderValue(int num) const;
		int GetSliderValue(char* name) const;

		inline void Play(void);
		inline void Stop(void);

		bool Encode(effectFunc effect, String filename, encode_type type, keydomain* valueKey, double fps, int frameLength);

		void MainLoop(FrameCallback framecb, bool copy_src = true, void* data = 0);

	};

}

#define _INC_CVPLAYER
#endif
