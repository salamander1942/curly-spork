#include <iostream>
#include <vector>
#include <tuple>
#include <array>
#include <list>
#include <cmath>
#include <stack>
/*
A program to parse a user inputed mathimatical expression and completes it, while storing it in an efficent storable format
It converts the mathimatical expression into a mini program, which is then run, allowing the math to be both completed and stored
This is also intended to allow a variable, like X to be inserted so this function can be graphed
*/
// the data type used for calculations
using number = long double;

//vector<short> Expression {}; // an array to store the expression "program"
std::vector<number> Numbers {}; // an array to store all numbers before and during calculations
std::array<char, 6> OpArraySymbolChar  {'^','/','%','*','-','+'}; // an array of all supported operands and thier supported values
std::array<char, 3> SpecalCharacters {'(',')'};

/*

adr1
adr2
op
numbers
*/
inline int find_index_op(char character) {
    // a small function to return the index of an operand in the OpArraySymbolChar
    // to make sorting faster, along with order of operations

    for (int item = 0; item < OpArraySymbolChar.size(); ++item) {
        if (OpArraySymbolChar.at(item) == character) return item;
    }

    // this should never happen ever, if so then send an error code
    return 127;
}

inline std::vector<number> parse_numbers(const std::string &inputString) {
    /*
    A function to parse the numbers out of an inputted string. Returns a vector containing the
    numbers in order how they appeared in the string
    */

    // return value
    std::vector<number> numbers {};

    int i = 0;
    int start = 0; // the start in the string where the number will be
    int posInNumArray = 1; // the pos a number will occupy in the number array
    bool nextNumNeg = false;
    bool isFloat = false;

    // iterate over the whole string, except the last number, which will get a special check
    while (i < inputString.length()) {
        // iterate over the list and look for special characters

        // check if proceeding number is negitive
        if (inputString.at(i) == '#') {
            nextNumNeg = true;
            start = i+1; // make shure the number does not include the hashtag
        }

        // check for a period to determine if it is a float, or an int
        if (inputString.at(i) =='.') {
            isFloat = true;
            ++i;
            continue; // skip furthur checks
        }

        // check if it is an operandOpArraySymbolChar or a bracket, by seeing if it is not a number
        if (!(inputString.at(i) > 47 && inputString.at(i)  < 58)) {

            // check if it is float or int first
            number num;

            // convert the number into number using the apropriate data type
            if (isFloat) {
                num = (number) stod(inputString.substr(start, inputString.size() - i));
            } else {
                num = (number) stoi(inputString.substr(start, inputString.size() - i));
            }

            // print out the number for debug
            std::cout << "Stored the number : " ;
            std::cout << num;
            std::cout <<'\n';

            if (nextNumNeg) num *= -1;

            // add the number to the vector
            numbers.push_back(num);

            // reset some of the counters and iterate for the next loop
            isFloat = false;
            nextNumNeg = false;
            start = i + 1;

            // search for the start of the next number, untill the end of the string if nessisary
            if (start != inputString.length() -1){
                while (start != inputString.length() &&!(inputString.at(start) > 47 && inputString.at(start) < 58) ) {
                    ++start;
                    i = start;
                }
            }
        }


        // iterate after all checks are run
        ++i;
    }
    // get the last number if it is there
    if (inputString.at(inputString.length() -1 ) > 47 &&
            inputString.at(inputString.length() -1)  < 58) {
        // now that it is known that this is a number, store the number in the number array

        // check if it is float or int first
        number num;

        // convert the number into number using the apropriate data type
        if (isFloat) {
            num = (number) stod(inputString.substr(start, inputString.size() - 1));
        } else {
            num = (number) stoi(inputString.substr(start, inputString.size() - 1));
        }

        // print out the number for debug
        std::cout << "Stored the number (final check) : " ;
        std::cout << num;
        std::cout <<'\n';

        //check if the number is negitibe
        if (nextNumNeg) num *= -1;

        // add the number to the vector
        numbers.push_back(num);

    }

    // return the numbers
    std::cout << "exiting parse numbers function" <<std::endl;
    return numbers;

}

inline std::vector<char> parse_operands(const std::string &inputString) {
    /*
    A function to parse out all of the operands from the input string
    */

    // return value
    std::vector<char> operands {};

    // iterate over every char in the string, if it is a number, skip, else add it to the vector
    for (char character : inputString) {
        if (!( character > 47 && character < 58 )) {
            // now check if its a rare forbidden operand
            if (character == '#' || character == ')' || character == '(') continue;

            // else add it to the vector
            operands.push_back(character);
        }
    }

    return operands;

}

std::tuple<std::vector<short>, std::vector<short>, std::vector<char>, std::vector<number>> convert_to_code(std::string inputString) {
    /*
     * A function to convert an inputed string into a series of instructions (code) that can be used to solve an expression
     */

    // make parralel arrays to store the instructions information
    // for oder of operations, when an operation is complete, both adresses value will be changed to the
    // result of the operation
    // I am using parralel arrays because I wanted to challenge myself, i know stupid
    std::vector<short> adr1 = {};
    std::vector<short> adr2 = {};
    std::vector<number> numbers {}; // use the list data type, as there will be repeated edits in random locations
    std::vector<char> operands {};

    // find all of the numbers in the expression, and add them to the array.
    // If there is a # before the number, it is a negitive number
    // use a helper func to do this
    numbers = parse_numbers(inputString);

    // now print out the vector for debugging
    for (number num : numbers ) {
        std::cout << num << "\n";
    }

    // now that the numbers have been parsed, parse for operands
    operands = parse_operands(inputString);

    // print it out for debug
    for (char character : operands) {
        std::cout << character ;
    }

    // for debug ease of reading
    std::cout << '\n';

    // create all of the instructions
    for (short item = 0; item < operands.size(); ++item) {
        // the equation that governs which operands get which symbols is govered by the eqution
        // adr1 = index, adr2 = index=1
        adr1.push_back(item);
        adr2.push_back(item +1);

    }

    // replace all of the operators with thier index in the operator reference array
    for (int item = 0; item < operands.size(); ++item) operands.at(item) = find_index_op(operands.at(item));


    // now parse the adresses as the numers are parsed, by searching for each operation type
    for (int c = 0; c < OpArraySymbolChar.size(); ++c) {
        // now find occurences of the operation in the array, and adjust surrounding adresses accoringly
        // to account for the fact that that operation will be done first
        for (int item=0; item < operands.size() - 1; ++item) {
            if (operands.at(item) == c) {
                // check the operation directly after and before, if they are a lower/equal bedmass value,
                // shift their first adress back one value
                if (operands.at(item + 1) >= c) {
                    adr1.at(item +1 ) = adr1.at(item);
                    std::cout << "shifted\n";
                }

                // iterate back untill a lesser / equal operand is found, otherwise continue looking untll the end of the array
                for (int x = item -1 ; x > 0; --x){
                    if (operands.at(item) <= operands.at(item) ) break;
                    --operands.at(item);
                }

            }
        }
    }

    // now that all of the adresses have been changed to be correct for when the expression is completed
    // fulfilling order of operations, return the 4 arrays in a tuple
    return {adr1, adr2, operands, numbers};
}

inline std::tuple<std::vector<short>, std::vector<short>, std::vector<char>, std::vector<number>> sorter(std::vector<short> adr1, std::vector<short>adr2, std::vector<char>operands, std::vector<number> numbers){


    /*
    a funciton to sort by order of operations
    */

    // sort the instructions by order of operations (bubble sort because its easy)
    // also If somebody were to use a different sorting algorithm, it would be better
    // the additional check however still needs to be run though
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
            } else if ((operands[x] == operands[x+1]) && (adr2[x] < adr2[x+1])) {
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

inline std::tuple<std::vector<short>, std::vector<short>, std::vector<char>, std::vector<number>> parse_highest_level(std::string inputString, std::vector<short> lastAdresses) {
    /*
    NOTICE: WILL NEED TO ADD FUNCTION TO PARSE EXPRESSION THAT COULD HAVE NOTHING HERE!
    It could also be some bracketed expression implicitly or explicity multiplied by a constant,
    so also include logic for that ( like an expression like (5+2) or 5(4-2) or 5*(5-2) ) the latter
    the program might be able to handel
    */


    std::cout << "\tParsing the highest level of the expression!" << std::endl;

    std::vector<short> offsets {}; // a vector to store the apropriate offsets for each adress jump
    std::vector<short> offsetStart{}; // to hold the index of where the offset should start
    std::string subString;
    short nonBrktStart = 0; // the start of a bracket
    short nonBrktEnd = 0;
    bool flag = false;
    short counter = 0;

    // put all un bracketed parts of the expression into a substring, replacing all bracketed parts with
    // a place holder character (in this case 1)
    // also store all required adresss shifts that will be nessisary due to the bracketed segments
    // being removed (if i remove the middle part of the string when converting to code, and I try to
    // use the same adresses as outputed, it will be an adress inside a bracket
    for (short u = 0; u < inputString.size(); ++u){
        // count how many  right opening brackets there are, and add to the depth
        if (inputString.at(u) == '('){
            if (flag != true) {nonBrktEnd = u; }// get the starting index of the bracket
            ++counter; flag = true; // now that the brackets have been found, iterate both the counter and the flag

        }
        if (inputString.at(u) == ')') {
            --counter;
        }

        // if it is inside or starting a bracket
        if (flag) {
            // if its just finished going through a bracket, mark the start of the next non bracketed part
            if (counter == 0){
                flag = false;
                nonBrktStart = u + 1;
                offsets.push_back(u);

            } else if (u == nonBrktEnd) {
                // if the program has just reached the start of a new bracketed expression
                // add the non bracketed preceeding characters to the substring, along with a placeholder for
                // the bracketed part

                offsetStart.push_back(subString.length() / 2); // where the offset will start accounting for the bracket end
                subString = inputString.substr(nonBrktStart, inputString.size() - nonBrktEnd) + "1";

            }
        }
    }
    // if the expression does not end in a bracket, add the last of the expression to the substring
    if (nonBrktStart < subString.length()) {
        offsetStart.push_back(subString.length() / 2);
        subString = inputString.substr(nonBrktStart, inputString.size() - nonBrktEnd);
    }

    std::cout << "the string to be parsed is : "<< subString << std::endl;

    // now parse the substring
    auto [_adr1, _adr2, _op, _num] = convert_to_code(subString);

    // now add the apropriate offset to the adresses to account for the sections where brackets were removed
    for (short O = 0; O < offsetStart.size() - 1; ++O) {
        for(int X = offsetStart[O]; X < offsetStart[O+1]; ++X){
            ++_adr1[ offsets[O] ];
            ++_adr2[ offsets[O] ];
        }

    }
    // for the last offset, do untill the end of the list
    for(int X = offsetStart.back(); X < _adr1.size(); ++X){
            ++_adr1[ offsets.back() ];
            ++_adr2[ offsets.back() ];
    }

    // replace the temporary adresses for the brackets with the real adresses for the brackets
    for (short adress : offsetStart) {
        _adr1.at(adress) = lastAdresses.back();
        lastAdresses.pop_back();
    }

    // finally sort the output
    auto result = sorter(_adr1, _adr2, _op, _num);

    // return the parsed expression when the program is done running
    // the number return is only for debug, but it will never be used
    return result;



}

std::tuple<std::vector<short>, std::vector<short>, std::vector<char>, std::vector<number>> parse_expression(std::string inputString){

    /*
    a function that converts code into a list of instructions, taking into acount brackets
    this function requires that there are no syntax errors
    */

    // the bracket depth
    short bracketDepth = 0;

    // the vectors to store the operand, the nubers, and the operands adresses
    std::vector<short> adr1 {};
    std::vector<short> adr2 {};
    std::vector<char> operands = parse_operands(inputString);
    std::vector<number> numbers = parse_numbers(inputString);

    // first count how many brackets it goes deep
    {
    bool flag = false;
    short brkDpth = 0; // for comparing two bracket depths, a temporary variable
    short brkWidth = 0; // for how many brackets are beside eachother
    short counter = 0; // when it is zero, the left and right brackets have cancele out, meaning your at the bottom of a bracket set
    for (char c : inputString){
        // count how many  right opening brackets there are, and add to the depth
        if (c == '('){
            ++brkDpth; ++counter; flag = true; // now that the brackets have been found, iterate both the counter and the flag
        }
        if (c == ')') {
            --counter;
        }

        // if it has detected a full level of brackets, see if it is the deepest
        if (flag && (counter == 0)) {
            if (brkDpth > bracketDepth) bracketDepth = brkDpth;
            brkDpth = 0;
            flag = false;
            ++brkWidth; // a full level is also a full width
        }
        std::cout << "the bracket depth var is " << brkDpth << " and the deepest depth is " << bracketDepth << " and the char is" << c << std::endl;;
    }
    }

    // if there are no brackets, skip furthur braket logic
    if (bracketDepth == 0){
        std::cout << "no brackets detected" << std::endl;
        return convert_to_code(inputString);
    }


    // starting at the lowest bracket level
    // parse each bracket as a substring, then shift the result using a counter to get the actual full string location of the numbers and operations
    // then when parsing a higher level bracket, and encountering a lower level bracket, parse the bracket from the front, replace the bracketed expression with a placeholder, and then parse the back if any

    bool numTog = false; // assume the first char is a number or a bracket, following syntax rules
    short opIndex = 0; // the operatior index is reset to zero every iteration
    short brkDpth = 0;
    std::vector<short> lastAdresses {};
    std::string substring;

    // iterate down the bracket depts
    while (bracketDepth) {
        std::cout << "while bracket depth is : " << bracketDepth << std::endl;

        // iterate over the string, stopping at the lowest bracket depth
        for (short x = 0; x < inputString.size(); ++x) {
            std::cout << "\tfor " << x << " in range " << inputString.size() << std::endl;

            // count how many  right opening brackets there are, and add to the depth.
            if (inputString[x] == '(') ++brkDpth;
            if (inputString[x] == ')') --brkDpth;

            // count the operator adress depth if c is an operand
            if (!( inputString[x] > 47 && inputString[x] < 58 ) && !(inputString[x] == '#' || inputString[x] == ')' || inputString[x] == '(')) ++opIndex;

            // make shure this only exicutes when its also reached the end of a bracket
            if ((brkDpth == bracketDepth) && ((inputString.at(x) == '('))){
                std::cout << "\tfound the start of a bracket!" << std::endl;
                // now that the searched for depth is found, parse though the bracketed expression untill the end of the bracket is reached
                // record the starting adress, and find the ending adress

                ++x; // iterate X to avoid checking it again if its a '(' bracket
                short adrStart = x;

                while (true) {

                    // get the part of the expression in front of a theroretical lower bracket ( if the bracket doesn't exist, skip furthur logic)
                    if (inputString[x] == '(') {
                        numTog = true;
                        x -= 2; // to account for the operator in front of the bracket
                        break;
                    }

                    // first check if there is no center bracket set, and the brackets end, meaning the program has reached the end of the expression
                    if (inputString[x] == ')') break;


                    ++x; // iterate furthur though the string

                }

                // now that the first half of the brackets length has been found, decide weather to parse behind
                // the bracket or not, (its not nessisary if nothing is there
                if (numTog) {
                    std::cout << "\tNumtog is true, bracket is inside" << std::endl;
                    // first find the other end of the bracket
                    while(inputString[x] != ')') ++x;

                    // now find the end of the section after the bracket, and place it in a substring
                    while (inputString[x] != ')') ++x;

                    short brktStartAdr = x -1;

                    // add a pretend character to the substring as a placeholder for the brackt between the two  sides of the bracket
                    // the minus one on the other one is to avoid including the end bracket
                    substring = inputString.substr(adrStart, inputString.size() - brktStartAdr) + "1" + inputString.substr(brktStartAdr, inputString.size() - x - 1);

                    std::cout << "the string to be parsed is : "<< substring << std::endl;

                    // now parse the substring
                    auto [_adr1, _adr2, _op, _num] = convert_to_code(substring);

                    // count how many operands are before the first half of the bracket for apropriate offset
                    short toFirstBracket = inputString.substr(adrStart, inputString.size() - brktStartAdr).size();

                    // now add the apropriate offset to the fist few of the adresses
                    for (short &num : _adr1) num += opIndex;
                    for (short &num : _adr2) num += opIndex;

                    // replace the first adress of the bracketed part with the bracket adress
                    _adr1.at(toFirstBracket -1 ) = lastAdresses.back();
                    // remove the last adress so it is not used again
                    lastAdresses.pop_back();

                    // now add the apropriate offset to the fist few of the adresses
                    for (int i = 0; i < brktStartAdr; ++i) _adr1.at(i) += adrStart;
                    for (int i = 0; i < brktStartAdr; ++i) _adr2.at(i) += adrStart;

                    // and the apropriate offset to the last adresses
                    for (int i = 0; i < brktStartAdr; ++i) _adr1.at(i) += brktStartAdr;
                    for (int i = 0; i < brktStartAdr; ++i) _adr2.at(i) += brktStartAdr;

                    // finally sort the output
                    auto result = sorter(_adr1, _adr2, _op, _num);
                    _adr1 = std::get<0>(result);
                    _adr2 = std::get<1>(result);
                    _op = std::get<2>(result);
                    _num = std::get<3>(result);


                    // add the result to the main vector
                    for (int i = 0; i < _adr1.size(); ++i){
                        adr1.push_back(_adr1.at(i));
                        adr2.push_back(_adr2.at(i));
                        operands.push_back(_op.at(i));
                    }

                } else {

                    std::cout << "\tNumtog is false, bracket not inside" << std::endl;
                    // there is nothing behind the bracket, so parse the expression without any furthur complications
                    auto [_adr1, _adr2, _op, _num] = convert_to_code(inputString.substr(adrStart, x - adrStart));
                    // now sort it
                    auto result = sorter(_adr1, _adr2, _op, _num);
                    _adr1 = std::get<0>(result);
                    _adr2 = std::get<1>(result);
                    _op = std::get<2>(result);
                    _num = std::get<3>(result);


                    // now that the expressions have been parsed, shift the adresses to fit into the main expression
                    for (short &num : _adr1) num += opIndex;
                    for (short &num : _adr2) num += opIndex;

                    // add the expression to the main one, and store the final adress in the last adress vector
                    std::cout << "\tStoring numbers into the vector, making it this much longer : " << _op.size() << std::endl;
                    for (int i = 0; i < _op.size(); ++i){
                        adr1.push_back(_adr1.at(i));
                        adr2.push_back(_adr2.at(i));
                        operands.push_back(_op.at(i));
                    }

                    // add the last adress to the last adress array
                    lastAdresses.push_back(adr1.back());
                    std::cout << "\tStored last adress" << std::endl;
                }


            }

        // end of for loop, set numTog to false
        numTog = false;
        }

        // end of while loop, iterate down the bracketdepth, as the program climbs "up" the expression
        --bracketDepth;
    }

    // now parse the highest level of the expression
    auto [a, b, c, d] = parse_highest_level(inputString, lastAdresses);

    // and add it to the main array
    for (int x = 0; x < a.size(); ++x){
        adr1.push_back(a.at(x));
        adr2.push_back(b.at(x));
        operands.push_back(c.at(x));
    }

    // return the parsed expression, with the highest level parsed when the program is done running
    return {adr1, adr2, operands, numbers};
    }

number simplify(std::vector<short> adr1,
                std::vector<short>adr2,
                std::vector<char> operands,
                std::vector<number> numbers) {
    /*
    A function to simplify the stored equation into a numerical result
    */

    // start with iterating over each operation in the list, and apply the operation
    // write the result to adress one, and the final adress one should be the result
    for (int i = 0; i < operands.size(); ++i) {

        std::cout << "applying the " << (int) operands[i] << " operation to adresses :"
                  << adr1[i] << ' ' << adr2[i] << '\n';


        // now check what operation to do
        switch (operands[i]) {
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


bool test (){
    /*
    A function to test the program, notice that all couts have std:endl this is to ensure that
    all debuging messages are seen, although it will slow the program down
    */

    std::cout << "STARTING TESTS:" << std::endl;

    // a list of text to parse, and the expected result (if the function exicutes corectly)
    // for varius functions
    std::array<std::string, 5> TEST_STRINGS {
        "1-2+3*(4-5)+6",
        "(1-2)*3-4",
        "1-2*(3+4)",
        "1-(2+3)+(4-5)+6",
        "(1+2)-(3+4)",
    };
    // for the output of the function
    //(I should really replace this with its own name, for how often I've used this, but here i will break my
    // rules and use a struct to avoid an eye sore
    struct TestCase {
        std::vector<short> a;
        std::vector<short> b;
        std::vector<char> c;
        std::vector<number> d;
    };

    // manually created proper function outputs
    // use a large number for the bracket adress placeholder
    std::array<TestCase, 3> CORRECT_OUTPUT = {{
        TestCase{{2,0,1,2}, {255,1,2,5}, {3,4,5,5}, {1,2,3,1,6}},
        TestCase{{255,2}, {2,3}, {'*','-'}, {1,2,3,4}},
        TestCase{{1,0}, {255,2}, {'*','-'}, {1,2,3,4}}
    }};

    // for the results
    std::vector<short> adr1 {};
    std::vector<short> adr2 {};
    std::vector<char> operands {};
    std::vector<number> numbers {};

    std::cout << "TESTING THE PARSE HIGHEST EXPRESSION FUNCTION" << std::endl;

    // see if each time the function runs, it produces the expected output
    for (int x = 0; x < CORRECT_OUTPUT.size(); ++x){
        std::cout << "INPUT EXPRESSION : " << TEST_STRINGS.at(x) << std::endl;

        // print out the expected output
        std::cout << "EXPECTED OUTPUT : \n[";
        for (auto item : CORRECT_OUTPUT.at(x).a) std::cout << " " << item;
        std::cout << "] \n[";
        for (auto item : CORRECT_OUTPUT.at(x).b) std::cout << " " << item;
        std::cout << "] \n ";
        for (auto item : CORRECT_OUTPUT.at(x).c) std::cout << " " << (int) item; // the char values are not visible
        std::cout << "] \n[";
        for (auto item : CORRECT_OUTPUT.at(x).d) std::cout << " " << item;
        std::cout << "] " << std::endl;

        // now compute the function and compare the outputs
        auto [adr1, adr2, operands, numbers] = parse_highest_level(TEST_STRINGS.at(x), std::vector<short> {255});

        std::cout << "REAL OUTPUT : \n[";
        for (auto item : adr1) std::cout << " " << item;
        std::cout << "] \n[";
        for (auto item : adr2) std::cout << " " << item;
        std::cout << "] \n[";
        for (auto item : operands) std::cout << " " << (int) item; // the char values are not visible
        std::cout << "] \n[";
        for (auto item : numbers) std::cout << " " << item;
        std::cout << "] " << std::endl;

    }

    return true;


}

int main() {

    if(test()) return 0;

    return 1;

}
