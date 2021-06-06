//
// Created by markos on 5/6/21.
//

#ifndef DISTBUG_BUG_H
#define DISTBUG_BUG_H

#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

class Map;

class Bug {
public:
	enum class BugType{
		DIST_BUG = 0x00,
		DUMMY = 0xFF
	};
	enum class State {
		INIT = 0x00,
		MOVE = 0x01,
		FOLLOWING = 0x02,

		DUMMY = 0xFF
	};

	static Bug *bugFactory( const cv::Point2i &init_pos, const cv::Point2i &target_pos, BugType bug_type );
	virtual void move( const Map &map ) = 0;
	virtual void setPos( const cv::Point2i &init_pos ) = 0;
	virtual const cv::Point2i &getCurPos() = 0;

	std::string getStateString();
	cv::Point2i getPose();

protected:
	State m_state = State::MOVE;
	cv::Point2i m_init_pos, m_cur_pos, m_pose, m_target_pos;
};

class DistBug: public Bug {
public:
	DistBug( const cv::Point2i &init_pos, const cv::Point2i &target_pos );
	void move(const Map &map ) override;
	void setPos( const cv::Point2i &init_pos ) override;
	const cv::Point2i &getCurPos() override;

private:
	float m_min_dist = std::numeric_limits<float>::max();
private:
	void moveFreely(const Map& map);
	cv::Point2i iAmNearObstacle(const Map& map);
	void putObstacleToTheRight(const cv::Point2i &point);
	void followObstacle( const Map &map );
	void goForward();
	void turnToBlankSpaceIfNeeded(const Map& map);
	bool shouldIContinueFollowingTheObstacle(const Map& mat);
};


#endif //DISTBUG_BUG_H
