
#include <stdlib.h>
#include <stdio.h>

#include "ADTSet.h"
#include "set_utils.h"


// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μικρότερη τιμή του set που είναι μεγαλύτερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_greater(Set set, Pointer value){

    if(set_find(set,value)){
        return set_find(set, value);
    }
    else if(set_find(set,value) == NULL){
        // Εισάγεται το value στο st.
        set_insert(set,value);
        SetNode next_node = set_next(set,set_find_node(set,value));
        // αφαιρείτε το value
        set_remove(set, value);
        // Δεν υπάρχει επόμενο στεοιχείο αρα επιστρέφει NULL
        if( next_node == NULL)
            return NULL;
        else
            return set_node_value(set,next_node);
    }
    else
        return NULL;
}

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μεγαλύτερη τιμή του set που είναι μικρότερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_smaller(Set set, Pointer value){
    
    if(set_find(set,value)){
        return set_find(set, value);
    }
    else if(set_find(set,value) == NULL){
        // Εισάγεται το value στο set
        set_insert(set,value);
        SetNode pre_node = set_previous(set,set_find_node(set,value));
        // αφαιρείτε το value
        set_remove(set, value);
        // Δεν υπάρχει προηγούμενο στεοιχείο αρα επιστρέφει NULL
        if( pre_node == NULL)
            return NULL;
        else
            return set_node_value(set,pre_node);
    }
    else
        return NULL;
}