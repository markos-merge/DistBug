//
// Created by markos on 5/6/21.
//

#include <cstdio>
#include "Bug.h"

/*-------------------------------------------------------------*/
Bug *Bug::bugFactory(const cv::Point2i& init_pos, const cv::Point2i& target_pos, BugType bug_type)
{
	Bug *ret = nullptr;
	switch( bug_type ) {
	case BugType::DIST_BUG:
		ret = new DistBug( init_pos, target_pos );
		break;
	case BugType::DUMMY:
		printf( "Something went really wrong\n" );
		break;
	}

	return ret;
}

/*-------------------------------------------------------------*/
DistBug::DistBug(const cv::Point2i& init_pos, const cv::Point2i& target_pos)
{
	m_init_pos = init_pos;
	m_cur_pos = m_init_pos;
	m_target_pos = target_pos;
}

/*-------------------------------------------------------------*/
void DistBug::setPos(const cv::Point2i& init_pos)
{
	m_cur_pos = init_pos;
}

/*-------------------------------------------------------------*/
const cv::Point2i &DistBug::getCurPos()
{
	return m_cur_pos;
}

/*-------------------------------------------------------------*/
void DistBug::move()
{
	// i will do many things her
}