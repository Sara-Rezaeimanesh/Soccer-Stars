#include "RSDL-master/src/rsdl.hpp"
#include "player.hpp"
#include "playermove.hpp"
#include "team.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>
#define MAX_INITITAL_SPEED_PF 25
#define MAX_THROW_RADIUS 80
#define END1 1 
#define END2 2 
#define FRICTION_ACCEL -2
#define POWER 2
#define NOSPEED 0 
#define BALL 1
#define PLAYER 2 
#define NOSPEED 0
#define NONE -1 

PlayerMove::PlayerMove()
{
	num_of_hits = 0 ;
	status = finished ; 
	contacted_type = 0 ; 
	speed_magn = 0 ; 
	shoot_radius = 0 ; 
	handle_stat = 0 ; 
	shootline_end1 = Point( NOSPEED , NOSPEED ) ;
	shootline_end2 = Point( NOSPEED , NOSPEED ) ;
}

void PlayerMove::update_status( int event_stat )
{
	status = event_stat ;
}

void PlayerMove::save_shoot_magnitude()
{
	int radius = player->calculate_distance( curr_mpos ) ;
	shoot_radius = ( radius < MAX_THROW_RADIUS ) ? radius : MAX_THROW_RADIUS ;
}

void PlayerMove::save_shootline_ends()
{
	shootline_end1 = player->calculate_shootline_end( curr_mpos , END1 , shoot_radius ) ;
	shootline_end2 = player->calculate_shootline_end( curr_mpos , END2 , shoot_radius ) ;
}

void PlayerMove::draw_shoot_magnitude( Window* window )
{
	player->draw_shoot_circle( shoot_radius , window ) ;
	window->draw_line( shootline_end2 , shootline_end1 , WHITE ) ;
}

void PlayerMove::add_active_player( Player* player_ )
{
	player = player_ ;
	update_status( pending ) ;
}

void PlayerMove::save_mouse_position( Point curr_mpos_ ) 
{
	curr_mpos = curr_mpos_ ; 
}

void PlayerMove::save_speed_properties( Point mouse_pos ) 
{
	save_mouse_position( mouse_pos ) ;
	save_shoot_magnitude() ;
	save_shootline_ends() ;
	save_speed() ;
}

bool PlayerMove::is_pending() 
{
	if( status == pending )
		return true ; 
	return false ;
}

void PlayerMove::save_speed() 
{
	if( !shoot_radius )
	{
		speed = Point( NOSPEED , NOSPEED ) ;
		speed_magn = NOSPEED ; 
	}
	int actual_dist = sqrt( pow( player->calculate_x_distance( curr_mpos.x ) , POWER ) 
						+ pow( player->calculate_y_distance( curr_mpos.y ) , POWER ) ) ;
	dist_magn = ( shoot_radius < MAX_THROW_RADIUS ) ? MAX_THROW_RADIUS :  actual_dist ;
	speed.x = (-1)*(double)( player->calculate_x_distance( curr_mpos.x ) ) 
				* (double)MAX_INITITAL_SPEED_PF / dist_magn ;
	speed.y = (-1)*(double)( player->calculate_y_distance( curr_mpos.y ) ) 
				* (double)MAX_INITITAL_SPEED_PF / dist_magn ;
	speed_magn = sqrt( pow( speed.x , POWER ) + pow( speed.y , POWER ) ) ;
	dist_magn = ( shoot_radius >= MAX_THROW_RADIUS ) ? MAX_THROW_RADIUS :  actual_dist ;
}

bool PlayerMove::exception_exits( Point speed_ )
{
	if( speed_.x == NOSPEED || speed_.y == NOSPEED )
		return true ;
	return false ; 
}

void PlayerMove::handle_exceptions( Point& speed_ , double speed_magn_ , double sin , double cos )
{
	if( speed_.x == NOSPEED && speed_.y != NOSPEED )
	{
		speed_.x = NOSPEED ;
		speed_.y = sin * speed_magn_ * ( speed_.y / abs( speed_.y ) ) ;
	}
	if( speed_.y == NOSPEED && speed_.x != NOSPEED )
	{
		speed_.y = NOSPEED ;
		speed_.x = cos * speed_magn_ * ( speed_.x / abs( speed_.x ) ) ;
	}
	if( speed_.x == NOSPEED && speed_.y == NOSPEED )
	{
		speed_.x = NOSPEED ;
		speed_.y = NOSPEED ;
	}
}

void PlayerMove::run_move() 
{
	if( speed_magn <= -(FRICTION_ACCEL) )
	{
		if( all_hits_are_finished() )
			status = finished ;
		return ; 
	}
	player->move( speed ) ;
	speed_magn += FRICTION_ACCEL ;
	double sin = abs( speed.y / speed_magn ) ;
	double cos = abs( speed.x / speed_magn ) ; 
	if( exception_exits( speed ) )
	{
		handle_exceptions( speed , speed_magn , sin , cos ) ;
		return ; 
	}
	speed.x = cos * speed_magn * ( speed.x / abs( speed.x ) ) ;
	speed.y = sin * speed_magn * ( speed.y / abs( speed.y ) ) ;
} 

void PlayerMove::save_hit_speed( Point speed_ , Player* hit_player_ ) 
{
	speed = speed_ ;
	speed_magn = sqrt( pow( speed.x , POWER ) + pow( speed.y , POWER ) ) ;
	player = hit_player_ ;
}

void PlayerMove::save_hit_player( Player* hit_player_ , int encountered_idx , int type ) 
{
	Point new_hit_speed = handle_contact( hit_player_->get_position() , type , hit_player_->get_mass() ) ;
	PlayerMove* new_player_move = new PlayerMove() ;
	hit_idxs.push_back( encountered_idx ) ;
	contacted_type = PLAYER ;
	hit_players.push_back( hit_player_ ) ;
	num_of_hits++ ;
	save_hit_player_speed( new_hit_speed ) ;
	new_player_move->save_hit_speed( new_hit_speed , hit_player_ ) ;
	hit_moves.push_back( new_player_move ) ;
}

Point PlayerMove::calculate_speed( Point speed1 , Point speed2 , int mass1 , int mass2 , int dist , Point pos1 , Point pos2 )
{
	Point new_speed ;
	new_speed.x = speed1.x - (double)( 2 * mass2 * ( ( speed1.x - speed2.x ) * ( pos1.x - pos2.x ) 
						+ ( speed1.y - speed2.y ) * ( pos1.y - pos2.y ) ) * ( pos1.x - pos2.x ) ) / 
						( ( mass2 + mass1 ) * pow( dist , POWER ) ) ;
	new_speed.y = speed1.y - (double)( 2 * mass2 * ( ( speed1.x - speed2.x ) * ( pos1.x - pos2.x ) 
						+ ( speed1.y - speed2.y ) * ( pos1.y - pos2.y ) ) * ( pos1.y - pos2.y ) ) / 
						( ( mass2 + mass1 ) * pow( dist , POWER ) ) ;
	return new_speed ;
}

Point PlayerMove::handle_contact( Point hit_pos , int type , int hit_mass ) 
{
	handle_stat++ ;
	hit_status.push_back( pending ) ; 
	contacted_type = type ; 
	int player_mass = player->get_mass() ;
	Point pposition = player->get_previous_position() ;
	double dist = sqrt( pow( pposition.x - hit_pos.x , POWER ) + pow( pposition.y - hit_pos.y , POWER ) ) ;
	Point new_speed = calculate_speed( speed , Point(NOSPEED,NOSPEED) , player_mass , hit_mass , dist , pposition , hit_pos ) ;
	Point new_hit_speed = calculate_speed( Point(NOSPEED,NOSPEED) , speed , hit_mass , player_mass , dist , hit_pos , pposition ) ;
	speed = new_speed ; 
	hit_speed_magns.push_back( sqrt( pow( new_hit_speed.x , POWER ) + pow( new_hit_speed.y , POWER ) ) ) ;
	return new_hit_speed ;
}

void PlayerMove::save_hit_player_speed( Point new_hit_speed ) 
{
	hit_player_speeds.push_back( new_hit_speed ) ;
}

void PlayerMove::update_hit_status( int status ) 
{
	hit_speed_magns.push_back( NOSPEED ) ;
	hit_status.push_back( status ) ; 
}

void PlayerMove::run_hit_move( int i )
{
	if( hit_speed_magns[i] <= -(FRICTION_ACCEL) )
	{
		hit_idxs[i] = NONE ;
		hit_status[i] = finished ;
		if( speed_magn <= -(FRICTION_ACCEL) && all_hits_are_finished() )
			status = finished ; 
		return ; 
	}
	hit_players[i]->move( hit_player_speeds[i] ) ;
	hit_speed_magns[i] += FRICTION_ACCEL ;
	double sin = abs( (double)hit_player_speeds[i].y / (double)hit_speed_magns[i] ) ;
	double cos = abs( (double)hit_player_speeds[i].x / (double)hit_speed_magns[i] ) ;
	if( exception_exits( hit_player_speeds[i] ) )
	{
		handle_exceptions( hit_player_speeds[i] , hit_speed_magns[i] , sin , cos ) ;
		return ;
	}
	hit_player_speeds[i].x = cos * hit_speed_magns[i] * ( hit_player_speeds[i].x / abs( hit_player_speeds[i].x ) ) ;
	hit_player_speeds[i].y = sin * hit_speed_magns[i] * ( hit_player_speeds[i].y / abs( hit_player_speeds[i].y ) ) ;
}

void PlayerMove::run_player_action() 
{
	for( int i = 0 ; i < num_of_hits ; i++ )
		run_hit_move( i ) ;
}

bool PlayerMove::is_in_action() 
{
	if( status == in_action )
		return true ; 
	return false ;
}

bool PlayerMove::is_initiated()
{
	if( status != not_initiated )
		return false ; 
	return true ;
}

int PlayerMove::find_encountered_idx( Team& team ) 
{
	try
	{
		Point pos = player->get_position() ;
		int encountered_idx = team.find_encountered_idx( player ) ;
		return encountered_idx ;
	}
	catch(...)
	{
		throw std::runtime_error( "no encounters" ) ;
	}
}

bool PlayerMove::hits_ball( Point bposition ) 
{
	if( player->encounters( bposition ) )
	{
		hit_status.push_back( pending ) ;
		return true ; 
	}
	return false ; 
}

bool PlayerMove::has_hit_action() 
{
	if( all_hits_are_finished() )
		return true ; 
	return false ;
}

bool PlayerMove::all_hits_are_finished()
{
	if( num_of_hits == 0 )
		return true ; 
	for( int i = 0 ; i < num_of_hits ; i++ )
			if( hit_speed_magns[i] <= -(FRICTION_ACCEL) )
				return true ;
	return false ; 
}

bool PlayerMove::is_finished()
{
	if( status == finished )
		return true ; 
	return false ; 
}

bool PlayerMove::hit_already_added( int encountered_idx ) 
{
	for( int i = 0 ; i < hit_idxs.size() ; i++ )
		if( hit_idxs[i] == encountered_idx )
			return true ;
	return false ; 
}