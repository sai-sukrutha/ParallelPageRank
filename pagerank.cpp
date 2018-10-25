#include<iostream>
#include<vector>
#include<array>
#include<algorithm>
using namespace std;
vector<int> game[4];
float ranks[4];
float Prevranks[4];
int outdegree[4]={0};
bool adj[4][4]={false};

bool checksame()
{
int j,count=0;
	
		for(j=0;j<4;j++)
		{

			if(abs(Prevranks[j]-ranks[j])<0000.1)
				count++;
		}
	
	if(count==4)
		return true;
	else
		return false;
}

void calculateRank()
{
	while(1)
	{


for(int i=0;i<4;i++)
{ int t=0;
	for(int j=0;j<4;j++)
	{
		if(adj[i][j])
		{
            t=1;
			ranks[i]+=0.85*ranks[j]/outdegree[j];
		}
		
     }
     if(t)
     ranks[i]=+0.15*ranks[i]/4;
}
if(checksame())
	break;
else
	copy(begin(ranks), end(ranks), begin(Prevranks));
for(int i=0;i<4;i++)
{
cout<<ranks[i];
}
cout<<"\n";
}

}
int main()
{
  
game[1].push_back(0);
outdegree[1]++;
adj[0][1]=true;
game[2].push_back(0);
outdegree[2]++;
adj[0][2]=true;
game[2].push_back(1);
outdegree[2]++;
adj[1][2]=true;
game[3].push_back(0);
outdegree[3]++;
adj[0][3]=true;
game[3].push_back(1);
outdegree[3]++;
adj[1][3]=true;
game[3].push_back(2);
outdegree[3]++;
adj[2][3]=true;
for(int i=0;i<4;i++)
{


	ranks[i]=0.25;
}
for(int i=0;i<4;i++)
{
if(game[i].size()==3)
	ranks[i]=0.0;
}
for(int i=0;i<4;i++)
{
cout<<ranks[i]<<"\n";
}

copy(begin(ranks), end(ranks), begin(Prevranks));
calculateRank();
for(int i=0;i<4;i++)
{
cout<<ranks[i];
}
cout<<"\n";
cout<<*max_element(ranks,ranks+4);


	return 0;
}