#include<iostream>
#include<vector>
#include<array>
#include<algorithm>
#include <sstream>
#include <string>
#include <omp.h>
using namespace std;

vector<double> ranks;
vector<double> Prevranks;
vector<double> powerranks;
vector<double> powerPrevranks;
vector<int> outdegree;
vector<vector<bool>> adj;

vector<vector<double>>powerMatrix;
vector<vector<double>> initialMatrix;
vector<double>rankinit;

//Sorting for Printing


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

bool checksamepower()
{
    int j,count=0;
	
	for(j=0;j<powerranks.size();j++)
	{

		if(abs(powerPrevranks[j]-powerranks[j])<0.0001)
			count++;
	}

	if(count==powerranks.size())
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
	}
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
	int game;
	cout<<"Press 1 for Team Sport, 2 for Individual sport  ";
    cin>>game;
    cout.precision(5);
    int count=0,temp;
    int nplayers;
    if(game==1)
	    cout<<"Enter Number of Teams : ";
    else
	 	cout<<"Enter Number of participants : ";
	cin>>nplayers;
	for(int i=0;i<nplayers;i++)
	{
		ranks.push_back(1.0/nplayers);
		powerranks.push_back(1.0/nplayers);
	}
	Prevranks.resize(nplayers);
	powerPrevranks.resize(nplayers);
	copy(ranks.begin(), ranks.end(), Prevranks.begin());
	copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());

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
	if(game==1)
		cout<<"Enter Number of Tournaments : ";
	else
		cout<<"Enter Number of Races : ";
	cin>>temp;
	while(count++ <temp)
	{     
	  	if(game==1)
	  	{
		  	cout<<"Enter Number of matches for Tournament "+to_string(count)<<endl;
		  	cin>>matches;
		  	while(matches-- >0)
		  	{
		        cout<<"Enter winner and loser ";
		  		cin>>m>>n;
		  		if(m>nplayers-1||n>nplayers-1)
		  		{
		  			cout<<"Enter valid Team Id"<<endl;
		  			matches++;
		  			continue;
		  		}	  		
				outdegree[n]++;
				adj[m][n]=true;
		    }
	    }
	    else
	    {
			cout<<"Enter Race Completion order for Race "+to_string(count)<<endl;
			vector<int> r;
			int x;
			while( cin>>x )
			{
				int f=0;
				if(x>nplayers-1)
				{
					cout<<"Enter valid player Id"<<endl;
					f=1;
				}
				if(f==0)
				    r.push_back(x);
				if(cin.get()=='\n')
					break;
			}

			int i=0,j=0;
			for(i=0;i<r.size()-1;i++)
			{
				for(j=i+1;j<r.size();j++)
				{
					adj[r[i]][r[j]]=true;
					outdegree[r[j]]++;
				}
			}
	    }

		#pragma omp parallel for
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

		cout.precision(5);
		calculateRank();
		adj.clear();
		outdegree.clear();
vector <pair<int,double>> rank_vect;
vector <pair<int,double>> powerrank_vect;
		//Sorting the ranks for printing
		for(int i=0;i<nplayers;i++)
            rank_vect.push_back(make_pair(i,ranks[i]));
        sort(rank_vect.begin(),rank_vect.end(),sortbyrank);
        
		cout<<"Vanilla Page Ranks "<<endl;
		cout<<"--------------------------------"<<endl;
		cout<<"Rank\t Id\tScore                  "<<endl;
		cout<<"--------------------------------"<<endl;
		for(int i=0;i<rank_vect.size();i++)
		{
			cout<<i+1<<"        "<<rank_vect[i].first<<"   "<<rank_vect[i].second<<endl;
		}
		cout<<"\n";
		rank_vect.clear();
		//cout<<"Max-Rank Vanilla  "<<*max_element(ranks.begin(),ranks.end())<<endl;
        
		cout.precision(30);
        
		calculateRankPower();

		//Sorting the ranks for printing
		for(int i=0;i<nplayers;i++)
            powerrank_vect.push_back(make_pair(i,powerranks[i]));
        sort(powerrank_vect.begin(),powerrank_vect.end(),sortbyrank);

		cout<<"Power matrix Page Ranks  "<<endl;
		cout<<"--------------------------------"<<endl;
		cout<<"Rank\t Id\tScore                  "<<endl;
		cout<<"--------------------------------"<<endl;
		
		for(int i=0;i<powerrank_vect.size();i++)
		{
			cout<<i+1<<"        "<<powerrank_vect[i].first<<"   "<<powerrank_vect[i].second<<endl;
		}		
		cout<<endl;
        powerrank_vect.clear();
		//cout<<"Max-Rank Power matrix  "<<*max_element(powerranks.begin(),powerranks.end())<<endl;
	}

	return 0;
}