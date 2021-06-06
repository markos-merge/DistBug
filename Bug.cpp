//
// Created by markos on 5/6/21.
//

#include <cstdio>
#include <algorithm>
#include "Bug.h"
#include "Map.h"

#define STEP 3
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
std::string Bug::getStateString()
{
	std::string ret( "NONE" );
	switch( m_state ) {

	case State::INIT:
		ret = "INIT";
		break;
	case State::MOVE:
		ret = "MOVE";
		break;
	case State::FOLLOWING:
		ret = "FOLLOWING";
		break;
	case State::DUMMY:
		ret = "DUMMY";
		break;
	}

	return ret;
}
cv::Point2i Bug::getPose()
{
	return m_pose;
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
static bool checkForObstacleInDir( const cv::Point2i &cur_pos, const cv::Point2i &dir, const Map &map )
{
	return !map.getColor( cur_pos + dir );
}
/*-------------------------------------------------------------*/
cv::Point2i DistBug::iAmNearObstacle(const Map& map)
{
	cv::Point2i ans = cv::Point2i( 0, 0 );

	if(checkForObstacleInDir( m_cur_pos, cv::Point2i( 1,0 ), map ) ) {
		ans = cv::Point2i( 1, 0 );
	} else if(checkForObstacleInDir( m_cur_pos, cv::Point2i( 1,1 ), map ) ) {
		ans = cv::Point2i( 1, 1 );
	} else if(checkForObstacleInDir( m_cur_pos, cv::Point2i( 0,1 ), map ) ) {
		ans = cv::Point2i( 0, 1 );
	} else if(checkForObstacleInDir( m_cur_pos, cv::Point2i(-1,1 ), map ) ) {
		ans = cv::Point2i(-1, 1 );
	} else if(checkForObstacleInDir( m_cur_pos, cv::Point2i(-1,0 ), map ) ) {
		ans = cv::Point2i(-1, 0 );
	} else if(checkForObstacleInDir( m_cur_pos, cv::Point2i( -1,-1 ), map ) ) {
		ans = cv::Point2i(-1,-1 );
	} else if(checkForObstacleInDir( m_cur_pos, cv::Point2i( 0,-1 ), map ) ) {
		ans = cv::Point2i( 0, -1 );
	} else if(checkForObstacleInDir( m_cur_pos, cv::Point2i( 1,-1 ), map ) ) {
		ans = cv::Point2i( 1, -1 );
	}

	return ans;
}
/*-------------------------------------------------------------*/
void DistBug::moveFreely( const Map &map )
{
	double angle = M_1_PI * 180. * std::atan2( m_target_pos.y - m_cur_pos.y, m_target_pos.x - m_cur_pos.x );
	if( 0 <= angle && angle < 22.5 ) {
		m_pose = cv::Point2i( 1, 0 );
	} else if( 22.5 <= angle && angle < 3*22.5 ) {
		m_pose = cv::Point2i( 1, 1 );
	} else if( 3*22.5 <= angle && angle < 5*22.5 ) {
		m_pose = cv::Point2i( 0, 1 );
	} else if( 5*22.5 <= angle && angle < 7*22.5 ) {
		m_pose = cv::Point2i(-1,  1 );
	} else if( 7*22.5 <= angle && angle <= 180 ) {
		m_pose = cv::Point2i( -1, 0 );
	} else if( -180 <= angle && angle <= -180+22.5 ) {
		m_pose = cv::Point2i( -1, 0 );
	} else if( -180 + 22.5 <= angle && angle <= -180+ 3*22.5 ) {
		m_pose = cv::Point2i( -1,-1 );
	} else if( -180 + 3*22.5 <= angle && angle <= -180+ 5*22.5 ) {
		m_pose = cv::Point2i( 0,-1 );
	} else if( -180 + 5*22.5 <= angle && angle <= -180+ 7*22.5 ) {
		m_pose = cv::Point2i( 1,-1 );
	} else if( -180 + 7*22.5 <= angle && angle <= 0 ) {
		m_pose = cv::Point2i( 1, 0 );
	}

	m_cur_pos += m_pose;

	auto obs_dir = iAmNearObstacle(map);
	if( obs_dir != cv::Point2i( 0, 0 ) ){
		putObstacleToTheRight( obs_dir );
		m_state = State::FOLLOWING;
	}
}

/*-------------------------------------------------------------*/
void DistBug::putObstacleToTheRight(const cv::Point2i &obs_dir)
{
	if( obs_dir == cv::Point2i( 1, 0 ) ) {
		m_pose = cv::Point2i( 0, 1 );
	} else if( obs_dir == cv::Point2i( 1, 1 ) ) {
		m_pose = cv::Point2i( 0, 1 );
	} else if( obs_dir == cv::Point2i( 0, 1 ) ) {
		m_pose = cv::Point2i(-1, 0 );
	} else if( obs_dir == cv::Point2i(-1, 1 ) ) {
		m_pose = cv::Point2i(-1, 0 );
	} else if( obs_dir == cv::Point2i(-1, 0 ) ) {
		m_pose = cv::Point2i( 0,-1 );
	} else if( obs_dir == cv::Point2i(-1,-1 ) ) {
		m_pose = cv::Point2i( 0,-1 );
	} else if( obs_dir == cv::Point2i( 0,-1 ) ) {
		m_pose = cv::Point2i( 1, 0 );
	} else if( obs_dir == cv::Point2i( 1,-1 ) ) {
		m_pose = cv::Point2i( 1, 1 );
	}

}

static cv::Point2i getCWPerp( const cv::Point2i &pose )
{
	cv::Point2i temp;

	cv::Point3i temp_pose( pose.x, pose.y, 0 );
	temp_pose = temp_pose.cross( cv::Point3i( 0, 0, 1 ) );
	temp = cv::Point2i( temp_pose.x, temp_pose.y );

	return temp;
}
/*-------------------------------------------------------------*/
static cv::Point2i turnCW( cv::Point2i &pose )
{
	cv::Point2i temp;
	cv::Point3i temp_pose( pose.x, pose.y, 0 );
	temp_pose = temp_pose.cross( cv::Point3i( 0, 0, 1 ) );
	temp = cv::Point2i( temp_pose.x, temp_pose.y ) + pose;
	if( std::abs( temp.x ) == 2 ) {
		temp.x /= 2;
		temp.y /= 2;
	}

	return temp;
}
/*-------------------------------------------------------------*/
static cv::Point2i turnCCW( cv::Point2i &pose )
{
	cv::Point2i temp;
	cv::Point3i temp_pose( pose.x, pose.y, 0 );
	temp_pose = temp_pose.cross( cv::Point3i( 0, 0, -1 ) );
	temp = cv::Point2i( temp_pose.x, temp_pose.y ) + pose;
	if( std::abs( temp.x ) == 2 || std::abs( temp.y ) == 2 ) {
		temp.x /= 2;
		temp.y /= 2;
	}

	return temp;
}
/*-------------------------------------------------------------*/
void DistBug::goForward()
{
	m_cur_pos += m_pose;
}

void DistBug::turnToBlankSpaceIfNeeded( const Map &map )
{
	cv::Point2i cur_pose = turnCW( m_pose );
	while( !map.getColor( m_cur_pos + cur_pose ) ) {
		cur_pose = turnCCW( cur_pose );
	}
	m_pose = cur_pose;
}

/*-------------------------------------------------------------*/
bool isPointCloseToLine( const cv::Point2i &start_point, const cv::Point2i &end_point, const cv::Point2i &point )
{
	bool ans = false;
	cv::Point2f a = cv::Point2f( end_point.x, end_point.y ) - cv::Point2f( start_point.x, start_point.y );
	cv::Point2f b = cv::Point2f( point.x, point.y ) - cv::Point2f( start_point.x, start_point.y );
	float scalar_prj = a.dot( b ) /( a.x*a.x + a.y * a.y );
	cv::Point2f prj = scalar_prj * a;
	float dist = cv::norm(prj - b );
	if( dist < 0.5 && 0 < scalar_prj && scalar_prj < 1. ) {
		ans = true;
	}

	return ans;
}
/*-------------------------------------------------------------*/
static float getDistance( const cv::Point2i &start, const cv::Point2i &trg )
{
	cv::Point2f tmp = trg - start;
	return std::sqrt( tmp.x * tmp.x + tmp.y * tmp.y );
}
/*-------------------------------------------------------------*/
bool DistBug::shouldIContinueFollowingTheObstacle( const Map &mat )
{
	bool ans = true;
	float cur_dist = getDistance( m_cur_pos, m_target_pos );
	float min_dist = cur_dist;
	const auto &obstacles = mat.getObstaclesPoints();

	for( auto &point: obstacles ) {
		if( isPointCloseToLine( m_cur_pos, m_target_pos, point ) ) {
			cv::Point2f temp( point.x - m_cur_pos.x, point.y - m_cur_pos.y );
			min_dist = std::min( min_dist, std::sqrt( temp.x * temp.x + temp.y *temp.y ) );
		}
	}

	if( cur_dist - min_dist < m_min_dist - STEP ) {
		ans = false;
	}


	return ans;
}

/*-------------------------------------------------------------*/
void DistBug::followObstacle( const Map &map )
{
	float dist = getDistance( m_cur_pos, m_target_pos );
	m_min_dist = std::min( m_min_dist, dist );

	if( shouldIContinueFollowingTheObstacle(map) ) {
		turnToBlankSpaceIfNeeded( map );
		goForward();
	} else {
		m_state = State::MOVE;
		moveFreely( map );
	}
}

/*-------------------------------------------------------------*/
void DistBug::move( const Map &map )
{
	if( m_state == State::MOVE ) {
		moveFreely( map );
	} else if( m_state == State::FOLLOWING ) {
		followObstacle( map );
	}
}
