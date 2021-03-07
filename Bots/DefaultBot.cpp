#include <stddef.h>
#include "../GameClasses.h"

Bot::Bot(int xx, int yy) throw(){
    alive = true;
	bx = xx;
	by = yy;
	next = NULL;
    owner = OwnerID_Default;
	mychar = 'b';
};