/*                                                   */
/*  SORTDENI.C   :   Demonstration for Sort Library  */
/*                                                   */
/*                Programmed By Daenamu              */
/*                                                   */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"

#define SORTED 0
#define RANDOM 1
#define HALFSO 2
#define REVERS 3

typedef void (*SORTFNC)(void*, int, int, FCMP);
typedef void (*ESFNC)(FILE*, FILE*, int, void*, int, FCMP);

int intcmp(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

void set_array(int* a, int n, int m) {
	char* fname[] = { "ASCEND.DAT", "RANDOM.DAT", "HALFSORT.DAT", "DESCEND.DAT" };
	FILE* fp;

	if ((fp = fopen(fname[m], "rb")) == NULL) {
		printf("\bFile reading error!");
		return;
	}
	fread(a, sizeof(int), n, fp);
	fclose(fp);
}

int is_sorted(int* a, int n) {
	int i;
	for (i = 0; i < n - 1; i++) {
		if (a[i] > a[i + 1]) {
			return 0;
		}
	}
	return 1;
}

float sort_testnm(int n, int m, SORTFNC sort_func) {
	clock_t t1, t2;
	float t;
	int* a;
	if ((a = (int*)malloc(sizeof(int) * n)) == NULL) {
		printf("\nOut of Memory ...");
		exit(1);
	}
	set_array(a, n, m);

	t1 = clock();
	sort_func(a, n, sizeof(int), intcmp);
	t2 = clock();
	t = (float)(t2 - t1);

	if (!is_sorted(a, n)) t = -1;
	free(a);
	return t;
}

void sort_testn(int n, SORTFNC sort_func) {
	float t;
	int i;
	printf("\n    %6d", n);
	for (i = SORTED; i <= REVERS; i++) {
		t = sort_testnm(n, i, sort_func);
		printf("  %10.3f", t);
	}
}

void sort_test(char* name, SORTFNC sort_func) {
	int n[] = { 10, 100, 500, 1000, 2000, 5000, 7000, 10000, -1};
	int i = 0;
	printf("\n*** %s"
		"\n     N       Sorted      Random     Half-sorted   Reversed"
		"\n     ------  ---------   ---------  ------------  ----------"
		, name);
	
	while (n[i] > -1) {
		sort_testn(n[i++], sort_func);
	}
	printf("\n");
}

#define MAX 10000

void make_data(void) {
	FILE* fp;
	int i;
	srand((unsigned int)time(NULL));

	if ((fp = fopen("ASCEND.DAT", "wb")) == NULL) {
		printf("\n Disk Error!");
		exit(1);
	}
	printf("\n Creating ascending file ...");
	for (i = 1; i <= MAX; i++) {
		_putw(i, fp);
	}
	fclose(fp);

	if ((fp = fopen("DESCEND.DAT", "wb")) == NULL) {
		printf("\n Disk Error!");
		exit(1);
	}
	printf("\n Creating descending file ...");
	for (i = MAX; i > 0; i--) {
		_putw(i, fp);
	}
	fclose(fp);

	if ((fp = fopen("RANDOM.DAT", "wb")) == NULL) {
		printf("\n Disk Error!");
		exit(1);
	}
	printf("\n Creating random file ...");
	for (i = 0; i < MAX; i++) {
		_putw(rand() % MAX + 1, fp);
	}
	fclose(fp);

	if ((fp = fopen("HALFSORT.DAT", "wb")) == NULL) {
		printf("\n Disk Error!");
		exit(1);
	}
	printf("\n Creating half-sorted file ...");
	for (i = 0; i < MAX; i++) {
		if (i % 3) _putw(rand() % MAX + 1, fp);
		else _putw(i, fp);
	}
	fclose(fp);
}

#define BUFSIZE 4098

void external_sort_text(ESFNC sort_func) {
	clock_t t1, t2;
	float t;
	void* buf;
	FILE* src, * dst;
	char* fn[] = { "ASCEND.DAT", "DESCEND.DAT", "RANDOM.DAT", "HALFSORT.DAT" };
	int i;
	if ((buf = malloc(BUFSIZE)) == NULL) {
		printf("\nOut of memory");
		exit(1);
	}
	printf("\n*** External Sort"
		"\n     N       Sorted      Random     Half-sorted   Reversed"
		"\n     ------  ---------   ---------  ------------  ----------");
	printf("\n    %6d", MAX);

	for (i = 0; i < 4; i++) {
		if ((src = fopen(fn[i], "rb")) == NULL) {
			printf("\nDist Error!");
			exit(1);
		}
		if ((dst = fopen("RESULT.DAT", "wb")) == NULL) {
			printf("\nDist Error!");
			exit(1);
		}
		
		t1 = clock();
		sort_func(src, dst, sizeof(int), buf, BUFSIZE, intcmp);
		t2 = clock();
		t = (float)(t2-t1);
		printf("  %10.3f", t);
		fclose(src);
		fclose(dst);
	}
	free(buf);
}

int main() {
	make_stack();
	make_data();
	sort_test("Selection Sort", select_sort);
	sort_test("Insertion Sort", insert_sort);
	sort_test("Bubble Sort", bubble_sort);
	sort_test("Shell Sort", shell_sort);
	sort_test("Quick Sort1 : using median", quick_sort1); 
	sort_test("Quick Sort2 : using random", quick_sort2);
	sort_test("Merge Sort", merge_sort);
	sort_test("Heap Sort", heap_sort);
	external_sort_text(external_sort);
	remove_stack();
}