#include <iostream>
#include <vector>
#include <tuple>
#include <array>
#include <list>
#include <cmath>
/*
A program to parse a user inputed mathimatical expression and completes it, while storing it in an efficent storable format
It converts the mathimatical expression into a mini program, which is then run, allowing the math to be both completed and stored
This is also intended to allow a variable, like X to be inserted so this function can be graphed
*/
// the data type used for calculations
using number = long double;
using namespace std;

//vector<short> Expression {}; // an array to store the expression "program"
vector<number> Numbers {}; // an array to store all numbers before and during calculations
array<char, 6> OpArraySymbolChar  {'^','/','%','*','-','+'}; // an array of all supported operands and thier supported values
array<char, 3> SpecalCharacters {'(',')'};

/*
Format for a program:
Operation
number
number
result goes to an adress in a list (0 is result)
operation
number
number
result
 */

 /*

 adr1
 adr2
 op
 numbers
 */
inline int find_index_op(char character){
    // a small function to return the index of an operand in the OpArraySymbolChar
    // to make sorting faster, along with order of operations

    for (int item = 0; item < OpArraySymbolChar.size(); ++item){
        if (OpArraySymbolChar.at(item) == character) return item;
    }

    // this should never happen ever, if so then send an error code
    return 127;
}

inline std::vector<number> parse_numbers(const string &inputString){
    /*
    A function to parse the numbers out of an inputted string. Returns a vector containing the
    numbers in order how they appeared in the string
    */

    // return value
    vector<number> numbers {};

    int i = 0;
    int start = 0; // the start in the string where the number will be
    int posInNumArray = 1; // the pos a number will occupy in the number array
    bool nextNumNeg = false;
    bool isFloat = false;

    while (i < inputString.length()){
        // iterate over the list and look for special characters

        // check if proceeding number is negitive
        if (inputString.at(i) == '#') {
            nextNumNeg = true;
            start = i+1; // make shure the number does not include the hashtag

        }

        // check for a period to determine if it is a float, or an int
        if (inputString.at(i) =='.'){
            isFloat = true;
        }


        // check if it is an operandOpArraySymbolChar
        for (char character : OpArraySymbolChar){
            if (character == inputString.at(i)){
                // now that it is known that this is an operand, store the number in the number array

                // check if it is float or int first
                number num;

                // convert the number into number using the apropriate data type
                if (isFloat){
                    num = (number) stod(inputString.substr(start, i));
                } else {
                    num = (number) stoi(inputString.substr(start, i));
                }

                // print out the number for debug
                cout << "Stored the number : " ;
                cout << num;
                cout <<'\n';

                if (nextNumNeg) num *= -1;

                // add the number to the vector
                numbers.push_back(num);

                // reset some of the counters and iterate for the next loop
                isFloat = false;
                nextNumNeg = false;
                start = i + 1;
                break;
            }
        }

        // iterate after all checks are run
        ++i;
    }

    // get the last number if it is there
    if (inputString.at(inputString.length() -1 ) > 47 &&
     inputString.at(inputString.length() -1)  < 58){
        // now that it is known that this is an operand, store the number in the number array

        // check if it is float or int first
        number num;

        // convert the number into number using the apropriate data type
        if (isFloat){
            num = (number) stod(inputString.substr(start, i));
        } else {
            num = (number) stoi(inputString.substr(start, i));
        }

        // print out the number for debug
        cout << "Stored the number : " ;
        cout << num;
        cout <<'\n';

        //check if the number is negitibe
        if (nextNumNeg) num *= -1;

        // add the number to the vector
        numbers.push_back(num);
     }

     // return the numbers
     return numbers;
}

inline std::vector<char> parse_operands(const string &inputString){
    /*
    A function to parse out all of the operands from the input string
    */

    // return value
    vector<char> operands {};

    // iterate over every char in the string, if it is a number, skip, else add it to the vector
    for (char character : inputString){
        if (!( character > 47 && character < 58 )) {
            // now check if its a rare forbidden operand
            if (character == '#') continue;

            // else add it to the vector
            operands.push_back(character);
        }
    }

    return operands;

}


//tuple<vector<short>, vector<number>>
std::tuple<std::vector<short>,
            std::vector<short>,
            std::vector<char>,
            std::vector<number>>
ConvertToCode(string inputString){
    /*
     * A function to convert an inputed string into a series of instructions (code) that can be used to solve an expression
     */

    // make parralel arrays to store the instructions information
    // for oder of operations, when an operation is complete, both adresses value will be changed to the
    // result of the operation
    // I am using parralel arrays because I wanted to challenge myself, i know stupid
    std::vector<short> adr1 = {};
    std::vector<short> adr2 = {};
    vector<number> numbers {}; // use the list data type, as there will be repeated edits in random locations
    vector<char> operands {};

    // find all of the numbers in the expression, and add them to the array.
    // If there is a # before the number, it is a negitive number
    // use a helper func to do this
    numbers = parse_numbers(inputString);

    // now print out the vector for debugging
    for (number num : numbers ){
        std::cout << num << "\n";
    }

    // now that the numbers have been parsed, parse for operands
    operands = parse_operands(inputString);

    // print it out for debug
    for (char character : operands){
        std::cout << character ;
    }

    // for debug ease of reading
    std::cout << '\n';

    // create all of the instructions
    for (short item = 0; item < operands.size(); ++item){
        // the equation that governs which operands get which symbols is govered by the eqution
        // adr1 = index, adr2 = index=1
        adr1.push_back(item);
        adr2.push_back(item +1);

    }

    // replace all of the operators with thier index in the operator reference array
    for (int item = 0; item < operands.size(); ++item) operands.at(item) = find_index_op(operands.at(item));


    // now parse the adresses as the numers are parsed, by searching for each operation type
    for (int c = OpArraySymbolChar.size(); c > 0; --c){
        // now find occurences of the operation in the array, and adjust surrounding adresses accoringly
        // to account for the fact that that operation will be done first
        for (int item=0; item < operands.size() - 1; ++item){
            if (operands.at(item) == c){
                // check the operation directly after and before, if they are a lower/equal bedmass value,
                // shift their first adress back one value
                if (operands.at(item + 1) <= c) {
                    adr1.at(item +1 ) = adr1.at(item);
                    std::cout << "shifted\n";
                }
            }
        }
    }


    // sort the instructions by order of operations (bubble sort because its easy)
    // also If somebody were to use a different sorting algorithm, it would break the code
    // this is because the first occurences of the values are placed after the last, meaning if
    // another sorting algorithm was used that didn't respect this, it would break order of operations
    // and therefore break the code. I should find some way to fix this in the future.
    bool swaps = true;
    auto n = operands.size() - 1;
    while (swaps){
        swaps = false;
        for (int x = 0; x < n; ++x) {
            if (operands[x] > operands[x+1]) {
                std::swap(operands[x], operands[x+1]);
                std::swap(adr1[x], adr1[x+1]);
                std::swap(adr2[x], adr2[x+1]);
                swaps = true;
            }
        }
        --n;
    }

    // now that all of the adresses have been changed to be correct for when the expression is completed
    // fulfilling order of operations, return the 4 arrays in a tuple
    return {adr1, adr2, operands, numbers};
}

number simplify(std::vector<short> adr1,
                std::vector<short>adr2,
                std::vector<char> operands,
                std::vector<number> numbers){
    /*
    A function to simplify the stored equation into a numerical result
    */

    // start with iterating over each operation in the list, and apply the operation
    // write the result to adress one, and the final adress one should be the result
    for (int i = 0; i < operands.size(); ++i){

        std::cout << "applying the " << (int) operands[i] << " operation to adresses :"
        << adr1[i] << ' ' << adr2[i] << '\n';


        // now check what operation to do
        switch (operands[i]){
            case(0):
                numbers[adr1[i]] = std::pow(numbers[adr1[i]], numbers[adr2[i]]);
                break;
            case(1):
                numbers[adr1[i]]= numbers[adr1[i]] / numbers[adr2[i]];
                break;

            case(2):
                numbers[adr1[i]] = std::fmod(numbers[adr1[i]], numbers[adr2[i]]);
                break;

            case(3):
                numbers[adr1[i]] = numbers[adr1[i]] * numbers[adr2[i]];
                break;

            case(4):
                numbers[adr1[i]] = numbers[adr1[i]] - numbers[adr2[i]];
                break;

            case(5):
                numbers[adr1[i]] = numbers[adr1[i]] + numbers[adr2[i]];
                break;

            default:
                // add the numbers by default
                numbers[adr1[i]] = numbers[adr1[i]] + numbers[adr2[i]];

        }

        std::cout << "the result was " << numbers[adr1[i]] << '\n';
    }
    // now the vector should be left with one item, return this item (the first adress of the last
    // operation)
    return numbers[adr1[operands.size() -1]];
}

int main(){
    string inputData {};

    cout << "Please enter a mathimatical expression (NO SYNTAX CHECKING YET)\n";
    cin >> inputData;
    auto [a1, a2, a3, a4] = ConvertToCode(inputData);

    // for debug purposes
    std::cout << "numbers\n";
    for (auto i : a4) {
        std::cout << i << " ";
    }
    std::cout << '\n';

    for (int x = 0; x < a3.size(); ++x){
        std::cout << (int) a3[x] << " | " << a1[x] << " | " << a2[x] << " | \n";
    }
    std::cout << '\n';
    // now crunch down the expression and print the result, fingers crossed
    cout << "the result is : ";
    cout << simplify(a1,a2,a3,a4);

    return 0;

}
