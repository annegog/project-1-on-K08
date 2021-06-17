#include "../include/raylib.h"

#include "../include/state.h"
#include "../include/interface.h"
#include "../include/ADTList.h"
#include <stdio.h>

// Assets
Texture bird_img;
Sound game_over_snd;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init(){
    // Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game_game_example");
	SetTargetFPS(60);
    InitAudioDevice();

    // Φόρτωση εικόνων και ήχων
	bird_img = LoadTextureFromImage(LoadImage("../game_example/assets/bird.png"));
	game_over_snd = LoadSound("../game_exampleassets/game_over.mp3");
}

// Κλείνει το interface του παιχνιδιού
void interface_close(){
    CloseAudioDevice();
	CloseWindow();
}

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state){
    BeginDrawing();

	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(WHITE);
   
    // Σχεδιάζουμε χαρακτήρα και τα αντικείμενα.
    // Ο χαρακτήρας έχει μία σταθερή θέση 
    // και μεταβάλλονται τα αντικείμενα ως προς τον χαρακτήρα.
    DrawTexture(bird_img, 300, 250 + state_info(state)->character->rect.y, BLACK);
    DrawRectangle(0, 250, SCREEN_WIDTH, 1, BLACK);
    // Σχεδιάζονται τα αντικείμενε τα οποία φαίνονται στην οθόνη.
    List objects = state_objects(state, state_info(state)->character->rect.x - 300,
                   state_info(state)->character->rect.x - 300 + 800);
    for(ListNode node = list_first(objects);
        node != LIST_EOF;
        node = list_next(objects,node)){
            
            Object obj = list_node_value(objects,node);
            if(obj->type == PORTAL){
                DrawRectangle(obj->rect.x - state_info(state)->character->rect.x + 300,
                        250 + obj->rect.y, obj->rect.width, obj->rect.height, GRAY);
            }
            else
                DrawRectangle(obj->rect.x - state_info(state)->character->rect.x + 300,
                        250 + obj->rect.y, obj->rect.width, obj->rect.height, RED);
             
    }

    // Σχεδιάζουμε το Σκορ, ο αριθμός της πύλης στη οποία πέρασε
    // ο χαρακτήρας και το FPS counter.
	DrawText(TextFormat("WINS %0i", state_info(state)->wins), 20, 50, 40, GRAY);
    DrawText(TextFormat("Portal %0i/100",state_info(state)->current_portal), 20, 20, 40, MAROON);
	DrawFPS(SCREEN_WIDTH - 80, 0);


    if(!state_info(state)->playing){
        DrawText("PRESS [ENTER] TO PLAY AGAIN",
			 GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, DARKGRAY); 

    }
    EndDrawing();
}