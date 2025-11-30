#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MOD 1000000007
#define BASE 31
#define MAXS 1000001
#define MAXP 100001


char s[MAXS];
char p[MAXP];

int hash[MAXS];
int power[MAXS];

int add(int a,int b){
	a+=b;
	if(a>=MOD) a-=MOD;
	return a;
}

int sub(int a,int b){
	a-=b;
	if(a<0) a+=MOD;
	return a;
}

int mul(int a,int b){
	return (int)(((long long)a*b)%MOD);
}

int char_to_int(char s){
	return s-'a'+1;
}

void compute_hash(char*s){
	int n = strlen(s);
	hash[0] = char_to_int(s[0]);
	power[0] = 1;
	for(int i = 1;i<n;i++){
		hash[i] = add(mul(hash[i-1],BASE),char_to_int(s[i]));
		power[i] = mul(power[i-1],BASE);
	}
}

int sub_hash(int l,int r){
	int h = hash[r];
	if(l>0){
		h = sub(h,mul(hash[l-1],power[r-l+1]));
	}
	return h;
}


int*rabin_karp(char*s,char*p,int*ressize){
	int n = strlen(s);
	int m = strlen(p);
	int*result = malloc((n-m+1)*sizeof(int));
	
	int phash = char_to_int(p[0]);
	for(int i = 1;i<m;i++){
		phash = add(mul(phash,BASE),char_to_int(p[i]));
	}
	
	compute_hash(s);
	for(int i = 0;i<=n-m;i++){
		int subhash = sub_hash(i,i+m-1);
		if(subhash==phash){
			result[(*ressize)++] = i;
		}
	}
	return result;
}


int main()
{
	scanf("%s",s);
	scanf("%s",p);
	int ressize = 0;
	int*result = rabin_karp(s,p,&ressize);
	if(!ressize){
		printf("-1\n");
		return 0;
	}
	printf("%d %d",result[0],ressize);
	return 0;
}
