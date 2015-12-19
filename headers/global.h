#ifndef GLOBAL_H
#define GLOBAL_H


// number of decks in every shoe
#define DECKS_NUM 8
// player's betting unit
#define WAGER 20
// number of boxes in every round
#define BOXES	1
// rolling commission rate %
#define COM_RATE 1
// number of rounds in a trip
// only applicable when random shuffle is used
#define ROUNDS 100
// number of shoes in a trip
// only applicable when manual shoe is used
#define SHOES 5
// number of trips in a simulation
#define TRIPS 10
// Detailed output or not?
#define DETAILED_OUTPUT 1
// generate graphs or not?
#define GENERATE_GRAPHS 1
// Shuffle method (0-> random shuffle, 1-> manual shoe)
#define SHUFFLE 0
// Split to 3 or 4 hands
#define NUM_SPLITS 4
// Can resplit Aces? (0-> no, 1-> yes)
#define RESPLITA 1
// House soft 17 hit or stand? (0-> hit, 1-> stand)
#define SOFT17 0
// Player's 21 push or lose to house BJ? (0-> lose, 1-> push)
#define PUSH21 0
// Player's 777 win double instantly or wait for house card? (0-> wait, 1-> win)
#define WIN777 0
// Can the player surrender? (0-> no, 1-> yes)
#define SURRENDER 1
// Double down on 9,10,11 or any 2 cards? (0-> 9,10,11, 1-> any 2 cards)
#define DOUBLE_ANY 1

// uncomment next line for fixed player's first two cards
 #define P_FIXED_CARDS
// uncomment next line for fixed house first card
// #define H_FIXED_CARDS
// uncomment next line for a random slit game
// #define RANDOM_SPLIT
// note that random split does not work with if
// P_FIXED_CARDS is defined




int num_decks,  num_boxes, num_shoes, num_trips, 
	detailed_out, generate_graphs, wager, com_rate,
	total_cards, num_rounds;

int shuffle, num_splits, resplitA, soft17, push21, win777, surrender, double_any;


ofstream html;
ofstream xml;
ofstream shoe_html;
ofstream trip_html;
ofstream overall_stats;
ofstream WL_streak;
ofstream WLdata;

std::vector<bool> v;

int P_1st_card, P_2nd_card, H_1st_card, P_next_card, H_next_card,
	card01, card02, next_card, p, H = 0,
	player_split_value, player_value_hard, player_value[10],
	player_value_soft, player_value_HS, house_value, house_BJ,
	shoe[16 * 52], used_cards;
// rows are 1st - 2nd card, columns are different boxes
int P_cards[2][10], P_value[10][4];

bool player_bust[10], BJ[10], split_on, BJ_on;

long int wins = 0, loses = 0, ties = 0, hands = 0,
		 dwins = 0, dloses = 0, dties = 0, dhands = 0,
		 swins = 0, sloses = 0, sties = 0, shands = 0,
		 box, bjs, busts, surr, triple7,
		 house_busts = 0, player_busts = 0, triple7_wins = 0,
		 houseBJ_P21_pushes = 0, shoes, trips, total_i = 0;

double bet[10];
long double score = 0.0, dscore = 0.0, sscore = 0.0;

string 	char_P_1st_card, char_P_2nd_card, char_P_next_card, char_H_next_card,
		char_H_1st_card, print_result,
		char_card01, char_card02, char_next_card, player_status[10],
		player_split_status, house_status,
		player_value_status, player_string[10], player_split_string,
		house_string, player_A_exist, house_A_exist, house_add_ten,
		P_cards_char[2][10], P_status[10][4], P_string[10][4];

char card_array[13];


#endif
