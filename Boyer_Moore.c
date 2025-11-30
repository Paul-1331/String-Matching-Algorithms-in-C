#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXS 10000001
#define MAXP 100001

char s[MAXS];
char p[MAXP];

int max(int a,int b){
	return a>b?a:b;
}

int*boyer_moore(char*s,char*p,int*ressize){
	int n = strlen(s);
	int m = strlen(p);
	int*res = malloc((n-m+1)*sizeof(int));
	
	int badchar[256];
	for(int i = 0;i<256;i++){
		badchar[i] = -1;
	}
	for(int i = 0;i<m;i++){
		badchar[p[i]] = i;
	}
	
	int shift = 0;
	while(shift<=n-m){
		int j = m-1;
		while(j>=0&&p[j]==s[shift+j]){
			j--;
		}
		if(j<0){
			res[(*ressize)++] = shift;
			shift+=(shift+m<n)?m-badchar[s[shift+m]]:1;
		}
		else{
			shift+= max(1,j-badchar[s[shift+j]]);
		}
	}
	return res;
}

int main()
{
	int i = 0;
	char c;
	while((c=getchar())!='\n'){
		s[i++] = c;
	}
	s[i] = '\0';
	i = 0;
	while((c=getchar())!='\n'){
		p[i++] = c;
	}
	p[i] = '\0';
	int ressize = 0;
	int*result = boyer_moore(s,p,&ressize);
	if(!ressize){
		printf("-1\n");
		return 0;
	}
	printf("%d\n",result[ressize-1]);
	return 0;
}
