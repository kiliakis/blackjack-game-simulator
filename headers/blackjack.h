//blackjack.h
#ifndef __BLACKJACK_H_INCLUDED__ 
#define __BLACKJACK_H_INCLUDED__

#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <vector>
#include <cstdio>

#include "global.h"

using namespace std;

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
	if ((player_value == 11) && (P_1st_card != 1 && P_2nd_card != 1)
			&& (H_1st_card != 1 && H_1st_card != 10)) {
		status = "Double";
		return status;
	}

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

	int H = 0;
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
		house_next_card_loop: draw_card(H_next_card, char_H_next_card);
		if (H_1st_card == 1 || H_next_card == 1)
			house_A_exist = "Yes";

		H++;				// use to check whether 2 cards only

		house_string = house_string + char_H_next_card;
		house_value = house_value + H_next_card;

		if (((H_1st_card == 1 && H_next_card == 10)
				|| (H_1st_card == 10 && H_next_card == 1)) && H == 1) {
			house_status = "BJ";
			house_BJ = house_BJ + 1;
			house_value = 21;
			house_A_exist = "No";
			return house_value;
		}

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

		if (house_value < 16.5)
			goto house_next_card_loop;

		// house soft 17: hit if soft17==0, stand otherwise)
		if(soft17==0 && house_A_exist=="Yes" && house_value == 17)
			goto house_next_card_loop;

		if (house_value > 21)
			house_busts++;
		house_A_exist = "No";
		house_add_ten = "No";
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
		long int & ties, string & player_status, string & player_string) {
	hand++;
	if (player_string == "777" && win777 == 1) {
		score += 2 * bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (player_status == "Surrender") {
		score -= (bet * (1 - 0.01 * com_rate));
		loses++;
		print_result = "<font color=\"red\"><b>Loss</b></font>";
		return -1;
	} else if (player_value > 21.5) {
		score -= (bet * (1 - 0.01 * com_rate));
		loses++;
		print_result = "<font color=\"red\"><b>Loss</b></font>";
		return -1;
	} else if (house_value > 21.5) {
		score += bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (house_value > player_value) {
		score -= (bet * (1 - 0.01 * com_rate));
		loses++;
		print_result = "<font color=\"red\"><b>Loss</b></font>";
		return -1;
	} else if (house_value < player_value) {
		score += bet;
		wins++;
		print_result = "<font color=\"blue\"><b>Win</b></font>";
		return 1;
	} else if (house_status == "BJ" && BJ[box] == false && player_value < 21) {
		score -= (bet * (1 - 0.01 * com_rate));
		loses++;
		print_result = "<font color=\"red\"><b>Loss</b></font>";
		return -1;
	} else if (house_status == "BJ" && BJ[box] == false && player_value == 21) {
		// player 21, house BJ
		if(push21 == 0){ // lose
			score -= (bet * (1 - 0.01 * com_rate));
			loses++;
			print_result = "<font color=\"red\"><b>Loss</b></font>";
			return -1;
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


#endif 