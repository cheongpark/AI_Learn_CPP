//������ weight, height, size �����
//�� 330���� �� ���� �ִµ� ������

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <chrono>

#include <random>
#include <set>

#define CalAccuracy true
#define K 5
#define TC 20 //�׽�Ʈ Ƚ��

#define WEIGHT 54
#define HEIGHT 176

using namespace std;

class Point {
public:
    Point(double weight, double height, string size);
    void printing();
    string to_string();

    double weight;
    double height;
    string size;
};

Point::Point(double weight, double height, string size) {
    this->weight = weight;
    this->height = height;
    this->size = size;
}

void Point::printing() {
    stringstream str;
    str << "������ : " << weight << ", Ű : " << height << ", �� ������ : " << size;
    string s = str.str();
    puts(s.c_str());
}

string Point::to_string() {
    stringstream str;
    str << "������ : " << weight << ", Ű : " << height << ", �� ������ : " << size;
    return str.str();
}

class Distance {
public:
    Distance(double distance, string size);

    double distance;
    string size;
};

Distance::Distance(double distance, string size) {
    this->distance = distance;
    this->size = size;
}

bool cmp(Distance x, Distance y) {
    return x.distance < y.distance;
}

vector<Point> csv_2_points(ifstream& file) {
    vector<Point> points;

    string line;
    while (getline(file, line)) {
        vector<string> row;
        size_t pos = 0;
        string token = " ";
        while ((pos = line.find(",")) != string::npos) {
            token = line.substr(0, pos);
            row.push_back(token);
            line.erase(0, pos + 1);
        }
        row.push_back(line);

        //cout << stod(row[0]) << "\t" << stod(row[1]) << "\t" << row[2] << endl;

        points.push_back({stod(row[0]), stod(row[1]), row[2]});
    }

    return points;
}

vector<Distance> cal_distance(vector<Point> points, double weight, double height) {
    vector<Distance> distances;

    stringstream pdfile;
    pdfile << weight << ", " << height << " Predict Distance.txt";
    ofstream predictDistance(pdfile.str());

    for(Point point : points) {
        Distance distance(sqrt(pow(point.weight - weight, 2) + pow(point.height - height, 2)), point.size);
        predictDistance << "�Ÿ� : " << distance.distance << ", ������ : " << distance.size << endl;
        distances.push_back(distance);
    }

    return distances;
}

vector<string> find_size(vector<Point> points) {
    vector<string> size_kind(points.size());
    transform(points.begin(), points.end(), size_kind.begin(), [](Point& size) {
        return size.size;
    });

    sort(size_kind.begin(), size_kind.end());

    auto last = unique(size_kind.begin(), size_kind.end());
    size_kind.erase(last, size_kind.end());

    return size_kind;
}

vector<int> counting_size(int k, vector<Distance> distances, vector<string> size_kind) {
    vector<int> size_count(size_kind.size());
    
    sort(distances.begin(), distances.end(), cmp);

    for(int i = 0; i < k; i++) {
        //cout << i + 1 << " " << distances[i].size << endl;
        
        for(int j = 0; j < size_kind.size(); j++) {
            if(distances[i].size == size_kind[j]) {
                size_count[j]++;
                break;
            }
        }
    }

    return size_count;
}

double model_score(int k, vector<Point> trains, vector<Point> tests) {
    int correct = 0, incorrect = 0;

    //trains�� tests �� �� �ϳ��� ������ ������ �� �� ����
    vector<string> size_kind = find_size(trains);

    for(auto test : tests) {
        vector<Distance> distances = cal_distance(trains, test.weight, test.height);

        vector<int> size_count = counting_size(k, distances, size_kind);
        int max_size_index = max_element(size_count.begin(), size_count.end()) - size_count.begin();

        cout << "���� : " << size_kind[max_size_index] << ", ���� : " << test.size << endl;
        size_kind[max_size_index] == test.size ? correct++ : incorrect++;
    }
    
    cout << "�׽�Ʈ Ƚ�� : " << tests.size() << endl;
    cout << "��Ȯ�� : " << static_cast<double>(correct) / tests.size() << ", ���� ���� : " << correct << ", Ʋ�� ���� : " << incorrect << endl;

    return static_cast<double>(correct) / tests.size();
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    ifstream csvFile("final_test.csv");
    ofstream dataDes("DataDes.txt");
    int k = K;
    int test_count = TC;
    double accuracy = 0;

    vector<Point> points = csv_2_points(csvFile);

    auto astart = chrono::high_resolution_clock::now();
    //ó���� ���� �����͸� �׽�Ʈ �����Ϳ��� ���� �־ �ߴµ� 1�� ����ϴµ� �ʹ� ���� �ɷ��� �׽�Ʈ �����͸� �и�
    if(CalAccuracy) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, points.size());

        std::set<int> generated;

        while (generated.size() < TC) {
            int num = dis(gen);
            if (generated.find(num) == generated.end()) {
                generated.insert(num);
            }
        }

        vector<Point> test_points;
        for(int index : generated)
            test_points.push_back(points[index]);

        accuracy = model_score(k, points, test_points);
    }
    auto aend = chrono::high_resolution_clock::now();
    
    //������ ���� �˾Ƴ���
    vector<string> size_kind = find_size(points);

    for(Point point : points)
        dataDes << point.to_string() << endl;

    int weight = WEIGHT, height = HEIGHT;
    //cin >> weight >> height;

    vector<Distance> distances = cal_distance(points, weight, height);
    
    vector<int> size_count = counting_size(k, distances, size_kind);

    for(int i = 0; i < size_count.size(); i++) {
        cout << "���� : " << size_count[i] << ", ������ : " << size_kind[i] << endl; 
    }

    int max_size_index = max_element(size_count.begin(), size_count.end()) - size_count.begin();
    cout << "����� ���� �� �´� ����?!! " << size_kind[max_size_index] << "�ϲ���" << endl;
    if(CalAccuracy)
        cout << "K : " << k << ", �� ��Ȯ�� " << accuracy << "�� �ݹ� X" << endl;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    auto aduration = chrono::duration_cast<chrono::milliseconds>(aend - astart);

    cout << endl << "��ü ���� �ð� : " << duration.count() << "ms" << endl;
    cout << endl << "��Ȯ�� ���� ���� �ð� : " << aduration.count() << "ms" << endl;
}