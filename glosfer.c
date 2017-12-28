#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>

#define Q_MAX_NUM 2000
#define DP_MAX    1000

struct q_node {
	int x;
	int y;
};

struct q_node queue[Q_MAX_NUM];
int q_rptr = 0;  // empty : rptr == wptr, full : not accepted
int q_wptr = 0;  

int dp[/*-,+*/2][/*x*/DP_MAX][/*-,+*/2][/*y*/DP_MAX];  // cache

bool check_valid(int x, int y) {
	int sum = 0;
	int const limit = 19;

	x = abs(x);
	y = abs(y);

	while(x > 0) {
		sum += x % 10;
		x /= 10;
	}

	while(y > 0) {
		sum += y % 10;
		y /= 10;
	}

	if (sum > limit) 
		return false;

	return true;
}

bool q_push(int x, int y) {
	int x_sign = 1, y_sign = 1;

	if ( x < 0 ) x_sign = 0;
	if ( y < 0 ) y_sign = 0;
  
	if (dp[x_sign][abs(x)][y_sign][abs(y)] == 1) return false;	
	if (check_valid(x, y) == false) return false;

	queue[q_wptr].x = x;
	queue[q_wptr].y = y;

	q_wptr = (q_wptr + 1) % Q_MAX_NUM;
	dp[x_sign][abs(x)][y_sign][abs(y)] = 1;

	if (q_wptr == q_rptr) {
		printf("err. queue is full\n");
		exit(1);
	}

	return true;
}

bool q_pop(int *x, int *y) {
	if (q_rptr == q_wptr) 
		return false;
	
	*x = queue[q_rptr].x;
	*y = queue[q_rptr].y;

	q_rptr = (q_rptr + 1) % Q_MAX_NUM;

	return true;
}


int main() {
	int count = 0;
	int x, y;

	memset(dp, 0, sizeof(dp));
	memset(queue, 0, sizeof(queue));

	// BFS
	q_push(0,0);
	while(q_pop(&x, &y)) {
		if (q_push(x, y+1) == true) count++;
		if (q_push(x+1, y) == true) count++;
		if (q_push(x, y-1) == true) count++;
		if (q_push(x-1, y) == true) count++;
	}

	printf("answer is %d\n",  count + 1);
}
