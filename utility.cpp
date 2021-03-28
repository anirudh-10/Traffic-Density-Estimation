#include<bits/stdc++.h>
using namespace std;
int i = 0;
vector<double> static_set,dynamic_set;
double error_value(double x,bool which)
{
	if(which)
	{
		return abs(x-dynamic_set[i]);
	}
	return abs(x-static_set[i]);
}
int main(int argc, char** argv)
{
	double error_static=0.0000,error_dynamic = 0.0000;
	argc--,argv++;
	ifstream fin;
	fin.open(argv[0]);
	string line_of_file;
	bool temp = false;
	while(getline(fin,line_of_file))
	{
		if(!temp)
		{
			temp=true;
			continue;
		}
		vector <string> tokens;
      	stringstream check1(line_of_file);
      	string temp;
      	while(getline(check1, temp, ','))
    	{
        	tokens.push_back(temp);
    	}
    	double static_queue,dynamic_queue;
    	static_queue = stoi(tokens[1]);
    	dynamic_queue = stoi(tokens[2]);
    	static_set.push_back(static_queue);
    	dynamic_set.push_back(dynamic_queue);
	}
	fin.open(argv[1]);
	temp=false;
	while(getline(fin,line_of_file))
	{
		if(!temp)
		{
			temp=true;
			continue;
		}
		vector <string> tokens;
      	stringstream check1(line_of_file);
      	string temp;
      	while(getline(check1, temp, ','))
    	{
        	tokens.push_back(temp);
    	}
    	double static_queue,dynamic_queue;
    	static_queue = stoi(tokens[1]);
    	dynamic_queue = stoi(tokens[2]);
    	error_static+=error_value(static_queue,0);
    	error_dynamic+=error_value(dynamic_queue,1);
    	i++;
	}
	cout<<"static queue error is : "<<error_static<<endl;
	cout<<"dynamic queue error is : "<<error_dynamic<<endl;

}