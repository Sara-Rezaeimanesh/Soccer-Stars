#ifndef FIELD_HPP
#define FIELD_HPP "FIELD_HPP"
#include <iostream>
#include "RSDL-master/src/rsdl.hpp"
class Field{

public:
	Field( Point src_ , Point dst_ ) ;
	void draw( Window *window ) ;
	Point return_ball_to_initial_pos( int bradius ) ;
	Point find_player_position( int team_no , Point given_position ) ;
	Point reflect( Point pposition , Point& speed_vec ) ;
	bool contains_player( Point pposition ) ;
	bool is_goal( Point pposition , int bradius ) ;
	int find_goaled_gate( Point pposition , int radius ) ;

private:
	Point src , dst ;

} ;

#endif