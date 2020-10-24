/*                                                   */
/*  SORT.C   :   Sorting Library                     */
/*                                                   */
/*                Programmed By Daenamu              */
/*                                                   */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

#define STACK_SIZE 20000
#define BASE(i) ((char*)base + (i)*width)

int top; /* 스택의 상단 */
int* stack; /* 스택 포인터 */

int make_stack(void) {
	if ((stack = (int*)malloc(sizeof(int) * STACK_SIZE)) == NULL) {
		return 0;
	}
	return 1;
}
void remove_stack(void) {
	free(stack);
}

void select_sort(void* base, int nelem, int width, FCMP fcmp) {
	/* 최소값을 찾는 선택 정렬 알고리즘 */
	void* min;
	int minindex;
	int i, j;

	if ((min = malloc(width)) == NULL) exit(1);
	for (i = 0; i < nelem - 1; i++) {
		minindex = i;
		memcpy(min, BASE(i), width);
		for (j = i + 1; j < nelem; j++) {
			if (fcmp(min, BASE(j)) > 0) {
				memcpy(min, BASE(j), width);
				minindex = j;
			}
		}
		memcpy(BASE(minindex), BASE(i), width);
		memcpy(BASE(i), min, width);
	}
	free(min);
}
void insert_sort(void* base, int nelem, int width, FCMP fcmp) {
	/* 삽입 정렬 알고리즘 */
	int i, j;
	void* t;
	if ((t = malloc(width)) == NULL) exit(1);
	for (i = 1; i < nelem; i++) {
		memcpy(t, BASE(i), width);
		j = i;
		while (fcmp(BASE(j - 1), t) > 0 && j > 0) {
			memcpy(BASE(j), BASE(j - 1), width);
			j--;
		}
		memcpy(BASE(j), t, width);
	}
	free(t);
}
void bubble_sort(void* base, int nelem, int width, FCMP fcmp) {
	/* 보초가 있는 거품 정렬 */
	int i, j;
	int s;
	void* t;
	if ((t = malloc(width)) == NULL) exit(1);
	for (i = 0; i < nelem - 1; i++) {
		s = 0;
		for (j = 1; j < nelem - i; j++) {
			if (fcmp(BASE(j - 1), BASE(j)) > 0) {
				memcpy(t, BASE(j - 1), width);
				memcpy(BASE(j - 1), BASE(j), width);
				memcpy(BASE(j), t, width);
				s = 1;
			}
		}
		if (s == 0) break;
	}
	free(t);
}
void shell_sort(void* base, int nelem, int width, FCMP fcmp) {
	/* h = 3*h+1 을 이용한 쉘 정렬 */
	int i, j, k, h;
	void* v;
	if ((v = malloc(width)) == NULL) exit(1);
	for (h = 1; h < nelem; h = 3 * h + 1);
	for (h /= 3; h > 0; h /= 3) {
		for (i = 0; i < h; i++) {
			for (j = i + h; j < nelem; j += h) {
				memcpy(v, BASE(j), width);
				k = j;
				while (k > h - 1 && fcmp(BASE(k - h), v) > 0) {
					memcpy(BASE(k), BASE(k - h), width);
					k -= h;
				}
				memcpy(BASE(k), v, width);
			}
		}
	}
	free(v);
}

void quick_sort1(void* base, int nelem, int width, FCMP fcmp) {
	/* 세 값의 증위를 이용하는 퀵 정렬 */
	void* v, * u;
	int i, j, t;
	int l, r;
	init_stack();
	v = malloc(width);
	u = malloc(width);
	if ((u == NULL) | (v == NULL)) exit(1);
	l = 0;
	r = nelem - 1;
	push(r);
	push(l);
	while (!is_stack_empty()) {
		l = pop();
		r = pop();
		if (r - l + 1 > 100) {
			t = (r + l) >> 1;
			if (fcmp(BASE(l), BASE(t)) > 0) {
				memcpy(v, BASE(l), width);
				memcpy(BASE(l), BASE(t), width);
				memcpy(BASE(t), v, width);
			}
			if (fcmp(BASE(l), BASE(r)) > 0) {
				memcpy(v, BASE(l), width);
				memcpy(BASE(l), BASE(r), width);
				memcpy(BASE(r), v, width);
			}
			if (fcmp(BASE(t), BASE(r)) > 0) {
				memcpy(v, BASE(t), width);
				memcpy(BASE(t), BASE(r), width);
				memcpy(BASE(r), v, width);
			}
			memcpy(v, BASE(t), width);
			memcpy(BASE(t), BASE(r - 1), width);
			memcpy(BASE(r - 1), v, width);
			i = l;
			j = r - 1;
			while (1) {
				while (fcmp(BASE(++i), v) < 0);
				while (fcmp(BASE(--j), v) > 0);
				if (i >= j) break;
				memcpy(u, BASE(i), width);
				memcpy(BASE(i), BASE(j), width);
				memcpy(BASE(j), u, width);
			}
			memcpy(u, BASE(i), width);
			memcpy(BASE(i), BASE(r - 1), width);
			memcpy(BASE(r - 1), u, width);
			push(r);
			push(i + 1);
			push(i - 1);
			push(l);
		}
		else {
			insert_sort(BASE(l), r - l + 1, width, fcmp);
		}
	}
	free(u);
	free(v);
}
void quick_sort2(void* base, int nelem, int width, FCMP fcmp) {
	/* 난수 분할을 이용하는 퀵 정렬 */
	srand((unsigned int)time(NULL));
	void* v, * u;
	int i, j, t;
	int l, r;
	init_stack();
	v = malloc(width);
	u = malloc(width);
	if ((u == NULL) | (v == NULL)) exit(1);
	l = 0;
	r = nelem - 1;
	push(r);
	push(l);
	while (!is_stack_empty()) {
		l = pop();
		r = pop();
		if (r - l + 1 > 100) {
			t = rand() % (r - l + 1) + l;
			memcpy(v, BASE(t), width);
			memcpy(BASE(t), BASE(r), width);
			memcpy(BASE(r), v, width);
			i = l - 1;
			j = r;
			while (1) {
				while (fcmp(BASE(++i), v) < 0);
				while (fcmp(BASE(--j), v) > 0);
				if (i >= j) break;
				memcpy(u, BASE(i), width);
				memcpy(BASE(i), BASE(j), width);
				memcpy(BASE(j), u, width);
			}
			memcpy(u, BASE(i), width);
			memcpy(BASE(i), BASE(r), width);
			memcpy(BASE(r), u, width);
			push(r);
			push(i + 1);
			push(i - 1);
			push(l);
		}
		else {
			insert_sort(BASE(l), r - l + 1, width, fcmp);
		}
	}
	free(u);
	free(v);
}
void merge_sort(void* base, int nelem, int width, FCMP fcmp) {
	/* 상향식 병합정렬 */
	int i, j, k, first, second, size;
	void* b;
	if ((b = malloc(width * nelem)) == NULL) {
		printf("\nOut of memory error!");
		return;
	}
	for (size = 1; size < nelem; size = size * 2) {
		first = -2 * size;
		second = first + size;
		while (second + size*2 < nelem) {
			first = second + size;
			second = first + size;
			i = first;
			j = second;
			k = first;
			while ((i < first + size) || ((j < second + size) && (j < nelem))) {
				if (fcmp(BASE(i), BASE(j)) <= 0) {
					if (i < (first + size)) {
						memcpy((char*)b + (k++) * width, BASE(i++), width);
					}
					else {
						memcpy((char*)b + (k++) * width, BASE(j++), width);
					}
				}
				else {
					if ((j < second + size) && (j < nelem)) {
						memcpy((char*)b + (k++) * width, BASE(j++), width);
					}
					else {
						memcpy((char*)b + (k++) * width, BASE(i++), width);
					}
				}
			}
		}
		memcpy(base, b, nelem * width);
	}
	free(b);
}

void downheap(void* base, int nelem, int width, FCMP fcmp, int k) {
	/* 힙 정렬을 위한 DOWNHEAP 함수 */
	int i;
	void* v;
	if ((v = malloc(width)) == NULL) exit(1);
	memcpy(v, BASE(k - 1), width);
	while (k <= nelem / 2) {
		i = k << 1;
		if (i < nelem && fcmp(BASE(i - 1), BASE(i)) < 0) i++;
		if (fcmp(v, BASE(i - 1)) >= 0) break;
		memcpy(BASE(k - 1), BASE(i - 1), width);
		k = i;
	}
	memcpy(BASE(k - 1), v, width);
	free(v);
}
void heap_sort(void* base, int nelem, int width, FCMP fcmp) {
	/* 상향식 힙 생성을 이용한 힙 정렬 */
	int k;
	void* t;
	if ((t = malloc(width)) == NULL) exit(1);
	for (k = nelem / 2; k >= 1; k--) {
		downheap(base, nelem, width, fcmp, k);
	}
	while (nelem > 1) {
		memcpy(t, base, width);
		memcpy(base, BASE(nelem - 1), width);
		memcpy(BASE(nelem - 1), t, width);
		downheap(base, --nelem, width, fcmp, 1);
	}
	free(t);
}

struct entry {
	FILE* fp;
	char fname[13];
	void* v;
	int is_eof;
};

void external_sort(FILE* src, FILE* dst, int width, void* buf, int buflen, FCMP fcmp) {
	/* 내부정렬은 쉘, 외부 파일은 병합 */
	int i, minindex, done;
	int nelem, nbuf, nfile;
	void* min;
	struct entry* tmp;

	if ((min = malloc(width))==NULL) exit(1);

	fseek(src, 0L, SEEK_END);
	nelem = (int)(ftell(src) / width);
	nbuf = buflen / width;
	nfile = nelem / nbuf + 1;

	tmp = (struct entry*)malloc(sizeof(struct entry) * nfile);
	for (i = 0; i < nfile; i++) {
		sprintf(tmp[i].fname, "%d.TMP", i);
		tmp[i].fp = fopen(tmp[i].fname, "wb");
		tmp[i].v = malloc(width);
		tmp[i].is_eof = 0;
	}

	rewind(src);

	for (i = 0; i < nfile - 1; i++) {
		fread(buf, width, nbuf, src);
		shell_sort(buf, nbuf, width, fcmp);
		fwrite(buf, width, nbuf, tmp[i].fp);
	}
	fread(buf, width, nelem % nbuf, src);
	shell_sort(buf, nelem % nbuf, width, fcmp);
	fwrite(buf, width, nelem % nbuf, tmp[i].fp);

	for (i = 0; i < nfile; i++) {
		rewind(tmp[i].fp);
		if (fread(tmp[i].v, width, 1, tmp[i].fp) == 0) {
			tmp[i].is_eof = 1;
		}
	}
	rewind(dst);

	while (1) {
		done = 1;
		for (i = 0; i < nfile; i++) {
			if (!tmp[i].is_eof) done = 0;
		}
		if (done) break;
		i = 0;
		while (tmp[i].is_eof) i++;
		minindex = i;
		memcpy(min, tmp[minindex].v, width);
		while (++i < nfile) {
			if (!tmp[i].is_eof && fcmp(min, tmp[i].v) > 0) {
				minindex = i;
				memcpy(min, tmp[i].v, width);
			}
		}
		fwrite(tmp[minindex].v, width, 1, dst);
		if (fread(tmp[minindex].v, width, 1, tmp[minindex].fp) == 0) {
			tmp[minindex].is_eof = 1;
		}
	}
	for (i = 0; i < nfile; i++) {
		fclose(tmp[i].fp);
		_unlink(tmp[i].fname);
		free(tmp[i].v);
	}
	free(min);
	free(tmp);
}