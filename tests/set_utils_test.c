/////////////////////////////////////////////////////////////////
//
// Test για το set_utils.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "set_utils.h"


// Δεσμέυει και επιστρέφει εναν ακέραιο. 
int * create_int(int value){
    int* p = malloc(sizeof(int));
    *p = value;
    return p; 
}

int compare_ints(Pointer a, Pointer b){
    return *(int*)a -*(int *)b;
}

// Ελέγχουμε την περίπτωση που: a υπάρχει το στοιχείο στο set,
// b δεν υπέρχει άρα επιστρέφεται το αμέσως επόμενο,
// c τη περίπτωση οπου δεν υπάρχει επόμενο στοιχείο,
// d δεν υπέρχει άρα επιστρέφεται το αμέσως προηγόυμενο και 
// e τη περίπτωση οπου δεν υπάρχει πτοηγόυμενο στοιχείο. 

void test_set_find(){
    Set set = set_create(compare_ints, NULL);
    set_insert(set, create_int(-2));
    set_insert(set, create_int(15));
    set_insert(set, create_int(8));
    set_insert(set, create_int(1034));
    set_insert(set, create_int(-15));

    TEST_ASSERT(set_find_eq_or_greater(set, create_int(8)) != NULL);
    TEST_ASSERT(set_find_eq_or_greater(set, create_int(3)) == set_find(set,create_int(8)));
    TEST_ASSERT(set_find_eq_or_greater(set, create_int(1500)) == NULL);
    TEST_ASSERT(set_find_eq_or_smaller(set, create_int(-1005)) == NULL);
    TEST_ASSERT(set_find_eq_or_smaller(set, create_int(3)) == set_find(set,create_int(-2)));
   
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_set_find", test_set_find },

	{ NULL} // τερματίζουμε τη λίστα με NULL
};