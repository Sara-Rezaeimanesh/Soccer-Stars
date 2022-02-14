#ifndef PLAYER_MOVE_HPP
#define PLAYER_MOVE_HPP "PLAYER_MOVE_HPP"
#include "RSDL-master/src/rsdl.hpp"
#include <vector>
#include "team.hpp"

class Player ;

enum Status{
	pending = 1 ,
	finished = 0 ,
	in_action = 2 ,
	not_initiated = 3 ,
	initiated = 4 
};

class PlayerMove{
public:
	PlayerMove() ;
	void update_status( int event_stat ) ;
	void save_speed_properties( Point mouse_pos ) ;
	void save_hit_speed( Point speed , Player* hit_player_ ) ;
	void add_active_player( Player* player_ ) ;
	void draw_shoot_magnitude( Window* window ) ;
	void run_move() ;
	void save_hit_player( Player* hit_player_ , int encountered_idx , int type ) ;
	void update_hit_status( int status ) ;
	void run_player_action() ;
	int find_encountered_idx( Team& team ) ;
	int get_contacted_type() { return contacted_type ; }
	bool hits_ball( Point bposition ) ;
	bool is_in_action() ;
	bool is_initiated() ;
	bool is_pending() ;
	bool has_hit_action() ;
	bool is_finished() ;
	bool hit_already_added( int encontered_idx ) ;
	Point handle_contact( Point hit_pos , int type , int hit_mass ) ;
	std:: vector <PlayerMove*> get_hit_moves() { return hit_moves ; }

private:
	void save_hit_player_speed( Point new_hit_speed ) ;
	void run_hit_move( int i ) ;
	void handle_exceptions( Point& speed_ , double speed_magn , double sin , double cos ) ;
	void save_shoot_magnitude() ;
	void save_shootline_ends() ;
	void save_mouse_position( Point curr_mpos_ ) ;
	void save_speed() ;
	Point calculate_speed( Point speed1 , Point speed2 , int mass1 , int mass2 , int dist , Point pos1 , Point pos2 ) ;
	bool  all_hits_are_finished() ;
	bool exception_exits( Point speed_ ) ;
	Player* player ;
	std::vector <Player*> hit_players ; 
	Point shootline_end1 , shootline_end2 , curr_mpos , speed , hit_player_speed ;
	std::vector <Point> hit_player_speeds ;
	int num_of_hits ;
	int status ; 
	int shoot_radius ; 
	int contacted_type ;
	int handle_stat ; 
	double speed_magn , dist_magn , hit_dist_magn ;
	std::vector <double> hit_speed_magns ; 
	std::vector <int> hit_status ;
	std::vector <int> hit_idxs ;
	std::vector <PlayerMove*> hit_moves ;
} ;

#endif