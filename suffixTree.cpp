#include <iostream>
#include <vector> 
#include <queue>
#include <map> 
#include <set>
#include <utility> //Pair
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <bitset>
#include <limits.h>
#include <string>
#include <fstream>
#define pb push_back
#define mp make_pair
#define mod 1000000007
using namespace std;

int* suffixArray;
int* lcp;

typedef struct{
	int index;
	int rank[2];
}suffix;

bool cmp(suffix a,suffix b){
	return a.rank[0]==b.rank[0]?a.rank[1]<b.rank[1]:a.rank[0]<b.rank[0];
}

void buildSuffixArray(string txt){
	int n=txt.length();
	suffix a[n];
	for(int i=0;i<n;i++){
		a[i].index=i;
		a[i].rank[0]=txt[i]-97;
		a[i].rank[1]=i+1<n?txt[i+1]-97:-1;
	}	
	sort(a,a+n,cmp);
	int index[n];
	for(int i=2;i<n;i*=2){
		int rank=0;
		a[0].rank[0]=rank;
		int prevrank=0;
		index[a[i].index]=0;
		for(int j=1;j<n;j++){
			if(a[j].rank[0]==prevrank && a[j].rank[1]==a[j-1].rank[1]){	
				a[j].rank[0]=rank;				
			}
			else{
				prevrank=a[j].rank[0];
				a[j].rank[0] = ++rank;			
			}
			index[a[j].index]=j;
		}
		for(int j=0;j<n;j++){
			int nextindex=a[j].index+i;
			if(nextindex<n){
				a[j].rank[1]=a[index[nextindex]].rank[0];
			}
			else	
				a[j].rank[1]=-1;
		}
		sort(a,a+n,cmp);		
	}
	suffixArray=new int[n];
	for(int i=0;i<n;i++){
		suffixArray[i]=a[i].index;
	}
}
void buildLCPkasai(string txt){
	int n=txt.length();
    int k=0;
    vector<int> rank(n,0);

    for(int i=0; i<n; i++) rank[suffixArray[i]]=i;

	lcp = new int[n];
    for(int i=0; i<n; i++, k?k--:0){
        if(rank[i]==n-1) {k=0; continue;}
        int j=suffixArray[rank[i]+1];
        while(i+k<n && j+k<n && txt[i+k]==txt[j+k]) k++;
        lcp[rank[i]]=k;
    }
}
string processtxt(string txt){
	int n=txt.length();
	string txt1="";
	for(int i=0;i<n;i++){
		char curr=txt[i];
		if(curr>=65 && curr<91){
			txt1+=(char)(curr+32);
		}
		else txt1+=curr;
	}
	return txt1;	
}
int noOfOccurences(string txt,string pattern){
	int l=0;
	int n=txt.length();
	int r=n-1;
	int count=0;
	int lenp=pattern.length();
	int till=n-1;
	
	//Process pattern
	string pat1="";
	for(int i=0;i<lenp;i++){
		char curr=pattern[i];
		if(curr>=65 && curr<91){
			pat1+=(char)(curr+32);
		}
		else pat1+=curr;
	}
	pattern=pat1;

	int mid=(l+r)/2;
	string curr=txt.substr(suffixArray[mid]);
	int l1=curr.length();
	int offset=0;
	while(offset<l1 && offset<lenp && curr.at(offset)==pattern.at(offset)){
		offset++;
	}		
	if(offset==lenp){
		count+=1;
		int temp=mid;
		while(lcp[temp]>=lenp){
			count+=1;temp+=1;
		}
		temp=mid-1;
		while(lcp[temp]>=lenp){
			count+=1;temp-=1;
		}
		return count;
	}
	if(curr<pattern)l=mid+1;
	else r=mid-1;
	int prevlcp=offset;
	while(l<=r){
			int prevmid=mid;
			mid=(l+r)/2;
			int start=prevmid<mid?prevmid:mid;
			int end=prevmid<mid?mid:prevmid;
			int min =INT_MAX;
			for(int i=start;i<end;i++){
				if(lcp[i]<min)min=lcp[i];
			}
			if(min>prevlcp){
				if(l>prevmid)l=mid+1;
				else r=mid-1;
			}
			else if(min<prevlcp){
				if(l<prevmid)l=mid+1;
				else r=mid-1;
				prevlcp=min;
			}
			else{
				curr=txt.substr(suffixArray[mid]);
				int currl=curr.length();
				offset=min;
				while(offset<currl && offset<lenp && curr.at(offset)==pattern.at(offset)){
					offset++;
				}
				prevlcp=offset;
				if(offset==lenp){
					count+=1;
					int temp=mid;
					while(lcp[temp]>=lenp){
					count+=1;temp+=1;
					}
					temp=mid-1;
					while(lcp[temp]>=lenp){
						count+=1;temp-=1;
					}
					return count;
				}
				if(offset<currl){
					if(curr.at(offset)>pattern.at(offset))r=mid-1;
					else l=mid+1;
				}
				else{
					l=mid+1;
				}
			}
	}
	return count;
}
int main(){
	ifstream file("AESOP TALES (with random URLs).txt");
	string str;
	string txt="";
	while (getline(file, str)){
		txt += str;
	 	txt.push_back('\n');
	} 
	txt=processtxt(txt);
    int n = txt.length();

    buildSuffixArray(txt);
	buildLCPkasai(txt);
	cout<<noOfOccurences(txt,"wolf")<<endl;
    return 0;
}
