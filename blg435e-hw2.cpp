//BLG435E-AI Homework 2
//Mehmet Gencay Ertürk - 150130118
#include <bits/stdc++.h>
#include <ctime>
using namespace std;
int nodes_expand=0;
class Node{
	public:
		int red_score,blue_score;
		map<pair<int,int>,char> table;
		Node(const char* str,char isalpha);
};
int minimax(int depth,Node* cur,int alpha,int beta,int row,int col,char turn,bool pruning)
{
	if(depth==0)
	{
		ofstream file("output.txt");
		file<<cur->red_score<<endl<<cur->blue_score<<endl;
		file.close();
		return abs(cur->red_score-cur->blue_score);
	}	
	else
	{
		int value=(turn=='R')?-1000:1000;
		for(int i=0;i<2*row+1;i++)
		{
			int limit=(i%2==0)?col:col+1;
			for(int j=0;j<limit;j++)
			{
				if(cur->table[pair<int,int>(i,j)]=='O')
				{
					nodes_expand++;
					Node n=*cur;
					n.table[pair<int,int>(i,j)]=turn;
					if(i%2==0)
					{
						if(i>0)
						{
							char x=n.table[pair<int,int>(i-2,j)],y=n.table[pair<int,int>(i-1,j)],z=n.table[pair<int,int>(i-1,j+1)];
							if(x!='O' && y!='O' && z!='O')
							{
								if(turn=='R'){n.red_score++;}
								else{n.blue_score++;}
							}
						}
						if(i<2*row)
						{
							char x=n.table[pair<int,int>(i+2,j)],y=n.table[pair<int,int>(i+1,j)],z=n.table[pair<int,int>(i+1,j+1)];
							if(x!='O' && y!='O' && z!='O')
							{	
								if(turn=='R'){n.red_score++;}
								else{n.blue_score++;}
							}
						}
					}
					else
					{
						if(j>0)
						{
							char x=n.table[pair<int,int>(i-1,j-1)],y=n.table[pair<int,int>(i,j-1)],z=n.table[pair<int,int>(i+1,j-1)];
							if(x!='O' && y!='O' && z!='O')
							{
								if(turn=='R'){n.red_score++;}
								else{n.blue_score++;}
							}
						}
						if(j<col)
						{
							char x=n.table[pair<int,int>(i,j+1)],y=n.table[pair<int,int>(i-1,j)],z=n.table[pair<int,int>(i+1,j)];
							if(x!='O' && y!='O' && z!='O')
							{
								if(turn=='R'){n.red_score++;}
								else{n.blue_score++;}
							}
						}
					}
					if(turn=='R')
					{
						value=max(value,minimax(depth-1,&n,alpha,beta,row,col,'B',pruning));
						if(pruning)
						{
							alpha=max(value,alpha);
							if(alpha>=beta){break;}
						}
					}
					else
					{
						value=min(value,minimax(depth-1,&n,alpha,beta,row,col,'R',pruning));
						if(pruning)
						{
							beta=min(value,beta);
							if(alpha>=beta){break;}
						}
					}
				}
			}
		}
		return value;
	}
}
bool check_file(const char* str)
{
	ifstream file(str);
	if(!file.is_open())
	{
		cerr<<"Error! File not found.\n";
		return false;
	}
	else
	{
		if(!file.eof())
		{
			int row=file.get()-'0';
			file.get();
			if(row<1 || row>3)
			{
				cerr<<"Error! Invalid number of rows.\n";
				file.close();
				return false;
			}
			else
			{
				if(!file.eof())
				{
					int col=file.get()-'0';
					file.get();
					if(col<1 || col>3)
					{
						cerr<<"Error! Invalid number of columns.\n";
						file.close();
						return false;
					}
					else
					{
						int line=0;
						while(!file.eof())
						{
							file.get();
							line++;
						}
						if(line!=4*row*col+2*row+2*col+4)
						{
							cerr<<"Error! Invalid input.\n";
							file.close();
							return false;
						}
						else
						{
							file.close();
							return true;
						}
					}					
				}
				else
				{
					cerr<<"Error! Not enough data.\n";
					file.close();
					return false;
				}
			}
		}
		else
		{
			cerr<<"Error! Not enough data.\n";
			file.close();
			return false;
		}
	}	
}
Node::Node(const char* str,char isalpha)
{
	if(!check_file(str)){exit(1);}
	ifstream file(str);
	int row=file.get()-'0';
	file.get();
	int col=file.get()-'0';
	file.get();
	int closed=0;
	for(int i=0;i<row+1;i++)
	{
		for(int j=0;j<col;j++)
		{
			pair<int,int> p=make_pair(2*i,j);
			char x=file.get();
			if(x=='0'){table[p]='O';}
			else if(x=='1')
			{
				table[p]='C';
				closed++;
			}
			else
			{
				cerr<<"Error! Invalid row value.\n";
				exit(1);
			}
			file.get();
		}	
	}
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col+1;j++)
		{
			pair<int,int> p=make_pair(2*i+1,j);
			char x=file.get();
			if(x=='0'){table[p]='O';}
			else if(x=='1')
			{
				table[p]='C';
				closed++;
			}
			else
			{
				cerr<<"Error! Invalid column value.\n";
				exit(1);
			}			
			file.get();
		}	
	}
	if(closed==2*row*col+row+col)
	{
		cerr<<"Error! At least one edge must be open.\n";
		exit(1);
	}
	red_score=file.get()-'0';
	file.get();
	if(red_score<0 || red_score>9)
	{
		cerr<<"Error! Invalid red player score.\n";
		exit(1);
	}
	blue_score=file.get()-'0';
	if(blue_score<0 || blue_score>9)
	{
		cerr<<"Error! Invalid blue player score.\n";
		exit(1);
	}	
	file.close();
	if(isalpha=='Y'){int result=minimax(2*row*col+row+col-closed,this,-1000,1000,row,col,'R',true);}
	else if(isalpha=='N'){int result=minimax(2*row*col+row+col-closed,this,-1000,1000,row,col,'R',false);}
}
int main(int argc,char** argv)
{
	if(argc==3)
	{
		clock_t start,end;
		if(strcmp(argv[2],"Y")==0)
		{
			start=clock();
			Node n(argv[1],'Y');
			end=clock();
			cout<<"Number of generated nodes:"<<nodes_expand<<endl<<"Total runtime (ms):"<<(double)(1000*((end-start)/CLOCKS_PER_SEC));
		}
		else if(strcmp(argv[2],"N")==0)
		{
			start=clock();
			Node n(argv[1],'N');
			end=clock();
			cout<<"Number of generated nodes:"<<nodes_expand<<endl<<"Total runtime (ms):"<<(double)(1000*((end-start)/CLOCKS_PER_SEC));
		}		
		else{cerr<<"Error! Argument 2 is invalid.\n";}
	}
	else{cerr<<"Error! Not enough number of arguments.\n";}
	return 0;
}
