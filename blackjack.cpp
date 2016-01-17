#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <vector>
#include <cstdio>
//#include <unistd.h>
#include <getopt.h>

/**********/
// num decks uper limit = 16
// num boxes uper limit = 10
/**********/
using namespace std;
// number of decks in every shoe
#define DECKS_NUM 8
// player's betting unit
#define WAGER 100
// number of boxes in every round
#define BOXES	1
// rolling commission rate %
#define COM_RATE 1
// number of rounds in a trip
// only applicable when random shuffle is used
#define ROUNDS 1000
// number of shoes in a trip
// only applicable when manual shoe is used
#define SHOES 2
// number of trips in a simulation
#define TRIPS 10
// Detailed output or not?
#define DETAILED_OUTPUT 1
// generate graphs or not?
#define GENERATE_GRAPHS 1
// Shuffle method (0-> random shuffle, 1-> manual shoe)
#define SHUFFLE 1
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
// House BJ win all bets or original only? (0-> original, 1-> all)
#define BJWINALL 1


// uncomment next line for fixed player's first two cards
 #define P_FIXED_CARDS
// uncomment next line for fixed house first card
 #define H_FIXED_CARDS
// uncomment next line for a random slit game
// #define RANDOM_SPLIT
// note that random split does not work with if
// P_FIXED_CARDS is defined

static inline void loadbar(unsigned int x, unsigned int n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100+1) != 0) ) return;
 
    float ratio  =  x/(float)n;
    unsigned int   c      =  ratio * w;
 
    cout << setw(3) << (int)(ratio*100) << "% [";
    for (unsigned int x=0; x<c; x++) cout << "=";
    for (unsigned int x=c; x<w; x++) cout << " ";
    cout << "]\r" << flush;
}

double com_rate;

int num_decks,  num_boxes, num_shoes, num_trips, 
	detailed_out, generate_graphs, wager,
	total_cards, num_rounds;

int shuffle, num_splits, resplitA, soft17, push21, win777, surrender, double_any, 
	bjwinall;

void parse_args(int argc, char **argv) 
{
    int c;
    extern char *optarg;

    #define USAGE 	\
     	"usage:\t%s [--option argument]\n"                                								\
		"options:\n"                                                  									\
		"  --shuffle [0 or 1]			0-> random shuffle, 1-> manual shoe (default: 0)\n"				\
		"  --splits [3 or 4]			Number of split hands (default: 4)\n"          					\
		"  --resplitA [0 or 1]			Can resplit aces? 1-yes, 0-no (default: 1)\n"     				\
		"  --soft17 [0 or 1]			House soft 17 hit or stand? 0->hit, 1->stand (default: 0)\n"	\
		"  --push21 [0 or 1]			Player's 21 push or lose to house BJ? 0-> lose, 1-> push (default: 0)\n" \
		"  --win777 [0 or 1]			Player's 777 win double or wait for house card? 0-> wait, 1-> win (default: 0)\n" \
		"  --surrender [0 or 1]			Can Player surrender? 0-> no, 1-> yes (default: 1)\n" \
		"  --double [0 or 1]			Double on 9,10,11 or any 2 cards? 0-> 9,10,11, 1-> any (default: 1)\n" \
		"  --bjwinall [0 or 1]			House BJ win all bets or original bet only? 0-> original 1-> all (default: 1)\n" \
		"  --help                  		Show this help message\n"
    
    shuffle = SHUFFLE;
    num_splits = NUM_SPLITS;
    resplitA = RESPLITA;
    soft17 = SOFT17;
    push21 = PUSH21;
    win777 = WIN777;
    double_any = DOUBLE_ANY;
    surrender = SURRENDER;
    bjwinall = BJWINALL;
	num_rounds = ROUNDS;
    num_decks = DECKS_NUM;
    num_boxes = BOXES;
    num_trips = TRIPS;
    num_shoes = SHOES;
    wager = WAGER;
    com_rate = COM_RATE;
    detailed_out = DETAILED_OUTPUT;
    generate_graphs = GENERATE_GRAPHS;
    
	while(1){
   	    int option_index = 0;
	    static struct option long_options[] = {
	    	{"help",	         	no_argument,       0,  0 },
	    	{"shuffle",  			required_argument, 0,  0 },
	        {"splits",  			required_argument, 0,  0 },
	        {"resplitA",  			required_argument, 0,  0 },
	        {"soft17",  			required_argument, 0,  0 },
   	        {"push21",  			required_argument, 0,  0 },
   	        {"win777",  			required_argument, 0,  0 },
   	        {"surrender",  			required_argument, 0,  0 },
   	        {"double_any",  		required_argument, 0,  0 },
   	        {"bjwinall",  			required_argument, 0,  0 },
	        { 0 , 					0, 				   0,  0 }
	    };
	    //const char *optstring = "rn:dc:bx:tr:sh:wg:cr:";
	    c = getopt_long(argc, argv, "",long_options, &option_index);
	    //printf("c is %d\n",c);
	    if (c == -1) break;
        switch (c) {
        	case 0:
        		//printf("Option index is %d\n",option_index );
        		switch (option_index){
        			case 0:
        				printf( USAGE, argv[0]);
                		exit(1);
        				break;
        			case 1:
        				shuffle = atoi(optarg);
        				if (!(shuffle ==0 || shuffle ==1))
        				{
        					printf("shuffle argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
        			case 2:
        				num_splits = atoi(optarg);
        				if (!(num_splits ==3 || num_splits ==4))
        				{
        					printf("splits argument must be 3 or 4\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
        			case 3:
        				resplitA = atoi(optarg);
        				if (!(resplitA ==0 || resplitA ==1))
        				{
        					printf("resplitA argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
        			case 4:
        				soft17 = atoi(optarg);
        				if (!(soft17 ==0 || soft17 ==1))
        				{
        					printf("soft17 argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
        			case 5:
        				push21 = atoi(optarg);
        				if (!(push21 ==0 || push21 ==1))
        				{
        					printf("push21 argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
        			case 6:
        				win777 = atoi(optarg);
        				if (!(win777 ==0 || win777 ==1))
        				{
        					printf("win777 argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
        			case 7:
        				surrender = atoi(optarg);
        				if (!(surrender ==0 || surrender ==1))
        				{
        					printf("surrender argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
        			case 8:
        				double_any = atoi(optarg);
        				if (!(double_any ==0 || double_any ==1))
        				{
        					printf("double_any argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
 					case 9:
        				bjwinall = atoi(optarg);
        				if (!(bjwinall ==0 || bjwinall ==1))
        				{
        					printf("bjwinall argument must be 0 or 1\n");
        					printf( USAGE, argv[0]);
                			exit(1);
        				}
        				break;
       			default:
        				break;
        		}
                break;
            case '?':
                printf( USAGE, argv[0]);
                exit(1);
    	}
    }
    /*
    if (num_rows <= 0 || num_cols <= 0 || grid_size <= 0) {
        printf("Illegal argument value. All values must be numeric and greater than 0\n");
        exit(1);
    }
	*/
    printf("Shuffle method (0-> random shuffle, 1-> manual shoe)\t\t\t= %d\n", shuffle);
    printf("Number of split hands\t\t\t\t\t\t\t= %d\n", num_splits);
    printf("Can resplit Aces (1-yes, 0-no)\t\t\t\t\t\t= %d\n", resplitA);
    printf("House soft 17 hit or stand? (0->hit, 1->stand)\t\t\t\t= %d\n", soft17);
    printf("Player's 21 push or lose to house BJ? (0-> lose, 1-> push)\t\t= %d\n", push21);
    printf("Player's 777 win double or wait for house card? (0-> wait, 1-> win)\t= %d\n", win777);
    printf("Can Player surrender? (0-> no, 1-> yes)\t\t\t\t\t= %d\n", surrender);
    printf("Double down on 9,10,11 or any 2 cards? (0-> 9,10,11, 1-> any)\t\t= %d\n", double_any);
    printf("House BJ win all bets or original bet only? (0-> original, 1-> all)\t= %d\n", bjwinall);


    // 6 decks for random shuffle machine
    if (shuffle == 0)
    {
    	num_decks = 6;
    }

    total_cards = num_decks * 52;    
}


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
		house_string, player_A_exist,
		P_cards_char[2][10], P_status[10][4], P_string[10][4];

char card_array[13];

// Functions declaration
//void parse_args(int argc, char **argv); 
void html_files_init(), generate_WLstreak();
void html_files_close(long double hw,long double hl,
		long int tsw,long int tst, long int tsl);
void stand(), hit(), Double(), bj(), split(), surrender_f();
void draw_card(int & a, string & b);
int resolve_winner(int, int, double, long int, long double &, long int &,
		long int &, long int &, long int &, string &, string &, bool);
void html_print(long int, long int, ofstream &);
char stringconversion(int);
int numericconversion(int);
string check_player_1st_2_cards(int & A, int & B, int & C, int & D);
int player_hit(int & P_1st_card, int & P_2nd_card, int & H_1st_card,
		int & player_value, int & P_next_card, string & player_string);
int house_hit(int & house_value, string & house_status, string & house_string);
int player_split(int & card01, int & card02, int & next_card,
		string char_card01, string char_card02, string & char_next_card,
		int & player_value, string & player_string, string & player_status);

int main(int argc, char **argv) {

	//vector<bool> vector;
	//***uncomment this for a different random game
	//srand (time(NULL));

	parse_args(argc, argv);
	html_files_init();

	int interval;
	if (shuffle == 0){
		interval = max(1, num_trips* num_rounds / 100);
	}else{
		interval = max( num_shoes * num_trips *
				(total_cards / (num_boxes * 4 + 3)) / 100, 1);
	}
	
	int burn_card_num;
	string burn_card_char;

	for (int i = 0; i < total_cards; i++)
		shoe[i] = (i + 1) % 13 + 1;

	long int total_shoes_won = 0, total_shoes_lost = 0, total_shoes_tied = 0,
			 shoe_wins = 0, shoe_loses = 0, shoe_ties = 0, i = 0 ;
	long double shoe_score = 0, prev_score, highest_win = 0, highest_lose = 0;
	for (trips = 1; trips <= num_trips; trips++) {
		prev_score = score;
		long int trip_wins = 0, trip_loses = 0, trip_ties = 0;
		int shoes_won = 0, shoes_lost = 0, shoes_tied = 0;
		long double trip_score = 0, trip_high_win = 0, trip_high_lose = 0;
		shoes = 1;
		used_cards = 0;
		 

		random_shuffle(shoe, shoe + total_cards);
		
		draw_card(burn_card_num, burn_card_char);
		
		if (detailed_out){
			html << "<tr bgcolor=\"#439768\"> <td height = \"45\" colspan=9 > <b> \tTrip number\t"
				 << trips << "</b> </td></tr>\n";
			if(shuffle == 1){
				html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > <b> \tStart of shoe number\t" 
					<< shoes << "</b> </td></tr>\n";
			}
			html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Face up burnt card: "
				 << burn_card_char << " </td></tr>";
		}
		if (burn_card_num == 7) {
			for (int i = 0; i < 7; i++)
				draw_card(burn_card_num, burn_card_char);
			if(detailed_out){
				html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 7 more cards </td></tr>";
			}
		} else if (burn_card_num == 10) {
			for (int i = 0; i < 10; i++)
				draw_card(burn_card_num, burn_card_char);
			if(detailed_out){
				html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 10 more cards </td></tr>";
			}
		}
		if(shuffle == 1){
			shoe_html << "<tr bgcolor=\"#FFA319\"> <td height = \"45\" colspan=5 > <b> \tTrip number\t"
				  << trips << "</b> </td></tr>\n";
		}

		for (;;) {
			// Burn a card face down at the start of each hand
			draw_card(burn_card_num, burn_card_char);

			// if manual shoe
			if (shuffle == 1 && used_cards > 2 * total_cards / 3) {

				shoe_html << "<tr> <td> Shoe " << shoes << "</td><td>"
						<< wins - shoe_wins << "</td><td>" << ties - shoe_ties
						<< "</td><td>" << loses - shoe_loses << "</td><td>"
						<< fixed << score - shoe_score << "</td></tr>\n";
				trip_wins += wins - shoe_wins;
				trip_loses += loses - shoe_loses;
				trip_ties += ties - shoe_ties;
				trip_score += score - shoe_score;
				if (score > shoe_score) {
					shoes_won++;
				} else if (score < shoe_score) {
					shoes_lost++;
				} else {
					shoes_tied++;
				}
				shoe_wins = wins;
				shoe_loses = loses;
				shoe_ties = ties;
				shoe_score = score;
				shoes++;
				if (shoes > num_shoes) {
					break;
				}
				if(detailed_out){
					html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > <b> End of shoe as "
						<< used_cards << " cards have been played. <br>";
				}
				random_shuffle(shoe, shoe + total_cards);
				used_cards = 0;
				if(detailed_out){
					html << "\tStart of shoe number\t" << shoes << "</b> </td></tr>\n";
				}
				draw_card(burn_card_num, burn_card_char);
				if(detailed_out){
					html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Face up burnt card: "
					 << burn_card_char << " </td></tr>";
				}
				if (burn_card_num == 7) {
					for (int i = 0; i < 7; i++)
						draw_card(burn_card_num, burn_card_char);
					if(detailed_out){
						html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 7 more cards </td></tr>";
					}
				} else if (burn_card_num == 10) {
					for (int i = 0; i < 10; i++)
						draw_card(burn_card_num, burn_card_char);
					if(detailed_out){
					html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 10 more cards </td></tr>";
					}
				}
				loadbar(trips, num_trips);
			}else if(shuffle == 0 && i>=num_rounds){
				loadbar(trips, num_trips);
				trip_wins += wins - shoe_wins;
				trip_loses += loses - shoe_loses;
				trip_ties += ties - shoe_ties;
				trip_score += score - shoe_score;
				shoe_wins = wins;
				shoe_loses = loses;
				shoe_ties = ties;
				shoe_score = score;
				i = 0;
				break;
			}

			// first card of every box
			for (int k = 0; k < num_boxes; k++) {
				draw_card(P_cards[0][k], P_cards_char[0][k]);
			}

			// dealer's first card
			draw_card(H_1st_card, char_H_1st_card);
			
			// second card of every box
			for (int k = 0; k < num_boxes; k++) {
#ifdef RANDOM_SPLIT
				P_cards[1][k] = P_cards[0][k];
				P_cards_char[1][k] = P_cards_char[0][k];
#else
				draw_card(P_cards[1][k], P_cards_char[1][k]);
#endif
			}

// if you have defined P_FIXED_CARDS, here you can set player's 1st and 2nd cards
#ifdef P_FIXED_CARDS
			for (int k = 0; k < num_boxes; k++) {
				P_cards[0][k] = 5;		//player's 1st card
				P_cards_char[0][k] = stringconversion(P_cards[0][k]);
				P_cards[0][k] = numericconversion(P_cards[0][k]);
				P_cards[1][k] = 6;//player's 2nd card
				P_cards_char[1][k] = stringconversion(P_cards[1][k]);
				P_cards[1][k] = numericconversion(P_cards[1][k]);
			}
#endif


// if you have defined H_FIXED_CARDS, here you can set house 1st card
#ifdef H_FIXED_CARDS
			H_1st_card = 10;		//House 1st card
			char_H_1st_card = stringconversion(H_1st_card);
			H_1st_card = numericconversion(H_1st_card);
#endif

			house_string = char_H_1st_card;
			house_value = H_1st_card;
			house_status = "No_BJ";
			bjs = 0;
			triple7 = 0;
			busts = 0;
			surr = 0;
			for (box = 0; box < num_boxes; box++) {

				// must make it to make use of box to define where to draw cards
				char_P_1st_card = P_cards_char[0][box];
				char_P_2nd_card = P_cards_char[1][box];
				P_1st_card = P_cards[0][box];
				P_2nd_card = P_cards[1][box];

				player_string[box] = char_P_1st_card + char_P_2nd_card;

				player_value[box] = P_1st_card + P_2nd_card;
				BJ[box] = false;
				BJ_on = true;
				split_on = true;

				player_status[box] = check_player_1st_2_cards(P_cards[0][box],
						P_cards[1][box], H_1st_card, player_value[box]);

				if (player_status[box] == "Stand") {
					stand();
					bet[box] = wager;
				}

				if (player_status[box] == "Hit") {
					hit();
					bet[box] = wager;
				}

				if (player_status[box] == "Double") {
					Double();
					bet[box] = 2 * wager;
				}
				if (player_status[box] == "BJ") {
					bj();
					bet[box] = 3 * wager / 2;
				}
				if (player_status[box] == "Surrender") {
					surrender_f();
					bet[box] = wager / 2;
				}

				if (player_status[box] == "Split") {
					split();
				}

			}		// end of box
			player_busts += busts;
			triple7_wins += triple7;
			int certain_hands = busts + surr;
			if (win777 == 1){
				certain_hands += triple7;
			}
			if (certain_hands < num_boxes) {
				house_value = house_hit(house_value, house_status,
						house_string);
			}
			// shuffle = 0 -> random shuffle
			// -> shuffle at the end of every round
			if (shuffle == 0){
				random_shuffle(shoe, shoe + total_cards);
				used_cards = 0;
			}

			for (int k = 0; k < num_boxes; k++) {
				html_print(i, k, html);
			}
			if(generate_graphs && (total_i % interval == 0)){
				xml << "<data unit=\"" << hands << "\" value=\"" << fixed
						<< score << "\"/>\n";
			}
			
			if (score - prev_score > trip_high_win) {
				trip_high_win = score - prev_score;
			} else if (score - prev_score < trip_high_lose) {
				trip_high_lose = score - prev_score;
			}
			// end of round
			i++;
			total_i++;
		}		// end of shoe
		if(shuffle == 1){
			total_shoes_lost += shoes_lost;
			total_shoes_won += shoes_won;
			total_shoes_tied += shoes_tied;
			shoe_html << "<tr bgcolor=\"#439768\"> <td> Total </td><td>"
					<< trip_wins << "</td><td>" << trip_ties << "</td><td>"
					<< trip_loses << "</td><td>" << fixed << trip_score << "</td></tr>\n";
			shoe_html << "<tr bgcolor=\"#439768\"> <td> Shoe </td><td>" << shoes_won
					<< "</td><td>" << shoes_tied << "</td><td>" << shoes_lost
					<< "</td><td bgcolor=\"black \"></td></tr>\n";
			trip_html << "<tr><td>" << trips << "</td><td>" << trip_wins
				<< "</td><td>" << trip_ties << "</td><td>" << trip_loses
				<< "</td><td>" << fixed << trip_high_win << "</td><td>" << fixed
				<< trip_high_lose << "</td> <td>" << trip_score << "</td><td>"
				<< fixed << score << "</td><td>" << shoes_won << "</td><td>"
				<< shoes_tied << "</td><td>" << shoes_lost << "</td> </tr>\n";
		}else if(shuffle == 0){
			trip_html << "<tr><td>" << trips << "</td><td>" << trip_wins
				<< "</td><td>" << trip_ties << "</td><td>" << trip_loses
				<< "</td><td>" << fixed << trip_high_win << "</td><td>" << fixed
				<< trip_high_lose << "</td> <td>" << trip_score << "</td><td>"
				<< fixed << score 
				<< "</td><td>-</td><td>-</td><td>-</td></tr>\n";
		}
		if (trip_high_win > highest_win) {
			highest_win = trip_high_win;
		}
		if (trip_high_lose < highest_lose) {
			highest_lose = trip_high_lose;
		}
	}		// end of trip
	html_files_close(	highest_win, highest_lose, total_shoes_won,
						total_shoes_tied, total_shoes_lost);

	cout << "\n";
	return 0;
}



void draw_card(int & card, string & ch) {
	card = shoe[used_cards % total_cards];
	ch = stringconversion(card);
	card = numericconversion(card);
	used_cards++;
}

void draw_card(int & card, char & ch) {
	card = shoe[used_cards % total_cards];
	ch = stringconversion(card);
	card = numericconversion(card);
	used_cards++;
}

char stringconversion(int cardvalue) {
	const char card_array[] = "A23456789TJQK";
	assert(cardvalue >= 1 && cardvalue <= 13);
	return card_array[cardvalue - 1];
}

int numericconversion(int cardvalue) {
	if (cardvalue >= 10)
		cardvalue = 10;
	return cardvalue;
}

string check_player_1st_2_cards(int & P_1st_card, int & P_2nd_card,
		int & H_1st_card, int & player_value) {
// check for Blackjack
	string status;
	if ((P_1st_card == 1 && P_2nd_card == 10)
			|| (P_1st_card == 10 && P_2nd_card == 1)) {
		if (BJ_on)
			status = "BJ";
		else
			status = "21";
		player_value = 21;
		return status;
	}

// check for Pairs

	if (((P_1st_card == 2 && P_2nd_card == 2)
			|| (P_1st_card == 3 && P_2nd_card == 3)
			|| (P_1st_card == 7 && P_2nd_card == 7))
			&& (H_1st_card > 1.5 && H_1st_card < 7.5) && split_on) {
		status = "Split";
		return status;
	}

	if ((P_1st_card == 7 && P_2nd_card == 7) && (H_1st_card == 10)) {
		if(win777 == 0 && surrender == 1) 
			status = "Surrender";
		else 
			status = "Hit";
		return status;
	}

	if ((P_1st_card == 6 && P_2nd_card == 6)
			&& (H_1st_card > 1.5 && H_1st_card < 6.5) && split_on) {
		status = "Split";
		return status;
	}

	if ((P_1st_card == 4 && P_2nd_card == 4)
			&& (H_1st_card == 5 || H_1st_card == 6) && split_on) {
		status = "Split";
		return status;
	}

	if ((P_1st_card == 1 && P_2nd_card == 1) && H_1st_card != 1 && split_on) {
		status = "Split";
		return status;
	}

	if ((P_1st_card == 1 && P_2nd_card == 1) && H_1st_card == 1) {
		if(split_on && bjwinall == 0)
			status = "Split";
		else
			status = "Hit";
		return status;
	}

	if ((P_1st_card == 8 && P_2nd_card == 8)
			&& (H_1st_card > 1.5 && H_1st_card < 9.5) && split_on) {
		status = "Split";
		return status;
	}

	if ((P_1st_card == 8 && P_2nd_card == 8) && (H_1st_card == 1)) {
		status = "Hit";
		return status;
	}
	if ((P_1st_card == 8 && P_2nd_card == 8) && (H_1st_card == 10)) {
		if(surrender == 1){
			status = "Surrender";
		}else{
			status = "Hit";
		}
		return status;
	}

	if ((P_1st_card == 9 && P_2nd_card == 9)
			&& (H_1st_card == 2 || H_1st_card == 3 || H_1st_card == 4
					|| H_1st_card == 5 || H_1st_card == 6 || H_1st_card == 8
					|| H_1st_card == 9) && split_on) {
		status = "Split";
		return status;
	}

	if ((P_1st_card == 9 && P_2nd_card == 9)
			&& (H_1st_card == 7 || H_1st_card == 10 || H_1st_card == 1)) {
		status = "Stand";
		return status;
	}

	if ((P_1st_card == 5 && P_2nd_card == 5)
			&& (H_1st_card > 1.5 && H_1st_card < 9.5)) {
		status = "Double";
		return status;
	}

	if ((P_1st_card == 5 && P_2nd_card == 5)
			&& (H_1st_card == 10 || H_1st_card == 1)) {
		status = "Hit";
		return status;
	}

	if (P_1st_card == 10 && P_2nd_card == 10) {
		status = "Stand";
		return status;
	}

// Check for Double
	
	if (player_value == 11 && P_1st_card != 1 && P_2nd_card != 1 && H_1st_card !=1){
		if(bjwinall == 0 || H_1st_card != 10)
			status = "Double";
		else
			status = "Hit";
		return status;
	}
	
	/*
	if ((player_value == 11) && (P_1st_card != 1 && P_2nd_card != 1)
			&& (H_1st_card != 1 && H_1st_card != 10)) {
		status = "Double";
		return status;
	}
	*/
	if ((player_value == 10) && (P_1st_card != 1 && P_2nd_card != 1)
			&& (H_1st_card != 1 && H_1st_card != 10)) {
		status = "Double";
		return status;
	}

	if ((player_value == 9) && (P_1st_card != 1 && P_2nd_card != 1)
			&& (H_1st_card > 2.5 && H_1st_card < 6.5)) {
		status = "Double";
		return status;
	}


// check soft hands
	if ((P_1st_card == 1 && P_2nd_card == 9)
			|| (P_1st_card == 9 && P_2nd_card == 1)) {
		status = "Stand";
		return status;
	}

	if ((P_1st_card == 1 && P_2nd_card == 8)
			|| (P_1st_card == 8 && P_2nd_card == 1)) {
		status = "Stand";
		return status;
	}

	if ((P_1st_card == 1 && P_2nd_card == 7)
			|| (P_1st_card == 7 && P_2nd_card == 1)) {
		if (H_1st_card == 9 || H_1st_card == 10 || H_1st_card == 1) {
			status = "Hit";
			return status;
		} else if (H_1st_card == 2 || H_1st_card == 7 || H_1st_card == 8) {
			status = "Stand";
			return status;
		} else if (H_1st_card > 2.5 && H_1st_card < 6.5) {
			if(double_any == 1){
				status = "Double";
			}else{
				status = "Hit";
			}
			return status;
		}
	}

	if ((P_1st_card == 1 && (P_2nd_card == 2 || P_2nd_card == 3))
			|| ((P_1st_card == 2 || P_1st_card == 3) && P_2nd_card == 1)) {
		if (H_1st_card == 5 || H_1st_card == 6) {
			if(double_any == 1){
				status = "Double";
			}else{
				status = "Hit";
			}
			return status;
		} else {
			status = "Hit";
			return status;
		}

	}

	if ((P_1st_card == 1 && (P_2nd_card == 4 || P_2nd_card == 5))
			|| ((P_1st_card == 4 || P_1st_card == 5) && P_2nd_card == 1)) {
		if (H_1st_card == 4 || H_1st_card == 5 || H_1st_card == 6) {
			if(double_any == 1){
				status = "Double";
			}else{
				status = "Hit";
			}
			return status;
		} else {
			status = "Hit";
			return status;
		}
	}

	if ((P_1st_card == 1 && P_2nd_card == 6)
			|| (P_1st_card == 6 && P_2nd_card == 1)) {
		if (H_1st_card > 2.5 && H_1st_card < 6.5) {
			if(double_any == 1){
				status = "Double";
			}else{
				status = "Hit";
			}
			return status;
		} else {
			status = "Hit";
			return status;
		}
	}

// check for Hit or Stand
	if (player_value > 16.5) {
		status = "Stand";
		return status;
	}

	if (player_value == 13 || player_value == 14 || player_value == 15
			|| player_value == 16) {
		if (H_1st_card > 1.5 && H_1st_card < 6.5) {
			status = "Stand";
			return status;
		}
	}

	if (player_value == 13 && (H_1st_card < 1.5 || H_1st_card > 6.5)) {
		status = "Hit";
		return status;
	}

	if ((player_value == 14 || player_value == 15)
			&& (H_1st_card == 1 || H_1st_card == 7 || H_1st_card == 8
					|| H_1st_card == 9)) {
		status = "Hit";
		return status;
	}

	if ((player_value == 14 || player_value == 15) && (H_1st_card == 10)) {
		if(surrender == 1){
			status = "Surrender";
		}else{
			status = "Hit";
		}
		return status;
	}

	if ((player_value == 16)
			&& (H_1st_card == 1 || H_1st_card == 7 || H_1st_card == 8)) {
		status = "Hit";
		return status;
	}

	if ((player_value == 16) && (H_1st_card == 9 || H_1st_card == 10)) {
		if(surrender == 1){
			status = "Surrender";
		}else{
			status = "Hit";
		}
		return status;
	}

	if (player_value == 12) {
		if (H_1st_card > 3.5 && H_1st_card < 6.5) {
			status = "Stand";
			return status;
		} else if (H_1st_card < 3.5 || H_1st_card > 6.5) {
			status = "Hit";
			return status;
		}
	}

	if (player_value == 11 && (P_1st_card != 1 && P_2nd_card != 1)
			&& (H_1st_card == 1 || H_1st_card == 10)) {
		status = "Hit";
		return status;
	}

	if (player_value == 10 && (P_1st_card != 1 && P_2nd_card != 1)
			&& (H_1st_card == 1 || H_1st_card == 10)) {
		status = "Hit";
		return status;
	}

	if (player_value == 9 && (P_1st_card != 1 && P_2nd_card != 1)
			&& (H_1st_card < 2.5 || H_1st_card > 6.5)) {
		status = "Hit";
		return status;
	}
	if (player_value == 8 && (P_1st_card != 1 && P_2nd_card != 1)) {
		status = "Hit";
		return status;
	}

	if (player_value < 7.5) {
		status = "Hit";
		return status;
	}
	cout << "This was not supposed to happen" << endl;
	status = "Null";
	return status;
}

int player_hit(int & P_1st_card, int & P_2nd_card, int & H_1st_card,
		int & player_value, int & P_next_card, string & player_string) {
	p = 0;
	player_string = "";
	player_string += stringconversion(P_1st_card);
	player_string += stringconversion(P_2nd_card);

	player_next_card_loop:

	string char_P_next_card;
	p = p + 1;
	draw_card(P_next_card, char_P_next_card);

///777 rule
	if (P_1st_card == 7 && P_2nd_card == 7 && P_next_card == 7 && p == 1) {
		triple7++;
	}

	if (P_1st_card == 1 || P_2nd_card == 1 || P_next_card == 1)
		player_A_exist = "Yes";
	player_string = player_string + char_P_next_card;
	player_value = player_value + P_next_card;

	int player_value_hard = player_value;
	int player_value_soft = player_value + 10;

	if (player_A_exist == "Yes") {
		if (player_value < 11.5) {
			player_value_HS = player_value_soft;
			player_value_status = "soft";
		}
		if (player_value > 11.5 && player_value < 21.5) {
			player_value_HS = player_value_hard;
			player_value_status = "hard";
		}
		if (player_value > 21.5) {
			player_value_HS = player_value_hard;
			player_value_status = "hard";
		}
	}

// player < 11.5 & not soft hand then hit
	if (player_value < 11.5 && player_value_status != "soft")
		goto player_next_card_loop;
// 12 vs 2 & 3 hit
	if ((H_1st_card == 2 || H_1st_card == 3) && (player_value == 12))
		goto player_next_card_loop;
//player 12, 13, 14, 15 & 16 vs house 7, 8, 9, 10 & A
	if ((H_1st_card == 7 || H_1st_card == 8 || H_1st_card == 9
			|| H_1st_card == 10 || H_1st_card == 1)
			&& ((player_value > 11.5 && player_value < 16.5)
					|| (player_value_HS > 11.5 && player_value_HS < 16.5)))
		goto player_next_card_loop;
// soft A2, A3, A4, A5 & A6 hit
	if (player_value_status == "soft"
			&& ((player_value > 12.5 && player_value < 17.5)
					|| (player_value_HS > 12.5 && player_value_HS < 17.5)))
		goto player_next_card_loop;
// soft A7 hit if House 1st card is 9, 10 & A
	if (player_value_status == "soft"
			&& (player_value == 18 || player_value_HS == 18)
			&& (H_1st_card == 9 || H_1st_card == 10 || H_1st_card == 1))
		goto player_next_card_loop;

	if (player_A_exist == "Yes")
		player_value = player_value_HS;

	player_A_exist = "No";
	player_value_status = "";
	player_value_HS = 0;

	return player_value;
}

int house_hit(int & house_value, string & house_status, string & house_string) {

	//int H = 0;
	string house_add_ten = "No", house_A_exist = "No", 
		   hand_type = "Hard";
	
	house_status = "No_BJ";
	if (bjs == num_boxes) {
		if (H_1st_card == 1 || H_1st_card == 10) {
			draw_card(H_next_card, char_H_next_card);
			house_value = house_value + H_next_card;
			if (H_1st_card == 1 || H_next_card == 1) {
				house_value = house_value + 10;
			}
			if (house_value == 21)
				house_status = "BJ";
			house_string = house_string + char_H_next_card;
		}
		return house_value;
	} else {
		if(H_1st_card == 1)
			house_A_exist = "Yes";		
		
		draw_card(H_next_card, char_H_next_card);
		
		if ((H_1st_card == 1 && H_next_card == 10) 
				|| (H_1st_card == 10 && H_next_card == 1)) {
			house_status = "BJ";
			house_BJ = house_BJ + 1;
			house_value = 21;
			house_string = house_string + char_H_next_card;
			return house_value;
		}
		
		
house_next_card_loop:
		
		house_string = house_string + char_H_next_card;
		house_value = house_value + H_next_card;
		
		if (H_next_card == 1)
			house_A_exist = "Yes";
		/*
		else if(H_next_card == 1 && house_A_exist == "Yes" && house_add_ten == "No"){
			house_A_exist = "No";
			house_value += 10;
			//house_add_ten = "Yes";
		}
		*/
		if (house_A_exist == "Yes" && house_value < 11.5) {
			house_value = house_value + 10;
			house_add_ten = "Yes";
		}

		if (house_value > 21.5 && house_A_exist == "Yes"
				&& house_add_ten == "Yes") {
			house_value = house_value - 10;
			house_A_exist = "No";
			house_add_ten = "No";
		}
		
		draw_card(H_next_card, char_H_next_card);

		// house soft 17: hit if soft17==0, stand otherwise)
		if (house_value < 16.5 || (soft17 == 0 && house_A_exist == "Yes" && 
					house_add_ten == "Yes" && house_value == 17))
			goto house_next_card_loop;
		/*
		if(soft17==0 && house_A_exist=="Yes" && house_add_ten=="Yes" && house_value == 17)
			goto house_next_card_loop;
		*/
		if (house_value > 21)
			house_busts++;
		//house_A_exist = "No";
		//house_add_ten = "No";
		house_status = "No_BJ";
		return house_value;
	}
}

int player_split(int & card01, int & card02, int & next_card,
		string char_card01, string char_card02, string & char_next_card,
		int & player_split_value, string & player_split_string,
		string & player_split_status) {
	player_split_value = card01 + card02;
	player_split_string = char_card01 + char_card02;
	if (card01 == 1) {
		player_split_status = "Split/Stand";
		player_split_value = player_split_value + 10;
		return player_split_value;
	}

	split_on = false;
	player_split_status = check_player_1st_2_cards(card01, card02, H_1st_card,
			player_split_value);
	split_on = true;

	if (player_split_status == "Hit") {
		player_split_status = "Split/Hit";
		player_split_value = player_hit(card01, card02, H_1st_card,
				player_split_value, next_card, player_split_string);

	}

	if (player_split_status == "Stand") {
		player_split_status = "Split/Stand";
		if (card01 == 1 || card02 == 1)
			player_split_value = player_split_value + 10;
		//return player_split_value;
	}

	if (player_split_status == "Double") {
		player_split_status = "Split/Double";
		player_split_value = card01 + card02;
		draw_card(next_card, char_next_card);
		player_split_value = player_split_value + next_card;
		if (card01 == 1 || card02 == 1 || next_card == 1) {
			player_split_value += player_split_value + 10 > 21 ? 0 : 10;
		}
		player_split_string = player_split_string + char_next_card;
		//return player_split_value;
	}
//}
	return player_split_value;
}

void stand() {
	if (P_1st_card == 1 || P_2nd_card == 1) {
		player_value[box] = player_value[box] + 10;
	}

}

void hit() {
	player_value[box] = player_hit(P_1st_card, P_2nd_card, H_1st_card,
			player_value[box], P_next_card, player_string[box]);

	busts += player_value[box] > 21;

}

void Double() {
	player_value[box] = P_1st_card + P_2nd_card;
	draw_card(P_next_card, char_P_next_card);
	player_value[box] = player_value[box] + P_next_card;
	player_string[box] = player_string[box] + char_P_next_card;

	if ((P_1st_card == 1 || P_2nd_card == 1 || P_next_card == 1)
			&& player_value[box] < 11.5)
		player_value[box] = player_value[box] + 10;

	busts += player_value[box] > 21;

}

void bj() {
	bjs++;
	BJ[box] = true;
	player_status[box] = "<font color=\"green\"><b>BJ</b></font>";
}

void surrender_f() {
	surr++;
}

void split() {
	int P_split_cards[2*num_splits];
	char P_chars[2*num_splits];
	memset(P_split_cards, 0, sizeof(P_split_cards));
	memset(P_chars, 0, sizeof(P_chars));
	P_split_cards[0] = P_1st_card;
	P_split_cards[2] = P_2nd_card;
	P_chars[0] = char_P_1st_card.at(0);
	P_chars[2] = char_P_2nd_card.at(0);
	draw_card(P_split_cards[1], P_chars[1]);
	draw_card(P_split_cards[3], P_chars[3]);
	int split_hands = 2;
	bool flag = true;
	// if we have ace split and resplitA == 0, we cannot resplit
	if(resplitA || P_chars[0]!='A'){
		while (split_hands <= num_splits && flag) {
			int i = 0;
			flag = false;
			while (i < 2 * split_hands && split_hands < num_splits) {
				if (P_split_cards[i] == P_split_cards[i + 1]) {
					P_split_cards[2 * split_hands] = P_split_cards[i + 1];
					P_chars[2 * split_hands] = P_chars[i + 1];
					draw_card(P_split_cards[i + 1], P_chars[i + 1]);
					draw_card(P_split_cards[2 * split_hands + 1],
							P_chars[2 * split_hands + 1]);
					split_hands++;

				} else {
					i = i + 2;
				}
			}
		}
	}
	for (int i = 0; i < 2 * split_hands; i = i + 2) {
		//do something for bj after ace split
		player_split(P_split_cards[i], P_split_cards[i + 1], next_card,
				string(1, P_chars[i]), string(1, P_chars[i + 1]),
				char_next_card, P_value[box][i / 2], P_string[box][i / 2],
				P_status[box][i / 2]);
	}
	for (int i = split_hands; i < num_splits; ++i) {
		P_value[box][i] = 0;
	}

	flag = true;
	for (int i = 0; i < split_hands; ++i) {
		flag = flag && (P_value[box][i] > 21);
	}
	busts += flag;
}

int resolve_winner(int player_value, int house_value, double bet, long int box,
		long double & score, long int & hand, long int & loses, long int & wins,
		long int & ties, string & player_status, string & player_string, bool first_hand) {
	hand++;
	if (player_string == "777" && win777 == 1) {
		score += 2 * bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (player_status == "Surrender") {
		if(bjwinall == 0 && first_hand && house_status == "BJ"){
			ties++;		
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		}else{
			score -= (bet * (1 - 0.01 * com_rate));
			//printf("Score is %Lf\n",score);
			//cout << "\nScore " << score;
			loses++;
			print_result = "<font color=\"red\"><b>Loss</b></font>";
			return -1;
		}
	} else if (player_value > 21.5) {
		if(bjwinall == 0 && !first_hand && house_status == "BJ"){
			ties++;		
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		}else{
			score -= (bet * (1 - 0.01 * com_rate));
			loses++;
			print_result = "<font color=\"red\"><b>Loss</b></font>";
			return -1;
		}
	} else if (house_value > 21.5) {
		score += bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (house_value > player_value) {	
		if(bjwinall == 0 && !first_hand && house_status == "BJ"){
			ties++;		
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		}else{
			score -= (bet * (1 - 0.01 * com_rate));
			loses++;
			print_result = "<font color=\"red\"><b>Loss</b></font>";
			return -1;
		}
	} else if (house_value < player_value) {
		score += bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (house_status == "BJ" && BJ[box] == false && player_value < 21) {	
		if(bjwinall == 0 && !first_hand && house_status == "BJ"){
			ties++;		
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		}else{
			score -= (bet * (1 - 0.01 * com_rate));
			loses++;
			print_result = "<font color=\"red\"><b>Loss</b></font>";
			return -1;
		}
	} else if (house_status == "BJ" && BJ[box] == false && player_value == 21) {
		// player 21, house BJ
		if(push21 == 0){ // lose
			if(bjwinall == 0 && !first_hand && house_status == "BJ"){
				ties++;		
				print_result = "<b>Push</b>";
				houseBJ_P21_pushes++;
				return 0;
			}else{
				score -= (bet * (1 - 0.01 * com_rate));
				loses++;
				print_result = "<font color=\"red\"><b>Loss</b></font>";
				return -1;
			}
		}else{	//push
			ties++;
			houseBJ_P21_pushes++;
			print_result = "<b>Push</b>";
			return 0;
		}
	} else if (house_status != "BJ" && BJ[box] == true) {
		score += bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (house_value == player_value) {
		ties++;
		print_result = "<b>Push</b>";
		return 0;
	} else {
		cout << "[Resolve winner] this was not supposed to happen\n";
		return 0;
	}

}

void html_print(long int i, long int box, ofstream & html) {
	stringstream s, s2, s3;
	int res;
	bool first_hand = true;
	if (box == 0) {
		s << house_value;
		s2 << house_string;
		s3 << i;
	}
	if (house_status == "BJ" && box == 0) {
		s.str("");
		s << "<font color=\"green\"><b>BJ</b></font>";
	}

	if (player_status[box] != "Split") {

		res = resolve_winner(player_value[box], house_value, bet[box], box,
				score, hands, loses, wins, ties, player_status[box],
				player_string[box], first_hand);
		first_hand = false;			
		v.push_back(res > 0);
		
		if (player_status[box] == "Double") {
			resolve_winner(player_value[box], house_value, bet[box], box,
					dscore, dhands, dloses, dwins, dties, player_status[box],
					player_string[box], first_hand);
		}
		// if you lose original bet, house has BJ, you doubled down but lost
		// you have not splitted hands, you take half your bet back
		
		if(bjwinall == 0 && house_status == "BJ" && res < 0 && player_status[box] == "Double"){
			//score += wager; 
			score += wager * (1 - 0.01 * com_rate);
		}
		
		if (player_string[box] == "777") {
			player_string[box] = "<b>777</b>";
		}
		if(detailed_out){
			html << "<tr> <td>" << s3.str() << "</td> <td>" << s2.str()
				<< "</td> <td>" << s.str() << "</td> <td>" << box + 1
				<< "</td> <td>" << player_string[box] << "</td> <td>"
				<< player_value[box] << "</td> <td>"<< fixed << score
				<< "</td> <td>" << player_status[box] << "</td> <td>"
				<< print_result << "</td></tr>\n";
		}
	} else if (player_status[box] == "Split") {
		// print 1st split hand
		int j = 0;
		while (j < num_splits && P_value[box][j] > 0) {
			if (P_status[box][j].find("Double") != string::npos) {
				bet[box] = 2 * wager;
				resolve_winner(P_value[box][j], house_value, bet[box], box,
						dscore, dhands, dloses, dwins, dties, P_status[box][j],
						P_string[box][j], false);
			} else {
				bet[box] = wager;
			}
			
			res = resolve_winner(P_value[box][j], house_value, bet[box], box,
					score, hands, loses, wins, ties, P_status[box][j],
					P_string[box][j], first_hand);
			
			v.push_back(res > 0);
			// if you lose only original bet, house has BJ, you lost the round,
			// and its not the first Split hand, you take back your money
			
			if(bjwinall == 0 && house_status == "BJ" && res < 0 && first_hand &&
					P_status[box][j].find("Double") != string::npos ){
					//score += wager;
					score += wager * (1 - 0.01 * com_rate);
			}
			
			resolve_winner(P_value[box][j], house_value, bet[box], box, sscore,
					shands, sloses, swins, sties, P_status[box][j],
					P_string[box][j], first_hand);
			first_hand = false;
			if(detailed_out){
				if (j == 0) {
					html << "<tr> <td>" << s3.str() << "</td> <td>" << s2.str()
							<< "</td> <td>" << s.str() << "</td> <td>";
				} else {
					html << "<tr> <td></td> <td></td> <td></td> <td>";
				}
				if (P_string[box][j] == "777") {
					P_string[box][j] = "<b>777</b>";
				}
				html << box + 1 << "</td> <td>" << P_string[box][j] << "</td> <td>"
						<< P_value[box][j] << "</td> <td>" << fixed << score
						<< "</td> <td>" << P_status[box][j] << "</td> <td>"
						<< print_result << "</td></tr>\n";
			}
			P_value[box][j] = 0;
			j++;
		}
	} else {
		cout << "[Html Print] this was not supposed to happen\n";
	}

}

void html_files_init() {
	remove("output/BJgame.html");
	remove("output/WLdata.xml");
	remove("output/linedata.xml");
	remove("output/overall.html");
	remove("output/shoe_overall.html");
	remove("output/trip_overall.html");
	remove("output/win_lose_streak.html");
	
	html.open("output/BJgame.html");
	//html.precision(0);
	xml.open("output/linedata.xml");
	if (shuffle == 1){
		shoe_html.open("output/shoe_overall.html");
	}
	trip_html.open("output/trip_overall.html");
	overall_stats.open("output/overall.html");
	WL_streak.open("output/win_lose_streak.html");
	WLdata.open("output/WLdata.xml");

	trip_html.precision(2);
	trip_html << "<!DOCTYPE html><html><head><style>table, th, td"
			<< " { border: 1px solid black; border-collapse: collapse;"
			<< "text-align:center }\n caption {font-weight:bold;\n font-size:1.1em;\n}</style> </head><body>\n"
			<< "<table style=\"width:100%\">\n"
			<< "<caption>Trip Information</caption>"
			<< "<tr> <th>Trips</th> <th>Win</th> <th>Tie</th> "
			<< " <th>Lose</th> <th>Highest Win</th> <th>Highest Lose</th>"
			<< "<th>Balance</th> <th>Overall Balance</th> <th>Shoe Win</th><th>Shoe Tie</th> <th>Shoe Lose</th></tr>\n";

	if (shuffle == 1){
		shoe_html.precision(2);
		shoe_html << "<!DOCTYPE html><html><head><style>table, th, td"
				<< " { border: 1px solid black; border-collapse: collapse;"
				<< "text-align:center }\n caption {font-weight:bold;\n font-size:1.1em;\n}</style> </head><body>\n"
				<< "<table style=\"width:100%\">\n"
				<< "<caption>Shoe Information</caption>"
				<< "<tr> <th>Shoe</th> <th>Win</th> <th>Tie</th> "
				<< " <th>Lose</th> <th>Balance</th></tr>\n";
	}

	xml.precision(2);
	xml << "<?xml version=\"1.0\"?>\n <JSChart> <optionset> \n"
			<< "<option set=\"setLineColor\" value=\"'#8D9386'\"/>\n"
			<< "<option set=\"setLineWidth\" value=\"3\"/>\n"
			<< "<option set=\"setGridColor\" value=\"'#a4a4a4'\"/>\n"
			<< "<option set=\"setGridOpacity\" value=\"0.8\"/>\n"
			<< "<option set=\"setAxisNameX\" value=\"'Hand'\"/>\n"
			<< "<option set=\"setAxisNameY\" value=\"''\"/>\n"
			<< "<option set=\"setAxisValuesColor\" value=\"'#333639'\"/>\n"
			<< "<option set=\"setAxisNameColor\" value=\"'#333639'\"/>\n"
			<< "<option set=\"setAxisColor\" value=\"'#9F0505'\"/>\n"
			<< "<option set=\"setTitle\" value=\"'Player Balance Graph'\"/>\n"
			<< "<option set=\"setTitleColor\" value=\"'#7D7D7D'\"/>\n"
			<< "<option set=\"setTitleFontSize\" value=\"16\"/>\n"
			<< "<option set=\"setSize\" value=\"1000,400\"/>\n"
			<< "</optionset> <dataset type=\"line\">\n";

	html.precision(2);
	html << "<!DOCTYPE html><html><head><style>table, th, td"
			<< " { border: 1px solid black; border-collapse: collapse;"
			<< "text-align:center } </style> </head><body>\n"
			<< "<table style=\"width:100%\">\n"
			<< "<tr> <th>Hand</th><th>House Cards</th> <th>House Total</th> <th>Box</th> "
			<< " <th>Player Cards</th> <th>Player Total</th> <th>Player Balance</th>"
			<< " <th>Player Status</th> <th>Game Result</th></tr>\n";

	overall_stats.precision(2);
	overall_stats << "<!DOCTYPE html><html><head><style>table, th, td"
			<< " { border: 1px solid black; border-collapse: collapse;"
			<< "text-align:center } </style> <script type=\"text/javascript\""
			<< " src=\"https://www.google.com/jsapi\"></script>"
			<< "<script type=\"text/javascript\" src=\"jscharts.js\"> </script> </head><body>\n"
			<< "<table style=\"width:100%\">\n";

}

void generate_WLstreak() {
	//in v there is true-false depending on win/lose
	//must create a new vector with wl streak values
	// create data.xml and color data
	vector<int> streak_v;
	int streak = 1;
	int max = 1;
	bool res = v.at(0);
	for (unsigned int i = 1; i < v.size(); i++) {
		if (res == v.at(i)) {
			streak++;
		} else {
			res = !res;
			streak_v.push_back(streak);
			if (streak > max) {
				max = streak;
			}
			streak = 1;
		}
	}
	streak_v.push_back(streak);

	WLdata << "<?xml version=\"1.0\"?>\n <JSChart> <optionset>\n"
			<< "<option set=\"setAxisColor\" value=\"'#9D9F9D'\"/>\n"
			<< "<option set=\"setAxisWidth\" value=\"1\"/>\n"
			<< "<option set=\"setAxisNameX\" value=\"''\"/>\n"
			<< "<option set=\"setAxisNameY\" value=\"'Streak value'\"/>\n"
			<< "<option set=\"setAxisNameColor\" value=\"'#655D5D'\"/>\n"
			<< "<option set=\"setAxisNameFontSize\" value=\"9\"/>\n"
			<< "<option set=\"setAxisPaddingLeft\" value=\"20\"/>\n"
			<< "<option set=\"setAxisValuesDecimals\" value=\"0\"/>\n"
			<< "<option set=\"setAxisValuesNumberY\" value=\"" << max
			<< "\"/>\n"
			<< "<option set=\"setAxisValuesColor\" value=\"'#9C1919'\"/>\n"
			<< "<option set=\"setAxisValuesFontSizeX\" value=\"5\"/>\n"
			<< "<option set=\"setTextPaddingLeft\" value=\"0\"/>\n"
			<< "<option set=\"setBarValues\" value=\"true\"/>\n"
			<< "<option set=\"setBarValuesFontSize\" value=\"5\"/>\n"
			<< "<option set=\"setBarBorderWidth\" value=\"0\"/>\n"
			<< "<option set=\"setTitleColor\" value=\"'#8C8382'\"/>\n"
			<< "<option set=\"setTitle\" value=\"'Win/Lose Streak Bar Chart'\"/>\n"
			<< "<option set=\"setGridColor\" value=\"'#5D5F5D'\"/>\n"
			<< "<option set=\"setAxisReversed\" value=\"true\"/>\n"
			<< "<option set=\"setIntervalEndY\" value=\"" << max << "\"/>\n"
			<< "<option set=\"setSize\" value=\"600,"
			<< min(20 * (int) streak_v.size(), 30000) << "\"/>\n"
			<< "</optionset> <dataset type=\"bar\">\n";

	for (unsigned int i = 0; i < streak_v.size(); i++) {
		WLdata << "<data unit=\"" << i << "\" value=\"" << streak_v.at(i)
				<< "\"/>\n";
		//cout << streak_v.at(i) << "\t" << res << endl;
		//res = !res;
	}
	WLdata << "</dataset>\n<colorset>";
	res = v.at(0);
	for (unsigned int i = 0; i < streak_v.size(); i++) {
		if (res) {
			WLdata << "<color value=\"#0000B2\" />\n";
		} else {
			WLdata << "<color value=\"#990000\" />\n";
		}
		res = !res;
	}

	WLdata << "</colorset></JSChart>";
	WLdata.close();

	WL_streak << "<!DOCTYPE html><html><head>"
			<< "<script type=\"text/javascript\" src=\"jscharts.js\"> </script> </head><body>\n"
			<< "<div id=\"barchart\"></div>\n"
			<< "<script type=\"text/javascript\">\n var myChart = new JSChart('barchart', 'bar');"
			<< "myChart.setDataXML(\"WLdata.xml\"); myChart.draw();\n</script>";
	WL_streak << "</body></html>";
	WL_streak.close();
}


void html_files_close(long double highest_win,long double highest_lose,
		long int total_shoes_won,long int total_shoes_tied, long int total_shoes_lost) {
	trip_html << "<tr><td>Total</td><td>" << wins << "</td><td>" << ties
			<< "</td><td>" << loses << "</td><td>" << fixed << highest_win
			<< "</td><td>" << fixed << highest_lose << "</td> <td>" << score
			<< "</td> <td>" << score
			<< "</td><td>" << total_shoes_won << "</td><td>" << total_shoes_tied
			<< "</td><td>" << total_shoes_lost << "</td> </tr>\n";
	trip_html << "</table></body></html>\n";
	trip_html.close();

	if(shuffle == 1){
		shoe_html << "</table></body></html>\n";
		shoe_html.close();
	}	
	html << "</table></body></html>\n";
	html.close();
	xml << "</dataset></JSChart>\n";
	xml.close();

	overall_stats << "<tr> <th>Total Hands</th><th>Total Wins</th><th>Total Ties</th>"
			<< " <th>Total Defeats</th> <th>Total Balance</th> </tr>\n"
			<< "<tr> <td>" << hands << "</td><td>" << wins << "</td><td>"
			<< ties << "</td><td>" << loses << "</td><td>" << fixed << score
			<< "</td></tr>"; //<< "</table><br>\n";
	//overall_stats << "<table style=\"width:100%\">\n";
	overall_stats << "<tr> <th>Double Hands</th><th>Double Wins</th><th>Double Ties</th>"
			<< " <th>Double Defeats</th> <th>Double Balance</th> </tr>\n"
			<< "<tr> <td>" << dhands << "</td><td>" << dwins << "</td><td>"
			<< dties << "</td><td>" << dloses << "</td><td>" << fixed << dscore
			<< "</td></tr>";	//<<"</table><br>\n";
	//overall_stats << "<table style=\"width:100%\">\n";
	overall_stats << "<tr> <th>Split Hands</th><th>Split Wins</th><th>Split Ties</th>"
			<< " <th>Split Defeats</th> <th>Split Balance</th> </tr>\n"
			<< "<tr> <td>" << shands << "</td><td>" << swins << "</td><td>"
			<< sties << "</td><td>" << sloses << "</td><td>" << fixed << sscore
			<< "</td></tr></table>\n";
	overall_stats << "<p><b>Total Player Busts:\t</b>" << player_busts
			<< " ( " << fixed << setprecision(2) << player_busts*100.0/hands << "% )" 
			<< "</p><p><b>Total House Busts:\t</b>" << house_busts 
			<< " ( " << fixed << setprecision(2) << house_busts*100.0/ total_i << "% )"  
			<< "</p><p><b>Total 777 wins:\t</b>" << triple7_wins << "</p>"
			<< "<p><b>Total Pushes with Player 21 vs House BJ:\t</b>"
			<< houseBJ_P21_pushes << "</p>";
	overall_stats << "<div id=\"piechart\" style=\"width: 900px; height: 500px;\"></div>\n"
			<< "<div id=\"linechart\" style=\"width: 900px; height: 500px;\"></div>\n"
			<< "<script type=\"text/javascript\">\n"
			<< " google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});\n"
			<< "google.setOnLoadCallback(drawChart);\n"
			<< "function drawChart() {\n"
			<< "var data = google.visualization.arrayToDataTable([\n"
			<< "['Task', 'Hours per Day']," << "['Wins'," << wins << "     ],\n"
			<< "['Defeats',  " << loses << "],['Ties',  " << ties << "]]);\n"
			<< "var options = { title: 'Total results after " << hands
			<< " hands'};\n"
			<< "var chart = new google.visualization.PieChart(document.getElementById('piechart'));\n"
			<< "chart.draw(data, options);} \n</script>\n";
	if(generate_graphs){
		overall_stats << "<script type=\"text/javascript\">\n var myChart = new JSChart('linechart', 'line');"
			<< "myChart.setDataXML(\"linedata.xml\"); myChart.draw();\n</script>";
	}

	overall_stats << "</body></html>\n";
	overall_stats.close();
	if(generate_graphs){
		generate_WLstreak();
	}

}

