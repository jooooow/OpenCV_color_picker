#pragma once
#include <opencv2/opencv.hpp>

class ColorPicker
{
public:
	ColorPicker(int window_size, int tube_width, std::string name, cv::Vec3f initial_hsv = cv::Vec3f(0, 1, 1));
	void OnMouse(int event, int x, int y, int flags);
	std::string GetName()
	{
		return name;
	}
	void Show()
	{
		cv::imshow(name, panel);
	}
	cv::Vec3b GetBgr()
	{
		return bgr;
	}

private:
	std::string name;
	int window_size;
	int tube_width;
	cv::Mat panel;
	cv::Vec3f hsv;
	cv::Vec3b bgr;

	void GeneratePanel();
	int isInWheel(int i, int j);
	int isInSquare(int x, int y);
	cv::Vec3f GetHSV(int i, int j);
	cv::Vec3b HSV2BGR(cv::Vec3f hsv);
	void UpdatePanel();
	void UpdateSample();
};