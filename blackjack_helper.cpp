#include "blackjack.h"
#include "utilities.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

BlackJack::BlackJack(int argc, char *argv[]) {

	parse_args(this, argc, argv);
	html_files_init(this);

	shoe.resize(total_cards);
	for (int i = 0; i < total_cards; i++)
		shoe[i] = (i + 1) % 13 + 1;

}


void BlackJack::start_of_trip(int trips) {

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
			     << 1 << "</b> </td></tr>\n";
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

void BlackJack::end_of_trip(double &highest_win, double &highest_lose,
                            int trips)
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

void BlackJack::end_of_shoe(int &shoes, int &shoe_wins,
                            int &shoe_loses, int &shoe_ties,
                            double &shoe_score, int i)
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
		random_shuffle(shoe.begin(), shoe.end());
		used_cards = 0;
		if (detailed_out) {
			html << "\tStart of shoe number\t" << shoes << "</b> </td></tr>\n";
		}
		string burn_card_char; int burn_card_num;
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


void BlackJack::end_of_hand(int i, int interval, double prev_score,
                            int P_value[10][4], string P_status[10][4],
                            string P_string[10][4], int house_value,
                            string house_string, string house_status,
                            int *bet, bool *BJ)
{
	player_busts += busts;
	triple7_wins += triple7;

	// shuffle = 0 -> random shuffle
	// -> shuffle at the end of every round
	if (shuffle == 0) {
		random_shuffle(shoe.begin(), shoe.end());
		used_cards = 0;
	}

	for (int k = 0; k < num_boxes; k++) {
		html_print(this, i, k, P_value[k],
		           P_string[k], P_status[k],
		           house_value, house_string, house_status,
		           bet[k], BJ[k], v);
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



int BlackJack::resolve_winner(int P_value, int house_value, double bet,
                              int box, double & score,
                              int & hand, int & loses,
                              int & wins, int & ties,
                              string P_status, string P_string,
                              string house_status, bool first_hand,
                              string &print_result, bool BJ)
{
	hand++;
	if (P_string == "777" && win777 == 1) {
		score += 2 * bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (P_status == "Surrender") {
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
	} else if (P_value > 21.5) {
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
	} else if (house_value > P_value) {
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
	} else if (house_value < P_value) {
		score += bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (house_status == "BJ" && BJ == false && P_value < 21) {
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
	} else if (house_status == "BJ" && BJ == false && P_value == 21) {
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
	} else if (house_value == P_value) {
		ties++;
		print_result = "<b>Push</b>";
		return 0;
	} else {
		cout << "[Resolve winner] this was not supposed to happen\n";
		return 0;
	}

}

void BlackJack::burn_card() {
	int card; string char_card;
	draw_card(card, char_card);
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


string BlackJack::stringconversion(int cardvalue) {
	string card_array = "A23456789TJQK";
	return string() + card_array[cardvalue - 1];
}

int BlackJack::numericconversion(int cardvalue) {
	if (cardvalue >= 10)
		cardvalue = 10;
	return cardvalue;
}
