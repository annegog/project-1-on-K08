//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->current_portal == 0);
	TEST_ASSERT(info->wins == 0);
	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);

	//Έλεγχος για την αρχική θέση του χαρακτήρα
	Object character = info->character;
	TEST_ASSERT(character->rect.x == 0);
	TEST_ASSERT(character->rect.y == -character->rect.height);

	//Δοκιμή αν εχουν δημιουργηθεί 4 * PORTAL_NUM αντικείμενα
	List obj = state_objects(state, 0 , 401 * SPACING);
	TEST_ASSERT(list_size(obj) == 4*PORTAL_NUM );
	//Δοκιμή για 2-3 διαφορετικές τιμές x_from, x_to
	obj = state_objects(state, 0, 35*SPACING);
	TEST_ASSERT(list_size(obj)== 34);
	obj = state_objects(state, 10*SPACING, 101*SPACING );
	TEST_ASSERT(list_size(obj) == 90);
	obj = state_objects(state, 250*SPACING, 301*SPACING );
	TEST_ASSERT(list_size(obj) == 50);
	
	state_destroy(state);
}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, ο χαρακτήρας μετακινείται 7 pixels μπροστά
	Rectangle old_rect = state_info(state)->character->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->character->rect;

	TEST_ASSERT( new_rect.x == old_rect.x + 7 && new_rect.y == old_rect.y );

	// Με πατημένο το δεξί βέλος, ο χαρακτήρας μετακινείται 12 pixels μπροστά
	keys.right = true;
	old_rect = state_info(state)->character->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->character->rect;
	TEST_ASSERT( new_rect.x == old_rect.x + 12 && new_rect.y == old_rect.y );
	
	// Με πατημένο το αριστερό, ο χαρακτήρας κινήται 12 pixels προς τα πίσω.
	keys.right = false;
	keys.left = true;
	state_info(state)->character->forward = false;
	old_rect = state_info(state)->character->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->character->rect;
	//TEST_ASSERT( new_rect.x == old_rect.x - 12 && new_rect.y == old_rect.y );

	// Άλμα ενώ κινήται προς τα μπροστά, ανεβαίνει προς τα πάνω 15 pixesl.
	keys.up = true;
	keys.left= false;
	keys.right = false;
	state_info(state)->character->forward = true; 
	old_rect = state_info(state)->character->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->character->rect;
	//TEST_ASSERT( new_rect.x == old_rect.x + 7 && new_rect.y == old_rect.y - 15 );

	// Ξεκινάει η πτώσει του χαρακτήρα, μετά το άλμα του. Αφού έχει φτάσει
	// τα απαιτούμενα pixes. Και κατεβαίνει προς τα κάτω 15 pixels
	// άλλα με διαφορετική φορά κίνησης γιατι είναι πατημένο το left.
	keys.right = false;
	keys.left = true;
	keys.up = true;
	state_info(state)->character->jumping = false;
	state_info(state)->character->rect.y = -100 - state_info(state)->character->rect.height;
	old_rect = state_info(state)->character->rect;
	state_update(state, &keys);
	new_rect= state_info(state)->character->rect;
	TEST_ASSERT(new_rect.y == old_rect.y + 15);
	
	//δεν έχω κάνει τεστ για τις συγκρούσεις!!
	// prepei na tis koitaksw 
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};