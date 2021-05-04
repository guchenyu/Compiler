#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<fstream>
#include <sstream>

using namespace std;

//定义保留字
static string ReservedWord[32] = {
"auto", "break", "case", "char", "const", "continue",
"default", "do", "double", "else", "enum", "extern",
"float", "for", "goto", "if", "int", "long",
"register", "return", "short", "signed", "sizeof", "static",
"struct", "switch", "typedef", "union", "unsigned", "void",
"volatile", "while"
};


//定义特殊字符和界符
static string Operator_Delimiter[36] = {
"+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
"!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
"&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
"}", "\\", ".", "\?", ":", "!"
};
static  string IDentifierTbl = { "" };//标识符表

int searchReserve(string ReservedWord[32], string s){
	for (int i = 0; i < 32; i++)
	{
		if (s.compare(ReservedWord[i]) == 0)
		{//若成功查找，则返回种别码
			return i + 1;//返回种别码
		}
	}
	return -1;//否则返回-1，代表查找不成功，即为标识符
}

//判断是否为字母
bool IsLetter(char letter) {
	return (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z' || letter == '_')? true : false;
}

//判断是否为数字
bool IsDigit(char digit) {
	return (digit >= '0' && digit <= '9') ? true : false;
}

//过滤注释
string filterResource(string r)
{
	string tempString;
	int count = 0;
	for (int i = 0; i <r.length(); i++)
	{
		if (r[i] == '/' && r[i + 1] == '/')
		{//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
			while (r[i] != '\n')
			{
				i++;//向后扫描
			}
		}
		if (r[i] == '/' && r[i + 1] == '*')
		{//若为多行注释“/* 。。。*/”则去除该内容
			i += 2;
			while (r[i] != '*' || r[i + 1] != '/')
			{
				i++;//继续扫描
				if (r[i] == '$')
				{
					printf("注释出错，没有找到 */，程序结束！！！\n");
					exit(0);
				}
			}
			i += 2;//跨过“*/”
		}
		if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
		{//若出现无用字符，则过滤；否则加载
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
		if (IsLetter(s[i])) {//如果开头为字母
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
				cout << "(标识符 ," << token << ")" << endl;
				fp << "(标识符 ," << token << ")" << endl;
			}
			else {
				cout << "(" << syn << " ," << token << ")" << endl;
				fp << "(" << syn << " ," << token << ")" << endl;
			}
			
			continue;//继续扫描
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
		cout << "词法分析完毕" << endl;
		}
		else {
			cout << "错误，无法识别字符" << s[i] << endl;
			continue;
		}
	}
}

//读取文件文本内容进入string
string readFileIntoString(ifstream &ifile)
{
	
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}

int main() {
	ifstream inflie("in.txt");
	ofstream outfile("out.txt");
	if (!inflie.is_open()) {
		cout << "打开文件失败" << endl;
	}
	string instream;
	instream = readFileIntoString(inflie);
	cout << instream << endl;
	string fin = filterResource(instream);
	Scanner(outfile,fin);
}