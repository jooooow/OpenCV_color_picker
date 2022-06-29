#include "color_picker.h"

static void onMouse(int event, int x, int y, int flags, void* param)
{
	ColorPicker* picker = static_cast<ColorPicker*>(param);
	picker->OnMouse(event, x, y, flags);
}

ColorPicker::ColorPicker(int window_size, int tube_width, std::string name, cv::Vec3f hsv)
{
	if (window_size % 2 == 0)
		window_size += 1;
	this->window_size = window_size;
	this->tube_width = tube_width;
	this->name = name;
	this->hsv = hsv;
	this->bgr = HSV2BGR(this->hsv);

	GeneratePanel();
	cv::namedWindow(name);
	cv::setMouseCallback(name, onMouse, this);
}

void ColorPicker::OnMouse(int event, int x, int y, int flags)
{
	switch (event)
	{
	case cv::EVENT_LBUTTONDOWN:
	{
		if (isInWheel(x, y))
		{
			hsv = GetHSV(x, y);
			UpdatePanel();
		}
		else if (isInSquare(x, y))
		{
			bgr = panel.at<cv::Vec3b>(cv::Point(x, y));
			UpdateSample();
		}
	}
	}
}

void ColorPicker::GeneratePanel()
{
	panel = cv::Mat::ones(cv::Size(window_size, window_size), CV_8UC3);
	memset(panel.data, 127, window_size * window_size * sizeof(cv::Vec3b));

	for (int j = 0; j < window_size; j++)
	{
		for (int i = 0; i < window_size; i++)
		{
			if (isInWheel(i, j))
			{
				cv::Vec3f hsv = GetHSV(i, j);
				cv::Vec3b color = HSV2BGR(hsv);
				panel.at<cv::Vec3b>(cv::Point(i, j)) = color;
			}
		}
	}
	UpdatePanel();
	UpdateSample();
}

void ColorPicker::UpdatePanel()
{
	float h = hsv[0];
	int ssize = 1.4 * (window_size / 2 - tube_width);
	for (int j = 0; j <= ssize; j++)
	{
		float s = (float)j / ssize;
		for (int i = 0; i <= ssize; i++)
		{
			float v = (float)i / ssize;
			cv::Vec3b bgr = HSV2BGR(cv::Vec3f(h, s, v));
			panel.at<cv::Vec3b>(cv::Point(window_size / 2 - ssize / 2 + i, window_size / 2 - ssize / 2 + j)) = bgr;
		}
	}
}

void ColorPicker::UpdateSample()
{
	int sample_size = 0.146 * window_size;
	for (int j = 0; j < sample_size; j++)
	{
		for (int i = 0; i < sample_size; i++)
		{
			panel.at<cv::Vec3b>(cv::Point(i, j)) = bgr;
		}
	}
}

int ColorPicker::isInWheel(int i, int j)
{
	int R1 = window_size / 2;
	int R2 = R1 - tube_width;
	int dist2 = pow(i - R1, 2) + pow(j - R1, 2);
	if (dist2 <= R1 * R1 && dist2 > R2 * R2)
		return 1;

	return 0;
}

int ColorPicker::isInSquare(int x, int y)
{
	int ssize = 1.4 * (window_size / 2 - tube_width);

	if (abs(x - window_size / 2) <= ssize / 2 && abs(y - window_size / 2) <= ssize / 2)
		return 1;
	return 0;
}

cv::Vec3f ColorPicker::GetHSV(int i, int j)
{
	int R1 = window_size / 2;
	int R2 = R1 - tube_width;

	int dist2 = pow(i - R1, 2) + pow(j - R1, 2);
	float theta = acos((i - R1) / (float)(sqrt(dist2))) / (3.1415926) * 180.0F;
	if (-(j - R1) > 0)
		theta = 360 - theta;

	float H = theta;
	float S = 1.0;
	float V = 1.0;

	return cv::Vec3f(H, S, V);
}

cv::Vec3b ColorPicker::HSV2BGR(cv::Vec3f hsv)
{
	float H = hsv[0];
	float S = hsv[1];
	float V = hsv[2];

	float C = V * S;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = V - C;
	float R_, G_, B_;
	if (0 <= H && H < 60)
	{
		R_ = C;
		G_ = X;
		B_ = 0;
	}
	else if (60 <= H && H < 120)
	{
		R_ = X;
		G_ = C;
		B_ = 0;
	}
	else if (120 <= H && H < 180)
	{
		R_ = 0;
		G_ = C;
		B_ = X;
	}
	else if (180 <= H && H < 240)
	{
		R_ = 0;
		G_ = X;
		B_ = C;
	}
	else if (240 <= H && H < 300)
	{
		R_ = X;
		G_ = 0;
		B_ = C;
	}
	else if (300 <= H && H <= 360)
	{
		R_ = C;
		G_ = 0;
		B_ = X;
	}

	int r = (R_ + m) * 255;
	int g = (G_ + m) * 255;
	int b = (B_ + m) * 255;

	return cv::Vec3b(b, g, r);
}
