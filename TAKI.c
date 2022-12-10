/*****************
Name: Lior Kashi
I.D:  209057025
*****************/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_NAME_LEN 20
#define START_CARDS_NUM 2
#define MAX_NAME_OF_SPECIAL_NUM 7
#define NUMBER_OF_CARD_TYPES 14
#define MAX_NAME_OF_SPECIAL_NUM_PLUS 15

#define TYP_TAKI 10
#define TYP_REDIRECTION 11
#define TYP_PLUS 12
#define TYP_STOP 13
#define TYP_COLOR 14

typedef struct card
{
	char color;
	int num;
	char specialNum[MAX_NAME_OF_SPECIAL_NUM];
	bool isSpecialCard;
	bool isBeenColorChanged;
} CARD;

typedef struct player
{
	char name[MAX_NAME_LEN];
	CARD* playerCards;
	int numOfCards;
	int physicSize;
	bool isChangeDirectionOn;

} PLAYER;

typedef struct stat
{
	int cardType;
	int count;
	char specialNum[MAX_NAME_OF_SPECIAL_NUM_PLUS];
} STAT;

STAT statArr[NUMBER_OF_CARD_TYPES];

void showCard(CARD nCard);
char randomChar();
int randomNum();
CARD showFirstUpperCard();
void startCards(PLAYER* players, int numOfPlayers);
void isAllocationSucceedPLAYER(PLAYER arr[]);
void isAllocationSucceedCARDS(CARD arr[]);
void startCards(PLAYER* players, int numOfPlayers);
void allocateStartCards(PLAYER* players, int numOfPlayers);
void printCardsOfPlayer(PLAYER* players, int numOfCards, int playerNumber);
bool ThereIsAWinner(char* name, int numOfCards);
CARD* TakeFromDeck(int playerNumber, PLAYER* players);
bool isLegalMove(CARD playerCard, CARD currentCard);
CARD* removeCard(PLAYER* players, int playerNumber, CARD cardToRemove, int cardIndexToRemove);
CARD updateUpperCard(CARD newCard);
CARD randomCard(int num);
CARD colorTypeAction(CARD currentCard);
void showColorCardType(CARD nCard);
CARD takiTypeAction(PLAYER* players, CARD currentCard, PLAYER currentPlayer, int playerNumber, int cardChose, int* numOfCards);
void printColorCardThatChangedByPlayer(CARD nCard);
bool checkInputValidity(int num, PLAYER currentPlayer);
int indexOfRedirectionTypeCard(int numberOfPlayers, PLAYER* players);
bool isThereRedirectionTypeCard(int numberOfPlayers, CARD currentCard);
void isAllocationSucceedSTATS(CARD arr[]);
void finalStat(PLAYER* players, int numOfPlayers, STAT* staticArr);
void numberToStrSpecial(int num, STAT statArr);
void mergeSort(STAT arr[], int size);
STAT* merge(STAT a1[], int n1, STAT a2[], int n2);
void printArray(STAT arr[], int size);
void copyArr(STAT dest[], STAT src[], int size);
CARD actValidCard(int numOfPlayers, CARD currentCard, PLAYER* players, int* i, int moveOption, bool* clockwise);
void manageLoopIndex(bool clockwise, int* i, int numOfPlayers);

void main()
{
	srand(time(NULL));
	PLAYER* players;
	CARD currentCard;
	int numOfPlayers, i, moveOption;
	bool clockwise=true;

	for (i=0; i<NUMBER_OF_CARD_TYPES;i++)
	{
		statArr[i].count = 0;
	}

	printf("~ Welcome to TAKI game ~\nPlease enter number of players: ");
	scanf("%d", &numOfPlayers);
	players = (PLAYER*)malloc(sizeof(PLAYER) * numOfPlayers);
	isAllocationSucceedPLAYER(players);

	for (i = 0; i < numOfPlayers; i++)
	{
		printf("Enter the first name of player #%d\n", i+1);
		scanf("%s", players[i].name);
	}
	currentCard= showFirstUpperCard();
	allocateStartCards(players, numOfPlayers);
	startCards(players, numOfPlayers);
	i = 0;
		for (i = 0; i < numOfPlayers; i++)
		{
			
			printf("\n%s's turn: \n", players[i].name);
			printCardsOfPlayer(players, players[i].numOfCards, i);
			printf("\nPlease enter 0 if you want to take a card from the deck\nor 1-%d if you want to put one of your cards in the middle:\n", players[i].numOfCards);
			scanf("%d", &moveOption);

			if (moveOption==0) // Taking a card from deck
			{
				players[i].playerCards = TakeFromDeck(i, players);
				printf("Upper card:\n");
				showCard(currentCard);
			}
			else if (!checkInputValidity(moveOption, players[i])) // Invalid choice
			{
				printf("Invalid choice! Try again.\n");
				printf("Upper card:\n");
				showCard(currentCard);
				i--;
			}
			else if (!isLegalMove(players[i].playerCards[moveOption - 1], currentCard)) // Illegal move
			{
				printf("Illegal move! Try again.\n");
				printf("Upper card:\n");
				showCard(currentCard);
				i--;
			}
			// Valid
			else 
				currentCard = actValidCard(numOfPlayers ,currentCard, players, &i,moveOption,&clockwise);

			if (ThereIsAWinner(players[i].name, players[i].numOfCards))
				break;

			manageLoopIndex(clockwise, &i,numOfPlayers);
		}

	finalStat(players, numOfPlayers, statArr);
}

void manageLoopIndex(bool clockwise, int* i, int numOfPlayers)
{
	if (*i == numOfPlayers - 1)
	{
		if (clockwise)
			*i = -1;
		else
			*i = *i - 2;
	}
	else if (*i == 0)
	{
		if (!clockwise)
			*i = numOfPlayers - 2;
	}
	else
	{
		if (!clockwise)
			*i = *i - 2;
	}
}
CARD actValidCard(int numOfPlayers, CARD currentCard, PLAYER* players, int* i, int moveOption, bool* clockwise)
{
	if (isLegalMove(players[*i].playerCards[moveOption - 1], currentCard))
	{
		currentCard = players[*i].playerCards[moveOption - 1];
		if (currentCard.num==TYP_COLOR) // It's a color card
		{
			printf("Upper card:\n");
			showCard(currentCard);
			currentCard = colorTypeAction(currentCard);
			players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);
			showColorCardType(currentCard);
		}

		else if (currentCard.num==TYP_TAKI) // It's a TAKI card
		{
			currentCard = players[*i].playerCards[moveOption - 1];
			players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);
			printf("Upper card:\n");
			showCard(currentCard);
			currentCard = takiTypeAction(players, currentCard, players[*i], *i, moveOption, &players[*i].numOfCards);
		}

		else if (currentCard.num==TYP_PLUS) // It's a plus card
		{
			if (players[*i].numOfCards == 1) // Plus is the last card
			{
				currentCard = players[*i].playerCards[moveOption - 1];
				players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);
				printf("Upper card:\n");

				showCard(currentCard);

				printf("\np#%s's turn: \n", players[*i].name);


				players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);
				players[*i].playerCards = TakeFromDeck(i, players);
			}
			else
			{
				currentCard = players[*i].playerCards[moveOption - 1];
				printf("Upper card:\n");
				showCard(currentCard);
				players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);
				(*i)--;
			}
		}
		else if (currentCard.num==TYP_STOP) // It's a stop card
		{
			currentCard = players[*i].playerCards[moveOption - 1];
			printf("Upper card:\n");
			showCard(currentCard);
			players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);

			if (*i == numOfPlayers - 1) // If the last player puts it
				*i = 0; // Start from the second player
			else
				(*i)++;// Skip
			if (players[*i].numOfCards == 0) // If the last card was 'stop'
				players[*i].playerCards = TakeFromDeck(*i, players);

		}

		else if (currentCard.num==TYP_REDIRECTION) // It's redirection card
		{
			*clockwise = !(*clockwise);
			currentCard = players[*i].playerCards[moveOption - 1];
			printf("Upper card:\n");
			showCard(currentCard);
			players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);
		}

		else // It's just an original card... (1-9)
		{
			currentCard = players[*i].playerCards[moveOption - 1];
			printf("Upper card:\n");
			showCard(currentCard);
			players[*i].playerCards = removeCard(players, *i, players[*i].playerCards[moveOption - 1], moveOption - 1);
		}

	}
	return currentCard;
}
void countStats(STAT* staticArr, CARD currentCard) // Counting for statistics
{
		switch (currentCard.num)
		{
		case (1):
			staticArr[currentCard.num].count++;
			break;
		case (2):
			staticArr[currentCard.num].count++;
			break;
		case (3):
			staticArr[currentCard.num].count++;
			break;
		case (4):
			staticArr[currentCard.num].count++;
			break;
		case (5):
			staticArr[currentCard.num].count++;
			break;
		case (6):
			staticArr[currentCard.num].count++;
			break;
		case (7):
			staticArr[currentCard.num].count++;
			break;
		case (8):
			staticArr[currentCard.num].count++;
			break;
		case (9):
			staticArr[currentCard.num].count++;
			break;
		}
}
void numberToStrSpecial(int num, STAT statArr) // Translate number to card type
{
	switch (num)
	{
	case(10):
		strcpy(statArr.specialNum, "TAKI");
	case (11):
		strcpy(statArr.specialNum, "<->");
	case (12):
		strcpy(statArr.specialNum, "+");
	case (13):
		strcpy(statArr.specialNum, "STOP");
	case (14):
		strcpy(statArr.specialNum, "COLOR");
	}
}
void finalStat(PLAYER* players, int numOfPlayers, STAT* statArr) // Showing statistics of a game
{
	printf("\n\n\n~ Game Statistics ~\nCard # | Frequency\n__________________\n");
	int i;

	mergeSort(statArr, NUMBER_OF_CARD_TYPES);
	printArray(statArr, NUMBER_OF_CARD_TYPES);
}
bool isThereRedirectionTypeCard(int numberOfPlayers, CARD currentCard) // Check if it is a redirection
{
	if (strcmp(currentCard.specialNum, "<->") == 0)
		return true;
	return false;
}
int indexOfRedirectionTypeCard(int numberOfPlayers, PLAYER* players) // Find index of a player's redirection card
{
	int i;
	for (i = 0; i < numberOfPlayers; i++)
	{
		if (strcmp(players[i].playerCards->specialNum, "<->") == 0)
			return i;
	}
	return -1; //none
}
bool checkInputValidity(int num, PLAYER currentPlayer) // Check if player puts the right number of a card (from those he have)
{
	if ((num >= 0) && (num <= currentPlayer.numOfCards))
		return true;
	return false;

}
void printColorCardThatChangedByPlayer(CARD nCard) // If player used a "COLOR" card type- its needs special print
{
	printf("\n*********\n*       *\n* %s *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, nCard.color);
}
CARD takiTypeAction(PLAYER* players, CARD currentCard, PLAYER currentPlayer, int playerNumber, int cardChose, int* numOfCards) // Actions of "TAKI" card
{
	if (players[playerNumber].numOfCards == 0)
	{
		return currentCard;
	}
	
	while ((players[playerNumber].numOfCards!=0))
	{
		printf("\n%s's turn:\n\n", currentPlayer.name);
		printCardsOfPlayer(players, players[playerNumber].numOfCards, playerNumber);
		printf("Please enter 0 if you want to take a card from deck and finish your turn.\n");
		printf("or 1-%d if you want to put one of your cards in the middle:\n", currentPlayer.numOfCards);
		scanf("%d", &cardChose);
		
		if (cardChose == 0) // Stop taki
		{
			players[playerNumber].playerCards = TakeFromDeck(playerNumber, players);
			printf("\nUpper card:\n");
			showCard(currentCard);
			break;
		}
		else if (!checkInputValidity(cardChose, currentPlayer))
		{
			printf("Invalid choice! Try again.\n");
		}
		else if (players[playerNumber].playerCards[cardChose - 1].color != currentPlayer.playerCards[cardChose - 1].color) // Invalid
		{
			printf("Invalid card! Try again.\n");
		}
		else // Valid
		{
			currentCard = players[playerNumber].playerCards[cardChose - 1];
			printf("\nUpper card:\n");
			players[playerNumber].playerCards = removeCard(players, playerNumber, players[playerNumber].playerCards[cardChose - 1], cardChose - 1);
			showCard(currentCard);
		}
	}
	return currentCard;
}
void showColorCardType(CARD nCard) // Printing "COLOR" card type
{
	if (nCard.isBeenColorChanged)
		printf("\n*********\n*       *\n* %s *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, nCard.color);
	else 
		printf("\n*********\n*       *\n* %s *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, ' ');
	
}
CARD colorTypeAction(CARD currentCard) // Actions of "COLOR" card type
{
		int chose=0 ;
		printf("\nPlease enter your color choice:\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
		scanf("%d", &chose);
		if (!(chose >= 1) && (chose <= 4)) // Invalid
		{
			while (!(chose >= 1) && (chose <= 4))
			{
				printf("Invalid color! Try again.\n");
				printf("\nPlease enter your color choice:\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
				scanf("%d", chose);
			}
			
		}	
		currentCard.num = 14; // number of color type card
		strcpy(currentCard.specialNum, "COLOR");
		currentCard.isSpecialCard = true;
		currentCard.isBeenColorChanged = true;
		switch (chose)
		{
		case (1):
			currentCard.color = 'Y';
			return currentCard;
		case (2):
			currentCard.color = 'R';
			return currentCard;
		case (3):
			currentCard.color = 'B';
			return currentCard;
		case (4):
			currentCard.color = 'G';
			return currentCard;
		}
}
CARD* removeCard(PLAYER* players, int playerNumber, CARD cardToRemove, int cardIndexToRemove) // Deleting a card from player's cards array
{
	int i;
	CARD* tmp;
	tmp = (CARD*)malloc(sizeof(CARD) * ((players[playerNumber].numOfCards) - 1));
	isAllocationSucceedCARDS(tmp);
	for (i = 0; i < players[playerNumber].numOfCards; i++)
	{
		if (cardIndexToRemove == 0) // 
		{
			if (i == cardIndexToRemove) // Move index
			{
				i++;
				if (players[playerNumber].playerCards[i].isSpecialCard)
				{
					tmp[i - 1].isSpecialCard = true;
					strcpy(tmp[i - 1].specialNum, players[playerNumber].playerCards[i].specialNum);
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
				}
				else
				{
					tmp[i - 1].isSpecialCard = false;
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
					tmp[i - 1].num = players[playerNumber].playerCards[i].num;
				}
			}
			else // continue copy
			{
				if (players[playerNumber].playerCards[i].isSpecialCard)
				{
					tmp[i - 1].isSpecialCard = true;
					strcpy(tmp[i - 1].specialNum, players[playerNumber].playerCards[i].specialNum);
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
				}
				else
				{
					tmp[i - 1].isSpecialCard = false;
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
					tmp[i - 1].num = players[playerNumber].playerCards[i].num;
				}
			}
		}
		else if (cardIndexToRemove == players[playerNumber].numOfCards - 1) // Card to remove is last 
		{
			if (i == cardIndexToRemove) //done
			{
				break;
			}
			else // Continue copy
			{
				if (players[playerNumber].playerCards[i].isSpecialCard)
				{
					tmp[i].isSpecialCard = true;
					strcpy(tmp[i].specialNum, players[playerNumber].playerCards[i].specialNum);
					tmp[i].color = players[playerNumber].playerCards[i].color;
				}
				else
				{
					tmp[i].isSpecialCard = false;
					tmp[i].color = players[playerNumber].playerCards[i].color;
					tmp[i].num = players[playerNumber].playerCards[i].num;
				}
			}
		}
		else // i != lastCard != first card 
		{
			if (i == cardIndexToRemove)
			{
				i++;
				if (players[playerNumber].playerCards[i].isSpecialCard)
				{
					tmp[i - 1].isSpecialCard = true;
					strcpy(tmp[i - 1].specialNum, players[playerNumber].playerCards[i].specialNum);
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
				}
				else
				{
					tmp[i - 1].isSpecialCard = false;
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
					tmp[i - 1].num = players[playerNumber].playerCards[i].num;
				}
			}
			else if (i < cardIndexToRemove)
			{
				if (players[playerNumber].playerCards[i].isSpecialCard)
				{
					tmp[i].isSpecialCard = true;
					strcpy(tmp[i].specialNum, players[playerNumber].playerCards[i].specialNum);
					tmp[i].color = players[playerNumber].playerCards[i].color;
				}
				else
				{
					tmp[i].isSpecialCard = false;
					tmp[i].color = players[playerNumber].playerCards[i].color;
					tmp[i].num = players[playerNumber].playerCards[i].num;
				}
			}
			else if (i > cardIndexToRemove)
			{
				if (players[playerNumber].playerCards[i].isSpecialCard)
				{
					tmp[i - 1].isSpecialCard = true;
					strcpy(tmp[i - 1].specialNum, players[playerNumber].playerCards[i].specialNum);
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
				}
				else
				{
					tmp[i - 1].isSpecialCard = false;
					tmp[i - 1].color = players[playerNumber].playerCards[i].color;
					tmp[i - 1].num = players[playerNumber].playerCards[i].num;
				}
			}

		}

		
	}
	(players[playerNumber].numOfCards)--;
	free(players[playerNumber].playerCards);
	return tmp;
}
CARD updateUpperCard(CARD newCard) // Updating the upper card
{
	CARD cur;
	if (newCard.isSpecialCard)
	{
		cur.color = newCard.color;
		strcpy(cur.specialNum, newCard.specialNum);
		cur.isSpecialCard = true;
	}
	else
	{
		cur.color = newCard.color;
		cur.num = newCard.num;
		cur.isSpecialCard = false;
	}
	
	return cur;
}
bool isLegalMove(CARD playerCard, CARD currentCard) // Checking if the player puts a legal card- right color/ number
{
	if (playerCard.isSpecialCard)
	{
		if ((strcmp(playerCard.specialNum, "color") == 0) || (playerCard.color == currentCard.color) || 
			(strcmp(playerCard.specialNum, currentCard.specialNum) == 0))  // If the card to put is COLOR type
			return true;
	}
	else
	{
		if ((playerCard.color == currentCard.color) || (playerCard.num == currentCard.num))
			return true;
	}
	return false;
}
CARD* TakeFromDeck(int playerNumber, PLAYER* players) // Adds a card to the cards array
{
	int i;
	int rndNum = randomNum();
	if (players[playerNumber].physicSize > players[playerNumber].numOfCards) // There is space
	{
		players[playerNumber].playerCards[players[playerNumber].numOfCards]=randomCard(rndNum);
		players[playerNumber].numOfCards++;
		return players[playerNumber].playerCards;
	}
	else
	{
		CARD* tmp;
		tmp = (CARD*)malloc(sizeof(CARD) * 2 * ((players[playerNumber].numOfCards)));		// Add more space
		isAllocationSucceedCARDS(tmp);
		players[playerNumber].physicSize * 2;
		for (i = 0; i < players[playerNumber].numOfCards; i++)
		{
			tmp[i] = players[playerNumber].playerCards[i];
		}
		tmp[players[playerNumber].numOfCards]= randomCard(randomNum());
		//countStats(statArr, tmp[players[playerNumber].numOfCards]);
		players[playerNumber].numOfCards++;
		return tmp;
	}
	
}
bool ThereIsAWinner(char* name, int numOfCards) // Check if there's a winner in the game right now
{
	if (numOfCards == 0)
	{
		printf("The winner is... player number %s", name);
		return true;
	}	
	else
		return false;
}
void allocateStartCards(PLAYER* players, int numOfPlayers) // Allocating player cards array
{
	int i;
	for (i = 0; i < numOfPlayers; i++)
	{
		players[i].playerCards = (CARD*)malloc(sizeof(CARD) * START_CARDS_NUM);
		isAllocationSucceedCARDS(players[i].playerCards);
	}
	
}
void startCards(PLAYER* players, int numOfPlayers) // Copy into the players their first cards
{
	int i, j, rndNum;
	for (i = 0; i < numOfPlayers; i++)
	{
		for (j = 0; j < START_CARDS_NUM; j++)
		{
			rndNum = randomNum();
			players[i].playerCards[j] = randomCard(rndNum);
			players[i].numOfCards = START_CARDS_NUM;
			players[i].isChangeDirectionOn = false;
		}
	}
}
void printCardsOfPlayer(PLAYER* players, int numOfCards, int playerNumber) // Printing cards of a player
{
	
	int j;
		for (j = 0; j < numOfCards; j++)
		{
			printf("\nCard #%d", j+1);
			if (players[playerNumber].playerCards[j].isBeenColorChanged)
				players[playerNumber].playerCards[j].isBeenColorChanged = false;
			showCard(players[playerNumber].playerCards[j]);
		}
}
CARD showFirstUpperCard() // Printing the first upper card (start card)
{
	CARD cur;
	printf("\nUpper card:");
	cur = randomCard(randomNum());
	while (cur.isSpecialCard)
		cur = randomCard(randomNum());
	showCard(cur);
	return cur;
}
void showCard(CARD nCard) // Printing a card
{
	if (nCard.isSpecialCard)
	{
		if ((0 == strcmp(nCard.specialNum,"color")) && (!nCard.isBeenColorChanged))
			printf("\n*********\n*       *\n* %s *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, ' ');
		else if ((0==strcmp(nCard.specialNum, "color")))
			printf("\n*********\n*       *\n* %s *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, nCard.color);
		else if (0==strcmp(nCard.specialNum, "TAKI"))
			printf("\n*********\n*       *\n*  %s *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, nCard.color);
		else if (0==strcmp(nCard.specialNum, "<->"))
			printf("\n*********\n*       *\n*  %s  *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, nCard.color);
		else if (0==strcmp(nCard.specialNum, "+"))
			printf("\n*********\n*       *\n*   %s   *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, nCard.color);
		else if (0==strcmp(nCard.specialNum, "stop"))
			printf("\n*********\n*       *\n*  %s *\n*   %c   *\n*       *\n*********\n", nCard.specialNum, nCard.color);
		
	}
	else
	{
		printf("\n*********\n*       *\n*   %d   *\n*   %c   *\n*       *\n*********\n", nCard.num, nCard.color);
	}
	
}

CARD randomCard(int num) // Making new random card
{
	CARD nCard;
	if (num <= 9) // Original card (num)
	{
		nCard.isSpecialCard = false;
	}
	else // Special card (num)
	{
		nCard.isSpecialCard = true;
		switch (num)
		{
		case (10):
			strcpy(nCard.specialNum, "TAKI");
			break;
		case (11):
			strcpy(nCard.specialNum, "<->");
			break;
		case (12):
			strcpy(nCard.specialNum, "+");
			break;
		case (13):
			strcpy(nCard.specialNum, "stop");
			break;
		case (14):
			strcpy(nCard.specialNum, "color");
			nCard.isBeenColorChanged = false;
			nCard.color = ' ';
			break;
		}
	}
	nCard.num = num;
	nCard.color = randomChar();
	(statArr[num].count)++;
	return nCard;
}
int randomNum() // Returning random int (1-14)
{
	int num;
	num = (1 + rand() % 14);
	return num;
}
char randomChar() // Returning random char G/R/Y/B
{
	int num = rand() % 4;
	switch (num)
	{
	case 0:
		return 'G';
	case 1:
		return 'R';
	case 2:
		return 'Y';
	case 3:
		return 'B';
	}
}

void isAllocationSucceedSTATS(CARD arr[]) // Check allocate
{
	if (arr == NULL) // Allocation didn't succeed
	{
		printf("Memory allocation failed.\n");
		exit(1);
	}
}
void isAllocationSucceedPLAYER (PLAYER arr[]) // Check allocate
{
	if (arr == NULL) // Allocation didn't succeed
	{
		printf("Memory allocation failed.\n");
		exit(1);
	}
}
void isAllocationSucceedCARDS(CARD arr[]) // Check allocate
{
	if (arr == NULL) // Allocation didn't succeed
	{
		printf("Memory allocation failed.\n");
		exit(1);
	}
}

void mergeSort(STAT arr[], int size) // Sort and merge array
{
	int i;
	STAT* tmpArr = NULL;
	if (size <= 1)
		return;

	mergeSort(arr, size / 2);
	mergeSort(arr + size / 2, size - size / 2);
	tmpArr = merge(arr, size / 2, arr + size / 2, size - size / 2);

	if (tmpArr)
	{
		copyArr(arr, tmpArr, size); // copy values from tmpArr to arr
		free(tmpArr);
	}
	else
	{
		printf("Memory allocation failure!!!\n");
		exit(1);	// end program immediately with code 1 (indicating an error)
	}
}
STAT* merge(STAT a1[], int size1, STAT a2[], int size2)  // Merging two arrays into one
{
	int ind1, ind2, resInd;
	STAT* res = (STAT*)malloc(NUMBER_OF_CARD_TYPES * sizeof(STAT));

	if (res)
	{
		ind1 = ind2 = resInd = 0;
		while ((ind1 < size1) && (ind2 < size2)) 
		{
			if (a1[ind1].count <= a2[ind2].count) 
			{
				res[resInd] = a1[ind1];
				ind1++;
			}
			else 
			{
				res[resInd] = a2[ind2];
				ind2++;
			}
			resInd++;
		}

		while (ind1 < size1) 
		{
			res[resInd] = a1[ind1];
			ind1++;
			resInd++;
		}
		while (ind2 < size2) 
		{
			res[resInd] = a2[ind2];
			ind2++;
			resInd++;
		}
	}
	return res;
}
void printArray(STAT arr[], int size) // Printing array
{
	int i;
	strcpy(arr[9].specialNum, " TAKI ");
	strcpy(arr[10].specialNum, " <->  ");
	strcpy(arr[11].specialNum, "  +   ");
	strcpy(arr[12].specialNum, " STOP ");
	strcpy(arr[13].specialNum, "COLOR ");

	for (i = 0; i < 9; i++)
	{
		printf("  %d   |   %d\n", i+1, arr[i].count);
	}
		
	for (i = 9; i < 14; i++)
	{
		printf("%s|   %d\n", arr[i].specialNum, arr[i].count);
	}
		
}
void copyArr(STAT dest[], STAT src[], int size) // Copy two arrays
{
	int i;
	for (i = 0; i < size; i++)
		dest[i].count = src[i].count;
}