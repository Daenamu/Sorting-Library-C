#ifndef _SORT_H
#define _SORT_H

extern int top;
extern int* stack;

typedef int (*FCMP)(const void*, const void*);

#define init_stack() (top = -1)
#define push(t) (stack[++top] = t)
#define pop() (stack[top--])
#define is_stack_empty() (top < 0)

int make_stack(void);
void remove_stack(void);

void select_sort(void* base, int nelem, int width, FCMP fcmp);
void insert_sort(void* base, int nelem, int width, FCMP fcmp);
void bubble_sort(void* base, int nelem, int width, FCMP fcmp);

void shell_sort(void* base, int nelem, int width, FCMP fcmp);

void quick_sort1(void* base, int nelem, int width, FCMP fcmp);
void quick_sort2(void* base, int nelem, int width, FCMP fcmp);

void merge_sort(void* base, int nelem, int width, FCMP fcmp);

void downheap(void* base, int nelem, int width, FCMP fcmp, int k);
void heap_sort(void* base, int nelem, int width, FCMP fcmp);

void external_sort(FILE *src, FILE *dst, int width, void* buf, int buflen, FCMP fcmp);

#endif
