#include "mtll.h"
#include <stdlib.h>
#include <stdio.h>

struct mtll *mtll_create(Data data, int type, int refno) {
    //TODO
    struct mtll* newNode = (struct mtll*)malloc(sizeof(struct mtll));
    newNode->t = data;
    newNode->type = type;
    newNode->listrefno = refno;
    newNode->next = NULL;
    return newNode;
}

void mtll_free(struct mtll *node) {
    //TODO
    if (node->next != NULL) {
        if (node->type == 4) {
            free(node->t.ref);
        }
        mtll_free(node->next);
    }
    free(node);
}

//TODO: all other functions

extern void add(struct mtll** head, Data value, int type, int refno) {
    if (head == NULL) {
        return;
    }

    struct mtll* curr = *head;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    if (refno != -1) {
        curr->next = mtll_create(value, type, refno);
    } else {
        curr->next = mtll_create(value, type, -1);
    }
}
 
extern void mtll_view(struct mtll *head) {
    struct mtll* curr = head;
    while (curr != NULL) {
        if (curr->next == NULL) {
            if (curr->type == 0) { //printing last elem
                printf("%d", curr->t.ival);
            } else if (curr->type == 1) {
                printf("%.2f", curr->t.fval);
            } else if (curr->type == 2) {
                printf("%c", curr->t.cval);
            } else if (curr->type == 3) {
                printf("%s", curr->t.sval);
            } else if (curr->type == 4) {
                printf("{List %d}", curr->listrefno);
            } 
        } else {
            if (curr->type == 0) {
                printf("%d -> ", curr->t.ival);
            } else if (curr->type == 1) {
                printf("%.2f -> ", curr->t.fval);
            } else if (curr->type == 2) {
                printf("%c -> ", curr->t.cval);
            } else if (curr->type == 3) {
                printf("%s -> ", curr->t.sval);
            } else if (curr->type == 4) {
                printf("{List %d} -> ", curr->listrefno);
            }
        }
        
        curr = curr->next;
    }
    printf("\n");
}

extern void mtll_view_type(struct mtll *head) {
    struct mtll* curr = head;
    while (curr != NULL) {
        if (curr->next == NULL) {
            if (curr->type == 0) { //printing last elem
                printf("int");
            } else if (curr->type == 1) {
                printf("float");
            } else if (curr->type == 2) {
                printf("char");
            } else if (curr->type == 3) {
                printf("string");
            } else if (curr->type == 4) {
                printf("reference");
            } 
        } else {
            if (curr->type == 0) {
                printf("int -> ");
            } else if (curr->type == 1) {
                printf("float -> ");
            } else if (curr->type == 2) {
                printf("char -> ");
            } else if (curr->type == 3) {
                printf("string -> ");
            } else if (curr->type == 4) {
                printf("reference -> ");
            } 
        }
        
        curr = curr->next;
    }
    printf("\n");
}

extern void mtll_view_all(struct mtll **llist, size_t s) {
    int cnt = 0;
    for (int i = 0; i < s; i++) {
        if (llist[i] == NULL) {
            continue;
        }
        cnt++;
    }
    printf("Number of lists: %d\n", cnt);
    for (int i = 0; i < s; i++) {
        if (llist[i] == NULL) {
            continue;
        }
        if (check_nested(llist, i)) {
            printf("Nested %d\n", i);
        } else {
            printf("List %d\n", i);
        }
        
    }
}

extern void mtll_remove(struct mtll **head, size_t s, size_t totalsize) {
    mtll_free(head[s]);
    head[s] = NULL;
    for (int i = 0; i < totalsize; i++) {
        if (head[i] != NULL && head[i]->type == 4 && head[i]->t.ref == head[s]) {
            head[i]->t.ref = NULL;
        }
    }
    printf("List %ld has been removed.\n", s);
    printf("\n");
    mtll_view_all(head, totalsize);
}   

extern int invalid_input(char * in) {
    for (int i = 0; in[i] != '\0'; i++) {
        if (in[i] == '{' || in[i] == '}') {
            return 1;
        }
    }
    return 0;
}

extern void mtll_insert(struct mtll **head, size_t list_id, size_t index, Data data, int type, int refno) {
    struct mtll* nodeAdd = mtll_create(data, type, refno);
    struct mtll* list = head[list_id];
    if (index == 0) {
        nodeAdd->next = list;
        head[list_id] = nodeAdd;
        
    } else {
        int cInd = 0;
        while (cInd < index - 1) { //at index to insert
            list = list->next;
            cInd++;
        }
        struct mtll * temp  = list->next; 
        list->next = nodeAdd;
        nodeAdd->next = temp;
    }
    update_ref_head(head);
    if (check_nested(head, list_id)) {
        printf("Nested %ld: ", list_id);
    } else {
        printf("List %ld: ", list_id);
    }
    mtll_view(head[list_id]);
}

extern void mtll_delete(struct mtll **head, size_t list_id, size_t index) {
    if (index == 0) {//deleting head
        struct mtll* currHead = head[list_id];
        head[list_id] = head[list_id]->next;
        free(currHead);
    } else {
        struct mtll* curr = head[list_id];
        int cInd = 0;
        while (cInd < index - 1) { //at index before the index to delete
            curr = curr->next;
            cInd++;
        }
        struct mtll* temp = curr->next;
        curr->next = curr->next->next; 
        free(temp);
    }
    update_ref_head(head);
    if (check_nested(head, list_id)) {
        printf("Nested %ld: ", list_id);
    } else {
        printf("List %ld: ", list_id);
    }
    
    mtll_view(head[list_id]);
}

extern void insert_nested(struct mtll** llist, int refno, int elemC) {
    Data data;
    data.ref = llist[refno];
    int type = 4;
    if (llist[elemC] == NULL) {
        llist[elemC] = mtll_create(data, type, refno);
    } else {
        add(&llist[elemC], data, type, refno);
    }
}

extern int check_nested(struct mtll** list, int n) {
    struct mtll* temp = list[n];
    while (temp != NULL) {
        if (temp->type == 4) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

extern void mtll_view_nested(struct mtll *head) {
    struct mtll* curr = head;
    while (curr != NULL) {
        if (curr->next == NULL) {
            if (curr->type == 0) { //printing last elem
                printf("%d", curr->t.ival);
            } else if (curr->type == 1) {
                printf("%.2f", curr->t.fval);
            } else if (curr->type == 2) {
                printf("%c", curr->t.cval);
            } else if (curr->type == 3) {
                printf("%s", curr->t.sval);
            } else if (curr->type == 4) {
                printf("{");
                mtll_view_nested_help(curr->t.ref);
                printf("}");
            } 
        } else {
            if (curr->type == 0) {
                printf("%d -> ", curr->t.ival);
            } else if (curr->type == 1) {
                printf("%.2f -> ", curr->t.fval);
            } else if (curr->type == 2) {
                printf("%c -> ", curr->t.cval);
            } else if (curr->type == 3) {
                printf("%s -> ", curr->t.sval);
            } else if (curr->type == 4) {
                printf("{");
                mtll_view_nested_help(curr->t.ref);
                printf("} -> ");
            }
        }
    
        curr = curr->next;
    }
    printf("\n");
}

extern void mtll_view_nested_help(struct mtll *head) {
    struct mtll* curr = head;
    while (curr != NULL) {
        if (curr->next == NULL) {
            if (curr->type == 0) { //printing last elem
                printf("%d", curr->t.ival);
            } else if (curr->type == 1) {
                printf("%.2f", curr->t.fval);
            } else if (curr->type == 2) {
                printf("%c", curr->t.cval);
            } else if (curr->type == 3) {
                printf("%s", curr->t.sval);
            } else if (curr->type == 4) {
                printf("{");
                mtll_view(curr->t.ref);
                printf("}");
            } 
        } else {
            if (curr->type == 0) {
                printf("%d -> ", curr->t.ival);
            } else if (curr->type == 1) {
                printf("%.2f -> ", curr->t.fval);
            } else if (curr->type == 2) {
                printf("%c -> ", curr->t.cval);
            } else if (curr->type == 3) {
                printf("%s -> ", curr->t.sval);
            } else if (curr->type == 4) {
                printf("{");
                mtll_view(curr->t.ref);
                printf("} -> ");
            }
        }
    
        curr = curr->next;
    }
}

extern int getSize(struct mtll**llist, size_t i) {
    int res = 0;
    struct mtll* curr = llist[i];
    while (curr != NULL) {
        res++;
        curr = curr->next;
    }
    return res;
}

extern int check_int_nl(char * p) {
    for (int i = 0; p[i] != '\n'; i++) { //check valid index
        if ((p[i] >= '0' && p[i] <= '9') || p[i] == '-') {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

extern int check_float_nl(char * p) {
    for (int i = 0; p[i] != '\n'; i++) { //check valid index
        if ((p[i] >= '0' && p[i] <= '9') || p[i] == '-' || p[i] == 'e' || p[i] == '.') {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

extern void update_ref_head(struct mtll** llist) {
    for (int i = 0; llist[i] != NULL; i++) {
        struct mtll* currList = llist[i];
        while (currList != NULL) {
            if (currList->type == 4) {
                if (currList->t.ref != llist[currList->listrefno]) {
                    currList->t.ref = llist[currList->listrefno];
                }
            }

            currList = currList->next;
        }
    }
}

extern int check_if_ref(struct mtll** llist, struct mtll* head) {
    for (int i = 0; llist[i] != NULL; i++) {
        struct mtll* currList = llist[i];
        while (currList != NULL) {
            if (currList->type == 4) {
                if (currList->t.ref == head) {
                    return 1;
                }
            }

            currList = currList->next;
        }
    }
    return 0;
}