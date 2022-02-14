#ifndef PALYER_HPP
#define PALYER_HPP "PALYER_HPP"
#include <string>
#include "RSDL-master/src/rsdl.hpp"
#include "field.hpp"

class Player{

public:
	Player( Point given_position_ , int radius_ , Field *field_ , int team_no , int pmass_ );
	void draw( Window *window , Point curr_pos , std::string img_add ) ;
	void draw_shoot_circle( int shoot_radius , Window* window ) ;
	void save_shootline_ends( Point curr_mpos ) ;
	void move( Point& speed ) ;
	Point calculate_shootline_end( Point curr_mpos , int end_no , int radius ) ;
	Point get_position() ;
	Point get_previous_position() { return previous_pos ; }
	int calculate_x_distance( int x ) ;
	int calculate_y_distance( int y ) ;
	int get_mass() { return pmass ; }
	int calculate_distance( Point curr_mpos ) ;
	bool encounters( Point position ) ;
	bool is_selected( Point curr_pos ) ;

private:
	Point pposition , previous_pos ;
	Field* field ;
	int radius ;
	int pmass ; 
} ;

#endif