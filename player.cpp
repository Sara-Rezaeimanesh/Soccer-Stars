#include "RSDL-master/src/rsdl.hpp"
#include "player.hpp"
#include <string>
#include "field.hpp"
#include <iostream>
#include <cmath>
#include "field.hpp"
#define POWER 2
#define END1 1 
#define END2 2 

Player::Player( Point given_position_ , int radius_ , Field *field_ , int team_no , int pmass_ )
{
	radius = radius_ ;
	field = field_ ;
	pposition = field->find_player_position( team_no , given_position_ ) ;
	pmass = pmass_ ;
}

int Player::calculate_distance( Point curr_mpos )
{
	return sqrt( pow( pposition.x - curr_mpos.x , POWER ) + pow( pposition.y - curr_mpos.y , POWER ) ) ;
}

void Player::draw( Window *window , Point delta_pos , std::string player_img_add ) 
{
	window->draw_img( player_img_add.c_str() , 
					  Rectangle( pposition.x + delta_pos.x - radius , pposition.y + delta_pos.y - radius ,
					  radius*2 , radius*2 ) ) ;
}

bool Player::is_selected( Point curr_pos ) 
{
	if( ( pposition.x - radius <= curr_pos.x && curr_pos.x <= pposition.x + radius ) 
		&& ( pposition.y - radius <= curr_pos.y &&  curr_pos.y <= pposition.y + radius ) )
	{
		previous_pos = pposition ;
		return true ;
	}
	return false ; 
}

void Player::draw_shoot_circle( int shoot_radius , Window* window ) 
{
	window->fill_circle( pposition  , shoot_radius , BLUE ) ;
}

Point Player::calculate_shootline_end( Point curr_mpos , int end_no , int radius ) 
{
	Point line_end ; 
	int dist_from_player = calculate_distance( curr_mpos ) ;
	if( dist_from_player == 0 )
			return pposition ; 
	if( end_no == END1 )
	{
		line_end.x = ( pposition.x - curr_mpos.x ) * ( dist_from_player - radius ) / ( dist_from_player ) + curr_mpos.x ;
		line_end.y = ( pposition.y - curr_mpos.y ) * ( dist_from_player - radius ) / ( dist_from_player ) + curr_mpos.y ;
	}
	if( end_no == END2 ) 
	{
		line_end.x = ( pposition.x - curr_mpos.x ) * ( dist_from_player + radius ) / ( dist_from_player )  + curr_mpos.x ;
		line_end.y = ( pposition.y - curr_mpos.y ) * ( dist_from_player + radius ) / ( dist_from_player )  + curr_mpos.y ;
	}
	return line_end ; 
}

int Player::calculate_x_distance( int x ) 
{
	return x - pposition.x ;
}

int Player::calculate_y_distance( int y ) 
{
	return y - pposition.y ;
}

void Player::move( Point& speed ) 
{
	pposition.x += speed.x ;
	pposition.y += speed.y ;
	if( !field->contains_player( pposition ) )
		pposition = field->reflect( pposition , speed ) ;
}

bool Player::encounters( Point position ) 
{
	if( position.x != pposition.x ||  position.y != pposition.y )
		if( sqrt( pow( pposition.x - position.x , POWER ) + pow( pposition.y - position.y , POWER ) ) <= radius )
			return true ;
	return false ; 
}

Point Player::get_position()
{
	return pposition ; 
}