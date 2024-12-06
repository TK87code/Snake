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


enum {
    SCENE_TITLE,
    SCENE_PLAY,
};

enum{
    DIR_NORTH,
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST,
};

typedef struct snake_segment{
    int x, y;
}segment_t;

tk_node_t* create_snake(void);
void draw_snake(tk_node_t *snake);
void scene_play(tk_node_t **snake, double *timer, int *direction);
void scene_title(int *scene);
void close_game(tk_node_t *snake);

int game_over;

int main(void)
{
    tk_node_t *snake;
    int scene;
    int direction;
    int apple_x;
    int apple_y;
    double timer;
    
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Snake");
    
    SetTargetFPS(60);
    
    /* Game variable init */
    scene = SCENE_TITLE;
    timer = 0.0;
    direction = DIR_WEST;
    
    /* Creating an initial snake */
    snake = create_snake();
    
    /* Spawn the first apple */
    //TODO
    
    while (!WindowShouldClose()){
        if (game_over != 0){
            game_over = 0;
            scene = SCENE_TITLE;
            direction = DIR_WEST;
            /* Delete the list, and create new */
            tk_list_delete(&snake);
            snake = create_snake();
        }
        
        switch(scene){
            case SCENE_TITLE:{
                scene_title(&scene);
            }break;
            
            case SCENE_PLAY:{
                scene_play(&snake, &timer, &direction);
            }break;
        }
    }
    
    close_game(snake);
    CloseWindow();        
    
    return 0;
}

/* Creating 10 segments and store in a linked list */
tk_node_t* create_snake(void)
{
    int i;
    const int initial_x = 25;
    const int initial_y = 25;
    tk_node_t *snake;
    segment_t *p;
    
    for (i = 0; i < 10; i++){
        p = malloc(sizeof(segment_t));
        
        if (!p){
            exit(1);
        }
        
        p->x = initial_x + i;
        p->y = initial_y;
        
        if (i == 0){
            snake = tk_list_create(p);
        }else
        {
            tk_list_push_back(snake, p);
        }
    }
    
    return snake;
}

void draw_snake(tk_node_t *snake)
{
    tk_node_t *current;
    segment_t *data;
    int c;
    
    /* loop untill at the end of the list */
    c = 0;
    current = snake;
    while (current != NULL){
        data = (segment_t*)current->data;
        if(c == 0){
            DrawRectangle(data->x * CELL_SIZE, data->y * CELL_SIZE, TILE_SIZE, TILE_SIZE, YELLOW);
        }else{
            DrawRectangle(data->x * CELL_SIZE, data->y * CELL_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
        }
        
        current = current->next;
        c++;
    }
}

void scene_title(int *scene)
{
    if (IsKeyPressed(KEY_SPACE)){
        *scene = SCENE_PLAY;
    }
    
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Press SPACE to play.", SCREEN_WIDTH / 2 - (strlen("Press SPACE to play.") * 5) , SCREEN_HEIGHT / 2, 20, WHITE);
    EndDrawing();
}

void scene_play(tk_node_t **snake, double *timer, int *direction)
{
    segment_t *new_segment;
    segment_t *head_data; /*Data of the current head*/
    tk_node_t *current;
    segment_t *current_data;
    
    /* update timer */
    *timer += GetFrameTime();
    
    head_data =(segment_t*)((tk_node_t*)(*snake)->data);
    /* Collision Check with wall */
    if (head_data->x < 0 || head_data->x > FIELD_WIDTH - 1 || head_data->y < 0 || head_data->y > FIELD_HEIGHT - 1){
        game_over = 1;
    }
    
    /* Collision check with body */
    for (current = (*snake)->next; current != NULL; current = current->next){
        current_data = (segment_t*)current->data;
        if (head_data->x == current_data->x && head_data->y == current_data->y){
            game_over = 1;
            break;
        }
    }
    
    /* collision Check with Apple */
    //TODO
    
    if (IsKeyPressed(KEY_LEFT)){
        *direction = DIR_WEST;
    }
    else if (IsKeyPressed(KEY_UP)){
        *direction = DIR_NORTH;
    }else if (IsKeyPressed(KEY_RIGHT)){
        *direction = DIR_EAST;
    }else if (IsKeyPressed(KEY_DOWN)){
        *direction = DIR_SOUTH;
    }
    
    if (*timer >= 0.2){
        *timer = 0;
        new_segment = malloc(sizeof(segment_t));
        if (!new_segment){
            exit(1);
        }
        
        /* Moving the snake */
        switch (*direction){
            case DIR_WEST:{
                new_segment->x = head_data->x - 1;
                new_segment->y = head_data->y;
            }break;
            
            case DIR_EAST:{
                new_segment->x = head_data->x + 1;
                new_segment->y = head_data->y;
            }break;
            
            case DIR_NORTH:{
                new_segment->x = head_data->x;
                new_segment->y = head_data->y - 1;
            }break;
            
            case DIR_SOUTH:{
                new_segment->x = head_data->x;
                new_segment->y = head_data->y + 1;
            }break;
        }
        
        /* Here, we moving the snake by pushing front & popping back */
        tk_list_push_front(snake, new_segment);
        tk_list_pop_back(*snake);
    }
    
    BeginDrawing();
    ClearBackground(BLACK);
    draw_snake(*snake);
    EndDrawing();
}

void close_game(tk_node_t *snake)
{
    tk_node_t *current;
    
    current = snake;
    while (current != NULL){
        free(current->data);
        current = current->next;
    }
}