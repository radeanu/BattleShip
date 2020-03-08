#include "pch.h"
#include <windows.h>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int selectedDir[4];
int nrOfSelectedDir = 0;
int targetRemains = 100;
int posibleCoordinates[100];
int enemyShipsDamage[4] = {0,0,0,0};
int userShipsDamage[4] = {0,0,0,0};

int lastDamage = 0;
int enemyLastHit[2]; // 1-st elm. row, 2-nd is col
int hitDirection = 0;
int lastHitDirection = 0;
int firstSuccessHit = 0;
bool userShipDamaged = false;
bool directionHitSuccess = false;

int userBattleShipMap[10][10];
int enemyBattleShipMap[10][10];
int battleShipMap[10][10];

bool sh1ArePlaced = false, sh2ArePlaced = false, sh3ArePlaced = false, sh4ArePlaced = false;
bool userActive = false, botActive = false;

int hitDamagedShip();
int displayOptionsMenu();
int getOppositeDirection();
int getShipDirection(int coord[]);
int getNewAtackDirection(int damage);
string getCoordinatesByTarget(int target);
int getIndexOfElement(int elem, string pos);
int checkDamage(int target, string playerType);
int getTargetBasedOnDirection(int row, int col);
int selectTargetCellBasedOnPreviousHit(int target, int damage);
int deleteElementFromArray(int arr[], int length, int element);
void play();
void botMove();
void userMove();
void resetValues();
void placeMyShips();
void printDeafultMap();
void displayUserShips();
void placeShipsForUser();
void prepareBattleShipMap();
void printBattleShipMapHeader();
void printBattleShipMapFooter();
void displayBothBattleShipsMap();
void printBattleShipMapLeftSide(int value);
void switchPLayerActivity(string playerType);
void setShipsCoordinatesRandom(string playerType);
void addShipsCoordinates(int shipSize, int shipCount);
void printBattleShipMapCellContent(int value, string type);
void setShipsCoordinatesBySize(int shipSize, string playerType);
void handleCoordinatesByDirection(int direction, int shipSize, int shipCoord[]);
bool gameIsOver();
bool isFailedOption(int value);
bool shipIsDestroyed(int damage, string playerType);
bool checkIfCellsAreSeparated(int coord[], int shipSize);
bool checkCoordinatesPosition(int coord[], int shipSize, int direction, string player);

int main(void) {
	srand((unsigned int)time(0));// avoid the same random sequence on each program launch
	prepareBattleShipMap();
	setShipsCoordinatesRandom("enemy");
	placeMyShips();
	
	play();

	return 0;
}

void play() {
	bool gameOver = false;
	userActive = true;

	while (!gameOver) {
		if (userActive) { 
			userMove(); 
		} else { 
			botMove(); 
		}

		gameOver = gameIsOver();
	}

	system("cls");
	if (userActive && gameOver) {
		cout << "Congratulations, you win, see you next time!\n\n";
	}
	else {
		cout << "Game over, see you next time!\n\n";
	}
}

void userMove() {
	int target;
	bool gameOver = gameIsOver();
	system("cls");

	displayBothBattleShipsMap();
	cout << "\nType enemy cell numbers to atack: ";
	cin >> target;

	int damage = checkDamage(target, "user");

	while (userActive && damage != 0 && ! gameOver) {
		system("cls");
		displayBothBattleShipsMap();

		cout << "Continue: ";
		cin >> target;

		damage = checkDamage(target, "user");
		gameOver = gameIsOver();
	}
}

void botMove() {
	int damage;
	int randIndex = rand() % targetRemains;
	int target = posibleCoordinates[randIndex];

	targetRemains = deleteElementFromArray(posibleCoordinates, targetRemains, target);

	system("cls");
	displayBothBattleShipsMap();
	cout << "\nIt's my turn...";
	Sleep(1000);

	if (userShipDamaged) {
		target = hitDamagedShip();
		targetRemains = deleteElementFromArray(posibleCoordinates, targetRemains, target);
	}

	string coordinates = getCoordinatesByTarget(target);
	cout << "\nLet's look at the point: " << coordinates << "\n";
	Sleep(2000);
	damage = checkDamage(target, "bot");
}

void prepareBattleShipMap() {
	int nr = 1;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			userBattleShipMap[i][j] = 0;
			enemyBattleShipMap[i][j] = 0;
			battleShipMap[i][j] = nr;
			nr++;
		}
	}

	for (int k = 0; k < 100; k++) {
		posibleCoordinates[k] = k + 1;
	}
}

void placeMyShips() {

	while (!sh1ArePlaced || !sh2ArePlaced || !sh3ArePlaced || !sh4ArePlaced) {
		system("cls");
		int setShipOption = displayOptionsMenu();

		switch (setShipOption) {
		case 1:
			if (sh1ArePlaced) { break; }
			addShipsCoordinates(1, 4);
			sh1ArePlaced = true;
			break;
		case 2:
			if (sh2ArePlaced) { break; }
			addShipsCoordinates(2, 3);
			sh2ArePlaced = true;
			break;
		case 3:
			if (sh3ArePlaced) { break; }
			addShipsCoordinates(3, 2);
			sh3ArePlaced = true;
			break;
		case 4:
			if (sh4ArePlaced) { break; }
			addShipsCoordinates(4, 1);
			sh4ArePlaced = true;
			break;
		case 5:
			if (sh1ArePlaced || sh2ArePlaced || sh3ArePlaced || sh4ArePlaced) { break; }
			placeShipsForUser();
			break;
		default:
			break;
		}
	}
}

int displayOptionsMenu() {
	int option;

	cout << "Select option - set ship from:\n";

	if (sh1ArePlaced) {
		cout << "    1) 1 cell  - Are setted\n";

	}
	else {
		cout << "    1) 1 cell  - Not set yet\n";
	}

	if (sh2ArePlaced) {
		cout << "    2) 2 cell  - Are setted\n";

	}
	else {
		cout << "    2) 2 cell  - Not set yet\n";
	}

	if (sh3ArePlaced) {
		cout << "    3) 3 cell  - Are setted\n";

	}
	else {
		cout << "    3) 3 cell  - Not set yet\n";
	}

	if (sh4ArePlaced) {
		cout << "    4) 4 cell  - Is setted\n";

	}
	else {
		cout << "    4) 4 cell  - Not set yet\n";
	}

	if (!sh1ArePlaced && !sh2ArePlaced && !sh3ArePlaced && !sh4ArePlaced) {
		cout << "    5) Do it for me\n";
	}

	cout << "\nYour option - ";
	cin >> option;

	return option;
}

int hitDamagedShip() {
	int oldTarget;

	if (directionHitSuccess) {
		oldTarget = battleShipMap[enemyLastHit[0]][enemyLastHit[1]];
	} else {
		oldTarget = firstSuccessHit;
	}

	int target = selectTargetCellBasedOnPreviousHit(oldTarget, lastDamage);

	return target;
}

int selectTargetCellBasedOnPreviousHit(int target, int damage) {
	int row = enemyLastHit[0];
	int col = enemyLastHit[1];

	if (!directionHitSuccess) {
		if (hitDirection < 0) {
			hitDirection = getOppositeDirection();
			row = getIndexOfElement(firstSuccessHit, "x");
			col = getIndexOfElement(firstSuccessHit, "y");
		} else {
			hitDirection = getNewAtackDirection(damage);

			while (isFailedOption(hitDirection)) {
				hitDirection = getNewAtackDirection(damage);
			}

			selectedDir[nrOfSelectedDir] = hitDirection;
			nrOfSelectedDir++;
		}
	}
	
	int newTarget = getTargetBasedOnDirection(row, col);

	return newTarget;
}

int getOppositeDirection() {
	int oppositeHitDirection = lastHitDirection;

	switch (oppositeHitDirection) {
		case 1: oppositeHitDirection = 3; break;
		case 3: oppositeHitDirection = 1; break;
		case 2: oppositeHitDirection = 4; break;
		case 4: oppositeHitDirection = 2; break;
		default: break;
	}

	return oppositeHitDirection;
}

int getNewAtackDirection(int damage) {
	int newDirection = rand() % 4 + 1;

	while (newDirection == hitDirection) {
		newDirection = rand() % 4 + 1;
	}

	return newDirection;
}

int getTargetBasedOnDirection(int row, int col) {

	if (row == 0 && hitDirection == 1) {
		hitDirection = 3;
		row = getIndexOfElement(firstSuccessHit, "x");
	} else if (row == 9 && hitDirection == 3) {
		hitDirection = 1;
		row = getIndexOfElement(firstSuccessHit, "x");
	} else if (col == 0 && hitDirection == 4) {
		hitDirection = 2;
		col = getIndexOfElement(firstSuccessHit, "y");
	} else if (col == 9 && hitDirection == 2) {
		hitDirection = 4;
		col = getIndexOfElement(firstSuccessHit, "y");
	}

	switch (hitDirection) {
		case 1: row -= 1; break; // UP
		case 2: col += 1; break; // RIGHT
		case 3: row += 1; break; // DOWN
		case 4: col -= 1; break; // LEFT
		default: break;
	}

	return battleShipMap[row][col];
}

int checkDamage(int target, string playerType) {
	int row = getIndexOfElement(target, "x");
	int col = getIndexOfElement(target, "y");
	int damage; // damage = ship size
	
	if (playerType == "user") { damage = enemyBattleShipMap[row][col]; } 
	else { damage = userBattleShipMap[row][col]; }

	if(damage <= 0) { 
		if (userShipDamaged == true && directionHitSuccess == true) {
			lastHitDirection = hitDirection;
			hitDirection = -1;
		}

		directionHitSuccess = false;

		if (playerType == "user") {
			enemyBattleShipMap[row][col] = -2; // mark failed cells
		} else {
			userBattleShipMap[row][col] = -2;
		}

		switchPLayerActivity(playerType);

		return 0;
	} 
	
	if (playerType == "user") {
		enemyShipsDamage[damage - 1] += 1;
		enemyBattleShipMap[row][col] = -1; // mark damaged cells
		cout << "You hit me\n";
		Sleep(1500);
	} 
	
	if (playerType == "bot") {
		userShipsDamage[damage - 1] += 1;
		userBattleShipMap[row][col] = -1;

		if (damage > 1) {
			cout << "I got you :)\n";
			Sleep(2000);

			if (!userShipDamaged) {
				firstSuccessHit = target;
			}

			enemyLastHit[0] = row;
			enemyLastHit[1] = col;
			userShipDamaged = true;
			lastDamage = damage;
			if (hitDirection !=0) { directionHitSuccess = true; }
		}
	}

	if (shipIsDestroyed(damage, playerType) || damage == 1) {
		cout << "Ship destroyed!\n";
		Sleep(2000);
		resetValues();
	}

	return damage;
}

void resetValues() {
	enemyLastHit[0] = -1;
	enemyLastHit[1] = -1;
	userShipDamaged = false;
	directionHitSuccess = false;
	hitDirection = 0;
	firstSuccessHit = -1;

	for (int i = 0; i < nrOfSelectedDir; i++) {
		selectedDir[i] = 0;
	}

	nrOfSelectedDir = 0;
}

void placeShipsForUser() {
	bool itsOk = false;
	int option;
	system("cls");

	setShipsCoordinatesRandom("user");
	displayUserShips();

	while (!itsOk) {
		cout << "\nDo you agree with this option ?\n";
		cout << "	1) Yes\n";
		cout << "	2) No\n";
		cout << "Your option: ";
		cin >> option;

		if (option == 1) {
			itsOk = true;
		} else if(option == 2){
			system("cls");
			prepareBattleShipMap();
			setShipsCoordinatesRandom("user");
			displayUserShips();
		}
	}

	sh1ArePlaced = true, sh2ArePlaced = true, sh3ArePlaced = true, sh4ArePlaced = true;
}

void displayUserShips() {
	printBattleShipMapHeader();

	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 12; j++) {
			if (j == 1) { printBattleShipMapLeftSide(i); }
			else {
				int value = userBattleShipMap[i - 1][j - 2];

				switch (value)	{
					case 0: printf(" * |"); break;
					case -1: printBattleShipMapCellContent(value, "damaged"); break;
					case -2: printBattleShipMapCellContent(value, "failed"); break;
					default: printBattleShipMapCellContent(value, "visible"); break;
				}
			}
		}
		cout << "\n";
	}

	printBattleShipMapFooter();
}

void printDeafultMap() {
	int nr = 1;
	printBattleShipMapHeader();

	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 12; j++) {
			if (j == 1) {
				printBattleShipMapLeftSide(i);
			} else {
				int value = userBattleShipMap[i - 1][j - 2];

				if (value !=0 ) {
					printBattleShipMapCellContent(value, "visible");
				}
				else {
					printBattleShipMapCellContent(nr, "none");
				}
				nr++;
			}
		}
		cout << "\n";
	}

	printBattleShipMapFooter();
}

void setShipsCoordinatesRandom(string playerType) {
	// Set coordinates for ships from 1 cell | 4 - nr of ships in total
	for (int i = 0; i < 4;) {
		int col = rand() % 9; // from 0 to 9 inclusive
		int row = rand() % 9;

		int coord[2] = { row, col };
		bool isValid = checkCoordinatesPosition(coord, 1, -1, playerType);

		if (isValid) {
			if (playerType == "user") { 
				userBattleShipMap[row][col] = 1; 
			} else { enemyBattleShipMap[row][col] = 1; }
			i++;
		}
	}

	// Set coordinates for ships from 2 or more cell 
	for (int i = 4; i >= 2; i--) {
		setShipsCoordinatesBySize(i, playerType);
	}
}

void setShipsCoordinatesBySize(int shipSize, string playerType) {
	int ship[8];  // 8 - max coordinates
	int shipCount = 4 - (shipSize - 1); // get ship length(nr. of cells) based on ship size

	for (int i = 0; i < shipCount; ) {
		int direction = rand() % 4 + 1; // generate random direction up-1 right-2 down-3 left-4 

		ship[0] = rand() % 9; // set random cell [x,y]
		ship[1] = rand() % 9;

		// Append ship array with new coordonates
		handleCoordinatesByDirection(direction, shipSize, ship);

		bool isValid = checkCoordinatesPosition(ship, shipSize, direction, playerType);

		if (isValid) {
			for (int k = 0; k < shipSize * 2; k += 2) {
				if (playerType == "user") {
					userBattleShipMap[ship[k]][ship[k + 1]] = shipSize;
				} else { 
					enemyBattleShipMap[ship[k]][ship[k + 1]] = shipSize;
				}
			}

			i++;
		} 
	}
}

void handleCoordinatesByDirection(int direction, int shipSize, int shipCoord[]) {

	for (int i = 2; i < shipSize*2; i+=2) {
		switch (direction) {
			case 1: // UP
				shipCoord[i] = shipCoord[i - 2] - 1; // [2,3] => [2,3,1]
				shipCoord[i + 1] = shipCoord[i - 1]; // [2,3,1] => [2,3,1,3]
				break;
			case 2: // RIGHT
				shipCoord[i] = shipCoord[i - 2];         // [2,3] => [2,3,2]
				shipCoord[i + 1] = shipCoord[i - 1] + 1; // [2,3,2] => [2,3,2,4]
				break;
			case 3: // DOWN
				shipCoord[i] = shipCoord[i - 2] + 1;
				shipCoord[i + 1] = shipCoord[i - 1];
				break;
			case 4: // LEFT
				shipCoord[i] = shipCoord[i - 2];  
				shipCoord[i + 1] = shipCoord[i - 1] - 1;
				break;
			default:
				break;
		}
	}
}

void addShipsCoordinates(int shipSize, int shipCount) {
	int shipPosition;
	system("cls");
	printDeafultMap();

	if (shipSize == 1) {
		cout << "Press number of cell to place your ship\n";

		for (int k = 1; k <= shipCount;) {
			cout << "Ship nr." << k << ": ";
			cin >> shipPosition;

			int row = getIndexOfElement(shipPosition, "x");
			int col = getIndexOfElement(shipPosition, "y");

			if (row == -1 || col == -1) {
				cout << "Number is out of range, try again\n";
			}

			int coord[2] = { row, col };
			bool isValid = checkCoordinatesPosition(coord, 1, -1, "user");

			if (isValid) {
				userBattleShipMap[row][col] = 1;
				k++;
			} else {
				cout << "Sorry, you can't put there an ship\n";
			}
		}
	} else {
		cout << "To place an ship of 2 or more cell enter each number of cell\n";

		for (int k = 0; k < shipCount;) {
			int coord[8];
			cout << "Ship nr: " << k+1 << " from " << shipSize << " cell." << "\n";

			for (int i = 0, x = 0; i < shipSize;) {
				cout << "    " << i+1 << " cell: ";
				cin >> shipPosition;

				int row = getIndexOfElement(shipPosition, "x");
				int col = getIndexOfElement(shipPosition, "y");

				if (row == -1 || col == -1) {
					cout << "Number is out of range, try again\n";
				} else {
					coord[x] = row;
					coord[x + 1] = col;
					x += 2;
					i++;
				}
			}

			int direction = getShipDirection(coord);
			bool isValid = checkCoordinatesPosition(coord, shipSize, direction, "user");

			if (isValid) {
				for (int k = 0; k < shipSize * 2; k += 2) {
					userBattleShipMap[coord[k]][coord[k + 1]] = shipSize;
				}

				k++;
			} else {
				cout << "Sorry, this coordinates are invalid, try again :)\n";
			}
		}
	}
}

int getShipDirection(int coord[]) {
	if (coord[2] < coord[0]) {
		return 1; // up
	}

	if (coord[3] > coord[1]) {
		return 2; // right
	}

	if (coord[2] > coord[0]) {
		return 3; // down
	}

	if (coord[3] < coord[1]) {
		return 4; // left
	}

	return -1;
}

int getIndexOfElement(int elem, string pos) {

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (elem == battleShipMap[i][j]) {
				if (pos == "x") { return i; }
				if (pos == "y") { return j; }
			}
		}
	}

	return -1;
}

int deleteElementFromArray(int arr[], int length, int element){
	int i;	
	
	// Search x in array 
	for (i = 0; i < length; i++) {
		if (arr[i] == element) { break; }
	}

	// If x found in array 
	if (i < length) {
		// reduce size of array and move all elements on space ahead 
		length = length - 1;
		for (int j = i; j < length; j++) {
			arr[j] = arr[j + 1];
		}
	}

	return length;
}

string getCoordinatesByTarget(int target) {
	string label[10] = { "A","B","C","D","E","F","G","H","I","J" };
	int row = getIndexOfElement(target, "x");
	int col = getIndexOfElement(target, "y");

	return label[col] + "-" + to_string(row+1);
}

bool checkCoordinatesPosition(int coord[], int shipSize, int direction, string player) {
	int firstRow;
	int firstCol;
	int lastRow;
	int lastCol;

	if (shipSize == 1) {
		int row = coord[0];
		int col = coord[1];

		for (int i = row - 1; i <= row + 1; i++) {
			for (int j = col - 1; j <= col + 1; j++) {
				if (i >= 0 && j >= 0 && i <= 9 && j <= 9) {
					bool fail;
					if (player == "user") {
						fail = userBattleShipMap[i][j] != 0;
					} else {
						fail = enemyBattleShipMap[i][j] != 0;
					}
					if (fail) {
						return false;
					}
				}
			}
		}
	}

	if (shipSize > 1) {
		bool shipRepartitionIsOk;
		if (shipSize == 4) {
			shipRepartitionIsOk = checkIfCellsAreSeparated(coord, 5);
		}
		else {
			shipRepartitionIsOk = checkIfCellsAreSeparated(coord, shipSize);
		}

		if (!shipRepartitionIsOk) { return false; }

		if (direction == 1 || direction == 4) {
			firstRow = coord[shipSize * 2 - 2];
			firstCol = coord[shipSize * 2 - 1];
			lastRow = coord[0];
			lastCol = coord[1];
		}	else {
			firstRow = coord[0];
			firstCol = coord[1];
			lastRow = coord[shipSize * 2 - 2];
			lastCol = coord[shipSize * 2 - 1];
		}
		
		for (int i = 0; i < shipSize * 2; i++) {
			if(coord[i] < 0 || coord[i] > 9) {
				return false;
			}
		}

		for (int i = firstRow - 1; i <= lastRow + 1; i++) {
			for (int j = firstCol - 1; j <= lastCol + 1; j++) {
				if (i >= 0 && j >= 0 && i <= 9 && j <= 9) {
					bool fail;

					if (player == "user") {
						fail = userBattleShipMap[i][j] != 0;
					} else {
						fail = enemyBattleShipMap[i][j] != 0;
					}

					if (fail) {
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool checkIfCellsAreSeparated(int coord[], int shipSize) {
	for (int i = 0; i < shipSize; i+=2) {
		bool ok1 = abs(coord[i + 2] - coord[i]) == 1;
		bool ok2 = coord[i + 2] == coord[i];
		bool ok3 = abs(coord[i + 1] - coord[i + 3]) == 1;
		bool ok4 = coord[i + 1] == coord[i + 3];

		if (!((ok1 || ok3) && (ok2 || ok4))) {
			return false;
		}
	}

	return true;
}

bool shipIsDestroyed(int damage, string playerType) {
	int playerDamage;

	if (playerType == "user") { playerDamage = enemyShipsDamage[damage - 1]; }
	else { playerDamage = userShipsDamage[damage - 1]; }

	if (playerDamage%damage == 0) { return true; }

	return false;
}

bool isFailedOption(int value) {
	for (int i = 0; i < nrOfSelectedDir; i++) {
		if (selectedDir[i] == value) {
			return true;
		}
	}

	return false;
}

bool gameIsOver() {
	int usersShipsDestroyed = 0;
	int enemiesShipsDestroyed = 0;

	for (int i = 0; i < 4; i++) {
		usersShipsDestroyed += userShipsDamage[i];
		enemiesShipsDestroyed += enemyShipsDamage[i];
	}

	return usersShipsDestroyed == 20 || enemiesShipsDestroyed == 20;
}

void switchPLayerActivity(string playerType) {
	if (playerType == "user") {
		cout << "Noop, I'm not there :)\n";
		Sleep(2000);
		userActive = false;
		botActive = true;
	}
	else {
		cout << "Looks like you're not there :(\n";
		Sleep(2000);
		botActive = false;
		userActive = true;
	}
}

void printBattleShipMapHeader() {
	printf("\033[0;31m"); // Change print color to red
	cout << "_____________________________________________" << endl;
	cout << "|___|_A_|_B_|_C_|_D_|_E_|_F_|_G_|_H_|_I_|_J_|" << endl;
	printf("\033[0m"); // Change print color to default
}

void displayBothBattleShipsMap() {
	cout << "Your Map\n";
	displayUserShips();

	cout << "=============================================\n";
	cout << "Enemy Map\n";
	int nr = 1;
	printBattleShipMapHeader();

	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 12; j++) {
			if (j == 1) {
				printBattleShipMapLeftSide(i);
			}
			else {
				int val = enemyBattleShipMap[i - 1][j - 2];

				switch (val) {
					case -1:  printBattleShipMapCellContent(val, "damaged"); break;
					case -2:  printBattleShipMapCellContent(val, "failed"); break;
					default: printBattleShipMapCellContent(nr, "none"); break;
				}
				nr++;
			}
		}
		cout << "\n";
	}

	printBattleShipMapFooter();
}

void printBattleShipMapCellContent(int value, string type) {

	if (type == "hidden") { printf(" * |"); }
	if (type == "none") { printf("%3d|", value); }

	if (type == "failed") {
		printf("\033[1m\033[31m");
		printf(" - ");
		printf("\033[0m");
		printf("|");
	}

	if (type == "damaged") { 
		printf("\033[1m\033[31m");
		printf(" X "); 
		printf("\033[0m");
		printf("|");
	} else if (type == "visible") { 
		printf("\033[1;34m");
		printf(" %d ", value); 
		printf("\033[0m");
		printf("|");
	}

	printf("\033[0m");
}

void printBattleShipMapLeftSide(int value) {
	printf("\033[0;31m");
	printf("|%3d|", value);
	printf("\033[0m");
}

void printBattleShipMapFooter() {
	printf("\033[0;31m");
	cout << "_____________________________________________\n" << endl;
	printf("\033[0m");
}