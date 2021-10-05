#include<bits/stdc++.h>
#define rep(n) for(int i=0;i<n;i++)
using namespace std;
int bucket_size;

vector<vector<pair<int,int>>> buckets;
// vector<pair<int,int>> allpoints;
vector<int> cell_to_bucket;      //because multiple cells can be pointing to the same bucket if not empty
int bucket_count=0;
int SIZE=400;
int toggle=0;
vector<double> xl={0,(double)SIZE}; //line cutts at x-axis by logical lines (used to identify cells by giving line equations)
vector<double> yl={0,(double)SIZE}; // line cuts at y-axis by logical lines (used to identify cells by giveing line equations)
vector<int> parent_cell_to_bucket;
vector<double> p_xl; //just tile xl,yl, p_xl & p_yl it will keep previous xl & yl data so that we can prevent the logical splitting 
							//if the data item is already in different cell by shareing the same bucked (parent or previous bucket) with other cell.
vector<double> p_yl;
struct point //point structure
{
	int id;
	int x;
	int y;
};
int y_sort(pair<int,int> a,pair<int,int> b) //will sort w.r.t y-coordinate
	{
		return a.second<b.second;
	}
void grid_details() //It will return whole grid details (range of cell coordinates)
	{
		int c_no=0;
		for(int i=0;i<xl.size()-1;i++)
		{
			for(int j=0;j<yl.size()-1;j++)
			{
				cout<<"\nCell:"<<c_no++;
				cout<<":("<<xl[i]<<","<<yl[j]<<") to ("<<xl[i+1]<<","<<yl[j+1]<<")";	
			}
		}
	}
int point_to_cell_no(int x,int y) //it will return the cell number on the basis of division line drawn
	{	
		int c_no=0;
		for(int i=0;i<xl.size()-1;i++)
		{
			for(int j=0;j<yl.size()-1;j++)
			{
				if (x>=xl[i] && x<=xl[i+1] and y>=yl[j] && y<=yl[j+1])
					{
					return c_no;
					}
				c_no++;
			}
		}
		return c_no;
	}
int p_point_to_cell_no(double x,double y) //It will return the parent cell no. of any points
	{	
		int c_no=0;
		for(int i=0;i<p_xl.size()-1;i++)
		{
			for(int j=0;j<p_yl.size()-1;j++)
			{
				if (x>=p_xl[i] && x<=p_xl[i+1] and y>=p_yl[j] && y<=p_yl[j+1])
					{
					return c_no;
					}
				c_no++;
			}
		}
		return c_no;
	}
int give_parent_bucket(int c_cell) //it will return the bucket no of the particular cell
	{
		double x,y; //cordinates of present cell
		int flag=0;
		int n=yl.size()-1;
		x=xl[(int)(c_cell/n)+1];  //It will give x coordinate of the pareent cell
		y=yl[(int)(c_cell%n)+1]; //It will give y coordinate of the pareent cell
		cout<<"\n #### PARENT UPDATE ####";
		cout<<"\nParent pxl:";
		for(auto x:p_xl)
			cout<<x<<"\t";

		cout<<"\nParent pyl:";
		for(auto x:p_yl)
			cout<<x<<"\t";
		cout<<"\nXL:";
		for(auto x:xl)
			cout<<x<<"\t";

		cout<<"\nyl:";
		for(auto x:yl)
			cout<<x<<"\t";

		int p_cell=p_point_to_cell_no(x,y);
		cout<<"\nEntring cell no:"<<c_cell;
		cout<<"\n Pridicted x and y:"<<x<<","<<y<<" with cell no:"<<p_cell;

		return parent_cell_to_bucket[p_cell];

	}
void update_cell_to_bucket() //It will update teh cell_to_bucket mapping variable
	{
	cell_to_bucket.clear();
	cell_to_bucket.resize((xl.size()-1)*(yl.size()-1));
	fill(cell_to_bucket.begin(),cell_to_bucket.end(),-1); //Filling with -1
	
	int j=0;
	
	cout<<"\n%%%%%%%%UPDATE EXECUTED:::";
	for(auto buc:buckets)
		{	
		cout<<"\nBucket no:"<<j<<":";
			for(auto z:buc)
				{
					cell_to_bucket[point_to_cell_no(z.first,z.second)]=j;
					cout<<z.first<<","<<z.second<<"\t";
					
				}
			j++;
		}
	rep(cell_to_bucket.size())
		{
			if(cell_to_bucket[i]==-1)
				{
					cell_to_bucket[i]=give_parent_bucket(i);
				}
		}
	cout<<"\n###Cell_to_bucket:###";
	rep(cell_to_bucket.size())
		{
			cout<<"\nCell:"<<i<<"\t Bucket:"<<cell_to_bucket[i];
		}
	cout<<"\n-----$$$$$$$---------";
	}	
void create_coordinates(int n) //It will create n coordinates
	{	
		int range=SIZE;
		ofstream fout;				
		fout.open("input.txt",ios::out);	//FILE OPENING
		for(int i=1;i<=n;i++)
		{
			fout<<i<<" "<<rand()%range+1<<" "<<rand()%range+1<<endl; //i x_cord Y-cord
		}
		fout.close();
	}
int logical_split_required(vector<pair<int,int>> b) //It will tell is logical spliting is required or datapoints are already split
	{
		if(b.size()<bucket_size)
			return 0;
		int c=point_to_cell_no(b[0].first,b[0].second);
		for(auto item:b)
			if(point_to_cell_no(item.first,item.second)!=c)
				return 0;
		return 1;
	}
void put_in_grid(point obj) //It will put the point object (have x & y coordinate) to the respective positing in the grid and make respective
							// logical or pyhsical split with respect to the data point arrived.
	{
		cout<<"\n#############################(coordinate arrived):"<<obj.x<<","<<obj.y;
		int cell_index=point_to_cell_no(obj.x,obj.y);
		int bucket_index=cell_to_bucket[cell_index];

		vector<pair<int,int>> &b=buckets[bucket_index];
		if (b.size()<bucket_size)
			b.push_back(make_pair(obj.x,obj.y));
		else
		{
			int all_identical=1;
			int identical_val;
			vector<pair<int,int>> new_bucket1,new_bucket2;
			b.push_back(make_pair(obj.x,obj.y));
			if (toggle==0)  //MAKING THE ALGORITHM TO DO LOGICAL SPLITTING ALONG X & Y AXIS CONSECUTIVELY 
			{
				//THIS SECTION WILL DECIDE IF SPLIT IS REQUIRED IN X-AXIS 

				sort(b.begin(),b.end());  //sorthing with respect to x-axis coordinates
				if(logical_split_required(b)==1)  //It logincal splitting is required or not 
				{
					identical_val=b[0].first;
					cout<<"\n]]]]]]]]   Logical spliting required --x-axis [[[[[[[[[[[[[";
					double new_cut=0;
					
					for(auto ii:b)
						new_cut+=ii.first;
					new_cut/=b.size();
					
					for(auto ii:b)
					{
						if(ii.first!=identical_val)
							{
								all_identical=0; //checking if all the element are same to handle the dublicates
								break;
							}
					}
					cout<<endl;
					for(auto zzz:b)
						cout<<zzz.first<<","<<zzz.second<<"\t";
					
					cout<<"\n--------New cut:"<<new_cut;
					if(all_identical)
						cout<<"\nSINCE(X) DUBLICATE ELEMENTS ARE MORE THAN THE SIZE OF THE BUCKET, WE WILL MAKE NEW BUCKET AND NOT DOING LOGICAL SPLIT TO SAVE DUPLICATES";
					
					p_yl=yl;
					p_xl=xl;
					parent_cell_to_bucket=cell_to_bucket;

					auto found=find(xl.begin(),xl.end(),new_cut);
					if(found==xl.end() && all_identical==0)
						xl.push_back(new_cut);
					
					sort(xl.begin(),xl.end());
					toggle=(toggle+1)%2;
					//update_cell_to_bucket();
				}			
			}
			else  //For y-axis logical split
			{
				//THIS SECTION WILL DECIDE IF SPLIT IS REQUIRED IN X-AXIS 
				identical_val=b[0].second;
				sort(b.begin(),b.end(),y_sort);
				
				if(logical_split_required(b)==1)
				{
					cout<<"\n]]]]]]]]   Logical spliting required --y-axis [[[[[[[[[[[[[";
					double new_cut=0;
					for(auto ii:b)
						new_cut+=ii.second;
					new_cut/=b.size();

					for(auto ii:b)
					{
						if(ii.second!=identical_val)
							{
								all_identical=0;
								break;
							}
					}
					cout<<endl;
					for(auto zzz:b)
						cout<<zzz.first<<","<<zzz.second<<"\t";
					
					cout<<"\n--------New cut:"<<new_cut;

					if(all_identical)
						cout<<"\nSINCE (Y) DUBLICATE ELEMENTS ARE MORE THAN THE SIZE OF THE BUCKET, WE WILL MAKE NEW BUCKET AND NOT DOING LOGICAL SPLIT TO SAVE DUPLICATES";
					p_yl=yl;
					p_xl=xl;
					parent_cell_to_bucket=cell_to_bucket;

					auto found=find(yl.begin(),yl.end(),new_cut);
					if(found==yl.end() && all_identical==0)
						yl.push_back(new_cut);

					sort(yl.begin(),yl.end());
					toggle=(toggle+1)%2;
				}
			}
		cout<<"\n]]]]]]] Physical Splitting [[[[[[[[";
		int cut_cell=point_to_cell_no(b[0].first,b[0].second);
				for(auto item:b)
					{
						
						int temp=point_to_cell_no(item.first,item.second);
						
						if(temp==cut_cell)
							new_bucket1.push_back(make_pair(item.first,item.second));
						else
							new_bucket2.push_back(make_pair(item.first,item.second));
					}
					
				b=new_bucket1;
				if(new_bucket2.size()>0)
				{
					buckets.push_back(new_bucket2);
				}
				else
				{
					new_bucket2.clear();
					new_bucket1.clear();
					int k=0;
					for(auto item:b)
					{
						// cout<<"\n9skjfjklskjfsdfjflslkfjkskfjlklsfkljskjflsjklfjlk\t"<<item.first<<"\t";
						if(k<bucket_size)
							new_bucket2.push_back(item);
						else
							new_bucket1.push_back(item);
						k++;
					}
					b=new_bucket2;
					buckets.push_back(new_bucket1);
				}

			update_cell_to_bucket();
		}
		int j=0;
		cout<<"\nX-intercept:";
		for(auto x:xl)
			cout<<x<<"\t";
		cout<<"\nY-intercept:";
		for(auto x:yl)
			cout<<x<<"\t";
		cout<<"Size:"<<buckets.size();
		for(auto buc:buckets)
		{	cout<<"\nBucket: no:"<<j<<endl;
			j++;
			for(auto i:buc)
				cout<<i.first<<" "<<i.second<<endl;
		}	
	}	
void simulate_buckets()
	{
		ifstream fin;
		fin.open("input.txt",ios::in);
		point obj;
		while(fin>>obj.id>>obj.x>>obj.y)
		{
			put_in_grid(obj);                  
		}
		fin.close();
	}
int main()
	{
	int n;
	cout<<"\nEnter the bucket size:";
	cin>>bucket_size;
	cout<<"\nEnter the number of elements you wan't to ceate:";
	cin>>n;
	create_coordinates(n); //Generating the points
	// bucket_size=3;
	vector<pair<int,int>> initial_bucket;
	buckets.push_back(initial_bucket);
	cell_to_bucket.push_back(0);  //by default, first cell will be bucket no. 0
	
	int j=0;
	simulate_buckets();
	cout<<"\n##############  FINAL OUTPUT  ######";
	cout<<endl;
	cout<<"\nLine parallel to Y-axis in grid ( X-axis intercepts):";
	for(auto x:xl)
		cout<<x<<"\t";
	cout<<"\nLine parallel to X-axis in grid (Y-axis intercepts):";
	for(auto x:yl)
		cout<<x<<"\t";
	cout<<"\nSize:"<<buckets.size();
	ofstream fout;
	for(auto buc:buckets)
		{	cout<<"\nBucket: no:"<<j<<endl;
			string file="Bucket_"+to_string(j)+".txt";
			j++;
			fout.open(file,ios::out);	//FILE OPENING
			for(auto i:buc)
			{
				cout<<i.first<<" "<<i.second<<endl;
				fout<<i.first<<","<<i.second<<endl;  //updating the bucket file
			}
			fout.close();
		}
	int i=0;
	cout<<"\nMapping.....";
	for(auto x:cell_to_bucket)
	{
		cout<<"\nCell:"<<i<<" Mapped to Bucket:"<<x;
		i++;	
	}
	cout<<"\n\n-----FINAL GRID DETAIL------"<<endl;
	grid_details();
	return 0;
	}