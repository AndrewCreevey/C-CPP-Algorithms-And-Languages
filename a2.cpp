#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

//This Table structure is used to keep my Instruction Table variables
struct Table {
	string state;
	string symbol;
	string newState;
	string newSymbol;
	int movement;
};

//This structure is used to store all my variables
struct Data {
	set <string> States;
	set <string> Alphabet;
	vector <string> Input;
	string InitState;
	set <string> FinalState;
	vector <Table> Instructions;
};

/* --------------------------------------------------------------------------------- */
//Function declarations:

void Simulation (Data &d);
set <string> Make_Set (string str);
void Print_Set (set <string> s);
Table Make_Table (string str, Data d);
void Print_Table (Table x, bool first);
Data GetInput();
bool SkipComments (string line);

/* --------------------------------------------------------------------------------- */
//Main:

int main () {
	
	Data input;
	input = GetInput(); //fill input with input!
	
	//Print out content
	cout << "       States: ";  	Print_Set(input.States);
	cout << "     Alphabet: "; 	Print_Set(input.Alphabet);
	
	cout << "      Input's: ";
	int len = input.Input.size();
	for (int i = 0; i < len; i++) cout << input.Input[i] << " ";
	
	cout << endl << "Initial State: " << input.InitState << endl;
	cout << "  Final State: "; 	Print_Set(input.FinalState);
	cout << " Instructions: ";
	len = input.Instructions.size();
	Print_Table (input.Instructions[0], true);
	for (int i = 1; i < len; i++) Print_Table(input.Instructions[i], false);
	
	//Simulation the machine
	cout << endl << "Simulating Machine:" << endl;
	Simulation(input);
}

/* --------------------------------------------------------------------------------- */
//Functions: 

void Simulation (Data &d) {
	string currentState = d.InitState;
	string currentSymbol = "";
	int position = 0, len = 0, tableNum = 0;
	set <string>::iterator it1, it2;
	bool halt = false;
	
	//If the input is empty, this ensures there is at least one input
	len = d.Input.size();
	if (position >= len) {
		d.Input.push_back("^");
		len+=1;
	}
	
	while (halt == false) {
		/* ----- Print out line before we compute ----- */
		cout << currentState << " : ";
		for (int i = 0; i < len; i++) {
			cout << d.Input[i];
			if (position == i) cout << "<";
			else cout << " ";
			if (i+1 == len) cout << endl;
		}
		
		if (position < 0) {
			halt = true;
		}
		
		bool found = false;
		currentSymbol = d.Input[position];
		tableNum = d.Instructions.size();
		
		for (int i = 0; i < tableNum; i++) {
			//if the state and symbol match a set of instructions
			if (currentState == d.Instructions[i].state && currentSymbol == d.Instructions[i].symbol) {
				found = true;
				//set the current state to new state, and change the current symbol to the new symbol
				currentState = d.Instructions[i].newState;
				d.Input[position] = d.Instructions[i].newSymbol;
				position += d.Instructions[i].movement; //Edit the position
				//If the position exceeds the initial length of input, add a ^ to the end
				len = d.Input.size();
				if (position >= len) {
					d.Input.push_back("^");
					len+=1;
				}
				break; //if we find the matching instructions, then we break out of the loop after we are done
			}
		}
		//If we did not find any matching instructions, throw an error
		if (found == false) {
			cout << "Error: No instructions matching " << currentState << " " << currentSymbol << " were found. Exiting..." << endl;
			exit(0);
		}
		//If we are at the halt stage, then set halt to true. This will exit this loop.
		it1 = d.FinalState.find(currentState);
		if (position < 0 || it1 != d.FinalState.end()) {
			halt = true;
		}
	}
	if (halt == true) {
		//Print out the current line
		cout << currentState << " : ";
		len = d.Input.size();
		for (int i = 0; i < len; i++) {
			cout << d.Input[i];
			if (position == i) cout << "<";
			else cout << " ";
			if (i+1 == len) cout << endl;
		}
		cout << "Program Halted (Success)" << endl;
	}
}

//Make a set
set <string> Make_Set (string str) {
	stringstream ss (str);
	string buf;
	set <string> temp;
	
	while (ss >> buf) {
		temp.insert (buf);
	}
	return temp;
}

//Print a set
void Print_Set (set <string> s) {
	set <string>::iterator it;
	for (it = s.begin(); it != s.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

//This function is used to make a table out of an input string
Table Make_Table (string str, Data d) {
	stringstream ss(str); 	//make into string stream
	string buf;					//temp buffer
	int movement;						//temp int
	Table temp;					//temp table
	set <string>::iterator it; //iterator
	
	//Set state
	ss >> buf;					//Seperate the content and add to respective variables
	it = d.States.find(buf);
	if (it == d.States.end()) { //If state is not in the list of states, throw and error and exit
		cout << "Error: State '" << buf << "' was not valid as it is not a state. Exiting..." << endl;
		exit(0);
	} else {
		temp.state = buf;
	}
	//Set symbol
	ss >> buf;
	it = d.Alphabet.find(buf);
	if (it == d.Alphabet.end()) { //If symbol is not in the alphabet, throw and error and exit
		cout << "Error: Symbol '" << buf << "' was not valid as it is not in the alphabet. Exiting..." << endl;
		exit(0);
	} else {
		temp.symbol = buf;
	}
	//Set newState
	ss >> buf;
	it = d.States.find(buf);
	if (it == d.States.end()) { //If state is not in the list of states, throw and error and exit
		cout << "Error: newState '" << buf << "' was not valid as it is not a state. Exiting..." << endl;
		exit(0);
	} else {
		temp.newState = buf;
	}
	//Set newSymbol
	ss >> buf;
	it = d.Alphabet.find(buf);
	if (it == d.Alphabet.end()) { //If symbol is not in the alphabet, throw and error and exit
		cout << "Error: newSymbol '" << buf << "' was not valid as it is not in the alphabet. Exiting..." << endl;
		exit(0);
	} else {
		temp.newSymbol = buf;
	}
	//Set movement
	ss >> movement;
	if (movement == 1 || movement == -1) { 
		temp.movement = movement;
	} else { 
		cout << "Error: Movement can only be +/- 1. Exiting..." << endl;
		exit(0); 
	}
	
	return temp; //return the table
}

//This function is used to print out a table
void Print_Table (Table x, bool first) {
	if (first != true) cout << "\t       "; //This is used for formatting
	cout << x.state << " " << x.symbol << " -> ";
	cout << x.newState << " " << x.newSymbol << " ";
	if (x.movement > 0) {
		cout << "+" << x.movement << endl;
	} else {
		cout << x.movement << endl;
	}
}

//function to skip comments
bool SkipComments (string line) {
	if (line[0] == '#') return true;
	return false;
}

/* --------------------------------------------------------------------------------- */
//Input function:

Data GetInput() {
	Data temp;
	string tempBuffer;
	set <string> :: iterator it;
	
	ifstream File;
	File.open ("a2_binary_increment.tm");
	
	//Check file opened correctly
	if (File.is_open() == false) {
		cout << "Error: Could not read file." << endl; 
		exit(0);
	}
	
	/* --------------- States --------------- */
	getline (File, tempBuffer);
	
	//While we are reading a comment
	while (SkipComments(tempBuffer) == true) {
		//skip until we receive first proper line of input (Q)
		getline (File, tempBuffer);
	}
	temp.States = Make_Set(tempBuffer);
	
	/* --------------- Alphabet --------------- */
	getline (File, tempBuffer);
	
	//While we are reading a comment
	while (SkipComments(tempBuffer) == true) {
		//skip until we receive first proper line of input (Q)
		getline (File, tempBuffer);
	}
	temp.Alphabet = Make_Set(tempBuffer);

	/* --------------- Input word --------------- */
	getline (File, tempBuffer);
	
	//While we are reading a comment
	while (SkipComments(tempBuffer) == true) {
		//skip until we receive first proper line of input (Q)
		getline (File, tempBuffer);
	}

	stringstream ss (tempBuffer);
	string buf;
	
	while (ss >> buf) {
		//Check that each input is contained within defined alphabet
		it = temp.Alphabet.find(buf);
		if (it == temp.Alphabet.end()) {
			cout << "Error: Input word '" << buf << "' was not valid as it is not in the alphabet. Exiting..." << endl;
			exit(0);
		} else {
			temp.Input.push_back(buf);
		}
	}
	
	/* --------------- Initial State --------------- */
	getline (File, tempBuffer);
	
	//While we are reading a comment
	while (SkipComments(tempBuffer) == true) {
		//skip until we receive first proper line of input (Q)
		getline (File, tempBuffer);
	}
	
	//Check that Initial state is contained within defined states
	it = temp.States.find(tempBuffer);
	if (it == temp.States.end()) {
		cout << "Error: Initial State '" << tempBuffer << "' was not valid as it is not a state. Exiting..." << endl;
		exit(0);
	} else {
		temp.InitState = tempBuffer;
	}
	
	/* --------------- Final State --------------- */
	getline (File, tempBuffer);
	
	//While we are reading a comment
	while (SkipComments(tempBuffer) == true) {
		//skip until we receive first proper line of input (Q)
		getline (File, tempBuffer);
	}
	
	//Check that Final state is contained within defined states
	it = temp.States.find(tempBuffer);
	if (it == temp.States.end()) {
		cout << "Error: Final State '" << tempBuffer << "' was not valid as it is not a state. Exiting..." << endl;
		exit(0);
	} else {
		temp.FinalState = Make_Set(tempBuffer);
	}
	
	/* --------------- Instruction Table --------------- */
	getline (File, tempBuffer);
	//Check if we are in our first loop
	bool firstLoop = true;
	//While we are reading a comment
	while (SkipComments(tempBuffer) == true) {
		//skip until we receive first proper line of input (Q)
		getline (File, tempBuffer);
	}
	int i = 0;
	while (!File.eof()) {
		//On first loop, if no comments are present, use up previous getline before grabbing the next line
		if (firstLoop == false) { 
			//if not first loop, get the next line
			getline (File, tempBuffer);
		} else {
			//and if the first loop, set to false then continue
			firstLoop = false;
		}
		//Make a table object and push into the Table vector
		Table tempTable = Make_Table(tempBuffer, temp);
		int checkSize = temp.Instructions.size();
		//Check that the instruction you are reading is not already contained within the instruction table
		for (int j = 0; j < checkSize; j++) {
			if (temp.Instructions[j].state == tempTable.state && temp.Instructions[j].symbol == tempTable.symbol) {
				if (temp.Instructions[j].newState == tempTable.newState && temp.Instructions[j].newSymbol == tempTable.newSymbol) {
					//If there is already an instruction in the table, display an error and exit
					cout << "Error: There is already that instruction entered, this is a double up. Exiting..." << endl;
				} else {
					cout << "Error: There is already an instruction with that state/symbol. Exiting..." << endl;
				}
			}
		}
		//If not matching instruction was found, add the table to the instruction table.
		temp.Instructions.push_back(tempTable);
		i++;
	}
	//Check that Instruction Table has been implemented correctly
	//numInstructions must not be more than (numStates - numFinalStates) * numSymbols
	//e.g. 9 = (4-1)*3  <- 9 is the maximum number of instructions possible.
	//If there was more than 9 instructions, this will throw an error.
	int numStates = 0, numSymbols = 0, numFinalStates = 0, numInstructions = 0;
	numStates = temp.States.size();
	numSymbols = temp.Alphabet.size();
	numFinalStates = temp.FinalState.size();
	numInstructions = temp.Instructions.size();
	if (numInstructions > (numStates - numFinalStates) * numSymbols) {
		cout << "Error: Too many instructions! You can only have a maximum of "<< ((numStates - numFinalStates) * numSymbols) <<" State/Symbol combinations!" << endl;
		cout << "You currently have " << numInstructions  << " instructions. Exiting..." << endl;
		exit(0);
	}
	File.close();
	return temp;
}

/* --------------------------------------------------------------------------------- */
