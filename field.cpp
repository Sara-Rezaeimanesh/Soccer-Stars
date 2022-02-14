#include "RSDL-master/src/rsdl.hpp"
#include <string>
#include "field.hpp"
#define FIRST 1
#define SECOND 2 
#define GATESTRT 160
#define GATEEND 320
#define HALF 2
#define NOAXIS 0 
#define TEAMTWO 1
#define TEAMONE 0
#define WARNING_RESOLVER -1
#define FIELD_ADD "field.jpg"

Field::Field( Point src_ , Point dst_ )
{
	src = src_ ;
	dst = dst_ ;
}

Point Field::find_player_position( int team_no , Point given_pos )
{
	if( team_no == FIRST )
		return given_pos ;
	if( team_no == SECOND )
		return Point( dst.x - given_pos.x , given_pos.y ) ;
	return Point( NOAXIS , NOAXIS ) ;
}

Point Field::return_ball_to_initial_pos( int bradius ) 
{
	return Point( src.x + ( dst.x / HALF ) , src.y + ( dst.y / HALF ) ) ;
}

void Field::draw( Window *window ) 
{
	//MEND
	window->draw_img( FIELD_ADD , Rectangle( src.x , src.y , dst.x , dst.y ) ) ;
}

bool Field::contains_player( Point pposition ) 
{
	  return pposition.x >= src.x && pposition.x < dst.x && pposition.y >= src.y && pposition.y < dst.y ;
}

Point Field::reflect( Point pposition , Point& speed_vec ) 
{
    if( !contains_player( pposition ) ) 
    {
        if ( pposition.x < src.x ) {
            pposition.x = -pposition.x;
            speed_vec.x = -speed_vec.x;
        }
        if ( pposition.x >= dst.x ) {
            pposition.x = 2 * dst.x - pposition.x;
            speed_vec.x = -speed_vec.x;
        }
        if ( pposition.y < src.y ) {
            pposition.y = -pposition.y;
            speed_vec.y = -speed_vec.y;
        }
        if ( pposition.y >= dst.y ) {
            pposition.y = 2 * dst.y - pposition.y;
            speed_vec.y = -speed_vec.y;
        }
    }
    return pposition ;
}

bool Field::is_goal( Point pposition , int bradius )
{
	if( pposition.y > GATESTRT && pposition.y < GATEEND && 
		( pposition.x <= src.x || pposition.x >= dst.x ) ) 
		return true ; 
	return false ; 
}

int Field::find_goaled_gate( Point pposition , int bradius ) 
{
	if( pposition.x >= dst.x )
		return TEAMTWO ; 
	if( pposition.x <= src.x )
		return TEAMONE ; 
	return WARNING_RESOLVER ;
}
