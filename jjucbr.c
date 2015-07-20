#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static unsigned long long	fm_count;
static volatile bool		proceed = false;
static void done(int unused) {
	proceed = false;
	unused = unused;
}
typedef struct b b;
struct b {
	int t;
	int n;
};
void divi(b *b1, b *b2) {
	b1->t = b2->n * b1->t;
	b1->n = b1->n * b2->t;
	if (b1->n < 0) {
		b1->t = b1->t * -1;
		b1->n = b1->n * -1;
	}
}
void addi(b *b1, b *b2, b *b3) {
	b3->n = b1->n * b2->n;
	b3->t = b1->t * b2->n + b2->t * b1->n;
}
void subi(b *b1, b *b2, b *b3) {
	b3->n = b1->n * b2->n;
	b3->t = b1->t * b2->n - b2->t * b1->n;
}

void xprint(int r, int k, b a[r][k]) {
	int i;

	for (i = 0; i < r; ++i)
	{
		int j;
		for (j = 0; j < k; ++j)
		{
			printf("%d/%d\t", a[i][j].t, a[i][j].n);
		}
		printf("\n");
	}
	printf("\n");
}

int divide(int r, int k, int s, b a[r][k] , char c[3][r + 1]) {

	int lz = 1, lp = 1, ln = 1;
	for (int i = 0; i < r; ++i)
	{
		b divid = a[i][s];
		if (divid.t == 0) {
			c[0][lz] = i;
			lz++;
		} else {
			if (divid.t < 0) {
				c[2][ln] = i;
				ln++;
			}
			else {
				c[1][lp] = i;
				lp++;
			}
			for (int j = 0; j < k ; ++j)
			{
				divi(&a[i][j], &divid);
			}
		}
	}
	c[0][0] = lz - 1;
	c[1][0] = lp - 1;
	c[2][0] = ln - 1;
	return 1;
}

char check(int r, b a[r][2]) {
	char c[3][r + 1];
	divide(r, 2, 1, a, c);
	float min = -268435456 , max = 268435456;
	for (int i = 1; i <= c[1][0]; ++i)
	{
		max = MIN(max, (float)a[c[1][i]][0].t / (float)a[c[1][i]][0].n);
	}
	for (int i = 1; i <= c[2][0]; ++i)
	{
		min = MAX(min, (float)a[c[2][i]][0].t / (float)a[c[2][i]][0].n);
	}
	return min < max;
}

void elim(int r, int k, b a[r][k], b ar[][k - 1], char c[3][r + 1]) {
	int t = 0;
	for (int i = 1; i <= c[1][0]; ++i)
	{
		for (int j = 1; j <= c[2][0]; ++j)
		{
			for (int l = 0; l < k - 1; ++l)
			{
				subi(&a[c[1][i]][l], &a[c[2][j]][l], &ar[t][l]);
			}
			t++;
		}
	}
	for (int i = 1; i <= c[0][0]; ++i)
	{
		for (int l = 0; l < k - 1; ++l)
		{
			ar[t][l] = a[c[0][i]][l];
		}
		t++;
	}
}

char jc_elim(int r, int k, b a[r][k]) {
	if (k == 2)
	{
		if (a[0][1].t == 0 && a[0][0].t < 0)
		{
			return 0;
		}
		char re = 1;
		for (int i = 0; i < r; ++i)
		{
			if (a[i][1].t >= 0) {
				re = 0;
				break;
			}
		}
		if (re) {
			return 1;
		}
		return check(r, a);
	}
	char c[3][r + 1];
	if (divide(r, k, k - 1, a, c) == 0) {
		b ar[r][k - 1];
		for (int i = 0; i < r; ++i)
		{
			for (int j = 0; j < k - 1; ++j)
			{
				ar[i][j] = a[i][j];
			}
		}
		jc_elim(r, k - 1, ar);
	}
	b ar[c[0][0] + c[1][0]*c[2][0]][k - 1];
	elim(r, k, a, ar, c);
	return jc_elim(c[0][0] + c[1][0] * c[2][0], k - 1, ar);
}

unsigned long long jjucbr(char* aname, char* cname, int seconds)
{
	FILE*		afile = fopen(aname, "r");
	FILE*		cfile = fopen(cname, "r");
	fm_count = 0;
	int r, k;
	fscanf (cfile, "%d", &r);
	fscanf (afile, "%d", &r);
	fscanf (afile, "%d", &k);
	k++;
	b a[r][k];
	for (int i = 0; i < r; ++i)
	{
		fscanf (cfile, "%d", &a[i][0]);
		a[i][0].n = 1;
		for (int j = 1; j < k; ++j)
		{
			fscanf (afile, "%d", &a[i][j]);
			a[i][j].n = 1;
		}
	}
	fclose(afile);
	fclose(cfile);
	if (seconds == 0) {
		return jc_elim(r, k, a);
	}
	signal(SIGALRM, done);
	alarm(seconds);
	proceed = true;
	while (proceed) {
		jc_elim(r, k, a);
		fm_count++;
	}
	return fm_count;
}
