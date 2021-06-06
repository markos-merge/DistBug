//
// Created by markos on 5/6/21.
//

#include "Map.h"
#include <opencv2/imgcodecs.hpp>

Map::Map( const char *filename )
{
	m_img = cv::imread( filename, cv::IMREAD_GRAYSCALE );
	for( int i = 0; i < m_img.rows; ++i ) {
		for( int j = 0; j < m_img.cols; ++j ) {
			if( !m_img.at< unsigned char >( i, j ) ) {
				m_obstacles.emplace_back( i, j );
			}
		}
	}
}

int Map::getColor( const cv::Point2i &ij ) const
{
	int ret = 0;

	if( m_img.rows > ij.x && m_img.cols > ij.y )
		ret = m_img.at<unsigned char>( ij.x, ij.y );


	return ret;
}
const std::vector<cv::Point2i>& Map::getObstaclesPoints() const
{
	return m_obstacles;
}
const cv::Mat& Map::getMat()
{
	return m_img;
}

