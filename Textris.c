#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<string.h>
#include <windows.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

#define INPUT_TIMEOUT 1
#define CHECK_TIMEOUT 800

//This will change the fps , if the windows stutters a lot put a slightly higher number than 30
#define RENDER_TIMEOUT 30

#define BLOCK 35

#define START_SPEED 300

#define SCORE_DISPLAY_LIMIT 10

#define PIECE_SIZE 4

typedef struct{
	int x;
	int y;
	int wid;
	int hei;
	int type;
	char grid[PIECE_SIZE][PIECE_SIZE];           
}piece;

char getInput();
int ticked(DWORD check_time);
DWORD getCheckTime(int time);
void printRow();
piece newPiece();
piece keyHandler(char grid[GRID_HEIGHT][GRID_WIDTH],piece activePiece,char key);
int touchesPieceTop(char grid[GRID_HEIGHT][GRID_WIDTH],piece block);
int touchesPieceSide(char grid[GRID_HEIGHT][GRID_WIDTH],piece block);
void rotate_arr(char arr[PIECE_SIZE][PIECE_SIZE],int size);
int getSteps(int score,int limit);

int score=0;
int high_score=0;

DWORD check_time,down_time,render_time;
int speed=START_SPEED;


int main(){
	system("title Textris v0");
	system("color 0a");
	system("mode con: cols=49 lines=27");
	srand(time(NULL));
	
	//game 
	int scoreStep=SCORE_DISPLAY_LIMIT-1;
	int high_scoreStep=SCORE_DISPLAY_LIMIT-1;
	
	while (1==1){
	

	char key='.';
	int flag=1;
	
	
	
	int pause=0;
	
	char grid[GRID_HEIGHT][GRID_WIDTH];
		
	int i;
	int j;
	
	for (i=0;i<GRID_WIDTH;i++){
		for (j=0;j<GRID_HEIGHT;j++){
			grid[j][i]=' ';
		}
	}	
	
	piece activePiece = newPiece();
	piece nextPiece = newPiece();

	check_time=getCheckTime(CHECK_TIMEOUT);
	down_time=getCheckTime(speed);
	render_time=getCheckTime(RENDER_TIMEOUT);
	while(key!='0'&&flag==1){
		int preScore=score;
		//keyInput
		key=getInput();
		if(key!='.'&&key!='\0')activePiece=keyHandler(grid,activePiece,key);
		if (key==' ')pause=-(pause-1);
		if (pause==0){
			//logic
			if (activePiece.y+activePiece.hei<GRID_HEIGHT&&touchesPieceTop(grid,activePiece)==1&&ticked(down_time)==0){
				activePiece.y++;
				check_time=getCheckTime(CHECK_TIMEOUT);
				down_time=getCheckTime(speed);

			}
			else if(ticked(check_time)==0){
				for (i=0;i<PIECE_SIZE;i++){
					for (j=0;j<PIECE_SIZE;j++){
						if (activePiece.grid[i][j]==BLOCK){
							grid[i+activePiece.y][j+activePiece.x]=activePiece.grid[i][j];
						}
					}	
				}
				int rows=0;
				for (i=activePiece.y;i<activePiece.y+activePiece.hei;i++){
					int stop=1;
					for (j=0;j<GRID_WIDTH;j++){
						if (grid[i][j]!=BLOCK){
							stop=0;
							break;
						}
					}
					if(stop==1){
						int q;
						int k;
						for (q=i;q>0;q--){
							for (k=0;k<GRID_WIDTH;k++){
								grid[q][k]=grid[q-1][k];
							}

						}
						rows++;
					}

				}
				if (rows==4){
					score+=600+(START_SPEED-speed)*2;
					speed-=10;
				}
				else if(rows>0){
					score+=rows*100+(START_SPEED-speed)*2;
					speed-=5*rows;
				}
				if (touchesPieceTop(grid,nextPiece)==0||activePiece.y<0)
					flag=0;
				activePiece=nextPiece;
				nextPiece.y;
				nextPiece=newPiece();
				check_time=getCheckTime(CHECK_TIMEOUT);
				render_time+=RENDER_TIMEOUT;
				down_time=getCheckTime(speed);

			}
		if (preScore!=score){
			scoreStep=SCORE_DISPLAY_LIMIT-getSteps(score,SCORE_DISPLAY_LIMIT);
		}
		if (score>high_score){
			high_scoreStep=scoreStep;
			high_score=score;
		}

		if (ticked(render_time)==0||flag==0){
			//render
			system("cls");
			printRow(GRID_WIDTH*4+9);
			printf("\n\t  _________\t\t\t\t|\t\t  |Textris|\t\t\t\t|\n\t");
			printRow(GRID_WIDTH+2);
			printf("\t  NEXT\t\t\t|\n"); 
			for (j=0;j<GRID_HEIGHT;j++){
			printf("\t|");
			char gridTemp[GRID_WIDTH];
			for (i=0;i<GRID_WIDTH;i++){
					if ((i>=activePiece.x&&i<activePiece.x+PIECE_SIZE)&&(j>=activePiece.y&&j<activePiece.y+PIECE_SIZE)){
						if (activePiece.grid[j-activePiece.y][i-activePiece.x]==BLOCK){
							gridTemp[i]=activePiece.grid[j-activePiece.y][i-activePiece.x];
						}
						else{
							gridTemp[i]=grid[j][i];
						}
					}
					else{
							gridTemp[i]=grid[j][i];
						}
				}
				printf("%s",gridTemp);
				printf("|");
				if (j==0||j==5){
					printf("\t");
					printRow(7);
					printf("\t\t\t");
				}
				else if(j>0&&j<5){
					int k;
					printf("\t| ");
					for(k=0;k<PIECE_SIZE;k++){
						if (nextPiece.grid[j-1][k]==BLOCK)
							printf("%c",nextPiece.grid[j-1][k]);
						else{
							printf(" ");
						}
					}
					printf("|\t\t\t");
				}
				
				else if (j==9){
					printf("\tSCORE :     ",score);
					int q;
					for(q=0;q<scoreStep;q++){
						printf("0");
					}
					printf("%d\t",score);
				}
				else if (j==7){
					printf("\tHIGHSCORE : ");	
					int q;
					for(q=0;q<high_scoreStep;q++){
						printf("0");
					}
					printf("%d\t",high_score);
				}
				else if (j==12){
					printf("\tCONTROLLS :\t\t");
				}
				else if (j==13){
					printf("\t----------\t\t");
				}
				else if (j==14){
					printf("\tMOVE LEFT 'A'\t\t");
				}
				else if (j==15){
					printf("\tMOVE RIGHT 'D'\t\t");
				}
				else if (j==16){
					printf("\tMOVE DOWN 'S'\t\t");
				}
				else if (j==17){
					printf("\tROTATE 'W'\t\t");
				}
				else if (j==18){
					printf("\tPAUSE 'SPACE'\t\t");
				}
				else if (j==19){
					printf("\tRESET '0'\t\t");
				}
				else{
					printf("\t\t\t\t");
				}
				printf("|\n");
				fflush(stdout);
			}
			printf("\t");
			printRow(GRID_WIDTH+2);
			printf("\t\t\t\t|\n\t\t\t\t\t\t|\n");
			printRow(GRID_WIDTH*4+9);
			render_time=getCheckTime(RENDER_TIMEOUT);
			fflush(stdout);
		}
	}
		
	}
	score =0;	
	scoreStep=SCORE_DISPLAY_LIMIT-1;
}
	return 0;
}

char getInput(){
	char ch='.';
	DWORD check_time;

	check_time=getCheckTime(INPUT_TIMEOUT); //GetTickCount returns time in miliseconds, so I add 500 to wait input for half a second.

	while ((check_time>GetTickCount()))
    	{
        	if (kbhit())
        	{
        		fflush(stdin);
            	ch=_getch();
            	break;

        	}
    	}
    return ch;
}

int ticked(DWORD check_time){
	if (check_time>GetTickCount()){
		return 1;
	}
	return 0;
}

DWORD getCheckTime(int time){
	DWORD start_time, check_time;
	start_time=GetTickCount();
	check_time=start_time+time;
	return check_time;
}

void printRow(int x){
	int i;
	for (i=0;i<x;i++){
		printf("-");
	}
}

piece newPiece(){
	piece block;
	block.x=4;
	block.y=-4;
	block.type=rand()%7;
	if (block.type==0){
		block.wid=2;
		block.hei=2;
		strcpy(block.grid[0],"00   ");
		strcpy(block.grid[1],"00   ");
		strcpy(block.grid[2],"     ");
		strcpy(block.grid[3],"     ");
		block.y=-2;
	}
	else if (block.type==1){
		block.wid=2;
		block.hei=3;
		strcpy(block.grid[0],"0   ");
		strcpy(block.grid[1],"0   ");
		strcpy(block.grid[2],"00  ");
		strcpy(block.grid[3],"    ");
		block.y=-3;
	}
	else if (block.type==2){
		block.wid=2;
		block.hei=3;
		strcpy(block.grid[0]," 0  ");
		strcpy(block.grid[1]," 0  ");
		strcpy(block.grid[2],"00  ");
		strcpy(block.grid[3],"    ");
		block.y=-3;
	}else if (block.type==3){
		block.wid=3;
		block.hei=2;
		strcpy(block.grid[0],"00  ");
		strcpy(block.grid[1]," 00 ");
		strcpy(block.grid[2],"    ");
		strcpy(block.grid[3],"    ");
		block.y=-2;
	}
	else if (block.type==4){
		block.wid=1;
		block.hei=4;
		strcpy(block.grid[0],"0   ");
		strcpy(block.grid[1],"0   ");
		strcpy(block.grid[2],"0   ");
		strcpy(block.grid[3],"0   ");
	}
	else if (block.type==5){
		block.wid=3;
		block.hei=2;
		strcpy(block.grid[0]," 00 ");
		strcpy(block.grid[1],"00  ");
		strcpy(block.grid[2],"    ");
		strcpy(block.grid[3],"    ");
		block.y=-2;
	}
	else if (block.type==6){
		block.wid=2;
		block.hei=3;
		strcpy(block.grid[0],"0   ");
		strcpy(block.grid[1],"00  ");
		strcpy(block.grid[2],"0   ");
		strcpy(block.grid[3],"    ");
		block.y=-3;
	}
	int i,j;
	for (i=0;i<PIECE_SIZE;i++){
		for (j=0;j<PIECE_SIZE;j++){
			if (block.grid[i][j]=='\0')break;
			if (block.grid[i][j]=='0'){
				block.grid[i][j]=BLOCK;
			}		
		}	
	}
	return block;
}

piece keyHandler(char grid[GRID_HEIGHT][GRID_WIDTH],piece active,char key){
	if (key=='a'||key=='A'){
		if (active.x>0){
			active.x--;
			if (touchesPieceSide(grid,active)==0){
				active.x++;
			}
			else{
				check_time=getCheckTime(CHECK_TIMEOUT);
			}
		}
	}
	else if(key=='d'||key=='D'){
		if (active.x+active.wid<GRID_WIDTH){
			active.x++;
			if (touchesPieceSide(grid,active)==0){
				active.x--;
			}
			else{
				check_time=getCheckTime(CHECK_TIMEOUT);
			}
		}
	}
	else if (key=='w'||key=='W'){
		piece newp=active;
		int i,j;
		rotate_arr(newp.grid,4);
		int wid = newp.wid;
			newp.wid=newp.hei;
			newp.hei=wid;
		
		
		for(i=0;i<4&&newp.x>=0;i++){
		if (touchesPieceSide(grid,newp)!=0&&newp.x+newp.wid-1<GRID_WIDTH&&newp.y+newp.hei-1<GRID_HEIGHT){
			active=newp;
			check_time=getCheckTime(CHECK_TIMEOUT);
			break;
		}
		newp.x--;
		}
	}
	else if (key=='s'||key=='S'){
		if (active.y+active.hei<GRID_HEIGHT&&touchesPieceTop(grid,active)==1){
			active.y++;
			score++;
			down_time=getCheckTime(speed);
			check_time=getCheckTime(CHECK_TIMEOUT);
		}
		
	}
	return active;
}

int touchesWall(char p[PIECE_SIZE][PIECE_SIZE]){
	int i,j;
	int wid=0,hei=0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(p[i][j]==BLOCK){
				if (i>hei)hei=i;
				if (j>wid)wid=j;
			}
		}
	}
	return 1;
}

int touchesPieceTop(char grid[GRID_HEIGHT][GRID_WIDTH],piece block){
	int i,j;
	for (i=0;i<PIECE_SIZE;i++){
		for (j=0;j<PIECE_SIZE;j++){
			if (block.grid[i][j]==BLOCK){
				if (block.y+i+1<GRID_HEIGHT&&block.y+i+1>=0)
				if (grid[block.y+i+1][j+block.x]==BLOCK){
					return 0;
				}
			}
		}	
	}
	return 1;
}

int touchesPieceSide(char grid[GRID_HEIGHT][GRID_WIDTH],piece block){
	int i,j;
	for (i=0;i<PIECE_SIZE;i++){
		for (j=0;j<PIECE_SIZE;j++){
			if (block.grid[i][j]==BLOCK){
				if (block.x+j<GRID_WIDTH&&block.y+i+1>=0)
				if (grid[block.y+i][j+block.x]==BLOCK){
					return 0;
				}
			}
		}	
	}
	return 1;
}

void rotate_arr(char arr[PIECE_SIZE][PIECE_SIZE],int size) {

    char nw[PIECE_SIZE][PIECE_SIZE];
	char ag[PIECE_SIZE][PIECE_SIZE];

    int col,row;
    int minRow=5,minCol=5;

    for (row = 0; row < size; row++) {

        for (col = 0; col < size; col++) {

            nw[col][size-row-1] = arr[row][col];
            arr[row][col]=' ';
            
            

        }

    }

    for (row = 0; row < size; row++) {

        for (col = 0; col < size; col++) {

            ag[row][col] = nw[row][col];
			if(ag[row][col]==BLOCK){
				if(row<minRow)minRow=row;
				if(col<minCol)minCol=col;
			}
        }

    }
    
     for (row = minRow; row < size; row++) {

        for (col = minCol; col < size; col++) {

            arr[row-minRow][col-minCol] = ag[row][col];

        }

    }
    
    

}

int getSteps(int no,int limit){ //1
  		int totalDigits = 0;

  		while(no!=0){
   	 //4
    	no = no/10;
    	totalDigits ++;
  		}
  		if(totalDigits>limit)return limit;
  		return totalDigits;
	}
