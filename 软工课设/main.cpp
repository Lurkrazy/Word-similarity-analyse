#include "levenshtein.h"
#include <map>
#include <algorithm>
#include <Windows.h>
#include <sstream>

int totalWord;
vector<double>vec_similarity;
vector<string> wordList;//用于存放单词表
map<string, int> key;//键值对，每个单词对应其编号
vector<string> meanList;//存放单词意思
string line, word;//读入时使用

double return_vec_sim(int i, int j)//寻找矩阵压缩后的存储位置
{
	if (i > j) swap(i, j);//为了压缩存储，只存了上三角矩阵，查询时要求i < j，i为行，j为列
	return vec_similarity[i*(2 * totalWord - i - 1) / 2 + j - i - 1];//返回压缩存储后的相似度存放位置
}

bool isAdmin(string username, string password)//判断是否是管理员
{
	ifstream user("Admin.txt");
	string uName, psWord;
	while (user.good()) {
		user >> uName >> psWord;
		if (username == uName&&password == psWord) {
			user.close();
			return true;
		}
	}
	user.close();
	return false;
}

bool isUser(string username, string password)//判断是否是用户
{
	ifstream user("User.txt");
	string uName, psWord;
	while (user.good()) {
		user >> uName >> psWord;
		if (username == uName&&password == psWord) {
			user.close();
			return true;
		}
	}
	user.close();
	return false;
}

void readWordList()//从文件中读入单词表，存入wordList和meanList中
{
	string inFileName = "Wordlist.txt";//单词表，用于读取
	ifstream in(inFileName);
	if (!in) {
		cerr << "can't open the file" << endl;//打开文件失败，报错
		exit(-1);
	}
	int number = 0;
	string meaning;//单词意思
	while (in.good()) {
		in >> word;
		getline(in, meaning);
		meanList.push_back(meaning);
		wordList.push_back(word);
		key[word] = number++;
	}
	totalWord = wordList.size();
	in.close();//输入完毕，关闭文件
}

void adminOP()//管理员操作
{
	system("cls");
	while (1) {
		cout << "1-添加用户" << endl << "2-为单词表添加单词和词义" << endl;
		cout << "请输入您要进行的操作（输入0结束）：" << endl;
		string opType;//用于判断操作类型
		cin >> opType;
		if (opType == "0")break;//0操作直接退出
		string inUserName, inPassWord;
		if (opType == "1") {//1操作添加用户
			fstream uFile;
			while (1) {
				system("cls");
				uFile.open("User.txt", ios::out | ios::app);
				cout << "输入需要增加的用户名（输入0结束）：";
				cin >> inUserName;
				if (inUserName == "0")break;//输入0结束输入
				cout << "输入其对应的密码：";
				cin >> inPassWord;
				uFile << endl << inUserName << endl << inPassWord;//存入User.txt用户数据文件中
				cout << "增加用户成功！";
				system("pause");
			}
			uFile.close();
		}
		else if (opType == "2") {//2操作添加单词和词义
			fstream wFile;
			while (1) {
				system("cls");
				string inWord, inMean;
				cout << "请输入您需要添加的单词（输入0结束）：";
				cin >> inWord;
				if (inWord == "0")break;//输入0结束输入
				int i = key[inWord];
				if (i != 0 || i == 0 && inWord == wordList[0]) {//说明wordList中已经存有该单词和词义
					cout << "已经含有该单词，不必重复输入" << endl;
					system("pause");
					continue;
				}
				cout << "请输入其对应的词义：";
				cin >> inMean;
				wFile.open("Wordlist.txt", ios::out | ios::app);
				wFile << endl << inWord << "  " << inMean;//存入Wordlist.txt单词表中
				cout << "添加单词成功!" << endl;
				system("pause");
			}
			wFile.close();
		}
	}
	system("cls");
	cout << "即将退出！" << endl;
	system("pause");
}

void userOP()//用户操作
{
	cout << "是否使用上次运行的数据？（1为是，0为否）： ";//可基于已有的相似度数据运行
	bool ifUseTheData;
	cin >> ifUseTheData;
	cout << "正在读入数据，请稍候....";
	string save_vec_sim = "save_vec_sim.txt";//存放上次程序计算的相似度数据
	if (ifUseTheData) {//使用已有数据
		ifstream iVecSim(save_vec_sim);
		double fVecSim;//用于在文件中读取vec_similarity的数据
		while (iVecSim.good()) {
			iVecSim >> fVecSim;
			vec_similarity.push_back(fVecSim);//将已有相似度数据存入vec_similarity中
		}
		iVecSim.close();
	}
	else {//不使用已有数据，重新计算
		vector<string>::const_iterator left = wordList.begin();
		vector<string>::const_iterator right;
		ofstream oVecSim(save_vec_sim);
		double vec_sim;//用于输入
		for (; left != wordList.end() - 1; ++left) {//所有单词两两比较
			for (right = left + 1; right != wordList.end(); ++right) {
				vec_sim = levenshtein(left, right);
				vec_similarity.push_back(vec_sim);//将相似度数据存在vec_similarity中
				oVecSim << vec_sim << endl;//存入本次运行的数据
			}
		}
		oVecSim.close();
	}
	system("cls");
	
	//输入一个单词
	while (1) {
		cout << "请输入要查询的单词（输入0结束查询）： ";
		cin >> word;
		if (word == "0")break;
		int i = key[word];
		if (i == 0 && word != wordList[0]) {//在词库中查不到该单词
			cout << "输入有误或词库中无该单词，请重新输入" << endl;
			system("pause");
			system("cls");
			continue;
		}
		string outFileName = "log.txt";//存放查询记录
		ofstream log(outFileName);
		if (!log) {
			cerr << "can't create log.txt" << endl;//报错
		}
		cout << "该单词的意思为：" << meanList[i] << endl << endl;
		while (1) {
			cout << "对于单词 " << word << " 请输入想查询的相似度（输入0结束查询）：";
			double setSim;//设置相似度标准
			cin >> setSim;
			if (setSim == 0)break;
			if (setSim <= 0.5) {
				cout << "相似度不高于0.5就不像了哦！请重新输入！" << endl;
				system("pause");//暂停
				cin.get();
				system("cls");
				continue;
			}
			//输出与其相似的单词
			cout << "与" + word + "相似度大于" << setSim << "的单词有：" << endl;
			log << "与" + word + "相似度大于" << setSim << "的单词有：" << endl;
			for (int j = 0; j < totalWord; j++) {
				if (i == j)continue;//不和自己比较
				if (return_vec_sim(i, j) >= setSim) {
					cout << wordList[j] << meanList[j] << endl;//输出单词
					log << wordList[j] << meanList[j] << endl;//输出单词意思
				}
			}
			log << endl;
			cout << endl;
		}
		log.close();//关闭文件
		system("cls");
	}
	cout << "查询完成，你可以在log.txt文件中查看相似单词的查询记录" << endl << "即将退出！" << endl;
	system("pause");
}


int main()
{
	readWordList();//读入单词表
	string username, password;
	cout << "输入用户名：";
	cin >> username;
	cout << "输入密码：";
	cin >> password;
	if (isAdmin(username, password)) {//管理员登陆
		adminOP();//管理员操作
	}
	else if (isUser(username, password)) {//用户登录
		userOP();//用户操作
	}
	else {
		cout << "用户名或密码输入错误!" << endl << "即将退出！" << endl;
		system("pause");
	}
	return 0;
}