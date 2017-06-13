#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include "Int.h"
#include "Loop.h"

using namespace std;

bool isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

int main(int argc, char *argv[])
{
	//variables
	unsigned long long lineNumber = 0;
	unsigned long long lineNumberMax = 0;

	vector<Int> intVariables;
	vector<Int>::iterator it;
	vector<Int>::iterator it_result;
	vector<Int>::iterator it_op1;
	vector<Int>::iterator it_op2;
	vector<Int>::iterator lValueIterator;
	vector<Int>::iterator rValueIterator;
	
	Loop loop;

	string fileToCompileName;

	/* regex patterns */

	//print patterns
	regex printPattern("\\s*print \"(.*)\"");
	regex printLnPattern("\\s*println \"(.*)\"");
	regex printLnNewLinePattern("\\s*println");

	//print with variables
	regex printVariablePattern("\\s*print (.*)");
	regex printLnVariablePattern("\\s*println (.*)");

	//print variables' values with text
	//regex printVariablesWithTextPattern("print (.*\\${.*}.*)");
	regex printVariablesWithTextPattern("\\s*print (.*\\$.*)");
	//regex printlnVariablesWithTextPattern("println (.*\\${.*}.*)");
	regex printlnVariablesWithTextPattern("\\s*println (.*\\$\\(.*\\).*)");

	//init new variables
	regex intVarPattern("\\s*var (.*): Int");
	regex intVarInitiatePattern("\\s*var (.*): Int = (.*)");

	//assign new value to variables
	//regex assignNewValueToVariablePattern("\\s*(.*) = ([[:digit:]]*)\\s*");
	regex assignNewValueToVariablePattern("\\s*(.*) = ([[:digit:]]*)$");
	regex assignVariableValueToVariablePattern("\\s*(.*) = (.*)$");
	
	//TODO: assign one variable value to another!!!
	
	//TODO: a = a + b doesn't check if for example b variable exists!!! So 'a' variable has garbage inside

	//basic math operations
	//TODO: these only work on two variables and not on two number literals or number literal and a variable!!!
	regex addIntVarsPattern("\\s*(.*) = (.*) \\+ (.*)");
	regex subtractIntVarsPattern("\\s*(.*) = (.*) - (.*)");
	regex multiplyIntVarsPattern("\\s*(.*) = (.*) \\* (.*)");
	regex divideIntVarsPattern("\\s*(.*) = (.*) / (.*)");
	regex moduloIntVarsPattern("\\s*(.*) = (.*) % (.*)");
	
	//loops
	regex loopPattern("\\s*loop ([[:digit:]]+)");
	regex endLoopPattern("\\s*end loop");

	//comments and empty lines
	regex oneLineCommentPattern("^\\s*#");
	regex emptyLinePattern("^$");

	smatch result;

	if (argc == 1)
	{
		cout << "Welcome to Linuxer 0.01" << endl;
		cout << "Usage: " << argv[0] << " <program_name.lnx>" << endl;
		return 1;
	}

	if (argc > 1)
	{
		fileToCompileName = argv[1];
	}

	ifstream fileToCompile;
	string line = "";
	vector<string> programSourceCode;
	programSourceCode.push_back(line); //add empty string at the beginning, so we can start numbering lines from 1, not from 0

	fileToCompile.open(fileToCompileName);
	
	if (fileToCompile.is_open())
	{
		while (getline(fileToCompile, line))
		{
			programSourceCode.push_back(line);
			++lineNumberMax;
		}
	}

	if (fileToCompile.is_open())
	{
		//while (getline(fileToCompile, line))
		
		//start the actual parsing - go through the file with script
		lineNumber = 1; //start numbering from 1
		
		while (lineNumber <= lineNumberMax)
		{
			//cout << "[DEBUG]: " << lineNumber << endl;
			//cout << "[DEBUG]: " << lineNumberMax << endl;
			
			line = programSourceCode.at(lineNumber);

			//comment line - begins with '#' sign
			if (regex_search(line, result, oneLineCommentPattern))
			{
				//one line comment - don't parse line
			}

			//print text and variables' values
			else if (regex_search(line, result, printVariablesWithTextPattern))
			{
				//cout << "DEBUG: pattern matched" << endl;
				string wholeString = result[1];

				for (auto const& var: intVariables)
				{
					//regex varRegex ("\\$" + var.varName);
					//regex varRegex ("\\${" + var.varName + "}");
					//regex varRegex ("\\$\\[" + var.varName + "]");
					regex varRegex ("\\$\\(" + var.varName + "\\)");
					wholeString = regex_replace(wholeString, varRegex, to_string(var.value));
				}

				if (wholeString.front() == '"')
				{
					wholeString.erase(0, 1); // erase the first character
					wholeString.erase(wholeString.size() - 1); // erase the last character
				}

				cout << wholeString;
			}

			//print text and variables' values
			else if (regex_search(line, result, printlnVariablesWithTextPattern))
			{
				//cout << "DEBUG: pattern matched" << endl;
				string wholeString = result[1];

				for (auto const& var: intVariables)
				{
					//regex varRegex ("\\$" + var.varName);
					//regex varRegex ("\\${" + var.varName + "}");
					//regex varRegex ("\\$\\[" + var.varName + "]");
					regex varRegex ("\\$\\(" + var.varName + "\\)");
					wholeString = regex_replace(wholeString, varRegex, to_string(var.value));
				}

				if (wholeString.front() == '"')
				{
					wholeString.erase(0, 1); // erase the first character
					wholeString.erase(wholeString.size() - 1); // erase the last character
				}

				cout << wholeString << endl;
			}

			//print text, and add go to a new line
			else if (regex_search(line, result, printLnPattern))
			{
				cout << result[1] << endl;
			}

			//print text
			else if (regex_search(line, result, printPattern))
			{
				cout << result[1];
			}

			//print variable value and go to a new line
			else if (regex_search(line, result, printLnVariablePattern))
			{
					string key = result[1];
					it = find_if(intVariables.begin(), intVariables.end(), [key] (const Int& o) -> bool {return o.varName == key;});
					cout << it->value << endl;
			}

			//print variable value
			else if (regex_search(line, result, printVariablePattern))
			{
					string key = result[1];
					it = find_if(intVariables.begin(), intVariables.end(), [key] (const Int& o) -> bool {return o.varName == key;});
					cout << it->value;
			}

			//initialize Int with a given value
			else if (regex_search(line, result, intVarInitiatePattern))
			{
				//TODO: append to Vector of type Int
				Int varInt;
				varInt.varName = result[1];
				varInt.value = stoi(result[2]);
				intVariables.push_back(varInt);
			}

			//initialize Int with default value of 0
			else if (regex_search(line, result, intVarPattern))
			{
				//TODO: append to Vector of type Int
				Int varInt;
				varInt.varName = result[1];
				varInt.value = 0; //set to a reasonable default name
				intVariables.push_back(varInt);
			}

			//assign new value to Int variable
			else if (regex_search(line, result, assignNewValueToVariablePattern))
			{
				//cout << "DEBUG: result[1] = " << result[1] << ", result[2] = " << result[2] << endl;
				Int varInt;
				varInt.varName = result[1];
				varInt.value = stoi(result[2]);

				//find existing variable and assign new value to this variable
				//TODO: type checking! If there other types than Int will be implemented types need to be checked!!!
				string key1 = varInt.varName;
				it_result = find_if(intVariables.begin(), intVariables.end(), [key1] (const Int& o) -> bool {return o.varName == key1;});
				it_result->value = varInt.value;
			}
			
			//add two Ints
			else if (regex_search(line, result, addIntVarsPattern))
			{
				int op1value = 0;
				int op2value = 0;
			
				string key1 = result[1];
				it_result = find_if(intVariables.begin(), intVariables.end(), [key1] (const Int& o) -> bool {return o.varName == key1;});

				string key2 = result[2];
				
				if (isNumber(key2))
				{
					op1value = stoi(key2);
				}
				else
				{
					it_op1 = find_if(intVariables.begin(), intVariables.end(), [key2] (const Int& o) -> bool {return o.varName == key2;});
					op1value = it_op1->value;
				}

				string key3 = result[3];
				
				if (isNumber(key3))
				{
					op2value = stoi(key3);
				}
				else
				{
					it_op2 = find_if(intVariables.begin(), intVariables.end(), [key3] (const Int& o) -> bool {return o.varName == key3;});
					op2value = it_op2->value;
				}

				it_result->value = op1value + op2value;
			}

			//subtract one Int from another
			else if (regex_search(line, result, subtractIntVarsPattern))
			{
				int op1value = 0;
				int op2value = 0;
			
				string key1 = result[1];
				it_result = find_if(intVariables.begin(), intVariables.end(), [key1] (const Int& o) -> bool {return o.varName == key1;});

				string key2 = result[2];
				
				if (isNumber(key2))
				{
					op1value = stoi(key2);
				}
				else
				{
					it_op1 = find_if(intVariables.begin(), intVariables.end(), [key2] (const Int& o) -> bool {return o.varName == key2;});
					op1value = it_op1->value;
				}

				string key3 = result[3];
				
				if (isNumber(key3))
				{
					op2value = stoi(key3);
				}
				else
				{
					it_op2 = find_if(intVariables.begin(), intVariables.end(), [key3] (const Int& o) -> bool {return o.varName == key3;});
					op2value = it_op2->value;
				}

				it_result->value = op1value - op2value;
			}

			//multiply two Ints
			else if (regex_search(line, result, multiplyIntVarsPattern))
			{
				int op1value = 0;
				int op2value = 0;
			
				string key1 = result[1];
				it_result = find_if(intVariables.begin(), intVariables.end(), [key1] (const Int& o) -> bool {return o.varName == key1;});

				string key2 = result[2];
				
				if (isNumber(key2))
				{
					op1value = stoi(key2);
				}
				else
				{
					it_op1 = find_if(intVariables.begin(), intVariables.end(), [key2] (const Int& o) -> bool {return o.varName == key2;});
					op1value = it_op1->value;
				}

				string key3 = result[3];
				
				if (isNumber(key3))
				{
					op2value = stoi(key3);
				}
				else
				{
					it_op2 = find_if(intVariables.begin(), intVariables.end(), [key3] (const Int& o) -> bool {return o.varName == key3;});
					op2value = it_op2->value;
				}

				it_result->value = op1value * op2value;
			}

			//divide two Ints
			else if (regex_search(line, result, divideIntVarsPattern))
			{
				int op1value = 0;
				int op2value = 0;
			
				string key1 = result[1];
				it_result = find_if(intVariables.begin(), intVariables.end(), [key1] (const Int& o) -> bool {return o.varName == key1;});

				string key2 = result[2];
				
				if (isNumber(key2))
				{
					op1value = stoi(key2);
				}
				else
				{
					it_op1 = find_if(intVariables.begin(), intVariables.end(), [key2] (const Int& o) -> bool {return o.varName == key2;});
					op1value = it_op1->value;
				}

				string key3 = result[3];
				
				if (isNumber(key3))
				{
					op2value = stoi(key3);
				}
				else
				{
					it_op2 = find_if(intVariables.begin(), intVariables.end(), [key3] (const Int& o) -> bool {return o.varName == key3;});
					op2value = it_op2->value;
				}

				if (op2value == 0)
				{
					cout << "\n[ERROR]: Division by zero" << endl;
					return 2; //TODO: think of error numbers per each error type
				}
				else
				{
					it_result->value = op1value / op2value;
				}
			}

			//modulo two Ints
			else if (regex_search(line, result, moduloIntVarsPattern))
			{
				int op1value = 0;
				int op2value = 0;
			
				string key1 = result[1];
				it_result = find_if(intVariables.begin(), intVariables.end(), [key1] (const Int& o) -> bool {return o.varName == key1;});

				string key2 = result[2];
				
				if (isNumber(key2))
				{
					op1value = stoi(key2);
				}
				else
				{
					it_op1 = find_if(intVariables.begin(), intVariables.end(), [key2] (const Int& o) -> bool {return o.varName == key2;});
					op1value = it_op1->value;
				}

				string key3 = result[3];
				
				if (isNumber(key3))
				{
					op2value = stoi(key3);
				}
				else
				{
					it_op2 = find_if(intVariables.begin(), intVariables.end(), [key3] (const Int& o) -> bool {return o.varName == key3;});
					op2value = it_op2->value;
				}

				if (op2value == 0)
				{
					cout << "\n[ERROR]: Division by zero" << endl;
					return 2; //TODO: think of error numbers per each error type
				}
				else
				{
					it_result->value = op1value % op2value;
				}
			}
			
			//assign variable value to Int variable
			else if (regex_search(line, result, assignVariableValueToVariablePattern))
			{
				//cout << "DEBUG: result[1] = " << result[1] << ", result[2] = " << result[2] << endl;
				Int varInt;
				varInt.varName = result[1];
				
				//rValue to be assigned to lValue variable
				string key1 = result[2];
				rValueIterator = find_if(intVariables.begin(), intVariables.end(), [key1] (const Int& o) -> bool {return o.varName == key1;});
				
				//find existing variable and assign new value to this variable
				//TODO: type checking! If there other types than Int will be implemented types need to be checked!!!
				string key2 = varInt.varName;
				lValueIterator = find_if(intVariables.begin(), intVariables.end(), [key2] (const Int& o) -> bool {return o.varName == key2;});
				lValueIterator->value = rValueIterator->value;
			}
			
			//loop <count>, for example loop 10
			else if (regex_search(line, result, loopPattern))
			{
				loop.inLoop = true;
				loop.firstLine = lineNumber; //first line of the loop
				loop.howManyTimes = stoi(result[1]);
				//cout << "\n[DEBUG]: " << "loop.howManyTimes = " << loop.howManyTimes << endl;
				//cout << "lineNumber = " << lineNumber << endl;
			}
			
			//end loop... literally
			else if (regex_search(line, result, endLoopPattern))
			{
				--loop.howManyTimes;
				
				if (loop.howManyTimes == 0)
				{
					loop.inLoop = false;
					loop.rewind = false;
				}
				else
				{
					lineNumber = loop.firstLine;
					loop.rewind = true;
				}
			}

			//print just a new line
			else if (regex_search(line, result, printLnNewLinePattern))
			{
				cout << endl;
			}

			//ignore empty line in code
			else if (regex_search(line, result, emptyLinePattern))
			{
				//emtpy line - do nothing
			}

			//syntax error if uknown command
			else
			{
				cout << "\n[ERROR]: ";
				cout << "Syntax error in line " << lineNumber << ": I don't understand \"" << line << "\"!" << endl;
			}
		
			++lineNumber;
		}

		fileToCompile.close();
	}
	else
	{
		cout << "Unable to open file " << fileToCompileName << endl;
	}	

	
	return 0;
}
