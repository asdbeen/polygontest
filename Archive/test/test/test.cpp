
#include <vector>
#include <iostream>

#include <algorithm>
#include <string>
#include <tuple>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>




using namespace std;




int main(void)
{
	double num;
	double down = 0.0, top = 2.4;
	double a[100];
	int i;

	srand((unsigned int)time(NULL));
	for (i = 0;i < 100;i++)
	{
		num = (double)rand() / RAND_MAX;

		a[i] = num;
		printf("a[%d]=%f\n", i, a[i]);
	}




	/*random_device rd;
	mt19937_64 eng(rd());
	uniform_int_distribution<unsigned long long> distr(1, 6);

	for (int n = 0;n < 40;n++)
	{
		cout << distr(eng) ;
	}*/




	/*for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			cout << edges[i][j] << " ";
		}
		
	}

	cout << "\n";
	sort(edges.begin(), edges.end());

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << edges[i][j] << " ";
		}

	}*/


	/*vector<int> A = { 1,2,3 };
	vector<float> B = { 0.1,2.2,3.1 };

	tuple <vector<int>, vector<float>> C = { A, B };


	vector<int> newA = get<0>(C);


	vector<float> newB = get<1>(C);


	 C = { A, B };*/
	

	/*for (int i = 0; i < 5; i++)
	{
		
			cout << A[i] << " ";
		

	}*/

	/*vector<int> A{ 0,1,2,3,4 };
	vector<int> B{ 100,101,102,103,104 };

	
		for (int num :  (A,B))
		{
			cout << num << " ";
		}
	*/
	/*std::tuple<int,int> B{1,2};
	std::map <tuple<int, int>, int> A;

	int pointnum_1 = 1;
	int pointnum_2 = 2;
	int 	next_pointnum = 6;

	


	A[{pointnum_1, pointnum_2}] = next_pointnum;

	cout << A[{pointnum_1, pointnum_2}] << " ";*/
    






	return 0;
}



