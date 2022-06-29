#include "color_picker.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
	ColorPicker color_picker(500, 50, "color_picker");
	while (1)
	{
		color_picker.Show();
		std::cout << color_picker.GetBgr() << std::endl;
		if (cv::waitKey(30) == 'q')
			break;
	}
	return 0;
}