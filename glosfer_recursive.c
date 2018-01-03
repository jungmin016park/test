#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>

#define DP_MAX    1000
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

int solve(int x, int y) {
    int ret = 0;
    int x_sign = 0, y_sign = 0;  // - : 0, + : 1
    
    if ( x >= 0 ) x_sign = 1;  
    if ( y >= 0 ) y_sign = 1;  
    if (dp[x_sign][abs(x)][y_sign][abs(y)] == 1) return 0;  
    if (check_valid(x, y) == false) return 0;
    
    dp[x_sign][abs(x)][y_sign][abs(y)] = 1;
    ret++;

    ret += solve(x-1, y); 
    ret += solve(x, y-1);
    ret += solve(x+1, y); 
    ret += solve(x, y+1);

    return ret;
}

int main() {

    memset(dp, 0, sizeof(dp));

    printf("answer is %d\n", solve(0, 0));
}
