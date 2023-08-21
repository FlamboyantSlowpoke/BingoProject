//Benjamin Stacey G00402833


/**
 * BUGS   
 *          none :D
*/
//imports
#include<stdio.h>
#include"colour.h"
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>


//global variables
#define MAX_PLAYERS 4

#define BCARD_WIDTH 9
#define BCARD_HEIGHT 3
#define CELL_ROW_FILLED 5
#define CELL_ROW_BLANK (BCARD_WIDTH - CELL_ROW_FILLED)

#define NUM_MIN 1
#define NUM_MAX 90
#define DRAWN_MAX 1000



// structs
typedef struct CELL
{
    int number;
    bool guessed;
}CELL;

typedef struct BINGOCARD
{
    int playerNum;
    int prize;
    CELL** cells[BCARD_HEIGHT];
}BINGOCARD;



//functions
void generateRandomSeed();

void startGame(BINGOCARD* cards[], int amount);
void initialiseGame();
void newGame(int players);
bool loadGame();
bool saveGame(BINGOCARD* cards[], int numberDrawn[], int players);
void startGamewithLoadedData(BINGOCARD* cards[], int amount,  int guessed[], int length);

void mainMenu();
void gameMenu();
void loadGameMenu();

void exitNOSAVE();

int drawNum(int drawnNumbers[], int length, int min, int max);
int checkCard(BINGOCARD* bingoCard);
void updateCard(BINGOCARD* bingoCard, int numberDrawn);
void updateCards(BINGOCARD* cards[], int numberDrawn, int amount);


BINGOCARD* makeBingocard(int playerNum);
BINGOCARD* newBingoCard();

void printBingoCard(BINGOCARD* bingoCard);
void printBingoCards(BINGOCARD* cards[], int amount);
void printGameStatus(int numberDrawn[], int length, BINGOCARD* cards[], int amount);
void PrintDrawNum(int numberDrawn);

void pullPrize();
void jackPot();


//main method
int main(void)
{
    generateRandomSeed();
    //variables
    int mainMenuChoice;

    yellow();printf("--------------BINGO--------------\n\n");

    mainMenu();
    purple(); scanf("%d", &mainMenuChoice);yellow();

    //takes choice and excutes choice of function
    while(mainMenuChoice != -1)
    {
        switch (mainMenuChoice)
        {
            case 1:
            initialiseGame();
            green();printf("GAME CREATED\n");yellow();
            break;
        
            case 2:
            green();printf("LOADING SAVED GAMES\n");yellow();
            loadGame();
            break;
        }
        if(mainMenuChoice != -1 && mainMenuChoice!= 1 && mainMenuChoice != 2)
        {
            red();printf("INVALID NUMBER\n");
            break;
        }
    }

    //terminates program
    return 0;
}


//functions
BINGOCARD* newBingoCard()
{
    //allocate new card memory
    BINGOCARD* bingoCard = (BINGOCARD*) malloc(sizeof(BINGOCARD));

    bingoCard->prize = 0;

    //fill cells
    for(int y = 0; y <BCARD_HEIGHT; y++)
    {
        bingoCard->cells[y] = (CELL**) malloc(sizeof(CELL*) * BCARD_WIDTH);
        for(int x = 0; x < BCARD_WIDTH; x++)
        {
            //allocate memory per cell
            bingoCard->cells[y][x] = (CELL*) malloc(sizeof(CELL));

            //set cell variables
            bingoCard->cells[y][x]->number = 0;
            bingoCard->cells[y][x]->guessed = false;
        }
    }
    return bingoCard;
}

BINGOCARD* makeBingoCard(int playerNum)
{

    BINGOCARD* bingoCard = newBingoCard();

    // set base variables
    bingoCard->playerNum = playerNum + 1;

    // fill cells
    for (int y = 0; y < BCARD_HEIGHT; y++)
    {
        // allocate empty
        for (int i = 0; i < CELL_ROW_BLANK; i++)
        {
            // min, max location for blanks
            int min = 0;
            int max = BCARD_WIDTH - 1;

            // adjust min, max if first or last row
            if (y == 0 || y == BCARD_HEIGHT - 1)
            {
                min++;  // min = min + 1
                max--;  // max = max - 1
            }

            // get a random cell that will be empty
            int x = ((int) (rand() % max) + min);

            // check if the cell is already empty
            if (bingoCard->cells[y][x]->guessed == true)
            {
                i--;
            }
            else
            {
                // update the cell to be guessed / empty
                bingoCard->cells[y][x]->guessed = true;
            }

        }
    }

    // generate and fill cell numbers
    for (int x = 0; x < BCARD_WIDTH; x++)
    {
        // generate random numbers
        int nums[BCARD_HEIGHT];
        for (int i = 0; i < BCARD_HEIGHT; i++)
        {
            // generate a random number (between (1 - 10) + (x * 10), so 1 - 10, 11 - 20, etc)
            nums[i] =  (((int) (rand() % 9)) + 1) + (x * 10);

            // compare against others
            for (int comp_i = 0; comp_i < BCARD_HEIGHT; comp_i++)
            {
                if (i != comp_i && nums[i] == nums[comp_i])
                {
                    i--;
                }
            }
        }

        // insert numbers
        int prev = 0;

        for (int y = 0; y < BCARD_HEIGHT; y++)
        {
            int current = INT_MAX;

            for (int comp = 0; comp < BCARD_HEIGHT; comp++)
            {
                // get smallest number
                if (current > nums[comp] && nums[comp] > prev && current > prev)
                {
                    current = nums[comp];
                }
            }

            prev = current;

            // check and only update if not a blank space
            if (bingoCard->cells[y][x]->guessed == false)
            {
                bingoCard->cells[y][x]->number = current;
            }
        }
    }

    // return
    return bingoCard    ;
}

void printBingoCard(BINGOCARD* bingoCard)
{
    checkCard(bingoCard);
    printf("\nB I N G O   C A R D\n\n");

    for (int y = 0; y < BCARD_HEIGHT; y++)
    {
        for (int x = 0; x < BCARD_WIDTH; x++)
        {
           
            if (bingoCard->cells[y][x]->number == 0)
            {
                yellow();
                printf(" \t");
            }
            else
            {
                if (bingoCard->cells[y][x]->guessed == true)
                {
                    green();
                }
                else
                {
                    cyan();
                }
                printf("%d\t", bingoCard->cells[y][x]->number);
                yellow();
            }
        }

        printf("\n\n");
    }

}

void printBingoCards(BINGOCARD* cards[], int amount)
{
    yellow();printf("\n\nB I N G O   C A R D S\n\n");
    for (int i = 0; i < amount; i++)
    {
        printBingoCard(cards[i]);
    }
}

void startGameWithLoadedData(BINGOCARD* cards[], int amount, int numberDrawn[], int length)
{
    int choice = 0;
  
        // display cards
        printBingoCards(cards, amount);

        // loop choice until finished
        do
        {
            // display menu
            gameMenu();
            scanf("%d", &choice);

            // check choice
            int num;
            switch(choice)
            {
                case 1:
                    // draw number
                    num = drawNum(numberDrawn, length, NUM_MIN, NUM_MAX);
                    numberDrawn[length] = num;
                    length++;
                    updateCards(cards, amount, num);
                    PrintDrawNum(num);
                    printBingoCards(cards, amount);
                    break;
                case 2:
                     // save game
                    if (saveGame(cards, numberDrawn, amount))
                    {
                        green();printf("\nGame saved successfully!\n");yellow();
                    }
                    else
                    {
                        red();printf("\nUnable to save game!\n");yellow();
                    }
                    break;
                case 3:
                    // view statistics
                    printGameStatus(numberDrawn, length, cards, amount);
                    break;
                case 4:
                    // exit without saving 
                    exitNOSAVE();
                    break;
                default:
                    // invalid choice
                    red();printf("Invalid choice!\nPlease use an option from 1 - 4\n\n> ");yellow();
                    break;
            }
        } while (choice != 4);
}

bool loadGame()
{
    FILE *f1;
    f1 = fopen("SAVEDATA.txt", "r");
    if(f1 == NULL)
    {
        red();printf("SAVED DATA NOT FOUND\n");yellow();
        return false;
    }
    else
    {
        int players = 0;
        fscanf(f1, "%d", &players);

        int numberDrawn[DRAWN_MAX];
        int minimum = 0;
        int maximum = 0;

        for (int i = 0; i < DRAWN_MAX; i++)
        {
            fscanf(f1, "%d", &numberDrawn[i]);
            minimum++;
            if(numberDrawn[i] != 0)
            {
                maximum = minimum;
            }
        }

        //load card data
        BINGOCARD* cards[players];
        for(int p = 0; p < players; p++)
        {
            //
            BINGOCARD* bingoCard = newBingoCard();
            bingoCard->playerNum = p + 1;

            //fill cards from save data
            for (int y = 0; y < BCARD_HEIGHT; y++)
            {
                for (int x = 0; x < BCARD_WIDTH; x++)
                {
                    int gameStatus = 0;
                    fscanf(f1, "%d %d", &bingoCard->cells[y][x]->number, &gameStatus);
                    if (gameStatus == 1)
                    {
                        bingoCard->cells[y][x]->guessed = true;
                    }
                }
            }
            cards[p] = bingoCard;
        }

        //resume game with loaded data
        startGameWithLoadedData(cards, players, numberDrawn, maximum);

        //succesful method
        return true;
    }
  
}

bool saveGame(BINGOCARD* cards[], int numberDrawn[], int players)
{
    //opens file for writing
    FILE *f1;
    f1 = fopen("SAVEDATA.txt", "w");

    //returns false if file is not found
    if(f1 == NULL)
    {
        red();printf("ERROR\n");yellow();
        return false;
    }

    //takes in player number and creates a loop to save the numbers pulled
    fprintf(f1, "%d\n", players);
    for(int i = 0; i < DRAWN_MAX; i++)
    {
        fprintf(f1, "%d ", numberDrawn[i]);
    }
    fprintf(f1, "\n");

    //saves individuel player data
    for (int i = 0; i < players; i++)
    {
        for (int y = 0; y < BCARD_HEIGHT; y++)
        {
            for (int x = 0; x < BCARD_WIDTH; x++)
            {
                fprintf(f1, "%d %d ", cards[i]->cells[y][x]->number, cards[i]->cells[y][x]->guessed == true ? 1 : 0);
            }
        }
        fprintf(f1, "\n");
    }
    

    //close file and return user feedback
    fclose(f1);
    green();printf("SAVED\n");yellow();
    
    return true;
}

int drawNum(int drawnNumbers[], int length, int min, int max)
{
    do
    {
        //generate random number
        int numberDrawn = ((int) (rand() % max) + min);
        //duplicate troubleshoot function
        //printf("Drawing number: %d\n", numberDrawn);

        //reads over array
        for (int i = 0; i < length; i++)
        {
            //checks if number is already in array
            if (drawnNumbers[i] == numberDrawn)
            {
                //if number is already in array, generates a new number
                numberDrawn = -1;
                break;
            }
        }

        //return the number if number isnt reset
        if (numberDrawn >= 0)
        {
            return numberDrawn;
        }
    }while (true);
}

void PrintDrawNum(int numberDrawn)
{
    //takes number drawn and outputs a fun statement for the user
    if(numberDrawn == 9 || numberDrawn == 29 || numberDrawn == 39 || numberDrawn == 49 || numberDrawn == 59 || numberDrawn == 69 || numberDrawn == 79 || numberDrawn == 89)
    {
        yellow();printf("Always so sublime, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 8 || numberDrawn == 28 || numberDrawn == 38 || numberDrawn == 48 || numberDrawn == 58 || numberDrawn == 68 || numberDrawn == 78 || numberDrawn == 88)
    {
        yellow();printf("Looks like someone's got a date, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 7 || numberDrawn == 27 || numberDrawn == 37 || numberDrawn == 47 || numberDrawn == 57 || numberDrawn == 67 || numberDrawn == 77 || numberDrawn == 87)
    {
        yellow();printf("A stairway to heaven?? it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 6 || numberDrawn == 26 || numberDrawn == 36 || numberDrawn == 46 || numberDrawn == 56 || numberDrawn == 66 || numberDrawn == 76 || numberDrawn == 86)
    {
        yellow();printf("Devil's got a few tricks, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 5 || numberDrawn == 25 || numberDrawn == 35 || numberDrawn == 45 || numberDrawn == 55 || numberDrawn == 65 || numberDrawn == 75 || numberDrawn == 85)
    {
        yellow();printf("Doesn't matter where we drive, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 4 || numberDrawn == 24 || numberDrawn == 34 || numberDrawn == 44 || numberDrawn == 54 || numberDrawn == 64 || numberDrawn == 74 || numberDrawn == 84)
    {
        yellow();printf("Someone might go home in awe, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 3 || numberDrawn == 23 || numberDrawn == 33 || numberDrawn == 43 || numberDrawn == 53 || numberDrawn == 63 || numberDrawn == 73 || numberDrawn == 83)
    {
        yellow();printf("A few trips to the sea, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 2 || numberDrawn == 22 || numberDrawn == 32 || numberDrawn == 42 || numberDrawn == 52 || numberDrawn == 62 || numberDrawn == 72 || numberDrawn == 82)
    {
        yellow();printf("A room with a view? it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 1 || numberDrawn == 21 || numberDrawn == 31 || numberDrawn == 41 || numberDrawn == 51 || numberDrawn == 61 || numberDrawn == 71 || numberDrawn == 81)
    {
        yellow();printf("Flying too close to the sun?? it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 20 || numberDrawn == 30 || numberDrawn == 40 || numberDrawn == 50 || numberDrawn == 60 || numberDrawn == 70 || numberDrawn == 80 || numberDrawn == 90)
    {
        yellow();printf("A horn'O'Plenty, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 13 || numberDrawn == 14 || numberDrawn == 15 || numberDrawn == 16 || numberDrawn == 17 || numberDrawn == 18 || numberDrawn == 19)
    {
        yellow();printf("A winner to be foreseen, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 10 || numberDrawn == 11)
    {
        yellow();printf("Everyone get their pen, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else if(numberDrawn == 12)
    {
        yellow();printf("the wealth of an elve, it's number [");cyan();printf("%d", numberDrawn);yellow();printf("]!!\n\n");
    }
    else{
        red();printf("ERROR\n");yellow();
    }
}

void updateCard(BINGOCARD* bingoCard, int number)
{
    //reads through card to update guessed status
    for (int y = 0; y < BCARD_HEIGHT; y++)
    {
        for (int x = 0; x < BCARD_WIDTH; x++)
        {
            //check for number 
            if (bingoCard->cells[y][x]->number == number || bingoCard->cells[y][x]->number == 0)
            {
                //set the guessed status to true
                bingoCard->cells[y][x]->guessed = true;
            }
        }
    }
}

void updateCards(BINGOCARD* bingoCard[], int amount, int number)
{
    
    for (int i = 0; i < amount; i++)
    {
        updateCard(bingoCard[i], number);
    }
}

void printGameStatus(int numbers[], int length, BINGOCARD* bingoCard[], int amount)
{
    //print out numbers that have been drawn from the array
    printf("Numbers drawn: \n");
    for(int i = 0; i < length; i++)
    {
        green();printf("%d, ", numbers[i]);yellow();
    }
    printf("\n\n");
    //get remaining numbers for win coditions for players
    int remaining[amount];
    //check cards
    for (int i = 0; i < amount; i++)
    {
        remaining[i] = checkCard(bingoCard[i]);
    
        //display card details
        yellow();printf("\n\nPlayer: ");green();printf("%i\n", bingoCard[i]->playerNum);yellow();
        yellow();printf("Prize: ");green();printf("%i\n",bingoCard[i]->prize);yellow();
        yellow();printf("Closest remaining tiles: ");green();printf("%i\n", remaining[i]);yellow();
        printf("\n");

    }
}

void exitNOSAVE()
{
    //brings up the main menu to give players a choice in exiting or continueing games
    int exitGameChoice;
    mainMenu();
    purple();scanf("%d", &exitGameChoice);yellow();
    switch(exitGameChoice)
    {
        case -1:
        clear();
        exit(0);
        break;

        case 1:
        initialiseGame();
        break;

        case 2:
        loadGame();
        break;
    }
    
}

void initialiseGame()
{
    int players = 0;
    do 
    {
        yellow();printf("Welcome to Bingo!\n\nHow many players are there?\n");
        scanf("%d", &players);

        if (players > MAX_PLAYERS || players <= 0)
        {
            red();printf("Invalid number of players, please enter a number between 1 and 4\n");yellow();
        }

    }while (players <= 0 || players > MAX_PLAYERS);

    newGame(players);

}

void startGame(BINGOCARD* bingoCard[], int amount)
{
    //calls main game loop with an empty table
    
    int drawnNumbers[DRAWN_MAX];
    for(int i = 0; i < DRAWN_MAX; i++)
    {
        drawnNumbers[i] = 0;
    }
    
    startGameWithLoadedData(bingoCard, amount, drawnNumbers, 0);
}

//creates a new game instance with blank cards
void newGame(int players)
{

    BINGOCARD* bingoCard[players];

    //generates a card for each player specifed
    for(int i = 0; i < players; i++)
    {
        bingoCard[i] = makeBingoCard(i);
    }
    
    startGame(bingoCard, players);
    
}

void mainMenu()
{
    printf("Press 1 to make a new bingo game\n");
    printf("Press 2 to load a bingo game from previous saves\n");
    printf("Press -1 to exit program\n");
    
}

void gameMenu()
{
    printf("Press 1 to draw a number\n");
    printf("Press 2 to save the current game status\n");
    printf("Press 3 to output game statistics\n");
    printf("Press 4 to exit the game without saving\n");
}

int checkCard(BINGOCARD* bingoCard)
{
    int remaining = INT_MAX;

    //check rows
    int rows = 0;
    for (int y = 0; y < BCARD_HEIGHT; y++)
    {
        int guessed = 0;
        for (int x = 0; x < BCARD_WIDTH; x++)
        {
            if(bingoCard->cells[y][x]->guessed)
            {
                guessed++;
            }
        }

        if (guessed == BCARD_WIDTH)
        {
            rows++;
        }

        if ((BCARD_WIDTH - guessed) < remaining && (BCARD_WIDTH - guessed) > 0)
        {
            remaining = (BCARD_WIDTH - guessed);
        }

    }


    // check corners
    int corners = 0;
    if (bingoCard->cells[0][0]->guessed) { corners++; }
    if (bingoCard->cells[0][BCARD_WIDTH - 1]->guessed) { corners++; }
    if (bingoCard->cells[BCARD_HEIGHT - 1][0]->guessed) { corners++; }
    if (bingoCard->cells[BCARD_HEIGHT - 1][BCARD_WIDTH - 1]->guessed) { corners++; }

    // update remaining (unless it's 0)
    if (((4 - corners) < remaining) && (4 - corners) > 0)
    {
        remaining = (4 - corners);
    }

    // update prize
    bingoCard->prize = rows;

    if (corners >= 4)
    {
        bingoCard->prize++;
    }

    if (bingoCard->prize == 4)
    {
        jackPot();
    }

    //return the lowest remaining number
    return remaining;
}
   
void pullPrize()
{
  printf("You have won a prize!\n");
}

void jackPot()
{
  printf("You have won the jackpot!\n");
  exit(0);
}

void generateRandomSeed() 
{
    time_t t;
    srand((unsigned)time(&t));
}