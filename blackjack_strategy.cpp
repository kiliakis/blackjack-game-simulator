#include "blackjack.h"

#include <vector>
#include <iostream>

using namespace std;


void BlackJack::player_play_hand(int P_1st_card, int P_2nd_card,
                                 string char_P_1st_card,
                                 string char_P_2nd_card,
                                 int H_1st_card,
                                 string char_H_1st_card,
                                 string P_string[4],
                                 int P_value[4], string P_status[4],
                                 int &bet, bool& BJ)
{
	// string char_P_1st_card = stringconversion(P_1st_card);
	// string char_P_2nd_card = stringconversion(P_2nd_card);
	// string char_H_1st_card = stringconversion(H_1st_card);
	P_string[0] = char_P_1st_card + char_P_2nd_card;
	string house_string = char_H_1st_card;
	P_value[0] = P_1st_card + P_2nd_card;

	P_status[0] = check_player_1st_2_cards(P_1st_card, P_2nd_card,
	                                       H_1st_card, P_value[0]);

	if (P_status[0] == "Stand") {
		stand(P_1st_card, P_2nd_card, P_value[0]);
		bet = wager;
	}

	if (P_status[0] == "Hit") {
		hit(P_1st_card, P_2nd_card,
		    char_P_1st_card, char_P_2nd_card,
		    H_1st_card, P_value[0], P_string[0]);
		bet = wager;
	}

	if (P_status[0] == "Double") {
		Double(P_1st_card, P_2nd_card, P_value[0], P_string[0]);
		bet = 2 * wager;
	}
	if (P_status[0] == "BJ") {
		bj(BJ, P_status[0]);
		bet = 3 * wager / 2;
	}
	if (P_status[0] == "Surrender") {
		surrender_f();
		bet = wager / 2;
	}

	if (P_status[0] == "Split") {
		split(P_1st_card, P_2nd_card, char_P_1st_card, char_P_2nd_card,
		      P_value, P_string, P_status, H_1st_card);
	}

}



string BlackJack::check_player_1st_2_cards(int P_1st_card, int P_2nd_card,
        int H_1st_card, int &P_value) {

// check for BlackJack
	string status;
	if ((P_1st_card == 1 && P_2nd_card == 10)
	        || (P_1st_card == 10 && P_2nd_card == 1)) {
		if (BJ_on)
			status = "BJ";
		else
			status = "21";
		P_value = 21;
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

	if (P_value == 11 && P_1st_card != 1 && P_2nd_card != 1 && H_1st_card != 1) {
		if (bjwinall == 0 || H_1st_card != 10)
			status = "Double";
		else
			status = "Hit";
		return status;
	}

	if ((P_value == 10) && (P_1st_card != 1 && P_2nd_card != 1)
	        && (H_1st_card != 1 && H_1st_card != 10)) {
		status = "Double";
		return status;
	}

	if ((P_value == 9) && (P_1st_card != 1 && P_2nd_card != 1)
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
		} else if (H_1st_card > 2 && H_1st_card < 7) {
			if (double_any == 1) {
				status = "Double";
			} else {
				status = "Stand";
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
	if (P_value > 16.5) {
		status = "Stand";
		return status;
	}

	if (P_value == 13 || P_value == 14 || P_value == 15
	        || P_value == 16) {
		if (H_1st_card > 1.5 && H_1st_card < 6.5) {
			status = "Stand";
			return status;
		}
	}

	if (P_value == 13 && (H_1st_card < 1.5 || H_1st_card > 6.5)) {
		status = "Hit";
		return status;
	}

	if ((P_value == 14 || P_value == 15)
	        && (H_1st_card == 1 || H_1st_card == 7 || H_1st_card == 8
	            || H_1st_card == 9)) {
		status = "Hit";
		return status;
	}

	if ((P_value == 14 || P_value == 15) && (H_1st_card == 10)) {
		if (surrender == 1) {
			status = "Surrender";
		} else {
			status = "Hit";
		}
		return status;
	}

	if ((P_value == 16)
	        && (H_1st_card == 1 || H_1st_card == 7 || H_1st_card == 8)) {
		status = "Hit";
		return status;
	}

	if ((P_value == 16) && (H_1st_card == 9 || H_1st_card == 10)) {
		if (surrender == 1) {
			status = "Surrender";
		} else {
			status = "Hit";
		}
		return status;
	}

	if (P_value == 12) {
		if (H_1st_card > 3.5 && H_1st_card < 6.5) {
			status = "Stand";
			return status;
		} else if (H_1st_card < 3.5 || H_1st_card > 6.5) {
			status = "Hit";
			return status;
		}
	}

	if (P_value == 11 && (P_1st_card != 1 && P_2nd_card != 1)
	        && (H_1st_card == 1 || H_1st_card == 10)) {
		status = "Hit";
		return status;
	}

	if (P_value == 10 && (P_1st_card != 1 && P_2nd_card != 1)
	        && (H_1st_card == 1 || H_1st_card == 10)) {
		status = "Hit";
		return status;
	}

	if (P_value == 9 && (P_1st_card != 1 && P_2nd_card != 1)
	        && (H_1st_card < 2.5 || H_1st_card > 6.5)) {
		status = "Hit";
		return status;
	}
	if (P_value == 8 && (P_1st_card != 1 && P_2nd_card != 1)) {
		status = "Hit";
		return status;
	}

	if (P_value < 7.5) {
		status = "Hit";
		return status;
	}

	cout << "This was not supposed to happen" << endl;
	status = "Null";
	return status;
}


int BlackJack::player_hit(int P_1st_card, int P_2nd_card,
                          string char_P_1st_card, string char_P_2nd_card,
                          int H_1st_card,
                          int & P_value, string & P_string)
{
	int p = 0;
	P_string = char_P_1st_card + char_P_2nd_card;

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

	P_string = P_string + char_P_next_card;
	P_value = P_value + P_next_card;


	if (player_A_exist == "Yes") {
		if (P_value < 11.5) {
			player_value_HS = P_value + 10;
			player_value_status = "soft";
		}
		if (P_value > 11.5 && P_value < 21.5) {
			player_value_HS = P_value;
			player_value_status = "hard";
		}
		if (P_value > 21.5) {
			player_value_HS = P_value;
			player_value_status = "hard";
		}
	}

	// player < 11.5 & not soft hand then hit
	if (P_value < 11.5 && player_value_status != "soft")
		goto player_next_card_loop;

	// 12 vs 2 & 3 hit
	if ((H_1st_card == 2 || H_1st_card == 3) && (P_value == 12))
		goto player_next_card_loop;

	//player 12, 13, 14, 15 & 16 vs house 7, 8, 9, 10 & A
	if (((H_1st_card > 6 && H_1st_card < 11) || H_1st_card == 1)
	        && ((P_value > 11.5 && P_value < 16.5)
	            || (player_value_HS > 11.5 && player_value_HS < 16.5)))
		goto player_next_card_loop;

	// soft A2, A3, A4, A5 & A6 hit
	if (player_value_status == "soft"
	        && ((P_value > 12.5 && P_value < 17.5)
	            || (player_value_HS > 12.5 && player_value_HS < 17.5)))
		goto player_next_card_loop;

	// soft A7 hit if House 1st card is 9, 10 & A
	if (player_value_status == "soft"
	        && (P_value == 18 || player_value_HS == 18)
	        && (H_1st_card == 9 || H_1st_card == 10 || H_1st_card == 1))
		goto player_next_card_loop;

	if (player_A_exist == "Yes")
		P_value = player_value_HS;

	return P_value;
}


int BlackJack::house_hit(int H_1st_card, int & house_value,
                         string & house_status,
                         string & house_string)
{

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


void BlackJack::player_split(int card01, int card02,
                            string char_card01,
                            string char_card02,
                            int & player_split_value,
                            string & player_split_string,
                            string & player_split_status,
                            int H_1st_card)
{
	player_split_value = card01 + card02;
	player_split_string = char_card01 + char_card02;
	if (card01 == 1) {
		player_split_status = "Split/Stand";
		player_split_value = player_split_value + 10;
		return;
	}

	split_on = false;
	player_split_status = check_player_1st_2_cards(card01, card02, H_1st_card,
	                      player_split_value);
	split_on = true;

	if (player_split_status == "Hit") {
		player_split_status = "Split/Hit";
		player_split_value = player_hit(card01, card02,
		                                char_card01, char_card02,
		                                H_1st_card,
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
	// return;
}


void BlackJack::stand(int P_1st_card, int P_2nd_card, int &P_value) {
	if (P_1st_card == 1 || P_2nd_card == 1) {
		P_value = P_value + 10;
	}

}


void BlackJack::hit(int P_1st_card, int P_2nd_card,
                    string char_P_1st_card, string char_P_2nd_card,
                    int H_1st_card, int &P_value,
                    string &P_string)
{

	P_value = player_hit(P_1st_card, P_2nd_card,
	                     char_P_1st_card,
	                     char_P_2nd_card,
	                     H_1st_card, P_value, P_string);

	busts += P_value > 21;

}


void BlackJack::Double(int P_1st_card, int P_2nd_card,
                       int &P_value, string &P_string)
{
	int next_card;
	string char_next_card;
	P_value = P_1st_card + P_2nd_card;
	draw_card(next_card, char_next_card);
	P_value += next_card;
	P_string += char_next_card;

	if ((P_1st_card == 1 || P_2nd_card == 1 || next_card == 1)
	        && P_value < 11.5)
		P_value = P_value + 10;

	busts += P_value > 21;

}


void BlackJack::bj(bool &BJ, string &P_status) {
	bjs++;
	BJ = true;
	P_status = "<font color=\"green\"><b>BJ</b></font>";
}

void BlackJack::surrender_f() {
	surr++;
}


void BlackJack::split(int P_1st_card, int P_2nd_card,
                      string char_P_1st_card, string char_P_2nd_card,
                      int P_value[4], string P_string[4],
                      string P_status[4], int H_1st_card)
{
	vector<int> P_split_cards(2 * num_splits, 0);
	vector<string> P_chars(2 * num_splits, "");
	P_split_cards[0] = P_1st_card;
	P_split_cards[2] = P_2nd_card;
	P_chars[0] = char_P_1st_card;
	P_chars[2] = char_P_2nd_card;
	draw_card(P_split_cards[1], P_chars[1]);
	draw_card(P_split_cards[3], P_chars[3]);
	int split_hands = 2;
	bool flag = true;
	// if we have ace split and resplitA == 0, we cannot resplit
	if (resplitA || P_chars[0] != "A") {
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
		             P_value[i / 2],
		             P_string[i / 2],
		             P_status[i / 2],
		             H_1st_card);
	}
	for (int i = split_hands; i < num_splits; ++i) P_value[i] = 0;

	flag = true;
	for (int i = 0; i < split_hands; ++i) {
		flag = flag && (P_value[i] > 21);
	}
	busts += flag;
}
