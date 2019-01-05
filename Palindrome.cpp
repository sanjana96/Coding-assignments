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
#include <limits>
#include <string>
#include <fstream>
#include <limits.h>
#define pb push_back
#define mp make_pair
#define mod 1000000007
using namespace std;
 
void palindrome(string s, int limit)
{
    map<string, int> m;
    int n = s.size();

    int R[2][n+1];
    s = "@" + s + "#";
 
    for (int j = 0; j <= 1; j++)
    {
        int rp = 0;   
        R[j][0] = 0;
 
        int i = 1;
        while (i <= n)
        {
            while (s[i - rp - 1] == s[i + j + rp])
                rp++;  
            R[j][i] = rp;
            int k = 1;
            while ((R[j][i - k] != rp - k) && (k < rp))
            {
                R[j][i + k] = min(R[j][i - k],rp - k);
                k++;
            }
            rp = max(rp - k,0);
            i += k;
        }
    }
    s = s.substr(1, n);

    m[string(1, s[0])]=1;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j <= 1; j++)
            for (int rp = R[j][i]; rp > 0; rp--)
               m[s.substr(i - rp - 1, 2 * rp + j)]=1;
        m[string(1, s[i])]=1;
    }

   map<string, int>::iterator ii;
   for (ii = m.begin(); ii!=m.end(); ++ii)
   {
   	if((*ii).first.length()>=limit && ((*ii).first).find(" ")==string::npos && ((*ii).first).find("\n")==string::npos)
   		cout << (*ii).first<<"   "<<s.find((*ii).first)<<'\n';
   }
}
 
int main()
{
	ifstream file("AESOP TALES (with random URLs).txt");
	string str;
	string txt="";	
	while (getline(file, str)){
		txt += str;
	 	txt.push_back('\n');
	} 
    // Find all palindromes of length passed in the file given
    palindrome(txt, 5);
    return 0;
}
