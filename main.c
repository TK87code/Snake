#include "raylib.h"
#include "tk_list.h"
#include <stdlib.h> /* malloc() */
#include <string.h> /* strlen() */

#define TILE_SIZE 15
#define CELL_SIZE 16 /* Tile(grid) size in pixel */

#define FIELD_WIDTH 50 /* Field width in grids */
#define FIELD_HEIGHT 50 /* Field height in grids */

#define SCREEN_WIDTH FIELD_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT FIELD_HEIGHT * CELL_SIZE

#define SPEED 0.2

enum {
    SCENE_TITLE,
    SCENE_PLAY,
};

typedef struct snake_segment{
    int x, y;
}segment_t;

tk_node_t* create_snake(void);
void draw_snake(tk_node_t *snake);
void scene_play(tk_node_t **snake, double *timer, int* dx, int* dy);
void scene_title(int *scene);
int does_collide(tk_node_t *snake, int dx, int dy);

int game_over;

int main(void)
{
    tk_node_t *snake;
    int scene;
    double timer;
    int dx, dy;
    
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
    
    /* Spawn the first apple */
    //TODO
    
    while (!WindowShouldClose()){
        if (game_over != 0){
            game_over = 0;
            scene = SCENE_TITLE;
            timer = 0.0;
            dy = 0;
            dx = -1;
            /* Delete the list, and create new */
            tk_list_delete(&snake);
            snake = create_snake();
        }
        
        /* Scene switching */
        switch(scene){
            case SCENE_TITLE:{
                scene_title(&scene);
            }break;
            
            case SCENE_PLAY:{
                scene_play(&snake, &timer, &dx, &dy);
            }break;
        }
    }
    
    tk_list_delete(&snake);
    CloseWindow();        
    
    return 0;
}

/* Creating 10 segments and store in a linked list */
tk_node_t* create_snake(void)
{
    int i;
    const int initial_x = 25;
    const int initial_y = 25;
    tk_node_t *snake = NULL;
    segment_t *p;
    
    for (i = 0; i < 10; i++){
        p = malloc(sizeof(segment_t));
        if (!p) exit(1);
        
        p->x = initial_x + i;
        p->y = initial_y;
        
        if (i == 0) snake = tk_list_create(p);
        else tk_list_push_back(snake, p);
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
                  TILE_SIZE, TILE_SIZE, YELLOW);
}

void scene_title(int *scene)
{
    if (IsKeyPressed(KEY_SPACE)) *scene = SCENE_PLAY;
    
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Press SPACE to play.", 
             SCREEN_WIDTH / 2 - (int)(strlen("Press SPACE to play.") * 5), 
             SCREEN_HEIGHT / 2, 20, 
             WHITE);
    EndDrawing();
}

void scene_play(tk_node_t **snake, double *timer, int* dx, int* dy)
{
    segment_t *new_segment;
    
    /* update timer */
    *timer += GetFrameTime();
    
    /* Input handling */
    if (IsKeyPressed(KEY_LEFT)){
        *dx = *dy = 0;
        *dx = -1;
    }
    else if (IsKeyPressed(KEY_UP)){
        *dx = *dy = 0;
        *dy = -1;
    } 
    else if (IsKeyPressed(KEY_RIGHT)){
        *dx = *dy = 0;
        *dx = 1;
    } 
    else if (IsKeyPressed(KEY_DOWN)){
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
    
    /* Even whe the snake collide and set the flag of game over, BeginDrawing() should be called because of event handling.*/
    BeginDrawing();
    ClearBackground(BLACK);
    draw_snake(*snake);
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