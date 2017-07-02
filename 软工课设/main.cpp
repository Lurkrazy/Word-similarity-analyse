#include "levenshtein.h"
#include <map>
#include <algorithm>
#include <Windows.h>
#include <sstream>

int totalWord;
vector<double>vec_similarity;
vector<string> wordList;//���ڴ�ŵ��ʱ�
map<string, int> key;//��ֵ�ԣ�ÿ�����ʶ�Ӧ����
vector<string> meanList;//��ŵ�����˼
string line, word;//����ʱʹ��

double return_vec_sim(int i, int j)//Ѱ�Ҿ���ѹ����Ĵ洢λ��
{
	if (i > j) swap(i, j);//Ϊ��ѹ���洢��ֻ���������Ǿ��󣬲�ѯʱҪ��i < j��iΪ�У�jΪ��
	return vec_similarity[i*(2 * totalWord - i - 1) / 2 + j - i - 1];//����ѹ���洢������ƶȴ��λ��
}

bool isAdmin(string username, string password)//�ж��Ƿ��ǹ���Ա
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

bool isUser(string username, string password)//�ж��Ƿ����û�
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

void readWordList()//���ļ��ж��뵥�ʱ�����wordList��meanList��
{
	string inFileName = "Wordlist.txt";//���ʱ����ڶ�ȡ
	ifstream in(inFileName);
	if (!in) {
		cerr << "can't open the file" << endl;//���ļ�ʧ�ܣ�����
		exit(-1);
	}
	int number = 0;
	string meaning;//������˼
	while (in.good()) {
		in >> word;
		getline(in, meaning);
		meanList.push_back(meaning);
		wordList.push_back(word);
		key[word] = number++;
	}
	totalWord = wordList.size();
	in.close();//������ϣ��ر��ļ�
}

void adminOP()//����Ա����
{
	system("cls");
	while (1) {
		cout << "1-����û�" << endl << "2-Ϊ���ʱ���ӵ��ʺʹ���" << endl;
		cout << "��������Ҫ���еĲ���������0��������" << endl;
		string opType;//�����жϲ�������
		cin >> opType;
		if (opType == "0")break;//0����ֱ���˳�
		string inUserName, inPassWord;
		if (opType == "1") {//1��������û�
			fstream uFile;
			while (1) {
				system("cls");
				uFile.open("User.txt", ios::out | ios::app);
				cout << "������Ҫ���ӵ��û���������0��������";
				cin >> inUserName;
				if (inUserName == "0")break;//����0��������
				cout << "�������Ӧ�����룺";
				cin >> inPassWord;
				uFile << endl << inUserName << endl << inPassWord;//����User.txt�û������ļ���
				cout << "�����û��ɹ���";
				system("pause");
			}
			uFile.close();
		}
		else if (opType == "2") {//2������ӵ��ʺʹ���
			fstream wFile;
			while (1) {
				system("cls");
				string inWord, inMean;
				cout << "����������Ҫ��ӵĵ��ʣ�����0��������";
				cin >> inWord;
				if (inWord == "0")break;//����0��������
				int i = key[inWord];
				if (i != 0 || i == 0 && inWord == wordList[0]) {//˵��wordList���Ѿ����иõ��ʺʹ���
					cout << "�Ѿ����иõ��ʣ������ظ�����" << endl;
					system("pause");
					continue;
				}
				cout << "���������Ӧ�Ĵ��壺";
				cin >> inMean;
				wFile.open("Wordlist.txt", ios::out | ios::app);
				wFile << endl << inWord << "  " << inMean;//����Wordlist.txt���ʱ���
				cout << "��ӵ��ʳɹ�!" << endl;
				system("pause");
			}
			wFile.close();
		}
	}
	system("cls");
	cout << "�����˳���" << endl;
	system("pause");
}

void userOP()//�û�����
{
	cout << "�Ƿ�ʹ���ϴ����е����ݣ���1Ϊ�ǣ�0Ϊ�񣩣� ";//�ɻ������е����ƶ���������
	bool ifUseTheData;
	cin >> ifUseTheData;
	cout << "���ڶ������ݣ����Ժ�....";
	string save_vec_sim = "save_vec_sim.txt";//����ϴγ����������ƶ�����
	if (ifUseTheData) {//ʹ����������
		ifstream iVecSim(save_vec_sim);
		double fVecSim;//�������ļ��ж�ȡvec_similarity������
		while (iVecSim.good()) {
			iVecSim >> fVecSim;
			vec_similarity.push_back(fVecSim);//���������ƶ����ݴ���vec_similarity��
		}
		iVecSim.close();
	}
	else {//��ʹ���������ݣ����¼���
		vector<string>::const_iterator left = wordList.begin();
		vector<string>::const_iterator right;
		ofstream oVecSim(save_vec_sim);
		double vec_sim;//��������
		for (; left != wordList.end() - 1; ++left) {//���е��������Ƚ�
			for (right = left + 1; right != wordList.end(); ++right) {
				vec_sim = levenshtein(left, right);
				vec_similarity.push_back(vec_sim);//�����ƶ����ݴ���vec_similarity��
				oVecSim << vec_sim << endl;//���뱾�����е�����
			}
		}
		oVecSim.close();
	}
	system("cls");
	
	//����һ������
	while (1) {
		cout << "������Ҫ��ѯ�ĵ��ʣ�����0������ѯ���� ";
		cin >> word;
		if (word == "0")break;
		int i = key[word];
		if (i == 0 && word != wordList[0]) {//�ڴʿ��в鲻���õ���
			cout << "���������ʿ����޸õ��ʣ�����������" << endl;
			system("pause");
			system("cls");
			continue;
		}
		string outFileName = "log.txt";//��Ų�ѯ��¼
		ofstream log(outFileName);
		if (!log) {
			cerr << "can't create log.txt" << endl;//����
		}
		cout << "�õ��ʵ���˼Ϊ��" << meanList[i] << endl << endl;
		while (1) {
			cout << "���ڵ��� " << word << " ���������ѯ�����ƶȣ�����0������ѯ����";
			double setSim;//�������ƶȱ�׼
			cin >> setSim;
			if (setSim == 0)break;
			if (setSim <= 0.5) {
				cout << "���ƶȲ�����0.5�Ͳ�����Ŷ�����������룡" << endl;
				system("pause");//��ͣ
				cin.get();
				system("cls");
				continue;
			}
			//����������Ƶĵ���
			cout << "��" + word + "���ƶȴ���" << setSim << "�ĵ����У�" << endl;
			log << "��" + word + "���ƶȴ���" << setSim << "�ĵ����У�" << endl;
			for (int j = 0; j < totalWord; j++) {
				if (i == j)continue;//�����Լ��Ƚ�
				if (return_vec_sim(i, j) >= setSim) {
					cout << wordList[j] << meanList[j] << endl;//�������
					log << wordList[j] << meanList[j] << endl;//���������˼
				}
			}
			log << endl;
			cout << endl;
		}
		log.close();//�ر��ļ�
		system("cls");
	}
	cout << "��ѯ��ɣ��������log.txt�ļ��в鿴���Ƶ��ʵĲ�ѯ��¼" << endl << "�����˳���" << endl;
	system("pause");
}


int main()
{
	readWordList();//���뵥�ʱ�
	string username, password;
	cout << "�����û�����";
	cin >> username;
	cout << "�������룺";
	cin >> password;
	if (isAdmin(username, password)) {//����Ա��½
		adminOP();//����Ա����
	}
	else if (isUser(username, password)) {//�û���¼
		userOP();//�û�����
	}
	else {
		cout << "�û����������������!" << endl << "�����˳���" << endl;
		system("pause");
	}
	return 0;
}