/* Tony Nordstrom */
/* November 28 02024 */
/* Project name: Cellular Automata */
/* Project path: /cygdrive/c/dev/c++/cellular_automata */
/* To build: g++ -Wall -o "main" "main.cpp" */
/* To run: ./main.exe */
/* 
 Version 0.1
 - Fill grid with random characters
 - Print the grid
 - Calculate the number of neighbouring full cells for each cell
 - Print the neighbour grid
 Version 0.2
 - Update the grid with rules for each cell
 These are Conway's Game of Life rules:
 Any alive cell that is touching less than two alive neighbours dies.
 Any alive cell touching four or more alive neighbours dies.
 Any alive cell touching two or three alive neighbours does nothing.
 Any dead cell touching exactly three alive neighbours becomes alive.
 - Accepts commands
 - Runs in a loop
 - Prints version / about string
 Version 0.3
 - Command j reads rules from a json file, using json library from https://json.nlohmann.me/
 - Refactored out unchanged 3rd state in rules
 - Grid is now bool not char
 - Converts to char only on print
 - Command p prints out the rules
 Version 0.4
 - Command b runs built in self test
 Version 0.5
 - Command i sets grid to random
 - Command z sets rules to random
 - Command d sets rules to default
 - Command e edits rules manually
 - Command s saves rules to json
 */

#include <iostream>
#include <fstream>
#include <random>
#include "json.hpp"

/* Define to invoke a failure of the BIST */
#undef BIST_FAIL

using json = nlohmann::json;

const std::string version { "0.5" };
const char empty { ' ' };
const char full { '#' };
const int grid_length { 20 };
const int grid_width { 80 };
bool grid[grid_length][grid_width];
int neigh[grid_length][grid_width];
bool full_cell_rules[9];
bool empty_cell_rules[9];

const bool full_cell_rules_default[9] =
{ 0, 0, 1, 1, 0, 0, 0, 0, 0 };
const bool empty_cell_rules_default[9] =
{ 0, 0, 0, 1, 0, 0, 0, 0, 0 };

/* Calculate the number of neighbouring full cells */
int neighbours (int x, int y)
{
	int retval { 0 };
	bool on_edge_x { false };
	bool on_edge_y { false };
	bool on_edge_xx { false };
	bool on_edge_yy { false };
	
	/* Edge detection */
	if (x == 0)
		on_edge_x = true;
	if (y == 0)
		on_edge_y = true;
	if (x == grid_length)
		on_edge_xx = true;
	if (y == grid_width)
		on_edge_yy = true;
		
	if ((on_edge_x == false) && (on_edge_y == false))
	{
		if (grid[x-1][y-1] == true)
		{
			retval++;
		}
	}
	if (on_edge_y == false)
	{
		if (grid[x][y-1] == true)
		{
			retval++;
		}
	}
	if ((on_edge_xx == false) && (on_edge_y == false))
	{
		if (grid[x+1][y-1] == true)
		{
			retval++;
		}
	}
	if (on_edge_x == false)
	{
		if (grid[x-1][y] == true)
		{
			retval++;
		}
	}
	if (on_edge_xx == false)
	{
		if (grid[x+1][y] == true)
		{
			retval++;
		}
	}
	if ((on_edge_x == false) && (on_edge_yy == false))
	{
		if (grid[x-1][y+1] == true)
		{
			retval++;
		}
	}
	if (on_edge_yy == false)
	{
		if (grid[x][y+1] == true)
		{
			retval++;
		}
	}
	if ((on_edge_xx == false) && (on_edge_yy == false))
	{
		if (grid[x+1][y+1] == true)
		{
#ifndef BIST_FAIL // Screw up the neighbour count to cause BIST to fail
			retval++;
#endif
		}
	}
	
	return retval;
}

/* Apply the neighbour rules to the cell */
void apply_rules(int x, int y)
{
	if (grid[x][y] == true)
	{
		grid[x][y] = full_cell_rules[neigh[x][y]];
	}
	else if (grid[x][y] == false)
	{
		grid[x][y] = empty_cell_rules[neigh[x][y]];
	}
	
	return;
}

/*
 * Read rule set from json file
 * Do I have to close the file?
 */
void get_json(void)
{

	std::ifstream i("rules.json");
	auto j = json::parse(i);
	std::cout << j.dump(2) << '\n';

	int index { 0 };
	for (auto it = j.begin(); it != j.end(); ++it)
	{
		std::cout << it.key() << ": " << it.value() << '\n';
		if (index < 9)
		{
			empty_cell_rules[index] = it.value();
		}
		else
		{
			full_cell_rules[index-9] = it.value();
		}
		index++;
	}	
	return;
}

/*
 * Save rule set to json file 
 * Not sure how to prettify it with whitespace, future enhancement
 * Do I have to close the file? 
 */
void save_json(void)
{
	std::cout << "Writing rules to json file\n";
	
	std::ofstream o("output.json");
	json j = json::object({ {"fcrule0", full_cell_rules[0]},
							{"fcrule1", full_cell_rules[1]},
							{"fcrule2", full_cell_rules[2]},
							{"fcrule3", full_cell_rules[3]},
							{"fcrule4", full_cell_rules[4]},
							{"fcrule5", full_cell_rules[5]},
							{"fcrule6", full_cell_rules[6]},
							{"fcrule7", full_cell_rules[7]},
							{"fcrule8", full_cell_rules[8]},
							{"ecrule0", empty_cell_rules[0]},
							{"ecrule1", empty_cell_rules[1]},
							{"ecrule2", empty_cell_rules[2]},
							{"ecrule3", empty_cell_rules[3]},
							{"ecrule4", empty_cell_rules[4]},
							{"ecrule5", empty_cell_rules[5]},
							{"ecrule6", empty_cell_rules[6]},
							{"ecrule7", empty_cell_rules[7]},
							{"ecrule8", empty_cell_rules[8]} });
	o << j << '\n';
	
	return;
}

/* Print the current rules */
void print_rules(void)
{
	std::cout << "empty_cell_rules: ";
	for (int i { 0 }; i <= 8; i++)
	{
		std::cout << empty_cell_rules[i] << ", ";
	}
	std::cout << '\n';
	
	std::cout << "full_cell_rules: ";
	for (int i { 0 }; i <= 8; i++)
	{
		std::cout << full_cell_rules[i] << ", ";
	}
	std::cout << '\n';
	
	return;
}

/* Print the grid */
void print_grid(void)
{
	for (int i { 0 }; i < grid_length; i++)
	{
		for (int j { 0 }; j < grid_width; j++)
		{
			std::cout << (grid[i][j] ? full : empty);
		}
			
		std::cout << '\n';
	}
	
	return;
}

/* Run the Built In Self Test */
/* To invoke a failure, #define BIST_FAIL */
void run_bist(void)
{
	/* Y-coordinate of centres of each test pattern (X-coord always 1) */
	const int centres_empty [] = { 1, 4, 7, 10, 13, 16, 19, 22, 25 };
	const int centres_full [] = { 28, 31, 34, 37, 40, 43, 46, 49, 52 };

	int neighbours_empty [9];
	int neighbours_full [9];
	const int neighbours_expected [] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	
	bool pass { true };
	
	std::cout << "Running the Built In Self Test\n";
	
	/* Write the test pattern to the grid, 9 patterns x 2 states */
	/* Sorry this is so brute-force but I couldn't find another way with 
	 * C-style arrays maybe std::vector once I learn about it would be better */
	
	/* Pattern 0 */
	grid[0][0] = 0;
	grid[0][1] = 0;
	grid[0][2] = 0;
	grid[1][0] = 0;
	grid[1][1] = 0;
	grid[1][2] = 0;
	grid[2][0] = 0;
	grid[2][1] = 0;
	grid[2][2] = 0;
	
	/* Pattern 1 */
	grid[0][3] = 0;
	grid[0][4] = 0;
	grid[0][5] = 0;
	grid[1][3] = 0;
	grid[1][4] = 0;
	grid[1][5] = 0;
	grid[2][3] = 0;
	grid[2][4] = 0;
	grid[2][5] = 1;

	/* Pattern 2 */
	grid[0][6] = 0;
	grid[0][7] = 0;
	grid[0][8] = 0;
	grid[1][6] = 0;
	grid[1][7] = 0;
	grid[1][8] = 0;
	grid[2][6] = 0;
	grid[2][7] = 1;
	grid[2][8] = 1;
	
	/* Pattern 3 */
	grid[0][9]  = 0;
	grid[0][10] = 0;
	grid[0][11] = 0;
	grid[1][9]  = 0;
	grid[1][10] = 0;
	grid[1][11] = 0;
	grid[2][9]  = 1;
	grid[2][10] = 1;
	grid[2][11] = 1;

	/* Pattern 4 */
	grid[0][12] = 0;
	grid[0][13] = 0;
	grid[0][14] = 0;
	grid[1][12] = 0;
	grid[1][13] = 0;
	grid[1][14] = 1;
	grid[2][12] = 1;
	grid[2][13] = 1;
	grid[2][14] = 1;

	/* Pattern 5 */
	grid[0][15] = 0;
	grid[0][16] = 0;
	grid[0][17] = 0;
	grid[1][15] = 1;
	grid[1][16] = 0;
	grid[1][17] = 1;
	grid[2][15] = 1;
	grid[2][16] = 1;
	grid[2][17] = 1;

	/* Pattern 6 */
	grid[0][18] = 0;
	grid[0][19] = 0;
	grid[0][20] = 1;
	grid[1][18] = 1;
	grid[1][19] = 0;
	grid[1][20] = 1;
	grid[2][18] = 1;
	grid[2][19] = 1;
	grid[2][20] = 1;

	/* Pattern 7 */
	grid[0][21] = 0;
	grid[0][22] = 1;
	grid[0][23] = 1;
	grid[1][21] = 1;
	grid[1][22] = 0;
	grid[1][23] = 1;
	grid[2][21] = 1;
	grid[2][22] = 1;
	grid[2][23] = 1;

	/* Pattern 8 */
	grid[0][24] = 1;
	grid[0][25] = 1;
	grid[0][26] = 1;
	grid[1][24] = 1;
	grid[1][25] = 0;
	grid[1][26] = 1;
	grid[2][24] = 1;
	grid[2][25] = 1;
	grid[2][26] = 1;

	/* Pattern 0-1 */
	grid[0][27] = 0;
	grid[0][28] = 0;
	grid[0][29] = 0;
	grid[1][27] = 0;
	grid[1][28] = 1;
	grid[1][29] = 0;
	grid[2][27] = 0;
	grid[2][28] = 0;
	grid[2][29] = 0;
	
	/* Pattern 1-1 */
	grid[0][30] = 0;
	grid[0][31] = 0;
	grid[0][32] = 0;
	grid[1][30] = 0;
	grid[1][31] = 1;
	grid[1][32] = 0;
	grid[2][30] = 0;
	grid[2][31] = 0;
	grid[2][32] = 1;

	/* Pattern 2-1 */
	grid[0][33] = 0;
	grid[0][34] = 0;
	grid[0][35] = 0;
	grid[1][33] = 0;
	grid[1][34] = 1;
	grid[1][35] = 0;
	grid[2][33] = 0;
	grid[2][34] = 1;
	grid[2][35] = 1;
	
	/* Pattern 3-1 */
	grid[0][36] = 0;
	grid[0][37] = 0;
	grid[0][38] = 0;
	grid[1][36] = 0;
	grid[1][37] = 1;
	grid[1][38] = 0;
	grid[2][36] = 1;
	grid[2][37] = 1;
	grid[2][38] = 1;

	/* Pattern 4-1 */
	grid[0][39] = 0;
	grid[0][40] = 0;
	grid[0][41] = 0;
	grid[1][39] = 0;
	grid[1][40] = 1;
	grid[1][41] = 1;
	grid[2][39] = 1;
	grid[2][40] = 1;
	grid[2][41] = 1;

	/* Pattern 5-1 */
	grid[0][42] = 0;
	grid[0][43] = 0;
	grid[0][44] = 0;
	grid[1][42] = 1;
	grid[1][43] = 1;
	grid[1][44] = 1;
	grid[2][42] = 1;
	grid[2][43] = 1;
	grid[2][44] = 1;

	/* Pattern 6-1 */
	grid[0][45] = 0;
	grid[0][46] = 0;
	grid[0][47] = 1;
	grid[1][45] = 1;
	grid[1][46] = 1;
	grid[1][47] = 1;
	grid[2][45] = 1;
	grid[2][46] = 1;
	grid[2][47] = 1;

	/* Pattern 7-1 */
	grid[0][48] = 0;
	grid[0][49] = 1;
	grid[0][50] = 1;
	grid[1][48] = 1;
	grid[1][49] = 1;
	grid[1][50] = 1;
	grid[2][48] = 1;
	grid[2][49] = 1;
	grid[2][50] = 1;

	/* Pattern 8-1 */
	grid[0][51] = 1;
	grid[0][52] = 1;
	grid[0][53] = 1;
	grid[1][51] = 1;
	grid[1][52] = 1;
	grid[1][53] = 1;
	grid[2][51] = 1;
	grid[2][52] = 1;
	grid[2][53] = 1;
	
	std::cout << "Test pattern:\n";
	
	/* Print the grid */
	print_grid();
	
	/* For each test pattern, calculate neighbours */
	
	std::cout << "Calculate neighbours:\n";

	for (int i { 0 }; i < 9; i++)
	{
		neighbours_empty[i] = neighbours(1, centres_empty[i]);
		neighbours_full[i] = neighbours(1, centres_full[i]);
	}
	
	/* Print the test pattern neighbours, compare against expected */
	std::cout << "neighbours empty: ";
	for (int i { 0 }; i < 9; i++)
	{
		std::cout << neighbours_empty[i] << ", ";
		if (neighbours_empty[i] != neighbours_expected[i])
		{
			pass = false;
			std::cout << "fail: [" << i << "] ";
		}
	}
	std::cout << '\n';
	
	std::cout << "neighbours full: ";
	for (int i { 0 }; i < 9; i++)
	{
		std::cout << neighbours_full[i] << ", ";
		if (neighbours_full[i] != neighbours_expected[i])
		{
			pass = false;
			std::cout << "fail: [" << i << "] ";
		}
	}
	std::cout << '\n';
	
	std::cout << "Test result: " << (pass ? "pass" : "fail") << '\n';
	
	return;
}

/* Fill the grid randomly */
void rand_grid(void)
{
	/* Fill the grid with random bool values */
	for (int i { 0 }; i < grid_length; i++)
	{
		for (int j { 0 }; j < grid_width; j++)
		{
			grid[i][j] = rand() % 2 ? false : true;
		}
	}
}

/* Generate random rules */
void rand_rules(void)
{
	for (int i { 0 }; i < 9; i++)
	{
		empty_cell_rules[i] = rand() % 2 ? false : true;
		full_cell_rules[i] = rand() % 2 ? false : true;
	}
}

/* Set rules to default */
void default_rules(void)
{
	for (int i { 0 }; i < 9; i++)
	{
		empty_cell_rules[i] = empty_cell_rules_default[i];
		full_cell_rules[i] = full_cell_rules_default[i];
	}
}

/* Edit rules manually */
void edit_rules(void)
{
	std::cout << "Edit rules\n";
	
	std::cout << "Empty or full? (e/f): ";
	char response { 0 };
	std::cin >> response;
	
	if ((response == 'e') or (response == 'f'))
	{
		std::cout << "Which rule? (0-8): ";
		int ruleno { 0 };
		std::cin >> ruleno;
		
		if ((ruleno >= 0) and (ruleno < 9))
		{
			std::cout << "What state? (0/1): ";
			bool state { 0 };
			std::cin >> state;
			
			if ((state == 0) or (state == 1))
			{
				if (response == 'e')
				{
					empty_cell_rules[ruleno] = state;
				}
				else
				{
					full_cell_rules[ruleno] = state;
				}
			}
			else
			{
				std::cout << "Invalid state\n";
			}
		}
		else
		{
			std::cout << "Invalid ruleno\n";
		}
	}
	else
	{
		std::cout << "Invalid option\n";
	}
	
	return;
}

int main ( void )
{
	/* Randomize the grid */
	rand_grid();
	
	/* Use default rules initially */
	default_rules();
	
	while (true)
	{
		print_grid();
		
		/* Calculate the neighbours, store in int array */
		for (int i { 0 }; i < grid_length; i++)
		{
			for (int j { 0 }; j < grid_width; j++)
			{
				neigh[i][j] = neighbours(i, j);
			}
		}
	
		/* Print the neighbour info */
		for (int i { 0 }; i < grid_length; i++)
		{
			for (int j { 0 }; j < grid_width; j++)
			{
				std::cout << neigh[i][j];
			}
		
			std::cout << '\n';
		}
	
		/* Apply the rules to the grid */
		for (int i { 0 }; i < grid_length; i++)
		{
			for (int j { 0 }; j < grid_width; j++)
			{
				apply_rules(i, j);
			}
		}
		
		/* Get a command from the user */
		char command { 0 };
		std::cin >> command;

		switch (command)
		{
			case 'q':
				goto exit;
			case 'r':
				continue; /* Doesn't do anything that any other non-q command does */
			case 'h':
			case '?':
				std::cout << "q - quit, r - run, h/? - help, a - about, j - load rules from json, "
							<< "p - print rules, b - run BIST, i - random grid, z - random rules, "
							<< "d - set rules to default, e - edit rules, s - save rules to json\n";
				break;
			case 'a':
				std::cout << "Cellular automata version " << version << " by Tony Nordstrom\n";
				break;
			case 'j':
				std::cout << "Reload rules from json file\n";
				get_json();
				break;
			case 'p':
				print_rules();
				break;
			case 'b':
				run_bist();
				break;
			case 'i':
				rand_grid();
				break;
			case 'z':
				rand_rules();
				break;
			case 'd':
				default_rules();
				break;
			case 'e':
				edit_rules();
				break;
			case 's':
				save_json();
			default:
				break;
		}
	}
	
exit:

	return 0;
}

