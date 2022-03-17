#pragma once

#define IMG	"F:\\SS\\‰æ‘œ148.jpg"

#include <opencv2/opencv.hpp>

#include "cvplayer.h"

using namespace cv;
using namespace cvp;

void keyEvent(int keycode, cvplayer* player);

void ColorBalance_effect(Mat src, Mat* dst, input_data* input);
void ColorBalance(void);

void Binary_effect(Mat src, Mat* dst, input_data* input);
void Binary(void);
