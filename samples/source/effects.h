#pragma once

#define IMG	"F:\\SS\\‰æ‘œ148.jpg"

#include <opencv2/opencv.hpp>

#include "cvplayer.h"

using namespace cv;
using namespace cvp;

void keyEvent(int keycode, void* _player);

void ColorBalance_effect(Mat src, Mat* dst, void* player, void* data);
void ColorBalance(void);

void Binary_effect(Mat src, Mat* dst, void* player, void* data);
void Binary(void);
