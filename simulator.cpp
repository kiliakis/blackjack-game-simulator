#include "blackjack.h"

using namespace std;


int main(int argc, char **argv) {

	//vector<bool> vector;
	//***uncomment this for a different random game
	//srand (time(NULL));
	auto bj = new BlackJack();

	int interval = bj->calc_interval();

	// long int total_shoes_won, total_shoes_lost, total_shoes_tied,
	long int shoe_wins, shoe_loses, shoe_ties, i;

	// total_shoes_won = total_shoes_lost = total_shoes_tied = 0;
	shoe_wins = shoe_loses = shoe_ties = i = 0;

	long double shoe_score, prev_score;//, highest_win, highest_lose;
	shoe_score = prev_score = 0;// = highest_win = highest_lose = 0;

	for (int trips = 1; trips <= bj->num_trips; trips++) { //start of trip
		prev_score = bj->score;
		int shoes = 1;

		bj->start_of_trip();

		for (;;) { // start of shoe, hand

			string P_cards_char[2][10];
			string P_string[10][4];
			string P_status[10][4];
			int P_cards[2][10];
			int P_value[10][4];

			// TODO continue from here

			// Burn a card face down at the start of each hand
			bj->burn_card();

			// if manual shoe
			// check if it's time to end shoe
			// bj->check_end_of_shoe();
			if (bj->shuffle == 1 && bj->used_cards > 2 * bj->total_cards / 3) {
				loadbar(trips, bj->num_trips);
				bj->end_of_shoe();
			} else if (bj->shuffle == 0 && i >= bj->num_rounds) {
				loadbar(trips, bj->num_trips);
				bj->end_of_shoe();
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
			if (win777 == 1) {
				certain_hands += triple7;
			}
			if (certain_hands < num_boxes) {
				house_value = house_hit(house_value, house_status,
				                        house_string);
			}
			// shuffle = 0 -> random shuffle
			// -> shuffle at the end of every round
			if (shuffle == 0) {
				random_shuffle(shoe, shoe + total_cards);
				used_cards = 0;
			}

			for (int k = 0; k < num_boxes; k++) {
				html_print(i, k, html);
			}
			if (generate_graphs && (total_i % interval == 0)) {
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
		} // end of shoe

		bj->end_of_trip(shoes_lost, shoes_won, shoes_tied, trip_loses,
		                trip_wins, trip_ties, trip_score, trip_high_win,
		                trip_high_lose);

	}		// end of trip

	html_files_close(highest_win, highest_lose, total_shoes_won,
	                 total_shoes_tied, total_shoes_lost);

	delete bj;
	cout << "\n";
	return 0;
}



