#include "longestArithSeqLength.h"

int longestArithSeqLength(int* nums, int n){

    if(n<=2) return n;

    int dp[1001][1001] = {0};
    int maxlen = 2;

    for(int i=0;i<n-1;i++){
        int aux = nums[i];
        for(int j=i+1;j<n;j++){
            int diff = (nums[j]-aux)+500;
            dp[j][diff] = (2 > (dp[i][diff]+1)) ? 2 : (dp[i][diff]+1);
            maxlen = maxlen > dp[j][diff] ? maxlen:dp[j][diff];            
        }
    }
    
    return maxlen;  
}