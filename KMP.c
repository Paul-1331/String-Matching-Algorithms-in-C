#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXS 1000001
#define MAXP 100001


char s[MAXS];
char p[MAXP];

int*LPS(char*p,int m){
	int*lps = malloc(m*sizeof(int));
	lps[0] = 0;
	int len = 0;
	int i = 1;
	
	while(i<m){
		if(p[i]==p[len]){
			len++;
			lps[i] = len;
			i++;
		}
		else{
			if(len>0){
				len = lps[len-1];
			}
			else{
				lps[i] = 0;
				i++;
			}
		}
	}
	return lps;
}

int*kmp(char*s,char*p,int*ressize){
	int n = strlen(s);
	int m = strlen(p);
	
	int*lps = LPS(p,m);
	int*res = malloc(n*sizeof(int));
	
	int i = 0;
	int j = 0;
	
	while(i<n){
		if(s[i]==p[j]){
			i++;
			j++;
		
			if(j==m){
				res[(*ressize)++] = i-j;
				j = lps[j-1];
			}
		}
		else{
			if(j>0){
				j = lps[j-1];
			}
			else{
				i++;
			}
		}
	}
	return res;
}

int main()
{
	scanf("%s",s);
	scanf("%s",p);
	int ressize = 0;
	int*result = kmp(s,p,&ressize);
	if(!ressize){
		printf("-1\n");
		return 0;
	}
	printf("%d %d",result[ressize-1],ressize);
	return 0;
}
