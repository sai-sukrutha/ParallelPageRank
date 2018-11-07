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
vector<int> outdegree;
vector<vector<bool>> adj;
vector<vector<double>>powerMatrix;
vector<vector<double>> initialMatrix;
vector<double>rankinit;
vector<vector<int>> edge;

bool checksamepower()
{
int j,count=0;
int size=powerranks.size();
	
		for(j=0;j<size;j++)
		{

			if(abs(powerPrevranks[j]-powerranks[j])<0.0001)
				count++;
		}
	
	if(count==size)
		return true;
	else
		return false;
}

void mul(vector<vector<double>> a,vector<vector<double>> b)
{
int size=powerranks.size();
 vector<vector<double>> mul(size,vector<double>(size,0)); 
    for (int i = 0; i < size; i++) 
    { 
        for (int j = 0; j < size; j++) 
        { 
            mul[i][j] = 0; 
            for (int k = 0; k < size; k++) 
                mul[i][j] += a[i][k]*b[k][j]; 
        } 
    } 
  
   
    for (int i=0; i<size; i++) 
        for (int j=0; j<size; j++) 
            powerMatrix[i][j] = mul[i][j];


}

void power(vector<vector<double>> &p,int n)
{
if(n==1)
	return;
else
{
	power(p,n/2);
	mul(p,p);
	if(n%2)
		mul(p,initialMatrix);
}

}

void mulwithrank(vector<vector<double>> adj,vector<double> v)
{
	int size=powerranks.size();
vector<double>mul(size,0); 
    for (int i = 0; i< size; i++) 
    { 
        for (int j = 0; j < 1; j++) 
        { 
            mul[i]= 0; 
            for (int k = 0; k < size; k++) 
                mul[i] += adj[i][k]*v[k]; 
        } 
    } 
  
   for(int i=0;i<size;i++)
   	powerranks[i]=mul[i];
  


}

void calculateRankPower()
{
	rankinit.assign(powerranks.begin(),powerranks.end());
	
	for(int i=1;i<100;i++)
        {
        	
        	power(powerMatrix,i);
        	
		mulwithrank(powerMatrix,rankinit);
		if(checksamepower())
			break;
		else
			copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());
		

			}
	
}

int main()

{
	int nodes;
	

	 //srand(time(0)); 
  
	//std::ofstream of;
    //of.open( "test.txt" );
     freopen("test.txt","w",stdout);
    //teedev td( of, std::cout );
    //tee_stream ts(td);
	cout<<1000<<endl;
	cout<<40000<<endl;
	long e=40000,k=0;
	
	//vector<vector<int>> edge;
	//long edge[e][2];
         edge.clear();
	auto start = high_resolution_clock::now();
	edge.resize(40000);
	while(k < e)
	{
		  
		edge[k].push_back(rand()%1000);
		edge[k].push_back(rand()%1000);
		
		k++;
	}
	
	for(int i=0;i<e;i++)
	{
		cout<<edge[i][0]<<" "<<edge[i][1];
		//c++;
		cout<<endl;
	}
	
	//of.close();
	edge.clear();
	freopen("test.txt","r",stdin);
	freopen("output.txt","w",stdout);
cout<<"Enter Nodes: "<<endl;
 cin>>nodes;
  for(int i=0;i<nodes;i++)
{


	
	powerranks.push_back(1.0/nodes);

}
powerPrevranks.resize(nodes);
copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());

outdegree.resize(nodes);
adj.resize(nodes);
for (int i = 0; i < nodes; ++i)
    adj[i].resize(nodes);

for(int i=0;i<nodes;i++)
{
	for(int j=0;j<nodes;j++)
	{
adj[i][j]=false;
	}
	}
powerMatrix.resize(nodes);
for (int i = 0; i < nodes; ++i)
    powerMatrix[i].resize(nodes);
initialMatrix.resize(nodes);
for (int i = 0; i < nodes; ++i)
    initialMatrix[i].resize(nodes);
int edges,m,n;
cout<<"Enter Edges: "<<endl;
cin>>edges;
while(edges--)
{       cout<<"Enter Head and Tail"<<endl;
	cin>>m>>n;
	outdegree[n]++;
adj[m][n]=true;
}


for(int i=0;i<nodes;i++)
{
	for(int j=0;j<nodes;j++)
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

initialMatrix.assign(powerMatrix.begin(),powerMatrix.end());
cout.precision(30);

calculateRankPower();
cout<<"Power matrix Page Ranks  "<<endl;
for(int j=0;j<powerranks.size();j++)
                 {
                         cout<<" "<<powerranks[j];

			}
		
cout<<endl;
cout<<"Max-Rank Power matrix  "<<*max_element(powerranks.begin(),powerranks.end())<<endl;
auto stop = high_resolution_clock::now(); 
  auto duration = duration_cast<microseconds>(stop - start); 
 cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl; 





	return 0;
}