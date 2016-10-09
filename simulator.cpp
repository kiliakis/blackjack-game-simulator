#include "blackjack.h"
#include "utilities.h"

using namespace std;


int main(int argc, char **argv) {

	//vector<bool> vector;
	//***uncomment this for a different random game
	//srand (time(NULL));
	auto bj = new BlackJack(argc, argv);

	int interval = bj->calc_interval();

	// int total_shoes_won, total_shoes_lost, total_shoes_tied,
	int shoe_wins, shoe_loses, shoe_ties, i;

	// total_shoes_won = total_shoes_lost = total_shoes_tied = 0;
	shoe_wins = shoe_loses = shoe_ties = i = 0;
	double highest_lose = 0, highest_win = 0;
	double shoe_score, prev_score;//, highest_win, highest_lose;
	shoe_score = prev_score = 0;// = highest_win = highest_lose = 0;

	for (int trips = 1; trips <= bj->num_trips; trips++) { //start of trip
		prev_score = bj->score;
		int shoes = 1;

		bj->start_of_trip(trips);

		for (;;) { // start of shoe, hand

			string P_cards_char[2][10];
			string P_string[10][4];
			string P_status[10][4];
			int P_cards[2][10];
			int P_value[10][4];
			int bet[10];
			bool BJ[10] = {false};
			int H_1st_card;
			string char_H_1st_card;
			string house_string, house_status;
			int house_value;

			// Burn a card face down at the start of each hand
			bj->burn_card();

			// check if it's time to end shoe
			if (bj->shuffle == 1 && bj->used_cards > 2 * bj->total_cards / 3) {
				loadbar(trips, bj->num_trips);
				bj->end_of_shoe(shoes, shoe_wins,
				                shoe_loses, shoe_ties,
				                shoe_score, i);
				if (shoes > bj->num_shoes) break;
			} else if (bj->shuffle == 0 && i >= bj->num_rounds) {
				loadbar(trips, bj->num_trips);
				bj->end_of_shoe(shoes, shoe_wins,
				                shoe_loses, shoe_ties,
				                shoe_score, i);
				i = 0;
				break;
			}

			// first card of every box
			for (int k = 0; k < bj->num_boxes; k++) {
				bj->draw_card(P_cards[0][k], P_cards_char[0][k]);
			}

			// dealer's first card
			bj->draw_card(H_1st_card, char_H_1st_card);

			// second card of every box
			for (int k = 0; k < bj->num_boxes; k++) {
#ifdef RANDOM_SPLIT
				P_cards[1][k] = P_cards[0][k];
				P_cards_char[1][k] = P_cards_char[0][k];
#else
				bj->draw_card(P_cards[1][k], P_cards_char[1][k]);
#endif
			}

// if you have defined P_FIXED_CARDS, here you can set player's 1st and 2nd cards
#ifdef P_FIXED_CARDS
			for (int k = 0; k < bj->num_boxes; k++) {
				P_cards[0][k] = 5;		//player's 1st card
				P_cards_char[0][k] = bj->stringconversion(P_cards[0][k]);
				P_cards[0][k] = bj->numericconversion(P_cards[0][k]);
				P_cards[1][k] = 6;//player's 2nd card
				P_cards_char[1][k] = bj->stringconversion(P_cards[1][k]);
				P_cards[1][k] = bj->numericconversion(P_cards[1][k]);
			}
#endif


// if you have defined H_FIXED_CARDS, here you can set house 1st card
#ifdef H_FIXED_CARDS
			H_1st_card = 10;		//House 1st card
			char_H_1st_card = bj->stringconversion(H_1st_card);
			H_1st_card = bj->numericconversion(H_1st_card);
#endif

			house_string = char_H_1st_card;
			house_value = H_1st_card;

			bj->bjs = 0;
			bj->triple7 = 0;
			bj->busts = 0;
			bj->surr = 0;

			for (int box = 0; box < bj->num_boxes; box++) {

				// must make it to make use of box to define where to draw cards
				string char_P_1st_card = P_cards_char[0][box];
				string char_P_2nd_card = P_cards_char[1][box];
				int P_1st_card = P_cards[0][box];
				int P_2nd_card = P_cards[1][box];
				// NOTE made player_* int instead of array
				P_string[box][0] = char_P_1st_card + char_P_2nd_card;

				// int player_value = P_1st_card + P_2nd_card;
				// bool BJ = false;
				bj->BJ_on = true;
				bj->split_on = true;

				bj->player_play_hand(P_1st_card, P_2nd_card, H_1st_card,
				                     P_string[box], P_value[box],
				                     P_status[box], bet[box], BJ[box]);


			} // end of box
			// bj->player_busts += bj->busts;

			int certain_hands = bj->busts + bj->surr
			                    + (bj->win777 == 1 ? bj->triple7 : 0);

			if (certain_hands < bj->num_boxes) {
				house_value = bj->house_hit(H_1st_card, house_value,
				                            house_status, house_string);
			}

			bj->end_of_hand(i, interval, prev_score,
			                P_value, P_status,
			                P_string, house_value, house_string,
			                house_status, bet, BJ);

			i++;
		} // end of shoe - round

		bj->end_of_trip(highest_win, highest_lose, trips);

	}		// end of trip

	html_files_close(bj, highest_win, highest_lose);

	delete bj;
	cout << "\n";
	return 0;
}



