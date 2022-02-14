#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP "SCOREBOARD_HPP"
#include "RSDL-master/src/rsdl.hpp"
#include <vector>


class ScoreBoard{

public:
	ScoreBoard() ;
	void add_info( int rounds_ , int goals_ ) ;
	void change_turns( int turn ) ;
	void update_score( int goaled_gate ) ;
	void draw( Window* window ) ;
	void show_results( Window* Window ) ;
	bool game_is_finished() ;

private:
	int turn ; 
	int rounds ; 
	int goals ;
	int game_status ; 
	std::vector <int> scores ; 
	std::vector <int> gameswon ; 
} ;

#endif