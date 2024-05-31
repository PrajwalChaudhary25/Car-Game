#include<iostream>
#include<conio.h>
#include<direct.h>
#include<windows.h>
#include<time.h>
#include<fstream>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

class CONSOLE {
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
public:
void setPosition(int x, int y);
void setcursor(bool visible, DWORD size);
void drawBorder(int level);
};
void CONSOLE::setPosition(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void CONSOLE::setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}
void CONSOLE::drawBorder(int level){
	for(int i=0; i<SCREEN_HEIGHT; i++){
		for(int j=0; j<17; j++){
			setPosition(0+j,i); cout<<"||";
			setPosition(WIN_WIDTH-j,i); cout<<"||";
		}
	}
	for(int i=0; i<SCREEN_HEIGHT; i++){
		setPosition(SCREEN_WIDTH,i); cout<<"||";
	}
	// Display level inside the border
    setPosition( 18, 1); // Adjust the position as needed
    cout << "--------------Level: " << level<<"--------------";
}
CONSOLE CON;


class Enemy {
public:
int enemyY[3];
int enemyX[3];
int enemyFlag[3];
public:
    void genEnemy(int ind);
    void drawEnemy(int ind);
    void eraseEnemy(int ind);
    void resetEnemy(int ind);
};
void Enemy::genEnemy(int ind){
	enemyX[ind] = 18 + rand()%(33);
}
void Enemy::drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		CON.setPosition(enemyX[ind], enemyY[ind]);   cout<<"****";
		CON.setPosition(enemyX[ind], enemyY[ind]+1); cout<<"*  *";
		CON.setPosition(enemyX[ind], enemyY[ind]+2); cout<<"*  *";
		CON.setPosition(enemyX[ind], enemyY[ind]+3); cout<<"****";
	}
}
void Enemy::eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		CON.setPosition(enemyX[ind], enemyY[ind]); cout<<"    ";
		CON.setPosition(enemyX[ind], enemyY[ind]+1); cout<<"    ";
		CON.setPosition(enemyX[ind], enemyY[ind]+2); cout<<"    ";
		CON.setPosition(enemyX[ind], enemyY[ind]+3); cout<<"    ";
	}
}
void Enemy::resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}
Enemy E1;


class car{
public:
int carPosX = WIN_WIDTH/2;
int carPosY ;
char car[4][4] = {  ' ','#','#',' ',
					'#','#','#','#',
					' ','#','#',' ',
					'#','#','#','#', };

    void drawCar();
    void eraseCar();
};
void car::drawCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(car[i][j]=='#'){
				CON.setPosition(j+carPosX, i+carPosY); cout<<car[i][j];
			}
		}
	}
}
void car::eraseCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(car[i][j]=='#'){
				CON.setPosition(j+carPosX, i+carPosY); cout<<" ";
			}
		}
    }
}
car C1;

class gameplay{
public:
int speed;
int score = 0;
int highscore=0;
int level=1;
void play();
void instructions();
void updateScore();
void Highscore();
void updatehighscore();
void resethighscore();
void updateLevel();
int collision();
void credits();
void gameover();
void victory();
};

void gameplay::updateLevel() {
	if (score <= 10 ){
        level = 1;
	}
    else if (score > 10 && score<=40) {
        level = 2;
        speed=50; // Set to the desired level
        // Add more levels if needed
    }
	else if(score>40 && score<=100){
		level = 3;
        speed=30;
	}
	else
	{
		victory();
	}
}

void gameplay::updateScore() {
    CON.setPosition(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
    CON.setPosition(WIN_WIDTH + 7, 6); cout << "Level: " << level << endl; // Display the current level
}

void gameplay::Highscore(){

	system("cls");
	ifstream infile;
	infile.open("score.txt");
	infile>>highscore;
	cout<<"-----------------------"<<endl;
	cout<<"HIGHSCORE::"<<highscore<<endl;
	cout<<"RESET HIGHSCORE<y/n>?"<<endl;
	char opp = getche();
	if (opp=='y'||opp=='Y'){
		resethighscore();
	}
    infile.close();
	cout<<"\npress any key to go back to menu";
	getch();

}


void gameplay::updatehighscore() {

	if (score>=highscore){
		highscore=score;
        ofstream outfile;
	    outfile.open("score.txt");
	    outfile<<highscore;
		outfile.close();
	}
}

void gameplay::resethighscore(){
	ofstream outfile;
	outfile.open("score.txt");
	highscore=0;
	outfile<<highscore;
	outfile.close();
}

void gameplay::play(){

	C1.carPosX = -1 + WIN_WIDTH/2;
    C1.carPosY =22;
    speed=50;
	score = 0;
	E1.enemyFlag[0] = 1;
	E1.enemyFlag[1] = 0;
	E1.enemyY[0] =E1.enemyY[1] = 1;

	system("cls");
	CON.drawBorder(level);
	updateScore();
	E1.genEnemy(0);
	E1.genEnemy(1);

	CON.setPosition(WIN_WIDTH + 7, 2);cout<<"Car Game";
	CON.setPosition(WIN_WIDTH + 6, 4);cout<<"----------";
	CON.setPosition(WIN_WIDTH + 6, 6);cout<<"----------";
	CON.setPosition(WIN_WIDTH + 7, 12);cout<<"Control ";
	CON.setPosition(WIN_WIDTH + 7, 13);cout<<"-------- ";
	CON.setPosition(WIN_WIDTH + 3, 14);cout<<" W Key - UP";
	CON.setPosition(WIN_WIDTH + 3, 15);cout<<" S Key - DOWN";
    CON.setPosition(WIN_WIDTH + 3, 16);cout<<" A Key - Left";
	CON.setPosition(WIN_WIDTH + 3, 17);cout<<" D Key - Right";

	CON.setPosition(18, 5);cout<<"Press any key to start";
	getch();
	CON.setPosition(18, 5);cout<<"                      ";

	while(1){
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( C1.carPosX > 18 )
					C1.carPosX -= 4;
			}
			if( ch=='d' || ch=='D' ){
				if( C1.carPosX < 50 )
					C1.carPosX += 4;
			}
            if( ch=='w' || ch=='W' ){
				if( C1.carPosY > 0 )
					C1.carPosY -= 4;
			}
			if( ch=='s' || ch=='S' ){
				if( C1.carPosY < 22 )
					C1.carPosY += 4;
			}
			if(ch==27){
				break;
			}
		}

		C1.drawCar();
		E1.drawEnemy(0);
		E1.drawEnemy(1);

		if( collision() == 1  ){
			PlaySound(TEXT("./crash.wav"),NULL,SND_SYNC);
			gameover();
			return;
		}
		Sleep(speed);
		C1.eraseCar();
		E1.eraseEnemy(0);
		E1.eraseEnemy(1);

		 if (E1.enemyY[0] == 10 && E1.enemyFlag[1] == 0) {
            E1.enemyFlag[1] = 1;
        }

        if (E1.enemyFlag[0] == 1)
            E1.enemyY[0] += 1;

        if (E1.enemyFlag[1] == 1)
            E1.enemyY[1] += 1;

        if (E1.enemyY[0] > SCREEN_HEIGHT - 4) {
            E1.resetEnemy(0);
            score++;
            updateLevel();
            updateScore();
			updatehighscore();
			CON.drawBorder(level);
        }
		if (E1.enemyY[1] > SCREEN_HEIGHT - 4) {
            E1.resetEnemy(1);
            score++;
            updateLevel();
            updateScore();
			updatehighscore();
			CON.drawBorder(level);
    	}
		if (score >= 100) {
			victory();
			credits();
            break; // exit the loop after victory
        }
 	}
}

void gameplay::instructions(){

	system("cls");
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Avoid Cars by moving left or right. ";
	cout<<"\n\n Press 'a' to move left";
	cout<<"\n Press 'd' to move right";
	cout<<"\n Press 'escape' to exit";
	cout<<"\n\nPress any key to go back to menu";
	getch();
}
int gameplay::collision() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (C1.car[i][j] == '#') {
                int carX = C1.carPosX + j;
                int carY = C1.carPosY + i;

                for (int k = 0; k < 4; ++k) {
                    for (int l = 0; l < 4; ++l) {
                        if (E1.enemyFlag[0] && E1.enemyY[0] + k == carY && E1.enemyX[0] + l == carX) {
                            return 1;
                        }
                        if (E1.enemyFlag[1] && E1.enemyY[1] + k == carY && E1.enemyX[1] + l == carX) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


void gameplay::gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t=============GAME-OVER============="<<endl;
	cout<<"\t\t|----------------*----------------|"<<endl;
	cout<<"\t\t|-------------* O O *-------------|"<<endl;
	cout<<"\t\t|------------*   #   *------------|"<<endl;
	cout<<"\t\t|--------*----*  x  *----*--------|"<<endl;
	cout<<"\t\t|----------*-----*-----*----------|"<<endl;
	cout<<"\t\t|------------*-------*------------|"<<endl;
	cout<<"\t\t|--------------*---*--------------|"<<endl;
	cout<<"\t\t|----------------*----------------|"<<endl;
	cout<<"\t\t|--------------*---*--------------|"<<endl;
	cout<<"\t\t|------------*-------*--- --------|"<<endl;
	cout<<"\t\t|----------*-----------*----------|"<<endl;
	cout<<"\t\t|--------*---------------*--------|"<<endl;
	cout<<"\t\t==================================="<<endl<<endl;
	cout<<"\t\tPress any key to go back to menu.";
	PlaySound(TEXT("./gameover.wav"),NULL,SND_SYNC);
	getch();
}
void gameplay::credits(){

	system("cls");
	cout<<"\n____________________________";
	cout<<"\n||#########CREDITS##########||";
	cout<<"\n----------------------------";
	cout<<"\n----------------------------";
	cout<<"\n |HEAD->Kohinoor dallakoti";
	cout<<"\n |COORDINATOR->Prajwal chaudhary";
	cout<<"\n |adMANAGER->Amrita shahi";
	cout<<"\n\nPress any key to go back to menu.";
	getch();
}
gameplay G;

void gameplay::victory() {
    system("cls");
    cout << endl;
    cout << "\t\t-----------------------------------------" << endl;
	cout << "\t\t-----------------------------------------" << endl;
    cout << "\t\t-----------------------------------------" << endl;
	cout << "\t\t-------__________________________--------" << endl;
    cout << "\t\t-------|                         |-------" << endl;
    cout << "\t\t-------|        *       *        |-------" << endl;
    cout << "\t\t-------|         *     *         |-------" << endl;
    cout << "\t\t-------|          *   *          |-------" << endl;
	cout << "\t\t-------|           * *           |-------" << endl;
    cout << "\t\t-------|            *            |-------" << endl;
    cout << "\t\t-------|_________VICTORY_________|-------" << endl;
	cout << "\t\t---------CONGRATULATIONS YOU WON!--------" << endl;
    cout << "\t\t-----------------------------------------" << endl;
	cout << "\t\t-----------------------------------------" << endl;
    cout << "\t\t-----------------------------------------" << endl << endl;
    cout << "\t\tPress any key to go back to menu.";
	PlaySound(TEXT("./victory.wav"),NULL,SND_SYNC);
    getch();
}

int main()
{
	CON.setcursor(0,0);
	srand( (unsigned)time(NULL));

	do{
		system("cls");
		CON.setPosition(10,5); cout<<" ---------------------------- ";
		CON.setPosition(10,6); cout<<" ||########CAR GAME########|| ";
		CON.setPosition(10,7); cout<<" ----------------------------";
		CON.setPosition(10,9); cout<<"|-1. Start Game";
		CON.setPosition(10,10); cout<<"|-2. Instructions";
		CON.setPosition(10,11); cout<<"|-3. Credits";
		CON.setPosition(10,11); cout<<"|-4. High Score";
		CON.setPosition(10,12); cout<<"|-5. Quit";
		CON.setPosition(10,14); cout<<"Select option: ";
		char op = getche();

		if( op=='1') G.play();
		else if( op=='2') G.instructions();
		else if( op=='3') G.credits();
		else if( op=='4') G.Highscore();
		else if( op=='5') exit(0);

	}while(1);

	return 0;
}
