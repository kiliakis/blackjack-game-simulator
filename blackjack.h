#ifndef BLACKJACK_H_
#define BLACKJACK_H_

#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <getopt.h>

/**********/
// num decks uper limit = 16
// num boxes uper limit = 10
/**********/


// number of decks in every shoe
const int DECKS_NUM = 8;
// player's betting unit
const int WAGER = 100;
// number of boxes in every round
const int BOXES	= 1;
// rolling commission rate %
const int COM_RATE = 1;
// number of rounds in a trip
// only applicable when random shuffle is used
const int ROUNDS = 1000;
// number of shoes in a trip
// only applicable when manual shoe is used
const int SHOES = 2;
// number of trips in a simulation
const int TRIPS = 10;
// Detailed output or not?
const int DETAILED_OUTPUT = 1;
// generate graphs or not?
const int GENERATE_GRAPHS = 1;
// Shuffle method (0-> random shuffle, 1-> manual shoe)
const int SHUFFLE = 1;
// Split to 3 or 4 hands
const int NUM_SPLITS = 4;
// Can resplit Aces? (0-> no, 1-> yes)
const int RESPLITA = 1;
// House soft 17 hit or stand? (0-> hit, 1-> stand)
const int SOFT17 = 0;
// Player's 21 push or lose to house BJ? (0-> lose, 1-> push)
const int PUSH21 = 0;
// Player's 777 win double instantly or wait for house card? (0-> wait, 1-> win)
const int WIN777 = 0;
// Can the player surrender? (0-> no, 1-> yes)
const int SURRENDER = 1;
// Double down on 9,10,11 or any 2 cards? (0-> 9,10,11, 1-> any 2 cards)
const int DOUBLE_ANY = 1;
// House BJ win all bets or original only? (0-> original, 1-> all)
const int BJWINALL = 1;


// uncomment next line for fixed player's first two cards
#define P_FIXED_CARDS
// uncomment next line for fixed house first card
#define H_FIXED_CARDS
// uncomment next line for a random slit game
// #define RANDOM_SPLIT
// note that random split does not work with if
// P_FIXED_CARDS is defined


void loadbar(unsigned int x, unsigned int n, unsigned int w = 50);

using namespace std;

class BlackJack {
public:
	int shuffle = SHUFFLE;
	int num_splits = NUM_SPLITS;
	int resplitA = RESPLITA;
	int soft17 = SOFT17;
	int push21 = PUSH21;
	int win777 = WIN777;
	int double_any = DOUBLE_ANY;
	int surrender = SURRENDER;
	int bjwinall = BJWINALL;
	int num_rounds = ROUNDS;
	int num_decks = DECKS_NUM;
	int num_boxes = BOXES;
	int num_trips = TRIPS;
	int num_shoes = SHOES;
	int wager = WAGER;
	double com_rate = COM_RATE;
	int detailed_out = DETAILED_OUTPUT;
	int generate_graphs = GENERATE_GRAPHS;

	int used_cards, total_cards,
	    triple7, bjs, num_boxes, house_BJ, house_busts, busts, surr,
	    hand, houseBJ_P21_pushes;
	bool BJ_on, split_on;
	long int wins = 0, loses = 0, ties = 0, hands = 0,
	         dwins = 0, dloses = 0, dties = 0, dhands = 0,
	         swins = 0, sloses = 0, sties = 0, shands = 0;

	long int trip_wins, trip_loses, trip_ties;
	int shoes_won, shoes_lost, shoes_tied;
	long double trip_score, trip_high_win, trip_high_lose;
	long int total_shoes_won = 0, total_shoes_lost = 0, total_shoes_tied = 0;
	long double highest_win = 0, highest_lose = 0;

	ofstream html;
	ofstream xml;
	ofstream shoe_html;
	ofstream trip_html;
	ofstream overall_stats;
	ofstream WL_streak;
	ofstream WLdata;

	vector<int> shoe;

// 	vector<bool> v;

// 	int P_1st_card, P_2nd_card, H_1st_card, P_next_card,
// 	    card01, card02, next_card, H, player_value_hard, player_value[10],
// 	    player_value_soft, house_value, shoe[16 * 52];
// 	H = 0;
// // rows are 1st - 2nd card, columns are different boxes
// 	int P_cards[2][10], P_value[10][4];

// 	bool player_bust[10], BJ[10];


// 	long int box,
// 	     player_busts = 0, triple7_wins = 0,
// 	     shoes, trips, total_i = 0;

// 	double bet[10];
// 	long double score = 0.0, dscore = 0.0, sscore = 0.0;

// 	string 	char_P_1st_card, char_P_2nd_card, char_P_next_card,
// 	        char_H_1st_card,
// 	        char_card01, char_card02, char_next_card, player_status[10],
// 	        player_split_status, house_status,
// 	        player_string[10], player_split_string,
// 	        house_string,
// 	        P_cards_char[2][10], P_status[10][4], P_string[10][4];

// 	char card_array[13];



	BlackJack();
	~BlackJack() {};
	void end_of_trip(long shoes_lost, long shoes_won, long shoes_tied,
	                 long trip_loses, long trip_wins, long trip_ties,
	                 double trip_score, double trip_high_win,
	                 double trip_high_lose);
	void start_of_trip();
	int calc_interval();
	void burn_card();
// Functions declaration
	void html_files_init();
	void generate_WLstreak();
	void html_files_close(long double hw, long double hl,
	                      long int tsw, long int tst,
	                      long int tsl);

	void stand();
	void hit();
	void Double();
	void bj();
	void split();
	void surrender_f();
	void draw_card(int &a, string &b);
	int resolve_winner(int, int, double, long int,
	                   long double &, long int &,
	                   long int &, long int &,
	                   long int &, string &,
	                   string &, bool);
	void html_print(long int, long int, ofstream &);
	char stringconversion(int);
	int numericconversion(int);
	string check_player_1st_2_cards(int & A, int & B,
	                                int & C, int & D);
	int player_hit(int & P_1st_card, int & P_2nd_card,
	               int & H_1st_card, int & player_value,
	               int & P_next_card, string & player_string);
	int house_hit(int & house_value, string & house_status,
	              string & house_string);
	int player_split(int & card01, int & card02,
	                 int & next_card, string char_card01,
	                 string char_card02,
	                 string & char_next_card,
	                 int & player_value,
	                 string & player_string,
	                 string & player_status);

};



#endif // BLACKJACK_H_