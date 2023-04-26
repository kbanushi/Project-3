// -------------------------------------------
// Program 3: Game of 24 Part 2
// Course: CS 211, Spring 2023, UIC
// System: VSCode on MacOS
// Author: Krenar Banushi
// -------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node* ptrCharNode;
struct Node{
    char value;
    ptrCharNode next;
};

struct Stack{
    ptrCharNode head;
    int numOpenParentheses;
    int size;
};

///////////////////////////////////////FUNCTION PROTOTYPES///////////////////////////////////////
void pushFront(struct Stack* stack, char value);

char popFront(struct Stack* stack);

int calculateSegment(int num1, int num2, char operator);

char inputType(char element);

int inputPrecedence(char element);

int stackPrecendence(char element);

void emptyStack(struct Stack* stack);

char* convertToPostfix(struct Stack* stack, char infix[81], int* errCode);

void charStackConstructor(struct Stack* stack);

int calculatePostFix(char* postfix, int errCode);

void addPuzzle(int*** puzzleArr, int* maxSize, int* currSize, char line[81]);

void clearPuzzleArr(int*** puzzleArr, int* currSize, int* maxSize);

void readFile(int*** puzzleArr, int* maxSize, int* currSize, char fileName[81]);

int usedAllNums(int* nums, char* input);

int validSymbols(char* input);

void fillPuzzleArr(int*** puzzleArr, int* maxSize, int* currSize, char selection);

int validNumValues(char* input);

int validInput(int* nums, char* input);

void play(int** puzzleArr, int currSize);

void setUpGame(int*** puzzleArr, int* maxSize, int* currSize);
///////////////////////////////////////FUNCTIONS///////////////////////////////////////
/// @brief Create a new node and push it to the front of a linked list given the head of the list.  Additionally updates number of open parentheses if one is seen.
/// @param stack stack to push new value to
/// @param value char of new value to prepend
void pushFront(struct Stack* stack, char value){
    ptrCharNode temp = (ptrCharNode)malloc(sizeof(struct Node));
    temp->value = value;
    temp->next = stack->head;
    stack->head = temp;
    (stack->size)++;
    if (value == '(')
        (stack->numOpenParentheses)++;
}

/// @brief Remove node at the front of the list and return its value.  Does not decrement numOpenParentheses.
/// @param stack stack to pop the front node from
/// @return char of value that was in the front node
char popFront(struct Stack* stack){
    if (stack->head == NULL)
        return 'f';

    ptrCharNode next = (stack->head->next);
    char val = (stack->head->value);
    free(stack->head);
    (stack->head) = next;
    (stack->size)--;

    return val;
}

/// @brief Return evaluation of two integers using char representing the operator
/// @param num1 integer on left side of expression
/// @param num2 integer on right side of expression
/// @param operator char of operator to use
/// @return integer offset to be cased to char
int calculateSegment(int num1, int num2, char operator){
    int result;
    switch (operator){
        case '+':
            result = num1 + num2;
            printf("%d + %d = %d.\n", num1, num2, result);
            break;
        case '-':
            result = num1 - num2;
            printf("%d - %d = %d.\n", num1, num2, result);
            break;
        case '*':
            result = num1 * num2;
            printf("%d * %d = %d.\n", num1, num2, result);
            break;
        case '/':
            result = num1 / num2;
            printf("%d / %d = %d.\n", num1, num2, result);
            break;
        default:
            printf("calculate segment error");
            return 0;
    }
    return result + '0';
}

/// @brief Return character specifying what type a given char is
/// @param element char to observe type of
/// @return char representing the char type (Ex: 'o' for operators, '(' for left parentheses, 'n' for numbers)
char inputType(char element){
    switch (element){
        case '+':
        case '-':
        case '*':
        case '/':
            return 'o';
        case '(':
            return 'l';
        case ')':
            return 'r';
        case ' ':
            return 's';
        case '\n':
            return 'a';
        case '\0':
            return 'e';
        default:
            return 'n';
    }
}

/// @brief return precedence of a character as an integer (operators and parentheses only)
/// @param element char to get precedence of 
/// @return integer of the character's precedence
int inputPrecedence(char element){
    switch (element){
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 3;
        case '(':
            return 5;
        case ')':
            return 0;
        case ' ':
        case '\n':
            printf("input precedence error (space)");
        default:
            printf("input precedence error (default)");
            return -1;
    }
}

/// @brief return precedence of character from the top of the stack (operators and left parentheses only)
/// @param element char to find precedence of
/// @return integer specifying the char's precedence
int stackPrecendence(char element){
    switch (element){
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
            return 4;
        case '(':
            return 0;
        default:
            printf("stack precedence error");
            return -1;
    }
}

/// @brief function used to pop off elements from stack until it is empty
/// @param stack stack passed by reference to empty
void emptyStack(struct Stack* stack){
    while (stack->size > 0){
        popFront(stack);
    }
    stack->head = NULL;
    stack->numOpenParentheses = 0;
}

/// @brief Convert an infix expression to a postfix expression and return an error code by reference if there are too many open or closed parentheses (1 or 2 respecitively)
/// @param stack stack passed by reference to use for conversion
/// @param infix char array of infix expresison
/// @param errCode integer passed by reference specifying if an error occurred
/// @return dynamically allocated char array of postfix result
char* convertToPostfix(struct Stack* stack, char infix[81], int* errCode){
    strcat(infix, ")"); //Add closing parentheses to input
    int length = strlen(infix), index = 0, tooManyClosed = 0;
    char front;
    char* postfix = (char*)malloc(sizeof(char) * (length + 1));
    pushFront(stack, '('); //Add opening parentheses to stack
    
    for (int i = 0; i < length; i++){
        if (infix[i] == '\n' || infix[i] == ' ')
            continue;
        else if (inputType(infix[i]) == 'n') //If operand, output to postfix immediately
            postfix[index++] = infix[i];
        else if (inputType(infix[i]) == 'l'){ //if left parentheses, push to stack immediately
            pushFront(stack, infix[i]);
        }
        else{
            if (infix[i] == ')' && (stack->numOpenParentheses)-- == 0){ //If too many closing parentheses, return current postfix expression
                *errCode = 2;
                postfix[index] = '\0';
                emptyStack(stack);
                return postfix;
            }
            while (stack->head != NULL && stackPrecendence(stack->head->value) >= inputPrecedence(infix[i])){ //Pop off elements until input precedence is > stack precendence
                front = popFront(stack);
                if (front != '(')
                    postfix[index++] = front; //Add element from stack to postfix if it is not a opening parentheses
                else
                    break; //Break once we hit an opening parentheses
            }
            if (infix[i] != ')')
                pushFront(stack, infix[i]); //once the input precendence is greater than the top of the stack precedence, add to stack
        }
    }
    if (stack->numOpenParentheses > 0)
        *errCode = 1;
    else
        *errCode = 0;

    postfix[index] = '\0';
    return postfix;
}

/// @brief function to initialize a stack head to NULL and size 0 (does not free old nodes)
/// @param stack stack passed by reference to initialize
void charStackConstructor(struct Stack* stack){
    stack->head = NULL;
    stack->numOpenParentheses = 0;
    stack->size = 0;
}

/// @brief evaluate a postfix expression and output steps to console using a pointer to a char array of the expression along with the error code
/// @param postfix char array to dynamically allocated postfix expression
/// @param errCode integer of error code returned by creating the postfix expression
/// @return integer of final result of the evaluation
int calculatePostFix(char* postfix, int errCode){
    int length = strlen(postfix), num1, num2, result;
    struct Stack stack;
    charStackConstructor(&stack);

    for (int i = 0; i < length; i++){
        if (inputType(postfix[i]) == 'n')
            pushFront(&stack, postfix[i]);
        else{
            num2 = popFront(&stack) - '0';
            num1 = popFront(&stack) - '0';
            result = calculateSegment(num1, num2, postfix[i]);
            pushFront(&stack, result);
        }
    }
    if (errCode == 2)
        printf("Error! Too many closing parentheses in the expression.\n\n");

    emptyStack(&stack);
    return result;
}

/// @brief using line of numbers, dynamically allocate integer array and append to array of int pointers
/// @param puzzleArr array of integer pointers
/// @param maxSize passed by reference integer of max size of array of int pointers
/// @param currSize passed by reference integer of current size of array of int pointers
/// @param line char array of line to append
void addPuzzle(int*** puzzleArr, int* maxSize, int* currSize, char line[81]){
    int length = strlen(line), pos = 0;
    int* tempArr = (int*)malloc(sizeof(int) * 4);

    if (*currSize == *maxSize){
        (*puzzleArr) = (int**)realloc(*puzzleArr, sizeof(int*) * (*maxSize * 2));
        (*maxSize) *= 2;
    }

    for (int i = 0; i < length; i++){
        if (inputType(line[i]) == 'n'){
            tempArr[pos++] = line[i] - '0';
        }
    }
    (*puzzleArr)[(*currSize)++] = tempArr;
}

/// @brief free all nodes from the puzzle array and reset pointer to null and sizes to 0
/// @param puzzleArr passed by reference array of int pointers
/// @param currSize passed by reference integer of current size of array of pointers
/// @param maxSize passed by reference integer of max size of array of pointers
void clearPuzzleArr(int*** puzzleArr, int* currSize, int* maxSize){
    for (int i = 0; i < *currSize; i++){
        free((*puzzleArr)[i]);
    }
    free (*puzzleArr);
    *puzzleArr = NULL;
    *currSize = 0;
    *maxSize = 0;
}

/// @brief Open file using provided file name and add every line to the puzzle array
/// @param puzzleArr passed by reference array of int pointers
/// @param maxSize passed by reference int of max size of int pointers
/// @param currSize passed by reference int of current size of int pointers
/// @param fileName char array of file name to open
void readFile(int*** puzzleArr, int* maxSize, int* currSize, char fileName[81]){
    char line[81];
    FILE* filePtr = fopen(fileName, "r");

    if (filePtr == NULL){
        printf("error opening file\n");
        return;
    }

    while (fgets(line, 81, filePtr)){
        addPuzzle(puzzleArr, maxSize, currSize, line);
    }

    fclose(filePtr);
}

/// @brief verify if the input used all of the numbers in the integer array once and only once
/// @param nums integer array of numbers
/// @param input char array of input
/// @return integer specifying if the input was valid (1 if valid, 0 if not valid)
int usedAllNums(int* nums, char* input){
    int length = strlen(input);
    int posUsed[] = {0,0,0,0};
    int foundPos = 0;

    for (int i = 0; i < length; i++){
        if (isdigit(input[i])){
            foundPos = 0;
            for (int j = 0; j < 4; j++){ //Check if the current character is found in the nums array
                if (input[i] - '0' == nums[j] && posUsed[j] == 0){
                    posUsed[j] = 1;
                    foundPos = 1;
                    break;
                }
            }
            if (foundPos == 0){
                return 0;
            }
        }
    }
    //Verify all numbers have been used
    for (int i = 0; i < 4; i++){
        if (posUsed[i] == 0)
            return 0;
    }
    return 1;
}

/// @brief Verify that the input uses valid symbols (spaces, parentheses, acceptable operators)
/// @param input char array of input to verify
/// @return int specifying if input was valid (1 if valid, 0 if not valid)
int validSymbols(char* input){
    int length = strlen(input);

    for (int i = 0; i < length; i++){
        if (inputType(input[i]) == 'n' && !isdigit(input[i])){ //If inputType returned default and it was not a digit, return 0
            return 0;
        }
    }
    return 1;
}

/// @brief Fill the puzzle array using a character specifying what difficulty to use
/// @param puzzleArr passed by reference array of pointers to integers
/// @param maxSize passed by reference integer of max size of array
/// @param currSize passed by reference integer of current size of array
/// @param selection char specifying which file to read
void fillPuzzleArr(int*** puzzleArr, int* maxSize, int* currSize, char selection){
    clearPuzzleArr(puzzleArr, currSize, maxSize);
    (*puzzleArr) = (int**)malloc(sizeof(int*) * 4);
    *currSize = 0;
    *maxSize = 4;

    switch (selection){
        case 'E':
            readFile(puzzleArr, maxSize, currSize, "easy.txt");
            break;
        case 'M':
            readFile(puzzleArr, maxSize, currSize, "medium.txt");
            break;
        case 'H':
            readFile(puzzleArr, maxSize, currSize, "hard.txt");
            break;
        default:
            readFile(puzzleArr, maxSize, currSize, "easy.txt");
    }
}

/// @brief verify if the input has correct number of operands to operators
/// @param input char array to verify
/// @return integer specifying if the input was valid (1 if valid, 0 if not valid) and output to console if not valid
int validNumValues(char* input){
    int length = strlen(input), numOperands = 0, numOperators = 0;

    //Count number of operands and operators
    for (int i = 0; i < length; i++){
        if (isdigit(input[i]))
            numOperands++;
        else if (inputType(input[i]) == 'o')
            numOperators++;
    }
    if (numOperands == numOperators + 1)
        return 1;
    printf("Error! Too many values in the expression.\n\n");
    return 0;
}

/// @brief verify if the input has the correct symbols and used all of the numbers in order (valid symbols is checked before used all nums)
/// @param nums int array of numbers to check against
/// @param input char array of input to validate
/// @return integer specifying if the input was valid (1 if valid, 0 if not valid) and output error message to console
int validInput(int* nums, char* input){
    if (!validSymbols(input)){
        printf("Error! Invalid symbol entered. Please try again.\n\n");
        return 0;
    }
    else if (!usedAllNums(nums, input)){
        printf("Error! You must use all four numbers, and use each one only once. Please try again.\n\n");
        return 0;
    }
    return 1;
}

/// @brief play the game using the array of integer pointers and the current size
/// @param puzzleArr array of integer poitners
/// @param currSize current size of array
void play(int** puzzleArr, int currSize){
    int randIndex, errCode, result;
    char input[81];
    char* postfix;
    struct Stack stack;
    charStackConstructor(&stack);

    do{
        randIndex = rand() % currSize;
        printf("The numbers to use are: %d, %d, %d, %d.\n", puzzleArr[randIndex][0], puzzleArr[randIndex][1], puzzleArr[randIndex][2], puzzleArr[randIndex][3]);
        printf("Enter your solution: ");
        fgets(input, 81, stdin);
    }while (!validInput(puzzleArr[randIndex], input)); //While the input is not valid, generate new random index and request new input

    postfix = convertToPostfix(&stack, input, &errCode);
    result = calculatePostFix(postfix, errCode) - '0';

    if (errCode == 2 || !validNumValues(input)){ //If there was an error in the input such as invalid parentheses or incorrect number of operands to operators, return.
        free(postfix);
        return;
    }
    if (result == 24){
        printf("Well done! You are a math genius.\n\n");
    }else{
        printf("Sorry. Your solution did not evaluate to 24.\n\n");
    }

    free(postfix);
    while (stack.size > 0){
        popFront(&stack);
    }
}

/// @brief Function to set up the game using a new difficulty level and play
/// @param puzzleArr passed by reference array to integer pointers
/// @param maxSize passed by reference integer of max size of array
/// @param currSize passed by reference integer of current size of array
void setUpGame(int*** puzzleArr, int* maxSize, int* currSize){
    char input[81];
    printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
    printf("Your choice --> ");
    fgets(input, 81, stdin);

    fillPuzzleArr(puzzleArr, maxSize, currSize, input[0]);
    play(*puzzleArr, *currSize);
}

int main(){
    srand(1);
    int** puzzleArr = NULL;
    int currSize = 0, maxSize = 0;
    char input[81], choice;

    printf("Welcome to the game of TwentyFour Part Two!\n");
	printf("Use each of the four numbers shown exactly once, \n");
	printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
	printf("to yield the value twenty-four.\n");
    setUpGame(&puzzleArr, &maxSize, &currSize);

    do{
        printf("Enter: \t1 to play again,\n");
	    printf("\t2 to change the difficulty level and then play again, or\n");
	    printf("\t3 to exit the program.\n");
	    printf("Your choice --> ");
        fgets(input, 81, stdin);

        switch(input[0]){
            case '1':
                play(puzzleArr, currSize);
                break;
            case '2':
                setUpGame(&puzzleArr, &maxSize, &currSize);
                break;
            case '3':
                printf("\nThanks for playing!\n");
	            printf("Exiting...\n");
                clearPuzzleArr(&puzzleArr, &currSize, &maxSize);
                break;
            default:
                clearPuzzleArr(&puzzleArr, &currSize, &maxSize);
        }
    }while(input[0] != '3');
    return 0;
}