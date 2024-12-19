#ifndef MTLL_H
#define MTLL_H
#include <stddef.h>

typedef union {
    int ival;
    float fval;
    char cval;
    char sval[129];
    struct mtll* ref;
} Data;

struct mtll {
    // Define your struct here
    Data t;
    int type; //0 = int, 1 = float, 2 = char, 3 = string, 4 = reference
    int listrefno;
    struct mtll* next;
};

// A few suggested function prototypes:
// Feel free to change or improve these as you need.

extern struct mtll *mtll_create(Data data, int type, int refno);

extern void add(struct mtll** head, Data value, int type, int refno);

extern void mtll_free(struct mtll *head);

extern void mtll_view(struct mtll *head);

extern void mtll_view_all(struct mtll **head, size_t);

extern void mtll_view_type(struct mtll *head);

//extern void mtll_remove(struct mtll **, size_t, struct mtll *);
extern void mtll_remove(struct mtll **head, size_t s, size_t totalsize);

// You will likely need other functions for Parts 2 and 3,
// And for general input / output management

extern int invalid_input(char * in); //0 if valid, else valid

extern void mtll_insert(struct mtll **head, size_t list_id, size_t index, Data data, int type, int refno);

extern void mtll_delete(struct mtll **head, size_t list_id, size_t index);

extern void insert_nested(struct mtll** llist, int refno, int elemC);

extern int check_nested(struct mtll** list, int n);

extern void update_ref_head(struct mtll** llist);

extern void mtll_view_nested(struct mtll *head);

extern void mtll_view_nested_help(struct mtll *head);

extern int getSize(struct mtll**llist, size_t i);

extern int check_int_nl(char * p);

extern int check_float_nl(char * p);

extern int check_if_ref(struct mtll** llist, struct mtll* head);

#endif
