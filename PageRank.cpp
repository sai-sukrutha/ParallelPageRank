#include<iostream>
#include<vector>
#include<array>
#include<algorithm>
#include <sstream>
#include <string>
#include <stdio.h>
#include <chrono> 
#include <iostream>
#include <fstream>

#include <stdlib.h> 

using namespace std::chrono; 
using namespace std;

vector<double> powerranks;
vector<double> powerPrevranks;
vector<long long> outdegree;
vector<vector<bool>> adj;
vector<vector<double>>powerMatrix;


vector<vector<long long>> edge;
long long nodes,edges;

bool checksamepower()
{
long long j,count=0,flag=0;

	
		for(j=0;j<nodes;j++)
		{

			if(abs(powerPrevranks[j]-powerranks[j])>0.0001)
			{
				flag=1;
				break;
			}
				
		}
			
	
	if(flag==0)
		return true;
	else
		return false;
}





void mulwithrank(vector<vector<double>> &adj,vector<double> &v)
{
	
vector<double>mul(nodes,0); 
    for (long long i = 0; i< nodes; i++) 
    { 
        for (long long j = 0; j < 1; j++) 
        { 
            mul[i]= 0; 
            for (long long k = 0; k < nodes; k++) 
                mul[i] += adj[i][k]*v[k]; 
        } 
    } 
  
   for(long long i=0;i<nodes;i++)
   	powerranks[i]=mul[i];
  


}

void calculateRankPower()
{
	
	
	for(int i=1;i<100;i++)
        {
        	
        	
        	
		mulwithrank(powerMatrix,powerPrevranks);
		if(checksamepower())
			break;
		else
			copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());
		

			}
	
}
bool sortbyrank(const pair<long long,double> &a, const pair<long long,double> &b) 
{ 
    return (a.second > b.second); 
} 
int main()

{
	
	
     /*freopen("test.txt","w",stdout);
  
	cout<<500<<endl;
	cout<<10000<<endl;
	long e=10000,k=0;
	
        
	
	edge.resize(10000);
	while(k < e)
	{
		  
		edge[k].push_back(rand()%500);
		edge[k].push_back(rand()%500);
		
		k++;
	}
	
	for(long long i=0;i<e;i++)
	{
		cout<<edge[i][0]<<" "<<edge[i][1];
		
		cout<<endl;
	}
	
	
	freopen("test.txt","r",stdin);
	freopen("output.txt","w",stdout);*/
	
cout<<"Enter Nodes: "<<endl;
 cin>>nodes;
  for(long long i=0;i<nodes;i++)
{


	
	powerranks.push_back(1.0/nodes);

}
powerPrevranks.resize(nodes);
copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());

outdegree.resize(nodes);
adj.resize(nodes);
for (long long i = 0; i < nodes; ++i)
    adj[i].resize(nodes);

for(long long i=0;i<nodes;i++)
{
	for(long long j=0;j<nodes;j++)
	{
adj[i][j]=false;
	}
	}
powerMatrix.resize(nodes);
for (long long i = 0; i < nodes; ++i)
    powerMatrix[i].resize(nodes);


long long m,n;
cout<<"Enter Edges: "<<endl;
cin>>edges;
while(edges--)
{       cout<<"Enter Head and Tail"<<endl;
	cin>>m>>n;
	outdegree[n]++;
adj[m][n]=true;
}

auto start = high_resolution_clock::now();
for(long long i=0;i<nodes;i++)
{
	for(long long j=0;j<nodes;j++)
	{
		if(adj[i][j])
		{
			//double x=(1.0/outdegree[j]);
			powerMatrix[i][j]=0.85*(1.0/outdegree[j])+0.15/nodes;
		}
		if(outdegree[j]==0)
		{
			powerMatrix[i][j]=1.0/nodes;
		}
	}
}

	edge.clear();
	adj.clear();
	outdegree.clear();

cout.precision(30);

calculateRankPower();
powerPrevranks.clear();
powerMatrix.clear();
vector <pair<int,double>> powerrank_vect;
cout<<"Power matrix Page Ranks  "<<endl;
for(long long j=0;j<nodes;j++)
                 {
                       powerrank_vect.push_back(make_pair(j,powerranks[j]));

			}
			  sort(powerrank_vect.begin(),powerrank_vect.end(),sortbyrank);
			  cout<<"Rank\t Id\tScore                  "<<endl;
		for(long long i=0;i<nodes;i++)
		{
			cout<<i+1<<"        "<<powerrank_vect[i].first<<"   "<<powerrank_vect[i].second<<endl;
			
		}
		
		
		powerrank_vect.clear();
		
cout<<endl;

auto stop = high_resolution_clock::now(); 
  auto duration = duration_cast<microseconds>(stop - start); 
 cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl; 





	return 0;
}