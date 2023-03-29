#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp(vector<double> x, vector<double> y) {
	return x[3] < y[3];
}

int main() {
	//������ ����
	vector<double> bream_length = { 
		25.4, 26.3, 26.5, 29.0, 29.0, 29.7, 29.7, 30.0, 30.0, 30.7, 31.0, 31.0,
		31.5, 32.0, 32.0, 32.0, 33.0, 33.0, 33.5, 33.5, 34.0, 34.0, 34.5, 35.0,
		35.0, 35.0, 35.0, 36.0, 36.0, 37.0, 38.5, 38.5, 39.5, 41.0, 41.0 
	};
	vector<double> bream_weight = {
		242.0, 290.0, 340.0, 363.0, 430.0, 450.0, 500.0, 390.0, 450.0, 500.0, 475.0, 500.0,
		500.0, 340.0, 600.0, 600.0, 700.0, 700.0, 610.0, 650.0, 575.0, 685.0, 620.0, 680.0,
		700.0, 725.0, 720.0, 714.0, 850.0, 1000.0, 920.0, 955.0, 925.0, 975.0, 950.0 
	};
	
	vector<double> smelt_length = {
		9.8, 10.5, 10.6, 11.0, 11.2, 11.3, 11.8, 11.8, 12.0, 12.2, 12.4, 13.0, 14.3, 15.0
	};
	vector<double> smelt_weight = {
		6.7, 7.5, 7.0, 9.7, 9.8, 8.7, 10.0, 9.9, 9.8, 12.2, 13.4, 12.2, 19.7, 19.9
	};

	vector<double> length, weight;

	//bream�� smelt �����͸� ������ �̾� ���̴� �ڵ�
	length.insert(length.end(), bream_length.begin(), bream_length.end());
	length.insert(length.end(), smelt_length.begin(), smelt_length.end());

	weight.insert(weight.end(), bream_weight.begin(), bream_weight.end());
	weight.insert(weight.end(), smelt_weight.begin(), smelt_weight.end());

	//length�� weight �����͸� �ϳ��� �ڷ����� ���� �ڴ°�
	vector<vector<double>> fish_data;
	
	for (int i = 0; i < length.size(); i++)
		fish_data.push_back({ length[i], weight[i] });

	//Ȯ�ο�
	if(0) 
		for (int i = 0; i < length.size(); i++)
			cout << fish_data[i][0] << "\t" << fish_data[i][1] << endl;

	//���� ������ ����
	vector<int> fish_target;
	for (int i = 0; i < bream_length.size(); i++) fish_target.push_back(1);
	for (int i = 0; i < smelt_length.size(); i++) fish_target.push_back(0);

	//Ȯ�ο�
	if (0)
		for (int i = 0; i < fish_target.size(); i++)
			cout << fish_target[i] << " ";

	//kn �� ����(�׳� �����Ϳ� ���䵥���� ���̴°���)
	int k = 5;
	vector<vector<double>> kn;
	for (int i = 0; i < fish_data.size(); i++)
		kn.push_back({ fish_data[i][0], fish_data[i][1], double(fish_target[i]) });

	//Ȯ�ο�
	if (0)
		for (int i = 0; i < length.size(); i++)
			cout << kn[i][0] << "\t" << kn[i][1] << "\t" << kn[i][2] << endl;

	//����
	//30, 600����
	double p1 = 30, p2 = 600;
	
	//��Ŭ���� �Ÿ� ������ �̿��ؼ� �Ÿ��� ���ϰ� kn���Ϳ� �����ϰ� �Ÿ� �������� �����Ͽ� ���� k���� �̾Ƽ� ���� �� ū�� ���Ѵ�.
	for (int i = 0; i < kn.size(); i++)
		kn[i].push_back(sqrt(pow((kn[i][0] - p1), 2) + pow((kn[i][1] - p2),2)));
	
	sort(kn.begin(), kn.end(), cmp);

	//Ȯ�ο�
	if (true)
		for (int i = 0; i < length.size(); i++)
			cout << kn[i][0] << "\t" << kn[i][1] << "\t" << kn[i][2] << "\t" << kn[i][3] << endl;

	int bream_c = 0, smelt_c = 0;
	for (int i = 0; i < k; i++) {
		(kn[i][2] == 1 ? bream_c++ : smelt_c++);
	}

	cout << (bream_c > smelt_c ? "�̰� bream��" : "�̰� smelt��") << endl;
}