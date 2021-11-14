#include <stdio.h>
#include <SDL.h>
#include "labyrinthe.h"


int sdl_run();


int main(int argc, char* args[]) {
    lab_init();
    sdl_run();
}

int sdl_run() {
    enum cell lab[SIZEH][SIZEL];
    int grid_cell_size = 36;
    int grid_width = SIZEL;
    int grid_height = SIZEH;

    // + 1 so that the last grid lines fit in the screen.
    int window_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;


    // The cursor ghost is a cursor that always shows in the cell below the
    // mouse cursor.
    SDL_Rect grid_cursor_ghost = {(grid_width - 1) / 2 * grid_cell_size, (grid_height - 1) / 2 * grid_cell_size, grid_cell_size,
                                  grid_cell_size};

    // Dark theme.
    SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
    SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
    SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
    SDL_Color grid_wall_color = {187, 187, 187, 255}; // White
    SDL_Color grid_output_color = {45, 92, 139, 255}; // dark bleu
    SDL_Color grid_player_color = {108, 66, 63, 255}; // dark red
    SDL_Color grid_Clue_color = {175, 146, 2, 255}; // dark yellow



    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                     SDL_GetError());
        return 0;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                    &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Create window and renderer: %s", SDL_GetError());
        return 0;
    }

    SDL_SetWindowTitle(window, "SDL Grid");

    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_i:
                        switchDisplayPath();
                        break;
                    case SDLK_UP:
                        doPlayerMove(Up);
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        doPlayerMove(Down);
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        doPlayerMove(Left);
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        doPlayerMove(Right);
                        break;
                    case SDLK_END:
                        lab_init();
                        break;
                }
                case SDL_MOUSEMOTION:
                    grid_cursor_ghost.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                    grid_cursor_ghost.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                    if (!mouse_active)
                        mouse_active = SDL_TRUE;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                        mouse_hover = SDL_TRUE;
                    else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                        mouse_hover = SDL_FALSE;
                    break;
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;
            }
        }

        // Draw grid background.
        SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
                               grid_background.b, grid_background.a);
        SDL_RenderClear(renderer);

        // Draw grid lines.
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                               grid_line_color.b, grid_line_color.a);

        for (int x = 0; x < 1 + grid_width * grid_cell_size;
             x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        for (int y = 0; y < 1 + grid_height * grid_cell_size;
             y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        // Draw grid ghost cursor.
        if (mouse_active && mouse_hover) {
            SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r,
                                   grid_cursor_ghost_color.g,
                                   grid_cursor_ghost_color.b,
                                   grid_cursor_ghost_color.a);
            SDL_RenderFillRect(renderer, &grid_cursor_ghost);
        }

        lab_getGride(lab);
        for (int i = 0; i < SIZEH; i++) {
            for (int j = 0; j < SIZEL; j++) {
                if(lab[i][j] != Empty){
                    SDL_Rect cell = {j*grid_cell_size,i*grid_cell_size,grid_cell_size,grid_cell_size};
                    switch (lab[i][j]) {
                        case Wall:
                            SDL_SetRenderDrawColor(renderer, grid_wall_color.r,
                                                   grid_wall_color.g,
                                                   grid_wall_color.b,
                                                   grid_wall_color.a);
                            break;
                        case Output:
                            SDL_SetRenderDrawColor(renderer, grid_output_color.r,
                                                   grid_output_color.g,
                                                   grid_output_color.b,
                                                   grid_output_color.a);
                            break;
                        case Player:
                            SDL_SetRenderDrawColor(renderer, grid_player_color.r,
                                                   grid_player_color.g,
                                                   grid_player_color.b,
                                                   grid_player_color.a);
                            break;
                        case Clue:
                            SDL_SetRenderDrawColor(renderer, grid_Clue_color.r,
                                                   grid_Clue_color.g,
                                                   grid_Clue_color.b,
                                                   grid_Clue_color.a);
                            break;
                        default:
                            break;
                    }
                    SDL_RenderFillRect(renderer, &cell);
                }

            }
        }

        //TODO brouillard de guerre

        //TODO afficher la dificulter du niveux

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}
