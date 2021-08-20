//TODO: add useless mechanic that spawns something
//on a per chance per event basis
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int MAX_SCORE;
int MAX_HEALTH = 10;

//color codes
#define TITLE_COLOR 1
#define GRASS_COLOR 2
#define STONE_COLOR 3
#define SCORE_COLOR 4
#define DESTROY_COLOR 5
#define BUILD_COLOR 6
#define ENEMY_COLOR 7
#define HEALTH_COLOR 8

//useful junk
int maxY, maxX, mvY, mvX, grassY, grassX;
int score, stoneY[1025], stoneX[1025], listGrassY[1025], listGrassX[1025];
int playerY, playerX, enemyY = 3, enemyX = 3, oldY = -1, oldX = -1;
int diff, diffRand[3], dropVar;
char difficulty[3], EASY[3], MEDI[3], HARD[3], health[10] = "**********";
char block = '#';

int indexX;
void healthBar(int trigger)
{
    if (trigger == -1)
    {
        //health[indexX] = ' ';
        mvaddch(0, indexX, ' ');
        MAX_HEALTH--;
        dropVar++;
    }
    indexX++;
    refresh ();
}

void healthDisplay (int diff)
{
    int lastI;
    for (unsigned int i = 0; i < strlen(health); i++)
    {
        mvaddch(0, i, health[i]);
        if (diff == 0 && i == strlen(health)-1)
        {
            MAX_HEALTH = 10;
            lastI = 9;
            break;
        }
        if (diff == 1 && i == 7) 
        {
            MAX_HEALTH = 8;
            lastI = i;
            break; 
        }
        if (diff == 2 && i == 4) 
        {
            MAX_HEALTH = 5;
            lastI = i;
            break; 
        }
    }
//    mvprintw(0, lastI+1, "[%d*]", MAX_HEALTH-dropVar);

/*if only this worked, but fuck it, thats the least
* dysfunctional part of this so-called "game"
* it's better to count the hp yourself instead of
* relying on something that doesn't work to do it
*/

}

/*
void blockSearch ()
{
    for (int d = 10; d > 0; d--);
}
*/

void enemyMaker (int range)
{
    //prevent enemy trail
    mvaddch(oldY, oldX, ' ');
    playerY = mvY, playerX = mvX;
    //make enemy more challenging depending on diff
    switch (diff)
    {
        case 0: 
            diffRand[0] = 2;
            diffRand[1] = 0;
            diffRand[2] = 6;
            diffRand[3] = 2;
            break;
        case 1: 
            diffRand[0] = 3;
            diffRand[1] = 1;
            diffRand[2] = 10;
            diffRand[3] = 2;
            break;
        case 2: 
            diffRand[0] = 4;
            diffRand[1] = 1;
            diffRand[2] = 10;
            diffRand[3] = 1;
            break;
    }
    //highly advanced AI, it will kill you
    if (abs(playerY-enemyY) <= range && abs (playerX-enemyX) <= range)
    {
    //chance to determine if enemy wants to follow you
    //is based on difficulty
    //this is to make it less impossible to escape from enemy
        if (playerY < enemyY && rand()%diffRand[0] > diffRand[1]) 
        {
            enemyY--;
        }
        if (playerY > enemyY && rand()%diffRand[0] > diffRand[1]) 
        {
            enemyY++;
        }
        if (playerX < enemyX && rand()%diffRand[0] > diffRand[1]) 
        {
            enemyX--;
        }
        if (playerX > enemyX && rand()%diffRand[0] > diffRand[1]) 
        {
            enemyX++;
        }
    }
    else
    {
        if (rand()%diffRand[2] > diffRand[3] && enemyX > 1
            && enemyY > 1 && enemyX < maxX-1 &&
            enemyY < maxY-1)
        {
            if (enemyX < maxX-1) enemyX += rand()%2;
            enemyY += rand()%2;
            if (enemyY < maxY-1) enemyY += rand()%2;
//            enemyX -= rand()%2;
        }
        else if (enemyX > 1 && enemyY > 1 && enemyX < maxX-1
                 && enemyY < maxY-1)
        {
//            enemyX += rand()%2;
            if (enemyX > 1) enemyX -= rand()%2;
            enemyY -= rand()%2;
            if (enemyY > 1) enemyY -= rand()%2;
        }
    }
    attron (COLOR_PAIR(ENEMY_COLOR));
    mvaddch(enemyY, enemyX, '&');
    attroff (COLOR_PAIR(ENEMY_COLOR));
    if (playerX == enemyX && playerY == enemyY)
        healthBar(-1);
    if (playerX == enemyX && playerY == enemyY
        && MAX_HEALTH == 0)
    {
        endwin();
        printf("Enemy killed you and you lost! \nScore: %d\n", score);
        system("pkill mpv");
        exit(0);
    }
    oldY = enemyY, oldX = enemyX;
}

void scoreMath (int modifier, char key)
{
    for (int i = 0; i < 1024; i++)
    {
        //destroy key
        if (key == 'x')
        {  
            if (mvY == stoneY[i] && mvX == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
// using mpv to play music is fine, but to play game sounds it's slow
// using mpg123 makes cursor disappear for some reason and is still slow
//                system("mpg123 -q --no-control ~/code/minecraft-cli/break.mp3 >/dev/null 2>/dev/null");
            } 
            if (mvY-1 == stoneY[i] && mvX == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
            if (mvY == stoneY[i] && mvX-1 == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
            if (mvY+1 == stoneY[i] && mvX == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
            if (mvY == stoneY[i] && mvX+1 == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
/* grass shouldnt give points
            if (grassY == listGrassY[i] && grassX == listGrassX[i])
            {
                modifier = 1;
                score += modifier;
            }
            listGrassY[i] = -1, listGrassX[i] = -1;
*/
        }
        //DESTROY key
        if (key == 'v')
        {
            if (mvY == stoneY[i] && mvX == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
            if (mvY-1 == stoneY[i] && mvX == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
            if (mvY == stoneY[i] && mvX-1 == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
            if (mvY+1 == stoneY[i] && mvX == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
            if (mvY == stoneY[i] && mvX+1 == stoneX[i])
            {
                score += modifier;
                stoneY[i] = -1, stoneX[i] = -1;
            }
/*
            if (grassY == listGrassY[i] && grassX == listGrassX[i])
            {
                modifier = 1;
                score += modifier;
            }
            listGrassY[i] = -1, listGrassX[i] = -1;
*/
        }
    }
    attron(COLOR_PAIR(SCORE_COLOR));
    mvprintw(0, 18, "Score:  %d  ", score);
    attroff(COLOR_PAIR(SCORE_COLOR));
    if (score > MAX_SCORE)
    {
        endwin();
        printf("You won with the overflow score of %d points! Congrats and thanks for playing!\n", score);
        system("pkill mpv");
        exit(0);
    }
    if (score >= MAX_SCORE-1)
    {
        endwin();
        printf("You won with a score of %d points! Thanks for playing!\n", score);
        system("pkill mpv");
        exit(0);
    }
}

void customUpper(char diff[])
{
    for (unsigned int i = 0; i < strlen(diff); i++)
    {
        if (diff[i] > 96) diff[i] -= 32;
    }
}

int diffLen[2];
int diffcmp ()
{
    for (unsigned int i = 0; i < strlen(difficulty); i++)
    {
        if (difficulty[i] == EASY[i]) diffLen[0]++;
        if (difficulty[i] == MEDI[i]) diffLen[1]++;
        if (difficulty[i] == HARD[i]) diffLen[2]++;
        else continue;

        if (diffLen[0] >= 4 || diffLen[1] >= 4 || diffLen[2] >= 4
            || strlen(difficulty) < strlen (HARD)
            || i == 4) break;

    }
    if (diffLen[0] == 4) 
    {
        diff = 0;
        MAX_SCORE = 256; 
        return 1;
    }
    if (diffLen[1] == 4) 
    {
        diff = 1; 
        MAX_SCORE = 512; 
        return 1;
    }
    if (diffLen[2] == 4) 
    {
        diff = 2; 
        MAX_SCORE = 1024; 
        return 1;
    }
    return 0;
}

void diffSelect ()
{
    printf ("Pick difficulty from the following list:\n*EASY\n*MEDI\n*HARD\n\n>");
    scanf ("%s", difficulty);
//    customUpper(difficulty);

    if (strcasecmp(difficulty, "EASY") == 0) diff = 0; MAX_SCORE = 256; healthDisplay(0);
    if (strcasecmp(difficulty, "MEDI") == 0) diff = 1; MAX_SCORE = 512; healthDisplay(1);
    if (strcasecmp(difficulty, "HARD") == 0) diff = 2; MAX_SCORE = 1024; healthDisplay(2);

/* TODO: find a better way of doing this
    while (1)
    {
        if (diffcmp() == 0)
        {
            printf ("\nInvalid difficulty! Please try entering a difficulty again.\n\n>");
            if (diffcmp()) break;
            scanf ("%s", difficulty);
            continue;
        }
        else break;
    }
*/
}

int main ()
{
    diffSelect();
    //system("mpv --no-terminal --no-audio-display ~/code/minecraft-cli/Haggstrom.mp3 &");
    initscr();
    cbreak();
    noecho();
    if (!has_colors())
        printf ("Your terminal does not support colors!\n");
    start_color();
    srand(time(0));

    //color codes' code
    init_pair (TITLE_COLOR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair (GRASS_COLOR, COLOR_BLACK, COLOR_GREEN);
    init_pair (STONE_COLOR, COLOR_BLACK, COLOR_RED);
    init_pair (SCORE_COLOR, COLOR_WHITE, COLOR_BLUE);
    init_pair (DESTROY_COLOR, COLOR_BLACK, COLOR_WHITE);
    init_pair (BUILD_COLOR, COLOR_MAGENTA, COLOR_GREEN);
    init_pair (ENEMY_COLOR, COLOR_WHITE, COLOR_BLUE);
    init_pair (HEALTH_COLOR, COLOR_RED, COLOR_BLACK);

    WINDOW * win = newwin (0, 0, 1, 0);

//    mvprintw (0, indexX, health);
//    printf ("%d\n", diff); //for debugging
    attron (COLOR_PAIR(HEALTH_COLOR));
    healthDisplay(diff);
    attroff (COLOR_PAIR(HEALTH_COLOR));

    attron(COLOR_PAIR(SCORE_COLOR));
    mvprintw(0, 18, "Score:  %d  ", score);
    attroff(COLOR_PAIR(SCORE_COLOR));

    refresh ();
    box (win, 0, 0);
    getmaxyx(win, maxY, maxX);
    wrefresh (win);
    attron (COLOR_PAIR(TITLE_COLOR));
    mvprintw (0, maxX/2-5, "Conner Indev");
    attroff (COLOR_PAIR(TITLE_COLOR));
    for (int i = 0; i < 1024; i++)
    {
        grassY = rand()%150, grassX = rand()%150; 
        mvY = rand()%300, mvX = rand()%300;
        stoneY[i]=mvY, stoneX[i]=mvX;
        listGrassY[i]=grassY, listGrassX[i]=grassX;
        mvwaddch(win, grassY, grassX, rand()%127
            | COLOR_PAIR(GRASS_COLOR));
        mvwaddch(win, mvY, mvX, rand()%127
                | COLOR_PAIR(STONE_COLOR));
    }
    wrefresh(win);
    wmove (win, maxY/2, maxX/2);
    mvY = maxY/2, mvX = maxX/2;
    while (1)
    {
        //difficulty handler, printfs are for debugging
        switch (diff)
        {
            case 0: 
                enemyMaker(10); 
//                printf("%d", diff);
                break;
            case 1: 
                enemyMaker(15); 
//                printf("%d", diff);
                break;
            case 2: 
                enemyMaker(25); 
//                printf("%d", diff);
                break;
        }
        refresh ();
        switch (wgetch(win))
        {
            //movement keys
            case 104: wmove (win, mvY, mvX--); refresh(); break; //'h'
            case 106: wmove (win, mvY++, mvX); refresh(); break; //'j'
            case 107: wmove (win, mvY--, mvX); refresh(); break; //'k'
            case 108: wmove (win, mvY, mvX++); refresh(); break; //'l'

            case 97:  wmove (win, mvY, mvX--); refresh(); break; //'a'
            case 115: wmove (win, mvY++, mvX); refresh(); break; //'s'
            case 119: wmove (win, mvY--, mvX); refresh(); break; //'w'
            case 100: wmove (win, mvY, mvX++); refresh(); break; //'d'

            case 0x44: wmove (win, mvY, mvX--); refresh(); break; //'left'
            case 0x42: wmove (win, mvY++, mvX); refresh(); break; //'down'
            case 0x41: wmove (win, mvY--, mvX); refresh(); break; //'up'
            case 0x43: wmove (win, mvY, mvX++); refresh(); break; //'right'
            //quit key
            case 113: //'q'
                endwin();
                printf("Hope you enjoyed playing. See you next time! \nScore: %d\n", score);
                system("pkill mpv");
                exit(0);
            //block selection keys
            case 49: //'1'
                block = '|';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: |   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 50: //'2'
                block = '^';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: ^   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 51: //'3'
                block = '*';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: *   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 52: //'4'
                block = '#';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: #   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 53: //'5'
                block = '!';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: !   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 54: //'6'
                block = '~';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: ~   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 55: //'7'
                block = '.';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: .   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 56: //'8'
                block = ',';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: ,   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 57: //'9'
                block = '/';
                mvprintw (0,0, "           ");
                attron(A_STANDOUT);
                mvprintw (0,0, "Block: /   ");
                attroff(A_STANDOUT);
                refresh(); break;
            case 48: //'0'
                mvprintw (0,0, "           ");
                refresh(); break;
            //build and destroy keys
            case 103: mvwaddch(win, mvY, mvX, block); beep(); //'g'
                refresh();
                mvprintw(0, maxX-20, "                    ");
                attron(COLOR_PAIR(BUILD_COLOR));
                mvprintw(0, maxX-13, "Mode: Build");
                attroff(COLOR_PAIR(BUILD_COLOR));
                break;
            case 120: mvwaddch(win, mvY, mvX, ' '); //'x'
                mvprintw(0, maxX-20, "                    ");
                attron(COLOR_PAIR(DESTROY_COLOR));
                mvprintw(0, maxX-13, "Mode: Destroy");
                attroff(COLOR_PAIR(DESTROY_COLOR));
                scoreMath(5, 'x'); 
                refresh();
//              scoreMath(1, 'g');                
                break;
            case 118: mvwprintw(win, mvY, mvX, "    "); //'v'
                mvprintw(0, maxX-20, "                    ");
                attron(COLOR_PAIR(STONE_COLOR));
                mvprintw(0, maxX-13, "Mode: DESTROY");
                attroff(COLOR_PAIR(STONE_COLOR));
                scoreMath(25, 'v');
                mvprintw (0, 0, "           ");
                attron (COLOR_PAIR(HEALTH_COLOR));
                healthDisplay(diff);
                attroff (COLOR_PAIR(HEALTH_COLOR));
                refresh();
                break; 
        }
    }
    getch();
    refresh();    
    printf("Hope you enjoyed playing. See you next time!\n");
    endwin();
    system("pkill mpv");
    exit(0);
}
