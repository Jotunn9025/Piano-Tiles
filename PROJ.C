#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <time.h>  

#define col1 0
#define col2 160
#define col3 320
#define col4 480

#define MAX_BOXES 8
#define NUM_COLUMNS 4
int columns[] = {col1, col2, col3, col4};
int end=0;
int speed=4,score=0;

struct Box {
    int x;
    int y;
    int active;
    int counter;
    int caught; 
};

struct Box boxes[MAX_BOXES];

void drawBorder();
void drawRect(int column, int y, int color);
void eraseRect(int column, int y);
void moveRect(struct Box* box);
int isColumnOccupied(int column);
struct Box* findLatestBox();
void updateWorld();
void floodFill(int column, int y);

void drawBorder() {
    setcolor(WHITE);
    line(160, 0, 160, getmaxy());
    line(320, 0, 320, getmaxy());
    line(480, 0, 480, getmaxy());
    setcolor(RED);
    line(0, 420, getmaxx(), 420);
}

void drawRect(int column, int y, int color) {
    int i;
    setcolor(color);
    for (i = 0; i < 200; i++) {
        line(column + 1, y + i, column + 159, y + i);
    }
}

void eraseRect(int column, int y) {
    int i;
    setcolor(BLACK); 
    for (i = 0; i < 200; i++) {
        line(column + 1, y + i, column + 159, y + i);
    }
}

void moveRect(struct Box* box) {
    int newColumn;
    struct Box* latestBox = findLatestBox();
    int color,i;

    if (box->active == 1) {
        color = (box->caught == 1) ? GREEN : LIGHTGRAY;

        setcolor(BLACK);
        for (i = 0; i < speed; i++) {
            line(box->x + 1, box->y + i, box->x + 159, box->y + i);
        }

        box->y += speed;

        setcolor(color);
        for (i = 200 - speed; i < 200; i++) {
            line(box->x + 1, box->y + i, box->x + 159, box->y + i);
        }

        if (box->caught == 1) {
            floodFill(box->x, box->y);
        }
    }

    if (box->y > 480) {
	if(box->caught==0)
	end=1;
        eraseRect(box->x, box->y);
        box->active = 0;
        box->y = -200;

        newColumn = columns[rand() % NUM_COLUMNS];
        while (isColumnOccupied(newColumn)) {
            newColumn = columns[rand() % NUM_COLUMNS];
        }
        box->x = newColumn;

        if (latestBox != NULL) {
            box->counter = latestBox->counter + 1;
        }
        box->caught = 0;
    }
}
int isColumnOccupied(int column) {
    int i;
    for (i = 0; i < MAX_BOXES; i++) {
        if (boxes[i].active && boxes[i].x == column && boxes[i].y >= -200 && boxes[i].y <= 0) {
            return 1; 
        }
    }
    return 0;
}

struct Box* findLatestBox() {
    int i;
    struct Box* latestBox = NULL;
    for (i = 0; i < MAX_BOXES; i++) {
        if (boxes[i].active) {
            if (latestBox == NULL || boxes[i].counter > latestBox->counter) {
                latestBox = &boxes[i];
            }
        }
    }
    return latestBox;
}

void updateWorld() {
    int i;
    char scoreText[25];
    struct Box* latestBox = findLatestBox();
    sprintf(scoreText, "SCORE: %d", score);
    if (end){
	cleardevice();
	settextstyle(3,0,8);
	setcolor(RED);
	outtextxy(10,240,"GAME OVER");
	setcolor(9);
	outtextxy(10,20,scoreText);
	getch();
	getch();
	exit(0);
}
    for (i = 0; i < MAX_BOXES; i++) {
        moveRect(&boxes[i]);
    }

    if (latestBox != NULL && latestBox->y == 0) {
        for (i = 0; i < MAX_BOXES; i++) {
            if (boxes[i].active == 0) {
                int column = columns[rand() % NUM_COLUMNS];
                while (isColumnOccupied(column)) {
                    column = columns[rand() % NUM_COLUMNS];
                }
                boxes[i].x = column;  
                boxes[i].y = -200;    
                drawRect(boxes[i].x, boxes[i].y, WHITE);  
                boxes[i].active = 1;  
                
                if (latestBox != NULL) {
                    boxes[i].counter = latestBox->counter + 1;  
                }
                break;
            }
        }
    }
    drawBorder();
    settextstyle(3,0,5);
    
    setfillstyle(1,WHITE);
    bar(0,0,textwidth(scoreText),60);
    setcolor(9);
    outtextxy(10,10,scoreText);
}


void floodFill(int column, int y) {
    int i;
    setcolor(GREEN);
    for (i = 0; i < 200; i++) {
        line(column + 1, y + i, column + 159, y + i);
    }
}

void main() {
    int gd = DETECT, gm;
    int i;
    char ch;
    int columnToFill; 
    int j;  
    int boxFound = 0; 
    srand(time(NULL));  

    for (i = 0; i < MAX_BOXES; i++) {
        boxes[i].y = -200;  
        boxes[i].active = 0;  
        boxes[i].counter = 0;  
        boxes[i].caught = 0;
    }

    printf("press any key to start game");
    getch();

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    drawBorder();  

    boxes[0].x = col2;  
    boxes[0].y = -200;  
    boxes[0].active = 1;  
    boxes[0].counter = 1;  

    while (!end) {  
        updateWorld();

        if (kbhit()) {
            ch = getch();
            columnToFill = -1;
            boxFound = 0;

            switch (ch) {
                case '1': columnToFill = col1; break;
                case '2': columnToFill = col2; break;
                case '3': columnToFill = col3; break;
                case '4': columnToFill = col4; break;
            }


            for (j = 0; j < MAX_BOXES; j++) {
                if (boxes[j].active && boxes[j].x == columnToFill && boxes[j].y >= 225 && boxes[j].y <= 420) {
                    floodFill(boxes[j].x, boxes[j].y);  
                    boxes[j].caught = 1;  
                    score++;  
                    boxFound = 1;  
                    break;  
                }
            }

            
            if (columnToFill != -1 && boxFound == 0) {
                end = 1;
            }
        }

        delay(10);
    }

    updateWorld();

    getch();
    closegraph();
}

