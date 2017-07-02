#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int Min(int a, int b, int c)//������С
{
	int i = a < b ? a : b;
	return i < c ? i : c;
}

int Max(int a, int b)//�������
{
	return a > b ? a : b;
}

//levenshtein�㷨������༭�㷨��ʵ�ֲ��֣������������ʣ������������ʵ����ƶ�
double levenshtein(vector<string>::const_iterator left, vector<string>::const_iterator right) {
	int Distance;
	double similarity;//���ƶ�
	int lLength = (*left).size();
	int rLength = (*right).size();
	if (!lLength || !rLength) {
		cerr << "������һ�����ʳ���Ϊ0������Wordlist�ļ��е������ʽ" << endl;
		exit(-1);
	}
	vector<vector<int> > Dis(lLength+1, vector<int>(rLength+1));//����Dis����

	//��Dis������г�ʼ��
	for (int i = 0; i < lLength + 1; i++) {
		Dis[i][0] = i;
	}
	for (int i = 0; i < rLength + 1; i++) {
		Dis[0][i] = i;
	}

	int temp1, temp2, temp3;//��ʱ����
	for (int i = 1; i < lLength + 1; i++) {//��������Dis����
		for (int j = 1; j < rLength + 1; j++) {
			if ((*left).at(i-1) == (*right).at(j-1)) {//���
				Distance = 0;
			}
			else Distance = 1;
			temp1 = Dis[i - 1][j] + 1;
			temp2 = Dis[i][j - 1] + 1;
			temp3 = Dis[i - 1][j - 1] + Distance;
			Dis[i][j] = Min(temp1, temp2, temp3);
		}
	}
	similarity = 1 - (double)Dis[lLength][rLength] / Max(lLength, rLength);//�������ƶ�
	return similarity;//�������ƶ�
}

