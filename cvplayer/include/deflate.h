#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include "opencv2/core/cuda.hpp"

#include "..\imgctl\imgctl.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "imgctl.lib")

#define DLL_EXPORT	__declspec(dllexport)

#define CVP_MAX_TRACKBAR	64

using namespace std;
using namespace cv;
