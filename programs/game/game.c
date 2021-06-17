
#include <stdio.h>
#include <stdlib.h>

#include "../../include/raylib.h"

#include "../../include/interface.h"
#include "../../include/state.h"

State state;

void update_and_draw(){
    KeyState keys = malloc(sizeof(*keys));
	
	state_update(state,keys);
	interface_draw_frame(state);
}

int main(){
	state = state_create();
	interface_init();
	
	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



