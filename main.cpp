#include <iostream>
#include <opencv2/highgui.hpp>
#include "Bug.h"
#include "Map.h"

int main()
{
	const char *filename = "./assets/obstacles3.jpg";
	Map map( filename );
//	cv::Mat mat = map.getMat().clone();
	cv::Mat mat = cv::imread( filename );
	DistBug bug( cv::Point2i( 2, 2 ), cv::Point2i( 456, 197 ) );

	auto cur_point = bug.getCurPos();
	std::cout << cur_point << " " << bug.getStateString() << std::endl;
	do {
		bug.move(map);
		auto cur_point = bug.getCurPos();
		std::cout << cur_point << " " << bug.getPose() << " " << bug.getStateString() << std::endl;
		mat.at<cv::Vec3b >( cur_point.x, cur_point.y ) = cv::Vec3b( 0, 0, 255 );
		cv::imshow( "Window", mat );
		if( cv::waitKey(0 ) == 'q' ){
			break;
		}

	} while(1);
	return 0;
}
