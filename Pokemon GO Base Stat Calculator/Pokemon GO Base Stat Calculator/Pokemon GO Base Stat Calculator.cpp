//2.16.2026-2.18.2026
//This is a tool to convert a Pokemon's main series stats to its stats in Pokemon GO using the known formulas for each of the three and save the results to a file.

//Preprocessor Libraries
#include <iostream> //For input/output
#include <string> //For getline
#include <fstream> //For outfile creation
#include <cmath> //For the round operator
#include <algorithm> //For the max operator

using namespace std; //Declare standard namespace

const string INDENT = "\t\t\t"; //Size of indent in wait=3 tabs
const double L40CPMultiplier = 0.7903; //Multiplier applied to CP at level 40
const double L50CPMultiplier = 0.84029999; //Multiplier applied to CP at level 50

void showDisclaimers(); //Prints off a series of information at the beginning to tell the user how the calculator works
void readValues(int&, int&, int&, int&, int& , int&, bool&); //Function to read-in the Pokemon's six main series stats
void convertStats(int, int, int, int, int, int, bool, int&, int&, int&); //Function to convert the read-in main series stats to GO stats
void printResults(int, int, int); //Function to display the Pokemon's GO base stats after calculating them
void saveResults(int, int, int, int, int, int, int, int, int); //Function to save main series and calculated GO stats to an outfile
void wait(); //Pause alternative
void clearCin(); //Clears cin field for wait function or wipe cin memory

int main() {
	int mainSeriesHP = 0; //Initialize all major variables
	int mainSeriesAttack = 0;
	int mainSeriesDefense = 0;
	int mainSeriesSpecialAttack = 0;
	int mainSeriesSpecialDefense = 0;
	int mainSeriesSpeed = 0;

	bool isMega = false;

	int GOStamina = 0;
	int GOAttack = 0;
	int GODefense = 0;

	cout << "Main Series to Pokemon GO Base Stat Converter\n"; //Program title
	cout << "----------------------------------------------\n";
	cout << "This tool takes a Pokemon's six main series stats and uses them to determine its three Pokemon GO stats.\n\n";
	cout << "Stats for existing mons in GO can easily be found online, but this tool can be useful to determine the stats\nof mons currently in the main series but not GO, or theoretical/fake mons.\n\n";


	cout << "                                .&&&&&&&&&&&&&&&                                " << endl; //Poke Ball ASCII art made using https://manytools.org/hacker-tools/convert-images-to-ascii-art/go/
	cout << "                          &&&&&&&&&&&&&&&&&&&&&&&&&&&&&                         " << endl;
	cout << "                      &&&&&&&(######################&&&&&&@                     " << endl;
	cout << "                   @&&&&&###############################&&&&&&                  " << endl;
	cout << "                 &&&&&#####################################&&&&&                " << endl;
	cout << "               &&&&###########################################&&&&              " << endl;
	cout << "             &&&&##############################################@&&&@            " << endl;
	cout << "            &&&&#################################################&&&&           " << endl;
	cout << "           &&&&####################&&&&&&&&&&&####################&&&&          " << endl;
	cout << "          @&&&##################&&&&&&     &&&&&&##################&&&&         " << endl;
	cout << "          &&&%################&&&&            .&&&@################&&&&         " << endl;
	cout << "          &&&################&&&&               &&&&################&&&         " << endl;
	cout << "          &&&&&&&&&&&&&&&&&&&&&&&               &&&&&&&&&&&&&&&&&&&&&&&         " << endl;
	cout << "          &&&                &&&&               &&&&                &&&         " << endl;
	cout << "          &&&&                &&&&            (&&&@                &&&&         " << endl;
	cout << "          &&&&                  &&&&&&     &&&&&&                  &&&&         " << endl;
	cout << "           &&&&                    &&&&&&&&&&@                    &&&&          " << endl;
	cout << "            &&&&                                                 &&&&           " << endl;
	cout << "             &&&&&                                             &&&&&            " << endl;
	cout << "               &&&&                                           &&&&              " << endl;
	cout << "                 &&&&&                                     &&&&&                " << endl;
	cout << "                   &&&&&&                               &&&&&&                  " << endl;
	cout << "                      &&&&&&&                       &&&&&&&                     " << endl;
	cout << "                          &&&&&&&&&&&&&&&&&&&&&&&&&&&&&                         " << endl;
	cout << "                                 &&&&&&&&&&&&&&&                                " << endl << endl;

	showDisclaimers(); //Print information about the calculator and its logic before values are entered
	readValues(mainSeriesHP, mainSeriesAttack, mainSeriesDefense, mainSeriesSpecialAttack, mainSeriesSpecialDefense, mainSeriesSpeed, isMega); //Read-in the Pokemon's six main series stats
	convertStats(mainSeriesHP, mainSeriesAttack, mainSeriesDefense, mainSeriesSpecialAttack, mainSeriesSpecialDefense, mainSeriesSpeed, isMega, GOStamina, GOAttack, GODefense); //Calculate the Pokemon's GO Stats using the entered main series values
	printResults(GOStamina, GOAttack, GODefense); //Print the Pokemon's calculated GO base stats

	char saveAnswer;
	cout << "\n\nWould you like to save the generated stats to a file? (Y or N): ";
	cin >> saveAnswer;
	while ((saveAnswer != 'Y') && (saveAnswer != 'y') && (saveAnswer != 'N') && (saveAnswer != 'n')) { //Checks for invalid characters
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nSave answer must be Y or N. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> saveAnswer; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters

	if ((saveAnswer == 'Y') || (saveAnswer == 'y')) {
		saveResults(mainSeriesHP, mainSeriesAttack, mainSeriesDefense, mainSeriesSpecialAttack, mainSeriesSpecialDefense, mainSeriesSpeed, GOStamina, GOAttack, GODefense); //Save the main series and calculated GO stats to an outfile if the user chose to
	}

	wait();
	return 0;
}








void showDisclaimers() { //Function to print information about the calculator and its logic before values are entered
	cout << "A FEW DISCLAIMERS:\n\n";
	cout << "-A Pokemon's Stamina (HP) in GO is determined solely by its main series HP.\n\n";
	cout << "-A Pokemon's Attack stat in GO is determined mainly by the higher of its two main series Attack stats,\nand to a lesser extent by it's lower Attack stat and Speed.\n\n";
	cout << "-A Pokemon's Defense stat in GO is determined using both its main series Defenses, with the higher one\nhaving a slightly greater impact. Speed also factors in a bit.\n\n";
	cout << "-If a Pokemon's max CP at level 50 WOULD be over 4,000 just going off the main series stats,\nthen a 9% nerf is applied to each base stat. This does NOT apply to Mega Evolutions.\n\n";
	cout << "-This pre-nerf CP is NOT the same as the Pokemon's actual CP.\n\n";
	cout << "-There are a few exceptions where a Pokemon doesn't quite follow the regular base stat calculation rules,\nsuch as both Primals and Mega Rayquaza receiving a 3% nerf to each of their base stats.\nBut under normal circumstances, these calculations are valid.\n\n";
	cout << "-The formulas for the conversions can be found at https://www.dragonflycave.com/pokemon-go/stats/,\nor in the screenshot in the source files.\n\n";
	cout << "-Additional details on GO stat calculation can be found\nat https://pokemongohub.net/post/wiki/pokemon-go-calculates-stats-max-cp/.\n\n";
	cout << "-A Pokemon's main series base stats can easily be found on sites like Bulbapedia or Serebii.\n\n";
	cout << "-A main series stat can range from 1 to 255.\n\n";
	cout << "-If a Pokemon received a stat BUFF in gens six or seven, such as Alakazam's Special Defense, then the\nPOST-buff stats are used to determine the GO stats.\n\n";
	cout << "-If a Pokemon received a stat NERF in gens eight and beyond, such as Zacian's Attack, then the\nPRE-nerf stats are used to determine the GO stats.\n\n";
	cout << "-----------------------------------------------------------------------------------------------------------\n\n";
}








void readValues(int& mainSeriesHP, int& mainSeriesAttack, int& mainSeriesDefense, int& mainSeriesSpecialAttack, int& mainSeriesSpecialDefense, int& mainSeriesSpeed, bool& isMega) { //Function to read-in the Pokemon's six main series stats
	//Read in the Pokemon's main series HP
	cout << "Enter the Pokemon's base HP in the main series: ";
	cin >> mainSeriesHP;
	while (cin.fail() || mainSeriesHP < 1 || mainSeriesHP > 255) { //Checks for invalid characters or amounts
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nA Pokemon's base stat must be a number between 1 and 255. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> mainSeriesHP; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters


	//Read in the Pokemon's main series Attack
	cout << "\nEnter the Pokemon's base Attack in the main series: ";
	cin >> mainSeriesAttack;
	while (cin.fail() || mainSeriesAttack < 1 || mainSeriesAttack > 255) { //Checks for invalid characters or amounts
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nA Pokemon's base stat must be a number between 1 and 255. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> mainSeriesAttack; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters


	//Read in the Pokemon's main series Defense
	cout << "\nEnter the Pokemon's base Defense in the main series: ";
	cin >> mainSeriesDefense;
	while (cin.fail() || mainSeriesDefense < 1 || mainSeriesDefense > 255) { //Checks for invalid characters or amounts
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nA Pokemon's base stat must be a number between 1 and 255. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> mainSeriesDefense; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters


	//Read in the Pokemon's main series Special Attack
	cout << "\nEnter the Pokemon's base Special Attack in the main series: ";
	cin >> mainSeriesSpecialAttack;
	while (cin.fail() || mainSeriesSpecialAttack < 1 || mainSeriesSpecialAttack > 255) { //Checks for invalid characters or amounts
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nA Pokemon's base stat must be a number between 1 and 255. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> mainSeriesSpecialAttack; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters


	//Read in the Pokemon's main series Special Defense
	cout << "\nEnter the Pokemon's base Special Defense in the main series: ";
	cin >> mainSeriesSpecialDefense;
	while (cin.fail() || mainSeriesSpecialDefense < 1 || mainSeriesSpecialDefense > 255) { //Checks for invalid characters or amounts
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nA Pokemon's base stat must be a number between 1 and 255. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> mainSeriesSpecialDefense; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters


	//Read in the Pokemon's main series Speed
	cout << "\nEnter the Pokemon's base Speed in the main series: ";
	cin >> mainSeriesSpeed;
	while (cin.fail() || mainSeriesSpeed < 1 || mainSeriesSpeed > 255) { //Checks for invalid characters or amounts
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nA Pokemon's base stat must be a number between 1 and 255. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> mainSeriesSpeed; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters


	//Ask if the stats are for a Mega Evolution, which could alter the calculation results if true
	char isMegaTemp;
	cout << "\nAre the stats in question for a Mega Evolution?\n(The 9% stat nerf mentioned above for Pokemon with a max CP above 4,000 doesn't apply to Megas.)\nY or N: ";
	cin >> isMegaTemp;
	while ((isMegaTemp != 'Y') && (isMegaTemp != 'y') && (isMegaTemp != 'N') && (isMegaTemp != 'n')) { //Checks for invalid characters
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "\nMega status must be Y or N. Try again: "; //Tells you if entry is invalid, and repeats until valid entry is entered
		cin >> isMegaTemp; //Stores entered value
	}
	clearCin(); //Clear cin storage in case there's any trailing invalid characters

	if ((isMegaTemp == 'Y') || (isMegaTemp == 'y')) {
		isMega = true; //Set isMega to true if a Y/y was entered
	}

	if ((isMegaTemp == 'N') || (isMegaTemp == 'n')) {
		isMega = false; //Set isMega to false if an N/n was entered
	}
}








void convertStats(int mainSeriesHP, int mainSeriesAttack, int mainSeriesDefense, int mainSeriesSpecialAttack, int mainSeriesSpecialDefense, int mainSeriesSpeed, bool isMega, int& GOStamina, int& GOAttack, int& GODefense) { //Function to convert the read-in main series stats to GO stats
	double tempStamina = 1.75 * mainSeriesHP + 50.0; //Apply the formula to convert main series HP to GO Stamina (stores it in a double temporary value to avoid off-by-one errors when potentially applying the nerf)

	if (mainSeriesAttack >= mainSeriesSpecialAttack) { //If the mainSeriesAttack is higher (or both Attack stats are identical), it gets multiplied by .875
		GOAttack = static_cast<int>(round(round(2 * (.875 * mainSeriesAttack + .125 * mainSeriesSpecialAttack)) * (1 + (mainSeriesSpeed - 75) / 500.0))); //Apply the formula to convert main series stats to GO Attack
	}

	else { //If the mainSeriesSpecialAttack is higher, it gets multiplied by .875
		GOAttack = static_cast<int>(round(round(2 * (.875 * mainSeriesSpecialAttack + .125 * mainSeriesAttack)) * (1.0 + (mainSeriesSpeed - 75) / 500.0))); //Apply the formula to convert main series stats to GO Attack
	}

	if (mainSeriesDefense >= mainSeriesSpecialDefense) { //If the mainSeriesDefense is higher (or both Defense stats are identical), it gets multiplied by .625
		GODefense = static_cast<int>(round(round(2 * (.625 * mainSeriesDefense + .375 * mainSeriesSpecialDefense)) * (1.0 + (mainSeriesSpeed - 75) / 500.0))); //Apply the formula to convert main series stats to GO Defense
	}

	else { //If the mainSeriesSpecialDefense is higher, it gets multiplied by .625
		GODefense = static_cast<int>(round(round(2 * (.625 * mainSeriesSpecialDefense + .375 * mainSeriesDefense)) * (1.0 + (mainSeriesSpeed - 75) / 500.0))); //Apply the formula to convert main series stats to GO Defense
	}

	int CP = static_cast<int>(floor(max(10.0, (GOAttack * pow(GODefense, 0.5) * pow(tempStamina, 0.5) * pow(L50CPMultiplier, 2))/ 10))); //Calculate the Pokemon's CP at level 50 to determine whether or not to apply the 9% base stat nerf/tax if the resulting CP would be over 4,000

	if ((CP > 4000) && (isMega == false)) { //For Pokemon with over 4,000 max CP at level 50 (excluding Megas), stats are reduced by 9%
		tempStamina = tempStamina * 0.91f;
		GOAttack = static_cast<int>(round(GOAttack * 0.91));
		GODefense = static_cast<int>(round(GODefense * 0.91));
	}

	GOStamina = ((CP > 4000) && (isMega == false)) ? static_cast<int>(round(tempStamina)) : static_cast<int>(tempStamina); //Set the real GOStamina variable to the temporary value, rounding it if the nerf was applied and truncating it if not
}








void printResults(int GOStamina, int GOAttack, int GODefense) { //Function to display the Pokemon's GO base stats after calculating them
	cout << "\n\nIn Pokemon GO, this Pokemon would have...\n";
	cout << "A base Stamina (HP) of " << GOStamina << ".\n";
	cout << "A base Attack of " << GOAttack << ".\n";
	cout << "A base Defense of " << GODefense << ".\n";
}








void saveResults(int mainSeriesHP, int mainSeriesAttack, int mainSeriesDefense, int mainSeriesSpecialAttack, int mainSeriesSpecialDefense, int mainSeriesSpeed, int GOStamina, int GOAttack, int GODefense) { //Function to save main series and calculated GO stats to an outfile
	string filename; //Create variable to store filename
	cout << "\nPlease enter the name of the file you would\nlike to save the stats to (with the file extension): "; //Get a filename and extension from the user
	getline(cin, filename);

	ofstream outfile(filename); //Create outfile variable

	if (outfile.is_open()) { //Makes sure outfile was created and opened successfully
		outfile << "Main Series Stats:\n"; //Print all entered main series stats
		outfile << "HP = " << mainSeriesHP << endl;
		outfile << "Attack = " << mainSeriesAttack << endl;
		outfile << "Defense = " << mainSeriesDefense << endl;
		outfile << "Special Attack = " << mainSeriesSpecialAttack << endl;
		outfile << "Special Defense = " << mainSeriesSpecialDefense << endl;
		outfile << "Speed = " << mainSeriesSpeed << endl << endl;

		outfile << "Pokemon GO Stats:\n"; //Print all calculated Pokemon GO stats
		outfile << "Stamina/HP = " << GOStamina << endl;
		outfile << "Attack = " << GOAttack << endl;
		outfile << "Defense = " << GODefense << endl;

		cout << "\nStats saved successfully-Check the source files!\n";
	}

	else { //Error message if outfile fails to open, which should never happen
		cout << "Cannot open file!\n";
		wait();
		exit(0);
	}
}








void wait() //Pause alternative
{
	clearCin();
	char ch;
	cout << endl << INDENT << "Press the Enter key to continue ... ";
	cin.get(ch);
}








void clearCin()
{
	//The following if-statement checks to see how many characters are in cin's buffer
	//If the buffer has characters in it, the ignore method gets rid of them.
	//If cin is in the fail state, clear puts it back to the ready state.
	//If cin is not already in the fail state, it still doesn't hurt to call the clear function.
	if (cin.rdbuf()->in_avail() > 0) //If the buffer is empty skip clear and ignore
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear the input buffer
	}
}