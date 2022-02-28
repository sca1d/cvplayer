#include "../include/cvplayer.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

namespace cvp {

	void cvplayer::InitVals(void) {

		#pragma omp parallel for
		for (int i = 0; i < CVP_MAX_TRACKBAR; i++) {
			vals[i].value	= 0;
			vals[i].def		= 0;
			vals[i].max		= 0;
		}

	}

	void cvplayer::MatCheck(Mat* _src) {

		if (_src->empty()) {
			printf("mat is null.\n");
			throw;
		}

	}

	void cvplayer::EncodingLog(int now, int length) {

		char text[32];
		sprintf_s(text, 32, "encoding : %df / %df", now, length);
		cv::putText(
			dst,
			text,
			cv::Point(30, 30),
			cv::FONT_HERSHEY_PLAIN,
			1.0,
			cv::Scalar(0, 0, 255),
			1
		);

	}

	void cvplayer::TrackbarEvent(int val, void* userdata) {
		eventdata* edata = reinterpret_cast<eventdata*>(userdata);
		*edata->val = val;
		*edata->update = 1;
		//*(int*)(userdata) = val;
	}

	void cvplayer::MouseEvent(int e, int x, int y, int flag, void* userdata) {

		if (e == EVENT_LBUTTONDOWN) {
			*reinterpret_cast<eventdata*>(userdata)->play = *reinterpret_cast<eventdata*>(userdata)->play > 0 ? 0 : 1;
			//*(int*)(userdata) = (*(int*)(userdata)) > 0 ? 0 : 1;
		}

	}

	cvplayer::cvplayer(void) {
		InitVals();
		cv::namedWindow(aft_win_text);
	}
	cvplayer::cvplayer(const char* filepath) {

		src = imread(filepath);
		MatCheck(&src);
		InitVals();
		cv::namedWindow(aft_win_text);

	}
	cvplayer::cvplayer(Mat* _src) {

		MatCheck(_src);
		src = _src->clone();
		InitVals();
		cv::namedWindow(aft_win_text);

	}

	void cvplayer::AddSlider(sliderdata data) {

		if (vals_count == CVP_MAX_TRACKBAR - 1) {
			printf("trackbar is max over.\n");
			return;
		}

		vals[vals_count].value	= data.def;
		vals[vals_count].def	= data.def;
		createTrackbar(data.name, aft_win_text, &(vals[vals_count].value), data.max, TrackbarEvent, (void*)(&edata));
		vals_count++;

	}

	int cvplayer::GetSliderValue(int num) {

		if (vals_count < num || CVP_MAX_TRACKBAR <= num) return 0;
		return vals[num].value;

	}

	bool cvplayer::Encode(effectFunc effect, String filename, int type, keydomain* valueKey, double fps, int frameLength) {
		
		namedWindow(enc_win_text);

		int width	= src.cols,
			height	= src.rows,
			color	= src.channels();

		if (type == ENC_MOV || type == ENC_AVI) {

			int fourcc;
			switch (type) {

			case ENC_MOV:
				fourcc = cv::VideoWriter::fourcc('M', 'P', '4', 'V');
				break;

			case ENC_AVI:
				fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
				break;

			}

			cv::VideoWriter output(filename, fourcc, fps, cv::Size(width, height), src.channels() > 1 ? true : false);

			for (int f = 0; f <= frameLength; f++) {

				dst = src.clone();

				#pragma omp parallel for
				for (int i = 0; i < vals_count; i++) {
					vals[i].value = f > 0 ? ((valueKey[i].end - valueKey[i].start) * (1.0 / ((double)frameLength / (double)f)) + valueKey[i].start) : valueKey[i].start;
				}

				effect.callBack(src, &dst, this, effect.data);
				output << dst;

				EncodingLog(f, frameLength);

				imshow(enc_win_text, dst);

				int keycode = waitKey(1);

				switch (keycode) {

				case 27:
				case 'q':

					int isStop = MessageBox(
						NULL,
						L"Cancel the encoding ?",
						L"cvplayer",
						MB_YESNO | MB_ICONQUESTION
					);

					if (isStop == IDYES) {

						destroyWindow(enc_win_text);
						output.release();
						remove(filename.c_str());

						return -1;
					}
					else break;

				}
			}

			#pragma omp parallel for
			for (int i = 0; i < vals_count; i++) {
				vals[i].value = vals[i].def;
			}

			MessageBox(NULL, L"finished encode!", L"cvplayer", NULL);
			destroyWindow(enc_win_text);

			bool ret = output.isOpened();
			output.release();

			return ret;

		}
		else if (type == ENC_GIF) {

			AVFormatContext* format_context = nullptr;
			if (avformat_alloc_output_context2(&format_context, nullptr, "mp4", nullptr) < 0) printf("avformat_alloc_output2 failed\n");

			AVFrame** frames = new AVFrame * [frameLength];
			AVRational time_base{ frameLength, fps };

			AVIOContext* io_context = nullptr;
			if (avio_open(&io_context, filename.c_str(), AVIO_FLAG_WRITE) < 0) printf("avio_open failed\n");

			format_context->pb = io_context;

			const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
			if (codec == nullptr) printf("encoder not found ...\n");

			AVCodecContext* codec_context = avcodec_alloc_context3(codec);
			if (codec_context == nullptr) printf("avcodec_alloc_context3 failed\n");

			for (int f = 0; f < frameLength; f++) {

				dst = src.clone();

				#pragma omp parallel for
				for (int i = 0; i < vals_count; i++) {
					vals[i].value = f > 0 ? ((valueKey[i].end - valueKey[i].start) * (1.0 / ((double)frameLength / (double)f)) + valueKey[i].start) : valueKey[i].start;
				}

				effect.callBack(src, &dst, this, effect.data);

				AVFrame* frame = av_frame_alloc();

				int cvLinesizes[1];
				cvLinesizes[0] = dst.step1();

				av_image_alloc(frame->data, frame->linesize, width, height,
					AVPixelFormat::AV_PIX_FMT_YUV420P, 1);

				SwsContext* conversion = sws_getContext(
					width, height, AVPixelFormat::AV_PIX_FMT_BGR24, width, height,
					(AVPixelFormat)frame->format, SWS_FAST_BILINEAR, NULL, NULL, NULL);
				sws_scale(conversion, &dst.data, cvLinesizes, 0, height, frame->data, frame->linesize);
				sws_freeContext(conversion);

				frames[f] = frame;

			}

			// set picture properties
			AVFrame* first_frame = frames[0];
			codec_context->pix_fmt = (AVPixelFormat)first_frame->format;
			codec_context->width = first_frame->width;
			codec_context->height = first_frame->height;
			codec_context->field_order = AV_FIELD_PROGRESSIVE;
			codec_context->color_range = first_frame->color_range;
			codec_context->color_primaries = first_frame->color_primaries;
			codec_context->color_trc = first_frame->color_trc;
			codec_context->colorspace = first_frame->colorspace;
			codec_context->chroma_sample_location = first_frame->chroma_location;
			codec_context->sample_aspect_ratio = first_frame->sample_aspect_ratio;

			codec_context->time_base = time_base;

			if (format_context->oformat->flags & AVFMT_GLOBALHEADER) {
				codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
			}

			AVDictionary* codec_options = nullptr;
			av_dict_set(&codec_options, "preset", "medium", 0);
			av_dict_set(&codec_options, "crf", "22", 0);
			av_dict_set(&codec_options, "profile", "high", 0);
			av_dict_set(&codec_options, "level", "4.0", 0);

			if (avcodec_open2(codec_context, codec_context->codec, &codec_options) != 0) printf("avcodec_open2 failed\n");

			AVStream* stream = avformat_new_stream(format_context, codec);
			if (stream == NULL)printf("avformat_new_stream failed\n");

			stream->sample_aspect_ratio = codec_context->sample_aspect_ratio;
			stream->time_base = codec_context->time_base;
			if (avcodec_parameters_from_context(stream->codecpar, codec_context) < 0) printf("avcodec_parameters_from_context failed\n");

			if (avformat_write_header(format_context, nullptr) < 0) printf("avformat_write_header failed\n");

		}

	}

	void cvplayer::MainLoop(FrameCallback framecb, void* data, KeyCallBack keycb) {

		int brk = 0;

		MatCheck(&src);

		dst = src.clone();

		imshow(bef_win_text, src);
		namedWindow(aft_win_text);
		setMouseCallback(aft_win_text, MouseEvent, (void*)(&edata));

		while (1) {

			if (play || update) {
				framecb(src, &dst, this, data);
				imshow(aft_win_text, dst);
			}

			update > 0 ? update = 0 : 0 ;

			int keycode = waitKey(30);

			if (keycb != nullptr) {
				keycb(keycode);
			}

			switch (keycode) {

			case 27:
			case 'q':
				brk = 1;
				break;

			}

			if (brk) break;

		}

		src.release();
		dst.release();
		destroyAllWindows();

	}

}
