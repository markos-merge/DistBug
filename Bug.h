//
// Created by markos on 5/6/21.
//

#ifndef DISTBUG_BUG_H
#define DISTBUG_BUG_H

#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/core/mat.hpp>

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
	virtual void move() = 0;
	virtual void setPos( const cv::Point2i &init_pos ) = 0;
	virtual const cv::Point2i &getCurPos() = 0;

protected:
	State m_state = State::INIT;
	cv::Point2i m_init_pos, m_cur_pos, m_target_pos;
};

class DistBug: public Bug {
public:
	DistBug( const cv::Point2i &init_pos, const cv::Point2i &target_pos );
	void move() override;
	void setPos( const cv::Point2i &init_pos ) override;
	const cv::Point2i &getCurPos() override;
};


#endif //DISTBUG_BUG_H
