#pragma once

#include <stdio.h>
#include <omp.h>
#include <Windows.h>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define DLL_EXPORT	__declspec(dllexport)

#define CVP_MAX_TRACKBAR	64

#define CVP_FIX_CHANNEL(_mat)	if (_mat.channels() < 3) cv::cvtColor(_mat, _mat, cv::COLOR_GRAY2BGR)

