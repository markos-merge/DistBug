//
// Created by markos on 5/6/21.
//

#ifndef DISTBUG_MAP_H
#define DISTBUG_MAP_H

#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>

class Map {
public:
	explicit Map(const char *filename );
	[[nodiscard]] int getColor( const cv::Point2i &ij ) const;
	[[nodiscard]] const std::vector< cv::Point2i > &getObstaclesPoints() const ;
	const cv::Mat &getMat();
private:
	cv::Mat m_img;
	std::vector< cv::Point2i > m_obstacles;
};

#endif //DISTBUG_MAP_H
