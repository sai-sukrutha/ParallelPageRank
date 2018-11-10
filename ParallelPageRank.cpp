#include<iostream>
#include<vector>
#include<array>
#include<algorithm>
#include <sstream>
#include <string>
#include <stdio.h>
#include <chrono> 
#include <omp.h>

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
vector<double> unidimensionalA;
vector<double> unidimensionalB;

void convert(vector<vector<double>> &a,vector<vector<double>> &b,int size)
{
	unidimensionalA.clear();
	unidimensionalB.clear();
	
for(int i=0;i<size;i++)
{
	for(int j=0;j<size;j++)
	{
		unidimensionalA.push_back(a[i][j]);
		unidimensionalB.push_back(b[j][i]);
	}
}

}
void mul(vector<vector<double>> &a,vector<vector<double>> &b)
{

 vector<vector<double>> mul(powerranks.size(),vector<double>(powerranks.size(),0)); 
int i,j,k,iOff, jOff;
	double tot;
	long size=powerranks.size();
	convert(a,b,size);
	#pragma omp parallel shared(mul) private(i, j, k, iOff, jOff, tot) num_threads(5)
	{

 #pragma omp for schedule(static)
    for ( i = 0; i < size; i++) 
    { 
    	iOff = i * size;
        for ( j = 0; j < size; j++) 
        { 
        	jOff = j * size;
            tot = 0; 
            for ( k = 0; k < size; k++) 
            	tot += unidimensionalA[iOff + k] * unidimensionalB[jOff + k];

                mul[i][j] =tot;
        } 
    } 
  
   
    for ( i=0; i<size; i++) 
        for ( j=0; j<size; j++) 
            powerMatrix[i][j] = mul[i][j];
}
}



void mulwithrank(vector<vector<double>> &adj,vector<double> &v)
{
	int size=powerranks.size();
vector<double>mul(size,0); 
#pragma omp parallel for
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
        	
        	
        	mul(powerMatrix,initialMatrix);
        	
		mulwithrank(powerMatrix,rankinit);
		if(checksamepower())
			break;
		else
			copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());
		

			}
	
}
bool sortbyrank(const pair<int,double> &a, const pair<int,double> &b) 
{ 
    return (a.second > b.second); 
} 
int main()

{
	int nodes;
	

	freopen("testp.txt","w",stdout);
	cout<<200<<endl;
	cout<<4000<<endl;
	int e=4000,k=0;
	
	auto start = high_resolution_clock::now();
	edge.resize(4000);
	while(k< e)
	{
		edge[k].push_back(rand()%200);
		edge[k].push_back(rand()%200);
		k++;
	}
	for(int i=0;i<e;i++)
	{
		cout<<edge[i][0]<<" "<<edge[i][1];
		cout<<endl;
	}
	
	freopen("testp.txt","r",stdin);
	freopen("outputp.txt","w",stdout);
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

#pragma omp parallel for
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
adj.clear();
outdegree.clear();
edge.clear();
initialMatrix.assign(powerMatrix.begin(),powerMatrix.end());
cout.precision(30);

calculateRankPower();
vector <pair<int,double>> powerrank_vect;
cout<<"Power matrix Page Ranks  "<<endl;
for(int j=0;j<nodes;j++)
                 {
                       powerrank_vect.push_back(make_pair(j,powerranks[j]));

			}
			  sort(powerrank_vect.begin(),powerrank_vect.end(),sortbyrank);
			   cout<<"Rank\t Id\tScore                  "<<endl;
		for(int i=0;i<nodes;i++)
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
