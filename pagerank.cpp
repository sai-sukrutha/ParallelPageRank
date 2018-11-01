#include<iostream>
#include<vector>
#include<array>
#include<algorithm>
using namespace std;

vector<double> ranks;
vector<double> Prevranks;
vector<int> outdegree;
vector<vector<bool>> adj;

vector<vector<double>>powerMatrix;
vector<vector<double>> initialMatrix;
vector<double>rankinit;

bool checksame()
{
int j,count=0;
	
		for(j=0;j<ranks.size();j++)
		{

			if(abs(Prevranks[j]-ranks[j])<0.0001)
				count++;
		}
	
	if(count==ranks.size())
		return true;
	else
		return false;
}


void calculateRank()
{
	int count=1;
	while(1)
	{
count++;

for(int i=0;i<ranks.size();i++)
{ 
	for(int j=0;j<ranks.size();j++)
	{
		if(adj[i][j])
		{
           
			ranks[i]+=0.85*ranks[j]/outdegree[j];
		}
		
     }
    
     ranks[i]+=0.15/ranks.size();

}
if(checksame()||count==15)
	break;
else
	copy(ranks.begin(), ranks.end(), Prevranks.begin());
        
for(int i=0;i<ranks.size();i++)
{
cout<<"  "<<ranks[i];

}
cout<<"\n";
}

}
void mul(vector<vector<double>> a,vector<vector<double>> b)
{

 vector<vector<double>> mul(ranks.size(),vector<double>(ranks.size(),0)); 
    for (int i = 0; i < ranks.size(); i++) 
    { 
        for (int j = 0; j < ranks.size(); j++) 
        { 
            mul[i][j] = 0; 
            for (int k = 0; k < ranks.size(); k++) 
                mul[i][j] += a[i][k]*b[k][j]; 
        } 
    } 
  
   
    for (int i=0; i<ranks.size(); i++) 
        for (int j=0; j<ranks.size(); j++) 
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
vector<double>mul(ranks.size(),0); 
    for (int i = 0; i< ranks.size(); i++) 
    { 
        for (int j = 0; j < 1; j++) 
        { 
            mul[i]= 0; 
            for (int k = 0; k < ranks.size(); k++) 
                mul[i] += adj[i][k]*v[k]; 
        } 
    } 
  
   for(int i=0;i<ranks.size();i++)
   	ranks[i]=mul[i];


}
void calculateRankPower()
{
	rankinit.assign(ranks.begin(),ranks.end());
	
	for(int i=1;i<100;i++)
        {
        	
        	power(powerMatrix,i);
        	
		mulwithrank(powerMatrix,rankinit);
		if(checksame())
			break;
		else
			copy(ranks.begin(), ranks.end(), Prevranks.begin());
		cout<<endl;

		for(int j=0;j<ranks.size();j++)
                 {
                         cout<<" power "<<ranks[j];

			}
		cout<<"\n";
			}
	
}
int main()
{
  cout.precision(5);
  int count=0;
  int nplayers;
  cout<<"Enter Number of players";
  cin>>nplayers;
  for(int i=0;i<nplayers;i++)
{


	ranks.push_back(1.0/nplayers);
}
Prevranks.resize(nplayers);
copy(ranks.begin(), ranks.end(), Prevranks.begin());
for(int i=0;i<nplayers;i++)
{
cout<<" "<<ranks[i]<<"\n";
}
int m,n,matches;
outdegree.resize(nplayers);
adj.resize(nplayers);
for (int i = 0; i < nplayers; ++i)
    adj[i].resize(nplayers);
powerMatrix.resize(nplayers);
for (int i = 0; i < nplayers; ++i)
    powerMatrix[i].resize(nplayers);
initialMatrix.resize(nplayers);
for (int i = 0; i < nplayers; ++i)
    initialMatrix[i].resize(nplayers);

  while(count++ <1)
  {
  	cout<<"enter matches";
  	cin>>matches;
  	while(matches-- >0)
  	{
                cout<<"Enter winner and loser";
  		cin>>m>>n;
  		
outdegree[n]++;
adj[m][n]=true;
}

for(int i=0;i<nplayers;i++)
{
	for(int j=0;j<nplayers;j++)
	{
		if(adj[i][j])
		{
			//double x=(1.0/outdegree[j]);
			powerMatrix[i][j]=0.85*(1.0/outdegree[j])+0.15/nplayers;
		}
		if(outdegree[j]==0)
		{
			powerMatrix[i][j]=1.0/nplayers;
		}
	}
}
initialMatrix.assign(powerMatrix.begin(),powerMatrix.end());


for(int i=0;i<nplayers;i++)
{
	for(int j=0;j<nplayers;j++)
	{
		cout<<fixed<<powerMatrix[i][j];
	}
	cout<<"\n";
}


calculateRank();
for(int i=0;i<nplayers;i++)
{
cout<<"  "<<ranks[i];
}

cout<<"\n";
cout<<*max_element(ranks.begin(),ranks.end());
cout.precision(30);
for(int i=0;i<nplayers;i++)
{
ranks[i]=1.0/nplayers;
}


copy(ranks.begin(), ranks.end(), Prevranks.begin());

calculateRankPower();
cout<<endl;
cout<<"max rank"<<*max_element(Prevranks.begin(),Prevranks.end());

}
	return 0;
}