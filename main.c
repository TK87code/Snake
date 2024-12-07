#include "raylib.h"
#include "tk_list.h"
#include <stdlib.h> /* malloc() */
#include <string.h> /* strlen() */

#define TILE_SIZE 15
#define CELL_SIZE 16 /* Tile(grid) size in pixel */

#define FIELD_WIDTH 25 /* Field width in grids */
#define FIELD_HEIGHT 25 /* Field height in grids */

#define SCREEN_WIDTH FIELD_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT FIELD_HEIGHT * CELL_SIZE

#define SPEED 0.2

enum {
    SCENE_TITLE,
    SCENE_PLAY,
    SCENE_SCORE,
};

typedef struct snake_segment{
    int x, y;
}segment_t;

/*** Function Prototypes ***/

/* Creating 10 segments and store in a linked list */
tk_node_t* create_snake(void);
/* Routine for gameplay scene */
void scene_play(tk_node_t **snake, double *timer, int* dx, int* dy, int *food_x, int *food_y);
/* Routine for title screen */
void scene_title(int *scene);
/* Routine for score screen */
void scene_score(tk_node_t **snake, double *timer, int *scene, int *food_x, int *food_y);
/* Check if snake collide vs world & itself. Return 1 if true, and turn on "gameover" flag.
 otherwise 0.*/
int does_collide(tk_node_t *snake, int dx, int dy);
/* Draw snake */
void draw_snake(tk_node_t *snake);
/* Spawning food */
void spawn_food(tk_node_t *snake, int *food_x, int *food_y);

/*** Global Variables ***/
int game_over;
int score = 1;

int main(void)
{
    tk_node_t *snake;
    int scene;
    double timer;
    int dx, dy; /* Direction x & y where the snake is about to move. */
    int food_x, food_y;
    
    /* Raylib window initialization */
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);
    
    /* Game variable init */
    scene = SCENE_TITLE;
    timer = 0.0;
    dy = 0;
    dx = -1;
    
    /* Creating an initial snake */
    snake = create_snake();
    
    /* Spawn the first food */
    spawn_food(snake, &food_x, &food_y);
    
    while (!WindowShouldClose()){
        if (game_over != 0){
            game_over = 0;
            score = 1;
            dy = 0;
            dx = -1;
            timer = 0.0;
            scene = SCENE_SCORE;
        }
        
        /* Scene switching */
        switch(scene){
            case SCENE_TITLE:{
                scene_title(&scene);
            }break;
            
            case SCENE_PLAY:{
                scene_play(&snake, &timer, &dx, &dy, &food_x, &food_y);
            }break;
            
            case SCENE_SCORE:{
                scene_score(&snake, &timer, &scene, &food_x, &food_y);
            }
        }
    }
    
    tk_list_delete(&snake);
    CloseWindow();        
    
    return 0;
}

tk_node_t* create_snake(void)
{
    int i;
    const int initial_x = FIELD_WIDTH / 2;
    const int initial_y = FIELD_HEIGHT / 2;
    tk_node_t *snake = NULL;
    segment_t *data_to_add;
    
    for (i = 0; i < 10; i++){
        data_to_add = malloc(sizeof(segment_t));
        if (!data_to_add) exit(1);
        
        data_to_add->x = initial_x + i;
        data_to_add->y = initial_y;
        
        if (i == 0) snake = tk_list_create(data_to_add);
        else tk_list_push_back(snake, data_to_add);
    }
    
    return snake;
}

void draw_snake(tk_node_t *snake)
{
    tk_node_t *current;
    
    /* Draw the snake body segments */
    for (current = snake->next; current != NULL; current = current->next){
        DrawRectangle(((segment_t*)current->data)->x * CELL_SIZE, 
                      ((segment_t*)current->data)->y * CELL_SIZE, 
                      TILE_SIZE, TILE_SIZE, GREEN);
    }
    
    /* Draw head */
    DrawRectangle(((segment_t*)snake->data)->x * CELL_SIZE,
                  ((segment_t*)snake->data)->y * CELL_SIZE, 
                  TILE_SIZE, TILE_SIZE, DARKGREEN);
}

void scene_title(int *scene)
{
    if (IsKeyPressed(KEY_SPACE)){
        *scene = SCENE_PLAY;
        return;
    } 
    
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("SNAKE GAME", 
             SCREEN_WIDTH / 2 - (int)(strlen("SNAKE GAME") * 7), 
             SCREEN_HEIGHT / 2 - 40, 20, 
             GOLD);
    
    DrawText("Press SPACE to play.", 
             SCREEN_WIDTH / 2 - (int)(strlen("Press SPACE to play.") * 5), 
             SCREEN_HEIGHT / 2 + 20, 20, 
             WHITE);
    EndDrawing();
}

void scene_play(tk_node_t **snake, double *timer, int *dx, int *dy, int *food_x, int *food_y)
{
    int i;
    segment_t *new_segment;
    tk_node_t *backend_node;
    
    /* update timer */
    *timer += GetFrameTime();
    
    /* Input handling */
    if (IsKeyPressed(KEY_LEFT)){
        *dx = *dy = 0;
        *dx = -1;
    }
    if (IsKeyPressed(KEY_UP)){
        *dx = *dy = 0;
        *dy = -1;
    } 
    if (IsKeyPressed(KEY_RIGHT)){
        *dx = *dy = 0;
        *dx = 1;
    } 
    if (IsKeyPressed(KEY_DOWN)){
        *dx = *dy = 0;
        *dy = 1;
    } 
    
    if (*timer >= SPEED && !does_collide(*snake, *dx, *dy)){
        *timer = 0;
        new_segment = malloc(sizeof(segment_t));
        if (!new_segment) exit(1);
        
        new_segment->x = ((segment_t*)(*snake)->data)->x + *dx;
        new_segment->y = ((segment_t*)(*snake)->data)->y + *dy;
        
        /* Here, I am moving the snake by pushing front & popping back */
        tk_list_push_front(snake, new_segment);
        tk_list_pop_back(*snake);
    }
    
    /* Eating food */
    if (*food_x == ((segment_t*)(*snake)->data)->x &&
        *food_y == ((segment_t*)(*snake)->data)->y){
        backend_node = tk_list_get_back(*snake);
        
        /* Grow snake segment by 5 */
        for (i = 0; i < 5; i++){
            new_segment = malloc(sizeof(new_segment));
            if(!new_segment) exit(1);
            new_segment->x = ((segment_t*)backend_node->data)->x;
            new_segment->y = ((segment_t*)backend_node->data)->y;
            tk_list_push_back(*snake, new_segment);
        }
        
        spawn_food(*snake, food_x, food_y);
    }
    
    /* Even whe the snake collide and set the flag of game over, BeginDrawing() should be called because of event handling.*/
    BeginDrawing();
    ClearBackground(BLACK);
    draw_snake(*snake);
    /* Drawing food */
    DrawRectangle(*food_x * CELL_SIZE, *food_y * CELL_SIZE, TILE_SIZE, TILE_SIZE, ORANGE);
    EndDrawing();
}

void scene_score(tk_node_t **snake, double *timer, int *scene, int *food_x, int *food_y)
{
    *timer += GetFrameTime();
    
    /* Press Space bar to play again */
    if (IsKeyPressed(KEY_SPACE)){
        /* Delete the list, and create new */
        tk_list_delete(snake);
        *snake = create_snake();
        spawn_food(*snake, food_x, food_y);
        *scene = SCENE_PLAY;
        return;
    }
    
    
    if (*timer >= 0.05 && tk_list_pop_back(*snake) == 0){
        score += 1;
        *timer = 0;
    }
    
    BeginDrawing();
    ClearBackground(BLACK);
    
    if ((*snake)->next != NULL) draw_snake(*snake);
    
    DrawText("Oh Dear . . !", 
             (SCREEN_WIDTH / 2) - ((int)(strlen("Oh Dear . . !") * 5)), 
             (SCREEN_HEIGHT / 2 - 40), 20, GOLD);
    
    DrawText(TextFormat("Score: %d", score),
             (SCREEN_WIDTH / 2) - (10 * 5),
             (SCREEN_HEIGHT / 2), 20, GOLD);
    
    if ((*snake)->next == NULL){
        DrawText("Press SPACE to Play again.", 
                 (SCREEN_WIDTH / 2) - ((int)(strlen("Press SPACE to Play again.") * 5)), 
                 (SCREEN_HEIGHT / 2 + 40), 20, WHITE);
    }
    
    EndDrawing();
    
}

int does_collide(tk_node_t *snake, int dx, int dy)
{
    tk_node_t *current;
    int result = 0;
    
    /* Snake vs wall */
    if (((segment_t*)snake->data)->x + dx < 0 || ((segment_t*)snake->data)->x + dx > FIELD_WIDTH - 1||
        ((segment_t*)snake->data)->y + dy < 0 || ((segment_t*)snake->data)->y + dy > FIELD_HEIGHT - 1){
        game_over = 1;
        result = 1;
    }
    
    /* Snake vs Snake */
    for (current = snake->next; current != NULL; current = current->next){
        if (((segment_t*)snake->data)->x + dx == ((segment_t*)current->data)->x &&
            ((segment_t*)snake->data)->y + dy == ((segment_t*)current->data)->y){
            game_over = 1;
            result = 1;
            break;
        }
    }
    
    return result;
}

void spawn_food(tk_node_t *snake, int *food_x, int *food_y)
{
    tk_node_t *current;
    
    *food_x = GetRandomValue(0, FIELD_WIDTH - 1);
    *food_y = GetRandomValue(0, FIELD_HEIGHT - 1);
    
    current = snake;
    while (current != NULL){
        if (*food_x == ((segment_t*)current->data)->x &&
            *food_y == ((segment_t*)current->data)->y){
            
            *food_x = GetRandomValue(0, FIELD_WIDTH - 1);
            *food_y = GetRandomValue(0, FIELD_HEIGHT - 1);
            current = snake;
        }else{
            current = current->next;
        }
    }
}