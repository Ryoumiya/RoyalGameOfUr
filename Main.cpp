#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <inttypes.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <array>

//Primary Object Libarary
//#include "PrimaryObject.h"

//Type Object Defenition
constexpr auto CIRCLE = 1;
constexpr auto SQUARE = 2;
constexpr auto UNIDEF = 0;

using namespace sf;
using namespace std;

constexpr auto D_BUG = 0;
bool Ai_Enable = true;
bool canMove = true;
const String Dicenaml = "DICE = ";
const String Playername = "Player = ";
const String Winner_is = "The Winner is = ";
int WhoIsThePlayer = 2;
int RandomNumberGetPc; //Hold the random Value
char InputString[6];	//Debug Input String
bool has_anyone_win = false;
char who_is_the_Winner = '-';
bool displayBox = false;
int globalcounterAiCall = 0;

struct FloatingCoord {
	float Xcoord;
	float Ycoord;
	uint8_t Type;
	float Length;
};

class AnyObject {
protected:
	FloatingCoord Fc;
public:
	AnyObject() {
		Fc.Xcoord = 0;
		Fc.Ycoord = 0;
		Fc.Length = 1;
		Fc.Type = UNIDEF;
	};
	AnyObject(float xp, float yp) {
		SetXYcoord(xp, yp);
	}
	void SetType(uint8_t type) {
		Fc.Type = type;
	}
	void SetLength(float rp) {
		Fc.Length = rp;
	}
	float GiveLength() {
		return Fc.Length;
	}
	void setXcoord(float Xcrd) {
		Fc.Xcoord = Xcrd;
	}
	void setYcoord(float Ycrd) {
		Fc.Ycoord = Ycrd;
	}
	void SetXYcoord(float Xc,float Yc) {
		setXcoord(Xc);
		setYcoord(Yc);
	}
	float GiveXcoord() {
		return Fc.Xcoord;
	}
	float GiveYcoord() {
		return Fc.Ycoord;
	}
	FloatingCoord GiveXYD() {
		return Fc;
	}
};

class Circles : public AnyObject, public CircleShape {
protected:
	int LocationinBoard;
	char PlayerCode;
	uint8_t BoardLoc[16];
public:
	Circles() {
		SetLength(1);
		LocationinBoard = -1;
		SetType(CIRCLE);
		PlayerCode = '0';
	};
	Circles(float xd,float yd,float rd) {
		SetXYcoord(xd, yd);
		setPosition(xd, yd);
		SetLength(rd);
		setRadius(GiveLength());
		LocationinBoard = -1;
		SetType(CIRCLE);
		PlayerCode = '0';
	};
	void Setboard(uint8_t Arr[]) {
		for (uint8_t i = 0; i < 16; i++){
			BoardLoc[i] = Arr[i];
		}
	}
	void SetLocation(float xf,float yf) {
		SetXYcoord(xf, yf);
		setPosition(xf, yf);
	}
	void SetLocationF(FloatingCoord Coord) {
		SetLocation(Coord.Xcoord,Coord.Ycoord);
	}
	void SetWidth(float rd) {
		SetLength(rd);
		setRadius(GiveLength());
	}
	float GiveRadius() {
		return GiveLength();
	}
	int GetBoardLoc() {
		return LocationinBoard;
	}
	void SetAsPlayer(uint8_t PlayerNumber) {
		if (PlayerNumber == 1) {
			PlayerCode = 'A';
			BoardLoc[0] = 20;BoardLoc[1] = 3;BoardLoc[2] = 2;BoardLoc[3] = 1;
			BoardLoc[4] = 0;BoardLoc[5] = 4;BoardLoc[6] = 5;BoardLoc[7] = 6;
			BoardLoc[8] = 7;BoardLoc[9] = 12;BoardLoc[10] = 13;	BoardLoc[11] = 16;
			BoardLoc[12] = 17;BoardLoc[13] = 15;BoardLoc[14] = 14;BoardLoc[15] = 21;
		}
		else if (PlayerNumber == 2) {
			PlayerCode = 'B';
			BoardLoc[0] = 22;BoardLoc[1] = 11;BoardLoc[2] = 10;BoardLoc[3] = 9;
			BoardLoc[4] = 8;BoardLoc[5] = 4;BoardLoc[6] = 5;BoardLoc[7] = 6;
			BoardLoc[8] = 7;BoardLoc[9] = 12;BoardLoc[10] = 13;BoardLoc[11] = 16;
			BoardLoc[12] = 17;BoardLoc[13] = 19;BoardLoc[14] = 18;BoardLoc[15] = 23;
		}
		else
		{
			cout << "PLEASE RE-CHECK PLAYER NUMBER" << endl;
		}
	}
	int GiveBoardLoc(int Num) {
		LocationinBoard = Num;
		return BoardLoc[Num];
	}
	char GetPlayer() {
		return PlayerCode;
	}
	int ConstSafeBoardDirct(int Num) {
		return BoardLoc[Num];
	}
	void printCoord(int i) {
		cout << "Circle Type " << PlayerCode << ",Number = " << i << ", X = " << GiveXcoord() << ", Y = " << GiveYcoord() << ", R =  "<< GiveLength() <<endl;
	}
};

struct PatternHolding {
	bool Occupied = false;
	char PlayerName = NULL;
	int PieceNumber = NULL;
	bool StackAble = false;
	bool Special = false;
};

class Squares :public AnyObject, public RectangleShape {
protected:
	PatternHolding Values;
public:
	Squares() {
		SetLength(1);
		SetType(SQUARE);
	}
	Squares(float xd,float yd,float wdx) {
		SetXYcoord(xd, yd);
		setPosition(xd, yd);
		SetLength(wdx);
		setSize(Vector2f(wdx, wdx));
		SetType(SQUARE);
	}
	void AllowStacking() {
		Values.StackAble = true;
	}
	bool CheckStack() {
		return Values.StackAble;
	}
	void Special() {
		Values.Special = true;
	}
	bool CheckSpecial() {
		return Values.Special;
	}
	bool IsItOccupied() {
		return Values.Occupied;
	}
	void setOccupation(char playerwho, int piece_number) {
		Values.Occupied = true;
		Values.PieceNumber = piece_number;
		Values.PlayerName = playerwho;
	}
	char getOccupier() {
		return Values.PlayerName;
	}
	int getPiece() {
		return Values.PieceNumber;
	}
	void SetDim(float wdx) {
		SetLength(wdx);
		setSize(Vector2f(wdx, wdx));
	}
	float getDim() {
		return GiveLength();
	}
	void SetLocation(float xf, float yf) {
		SetXYcoord(xf, yf);
		setPosition(xf, yf);
	}
	void setTop(uint8_t x1,char Playerwho) {
		Values.PieceNumber = int(x1);
		Values.PlayerName = Playerwho;
	}//Dercibt dont use//dont use
	int GetTop() {
		//Tranlational
		if (Values.PlayerName == 'A') {
			return 1;
		}
		else {
			return 2;
		}
	}
	void ResetOccupation() {
		Values.PieceNumber = NULL;
		Values.Occupied = false;
		Values.PlayerName = NULL;
	}
	FloatingCoord GiveCircleLocation() {
		FloatingCoord Tempo;
		Tempo.Xcoord = GiveXcoord() + 25.f;
		Tempo.Ycoord = GiveYcoord() + 25.f;
		return Tempo;
	}
};

void PutIntoString(char ch) {
	for (int8_t k = 0; k < 6;k++) {
		if ((InputString[k] == '-')) {
			InputString[k] = ch;
			return;
		}
	}
	return;
}

//Get the number into a limited and long type
uint32_t RandomGet32() {
	return (uint32_t)rand();
}

//Reduce it so it can be used with a range of 0-29;
int FakeDice() {
	return (int(RandomGet32()) % 5);
}

bool is_inRangeF(float Value, float LowerBound, float UpperBound) {
	if ((Value < UpperBound) && (Value > LowerBound)) {
		return true;
	}
	else {
		return false;
	}
}

//Check if a circle
bool CircleInsed(FloatingCoord N1, FloatingCoord N2) {
	float Distance;
	float NX = N1.Xcoord - N2.Xcoord + 20.f;
	float NY = N1.Ycoord - N2.Ycoord + 20.f;
	Distance = sqrtf((NX * NX) + (NY * NY));
	float NZ = N1.Length;
	if (Distance <= NZ) {
		return true;
		if (D_BUG) {
			cout << "Is it Inside Circle ? = " << "true" << endl;
		}
	}
	else {
		return false;
	}
}

// check if the 2 object is inside of eachother
bool DistanceInside(FloatingCoord ThePrimaryItem,FloatingCoord TheCheckedItem) {
	if (ThePrimaryItem.Type == SQUARE) {
		if ((TheCheckedItem.Xcoord < (ThePrimaryItem.Xcoord + ThePrimaryItem.Length)) && (TheCheckedItem.Xcoord > (ThePrimaryItem.Xcoord))) {
			if ((TheCheckedItem.Ycoord < (ThePrimaryItem.Ycoord + ThePrimaryItem.Length)) && (TheCheckedItem.Ycoord > (ThePrimaryItem.Ycoord))) {
				if (D_BUG) {
					cout << "Is it Inside Sqr ? = " << "true" << endl;
				}
				return true;
			}
		}
	}
	if (ThePrimaryItem.Type == CIRCLE) {
		
		return CircleInsed(ThePrimaryItem, TheCheckedItem);
	}
	if (ThePrimaryItem.Type == UNIDEF) {

	}
	if (D_BUG) {
		cout << "Is it Inside Anything ? = " << "False" << endl;
	}
	return false;
}

//Movemments Between Player And OtherPlayer / AI
void togglemovement() {
	canMove = !canMove;
}
void cantmove() {
	canMove = false;
}
void canmove() {
	canMove = true;
}

//Rolls the dice
void RollsDice() {
	RandomNumberGetPc = FakeDice();
}

//Change if the player is playing or AI
void TogglePlayer() {
	if (WhoIsThePlayer == 1) {
		WhoIsThePlayer = 2;
	}
	else if (WhoIsThePlayer == 2) {
		WhoIsThePlayer = 1;
	}
	else
	{
		WhoIsThePlayer = 1;
	}
}

void SwitchPlayer() {
	RollsDice();
	TogglePlayer();
	canmove();
	if (D_BUG) {
		cout << "\t  Next Player Turn , Player Playing = " << WhoIsThePlayer << endl;
	}
}

void ForcePlayerA() {
	RollsDice();
	WhoIsThePlayer = 1;
	canmove();
	if (D_BUG) {
		cout << "\t  Next Player Turn.F , Player Playing = " << WhoIsThePlayer << endl;
	}
}

void ForcePlayerB() {
	RollsDice();
	WhoIsThePlayer = 2;
	canmove();
	if (D_BUG) {
		cout << "\t  Next Player Turn.F , Player Playing = " << WhoIsThePlayer << endl;
	}
}


//AI IMPLEMNTATION

struct NodeValue{
	bool Legal;
	float Passing = 1;
};

float WeightVal(int i) {
	float ars[16] = {1.0 ,1.3 ,1.4 ,1.8 ,2 ,2.3 ,2.1 ,3 ,2.5 ,2.7 ,3.1 ,3.5 ,3.8 ,4 ,5};
	return ars[i];
}

float GetOperators(bool rosette, bool Kick, float weight) {
	float Output = weight;
	if (rosette) {
		Output *= 1.89;
	}else if (Kick) {
		Output *= 1.35;
	}
	else {
		Output *= 0.934;
	}
	return Output;
}

int Logic(bool z) {
	if (z) {
		return 1;
	}
	else {
		return 0;
	}
}

int AI_OUTPUT(NodeValue Node[]) {
	int Largest;
	int Biggest = -1;
	for (int i = 0; i < 6; i++) {
		if (D_BUG) {
			cout << "AI N." << i << " Node = " << Node[i].Passing << " P." << Logic(Node[i].Legal) << endl;
		}
		if (Node[i].Legal) {
			if (Node[i].Passing > Biggest) {
				Biggest = Node[i].Passing;
				Largest = i;
			}
		}
	}

	if (Biggest < 0) {
		return -1;
	}
	else {
		return Largest;
	}
}


//main
int main()
{
	//random seed
	srand((uint32_t)time(NULL));
	RandomNumberGetPc = FakeDice();
	//Mouse DataType
	FloatingCoord MouseValue;
	MouseValue.Length = 1;
	MouseValue.Type = UNIDEF;

	cout << "\t Loading Files" << endl;

	//Big box of things
	RectangleShape PopBox;
	PopBox.setSize(Vector2f(300, 100));
	PopBox.setPosition(Vector2f(310, 180));
	PopBox.setFillColor(Color::Color(Color::Magenta));

	cout << "Loading Texts" << endl;
	//Text
	Text TextHello;
	Font HumorFont;
	HumorFont.loadFromFile("HumorSans.ttf");
	TextHello.setFont(HumorFont);
	Text RText;
	Text DiceText;
	Text Current_player;
	Text PopBoxText;
	Font CoureFont;
	CoureFont.loadFromFile("FredokaOne.ttf");
	RText.setFont(CoureFont);
	DiceText.setFont(CoureFont);
	Current_player.setFont(CoureFont);
	PopBoxText.setFont(CoureFont);
	
	cout << "Loading Music" << endl;
	//Music
	Music BackgroudMusic;
	BackgroudMusic.openFromFile("Backgroud.wav");

	cout << "Loading Textures" << endl;
	//Textures
	Texture Rosette; if (!(Rosette.loadFromFile("Rosette.png"))) { cout << "Err Cant Load Rosette" << endl; }
	Texture DanmakT; if (!(DanmakT.loadFromFile("DanmakT.png"))) { cout << "Err Cant Load Danmaku T" << endl; }
	Texture DanmakB; if (!(DanmakB.loadFromFile("DanmakB.png"))) { cout << "Err Cant Load Rosette B" << endl; }
	Texture BoxCirl; if (!(BoxCirl.loadFromFile("BoxCire.png"))) { cout << "Err Cant Load BoxCircle" << endl; }
	Texture CircleM; if (!(CircleM.loadFromFile("Circles.png"))) { cout << "Err Cant Load Circle" << endl; }
	Texture CoinA; if (!(CoinA.loadFromFile("CoinA.png"))) { cout << "Err Cant Load CoinA" << endl; }
	Texture CoinB; if (!(CoinB.loadFromFile("CoinB.png"))) { cout << "Err Cant Load CoinB" << endl; }
	Rosette.setSmooth(true); 
	DanmakT.setSmooth(true);
	DanmakB.setSmooth(true);
	BoxCirl.setSmooth(true);
	CircleM.setSmooth(true);
	CoinA.setSmooth(true);
	CoinB.setSmooth(true);

	cout << "StateCheck" << endl;
	//Pieces
	Circles CoinsPlayer1[6];
	for (uint16_t k = 0; k < 6; k++) {
		CoinsPlayer1[k].SetWidth(20.f);
		CoinsPlayer1[k].SetLocation((float)(120 + (45 * k)), 50.f);
		CoinsPlayer1[k].setFillColor(Color::Color(sf::Color::Blue));
		CoinsPlayer1[k].SetAsPlayer(1);
		CoinsPlayer1[k].setTexture(&CoinA);
	}

	Circles CoinsPlayer2[6];
	for (uint16_t k = 0; k < 6; k++) {
		CoinsPlayer2[k].SetWidth(20.f);
		CoinsPlayer2[k].SetLocation((float)(120 + (45 * k)), 380.f);
		CoinsPlayer2[k].setFillColor(Color::Color(Color::Red));
		CoinsPlayer2[k].SetAsPlayer(2);
		CoinsPlayer2[k].setTexture(&CoinB);
	}
	
	//Board Blocks
	Squares BoardShapes[24];
	for (uint8_t k = 0; k < 24;k++) {
		BoardShapes[k].SetDim(90.f);
	}
	//Board Location Set
	if (1) {
	//Layer 1 Top Left Side
	BoardShapes[0].SetLocation(120.f, 95.f);	BoardShapes[0].setTexture(&Rosette); BoardShapes[0].Special();
	BoardShapes[1].SetLocation(210.f, 95.f);	BoardShapes[1].setTexture(&CircleM);
	BoardShapes[2].SetLocation(300.f, 95.f);	BoardShapes[2].setTexture(&DanmakB);
	BoardShapes[3].SetLocation(390.f, 95.f);	BoardShapes[3].setTexture(&BoxCirl);
	//Layer 2 Mid Left Side
	BoardShapes[4].SetLocation(120.f, 185.f);	BoardShapes[4].setTexture(&CircleM);
	BoardShapes[5].SetLocation(210.f, 185.f);	BoardShapes[5].setTexture(&DanmakT);
	BoardShapes[6].SetLocation(300.f, 185.f);	BoardShapes[6].setTexture(&CircleM);
	BoardShapes[7].SetLocation(390.f, 185.f);	BoardShapes[7].setTexture(&Rosette); BoardShapes[7].Special();
	//Layer 3 Bot Left Side
	BoardShapes[8].SetLocation(120.f, 275.f);	BoardShapes[8].setTexture(&Rosette); BoardShapes[8].Special();
	BoardShapes[9].SetLocation(210.f, 275.f);	BoardShapes[9].setTexture(&CircleM);
	BoardShapes[10].SetLocation(300.f, 275.f);	BoardShapes[10].setTexture(&DanmakB);
	BoardShapes[11].SetLocation(390.f, 275.f);	BoardShapes[11].setTexture(&BoxCirl);
	//Layer 2 Neck
	BoardShapes[12].SetLocation(480.f, 185.f);	BoardShapes[12].setTexture(&CircleM);
	BoardShapes[13].SetLocation(570.f, 185.f);	BoardShapes[13].setTexture(&DanmakT);
	//Layer 1 Right Side
	BoardShapes[14].SetLocation(660.f, 95.f);	BoardShapes[14].setTexture(&BoxCirl);
	BoardShapes[15].SetLocation(750.f, 95.f);	BoardShapes[15].setTexture(&Rosette); BoardShapes[15].Special();
	//Layer 2 Right Side
	BoardShapes[16].SetLocation(660.f, 185.f);	BoardShapes[16].setTexture(&CircleM);
	BoardShapes[17].SetLocation(750.f, 185.f);	BoardShapes[17].setTexture(&DanmakT);
	//Layer 3 Right Side
	BoardShapes[18].SetLocation(660.f, 275.f);	BoardShapes[18].setTexture(&BoxCirl);
	BoardShapes[19].SetLocation(750.f, 275.f);	BoardShapes[19].setTexture(&Rosette); BoardShapes[19].Special();
	//Layer 1 Mid StartPoint for Player A
	BoardShapes[20].SetLocation(480.f, 95.f);	BoardShapes[20].setFillColor(Color::Color(Color::Black)); BoardShapes[20].AllowStacking();	//Start A
	BoardShapes[21].SetLocation(570.f, 95.f);	BoardShapes[21].setFillColor(Color::Color(Color::Black)); BoardShapes[21].AllowStacking();	//End A
	//Layer 3 Mid StartPoint for Player B
	BoardShapes[22].SetLocation(480.f, 275.f);	BoardShapes[22].setFillColor(Color::Color(Color::Black)); BoardShapes[22].AllowStacking();	// Start B
	BoardShapes[23].SetLocation(570.f, 275.f);	BoardShapes[23].setFillColor(Color::Color(Color::Black)); BoardShapes[23].AllowStacking();	//End B
	}
	//Moves the Coin to Starting Posistion
	for (int i = 0; i < 6; i++) {
		CoinsPlayer1[i].SetLocationF(BoardShapes[CoinsPlayer1[i].GiveBoardLoc(0)].GiveCircleLocation());
		CoinsPlayer2[i].SetLocationF(BoardShapes[CoinsPlayer2[i].GiveBoardLoc(0)].GiveCircleLocation());
	}


	//Top Title Text
	TextHello.setString("The Royal Game Of Ur");
	TextHello.setPosition(390,10);
	TextHello.setCharacterSize(20);

	//Dice Text
	DiceText.setString(Dicenaml +  to_wstring(RandomNumberGetPc));
	DiceText.setPosition(30, 400);
	DiceText.setCharacterSize(20);
	RText.setString("Press the Button To End Turn -> ");
	RText.setPosition(380, 407);
	RText.setCharacterSize(15);
	Current_player.setCharacterSize(20);
	Current_player.setPosition(700, 400);
	PopBoxText.setPosition(335, 210);
	PopBoxText.setCharacterSize(25);

	//Place The Buttons
	Squares NextPlayerButton;
	NextPlayerButton.SetDim(30);
	NextPlayerButton.SetLocation(650, 400);
	NextPlayerButton.setFillColor(Color::Color(Color::Cyan));
	
	cout << "Loading Allright" << endl;
	//Put  inside array
	if (D_BUG) {
		for (uint8_t k = 0; k < 6; k++) {
//		printf_s("InputString[%d] = %c \n", int(k), InputString[k]);
			InputString[k] = '-';
		}
	}
	
	BackgroudMusic.setVolume(8.f);
	BackgroudMusic.setLoop(true);
	BackgroudMusic.play();

	//Starts Windows					  X   Y
	sf::RenderWindow window(sf::VideoMode(960, 460), "The Royal Game Of Ur", sf::Style::Close);

	//set FPS
	window.setFramerateLimit(60);

	window.setKeyRepeatEnabled(FALSE);
	
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	while (window.isOpen())
	{
		//Event Poll
		Event MainEvent;
		while (window.pollEvent(MainEvent))
		{
			switch (MainEvent.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::TextEntered:
				if (D_BUG) {
				//If enter is pressed
				if (MainEvent.text.unicode == 13) {
					printf("\n");
					//Puts all The Importat Stuff
					bool tyepCheck = false;

					//prints all Circle Coords
					if (InputString[0] == 'm') {
						for (int i = 0; i < 6; i++) {
							CoinsPlayer1[i].printCoord(i);
						}
						for (int i = 0; i < 6; i++) {
							CoinsPlayer2[i].printCoord(i);
						}
					}//Moves The Pieces
					else if (1) {
					uint8_t Datamovers[6];
					for (uint8_t k = 0; k < 6; k++) {
						Datamovers[k] = 0;
					}
					Datamovers[0] = uint8_t(InputString[0]) - 48;
					Datamovers[1] = uint8_t(InputString[2] - 48);
					if (InputString[4] == '1') {
						Datamovers[2] += 10;
						printf_s("Datamove[2] = 10\n");
					}
					if ((InputString[5] == '-')) {}
					else {
						Datamovers[2] += uint8_t(InputString[5]) - 48;
						printf_s("Datamove[5]\n");
					}
					if (Datamovers[0] < 3 && Datamovers[1] < 6 && Datamovers[3] < 20) {
						if (Datamovers[0] == 1) {
							printf("Moving 1\n");
							Datamovers[3] = BoardShapes[2].GetTop();
							CoinsPlayer1[Datamovers[1]].SetLocation(BoardShapes[Datamovers[2]].GiveXcoord() + 25, BoardShapes[Datamovers[2]].GiveYcoord() + 25);
						}
						else if (Datamovers[0] == 2) {
							printf("Moving 2\n");
							Datamovers[3] = BoardShapes[2].GetTop();
							CoinsPlayer2[Datamovers[1]].SetLocation(BoardShapes[Datamovers[2]].GiveXcoord() + 25, BoardShapes[Datamovers[2]].GiveYcoord() + 25);
						}
					}
					else {
						printf_s("FLASE DYCOTMY\n");
					}

					//Resets
					for (uint8_t k = 0; k < 6; k++) {
						printf_s("InputString[%d] = %c \t", int(k), InputString[k]);
						printf_s("DataMovers-[%d] = %d \n", int(k), int(Datamovers[k]));
						InputString[k] = '-';
					}
					//1:1:01
					//Player:coin:Dest
					}

				}
				else {
					cout << char(MainEvent.text.unicode);
					PutIntoString(char(MainEvent.text.unicode));
				}
				}
				break;
			case Event::MouseButtonPressed:
			{
				//if anyone has win cant press anything
				if (has_anyone_win) {
					break;
				}
				MouseValue.Xcoord = float(MainEvent.mouseButton.x);
				MouseValue.Ycoord = float(MainEvent.mouseButton.y);
				if (D_BUG) {
					cout << "X pos = " << MouseValue.Xcoord << '\t';
					cout << "Y pos = " << MouseValue.Ycoord << endl;
				}

				//Actualy Do Stuff
				
				//Loops To The Circles The First One the kick Everyone Else from trying
				if (canMove) {
					if (WhoIsThePlayer == 1) {
						//Checks for CoinA
						if (!Ai_Enable) {
							for (int i = 0; i < 6; i++) {
							if (DistanceInside(CoinsPlayer1[i].GiveXYD(), MouseValue)) {
								if (D_BUG) {
									cout << "Coin A , Number = " << i << endl;
									cout << "Player Playing = " << WhoIsThePlayer << endl;
								}
								
								
								//well looks like we need to manualy do everything 
								int current_board =  CoinsPlayer1[i].GiveBoardLoc(CoinsPlayer1[i].GetBoardLoc());
								int CPNum = CoinsPlayer1[i].GetBoardLoc() + RandomNumberGetPc; //Get the target 
								int target_board = CoinsPlayer1[i].GiveBoardLoc(CPNum); // Traslate To actual Coordinate
								bool Legal;
								if (BoardShapes[target_board].IsItOccupied()) {
									//now checks if the object is rosette
									if (BoardShapes[target_board].CheckSpecial()) {
										Legal = false;
										break;
									}
									else {
										if (BoardShapes[target_board].CheckStack()) {
											Legal = true;
										}
										else {
											char Occuped = BoardShapes[target_board].getOccupier();
											char Myself = CoinsPlayer1[i].GetPlayer();
											//if the target board allready have a item and it the same team
											if (Occuped == Myself) {
												Legal = false;
											}
											else {
												Legal = true;
												//kick the other player
												int pieceNumebr = BoardShapes[target_board].getPiece();
												int othert = CoinsPlayer2[pieceNumebr].GiveBoardLoc(0);
												CoinsPlayer2[pieceNumebr].SetLocationF(BoardShapes[othert].GiveCircleLocation());
											}

										}
									}
								}
								else {
									//Moves
									Legal = true;
								}

								//actualy do the moving
								if (Legal) {
									BoardShapes[current_board].ResetOccupation();
									char player_current = CoinsPlayer1[i].GetPlayer();
									BoardShapes[target_board].setOccupation(player_current, i);
									CoinsPlayer1[i].SetLocationF(BoardShapes[target_board].GiveCircleLocation());
									cantmove();
								}
								else {
									//cant do anything
								}
								
								break;
							}
							
						}
						}
					}
					else if (WhoIsThePlayer == 2) {
						//Checks for CoinB
						for (int i = 0; i < 6; i++) {
							if (DistanceInside(CoinsPlayer2[i].GiveXYD(), MouseValue)) {
								if (D_BUG) {
									cout << "Coin B , Number = " << i << endl;
									cout << "Player Playing = " << WhoIsThePlayer << endl;
								}

								int current_board = CoinsPlayer2[i].GiveBoardLoc(CoinsPlayer2[i].GetBoardLoc());
								int CPNum = CoinsPlayer2[i].GetBoardLoc() + RandomNumberGetPc; //Get the target 
								int target_board = CoinsPlayer2[i].GiveBoardLoc(CPNum); // Traslate To actual Coordinate
								bool Legal;
								if (BoardShapes[target_board].IsItOccupied()) {
									//now checks if the object is rosette
									if (BoardShapes[target_board].CheckSpecial()) {
										Legal = false;
										break;
									}
									else {
										if (BoardShapes[target_board].CheckStack()) {
											Legal = true;
										}
										else {
											char Occuped = BoardShapes[target_board].getOccupier();
											char Myself = CoinsPlayer2[i].GetPlayer();
											//if the target board allready have a item and it the same team
											if (Occuped == Myself) {
												Legal = false;
											}
											else {
												Legal = true;
												//kick the other player
												int pieceNumebr = BoardShapes[target_board].getPiece();
												int othert = CoinsPlayer1[pieceNumebr].GiveBoardLoc(0);
												CoinsPlayer1[pieceNumebr].SetLocationF(BoardShapes[othert].GiveCircleLocation());
											}

										}
									}
								}
								else {
									//Moves
									Legal = true;
								}

								//actualy do the moving
								if (Legal) {
									BoardShapes[current_board].ResetOccupation();
									char player_current = CoinsPlayer2[i].GetPlayer();
									BoardShapes[target_board].setOccupation(player_current, i);
									CoinsPlayer2[i].SetLocationF(BoardShapes[target_board].GiveCircleLocation());
									cantmove();
								}
								else {
									//cant do anything
								}

								break;

							}
						}
					}
				}


				//Check if The EndTurn Is Pressed
				if (DistanceInside(NextPlayerButton.GiveXYD(), MouseValue)) {
					

					//Your Move AI
					if (Ai_Enable) {

						SwitchPlayer(); // 2 to 1

						NodeValue EachNode[6];

						if (D_BUG) {
							cout << "Start AI Scan Gn."<< globalcounterAiCall << endl;
							cout << "rRn." << RandomNumberGetPc << endl;
							globalcounterAiCall++;
						}
						if (RandomNumberGetPc != 0) {

						for (int i = 0;i < 6; i++) {
							int targ = CoinsPlayer1[i].GetBoardLoc() + RandomNumberGetPc;

							if (targ < 16) {
								int current_board = CoinsPlayer1[i].ConstSafeBoardDirct(CoinsPlayer1[i].GetBoardLoc());
								int CPNum = CoinsPlayer1[i].GetBoardLoc() + RandomNumberGetPc; //Get the target 
								int target_board = CoinsPlayer1[i].ConstSafeBoardDirct(CPNum); // Traslate To actual Coordinate
								if (D_BUG) {
									cout << "N."<< targ << " C."<< CPNum << " S."<< CoinsPlayer1[i].GetBoardLoc() <<" i."<<i<< endl;
								}
								bool Legal = false;
								bool Kick = false;
								bool rosette = false;
								if (BoardShapes[target_board].IsItOccupied()) {
									//now checks if the object is rosette
									if (BoardShapes[target_board].CheckSpecial()) {
										Legal = false;
										break;
									}
									else {
										if (BoardShapes[target_board].CheckStack()) {
											Legal = true;
										}
										else {
											char Occuped = BoardShapes[target_board].getOccupier();
											char Myself = CoinsPlayer1[i].GetPlayer();
											//if the target board allready have a item and it the same team
											if (Occuped == Myself) {
												Legal = false;
											}
											else {
												Legal = true;
												//kick the other player
												Kick = true;
											}

										}
									}
								}
								else {
									//Moves
									Legal = true;
									if (BoardShapes[target_board].CheckSpecial()) {
										rosette = true;
									}
								}

								EachNode[i].Legal = Legal;//Only pass if it passing
								EachNode[i].Passing = GetOperators(rosette,Kick, WeightVal(targ));
							}
							else {
								if (D_BUG) {
									cout << "N." << targ << " S." << CoinsPlayer1[i].GetBoardLoc() << " i." << i << endl;
								}
								EachNode[i].Legal = false;//
								EachNode[i].Passing = -1;
							}
						}

						if (D_BUG) {
							cout << "Start AI End Scan" << endl;
						}
						//Actual AI computation
						if (D_BUG) {
							cout << "Start AI Computation" << endl;
						}
						int Who_to_move = AI_OUTPUT(EachNode);
						if (D_BUG) {
							cout << "End AI Computation = " << Who_to_move << endl;
						}
						if (Who_to_move != -1) {
						//Please Move the offending piece
							int i = Who_to_move;
							int current_board = CoinsPlayer1[i].GiveBoardLoc(CoinsPlayer1[i].GetBoardLoc());
							int CPNum = CoinsPlayer1[i].GetBoardLoc() + RandomNumberGetPc; //Get the target 
							int target_board = CoinsPlayer1[i].GiveBoardLoc(CPNum); // Traslate To actual Coordinate
							bool Legal;
							if (BoardShapes[target_board].IsItOccupied()) {
								//now checks if the object is rosette
								if (BoardShapes[target_board].CheckSpecial()) {
									Legal = false;
									break;
								}
								else {
									if (BoardShapes[target_board].CheckStack()) {
										Legal = true;
									}
									else {
										char Occuped = BoardShapes[target_board].getOccupier();
										char Myself = CoinsPlayer1[i].GetPlayer();
										//if the target board allready have a item and it the same team
										if (Occuped == Myself) {
											Legal = false;
										}
										else {
											Legal = true;
											//kick the other player
											int pieceNumebr = BoardShapes[target_board].getPiece();
											int othert = CoinsPlayer2[pieceNumebr].GiveBoardLoc(0);
											CoinsPlayer2[pieceNumebr].SetLocationF(BoardShapes[othert].GiveCircleLocation());
										}

									}
								}
							}
							else {
								//Moves
								Legal = true;
							}

							//actualy do the moving
							if (Legal) {
								BoardShapes[current_board].ResetOccupation();
								char player_current = CoinsPlayer1[i].GetPlayer();
								BoardShapes[target_board].setOccupation(player_current, i);
								CoinsPlayer1[i].SetLocationF(BoardShapes[target_board].GiveCircleLocation());
								cantmove();
							}
							else {
								//cant do anything
							}



						}
						if (D_BUG) {
							cout << "End AI segment" << endl;
						}
						else {
							if (D_BUG) {
								cout << "AI Skip 0" << endl;
							}
						}

						}
						ForcePlayerB(); // 1 to 2

						//Scan Winner
						if (1) {
							//For Coin A
							int j = 0;
							for (int i = 0; i < 6; i++) {
								j += CoinsPlayer1[i].GetBoardLoc();
							}
							if (D_BUG) {
								cout << "A J." << j << endl;
							}
							if (j == 90) {
								has_anyone_win = true;
								who_is_the_Winner = 'A';
							}
							//For coin B
							j = 0;
							for (int i = 0; i < 6; i++) {
								j += CoinsPlayer2[i].GetBoardLoc();
							}
							if (D_BUG) {
								cout << "B J." << j << endl;
							}
							if (j == 90) {
								has_anyone_win = true;
								who_is_the_Winner = 'B';
							}

						}

					}


				}

			}break;
			default:
				break;
			}
		}

		//Other computation

		//stop the music
		if (has_anyone_win) {
			BackgroudMusic.setLoop(false);
			displayBox = true;
			PopBoxText.setString(Winner_is + who_is_the_Winner);
		}
		//Checks if anyone have actualy wins

		//Clear Screen
		window.clear();
		DiceText.setString(Dicenaml + to_wstring(RandomNumberGetPc));
		if (canMove) {
			Current_player.setString(Playername + to_wstring(WhoIsThePlayer));
		}
		else {
		Current_player.setString(Playername + to_wstring(WhoIsThePlayer) + " X");
		}
		

		//Put Every .Draw Here
		window.draw(TextHello);	//Text
		window.draw(DiceText);
		window.draw(NextPlayerButton);
		window.draw(RText);
		window.draw(Current_player);
		for (uint8_t i = 0;	i < 24;	i++) {
			window.draw(BoardShapes[i]);			//Board
		}
		for (uint16_t i = 0; i < 6; i++) {
			window.draw(CoinsPlayer1[i]);
		}
		for (uint16_t i = 0; i < 6; i++) {
			window.draw(CoinsPlayer2[i]);
		}
		if (displayBox) {
			window.draw(PopBox);
			window.draw(PopBoxText);
		}



		//Actualy Draw Everything
		window.display();
	}
	return 0;
}
