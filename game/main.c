/**SPACE INVADERS was created with SDL2 in C by Yavuz Selim Corakli.**/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define E_WIDTH 20
#define E_HEIGHT 20
#define P_WIDTH 40
#define P_HEIGHT 10
#define BLT_WIDTH 10
#define BLT_HEIGHT 20
#define BASE_WIDTH 50
#define BASE_HEIGHT 50

void initialize();
void menu();
void game();
void enemy();
void enemyMove();
void player();
void playerMove();
void bulletPlayer(int *pos_x, int *pos_y);
void bulletEnemy(int *pos_x, int *pos_y);
void bulletMove();
void base();
void baseMove();
void score();
int gameOver();

SDL_Window *screen = NULL;
SDL_Renderer *renderer = NULL;

SDL_Event event;

int static gameStatus = 0;

SDL_Rect enemyRect1[20];
SDL_Rect enemyRect2[20];
SDL_Rect enemyRect3[10];
int e_speed = 25;
int e_landing = 10;

SDL_Rect playerRect;

SDL_Rect bulletPlayerRect;
SDL_Rect bulletEnemyRect;

SDL_Rect baseRect[4];

//Game main stream.
int main(int argc, char *argv[])
{
    initialize();
    menu();

    do{
        game();
        gameStatus = gameOver();
    }while(gameStatus == 1);

    return 0;
}

//SDL systems and screen initialized.
void initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL subsystems could NOT be initialized!", SDL_GetError());
        return 1;
    }
    else
    {
        SDL_Log("SDL subsystems were initialized.");
    }

    screen = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(screen == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Screen could NOT be created!", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    else
    {
        SDL_Log("Screen was created.");
    }
}

//Game main menu.
void menu()
{
    renderer = SDL_CreateRenderer(screen, -1, 0);

    SDL_Surface *surfaceMenu = SDL_LoadBMP("menu.bmp");
    SDL_Texture *textureMenu = SDL_CreateTextureFromSurface(renderer, surfaceMenu);
    SDL_FreeSurface(surfaceMenu);

    SDL_Surface *surfaceLogo = SDL_LoadBMP("logo.bmp");
    SDL_Texture *textureLogo = SDL_CreateTextureFromSurface(renderer, surfaceLogo);
    SDL_FreeSurface(surfaceLogo);

    SDL_Surface *surfaceOptions = SDL_LoadBMP("options.bmp");
    SDL_Texture *textureOptions = SDL_CreateTextureFromSurface(renderer, surfaceOptions);
    SDL_FreeSurface(surfaceOptions);

    SDL_Event event;

    while(1)
    {
        SDL_RenderCopy(renderer, textureMenu, NULL, NULL);

        SDL_Rect logoRect = {100, 0, 400, 400};
        SDL_RenderCopy(renderer, textureLogo, NULL, &logoRect);

        SDL_Rect optionsRect = {150, 300, 300, 250};
        SDL_RenderCopy(renderer, textureOptions, NULL, &optionsRect);

        SDL_RenderPresent(renderer);

        int start;
        while(SDL_PollEvent(&event))
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                start = 1;
                break;
            }
            else if(event.key.keysym.sym == SDLK_q)
            {
                SDL_Log("Screen was closed.");
                SDL_Log("SDL subsystems were shut down.");
                SDL_Quit();
            }
        }

        if(start == 1)
        {
            break;
        }

        SDL_RenderClear(renderer);
    }

    SDL_DestroyTexture(textureMenu);
    SDL_DestroyTexture(textureLogo);
    SDL_DestroyTexture(textureOptions);
}

//Game screen.
void game()
{
    SDL_Surface* surfaceGame = surfaceGame = SDL_LoadBMP("gameplay.bmp");
    SDL_Texture* textureGame = textureGame = SDL_CreateTextureFromSurface(renderer, surfaceGame);
    SDL_FreeSurface(surfaceGame);

    base();
    enemy();
    player();

    while(1)
    {
        SDL_RenderCopy(renderer, textureGame, NULL, NULL);
        SDL_RenderPresent(renderer);

        baseMove();
        enemyMove();
        playerMove();
        bulletEnemyMove();
        bulletPlayerMove();

        if(playerRect.y == -500)
        {
            return;
        }

        for(size_t m = 0; m < 20; m++)
        {
            if(enemyRect1[m].y <= -1000 && enemyRect2[m].y <= -1000)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        for(size_t m = 0; m < 10; m++)
        {
            if(enemyRect3[m].y <= -500)
            {
                if(m == 9 && enemyRect3[9].y <= -1000)
                {
                    return;
                }
                continue;
            }
            else
            {
                break;
            }
        }

        for(size_t m = 0; m < 20; m++)
        {
            if(enemyRect1[m].y == 500 || enemyRect2[m].y == 500 /*|| enemyRect3[m].y == 500*/)
            {
                return;
            }
            else
            {
                continue;
            }
        }

        SDL_RenderClear(renderer);
    }

    SDL_DestroyTexture(textureGame);
}

//Enemies created and placed.
void enemy()
{
    for(size_t i = 0; i < 20; i++)
    {
        if(i < 10)
        {
            enemyRect1[i].x = 25 * i;
            enemyRect1[i].y = 125;
        }
        else
        {
            enemyRect1[i].x = 25 *(19 - i);
            enemyRect1[i].y = 150;
        }
        enemyRect1[i].w = E_WIDTH;
        enemyRect1[i].h = E_HEIGHT;
    }

    for(size_t i = 0; i < 20; i++)
    {
        if(i < 10)
        {
            enemyRect2[i].x = 25 * i;
            enemyRect2[i].y = 75;
        }
        else
        {
            enemyRect2[i].x = 25 *(19 - i);
            enemyRect2[i].y = 100;
        }
        enemyRect2[i].w = E_WIDTH;
        enemyRect2[i].h = E_HEIGHT;
    }

    for(size_t i = 0; i < 10; i++)
    {
        enemyRect3[i].x = 25 * i;
        enemyRect3[i].y = 50;
        enemyRect3[i].w = E_WIDTH;
        enemyRect3[i].h = E_HEIGHT;
    }
}

//Enemies move and shoot.
int static counter = 0;
void enemyMove()
{
    SDL_SetRenderDrawColor(renderer, 128, 128, 0, 0);
    for(size_t j = 0; j < 20; j++)
    {

        SDL_RenderFillRect(renderer, &enemyRect1[j]);
        enemyRect1[j].x += e_speed;
    }

    SDL_SetRenderDrawColor(renderer, 0, 128, 128, 0);
    for(size_t j = 0; j < 20; j++)
    {

        SDL_RenderFillRect(renderer, &enemyRect2[j]);
        enemyRect2[j].x += e_speed;
    }

    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 0);
    for(size_t j = 0; j < 10; j++)
    {
        SDL_RenderFillRect(renderer, &enemyRect3[j]);
        enemyRect3[j].x += e_speed;
    }

    srand(time(NULL));
    int shooter = rand() % 20;
    if(counter % 35 == 0)
    {
        if(enemyRect1[shooter].y >= 0)
        {
            bulletEnemy(&enemyRect1[shooter].x, &enemyRect1[shooter].y);
        }
        else
        {
            if(enemyRect2[shooter].y >= 0)
            {
                bulletEnemy(&enemyRect2[shooter].x, &enemyRect2[shooter].y);
            }
            else
            {
                if(enemyRect3[shooter].y >= 0)
                {
                    bulletEnemy(&enemyRect3[shooter].x, &enemyRect3[shooter].y);
                }
                else
                {

                }
            }
        }
    }
    counter++;

    if(enemyRect1[9].x + enemyRect1[9].w >= SCREEN_WIDTH - 10 || enemyRect1[0].x <= 0)
    {
        e_speed = -e_speed;
        for(size_t j = 0; j < 20; j++)
        {
            enemyRect1[j].y += e_landing;
        }
        for(size_t j = 0; j < 20; j++)
        {
            enemyRect2[j].y += e_landing;
        }
        for(size_t j = 0; j < 10; j++)
        {
            enemyRect3[j].y += e_landing;
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(75);
}

//Player created and placed.
void player()
{
    playerRect.x = 0;
    playerRect.y = 500;
    playerRect.w = P_WIDTH;
    playerRect.h = P_HEIGHT;
}

//Player moved and shot.
void playerMove()
{
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    if(playerRect.x >= (P_WIDTH - 20))
                    {
                        playerRect.x -= 20;
                    }
                    break;
                case SDLK_RIGHT:
                    if(playerRect.x <= (SCREEN_WIDTH - (P_WIDTH + 20)))
                    {
                        playerRect.x += 20;
                    }
                    break;
                case SDLK_SPACE:
                    {
                        bulletPlayer(&playerRect.x, &playerRect.y);
                        break;
                    }
                default:
                    continue;
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 100, 255, 150, 255);
    SDL_RenderFillRect(renderer, &playerRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(75);
}

//Bullet used by enemies created and placed by enemy location.
void bulletEnemy(int *pos_x, int *pos_y)
{
    bulletEnemyRect.x = *pos_x;
    bulletEnemyRect.y = *pos_y + 20;
    bulletEnemyRect.w = BLT_WIDTH;
    bulletEnemyRect.h = BLT_HEIGHT;
}

//Bullet used by enemies move and goes to player or bases to destroy.
void bulletEnemyMove()
{
    int speed = 25;
    if(bulletEnemyRect.y <= 500)
    {
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 0);
        SDL_RenderFillRect(renderer, &bulletEnemyRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(25);

        bulletEnemyRect.y += speed;

        for(size_t k = 0; k < 4; k++)
        {
            if(bulletEnemyRect.y + bulletEnemyRect.h >= baseRect[k].y &&
            bulletEnemyRect.x + bulletEnemyRect.w >= baseRect[k].x &&
            bulletEnemyRect.x <= baseRect[k].x + baseRect[k].w)
            {
                bulletEnemyRect.y = -150;
                bulletEnemyRect.x = -150;
                baseRect[k].x = -200;
                baseRect[k].y = -200;
                break;
            }
        }
        for(size_t k = 0; k < 1; k++)
        {
            if(bulletEnemyRect.y + bulletEnemyRect.h >= playerRect.y &&
            bulletEnemyRect.x + bulletEnemyRect.w >= playerRect.x &&
            bulletEnemyRect.x <= playerRect.x + playerRect.w)
            {
                bulletEnemyRect.y = -150;
                bulletEnemyRect.x = -150;
                playerRect.y = -500;
                break;
            }
        }
    }
}


//Bullet used by player created and placed by player location.
void bulletPlayer(int *pos_x, int *pos_y)
{
    bulletPlayerRect.x = *pos_x + 15;
    bulletPlayerRect.y = *pos_y - 20;
    bulletPlayerRect.w = BLT_WIDTH;
    bulletPlayerRect.h = BLT_HEIGHT;
}

//Bullet used by player move and goes to enemies to kill also can damage bases.
void bulletPlayerMove()
{
    int speed = 75;
    if(bulletPlayerRect.y >= 10)
    {
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 0);
        SDL_RenderFillRect(renderer, &bulletPlayerRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(25);

        bulletPlayerRect.y -= speed;

        for(size_t k = 0; k < 4; k++)
        {
            if(bulletPlayerRect.y <= baseRect[k].y + baseRect[k].h &&
            bulletPlayerRect.x >= baseRect[k].x &&
            bulletPlayerRect.x <= baseRect[k].x + baseRect[k].w)
            {
                bulletPlayerRect.y = -100;
                bulletPlayerRect.x = -100;
                baseRect[k].x = -200;
                baseRect[k].y = -200;
                break;
            }
        }

        for(size_t k = 0; k < 20; k++)
        {
            if(bulletPlayerRect.y <= enemyRect1[k].y + enemyRect1[k].h &&
                bulletPlayerRect.x >= enemyRect1[k].x &&
                bulletPlayerRect.x <= enemyRect1[k].x + enemyRect1[k].w)
            {
                bulletPlayerRect.y = -100;
                bulletPlayerRect.x = -100;
                enemyRect1[k].y -= 4000;
                break;
            }
        }
        for(size_t k = 0; k < 20; k++)
        {
            if(bulletPlayerRect.y <= enemyRect2[k].y + enemyRect2[k].h &&
                bulletPlayerRect.x >= enemyRect2[k].x &&
                bulletPlayerRect.x <= enemyRect2[k].x + enemyRect2[k].w)
            {
                bulletPlayerRect.y = -100;
                bulletPlayerRect.x = -100;
                enemyRect2[k].y -= 4000;
                break;
            }
        }
        for(size_t k = 0; k < 10; k++)
        {
            if(bulletPlayerRect.y <= enemyRect3[k].y + enemyRect1[k].h &&
                bulletPlayerRect.x >= enemyRect3[k].x &&
                bulletPlayerRect.x <= enemyRect3[k].x + enemyRect1[k].w)
            {
                bulletPlayerRect.y = -100;
                bulletPlayerRect.x = -100;
                enemyRect3[k].y -= 4000;
                break;
            }
        }
    }
}

//Bases created and placed.
void base()
{
    for(size_t i = 0; i < 4; i++)
    {
        baseRect[i].x = 150 * (i + 1);
        baseRect[i].y = 350;
        baseRect[i].w = BASE_WIDTH;
        baseRect[i].h = BASE_HEIGHT;
    }
}

//Bases protect player from enemies.
void baseMove()
{
    SDL_SetRenderDrawColor(renderer, 100, 225, 150, 255);
    for(size_t j = 0; j < 4; j++)
    {
        SDL_RenderFillRect(renderer, &baseRect[j]);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}

//Scoreboard for numbers of enemies killed by player.
void score()
{

}

//Game end.
int gameOver()
{
    SDL_Surface *surfaceGameOver = SDL_LoadBMP("gameover.bmp");
    SDL_Texture *textureGameOver = SDL_CreateTextureFromSurface(renderer, surfaceGameOver);
    SDL_FreeSurface(surfaceGameOver);

    while(1)
    {
        SDL_RenderCopy(renderer, textureGameOver, NULL, NULL);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event))
        {
            if(event.key.keysym.sym == SDLK_q)
            {
                SDL_Log("Screen was closed.");
                SDL_Log("SDL subsystems were shut down.");
                SDL_DestroyWindow(screen);
                SDL_Quit();
            }
            else if(event.key.keysym.sym == SDLK_p)
            {
                SDL_RenderClear(renderer);
                return 1;
            }
        }
    }

    SDL_DestroyTexture(textureGameOver);
}
/**Space Invaders 1.0**/
