
#include <stdlib.h>
#include <stdio.h>

#include "../include/ADTVector.h"
#include "../include/ADTList.h"
#include "../include/state.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Vector objects;			// περιέχει στοιχεία Object (Εμπόδια / Εχθροί / Πύλες)
	List portal_pairs;		// περιέχει PortaPair (ζευγάρια πυλών, είσοδος/έξοδος)

	struct state_info info;
};

// Ζευγάρια πυλών
typedef struct portal_pair {
	Object entrance;		// η πύλη entrance
	Object exit;			// οδηγεί στην exit
}* PortalPair;

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));
	
	// Γενικές πληροφορίες
	state->info.current_portal = 0;			// Δεν έχουμε περάσει καμία πύλη
	state->info.wins = 0;					// Δεν έχουμε νίκες ακόμα
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.

	// Πληροφορίες για το χαρακτήρα.
	Object character = state->info.character = malloc(sizeof(*character));
	character->type = CHARACTER;
	character->forward = true;
	character->jumping = false;

    // Ο χαρακτήρας (όπως και όλα τα αντικείμενα) έχουν συντεταγμένες x,y σε ένα
    // καρτεσιανό επίπεδο.
	// - Στο άξονα x το 0 είναι η αρχή στης πίστας και οι συντεταγμένες
	//   μεγαλώνουν προς τα δεξιά.
	// - Στον άξονα y το 0 είναι το "δάπεδο" της πίστας, και οι
	//   συντεταγμένες μεγαλώνουν προς τα _κάτω_.
	// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
	// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
	// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
	// στο include/raylib.h).
	// 
	// Προσοχή: τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle, και
	// τα y μεγαλώνουν προς τα κάτω, οπότε πχ ο χαρακτήρας που έχει height=38,
	// αν θέλουμε να "κάθεται" πάνω στο δάπεδο, θα πρέπει να έχει y=-38.

	character->rect.width = 70;
	character->rect.height = 38;
	character->rect.x = 0;
	character->rect.y = - character->rect.height;

	// Δημιουργία των objects (πύλες / εμπόδια / εχθροί) και προσθήκη στο vector
	// state->objects. Η πίστα περιέχει συνολικά 4*PORTAL_NUM αντικείμενα, από
	// τα οποία τα PORTAL_NUM είναι πύλες, και τα υπόλοια εμπόδια και εχθροί.

	state->objects = vector_create(0, free);		// Δημιουργία του vector

	for (int i = 0; i < 4*PORTAL_NUM; i++) {
		// Δημιουργία του Object και προσθήκη στο vector
		Object obj = malloc(sizeof(*obj));
		vector_insert_last(state->objects, obj);

		// Κάθε 4 αντικείμενα υπάρχει μια πύλη. Τα υπόλοιπα αντικείμενα
		// επιλέγονται τυχαία.

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

		// Τα αντικείμενα είναι ομοιόμορφα τοποθετημένα σε απόσταση SPACING
		// μεταξύ τους, και "κάθονται" πάνω στο δάπεδο.

		obj->rect.x = (i+1) * SPACING;
		obj->rect.y = - obj->rect.height;
	}

	//Αρχικοποίηση της λίστας state->portal_pairs
	state->portal_pairs = list_create(free);  //Δημιουργία του list
	
	for(int i= 3; i < 4*PORTAL_NUM; i+= 4){
		
		// Δημιουργία του PortalPair
		PortalPair portal = malloc(sizeof(*portal));
		
		int random;
		while(((random = (rand() % 4*PORTAL_NUM))*4 -1) % 4*PORTAL_NUM == i);

		portal->entrance = (Object)vector_get_at(state->objects, i);
		portal->exit = (Object)vector_get_at(state->objects,random);
		
		// Δημιουργία μιας λίστας που έχει μέσα της ζευγάρια πυλών
		list_insert_next(state->portal_pairs, list_last(state->portal_pairs), portal);
	
		//Ελέγχω κάθε node της λίστας ώστε να δούμε αν υπάρχει 
		for(ListNode node = list_first(state->portal_pairs); 
			node != LIST_EOF; 
			node = list_next(state->portal_pairs, node)){
			
			PortalPair p = list_node_value(state->portal_pairs,node);
			// Αν δείχνουν στην ίδια διεύθυνση και το node δεν είναι στο
			// τελευταίο στοιχείο της λίστας υπάρχει ήδη αυτη η έξοδος.
			while(portal->exit == p->exit && node != list_last(state->portal_pairs)){
				// Αλλάζουμε τη διεύθυνση του exit απο τον κόμβο που μπήκε τελευταίος
				//random = (random + 4) % 4*PORTAL_NUM;
				random = ((random + 4) >= 4*PORTAL_NUM) ? 3 : (random + 4);
				portal->exit = vector_get_at(state->objects, random);
			}
		}
		
	}
	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &(state->info);
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη x είναι ανάμεσα στο x_from και x_to.

List state_objects(State state, float x_from, float x_to) {
	List listObject = list_create(NULL);
	ListNode list_node = LIST_BOF;
	//the rect.x of the objects
	for(VectorNode node = vector_first(state->objects);        
    	node != VECTOR_EOF;                         
    	node = vector_next(state->objects, node)) {  
			
			Object obj = vector_node_value(state->objects, node);
			if( obj->rect.x > x_from && obj->rect.x < x_to){
					list_insert_next(listObject, list_node, obj);
			}
	}
	return listObject;
}


// Αν ο Χαρακτήρας συγκρουστεί με Εχθρό ή Εμπόδιο τερματίζει το παιχνίδι.

bool objects_conflicts(State state, Object a, Object b){
	
	if((a->rect.x + a->rect.width >= b->rect.x && a->rect.x <= b->rect.x)
		|| (b->rect.x + b->rect.width >= a->rect.x && b->rect.x <= a->rect.x)){

		if((a->rect.y + a->rect.height >= b->rect.y && a->rect.y <= b->rect.y)
			|| (b->rect.y + b->rect.height >= a->rect.y && b->rect.y <= a->rect.y)){
					return true;
			}
	}
	return false;
}

		
// Αν έχουμε σύγκρουση δυο αντικειμένων, ψάχνουμε στην λίστα 
// το αντίστοιχο ταίρι της πύλης και το επιστρέφουμε, αλλιώς επιστρέφει NULL.

Object portal_partener(State state, Object portal){
	for(ListNode node= list_first(state->portal_pairs);
		node != LIST_EOF;
		node = list_next(state->portal_pairs, node)){
			PortalPair p_portal = list_node_value(state->portal_pairs,node);
			if(p_portal->entrance->rect.x == portal->rect.x){
				return p_portal->exit;
			}
	}
	return NULL;
}


// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	Object character = state->info.character;

	if(!state->info.playing){
		if(IsKeyDown(KEY_ENTER)){
			state_create();
			state->info.playing = true;
			
		}
	}
	else if(state->info.playing){
		// CHARACTER
		// Ξεκινάει προς την κατεύθυνση -> και προχωράει 7
		if(character->rect.x == 0){
			character->forward = true;
		}
		// Αν ο χαρακτήρας είναι <-(ή ->) και πατηθεί 
		// το keys->right(αντίστοιχα keys->left) γυρίζει ->( <-)
		if(IsKeyPressed(KEY_RIGHT) && character->forward == false){
			character->forward = true;
		}
		else if(IsKeyPressed(KEY_LEFT) && character->forward){
			character->forward = false;
		}

		// Αν πηγαίνει ήδη στην κατεύθυνση αυτή και keys right(ή left) προχωράει 12.
		if(IsKeyDown(KEY_RIGHT) && character->forward){
			character->rect.x += 12;
		}
		else if(IsKeyDown(KEY_LEFT) &&  !character->forward){
			character->rect.x -= 12;
		}
		else if(character->forward){
			character->rect.x += 7;
		}
		else if(!character->forward){
			character->rect.x -= 7;
		}
		
		// Άλμα
		// Αν βρίσκεται στο έδαφος και keys->up ξεκινάει άλμα και ανεβαίνει 15 pixels
		if(character->rect.y == - character->rect.height && IsKeyDown(KEY_UP)){
			character->jumping = true;
			character->rect.y -= 15;
		}
		else if((IsKeyUp(KEY_UP) || character->rect.y <= -220 - character->rect.height)
				&& character->rect.y < - character->rect.height){
			character->jumping = false;
			character->rect.y += 15;
		}
		else if(IsKeyDown(KEY_UP) && character->jumping){
			if(character->rect.y > -220 - character->rect.height)
				character->rect.y -= 15;
		}
		else if(!character->jumping && character->rect.y <= - character->rect.height){
			character->rect.y += 15;
		}
		else{
			if(character->rect.y < -character->rect.height)
				character->rect.y += 15;
		}
		if(character->rect.y > - character->rect.height){
			character->rect.y = - character->rect.height;
		}
		for(int i = 0; i < vector_size(state->objects); i++){
			//Τα αντικείμενα του vector ένα ενα.
			Object obj = vector_get_at(state->objects,i);
			
			// Αν ο Χαρακτήρας συγκρουστεί με Εχθρό ή Εμπόδιο.
			if(objects_conflicts(state, character, obj) && obj->type != PORTAL){
				state->info.playing = false;
				// character->rect.x = 0;
			}
			else if (obj->type == PORTAL && objects_conflicts(state, character, obj)){
				PortalPair p = list_node_value(state->portal_pairs,list_last(state->portal_pairs));
				Object portal = p->entrance;
				// Αν ο Χαρακτήρας συγκρουστεί με μια Πύλη
				Object exit = portal_partener(state, obj);
				
				character->rect.x = exit->rect.x;
				state->info.current_portal = *(int *)p->exit;
				character->rect.y = -50;
				//Αν ο χαρακτήρας συγκρουστεί με την τελευταία πύλη, τότε
				// προστίθεται μία νίκη και μετακινείται στην αρχή της πίστας
				if(obj->rect.x == portal->rect.x){
					state->info.wins++;
					character->rect.x = 0;
				}
			}
			
			for(int j = 0; j < vector_size(state->objects); j++){
				// για να ελέγξουμε και τα υπόλποιπα στοιχεία 
				Object obj2 = vector_get_at(state->objects, j);		
				// Άμ ο εχθρός συγκρουστεί με ένα εμπόδιο αλλάζει η φορά του.
				if(obj->type == ENEMY && obj2->type == OBSTACLE){
					if(objects_conflicts(state, obj, obj2)){
						if(!obj->forward){
							obj->forward = true;
							obj->rect.x += 5;
						}
						else if(obj->forward){
							obj->forward = false;
							obj->rect.x -= 5;
						}
					}
				}
				else if(obj->type == OBSTACLE && obj2->type == ENEMY){
					if(objects_conflicts(state, obj, obj2)){
						if(!obj2->forward){
							obj2->forward = true;
							obj2->rect.x += 5;
						}
						else if(obj2->forward){
							obj2->forward = false;
							obj2->rect.x -= 5;
						}
					}
				}
				// Άν ο εχθρός πέσει πάνω σε πύλη.
				if(obj->type == ENEMY && obj2->type == PORTAL){
					if(objects_conflicts(state, obj, obj2)){
						Object exit = portal_partener(state, obj2);
						obj->rect.x = exit->rect.x;
					}
				}
				else if(obj->type == PORTAL && obj2->type == ENEMY){
					if(objects_conflicts(state, obj, obj2)){
						Object exit = portal_partener(state, obj);
						obj2->rect.x = exit->rect.x;
					}
				}
			}

			if( obj->type == ENEMY){
				if(obj->forward)
					obj->rect.x += 5;
				else if(!obj->forward)
					obj->rect.x -= 5;
			}	
		}
	}
	else if(!state->info.playing && IsKeyDown(KEY_ENTER)){
		state->info.playing = true;
		character->rect.x = 0;
	}
	else if(IsKeyDown(KEY_P)){
		state->info.paused = true;
	}
	else if(state->info.paused && IsKeyDown(KEY_N)){
		state->info.paused = false;
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// list
	//int i = list_size(state->portal_pairs);
	// while( --i){
	// 	//PortalPair p = list_node_value(state->portal_pairs, list_last(state->portal_pairs));
	// 	// free(p->entrance);
	// 	// free(p->exit);
	// 	list_remove_next(state->portal_pairs,LIST_BOF);	
	// }
	list_destroy(state->portal_pairs);
	
	// vector
	//i = vector_size(state->objects);
	// while(--i){
	// 	// Object ob = vector_node_value(state->objects,vector_last(state->objects));
	// 	// free(ob);
	// 	vector_remove_last(state->objects);
	// }
	vector_destroy(state->objects);
	
	// state
	free(state);
	
}