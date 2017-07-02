#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int Min(int a, int b, int c)//返回最小
{
	int i = a < b ? a : b;
	return i < c ? i : c;
}

int Max(int a, int b)//返回最大
{
	return a > b ? a : b;
}

//levenshtein算法（距离编辑算法）实现部分，输入两个单词，返回两个单词的相似度
double levenshtein(vector<string>::const_iterator left, vector<string>::const_iterator right) {
	int Distance;
	double similarity;//相似度
	int lLength = (*left).size();
	int rLength = (*right).size();
	if (!lLength || !rLength) {
		cerr << "至少有一个单词长度为0，请检查Wordlist文件中的输入格式" << endl;
		exit(-1);
	}
	vector<vector<int> > Dis(lLength+1, vector<int>(rLength+1));//定义Dis矩阵

	//对Dis矩阵进行初始化
	for (int i = 0; i < lLength + 1; i++) {
		Dis[i][0] = i;
	}
	for (int i = 0; i < rLength + 1; i++) {
		Dis[0][i] = i;
	}

	int temp1, temp2, temp3;//临时变量
	for (int i = 1; i < lLength + 1; i++) {//遍历整个Dis矩阵
		for (int j = 1; j < rLength + 1; j++) {
			if ((*left).at(i-1) == (*right).at(j-1)) {//相等
				Distance = 0;
			}
			else Distance = 1;
			temp1 = Dis[i - 1][j] + 1;
			temp2 = Dis[i][j - 1] + 1;
			temp3 = Dis[i - 1][j - 1] + Distance;
			Dis[i][j] = Min(temp1, temp2, temp3);
		}
	}
	similarity = 1 - (double)Dis[lLength][rLength] / Max(lLength, rLength);//计算相似度
	return similarity;//返回相似度
}

