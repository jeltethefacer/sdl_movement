#include <SDL2/SDL.h>
#define WIDTH 1080
#define HEIGHT 1080

typedef struct Game
{
    SDL_Window *window;
    SDL_Surface *surface;
} Game;

typedef struct Square
{
    SDL_Rect *rect;
    Uint32 color;
} Square;

typedef struct Player
{
    int x;
    int y;
} Player;

typedef struct Point
{
    unsigned int x;
    unsigned int y;
} Point;

Game *init_game();
Player *init_player();
int destroy_game(Game *);
int draw_squares(Square * [8][8], Game *);
int make_chessboard(Square * [8][8], Game *);
int print_points(Point *[], int);
int add_building(Point, Point **, int);
int place_buildings(Point *[], Square * [8][8], Game *, int);

int main()
{
    Game *pGame = init_game();
    Player *pPlayer = init_player();
    Square *chessboard[8][8];
    Point *buildings[8 * 8];
    Point temp_point;
    int quit = 0, amount_buildings = 0;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        { //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN)
            { //Select surfaces based on key press
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (pPlayer->y > 0)
                        pPlayer->y--;
                    break;
                case SDLK_DOWN:
                    if (pPlayer->y < 7)
                        pPlayer->y++;
                    break;
                case SDLK_LEFT:
                    if (pPlayer->x > 0)
                        pPlayer->x--;
                    break;
                case SDLK_RIGHT:
                    if (pPlayer->x < 7)
                        pPlayer->x++;
                    break;
                case SDLK_b:
                    temp_point.x = pPlayer->x;
                    temp_point.y = pPlayer->y;
                    amount_buildings = add_building(temp_point, buildings, amount_buildings);

                    printf("%d\n", amount_buildings);
                    print_points(buildings, amount_buildings);
                    break;
                }
            }
        }
        make_chessboard(chessboard, pGame);

        place_buildings(buildings, chessboard, pGame, amount_buildings);
        chessboard[pPlayer->x][pPlayer->y]->color = SDL_MapRGB(pGame->surface->format, 255, 0, 0);
        draw_squares(chessboard, pGame);


        SDL_UpdateWindowSurface(pGame->window);
    }

    SDL_Delay(2000);

    destroy_game(pGame);

    return 0;
}

int add_building(Point place, Point *buidlings[], int amount_buildings)
{
    Point *pBuilding = malloc(sizeof(Point));
    pBuilding->x = place.x;
    pBuilding->y = place.y;

    buidlings[amount_buildings] = pBuilding;
    return ++amount_buildings;
}

int place_buildings(Point *points[], Square *square_array[8][8], Game *game, int n)
{
    for (int x = 0; x < n; x++)
    {
        square_array[points[x]->x][points[x]->y]->color = SDL_MapRGB(game->surface->format, 0, 255, 0);
    }
}

int print_points(Point *points[], int n)
{
    for (int x = 0; x < n; x++)
    {
        printf("%d, %d\n", points[x]->x, points[x]->y);
    }
}

int draw_squares(Square *square_array[8][8], Game *game)
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Square *square = square_array[x][y];
            int color = square_array[x][y]->color;
            SDL_FillRect(game->surface, square->rect, color);
        }
    }
}

int make_chessboard(Square *chessboard[8][8], Game *game)
{
    int color = 255;
    for (int y = 0; y < 8; y++)
    {
        if (y % 2 == 0)
        {
            color = 255;
        }
        else
        {
            color = 0;
        }
        for (int x = 0; x < 8; x++)
        {
            Square *square = malloc(sizeof(Square));
            SDL_Rect *rect = malloc(sizeof(SDL_Rect));
            rect->h = HEIGHT / 8;
            rect->w = WIDTH / 8;
            rect->y = 0 + HEIGHT / 8 * x;
            rect->x = 0 + WIDTH / 8 * y;
            square->rect = rect;
            if (color)
            {
                color = 0;
            }
            else
            {
                color = 255;
            }
            square->color = SDL_MapRGB(game->surface->format, color, color, color);
            chessboard[y][x] = square;
        }
    }
}

Player *init_player()
{
    Player *player = malloc(sizeof(Player));
    player->x = 0;
    player->y = 0;
    return player;
}

Game *init_game()
{
    Game *game = malloc(sizeof(Game));

    SDL_Init(SDL_INIT_VIDEO);

    game->window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    game->surface = SDL_GetWindowSurface(game->window);
}

int destroy_game(Game *game)
{
    SDL_DestroyWindow(game->window);

    SDL_Quit();
    return 0;
}