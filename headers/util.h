//util.h
#ifndef __UTIL_H_INCLUDED__ 
#define __UTIL_H_INCLUDED__

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
#include "global.h"

using namespace std;

static inline void loadbar(unsigned int x, unsigned int n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100+1) != 0) ) return;
 
    float ratio  =  x/(float)n;
    unsigned int   c      =  ratio * w;
 
    std::cout << setw(3) << (int)(ratio*100) << "% [";
    for (unsigned int x=0; x<c; x++) std::cout << "=";
    for (unsigned int x=c; x<w; x++) std::cout << " ";
    std::cout << "]\r" << flush;
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


void html_print(long int i, long int box, ofstream & html) {
	stringstream s, s2, s3;
	int res;
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
				player_string[box]);
		if (res > 0) {
			v.push_back(true);
		} else if (res < 0) {
			v.push_back(false);
		}
		if (player_status[box] == "Double") {
			resolve_winner(player_value[box], house_value, bet[box], box,
					dscore, dhands, dloses, dwins, dties, player_status[box],
					player_string[box]);
		}
		if (player_string[box] == "777") {
			player_string[box] = "<b>777</b>";
		}
		if(detailed_out){
			html << "<tr> <td>" << s3.str() << "</td> <td>" << s2.str()
				<< "</td> <td>" << s.str() << "</td> <td>" << box + 1
				<< "</td> <td>" << player_string[box] << "</td> <td>"
				<< player_value[box] << "</td> <td>" << fixed << score
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
						P_string[box][j]);
			} else {
				bet[box] = wager;
			}
			res = resolve_winner(P_value[box][j], house_value, bet[box], box,
					score, hands, loses, wins, ties, P_status[box][j],
					P_string[box][j]);
			if (res > 0) {
				v.push_back(true);
			} else if (res < 0) {
				v.push_back(false);
			}
			resolve_winner(P_value[box][j], house_value, bet[box], box, sscore,
					shands, sloses, swins, sties, P_status[box][j],
					P_string[box][j]);
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
	remove("../output/BJgame.html");
	remove("../tmp/WLdata.xml");
	remove("../tmp/linedata.xml");
	remove("../output/overall.html");
	remove("../output/shoe_overall.html");
	remove("../output/trip_overall.html");
	remove("../tmp/win_lose_streak.html");

	html.open("../output/BJgame.html");
	xml.open("../tmp/linedata.xml");
	if (shuffle == 1){
		shoe_html.open("../output/shoe_overall.html");
	}
	trip_html.open("../output/trip_overall.html");
	overall_stats.open("../output/overall.html");
	WL_streak.open("../output/win_lose_streak.html");
	WLdata.open("../tmp/WLdata.xml");

	trip_html.precision(0);
	trip_html << "<!DOCTYPE html><html><head><style>table, th, td"
			<< " { border: 1px solid black; border-collapse: collapse;"
			<< "text-align:center }\n caption {font-weight:bold;\n font-size:1.1em;\n}</style> </head><body>\n"
			<< "<table style=\"width:100%\">\n"
			<< "<caption>Trip Information</caption>"
			<< "<tr> <th>Trips</th> <th>Win</th> <th>Tie</th> "
			<< " <th>Lose</th> <th>Highest Win</th> <th>Highest Lose</th>"
			<< "<th>Balance</th> <th>Overall Balance</th> <th>Shoe Win</th><th>Shoe Tie</th> <th>Shoe Lose</th></tr>\n";

	if (shuffle == 1){
		shoe_html.precision(0);
		shoe_html << "<!DOCTYPE html><html><head><style>table, th, td"
				<< " { border: 1px solid black; border-collapse: collapse;"
				<< "text-align:center }\n caption {font-weight:bold;\n font-size:1.1em;\n}</style> </head><body>\n"
				<< "<table style=\"width:100%\">\n"
				<< "<caption>Shoe Information</caption>"
				<< "<tr> <th>Shoe</th> <th>Win</th> <th>Tie</th> "
				<< " <th>Lose</th> <th>Balance</th></tr>\n";
	}

	xml.precision(0);
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

	html.precision(0);
	html << "<!DOCTYPE html><html><head><style>table, th, td"
			<< " { border: 1px solid black; border-collapse: collapse;"
			<< "text-align:center } </style> </head><body>\n"
			<< "<table style=\"width:100%\">\n"
			<< "<tr> <th>Hand</th><th>House Cards</th> <th>House Total</th> <th>Box</th> "
			<< " <th>Player Cards</th> <th>Player Total</th> <th>Player Balance</th>"
			<< " <th>Player Status</th> <th>Game Result</th></tr>\n";

	overall_stats.precision(0);
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
		"  --help                  		Show this help message\n"
    
    shuffle = SHUFFLE;
    num_splits = NUM_SPLITS;
    resplitA = RESPLITA;
    soft17 = SOFT17;
    push21 = PUSH21;
    win777 = WIN777;
    double_any = DOUBLE_ANY;
    surrender = SURRENDER;
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


    // 6 decks for random shuffle machine
    if (shuffle == 0)
    {
    	num_decks = 6;
    }

    total_cards = num_decks * 52;    
}

#endif 
