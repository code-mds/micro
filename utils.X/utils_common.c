#include <ctype.h>
#include "utils_common.h"

void utils_common_tolower(char* str){
    int i;
    for(i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

void utils_common_delay(int counter) {    
    while(counter) {
        counter--;
    }   
}
