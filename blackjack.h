#ifndef BLACKJACK_H_
#define BLACKJACK_H_

#include <vector>
#include <fstream>


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
const int SHOES = 10;
// number of trips in a simulation
const int TRIPS = 10;
// Detailed output or not?
const int DETAILED_OUTPUT = 1;
// generate graphs or not?
const int GENERATE_GRAPHS = 1;
// Shuffle method (0-> random shuffle, 1-> manual shoe)
const int SHUFFLE = 1;
// Split to 3 or 4 hands
const int NUM_SPLITS = 3;
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
// #define P_FIXED_CARDS
// uncomment next line for fixed house first card
// #define H_FIXED_CARDS
// uncomment next line for a random slit game
// #define RANDOM_SPLIT
// note that random split does not work with if
// P_FIXED_CARDS is defined


// void loadbar(unsigned int x, unsigned int n, unsigned int w = 50);

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
	int com_rate = COM_RATE;
	int detailed_out = DETAILED_OUTPUT;
	int generate_graphs = GENERATE_GRAPHS;

	int used_cards, total_cards, triple7, bjs,
	    house_BJ, house_busts, busts, surr,
	    hand, houseBJ_P21_pushes;

	bool BJ_on, split_on;
	int wins = 0, loses = 0, ties = 0, hands = 0,
	    dwins = 0, dloses = 0, dties = 0, dhands = 0,
	    swins = 0, sloses = 0, sties = 0, shands = 0;

	double score = 0.0, dscore = 0.0, sscore = 0.0;

	int trip_wins, trip_loses, trip_ties;
	int shoes_won, shoes_lost, shoes_tied;
	double trip_score, trip_high_win, trip_high_lose;

	int total_shoes_won = 0, total_shoes_lost = 0,
	    total_shoes_tied = 0;
	double highest_win = 0, highest_lose = 0;

	int player_busts;
	int triple7_wins;

	int total_i;

	ofstream html;
	ofstream xml;
	ofstream shoe_html;
	ofstream trip_html;
	ofstream overall_stats;
	ofstream WL_streak;
	ofstream WLdata;

	vector<int> shoe;
	vector<bool> v;

	BlackJack(int argc, char *argv[]);
	~BlackJack() {};
	void end_of_trip(double &highest_win, double &highest_lose, int trip);
	void start_of_trip(int trips);
	void end_of_shoe(int &shoes, int &shoe_wins,
	                 int &shoe_loses, int &shoe_ties,
	                 double &shoe_score, int i);

	void end_of_hand(int i, int interval, double prev_score,
	                 int P_value[10][4], string P_status[10][4],
	                 string P_string[10][4], int house_value,
	                 string house_string, string house_status,
	                 int *bet, bool *BJ);
	int calc_interval();
	void burn_card();
	void player_play_hand(int P_1st_card, int P_2nd_card,
	                      string char_P_1st_card,
	                      string char_P_2nd_card,
	                      int H_1st_card,
	                      string char_H_1st_card,
	                      string P_string[4],
	                      int P_value[4], string P_status[4],
	                      int &bet, bool& BJ);

	void generate_WLstreak();

	void html_files_close(double hw, double hl,
	                      int tsw, int tst,
	                      int tsl);

	void stand(int P_1st_card, int P_2nd_card, int &P_value);

	void hit(int P_1st_card, int P_2nd_card,
	         string char_P_1st_card, string char_P_2nd_card,
	         int H_1st_card, int &P_value,
	         string &P_string);

	void Double(int P_1st_card, int P_2nd_card,
	            int &P_value, string &P_string);

	void bj(bool &BJ, string &P_status);

	void split(int P_1st_card, int P_2nd_card,
	           string char_P_1st_card, string char_P_2nd_card,
	           int P_value[4], string P_string[4],
	           string P_status[4], int H_1st_card);

	void surrender_f();
	void draw_card(int &a, string &b);

	int resolve_winner(int P_value, int house_value,
	                   double bet, int box, double & score,
	                   int & hand, int & loses,
	                   int & wins, int & ties,
	                   string P_status, string P_string,
	                   string house_status, bool first_hand,
	                   string &print_result, bool BJ);

	string stringconversion(int);
	int numericconversion(int);

	string check_player_1st_2_cards(int P_1st_card, int P_2nd_card,
	                                int H_1st_card, int & P_value);

	int player_hit(int P_1st_card, int P_2nd_card,
	               string char_P_1st_card, string char_P_2nd_card,
	               int H_1st_card, int & P_value,
	               string & P_string);

	int house_hit(int H_1st_card, int & house_value,
	              string & house_status,
	              string & house_string);

	void player_split(int card01, int card02,
	                 string char_card01,
	                 string char_card02,
	                 int & player_split_value,
	                 string & player_split_string,
	                 string & player_split_status,
	                 int H_1st_card);

};



#endif // BLACKJACK_H_