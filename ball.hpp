#ifndef BALL_HPP
#define BALL_HPP "BALL_HPP"
#include "scoreboard.hpp"
#include "RSDL-master/src/rsdl.hpp"
#include "playermove.hpp"

class Field ; 

class Ball{

public:
	Ball( Point corner_position_ , int radius_ , int bmass_ );
	bool is_hit( PlayerMove* player_move ) ;
	void draw( Window *window , Point delta_pos ) ;
	void save_new_speed( Point new_hit_speed ) ;
	void run_action( PlayerMove* player_move , ScoreBoard* score_board ) ;
	void save_field( Field* field_ ) ;
	Point get_position() { return bposition ; }
	int get_mass() { return bmass ; }

private:
	void move( ScoreBoard* score_board , PlayerMove* player_move ) ;
	Field* field ; 
	Point bposition ;
	Point speed ; 
	int bmass ; 
	int radius ;
	double speed_magn ;
} ;

#endif