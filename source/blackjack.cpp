#include "../headers/util.h"
#include "../headers/blackjack.h"
#include "../headres/global.h"

using namespace std;

/*
// Functions declaration
//void parse_args(int argc, char **argv); 
void html_files_init(), generate_WLstreak();
void html_files_close(long double hw,long double hl,
		long int tsw,long int tst, long int tsl);
void stand(), hit(), Double(), bj(), split(), surrender_f();
void draw_card(int & a, string & b);
int resolve_winner(int, int, double, long int, long double &, long int &,
		long int &, long int &, long int &, string &, string &);
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
*/
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
				P_cards[0][k] = 7;		//player's 1st card
				P_cards_char[0][k] = stringconversion(P_cards[0][k]);
				P_cards[0][k] = numericconversion(P_cards[0][k]);
				P_cards[1][k] = 2;//player's 2nd card
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





