#include "mtll.h"
//#include "mtll.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#define max_input 129
#define init_size 10

int main(int argc, char** argv) {
    //TODO
    //You should do general input / output management here.
    int curr_size = init_size;
    struct mtll** llist = (struct mtll**)malloc(init_size * sizeof(struct mtll*)); //10 initial size
    for (int i = 0; i < init_size; i++) {
        llist[i] = NULL; // Initialize each pointer to NULL
    }
    int elemCount = 0;

    char input[max_input];
    char temp[max_input];
    char * inpPtr;  
    while (fgets(input, max_input, stdin) != NULL) {
        if (feof(stdin)) {
            return 0;
        }
        //input[strcspn(input, "\n")] = 0;
        //printf("%s", input);
        strcpy(temp, input);
        inpPtr = strtok(input, " ");
        char* cmd = inpPtr; 
        //printf("%s", cmd);
        if (!strcmp(cmd, "NEW\n")) {
            printf("INVALID COMMAND: NEW\n");
            continue;
        } 
        if (!strcmp(cmd, "TYPE\n")) {
            printf("INVALID COMMAND: TYPE\n");
            continue;
        } 
        if (!strcmp(cmd, "REMOVE\n")) {
            printf("INVALID COMMAND: REMOVE\n");
            continue;
        } 
        if (!strcmp(cmd, "VIEW\n")) {
            printf("INVALID COMMAND: VIEW\n");
            continue;
        }
        if (!strcmp(cmd, "INSERT\n")) {
            printf("INVALID COMMAND: INSERT\n");
            continue;
        }
        if (!strcmp(cmd, "DELETE\n")) {
            printf("INVALID COMMAND: DELETE\n");
            continue;
        } 
        if (!strcmp(cmd, "VIEW-NESTED\n")) {
            printf("INVALID COMMAND: VIEW-NESTED\n");
            continue;
        }
        if (!strcmp(cmd, "NEW") && input[0] != ' ') { //make new list
            inpPtr = strtok(NULL, " ");
            int sz = atoi(inpPtr);
            int invalid = 0;
            int end = 0;
            for (int i = 0; inpPtr[i] != '\n'; i++) {
                if (inpPtr[i] >= '0' && inpPtr[i] <= '9') {
                    continue;
                } else {
                    invalid = 1;
                }
            }

            if (invalid) {
                if (llist[elemCount] != NULL) {
                    free(&llist[elemCount]);
                    llist[elemCount] = NULL;
                }
                printf("INVALID COMMAND: NEW\n");
                continue;
            }

            if (sz == 0) {
                struct mtll* nd = NULL;
                llist[elemCount] = nd;
            }

            for (int i = 0; i < sz; i++) {
                //take input for list
                char in[max_input];
                Data data;
                int intValue;
                float floatValue;
                int type = -1;
                int skip = 0;

                if (fgets(in, max_input, stdin) != NULL) {
                    //printf("%s", in);
                    if (in[0] == '{' && in[2] == '}' && strlen(in) == 4 && in[1] >= '0' && in[1] <= '9') { //nestedlist found
                        invalid = 0;
                        //char * c = in;
                        int number = in[1] - '0';
                        if (check_nested(llist, number)) { //trying to insert nested
                            invalid = 1;
                        } else {
                            insert_nested(llist, number, elemCount);
                            skip = 1;
                        }
                    
                    } else if (sscanf(in, "%d", &intValue) == 1 && check_int_nl(in)) {
                        //if (atoi(in) == atof(in) && check_int_nl(in)) { //int
                        data.ival = intValue;
                        type = 0;
                        //} else {
                            //sscanf(in, "%f", &floatValue); //float
                            //data.fval = floatValue;
                            //type = 1;
                        //} 
                    } else if (sscanf(in, "%f", &floatValue) == 1 && check_float_nl(in)) {
                        data.fval = floatValue;
                        type = 1;
                    } else if (strlen(in) == 2 && isprint(in[0])){ //char
                        data.cval = in[0];
                        type = 2;
                    } else {
                        in[strcspn(in, "\n")] = 0; //remove newline - https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
                        strcpy(data.sval, in);
                        type = 3;
                    }

                    //add node to list here
                    if (llist[elemCount] == NULL && skip != 1) {
                        llist[elemCount] = mtll_create(data, type, -1);
                    } else if (skip != 1) {
                        add(&llist[elemCount], data, type, -1);
                    }

                    //add invalid cehck for {,}
                    char * invalidCheck = in;
                    if (invalid_input(invalidCheck) && !skip) {
                        invalid = 1;
                    }
                } else {
                    end = 1;
                }
            }
            
            if (invalid) {
                if (llist[elemCount] != NULL) {
                    free(&llist[elemCount]);
                    llist[elemCount] = NULL;
                }
                printf("INVALID COMMAND: NEW\n");
            } else if (end == 0) {
                if (check_nested(llist, elemCount)) {
                    printf("Nested %d: ", elemCount);
                } else {
                    printf("List %d: ", elemCount);
                }
                mtll_view(llist[elemCount]);
                elemCount++;
            }

            if (elemCount == curr_size) {//resize 
                llist = realloc(llist, sizeof(struct mtll*) * curr_size * 2);
                curr_size *= 2;
                for (int i = curr_size/2; i < curr_size; i++) {
                    llist[i] = NULL; // Initialize each pointer to NULL
                }
            }
            continue;
        }
        if (!strcmp(cmd, "VIEW") && input[0] != ' ') {
            inpPtr = strtok(NULL, " ");
            int invalid = 0;
            cmd = inpPtr;
            if (!strcmp(cmd, "ALL\n")) {
                mtll_view_all(llist, elemCount);
                continue;
            }

            for (int i = 0; inpPtr[i] != '\n'; i++) {
                if (inpPtr[i] >= '0' && inpPtr[i] <= '9') {
                    continue;
                } else {
                    invalid = 1;
                }
            }
            
            if (invalid) {
                printf("INVALID COMMAND: VIEW\n");
                continue;
            }

            int ind = atoi(inpPtr); //ind of list to view
            if (llist[ind] == NULL || ind >= elemCount) {
                printf("INVALID COMMAND: VIEW\n");
            } else {
                mtll_view(llist[ind]);
            }
            continue;
        } 
        if (!strcmp(cmd, "TYPE") && input[0] != ' ') { //view types of each element in list
            inpPtr = strtok(NULL, " ");
            int invalid = 0;

            for (int i = 0; inpPtr[i] != '\n'; i++) {
                if (inpPtr[i] >= '0' && inpPtr[i] <= '9') {
                    continue;
                } else {
                    invalid = 1;
                }
            }
            
            if (invalid) {
                printf("INVALID COMMAND: TYPE\n");
                continue;
            }

            int ind = atoi(inpPtr);

            if (ind >= elemCount) {
                printf("INVALID COMMAND: TYPE\n");
            } else {
                mtll_view_type(llist[ind]);
            }
            continue;

        }
        if (!strcmp(cmd, "REMOVE") && input[0] != ' ') {
            inpPtr = strtok(NULL, " ");
            int invalid = 0;

            for (int i = 0; inpPtr[i] != '\n'; i++) {
                if (inpPtr[i] >= '0' && inpPtr[i] <= '9') {
                    continue;
                } else {
                    invalid = 1;
                }
            }
            
            if (invalid) {
                printf("INVALID COMMAND: REMOVE\n");
                continue;
            }

            int ind = atoi(inpPtr);
            if (llist[ind] == NULL) {
                printf("INVALID COMMAND: REMOVE\n");
            } else if (ind >= elemCount) {
                printf("INVALID COMMAND: REMOVE\n");
            } else {
                if (check_if_ref(llist, llist[ind])) {
                    printf("INVALID COMMAND: REMOVE\n");
                } else {
                    mtll_remove(llist, ind, curr_size);
                }
                
            }
            continue;
        } else if (!strcmp(cmd, "INSERT") && input[0] != ' ') {
            int toAdd = 0;
            int invalid = 0;
            inpPtr = strtok(NULL, " ");
            for (int i = 0; inpPtr[i] != '\0'; i++) { //check valid list
                if (inpPtr[i] >= '0' && inpPtr[i] <= '9') {
                    toAdd++;
                    continue;
                } else {
                    invalid = 1;
                }
            }
            int insertList = atoi(inpPtr);
            inpPtr = strtok(NULL, " ");
            for (int i = 0; inpPtr[i] != '\0'; i++) { //check valid insert pos
                if ((inpPtr[i] >= '0' && inpPtr[i] <= '9')) {
                    toAdd++;
                    continue;
                } else if (inpPtr[i] == '-') {
                    toAdd++;
                    continue;
                } else {
                    invalid = 1;
                }
            }
            int ind = atoi(inpPtr);
            if (ind < 0) {
                while ((ind*-1) > getSize(llist, insertList)) {
                    ind += getSize(llist, insertList);
                }
                ind = getSize(llist, insertList) + ind + 1;
            }

            if (invalid == 1 || ind > getSize(llist, insertList) || insertList < 0 || insertList >= elemCount) {
                printf("INVALID COMMAND: INSERT\n");
                continue;
            }

            //inpPtr = strtok(NULL, " "); //read val to insert

            Data d;
            char in[max_input];
            int intValue;
            float floatValue;
            int type = -1;
            int skip = 0;
            //strcpy(in, inpPtr);

            //printf("%s", test);
            /*while (inpPtr != NULL) {
                strcat(in, " ");
                strcat(in, inpPtr);
                inpPtr = strtok(NULL, " ");
            }*/
            inpPtr = strtok(temp, "");
            //printf("%s\n", inpPtr+toAdd+8+1);
            strcpy(in, inpPtr+toAdd+8+1);
            if (in[0] == '{' && in[2] == '}' && strlen(in) == 4 && in[1] >= '0' && in[1] <= '9') { //nestedlist found
                int number = in[1] - '0';
                if (number < 0 || number >= elemCount) {
                    printf("INVALID COMMAND: INSERT\n");
                    continue;
                }
                mtll_insert(llist, insertList, ind, d, 4, number);
                skip = 1;
            } else
            if (sscanf(in, "%d", &intValue) == 1) {
                if (atoi(in) == atof(in)) { //int
                    d.ival = intValue;
                    type = 0;
                } else {
                    sscanf(in, "%f", &floatValue); //float
                    d.fval = floatValue;
                    type = 1;
                } 
            } else if (strlen(in) == 2 && isprint(in[0])){ //char
                d.cval = in[0];
                type = 2;
            } else {
                in[strcspn(in, "\n")] = 0; //remove newline - https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
                strcpy(d.sval, in);
                type = 3;
            }

            if (skip == 0) {
                mtll_insert(llist, insertList, ind, d, type, -1);
            }
            continue;
        }
        if (!strcmp(cmd, "DELETE") && input[0] != ' ') { 
            int invalid = 0;
            inpPtr = strtok(NULL, " ");
            int deleteList = atoi(inpPtr);

            for (int i = 0; inpPtr[i] != '\0'; i++) { //check valid list
                if (inpPtr[i] >= '0' && inpPtr[i] <= '9') {
                    continue;
                } else {
                    invalid = 1;
                }
            }

            inpPtr = strtok(NULL, " ");
            int ind = atoi(inpPtr);

            for (int i = 0; inpPtr[i] != '\n'; i++) { //check valid insert pos
                if ((inpPtr[i] >= '0' && inpPtr[i] <= '9')) {
                    continue;
                } else if (inpPtr[i] == '-') {
                    continue;
                } else {
                    invalid = 1;
                }
            }

            if (invalid == 1 || ind > getSize(llist, deleteList) || deleteList < 0 || deleteList >= elemCount || getSize(llist, deleteList) == 0) {
                printf("INVALID COMMAND: DELETE\n");
                continue;
            }

            if (ind < 0) {
                while ((ind*-1) > getSize(llist, deleteList)) {
                    ind += getSize(llist, deleteList);
                }
                ind = getSize(llist, deleteList) + ind;
            }

            mtll_delete(llist, deleteList, ind);
            continue;
        }
        if (!strcmp(cmd, "VIEW-NESTED") && input[0] != ' ') {
            int invalid = 0;
            inpPtr = strtok(NULL, " ");
            int ind = atoi(inpPtr);
            for (int i = 0; inpPtr[i] != '\n'; i++) { //check valid index
                if (inpPtr[i] >= '0' && inpPtr[i] <= '9') {
                    continue;
                } else {
                    invalid = 1;
                }
            }

            if (invalid == 1 || ind < 0 || ind >= elemCount) {
                printf("INVALID COMMAND: VIEW-NESTED\n");
                continue;
            }

            if (check_nested(llist, ind)) {
                mtll_view_nested(llist[ind]);
            } else {
                mtll_view(llist[ind]);     
            }
            continue;
        } 
        printf("INVALID COMMAND: INPUT\n");
        //printf("-------------------------------------------\n");
    }
    
    for (int i = 0; i < curr_size; i++) {
        if (llist[i] != NULL) {
            mtll_free(llist[i]);
        }
    }
    free(llist);
    return 0;
}