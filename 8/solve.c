#include <math.h>
#include <stdio.h>

char ex_input[] = 
	"73167176531330624919225119674426574742355349194934"
	"96983520312774506326239578318016984801869478851843"
	"85861560789112949495459501737958331952853208805511"
	"12540698747158523863050715693290963295227443043557"
	"66896648950445244523161731856403098711121722383113"
	"62229893423380308135336276614282806444486645238749"
	"30358907296290491560440772390713810515859307960866"
	"70172427121883998797908792274921901699720888093776"
	"65727333001053367881220235421809751254540594752243"
	"52584907711670556013604839586446706324415722155397"
	"53697817977846174064955149290862569321978468622482"
	"83972241375657056057490261407972968652414535100474"
	"82166370484403199890008895243450658541227588666881"
	"16427171479924442928230863465674813919123162824586"
	"17866458359124566529476545682848912883142607690042"
	"24219022671055626321111109370544217506941658960408"
	"07198403850962455444362981230987879927244284909188"
	"84580156166097919133875499200524063689912560717606"
	"05886116467109405077541002256983155200055935729725"
	"71636269561882670428252483600823257530420752963450"
	;

#include <stdlib.h>
static char *
generate(size_t n) {
	char * retval = malloc(n+1);
	for (int i=0; i<n; i++) {
		int r = rand();
		unsigned char c = (char)r ^ (char)(r>>8) ^ (char)(r>>16) ^ (char)(r>>24);
		retval[i] = '0' + c%10;
	}
	retval[n] = '\0';
	return retval;
}

typedef unsigned long long rettype;
/*typedef unsigned __int128 rettype;*/
volatile rettype resultmax;
size_t resultmaxpos;

rettype
solve1(char *input, int k) {
	int c,i,j;
	rettype max=0;
	size_t maxpos=-1;
	rettype prods[k];

	for (j=0;j<k;j++) {
		prods[j] = 1;
	}
	i=0;
	while (input[i]) {
		c = input[i]-'0';
		for (j=0;j<k-1;j++) {
			prods[j] = prods[j+1] * c;
		}
		prods[k-1] = c;
		if (prods[0] > max && i >= k-1) {
			max = prods[0];
			maxpos = i;
		}
		i++;
	}

#if 0
	printf("Max Value %lld seen at position %zu\n", max, maxpos);
	printf("%d", input[maxpos-k+1] - '0');
	for (j=k-2;j>=0;j--) {
		printf(" * %d", input[maxpos-j] - '0');
	}
	printf(" = %lld\n", max);
#endif

	resultmax = max;
	resultmaxpos = maxpos;
	return max;
}

rettype
solve2(char *input, int k) {
	int i,j;
	rettype max=0;
	size_t maxpos=-1;
	rettype prod = 1;

	prod = 1;
	i = 0;
	while (input[i]) {
		if (prod > max && i >= k-1) {
			max = prod;
			maxpos = i-1;
		}
		if (input[i] != '0') {
			if (i >= k-1)
				prod /= input[i-k]-'0';
			prod *= input[i]-'0';
			i++;
		} else {
			while (input[i] == '0') {
				prod = 1;
				i++;
				for (j=0; j<k && input[i] && input[i] != '0'; j++) {
					prod *= input[i++]-'0';
				}
			}
		}
	}
#if 0
	printf("Max Value %lld seen at position %zu\n", max, maxpos);
	printf("%d", input[maxpos-k+1] - '0');
	for (j=k-2;j>=0;j--) {
		printf(" * %d", input[maxpos-j] - '0');
	}
	printf(" = %lld\n", max);
#endif
	resultmax = max;
	resultmaxpos = maxpos;
	return max;
}

#define F(a,b,c,d) ((unsigned char)(a)<<24| \
					(unsigned char)(b)<<16| \
					(unsigned char)(c)<<8|	\
					(unsigned char)(d))

unsigned factors_diff[] = {
	0, 
	0, 
	F(1,0,0,0), 
	F(0,1,0,0),
	F(2,0,0,0),
	F(0,0,1,0),
	F(1,1,0,0),
	F(0,0,0,1),
	F(3,0,0,0),
	F(0,2,0,0)
};


rettype
solve3(char *input, int k) {
	int i,j;
	rettype max=0;
	size_t maxpos=-1;
	unsigned factors_count = F(0,0,0,0);

	rettype pows[4][255];
	for (int i=0; i<255;i++) {
		pows[0][i] = i==0 ? 1 : pows[0][i-1]*2;
		pows[1][i] = i==0 ? 1 : pows[1][i-1]*3;
		pows[2][i] = i==0 ? 1 : pows[2][i-1]*5;
		pows[3][i] = i==0 ? 1 : pows[3][i-1]*7;
	}

	i=0;
	while (input[i]) {
		unsigned long long prod =
			pows[0][(factors_count>>24) & 0xFF] *
			pows[1][(factors_count>>16) & 0xFF] *
			pows[2][(factors_count>>8) & 0xFF] *
			pows[3][(factors_count) & 0xFF];
		if (prod > max && i >= k-1) {
			max = prod;
			maxpos = i-1;
		}
		if (input[i] != '0') {
			if (i >= k-1)
				factors_count -= factors_diff[input[i-k]-'0'];
			factors_count += factors_diff[input[i]-'0'];
			i++;
		} else {
			while (input[i] == '0') {
				factors_count = F(0,0,0,0);
				i++;
				for (j=0; j<k && input[i] && input[i] != '0'; j++) {
					factors_count += factors_diff[input[i++]-'0'];
				}
			}
		}
	}
#if 0
	printf("Max Value %lld seen at position %zu\n", max, maxpos);
	printf("%d", input[maxpos-k+1] - '0');
	for (j=k-2;j>=0;j--) {
		printf(" * %d", input[maxpos-j] - '0');
	}
	printf(" = %lld\n", max);
#endif
	resultmax = max;
	resultmaxpos = maxpos;
	return max;
}

rettype
solve4(char *input, int k) {
	int i,j;
	rettype max=0;
	size_t maxpos=-1;
	unsigned factors_count = F(0,0,0,0);

	rettype pows[4][255];
	unsigned maxclz=0, clzs[4][255];
	for (int i=0; i<255;i++) {
		pows[0][i] = i==0 ? 1 : pows[0][i-1]*2;
		pows[1][i] = i==0 ? 1 : pows[1][i-1]*3;
		pows[2][i] = i==0 ? 1 : pows[2][i-1]*5;
		pows[3][i] = i==0 ? 1 : pows[3][i-1]*7;
		clzs[0][i] = log2(pow(2,i));
		clzs[1][i] = log2(pow(3,i));
		clzs[2][i] = log2(pow(5,i));
		clzs[3][i] = log2(pow(7,i));
	}

	i=0;
	while (input[i]) {
		unsigned prodclz =
			clzs[0][(factors_count>>24) & 0xFF] +
			clzs[1][(factors_count>>16) & 0xFF] +
			clzs[2][(factors_count>>8) & 0xFF] +
			clzs[3][(factors_count) & 0xFF];
		if (prodclz+4 > maxclz) {
			rettype prod =
				pows[0][(factors_count>>24) & 0xFF] *
				pows[1][(factors_count>>16) & 0xFF] *
				pows[2][(factors_count>>8) & 0xFF] *
				pows[3][(factors_count) & 0xFF];
			if (prod > max && i >= k-1) {
				max = prod;
				maxclz = prodclz;
				maxpos = i-1;
			}
		}
		if (input[i] != '0') {
			if (i >= k-1)
				factors_count -= factors_diff[input[i-k]-'0'];
			factors_count += factors_diff[input[i]-'0'];
			i++;
		} else {
			while (input[i] == '0') {
				factors_count = F(0,0,0,0);
				i++;
				for (j=0; j<k && input[i] && input[i] != '0'; j++) {
					factors_count += factors_diff[input[i++]-'0'];
				}
			}
		}
	}
#if 0
	printf("Max Value %lld seen at position %zu\n", max, maxpos);
	printf("%d", input[maxpos-k+1] - '0');
	for (j=k-2;j>=0;j--) {
		printf(" * %d", input[maxpos-j] - '0');
	}
	printf(" = %lld\n", max);
#endif
	resultmax = max;
	resultmaxpos = maxpos;
	return max;
}


rettype
solve5(char *input, int k) {
	int i,j;
	rettype max=0;
	size_t maxpos=-1;
	unsigned factors_count = F(0,0,0,0), maxfactors=0;

	rettype pows[4][255];
	double maxclz=0, clzs[4][255];
	for (int i=0; i<255;i++) {
		pows[0][i] = i==0 ? 1 : pows[0][i-1]*2;
		pows[1][i] = i==0 ? 1 : pows[1][i-1]*3;
		pows[2][i] = i==0 ? 1 : pows[2][i-1]*5;
		pows[3][i] = i==0 ? 1 : pows[3][i-1]*7;
		clzs[0][i] = log2(pow(2,i));
		clzs[1][i] = log2(pow(3,i));
		clzs[2][i] = log2(pow(5,i));
		clzs[3][i] = log2(pow(7,i));
	}

	i=0;
	while (input[i]) {
		double prodclz =
			clzs[0][(factors_count>>24) & 0xFF] +
			clzs[1][(factors_count>>16) & 0xFF] +
			clzs[2][(factors_count>>8) & 0xFF] +
			clzs[3][(factors_count) & 0xFF];
		if (prodclz > maxclz) {
			maxclz = prodclz;
			maxpos = i-1;
			maxfactors = factors_count;
		}
		if (input[i] != '0') {
			if (i >= k-1)
				factors_count -= factors_diff[input[i-k]-'0'];
			factors_count += factors_diff[input[i]-'0'];
			i++;
		} else {
			while (input[i] == '0') {
				factors_count = F(0,0,0,0);
				i++;
				for (j=0; j<k && input[i] && input[i] != '0'; j++) {
					factors_count += factors_diff[input[i++]-'0'];
				}
			}
		}
	}

	max = (pows[0][(maxfactors>>24) & 0xFF] *
		   pows[1][(maxfactors>>16) & 0xFF] *
		   pows[2][(maxfactors>>8) & 0xFF] *
		   pows[3][(maxfactors) & 0xFF]);

#if 0
	printf("Max Value %lld seen at position %zu\n", max, maxpos);
	printf("%d", input[maxpos-k+1] - '0');
	for (j=k-2;j>=0;j--) {
		printf(" * %d", input[maxpos-j] - '0');
	}
	printf(" = %lld\n", max);
#endif
	resultmax = max;
	resultmaxpos = maxpos;
	return max;
}



#include <time.h>

#define REPS 100

int 
main(int argc, char *argv[], char *envp[]) {
	int n,k;
	char *input;
	struct timespec a,b,x;
	rettype ns;
#if 0
	rettype expectedmax;
	size_t expectedmaxpos;
#endif
	

	if (argc > 1) {
		n = atoi(argv[1]);
	} else {
		n = sizeof(ex_input);
	}
	if (argc > 2) {
		k = atoi(argv[2]);
	} else {
		k = 13;
	}
	if (argc > 3) {
		srand(atoi(argv[3]));
	} else {
		clock_gettime(CLOCK_MONOTONIC, &x);
		srand(x.tv_sec ^ x.tv_nsec);
	}
	if (n < 0 || k < 0 || k > n) {
		fprintf(stderr, "Usage: %s [N] [K] [seed]\n", argv[0]);
		exit(1);
	}

	if (n==sizeof(ex_input)) {
		input = ex_input;
	} else {
		input = generate(n);
	}

	printf("n, k, algorithm, time (us)\n");

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &a);
	for (int x=0;x<REPS;x++)
		(void)solve1(input, k);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &b);
	ns = ((b.tv_sec-a.tv_sec)*1000000000LL + (b.tv_nsec-a.tv_nsec))/REPS;
	printf("%d, %d, %d, %.3f\n", n, k, 1, (double)ns/1000);

#if 0
	expectedmax = resultmax;
	expectedmaxpos = resultmaxpos;
#endif
	printf("Digits: %.*s\n", k, input+resultmaxpos-k+1);

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &a);
	for (int x=0;x<REPS;x++)
		(void)solve2(input, k);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &b);
	ns = ((b.tv_sec-a.tv_sec)*1000000000LL + (b.tv_nsec-a.tv_nsec))/REPS;
	printf("%d, %d, %d, %.3f\n", n, k, 2, (double)ns/1000);
#if DEBUG
	printf("Digits: %.*s\n", k, input+resultmaxpos-k+1);
#endif

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &a);
	for (int x=0;x<REPS;x++)
		(void)solve3(input, k);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &b);
	ns = ((b.tv_sec-a.tv_sec)*1000000000LL + (b.tv_nsec-a.tv_nsec))/REPS;
	printf("%d, %d, %d, %.3f\n", n, k, 3, (double)ns/1000);
#if DEBUG
	printf("Digits: %.*s\n", k, input+resultmaxpos-k+1);
#endif

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &a);
	for (int x=0;x<REPS;x++)
		(void)solve4(input, k);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &b);
	ns = ((b.tv_sec-a.tv_sec)*1000000000LL + (b.tv_nsec-a.tv_nsec))/REPS;
	printf("%d, %d, %d, %.3f\n", n, k, 4, (double)ns/1000);
#if DEBUG
	printf("Digits: %.*s\n", k, input+resultmaxpos-k+1);
#endif

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &a);
	for (int x=0;x<REPS;x++)
		(void)solve5(input, k);
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &b);
	ns = ((b.tv_sec-a.tv_sec)*1000000000LL + (b.tv_nsec-a.tv_nsec))/REPS;
	printf("%d, %d, %d, %.3f\n", n, k, 5, (double)ns/1000);
#if DEBUG
	printf("Digits: %.*s\n", k, input+resultmaxpos-k+1);
#endif
}
