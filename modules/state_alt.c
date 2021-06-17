
#include <stdlib.h>
#include <stdio.h>

#include "ADTSet.h"
#include "ADTList.h"
#include "ADTMap.h"
#include "state.h"
#include "set_utils.h"


struct state {
	Set objects;			// περιέχει στοιχεία Object (Εμπόδια / Εχθροί / Πύλες)
	Map portal_pairs;		// περιέχει PortaPair (ζευγάρια πυλών, είσοδος/έξοδος)

	struct state_info info;
};

// Ζευγάρια πυλών
typedef struct portal_pair {
	Object entrance;		// η πύλη entrance
	Object exit;			// οδηγεί στην exit
}* PortalPair;

int * create_int(int value){
    int* p = malloc(sizeof(int));
    *p = value;
    return p; 
}

int compare_pointers(Pointer a, Pointer b){
	return a - b;
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create(){
    // Δημιουργία του set.
    State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.current_portal = 0;			// Δεν έχουμε περάσει καμία πύλη
	state->info.wins = 0;					// Δεν έχουμε νίκες ακόμα
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.

    // Πληροφορίες για το χαρακτήρα.
    // Τα έχω ίδια με το αρχικό state create (state.c).
	Object character = state->info.character = malloc(sizeof(*character));
	
	character->type = CHARACTER;
	character->forward = true;
	character->jumping = false;
    character->rect.width = 70;
	character->rect.height = 38;
	character->rect.x = 0;
	character->rect.y = - character->rect.height;

    // Δημιουργία των objects (πύλες / εμπόδια / εχθροί)
    // και προσθήκη στο set state->objects. 
    
    state->objects = set_create(compare_pointers,free);
	for(int i = 0; i < 4*PORTAL_NUM; i++){
		Object obj = malloc(sizeof(*obj));
		//printf("sssssssssssssssssssss\n");
		if(i % 4 == 3) {							// Το 4ο, 8ο, 12ο κλπ αντικείμενο
			obj->type = PORTAL;						// είναι πύλη.
			obj->rect.width = 100;
			obj->rect.height = 5;

		} else if(rand() % 2 == 0) {				// Για τα υπόλοιπα, με πιθανότητα 50%
			obj->type = OBSTACLE;					// επιλέγουμε εμπόδιο.
			obj->rect.width = 10;
			obj->rect.height = 80;
		} else {
			obj->type = ENEMY;						// Και τα υπόλοιπα είναι εχθροί.
			obj->rect.width = 30;
			obj->rect.height = 30;
			obj->forward = false;					// Οι εχθροί αρχικά κινούνται προς τα αριστερά.
		}
		obj->rect.x = (i+1) * SPACING;
		obj->rect.y = - obj->rect.height;
		
		set_insert(state->objects,obj);
	
	}

	
   	// state->portal_pairs = map_create();
	// for(int i = 0; i < PORTAL_NUM; i++){

	// 	PortalPair portal = malloc(sizeof(*portal));
		
	// 	int random;
	// 	while((random = rand() % PORTAL_NUM )== i);

	// 	portal->entrance = malloc(sizeof(*portal->entrance));
	// 	portal->exit = malloc(sizeof(*portal->exit));
	// 	portal->entrance = (Object)set_find(state->objects, create_int(i));
	// 	portal->exit = (Object)set_find(state->objects,create_int(random));

	// }
	
	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state){
    return &(state->info);
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη x είναι ανάμεσα στο x_from και x_to.

List state_objects(State state, float x_from, float x_to){
	List list_of_objects = list_create(free);
	ListNode lnode = LIST_BOF;
	
	for( float i = x_from; i < x_to; i++){
		//Object obj = ;
		// if(set_find_eq_or_greater(state->objects, create_int(i)) <= ???????)
		//	list_insert_next(list_of_objects, lnode, obj);

	}
	return list_of_objects;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys){

}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state){

}