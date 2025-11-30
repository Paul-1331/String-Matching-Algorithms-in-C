#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXS 1000001
#define MAXP 100001


char s[MAXS];
char p[MAXP];

int min(int a,int b){
	return a<b?a:b;
}

int*z_function(char*s){
	int n = strlen(s);
	int*z = calloc(n,sizeof(int));
	int l = 0;
	int r = 0;
	
	for(int i = 1;i<n;i++){
		if(i<=r){
			z[i] = min(r-i+1,z[i-l]);
		}
		
		while(i+z[i]<n&&s[z[i]]==s[i+z[i]]){
			z[i]++;
		}
		
		if(i+z[i]-1>r){
			l = i;
			r = i+z[i]-1;
		}
	}
	return z;
}


int*z_algo(char*s,char*p,int*ressize){
	int n = strlen(s);
	int m = strlen(p);
	int*result = malloc((n-m+1)*sizeof(int));
	char*t = malloc((n+m+2));
	strcpy(t,p);
	t[m] = '$';
	strcpy(t+m+1,s);
	int*z = z_function(t);
	n = strlen(t);
	for(int i = m+1;i<n;i++){
		if(z[i]==m){
			result[(*ressize)++] = i-(m+1);
		}
	}
	return result;	
}


int main()
{
	scanf("%[^\n]",s);
	getchar();
	scanf("%[^\n]",p);
	getchar();
	int ressize = 0;
	int*result = z_algo(s,p,&ressize);
	if(!ressize){
		printf("-1\n");
		return 0;
	}
	for(int i = 0;i<ressize;i++){
		printf("%d ",result[i]);
	}
	printf("\n");
	return 0;
}
