#pragma once

#define IMG	"F:\\SS\\‰æ‘œ148.jpg"

#include "cvplayer.h"

using namespace cvp;

void Binary_effect(Mat src, Mat* dst, void* player, void* data);
void Binary_keyCallBack(int keycode, void* _player);
void Binary(void);
