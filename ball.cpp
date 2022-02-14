#include "RSDL-master/src/rsdl.hpp"
#include "ball.hpp"
#include <cmath>
#include <iostream>
#include "playermove.hpp"
#include "field.hpp"
#include "scoreboard.hpp"
#define POWER 2 
#define FRICTION_ACCEL -2
#define FINISHED 0 
#define PENDING 1 
#define BALL_ADD "ball.png"

Ball::Ball( Point corner_position_ , int radius_ , int bmass_ )
{
	bposition.x = corner_position_.x + radius_ ; 
	bposition.y = corner_position_.y + radius_ ; 
	radius = radius_ ;
	bmass = bmass_ ;
}

void Ball::draw( Window *window , Point delta_pos ) 
{
	//MEND
	window->draw_img( BALL_ADD , Rectangle( bposition.x + delta_pos.x - radius , 
		                                                bposition.y + delta_pos.y - radius , radius*2 , radius*2 ) ) ;
}

bool Ball::is_hit( PlayerMove* player_move ) 
{
	if( player_move->hits_ball( bposition ) )
		return true ;
	return false ; 
}

void Ball::save_new_speed( Point new_hit_speed ) 
{
	speed = new_hit_speed ;
	speed_magn = sqrt( pow( speed.x , POWER ) + pow( speed.y , POWER ) ) ;
}

void Ball::run_action( PlayerMove* player_move , ScoreBoard* score_board ) 
{
	if( speed_magn <= -(FRICTION_ACCEL) )
	{
		player_move->update_hit_status( FINISHED ) ;
		return ;
	}
	move( score_board , player_move ) ;
	speed_magn += FRICTION_ACCEL ;
	double sin = abs( (double)speed.y / speed_magn ) ;
	double cos = abs( (double)speed.x / speed_magn ) ;
	speed.x = cos * speed_magn * ( speed.x / abs( speed.x ) ) ;
	speed.y = sin * speed_magn * ( speed.y / abs( speed.y ) ) ;
}

void Ball::move( ScoreBoard* score_board , PlayerMove* player_move )
{
	bposition.x += speed.x ;
	bposition.y += speed.y ;
	if( !field->contains_player( bposition ) )
		if( !field->is_goal( bposition , radius  ) )
			bposition = field->reflect( bposition , speed ) ;
	if( field->is_goal( bposition , radius ) ){
		int goaled_gate = field->find_goaled_gate( bposition , radius ) ;
		score_board->update_score( goaled_gate ) ;
		bposition = field->return_ball_to_initial_pos( radius ) ;
		player_move->update_hit_status( FINISHED ) ;
	}
}

void Ball::save_field( Field* field_ ) 
{
	field = field_ ;
}