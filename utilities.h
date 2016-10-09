#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "blackjack.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <getopt.h>

using namespace std;


static inline void html_print(BlackJack *bj, int i, int box,
                              int P_value[4], string P_string[4],
                              string P_status[4], int house_value,
                              string house_string, string house_status,
                              int bet, bool BJ, vector<bool> &v)
{
	stringstream s, s2, s3;
	int res;
	bool first_hand = true;
	string print_result;
	if (box == 0) {
		s << house_value;
		s2 << house_string;
		s3 << i;
	}
	if (house_status == "BJ" && box == 0) {
		s.str("");
		s << "<font color=\"green\"><b>BJ</b></font>";
	}

	if (P_status[0].find("Split") == string::npos) {

		res = bj->resolve_winner(P_value[0], house_value, bet, box,
		                         bj->score, bj->hands, bj->loses, bj->wins,
		                         bj->ties, P_status[0], P_string[0],
		                         house_status, first_hand, print_result, BJ);
		first_hand = false;
		v.push_back(res > 0);

		if (P_status[0] == "Double") {
			bj->resolve_winner(P_value[0], house_value, bet, box,
			                   bj->dscore, bj->dhands, bj->dloses, bj->dwins,
			                   bj->dties, P_status[0], P_string[0],
			                   house_status, first_hand, print_result, BJ);
		}
		// if you lose original bet, house has BJ, you doubled down but lost
		// you have not splitted hands, you take half your bet back

		if (bj->bjwinall == 0 && house_status == "BJ"
		        && res < 0 && P_status[0] == "Double") {
			//score += wager;
			bj->score += bj->wager * (1 - 0.01 * bj->com_rate);
		}

		if (P_string[0] == "777") {
			P_string[0] = "<b>777</b>";
		}
		if (bj->detailed_out) {
			bj->html << "<tr> <td>" << s3.str() << "</td> <td>" << s2.str()
			         << "</td> <td>" << s.str() << "</td> <td>" << box + 1
			         << "</td> <td>" << P_string[0] << "</td> <td>"
			         << P_value[0] << "</td> <td>" << fixed << bj->score
			         << "</td> <td>" << P_status[0] << "</td> <td>"
			         << print_result << "</td></tr>\n";
		}
	} else if (P_status[0].find("Split") != string::npos) {
		// print 1st split hand
		int j = 0;
		while (j < bj->num_splits && P_value[j] > 0) {
			if (P_status[j].find("Double") != string::npos) {
				bet = 2 * bj->wager;
				bj->resolve_winner(P_value[j], house_value, bet, box,
				                   bj->dscore, bj->dhands, bj->dloses, bj->dwins,
				                   bj->dties, P_status[j], P_string[j],
				                   house_status, false, print_result, BJ);
			} else {
				bet = bj->wager;
			}

			res = bj->resolve_winner(P_value[j], house_value, bet, box,
			                         bj->score, bj->hands, bj->loses, bj->wins,
			                         bj->ties, P_status[j], P_string[j],
			                         house_status, first_hand, print_result, BJ);

			bj->v.push_back(res > 0);
			// if you lose only original bet, house has BJ, you lost the round,
			// and its not the first Split hand, you take back your money

			if (bj->bjwinall == 0 && house_status == "BJ" && res < 0 && first_hand &&
			        P_status[j].find("Double") != string::npos ) {
				//score += wager;
				bj->score += bj->wager * (1 - 0.01 * bj->com_rate);
			}

			bj->resolve_winner(P_value[j], house_value, bet, box, bj->sscore,
			                   bj->shands, bj->sloses, bj->swins, bj->sties,
			                   P_status[j], P_string[j], house_status,
			                   first_hand, print_result, BJ);
			first_hand = false;
			if (bj->detailed_out) {
				if (j == 0) {
					bj->html << "<tr> <td>" << s3.str() << "</td> <td>" << s2.str()
					         << "</td> <td>" << s.str() << "</td> <td>";
				} else {
					bj->html << "<tr> <td></td> <td></td> <td></td> <td>";
				}
				if (P_string[j] == "777") {
					P_string[j] = "<b>777</b>";
				}
				bj->html << box + 1 << "</td> <td>" << P_string[j] << "</td> <td>"
				         << P_value[j] << "</td> <td>" << fixed << bj->score
				         << "</td> <td>" << P_status[j] << "</td> <td>"
				         << print_result << "</td></tr>\n";
			}
			P_value[j] = 0;
			j++;
		}
	} else {
		cerr << "[Html Print] this was not supposed to happen\n";
	}

}


static inline void generate_WLstreak(ofstream &WL_streak, ofstream &WLdata, vector<bool> v) {
	//in v there is true-false depending on win/lose
	//must create a new vector with wl streak values
	// create data.xml and color data
	vector<int> streak_v;
	int streak = 1;
	int max = 1;
	if (v.empty()) return;
	bool res = v[0];
	for (unsigned int i = 1; i < v.size(); i++) {
		if (res == v[i]) {
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


static inline void html_files_init(BlackJack *bj) {
	remove("output/BJgame.html");
	remove("output/WLdata.xml");
	remove("output/linedata.xml");
	remove("output/overall.html");
	remove("output/shoe_overall.html");
	remove("output/trip_overall.html");
	remove("output/win_lose_streak.html");

	bj->html.open("output/BJgame.html");
	//html.precision(0);
	bj->xml.open("output/linedata.xml");
	if (bj->shuffle == 1) {
		bj->shoe_html.open("output/shoe_overall.html");
	}
	bj->trip_html.open("output/trip_overall.html");
	bj->overall_stats.open("output/overall.html");
	bj->WL_streak.open("output/win_lose_streak.html");
	bj->WLdata.open("output/WLdata.xml");

	bj->trip_html.precision(2);
	bj->trip_html << "<!DOCTYPE html><html><head><style>table, th, td"
	              << " { border: 1px solid black; border-collapse: collapse;"
	              << "text-align:center }\n caption {font-weight:bold;\n font-size:1.1em;\n}</style> </head><body>\n"
	              << "<table style=\"width:100%\">\n"
	              << "<caption>Trip Information</caption>"
	              << "<tr> <th>Trips</th> <th>Win</th> <th>Tie</th> "
	              << " <th>Lose</th> <th>Highest Win</th> <th>Highest Lose</th>"
	              << "<th>Balance</th> <th>Overall Balance</th> <th>Shoe Win</th><th>Shoe Tie</th> <th>Shoe Lose</th></tr>\n";

	if (bj->shuffle == 1) {
		bj->shoe_html.precision(2);
		bj->shoe_html << "<!DOCTYPE html><html><head><style>table, th, td"
		              << " { border: 1px solid black; border-collapse: collapse;"
		              << "text-align:center }\n caption {font-weight:bold;\n font-size:1.1em;\n}</style> </head><body>\n"
		              << "<table style=\"width:100%\">\n"
		              << "<caption>Shoe Information</caption>"
		              << "<tr> <th>Shoe</th> <th>Win</th> <th>Tie</th> "
		              << " <th>Lose</th> <th>Balance</th></tr>\n";
	}

	bj->xml.precision(2);
	bj->xml << "<?xml version=\"1.0\"?>\n <JSChart> <optionset> \n"
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

	bj->html.precision(2);
	bj->html << "<!DOCTYPE html><html><head><style>table, th, td"
	         << " { border: 1px solid black; border-collapse: collapse;"
	         << "text-align:center } </style> </head><body>\n"
	         << "<table style=\"width:100%\">\n"
	         << "<tr> <th>Hand</th><th>House Cards</th> <th>House Total</th> <th>Box</th> "
	         << " <th>Player Cards</th> <th>Player Total</th> <th>Player Balance</th>"
	         << " <th>Player Status</th> <th>Game Result</th></tr>\n";

	bj->overall_stats.precision(2);
	bj->overall_stats << "<!DOCTYPE html><html><head><style>table, th, td"
	                  << " { border: 1px solid black; border-collapse: collapse;"
	                  << "text-align:center } </style> <script type=\"text/javascript\""
	                  << " src=\"https://www.google.com/jsapi\"></script>"
	                  << "<script type=\"text/javascript\" src=\"jscharts.js\"> </script> </head><body>\n"
	                  << "<table style=\"width:100%\">\n";

}


static inline void html_files_close(BlackJack *bj, long double highest_win,
                                    long double highest_lose)
{
	bj->trip_html << "<tr><td>Total</td><td>" << bj->wins << "</td><td>" << bj->ties
	              << "</td><td>" << bj->loses << "</td><td>" << fixed << highest_win
	              << "</td><td>" << fixed << highest_lose << "</td> <td>" << bj->score
	              << "</td> <td>" << bj->score
	              << "</td><td>" << bj->total_shoes_won << "</td><td>" << bj->total_shoes_tied
	              << "</td><td>" << bj->total_shoes_lost << "</td> </tr>\n";
	bj->trip_html << "</table></body></html>\n";
	bj->trip_html.close();

	if (bj->shuffle == 1) {
		bj->shoe_html << "</table></body></html>\n";
		bj->shoe_html.close();
	}
	bj->html << "</table></body></html>\n";
	bj->html.close();
	bj->xml << "</dataset></JSChart>\n";
	bj->xml.close();

	bj->overall_stats << "<tr> <th>Total Hands</th><th>Total Wins</th><th>Total Ties</th>"
	                  << " <th>Total Defeats</th> <th>Total Balance</th> </tr>\n"
	                  << "<tr> <td>" << bj->hands << "</td><td>" << bj->wins << "</td><td>"
	                  << bj->ties << "</td><td>" << bj->loses << "</td><td>" << fixed << bj->score
	                  << "</td></tr>"; //<< "</table><br>\n";
	//overall_stats << "<table style=\"width:100%\">\n";
	bj->overall_stats << "<tr> <th>Double Hands</th><th>Double Wins</th><th>Double Ties</th>"
	                  << " <th>Double Defeats</th> <th>Double Balance</th> </tr>\n"
	                  << "<tr> <td>" << bj->dhands << "</td><td>" << bj->dwins << "</td><td>"
	                  << bj->dties << "</td><td>" << bj->dloses << "</td><td>" << fixed << bj->dscore
	                  << "</td></tr>";	//<<"</table><br>\n";
	//overall_stats << "<table style=\"width:100%\">\n";
	bj->overall_stats << "<tr> <th>Split Hands</th><th>Split Wins</th><th>Split Ties</th>"
	                  << " <th>Split Defeats</th> <th>Split Balance</th> </tr>\n"
	                  << "<tr> <td>" << bj->shands << "</td><td>" << bj->swins << "</td><td>"
	                  << bj->sties << "</td><td>" << bj->sloses << "</td><td>" << fixed << bj->sscore
	                  << "</td></tr></table>\n";
	bj->overall_stats << "<p><b>Total Player Busts:\t</b>" << bj->player_busts
	                  << " ( " << fixed << setprecision(2) << bj->player_busts * 100.0 / bj->hands << "% )"
	                  << "</p><p><b>Total House Busts:\t</b>" << bj->house_busts
	                  << " ( " << fixed << setprecision(2) << bj->house_busts * 100.0 / bj->total_i << "% )"
	                  << "</p><p><b>Total 777 wins:\t</b>" << bj->triple7_wins << "</p>"
	                  << "<p><b>Total Pushes with Player 21 vs House BJ:\t</b>"
	                  << bj->houseBJ_P21_pushes << "</p>";
	bj->overall_stats << "<div id=\"piechart\" style=\"width: 900px; height: 500px;\"></div>\n"
	                  << "<div id=\"linechart\" style=\"width: 900px; height: 500px;\"></div>\n"
	                  << "<script type=\"text/javascript\">\n"
	                  << " google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});\n"
	                  << "google.setOnLoadCallback(drawChart);\n"
	                  << "function drawChart() {\n"
	                  << "var data = google.visualization.arrayToDataTable([\n"
	                  << "['Task', 'Hours per Day']," << "['Wins'," << bj->wins << "     ],\n"
	                  << "['Defeats',  " << bj->loses << "],['Ties',  " << bj->ties << "]]);\n"
	                  << "var options = { title: 'Total results after " << bj->hands
	                  << " hands'};\n"
	                  << "var chart = new google.visualization.PieChart(document.getElementById('piechart'));\n"
	                  << "chart.draw(data, options);} \n</script>\n";
	if (bj->generate_graphs) {
		bj->overall_stats << "<script type=\"text/javascript\">\n var myChart = new JSChart('linechart', 'line');"
		                  << "myChart.setDataXML(\"linedata.xml\"); myChart.draw();\n</script>";
	}

	bj->overall_stats << "</body></html>\n";
	bj->overall_stats.close();
	if (bj->generate_graphs) {
		generate_WLstreak(bj->WL_streak, bj->WLdata, bj->v);
	}

}


static inline void parse_args(BlackJack *bj, int argc, char **argv)
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


	while (1) {
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
		c = getopt_long(argc, argv, "", long_options, &option_index);
		//printf("c is %d\n",c);
		if (c == -1) break;
		switch (c) {
		case 0:
			//printf("Option index is %d\n",option_index );
			switch (option_index) {
			case 0:
				printf( USAGE, argv[0]);
				exit(1);
				break;
			case 1:
				bj->shuffle = atoi(optarg);
				if (!(bj->shuffle == 0 || bj->shuffle == 1))
				{
					printf("shuffle argument must be 0 or 1\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 2:
				bj->num_splits = atoi(optarg);
				if (!(bj->num_splits == 3 || bj->num_splits == 4))
				{
					printf("splits argument must be 3 or 4\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 3:
				bj->resplitA = atoi(optarg);
				if (!(bj->resplitA == 0 || bj->resplitA == 1))
				{
					printf("resplitA argument must be 0 or 1\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 4:
				bj->soft17 = atoi(optarg);
				if (!(bj->soft17 == 0 || bj->soft17 == 1))
				{
					printf("soft17 argument must be 0 or 1\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 5:
				bj->push21 = atoi(optarg);
				if (!(bj->push21 == 0 || bj->push21 == 1))
				{
					printf("push21 argument must be 0 or 1\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 6:
				bj->win777 = atoi(optarg);
				if (!(bj->win777 == 0 || bj->win777 == 1))
				{
					printf("win777 argument must be 0 or 1\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 7:
				bj->surrender = atoi(optarg);
				if (!(bj->surrender == 0 || bj->surrender == 1))
				{
					printf("surrender argument must be 0 or 1\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 8:
				bj->double_any = atoi(optarg);
				if (!(bj->double_any == 0 || bj->double_any == 1))
				{
					printf("double_any argument must be 0 or 1\n");
					printf( USAGE, argv[0]);
					exit(1);
				}
				break;
			case 9:
				bj->bjwinall = atoi(optarg);
				if (!(bj->bjwinall == 0 || bj->bjwinall == 1))
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
	printf("Shuffle method (0-> random shuffle, 1-> manual shoe)\t\t\t= %d\n", bj->shuffle);
	printf("Number of split hands\t\t\t\t\t\t\t= %d\n", bj->num_splits);
	printf("Can resplit Aces (1-yes, 0-no)\t\t\t\t\t\t= %d\n", bj->resplitA);
	printf("House soft 17 hit or stand? (0->hit, 1->stand)\t\t\t\t= %d\n", bj->soft17);
	printf("Player's 21 push or lose to house BJ? (0-> lose, 1-> push)\t\t= %d\n", bj->push21);
	printf("Player's 777 win double or wait for house card? (0-> wait, 1-> win)\t= %d\n", bj->win777);
	printf("Can Player surrender? (0-> no, 1-> yes)\t\t\t\t\t= %d\n", bj->surrender);
	printf("Double down on 9,10,11 or any 2 cards? (0-> 9,10,11, 1-> any)\t\t= %d\n", bj->double_any);
	printf("House BJ win all bets or original bet only? (0-> original, 1-> all)\t= %d\n", bj->bjwinall);


	// 6 decks for random shuffle machine
	if (bj->shuffle == 0)
	{
		bj->num_decks = 6;
	}

	bj->total_cards = bj->num_decks * 52;
}


static inline void loadbar(unsigned int x, unsigned int n, unsigned int w = 50)
{
	using namespace std;
	if ( (x != n) && (x % (n / 100 + 1) != 0) ) return;

	float ratio  =  x / (float)n;
	unsigned int c =  ratio * w;

	cout << setw(3) << (int)(ratio * 100) << "% [";
	for (unsigned int x = 0; x < c; x++) cout << "=";
	for (unsigned int x = c; x < w; x++) cout << " ";
	cout << "]\r" << flush;
}

#endif // UTILITIES_H_