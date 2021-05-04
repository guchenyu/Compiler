#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<fstream>
#include <sstream>

using namespace std;

//���屣����
static string ReservedWord[32] = {
"auto", "break", "case", "char", "const", "continue",
"default", "do", "double", "else", "enum", "extern",
"float", "for", "goto", "if", "int", "long",
"register", "return", "short", "signed", "sizeof", "static",
"struct", "switch", "typedef", "union", "unsigned", "void",
"volatile", "while"
};


//���������ַ��ͽ��
static string Operator_Delimiter[36] = {
"+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
"!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
"&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
"}", "\\", ".", "\?", ":", "!"
};
static  string IDentifierTbl = { "" };//��ʶ����

int searchReserve(string ReservedWord[32], string s){
	for (int i = 0; i < 32; i++)
	{
		if (s.compare(ReservedWord[i]) == 0)
		{//���ɹ����ң��򷵻��ֱ���
			return i + 1;//�����ֱ���
		}
	}
	return -1;//���򷵻�-1��������Ҳ��ɹ�����Ϊ��ʶ��
}

//�ж��Ƿ�Ϊ��ĸ
bool IsLetter(char letter) {
	return (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z' || letter == '_')? true : false;
}

//�ж��Ƿ�Ϊ����
bool IsDigit(char digit) {
	return (digit >= '0' && digit <= '9') ? true : false;
}

//����ע��
string filterResource(string r)
{
	string tempString;
	int count = 0;
	for (int i = 0; i <r.length(); i++)
	{
		if (r[i] == '/' && r[i + 1] == '/')
		{//��Ϊ����ע�͡�//��,��ȥ��ע�ͺ���Ķ�����ֱ�������س�����
			while (r[i] != '\n')
			{
				i++;//���ɨ��
			}
		}
		if (r[i] == '/' && r[i + 1] == '*')
		{//��Ϊ����ע�͡�/* ������*/����ȥ��������
			i += 2;
			while (r[i] != '*' || r[i + 1] != '/')
			{
				i++;//����ɨ��
				if (r[i] == '$')
				{
					printf("ע�ͳ���û���ҵ� */���������������\n");
					exit(0);
				}
			}
			i += 2;//�����*/��
		}
		if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
		{//�����������ַ�������ˣ��������
			tempString = tempString+r[i];
		}
	}
	tempString= tempString+'\0';
	cout << tempString << endl;
	return tempString;
}

void Scanner(ofstream &fp, string s) {
	char ch;
	int count = 0;
	
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ') {
			i++;
		}
		int syn = -2;
		char ch=s[i];
		string token;
		if (IsLetter(s[i])) {//�����ͷΪ��ĸ
			token =token+ s[i];
			i++;
			while (IsLetter(s[i]) || IsDigit(s[i])) {
				token=token+ s[i];
				i++;
			}
			i--;
			token = token+'\0';
			syn = searchReserve(ReservedWord, token);
			if (syn == -1) {
				cout << "(��ʶ�� ," << token << ")" << endl;
				fp << "(��ʶ�� ," << token << ")" << endl;
			}
			else {
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
			}
			
			continue;//����ɨ��
		}
		else if (IsDigit(s[i])) {
			while (IsDigit(s[i])) {
				token =token+ s[i];
				i++;
			}
			token = token + '\0';
			cout << "(99 ," << token <<")"<< endl;
			fp << "(99 ," << token << ")" << endl;
			continue;
		}
		else if ( s[i]== '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == ';' || s[i] == '(' || s[i] == ')' || s[i] == '^' || \
			s[i] == ',' || s[i] == '\"' || s[i] == '\'' || s[i] == '~' || s[i] == '#' || s[i] == '%' || s[i] == '[' || \
			s[i] == ']' || s[i] == '{' || s[i] == '}' || s[i] == '\\' || s[i] == '.' || s[i] == '\?' || s[i] == ':') {
			token.push_back(s[i]);
			//token = token+'\0';
			for (int p = 0; p < 36; p++) {
				if (token.compare(Operator_Delimiter[p]) == 0) {
					syn = 33 + p;
					cout << "(" << syn << " ," << token << ")" << endl;
					fp << "(" << syn << " ," << token << ")" << endl;
					break;
				}
			}
			continue;
		}
		else if (s[i] == '<') {
			token.push_back(s[i]);
			i++;
			if (s[i] == '=') {
				syn = 38;
				token = token + '=';
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
				continue;
			}
			else if (s[i] == '<') {
				syn = 58;
				token = token + '=';
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
				continue;
			}
			else {
				i--;
				syn = 37;
				//token = token + '\0';
				cout << "(" << syn << " ," << token << ")" << endl;
				fp<< "(" << syn << " ," << token << ")" << endl;
				continue;
			}
		}
		else if (s[i] == '>') {
			token.push_back(s[i]);
			i++;
			if (s[i] == '=') {
				syn = 40;
				token = token + '=';
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
				continue;
			}
			else if (s[i] == '>') {
				syn = 59;
				token = token + '=';
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
				continue;
			}
			else {
				i--;
				syn = 39;
				//token = token + '\0';
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
				continue;
			}
		}
		else if (s[i] == '=') {
			token.push_back(s[i]);
			i++;
			if (s[i] == '=') {
				syn = 42;
				token = token + '=' ;
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
				continue;
			}
			else {
				i--;
				syn = 41;
				//token = token + '\0';
				cout << "(" << syn << " ," << token << ")" << endl;
				fp<< "(" << syn << " ," << token << ")" << endl;
				continue;
			}
		}
		else if (s[i] == '!') {
		token.push_back(s[i]);
		i++;
		if (s[i] == '=') {
			syn = 43;
			token = token + '=' ;
			cout << "(" << syn << " ," << token << ")" << endl;
			fp << "(" << syn << " ," << token << ")" << endl;
			continue;
		}
		else {
			i--;
			syn = 68;
			//token = token ;
			cout << "(" << syn << " ," << token << ")" << endl;
			fp << "(" << syn << " ," << token << ")" << endl;
			continue;
			}
		}
		else if (s[i] == '&') {
		token.push_back(s[i]);
		i++;
		if (s[i] == '&') {
			syn = 53;
			token = token + '=';
			cout << "(" << syn << " ," << token << ")" << endl;
			fp << "(" << syn << " ," << token << ")" << endl;
			continue;
		}
		else {
			i--;
			syn = 52;
			//token = token + '\0';
			cout << "(" << syn << " ," << token << ")" << endl;
			fp << "(" << syn << " ," << token << ")" << endl;
			continue;
		}
		}
		else if (s[i] == '|') {
		token.push_back(s[i]);
		i++;
		if (s[i] == '|') {
			syn = 55;
			token = token + '=' ;
			cout << "(" << syn << " ," << token << ")" << endl;
			fp << "(" << syn << " ," << token << ")" << endl;
			continue;
		}
		else {
			i--;
			syn = 54;
			//token = token + '\0';
			cout << "(" << syn << " ," << token << ")" << endl;
			fp << "(" << syn << " ," << token << ")" << endl;
			continue;
		}
		}
		else if (s[i] == '$') {
			syn = 0;
			token.push_back(s[i]);
			cout << "(" << syn << " ," << token << ")" << endl;
			fp << "(" << syn << " ," << token << ")" << endl;
			continue;
		}
		else if (s[i] == '\0') {
		cout << "�ʷ��������" << endl;
		}
		else {
			cout << "�����޷�ʶ���ַ�" << s[i] << endl;
			continue;
		}
	}
}

//��ȡ�ļ��ı����ݽ���string
string readFileIntoString(ifstream &ifile)
{
	
	//���ļ����뵽ostringstream����buf��
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���
	return buf.str();
}

int main() {
	ifstream inflie("in.txt");
	ofstream outfile("out.txt");
	if (!inflie.is_open()) {
		cout << "���ļ�ʧ��" << endl;
	}
	string instream;
	instream = readFileIntoString(inflie);
	cout << instream << endl;
	string fin = filterResource(instream);
	Scanner(outfile,fin);
}