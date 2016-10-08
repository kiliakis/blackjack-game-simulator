#include "blackjack.h"
#include "utilities.h"
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

using namespace std;

BlackJack::BlackJack(int argc, char *argv[]) {

	parse_args(argc, argv, this);
	html_files_init();

	shoe.resize(total_cards);
	for (int i = 0; i < total_cards; i++)
		shoe[i] = (i + 1) % 13 + 1;

}

void BlackJack::burn_card() {
	int card; string char_card;
	draw_card(card, char_card);
}

void BlackJack::start_of_trip() {

	trip_wins = trip_loses = trip_ties = 0;
	shoes_won = shoes_lost = shoes_tied = 0;
	trip_score = trip_high_win = trip_high_lose = 0.0;
	used_cards = 0;
	random_shuffle(shoe.begin(), shoe.end());
	int burn_card_num; string burn_card_char;
	draw_card(burn_card_num, burn_card_char);

	if (detailed_out) {
		html << "<tr bgcolor=\"#439768\"> <td height = \"45\" colspan=9 > <b> \tTrip number\t"
		     << trips << "</b> </td></tr>\n";
		if (shuffle == 1) {
			html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > <b> \tStart of shoe number\t"
			     << shoes << "</b> </td></tr>\n";
		}
		html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Face up burnt card: "
		     << burn_card_char << " </td></tr>";
	}
	if (burn_card_num == 7) {
		for (int i = 0; i < 7; i++)
			draw_card(burn_card_num, burn_card_char);
		if (detailed_out) {
			html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 7 more cards </td></tr>";
		}
	} else if (burn_card_num == 10) {
		for (int i = 0; i < 10; i++)
			draw_card(burn_card_num, burn_card_char);
		if (detailed_out) {
			html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 10 more cards </td></tr>";
		}
	}
	if (shuffle == 1) {
		shoe_html << "<tr bgcolor=\"#FFA319\"> <td height = \"45\" colspan=5 > <b> \tTrip number\t"
		          << trips << "</b> </td></tr>\n";
	}
}

void BlackJack::end_of_trip(double &highest_win, double &highest_lose)
{
	if (shuffle == 1) {
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
	} else if (shuffle == 0) {
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
}

void BlackJack::end_of_shoe(int &shoes, long &shoe_wins,
                            long &shoe_loses, long &shoe_ties,
                            double &shoe_score)
{

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
			return;
		}
		if (detailed_out) {
			html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > <b> End of shoe as "
			     << used_cards << " cards have been played. <br>";
		}
		random_shuffle(shoe, shoe + total_cards);
		used_cards = 0;
		if (detailed_out) {
			html << "\tStart of shoe number\t" << shoes << "</b> </td></tr>\n";
		}
		draw_card(burn_card_num, burn_card_char);
		if (detailed_out) {
			html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Face up burnt card: "
			     << burn_card_char << " </td></tr>";
		}
		if (burn_card_num == 7) {
			for (int i = 0; i < 7; i++)
				draw_card(burn_card_num, burn_card_char);
			if (detailed_out) {
				html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 7 more cards </td></tr>";
			}
		} else if (burn_card_num == 10) {
			for (int i = 0; i < 10; i++)
				draw_card(burn_card_num, burn_card_char);
			if (detailed_out) {
				html << "<tr bgcolor=\"#FFA319\"> <td colspan=9 > Burnt 10 more cards </td></tr>";
			}
		}
	} else if (shuffle == 0 && i >= num_rounds) {
		trip_wins += wins - shoe_wins;
		trip_loses += loses - shoe_loses;
		trip_ties += ties - shoe_ties;
		trip_score += score - shoe_score;
		shoe_wins = wins;
		shoe_loses = loses;
		shoe_ties = ties;
		shoe_score = score;
	}
}


void BlackJack::player_play_hand(int P_1st_card, int P_2nd_card, int H_1st_card,
                                 string player_string[4], int player_value[4],
                                 string player_status[4], int &bet)
{
	string char_P_1st_card = stringconversion(P_1st_card);
	string char_P_2nd_card = stringconversion(P_2nd_card);
	string char_H_1st_card = stringconversion(H_1st_card);
	string player_string = char_P_1st_card + char_P_2nd_card;
	string house_string = char_H_1st_card;
	int player_value = P_1st_card + P_2nd_card;
	int house_value = H_1st_card;

	string player_status = check_player_1st_2_cards(P_1st_card, P_2nd_card,
	                       H_1st_card, player_value);

	if (player_status == "Stand") {
		stand();
		bet = wager;
	}

	if (player_status == "Hit") {
		hit();
		bet = wager;
	}

	if (player_status == "Double") {
		Double();
		bet = 2 * wager;
	}
	if (player_status == "BJ") {
		bj();
		bet = 3 * wager / 2;
	}
	if (player_status == "Surrender") {
		surrender_f();
		bet = wager / 2;
	}

	if (player_status == "Split") {
		split();
	}

}

void BlackJack::end_of_hand(int i, int interval, double prev_score) {
	// shuffle = 0 -> random shuffle
	// -> shuffle at the end of every round
	if (shuffle == 0) {
		random_shuffle(shoe, shoe + total_cards);
		used_cards = 0;
	}

	for (int k = 0; k < num_boxes; k++) {
		html_print(this, i, k, html,
		           player_value, house_status,
		           player_status, player_string,
		           wager, com_rate, bet,
		           P_status, P_string, P_value, v);
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
	total_i++;
}

int BlackJack::calc_interval() {
	if (shuffle == 0) {
		return max(1, num_trips * num_rounds / 100);
	} else {
		return max(num_shoes * num_trips *
		           (total_cards / (num_boxes * 4 + 3)) / 100, 1);
	}
}

void BlackJack::draw_card(int & card, string & ch)
{
	card = shoe[used_cards % total_cards];
	ch = stringconversion(card);
	card = numericconversion(card);
	used_cards++;
}

void BlackJack::draw_card(int & card, char & ch)
{
	card = shoe[used_cards % total_cards];
	ch = stringconversion(card);
	card = numericconversion(card);
	used_cards++;
}

char BlackJack::stringconversion(int cardvalue) {
	const char card_array[] = "A23456789TJQK";
	assert(cardvalue >= 1 && cardvalue <= 13);
	return card_array[cardvalue - 1];
}

int numericconversion(int cardvalue) {
	if (cardvalue >= 10)
		cardvalue = 10;
	return cardvalue;
}

string BlackJack::check_player_1st_2_cards(int P_1st_card, int P_2nd_card,
        int H_1st_card, int &player_value) {

// check for BlackJack
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
		if (win777 == 0 && surrender == 1)
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
		if (split_on && bjwinall == 0)
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
		if (surrender == 1) {
			status = "Surrender";
		} else {
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

	if (player_value == 11 && P_1st_card != 1 && P_2nd_card != 1 && H_1st_card != 1) {
		if (bjwinall == 0 || H_1st_card != 10)
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
			if (double_any == 1) {
				status = "Double";
			} else {
				status = "Hit";
			}
			return status;
		}
	}

	if ((P_1st_card == 1 && (P_2nd_card == 2 || P_2nd_card == 3))
	        || ((P_1st_card == 2 || P_1st_card == 3) && P_2nd_card == 1)) {
		if (H_1st_card == 5 || H_1st_card == 6) {
			if (double_any == 1) {
				status = "Double";
			} else {
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
			if (double_any == 1) {
				status = "Double";
			} else {
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
			if (double_any == 1) {
				status = "Double";
			} else {
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
		if (surrender == 1) {
			status = "Surrender";
		} else {
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
		if (surrender == 1) {
			status = "Surrender";
		} else {
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

int BlackJacl::player_hit(int P_1st_card, int P_2nd_card, int H_1st_card,
                          int & player_value, string & player_string)
{
	int p = 0;
	player_string = "";
	player_string += stringconversion(P_1st_card);
	player_string += stringconversion(P_2nd_card);

	string player_A_exist = "No";
	string player_value_status = "";
	int player_value_HS = 0;

player_next_card_loop:

	string char_P_next_card;
	int P_next_card;
	p = p + 1;
	draw_card(P_next_card, char_P_next_card);

	// 777 rule
	if (P_1st_card == 7 && P_2nd_card == 7 && P_next_card == 7 && p == 1)
		triple7++;

	if (P_1st_card == 1 || P_2nd_card == 1 || P_next_card == 1)
		player_A_exist = "Yes";

	player_string = player_string + char_P_next_card;
	player_value = player_value + P_next_card;


	if (player_A_exist == "Yes") {
		if (player_value < 11.5) {
			player_value_HS = player_value;
			player_value_status = "soft";
		}
		if (player_value > 11.5 && player_value < 21.5) {
			player_value_HS = player_value + 10;
			player_value_status = "hard";
		}
		if (player_value > 21.5) {
			player_value_HS = player_value + 10;
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
	if (((H_1st_card >  && H_1st_card < 11) || H_1st_card == 1)
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

	return player_value;
}


int BlackJack::house_hit(int & house_value, string & house_status,
                         string & house_string) {

	string house_A_exist = "No";
	string hand_type = "Hard";

	house_status = "No_BJ";
	int next_card;
	string char_next_card;

	if (bjs == num_boxes) {
		if (H_1st_card == 1 || H_1st_card == 10) {
			draw_card(next_card, char_next_card);
			house_string = house_string + char_next_card;
			house_value = house_value + next_card;
			if (H_1st_card == 1 || next_card == 1) {
				house_value = house_value + 10;
			}
			if (house_value == 21)
				house_status = "BJ";
		}
		return house_value;
	} else {
		if (H_1st_card == 1)
			house_A_exist = "Yes";

		draw_card(next_card, char_next_card);

		if ((H_1st_card == 1 && next_card == 10)
		        || (H_1st_card == 10 && next_card == 1)) {
			house_status = "BJ";
			house_BJ = house_BJ + 1;
			house_value = 21;
			house_string = house_string + char_next_card;
			return house_value;
		}

		string house_add_ten = "No";

house_next_card_loop:

		house_string = house_string + char_next_card;
		house_value = house_value + next_card;

		if (next_card == 1)
			house_A_exist = "Yes";

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

		draw_card(next_card, char_next_card);

		// house soft 17: hit if soft17==0, stand otherwise)
		if (house_value < 16.5 || (soft17 == 0 && house_A_exist == "Yes" &&
		                           house_add_ten == "Yes" && house_value == 17))
			goto house_next_card_loop;

		if (house_value > 21)
			house_busts++;

		house_status = "No_BJ";
		return house_value;
	}
}

int BlackJack::player_split(int card01, int card02,
                            string char_card01,
                            string char_card02,
                            string & player_split_string,
                            string & player_split_status)
{
	int player_split_value = card01 + card02;
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
		                                player_split_value,
		                                player_split_string);

	}

	if (player_split_status == "Stand") {
		player_split_status = "Split/Stand";
		if (card01 == 1 || card02 == 1)
			player_split_value = player_split_value + 10;
	}

	if (player_split_status == "Double") {
		player_split_status = "Split/Double";
		player_split_value = card01 + card02;
		int next_card;
		string char_next_card;
		draw_card(next_card, char_next_card);
		player_split_value = player_split_value + next_card;
		if (card01 == 1 || card02 == 1 || next_card == 1) {
			player_split_value += player_split_value + 10 > 21 ? 0 : 10;
		}
		player_split_string = player_split_string + char_next_card;
	}
	return player_split_value;
}

// NOTE call it with player_value[box]
void BlackJack::stand(int P_1st_card, int P_2nd_card, int &player_value) {
	if (P_1st_card == 1 || P_2nd_card == 1) {
		player_value = player_value + 10;
	}

}

// TODO fix the calls to this function
void BlackJack::hit(int P_1st_card, int P_2nd_card,
                    int H_1st_card, int &player_value,
                    string &player_string)
{

	player_value = player_hit(P_1st_card, P_2nd_card, H_1st_card,
	                          player_value, player_string);

	busts += player_value > 21;

}

// TODO fix the calls to this function

void BlackJack::Double(int P_1st_card, int P_2nd_card,
                       int &player_value, string &player_string)
{
	int next_card;
	string char_next_card;
	player_value = P_1st_card + P_2nd_card;
	draw_card(next_card, char_next_card);
	player_value += next_card;
	player_string += char_next_card;

	if ((P_1st_card == 1 || P_2nd_card == 1 || next_card == 1)
	        && player_value < 11.5)
		player_value = player_value + 10;

	busts += player_value > 21;

}

// TODO fix the calls to this function
void BlackJack::bj(bool *BJ, string &player_status) {
	bjs++;
	BJ = true;
	player_status = "<font color=\"green\"><b>BJ</b></font>";
}

void BlackJack::surrender_f() {
	surr++;
}

// TODO fix the calls to this function
// P_value[box] must be passed
void BlackJack::split(int P_1st_card, int P_2nd_card, string & char_P_1st_card,
                      string & char_P_2nd_card, int num_splits,
                      int *P_value, string *P_string, string *P_status)
{
	vector<int> P_split_cards(2 * num_splits, 0);
	vector<string> P_chars(2 * num_splits, "");
	// memset(P_split_cards, 0, sizeof(P_split_cards));
	// memset(P_chars, 0, sizeof(P_chars));
	P_split_cards[0] = P_1st_card;
	P_split_cards[2] = P_2nd_card;
	P_chars[0] = char_P_1st_card;
	P_chars[2] = char_P_2nd_card;
	draw_card(P_split_cards[1], P_chars[1]);
	draw_card(P_split_cards[3], P_chars[3]);
	int split_hands = 2;
	bool flag = true;
	// if we have ace split and resplitA == 0, we cannot resplit
	if (resplitA || P_chars[0] != 'A') {
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
		player_split(P_split_cards[i], P_split_cards[i + 1],
		             P_chars[i], P_chars[i + 1],
		             P_value[i / 2], P_string[i / 2],
		             P_status[i / 2]);
	}
	for (int i = split_hands; i < num_splits; ++i) P_value[i] = 0;

	flag = true;
	for (int i = 0; i < split_hands; ++i) {
		flag = flag && (P_value[i] > 21);
	}
	busts += flag;
}

// TODO house_status added
int BlackJack::resolve_winner(int player_value, int house_value, double bet,
                              long int box, double & score,
                              long int & hand, long int & loses,
                              long int & wins, long int & ties,
                              string player_status, string player_string,
                              string house_status, bool first_hand,
                              string &print_result, bool BJ)
{
	hand++;
	if (player_string == "777" && win777 == 1) {
		score += 2 * bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (player_status == "Surrender") {
		if (bjwinall == 0 && first_hand && house_status == "BJ") {
			ties++;
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		} else {
			score -= (bet * (1 - 0.01 * com_rate));
			loses++;
			print_result = "<font color=\"red\"><b>Loss</b></font>";
			return -1;
		}
	} else if (player_value > 21.5) {
		if (bjwinall == 0 && !first_hand && house_status == "BJ") {
			ties++;
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		} else {
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
		if (bjwinall == 0 && !first_hand && house_status == "BJ") {
			ties++;
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		} else {
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
	} else if (house_status == "BJ" && BJ == false && player_value < 21) {
		if (bjwinall == 0 && !first_hand && house_status == "BJ") {
			ties++;
			print_result = "<b>Push</b>";
			houseBJ_P21_pushes++;
			return 0;
		} else {
			score -= (bet * (1 - 0.01 * com_rate));
			loses++;
			print_result = "<font color=\"red\"><b>Loss</b></font>";
			return -1;
		}
	} else if (house_status == "BJ" && BJ == false && player_value == 21) {
		// player 21, house BJ
		if (push21 == 0) { // lose
			if (bjwinall == 0 && !first_hand && house_status == "BJ") {
				ties++;
				print_result = "<b>Push</b>";
				houseBJ_P21_pushes++;
				return 0;
			} else {
				score -= (bet * (1 - 0.01 * com_rate));
				loses++;
				print_result = "<font color=\"red\"><b>Loss</b></font>";
				return -1;
			}
		} else {	//push
			ties++;
			houseBJ_P21_pushes++;
			print_result = "<b>Push</b>";
			return 0;
		}
	} else if (house_status != "BJ" && BJ == true) {
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


