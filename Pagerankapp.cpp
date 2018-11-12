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
vector<long long> outdegree;
vector<vector<bool>> adj;

vector<vector<double>>powerMatrix;
long long nodes;


//Sorting for Printing


bool checksame()
{
    long long j,count=0;

	for(j=0;j<nodes;j++)
	{

		if(abs(Prevranks[j]-ranks[j])<0.0001)
			count++;
	}
	
	if(count==nodes)
		return true;
	else
		return false;
}

bool checksamepower()
{
    long long count=0;
	#pragma omp parallel for shared(count) num_threads(10)
	for(long long j=0;j<nodes;j++)
	{

		if(abs(powerPrevranks[j]-powerranks[j])<0.0001)
			count++;
	}

	if(count==nodes)
		return true;
	else
		return false;
}

void calculateRank()
{
	long long count=1;
	while(1)
	{
		count++;
		if(count != 2)
			fill(ranks.begin(),ranks.end(),0);

		for(long long i=0;i<ranks.size();i++)
		{ 
			for(long long j=0;j<ranks.size();j++)
			{
				if(adj[i][j])
				{       
					ranks[i]+=0.85*Prevranks[j]/outdegree[j];
				}	
		    }
		    ranks[i]+=0.15/ranks.size();
		}
		if(checksame()||count==15){
			copy(ranks.begin(), ranks.end(), Prevranks.begin());
			break;
		}
		else
			copy(ranks.begin(), ranks.end(), Prevranks.begin());
	}
}








void mulwithrank(vector<vector<double>> &adj,vector<double> &v)
{
	
vector<double>mul(nodes,0);
long long i,j,k; 
#pragma omp parallel for shared(mul) private(i, j, k) num_threads(5)
for ( i = 0; i< nodes; i++) 
    { 
        for ( j = 0; j < 1; j++) 
        { 
            mul[i]=0;
            for ( k = 0; k < nodes; k++) 
            	mul[i] += adj[i][k] * v[k];

             
            
                 
        } 
    } 
  copy(mul.begin(), mul.end(), powerranks.begin());
 
 


}


void calculateRankPower()
{
	
	
	for(long long i=1;i<=15;i++)
    {    	
       
        	
		mulwithrank(powerMatrix,powerPrevranks);
		if(checksamepower())
		{
			copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());	
			break;
		}
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
    cout.precision(30);
    long long count=0,temp;
    
    if(game==1)
	    cout<<"Enter Number of Teams : ";
    else
	 	cout<<"Enter Number of participants : ";
	cin>>nodes;
	if(game==1)
		cout<<"Team Ids are from 0 to "<<nodes-1<<endl;
	else
		cout<<"player Ids are from 0 to "<<nodes-1<<endl;
	for(int i=0;i<nodes;i++)
	{
		ranks.push_back(1.0/nodes);
		powerranks.push_back(1.0/nodes);
	}
	Prevranks.resize(nodes);
	powerPrevranks.resize(nodes);
	copy(ranks.begin(), ranks.end(), Prevranks.begin());
	copy(powerranks.begin(), powerranks.end(), powerPrevranks.begin());

	long long m,n,matches;
	outdegree.resize(nodes);

	adj.resize(nodes);
	for (int i = 0; i < nodes; ++i)
	    adj[i].resize(nodes);
	powerMatrix.resize(nodes);
	for (int i = 0; i < nodes; ++i)
	    powerMatrix[i].resize(nodes);
	
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
		  		if(m>nodes-1||n>nodes-1)
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
				if(x>nodes-1)
				{
					cout<<"Enter valid player Id"<<endl;
					f=1;
				}
				if(f==0)
				    r.push_back(x);
				if(cin.get()=='\n')
					break;
			}

			long long i=0,j=0;
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

		cout.precision(30);
		calculateRank();
		//adj.clear();
		//outdegree.clear();
vector <pair<long long,double>> rank_vect;
vector <pair<long long,double>> powerrank_vect;
		//Sorting the ranks for printing
		for(long long i=0;i<nodes;i++)
            rank_vect.push_back(make_pair(i,ranks[i]));
        sort(rank_vect.begin(),rank_vect.end(),sortbyrank);
        
        /*
		cout<<"Vanilla Page Ranks "<<endl;
		cout<<"--------------------------------"<<endl;
		cout<<"Rank\t Id\tScore                  "<<endl;
		cout<<"--------------------------------"<<endl;
		for(long long i=0;i<nodes;i++)
		{
			cout<<i+1<<"        "<<rank_vect[i].first<<"   "<<rank_vect[i].second<<endl;
		}
		cout<<"\n";*/
		rank_vect.clear();
		//cout<<"Max-Rank Vanilla  "<<*max_element(ranks.begin(),ranks.end())<<endl;
        
		cout.precision(30);
        
		calculateRankPower();

		//Sorting the ranks for printing
		for(int i=0;i<nodes;i++)
            powerrank_vect.push_back(make_pair(i,powerranks[i]));
        sort(powerrank_vect.begin(),powerrank_vect.end(),sortbyrank);

		cout<<"Power matrix Page Ranks  "<<endl;
		cout<<"--------------------------------"<<endl;
		cout<<"Rank\t Id\tScore                  "<<endl;
		cout<<"--------------------------------"<<endl;
		
		for(int i=0;i<nodes;i++)
		{
			cout<<i+1<<"        "<<powerrank_vect[i].first<<"   "<<powerrank_vect[i].second<<endl;
		}		
		cout<<endl;
        powerrank_vect.clear();
		//cout<<"Max-Rank Power matrix  "<<*max_element(powerranks.begin(),powerranks.end())<<endl;
	}

	return 0;
}