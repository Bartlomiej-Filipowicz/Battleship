
#include<cstdlib>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <chrono>
#include <string>
#include <algorithm>
using namespace std;
using namespace std::chrono;


#define ARROW_UP 72
#define ARROW_DOWN 80
#define ELEMENTS_IN_MENU 3 // 3 elements: singleplayer, multiplayer, exit
#define ENTER 13

#define BOARD_SIZE 7


int iComputerBoard[BOARD_SIZE][BOARD_SIZE];
int userBoard[BOARD_SIZE][BOARD_SIZE]; // 0 - no ship, 1 - there is a ship, 2 - it was hit
int userShipsNumber[5];
bool ifFieldCheckedByComp[BOARD_SIZE][BOARD_SIZE];

// ========================================


// *************** Prototypes **********************************************

class my_user
{
public:
    void userPosition();

public:
    void showBoard(int tab[BOARD_SIZE][BOARD_SIZE], int*, int);

private:
    void usersShips(int*, int*);

private:
    int lengthValidator(int, int, int, int, int*);

public:
    void startTheGame();
};


class my_computer
{
public:
    void createComputerBoard();

private:
    void randomShips(int, int tab[BOARD_SIZE][BOARD_SIZE]);

public:
    void showComputerBoard(int tab[BOARD_SIZE][BOARD_SIZE]);

public:
    void startTheGameComp();

private:
    void saveComputerBoard(); // requires <fstream>
};


class my_exit
{
public:
    void exitMenu();

public:
    void exitCompWins();

public:
    void exitUserWins();
};


// *************** COMPUTER **********************************************

void my_computer::createComputerBoard()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            iComputerBoard[i][j] = 0;
        }
    }

    srand(time(NULL)); // requires ctime, cstdio

    for (int i = 4; i > 0; i--)
    {
        for (int j = 1; (j + i) <= 5; j++)
        {
            randomShips(i, iComputerBoard);
        }
    }

    saveComputerBoard();
}


void my_computer::randomShips(int iNumOfFields, int iComputerBoard[BOARD_SIZE][BOARD_SIZE])
{
    int iFieldX, iFieldY, iDirection; // direction: 0 - to the right, 1 downwards
    int iPom;

    while (1 == 1)
    {
        iPom = 0;

        iFieldX = rand() % BOARD_SIZE;
        iFieldY = rand() % BOARD_SIZE;

        while (iFieldX > (BOARD_SIZE - iNumOfFields))
            iFieldX--;
        while (iFieldY > (BOARD_SIZE - iNumOfFields))
            iFieldY--;

        iDirection = rand() % 2;

        if (iDirection == 0)
        {
            for (int i = 0; i < iNumOfFields; i++)
            {
                if (iComputerBoard[iFieldX + i][iFieldY] == 1)
                {
                    iPom++;
                    break;
                }
            }
        }
        else
        {
            for (int i = 0; i < iNumOfFields; i++)
            {
                if (iComputerBoard[iFieldX][iFieldY + i] == 1)
                {
                    iPom++;
                    break;
                }
            }
        }

        // fields checked
        if (iPom == 0)
        {
            if (iDirection == 0)
            {
                for (int i = 0; i < iNumOfFields; i++)
                {
                    iComputerBoard[iFieldX + i][iFieldY] = 1;
                }
            }
            else
            {
                for (int i = 0; i < iNumOfFields; i++)
                {
                    iComputerBoard[iFieldX][iFieldY + i] = 1;
                }
            }
            return;
        }
    }
}


void my_computer::showComputerBoard(int iComputerBoard[BOARD_SIZE][BOARD_SIZE])
{
    system("cls");
    cout << "============================\n";
    cout << "Plansza komputera: \n\n";
    int i, j;
    char cPom;

    cout << " \t";
    for (i = 0; i < BOARD_SIZE; i++)
    {
        cPom = i + 'a';
        cout << cPom << " ";
    }
    cout << endl;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        if (i + 1 < 10)
            cout << "0" << i + 1 << "\t";
        else
            cout << i + 1 << "\t";

        for (j = 0; j < BOARD_SIZE; j++)
        {
            switch (iComputerBoard[i][j])
            {
            case 0:
                cout << "  ";
                break;
            case 1:
                cout << "  "; // space bar because we dont know ships' positions
                break;
            case 2:
                cout << "x ";
                break;
            case 3:
                cout << "- ";
                break;
            }
        }
        cout << endl;
    }
}


void my_computer::startTheGameComp()
{
    bool ifGuessed = true;
    char cSign, cNumber;
    int iNumber, iSign, iRemainShips;
    while (ifGuessed)
    {
        cout << endl << endl;

        showComputerBoard(iComputerBoard);
        my_user pomUserBoard;
        pomUserBoard.showBoard(userBoard, userShipsNumber, 0);
        cout << "\n*********************\n";

        iRemainShips = 0;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (iComputerBoard[i][j] == 1)
                {
                    iRemainShips++;
                }
            }
        }

        if (iRemainShips)
        {
            cout << "\n\nTeraz Ty zgadnij gdzie lezy statek komputera:\n\n";
            cout << "Znak: ";
            cin >> cSign;
            cout << "Numer: ";
            cin >> cNumber;

            cout << "Wpisane pole: " << cNumber << cSign << endl;

            iSign = (int)(cSign - 'a');
            iNumber = (int)(cNumber - '1');
            if (iComputerBoard[iNumber][iSign] == 1)
            {
                cout << "Uzytkownik zgadl" << endl;
                iComputerBoard[iNumber][iSign] = 2;

                cout << endl << "Wcisnij dowolny klawisz\n";
                _getch();
            }
            else
            {
                cout << "Uzytkownik nie zgadl" << endl;
                iComputerBoard[iNumber][iSign] = 3;
                ifGuessed = false;
            }
        }
        else
        {
            my_exit pomExit;
            pomExit.exitUserWins();
        }
    }
    //============================

    cout << endl << "Wcisnij dowolny klawisz\n";
    _getch();

    my_user pomUser;
    pomUser.startTheGame();
}


void my_computer::saveComputerBoard()
{
    ofstream output("computer_board.txt");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            output << iComputerBoard[i][j];
        }
        output << endl;
    }
}
// *************** USER *************************************************


void my_user::userPosition()
{
    //int userBoard[BOARD_SIZE][BOARD_SIZE] = {0}; // 0 - no ship, 1 - there is a ship, 2 - it was hit
    //int userShipsNumber[5] = {0}; // 4x1 field, 3x2 fileds, 2x3 fields, 1x4 fields
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            userBoard[i][j] = 0;

    for (int i = 0; i < 5; i++)
        userShipsNumber[i] = 0;

    bool t = true;
    int iPositionNumber, iPositionSign, iNum1, iNum2, iSign1, iSign2;
    int iTypeOfShip;
    int iHowManyShipsRemain = 10;

    while (iHowManyShipsRemain)
    {
        system("cls");
        cout << "Witaj w trybie singleplayer. Wpisz wspolrzedne statkow (zero, numer, znak)\n\n";
        showBoard(userBoard, userShipsNumber, 1);

        cout << "\n\nWspolrzedna poczatku -> enter -> wspolrzedna konca:\n";

        usersShips(&iPositionNumber, &iPositionSign);
        iNum1 = iPositionNumber;
        iSign1 = iPositionSign;

        usersShips(&iPositionNumber, &iPositionSign);
        iNum2 = iPositionNumber;
        iSign2 = iPositionSign;

        // swap to make bigger value the first
        if (iNum1 < iNum2)
            swap(iNum1, iNum2);

        if (iSign1 < iSign2)
            swap(iSign1, iSign2);

        if ((iNum1 >= 0 && iNum1 < BOARD_SIZE && iSign1 >= 0 && iSign1 < BOARD_SIZE) &&
            (iNum2 >= 0 && iNum2 < BOARD_SIZE && iSign2 >= 0 && iSign2 < BOARD_SIZE) &&
            (iNum1 == iNum2 || iSign1 == iSign2)) // it needs to be improved eg. sign the first, next number or two times the same ship
        {
            iTypeOfShip = lengthValidator(iNum1, iNum2, iSign1, iSign2, userShipsNumber);

            if (iTypeOfShip)
            {
                userBoard[iNum1][iSign1] = 1;
                userBoard[iNum2][iSign2] = 1;

                while (iNum2 < iNum1)
                {
                    userBoard[iNum2][iSign2] = 1;
                    iNum2++;
                }
                while (iSign2 < iSign1)
                {
                    userBoard[iNum2][iSign2] = 1;
                    iSign2++;
                }
                userShipsNumber[iTypeOfShip]++;
                iHowManyShipsRemain--;
            }

        }
    }

    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            ifFieldCheckedByComp[i][j] = false;

    startTheGame();
}

void my_user::showBoard(int userBoard[BOARD_SIZE][BOARD_SIZE], int userShipsNumber[5], int iShowShipsNum)
{
    cout << "=======================\n";
    cout << "Plansza gracza\n\n";

    int i, j;
    char cPom;

    cout << " \t";
    for (i = 0; i < BOARD_SIZE; i++)
    {
        cPom = i + 'a';
        cout << cPom << " ";
    }
    cout << endl;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        if (i + 1 < 10)
            cout << "0" << i + 1 << "\t";
        else
            cout << i + 1 << "\t";

        for (j = 0; j < BOARD_SIZE; j++)
        {
            switch (userBoard[i][j])
            {
            case 0:
                cout << "  ";
                break;
            case 1:
                cout << "o ";
                break;
            case 2:
                cout << "x ";
                break;
            }
        }
        cout << endl;
    }

    if (iShowShipsNum)
    {
        for (int i = 1, j = 4; i < 5; i++, j--)
        {
            cout << endl << "Masz " << userShipsNumber[i] << " na " << j << " statkow o rozmiarze " << i << " na 1";
        }
    }
}


void my_user::usersShips(int* iPositionNumber, int* iPositionSign)
{
    int a, b;

    string choice; // requires cstring
    choice.clear();
    cin >> choice;

    a = ((int)choice[0] - '0') * 10 + (int)choice[1] - '0';
    a--;
    *iPositionNumber = a;

    b = (int)choice[2] - 'a';
    *iPositionSign = b;
}


int my_user::lengthValidator(int iNum1, int iNum2, int iSign1, int iSign2, int userShipsNumber[5])
{
    int iLength = -1;

    if (iNum1 == iNum2)
    {
        iLength = iSign1 - iSign2 + 1;
        if (userShipsNumber[iLength] == (5 - iLength) || iLength > 4)
        {
            return 0;
        }
        else
        {
            return iLength;
        }
    }
    else
    {
        iLength = iNum1 - iNum2 + 1;
        if (userShipsNumber[iLength] == (5 - iLength) || iLength > 4)
        {
            return 0;
        }
        else
        {
            return iLength;
        }
    }
}


void my_user::startTheGame()
{
    bool ifGuessed = true;
    int iSign, iNumber, iRemainShips;
    while (ifGuessed)
    {
        //system("cls");
        //cout << "Plansza gracza\n\n";

        my_computer pomCompBoard;
        pomCompBoard.showComputerBoard(iComputerBoard);
        showBoard(userBoard, userShipsNumber, 0);

        iRemainShips = 0;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (iComputerBoard[i][j] == 1)
                {
                    iRemainShips++;
                }
            }
        }

        if (iRemainShips)
        {
            do
            {
                iSign = rand() % BOARD_SIZE;
                iNumber = rand() % BOARD_SIZE;
            } while (ifFieldCheckedByComp[iNumber][iSign] == true);
            ifFieldCheckedByComp[iNumber][iSign] = true;

            cout << "\n*********************\n";
            cout << "Komputer losuje pole..." << endl;
            Sleep(2000);
            cout << "Wylosowane pole: " << (iNumber + 1) << (char)(iSign + 'a') << endl;

            if (userBoard[iNumber][iSign] == 1)
            {
                cout << "Komputer zgadl" << endl;
                userBoard[iNumber][iSign] = 2;
            }
            else
            {
                cout << "Komputer nie zgadl" << endl;
                ifGuessed = false;
            }

            cout << endl << "Wcisnij dowolny klawisz\n";
            _getch();
        }
        else
        {
            my_exit pomExit;
            pomExit.exitCompWins();
        }
    }
    //============================

    my_computer pomComp;
    pomComp.startTheGameComp();
}


// *************** EXIT *****************************************************

void my_exit::exitCompWins()
{
    system("cls");
    cout << "\aKomputer wygral\n\nWcisnij dowolny klawisz, aby wyjsc...";

    _getch();
    exit(0);
}

void my_exit::exitMenu()
{
    system("cls");
    cout << "\aWyjscie z programu...";

    Sleep(2000);
    exit(0);
}

void my_exit::exitUserWins()
{
    system("cls");
    cout << "\aUzytkownik wygral\n\nWcisnij dowolny klawisz, aby wyjsc...";

    _getch();
    exit(0);
}

//**************** INTERFACE**************************************************

class my_interface
{
public:
    void userChoice();

private:
    void display(int);
};
//------------MULTIPLAYER------------------------------------
//----------------------------------------------------------

class Board
{
public:
	char board[10][10];

	void reset()
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++) board[i][j] = '-';
		}
	}

	void display_the_board();

	void positioning();

	bool marking_ships(char, char, char, char);
};

void Board::display_the_board()
{
	//displaying the indexes of columns
	cout << "\t ";
	for (int i = 0; i < 10; i++)cout << " " << i;
	cout << endl;
	//displaying the rest of the board
	for (int i = 0; i < 10; i++)
	{
		cout << "\t" << (char)(i + 65) << " ";
		for (int j = 0; j < 10; j++) cout << board[i][j] << " ";
		cout << endl;
	}
}

bool Board::marking_ships(char row1, char column1, char row2, char column2)
{
	// checking if a different ship is at this location 
	char pom;
	if (row1 == row2)
	{

		if (column1 > column2)
		{
			pom = column1;
			column1 = column2;
			column2 = pom;
		}
		for (int i = 0; i <= column2 - column1; i++)
		{
			if (board[row1 - 65][column1 - 48 + i] == 'S')return false;
		}
		//checking if surroundings are empty
		if (row1 - 65 - 1 >= 0 && column1 - 48 - 1 >= 0)
		{
			if (board[row1 - 65 - 1][column1 - 48 - 1] == 'S') return false;
		}
		if (column1 - 48 - 1 >= 0)
		{
			if (board[row1 - 65][column1 - 48 - 1] == 'S') return false;
		}
		if (row1 - 65 + 1 <= 9 && column1 - 48 - 1 >= 0)
		{
			if (board[row1 - 65 + 1][column1 - 48 - 1] == 'S') return false;
		}
		// the other end
		if (row1 - 65 - 1 >= 0 && column2 - 48 + 1 <= 9)
		{
			if (board[row1 - 65 - 1][column2 - 48 + 1] == 'S') return false;
		}
		if (column2 - 48 + 1 <= 9)
		{
			if (board[row1 - 65][column2 - 48 + 1] == 'S') return false;
		}
		if (row1 - 65 + 1 <= 9 && column2 - 48 + 1 <= 9)
		{
			if (board[row1 - 65 + 1][column2 - 48 + 1] == 'S') return false;
		}
		//the middle of a ship
		for (int i = column1 - 48; i <= column2 - 48; i++)
		{
			if (row1 - 65 - 1 >= 0)
			{
				if (board[row1 - 65 - 1][i] == 'S') return false;
			}
			if (row1 - 65 + 1 <= 9)
			{
				if (board[row1 - 65 + 1][i] == 'S') return false;
			}
		}




	}
	else //column1 == column2
	{
		if (row1 > row2)
		{
			pom = row1;
			row1 = row2;
			row2 = pom;
		}
		for (int i = 0; i <= row2 - row1; i++)
		{
			if (board[row1 - 65 + i][column1 - 48] == 'S')return false;

		}
		//checking if surroundings are empty
		if (row1 - 65 - 1 >= 0 && column1 - 48 - 1 >= 0)
		{
			if (board[row1 - 65 - 1][column1 - 48 - 1] == 'S') return false;
		}
		if (row1 - 65 - 1 >= 0)
		{
			if (board[row1 - 65 - 1][column1 - 48] == 'S') return false;
		}
		if (row1 - 65 - 1 >= 0 && column1 - 48 + 1 <= 9)
		{
			if (board[row1 - 65 - 1][column1 - 48 + 1] == 'S') return false;
		}
		//The other end
		if (row2 - 65 + 1 <= 9 && column1 - 48 - 1 >= 0)
		{
			if (board[row2 - 65 + 1][column1 - 48 - 1] == 'S') return false;
		}
		if (row2 - 65 + 1 <= 9)
		{
			if (board[row2 - 65 + 1][column1 - 48] == 'S') return false;
		}
		if (row2 - 65 + 1 <= 9 && column1 - 48 + 1 <= 9)
		{
			if (board[row2 - 65 + 1][column1 - 48 + 1] == 'S') return false;
		}
		//the middle of a ship
		for (int i = row1 - 65; i <= row2 - 65; i++)
		{
			if (column1 - 48 - 1 >= 0)
			{
				if (board[i][column1 - 48 - 1] == 'S') return false;
			}
			if (column1 - 48 + 1 <= 9)
			{
				if (board[i][column1 - 48 + 1] == 'S') return false;
			}
		}

	}
	// marking a ship on a board
	if (row1 == row2)
	{
		if (column1 <= column2)
		{
			for (int i = 0; i <= column2 - column1; i++)
			{
				board[row1 - 65][column1 - 48 + i] = 'S';
			}

		}
		else
		{
			for (int i = 0; i <= column1 - column2; i++)
			{
				board[row1 - 65][column2 - 48 + i] = 'S';
			}
		}

	}
	else
	{
		if (row1 <= row2)
		{
			for (int i = 0; i <= row2 - row1; i++)
			{
				board[row1 - 65 + i][column1 - 48] = 'S';
			}
		}
		else
		{
			for (int i = 0; i <= row1 - row2; i++)
			{

				board[row2 - 65 + i][column1 - 48] = 'S';
			}
		}
	}

	return true;
}

void Board::positioning()
{
	char row1, column1, row2, column2;

	for (int k = 0; k < 10; k++)
	{
		cout << "------Wybierz pozycje swoich okretow------" << endl << endl << "\tTwoja flota to:\n\t- 1 czteromasztowiec\n\t";
		cout << "- 2 trojmasztowce\n\t- 3 dwumasztowce\n\t- 4 jednomasztowce\n\n";

		display_the_board();

		if (k == 0)
		{
			cout << "Podaj wspolrzedne czteromasztowca: ";
			cin >> row1 >> column1 >> row2 >> column2;

			while ((row1 != row2 && column1 != column2) || (row1 == row2 && column1 == column2) || (abs(column1 - column2) != 3 && abs(row1 - row2) != 3) || (!marking_ships(row1, column1, row2, column2)))
			{
				cout << "Zle wspolrzedne!!! Sprobuj jeszcze raz: " << endl;
				cin >> row1 >> column1 >> row2 >> column2;
			}
			cout << "poprawne wspolrzedne";
		}
		else if (k == 1 || k == 2)
		{
			cout << "Podaj wspolrzedne trojmasztowca: ";
			cin >> row1 >> column1 >> row2 >> column2;

			while ((row1 != row2 && column1 != column2) || (row1 == row2 && column1 == column2) || (abs(column1 - column2) != 2 && abs(row1 - row2) != 2) || (!marking_ships(row1, column1, row2, column2)))
			{
				cout << "Zle wspolrzedne!!! Sprobuj jeszcze raz: " << endl;
				cin >> row1 >> column1 >> row2 >> column2;
			}
			cout << "poprawne wspolrzedne";
		}
		else if (k > 2 && k < 6)
		{
			cout << "Podaj wspolrzedne dwumasztowca: ";
			cin >> row1 >> column1 >> row2 >> column2;

			while ((row1 != row2 && column1 != column2) || (row1 == row2 && column1 == column2) || (abs(column1 - column2) != 1 && abs(row1 - row2) != 1) || (!marking_ships(row1, column1, row2, column2)))
			{
				cout << "Zle wspolrzedne!!! Sprobuj jeszcze raz: " << endl;
				cin >> row1 >> column1 >> row2 >> column2;
			}
			cout << "poprawne wspolrzedne";
		}
		else
		{
			cout << "Podaj wspolrzedne jednomasztowca: ";
			cin >> row1 >> column1;

			while (!marking_ships(row1, column1, row1, column1))
			{
				cout << "Zle wspolrzedne!!! Sprobuj jeszcze raz: " << endl;
				cin >> row1 >> column1;
			}
			cout << "poprawne wspolrzedne";
		}

		//system("pause");
		system("CLS");

	}

}

bool hit_and_sunk(char my_board[10][10], char enemy_board[10][10], char row, char column)
{
	int up, down, left, right;
	up = row - 65 - 1;
	while (up >= 0 && enemy_board[up + 1][column - 48] == 'X')
	{
		if (my_board[up][column - 48] == 'S' && enemy_board[up][column - 48] != 'X') return false;
		//else if (enemy_board[up][column - 48] == 'X')*ship_counter++;
		//else break;
		up--;
	}
	down = row - 65 + 1;
	while (down <= 9 && enemy_board[down - 1][column - 48] == 'X')
	{
		if (my_board[down][column - 48] == 'S' && enemy_board[down][column - 48] != 'X') return false;
		//else if (enemy_board[down][column - 48] == 'X')*ship_counter++;
		//else break;
		down++;
	}
	//if (*ship_counter > 1) return true;

	left = column - 48 - 1;
	while (left >= 0 && enemy_board[row - 65][left + 1] == 'X')
	{
		if (my_board[row - 65][left] == 'S' && enemy_board[row - 65][left] != 'X') return false;
		//else if (enemy_board[row - 65][left] == 'X')*ship_counter++;
		//else break;
		left--;
	}
	right = column - 48 + 1;
	while (right <= 9 && enemy_board[row - 65][right - 1] == 'X')
	{
		if (my_board[row - 65][right] == 'S' && enemy_board[row - 65][right] != 'X') return false;
		//else if (enemy_board[row - 65][right] == 'X')*ship_counter++;
		//else break;
		right++;
	}
	return true;
}

void scores(int current_score)
{
	int line, i = 0;
	int tab[100];
	ifstream myfile;
	myfile.open("wyniki.txt");
	while (myfile >> line)
	{
		tab[i++] = line;
	}
	myfile.close();
	sort(tab, tab + i);
	for (int h = 0; h < 10; h++)
	{
		cout << h + 1 << ". " << tab[h] << " seconds ";
		if (tab[h] == current_score) cout << "<--- YOUR SCORE";
		cout << endl;
	}
}

int multiplayer()
{
	Board my_board1;
	Board my_board2;
	Board enemy_board1;
	Board enemy_board2;
	char row, column, odp;
	int ship_counter_total1 = 0, ship_counter_total2 = 0;

	my_board1.reset();

	cout << "\t      PLAYER1:" << endl;

	cout << "Chcesz ustawic statki recznie? wcisnij t lub n" << endl;
	cin >> odp;

	if (odp == 't')
	{
		my_board1.positioning();
	}
	else
	{
		ifstream myfile;
		myfile.open("board1-example.txt");
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++) myfile >> my_board1.board[i][j];
		}
		myfile.close();
	}

	my_board1.display_the_board();
	cout << "Teraz gracz 2 rozstawia swoje statki(gracz 1 nie patrz!!!)";

	Sleep(6000);

	system("CLS");

	my_board2.reset();

	cout << "\t      PLAYER2:" << endl;

	cout << "Chcesz ustawic statki recznie? wcisnij t lub n" << endl;
	cin >> odp;

	if (odp == 't')
	{
		my_board2.positioning();
	}
	else
	{
		ifstream myfile2;
		myfile2.open("board2-example.txt");
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++) myfile2 >> my_board2.board[i][j];
		}
		myfile2.close();
	}

	my_board2.display_the_board();

	cout << "Statki rozstawione!! Za chwile rozpocznie sie gra!!";

	system("pause");
	system("CLS");

	enemy_board1.reset();
	enemy_board2.reset();

	// Get starting timepoint 
	auto start = high_resolution_clock::now();

	while (true)
	{
		while (true)
		{
			cout << "PLAYER1 TURN" << endl;
			cout << "Moja plansza:" << endl;
			my_board1.display_the_board();
			cout << "Plansza wroga:" << endl;
			enemy_board1.display_the_board();


			cout << "Dokonaj strzalu! (podaj wspolrzedne)";
			cin >> row >> column;
			if (my_board2.board[row - 65][column - 48] == 'S')
			{
				enemy_board1.board[row - 65][column - 48] = 'X';
				if (hit_and_sunk(my_board2.board, enemy_board1.board, row, column))
				{
					cout << "Trafiony zatopiony!!" << endl;
					ship_counter_total1++;
					//-----------------------------QUICK GAME-------------------------------------
					//ship_counter_total1 = 10;
					//-------------------------------------------------------------------
					if (ship_counter_total1 == 10)
					{
						cout << endl << "--------Player 1 won!!!----------" << endl;
						// Get ending timepoint 
						auto stop = high_resolution_clock::now();
						// Get duration
						auto duration = duration_cast<seconds>(stop - start);
						cout << "Czas, w ktorym zwyciezyles: "
							<< duration.count() << " seconds" << endl << endl;
						// zapisanie wyniku do pliku
						ofstream myfile;
						myfile.open("wyniki.txt", ios_base::app);
						myfile << duration.count();
						myfile << "\n";
						myfile.close();

						scores(duration.count());

						return 0;
					}
					cout << "ship_counter_total1 = " << ship_counter_total1 << endl;
				}
				else cout << "Trafiony!!" << endl;
			}
			else
			{
				cout << "Pudlo!!" << endl;
				enemy_board1.board[row - 65][column - 48] = '*';
				system("pause");
				system("CLS");
				break;
			}
			system("pause");
			system("CLS");
		}

		cout << "Teraz kolej na gracza 2" << endl << "1..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 2" << endl << "2..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 2" << endl << "3..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 2" << endl << "4..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 2" << endl << "5..";
		Sleep(1000);
		system("CLS");

		//--------player2
		while (true)
		{
			cout << "PLAYER2 TURN" << endl;
			cout << "Moja plansza:" << endl;
			my_board2.display_the_board();
			cout << "Plansza wroga:" << endl;
			enemy_board2.display_the_board();


			cout << "Dokonaj strzalu! (podaj wspolrzedne)";
			cin >> row >> column;
			if (my_board1.board[row - 65][column - 48] == 'S')
			{
				enemy_board2.board[row - 65][column - 48] = 'X';
				if (hit_and_sunk(my_board1.board, enemy_board2.board, row, column))
				{
					cout << "Trafiony zatopiony!!" << endl;
					ship_counter_total2++;

					if (ship_counter_total2 == 10)
					{
						cout << endl << "--------Player 2 won!!!----------" << endl;
						// Get ending timepoint 
						auto stop = high_resolution_clock::now();
						// Get duration
						auto duration = duration_cast<seconds>(stop - start);
						cout << "Czas, w ktorym zwyciezyles: "
							<< duration.count() << " seconds" << endl << endl;
						// zapisanie wyniku do pliku
						ofstream myfile;
						myfile.open("wyniki.txt", ios_base::app);
						myfile << duration.count();
						myfile << "\n";
						myfile.close();

						scores(duration.count());

						return 0;
					}
					cout << "ship_counter_total2 = " << ship_counter_total2 << endl;
				}
				else cout << "Trafiony!!" << endl;
			}
			else
			{
				cout << "Pudlo!!" << endl;
				enemy_board2.board[row - 65][column - 48] = '*';
				system("pause");
				system("CLS");
				break;
			}
			system("pause");
			system("CLS");
		}

		cout << "Teraz kolej na gracza 1" << endl << "1..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 1" << endl << "2..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 1" << endl << "3..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 1" << endl << "4..";
		Sleep(1000);
		system("CLS");

		cout << "Teraz kolej na gracza 1" << endl << "5..";
		Sleep(1000);
		system("CLS");

		//break;
	}
	return 0;
}


void my_interface::userChoice()
{
    int iPosition = 1;
    char cKey;

    while (true)
    {
        // display
        display(iPosition);

        // get user's key
        cKey = _getch(); // getch() requires conio.h
        switch (cKey)
        {
        case ARROW_DOWN:
            if (iPosition == ELEMENTS_IN_MENU)
                iPosition = 1;
            else
                iPosition++;
            break;
        case ARROW_UP:
            if (iPosition == 1)
                iPosition = ELEMENTS_IN_MENU;
            else
                iPosition--;
            break;
        case ENTER:
            switch (iPosition)
            {
            case 1:
            {
                my_computer pomComputer;
                pomComputer.createComputerBoard();

                my_user pomUser;
                pomUser.userPosition();

                return;
            }
            case 2:
            {
                multiplayer();
                return;
            }
            case 3:
            {
                my_exit pomExit;
                pomExit.exitMenu();
                return;
            }
            }
            break;
        }
    }
}


void my_interface::display(int iPosition)  // requires windows.h
{
    system("cls");
    cout << "Statki \n\n";
    switch (iPosition)
    {
    case 1:
        cout << "\n\t\t\t\t>>>\t\tTryb Singleplayer\t\t<<<\n\t\t\t\tTryb Multiplayer\n\t\t\t\tWyjscie\n";
        break;
    case 2:
        cout << "\n\t\t\t\tTryb Singleplayer\n\t\t\t\t>>>\t\tTryb Multiplayer\t\t<<<\n\t\t\t\tWyjscie\n";
        break;
    case 3:
        cout << "\n\t\t\t\tTryb Singleplayer\n\t\t\t\tTryb Multiplayer\n\t\t\t\t>>>\t\tWyjscie\t\t\t\t<<<\n";
        break;
    }
}



// *************************** MAIN *********************************
int main()
{
    my_interface pom;
    pom.userChoice();

    return 0;
}
