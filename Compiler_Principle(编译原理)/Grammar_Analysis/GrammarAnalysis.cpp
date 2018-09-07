#include "stdio.h"
#include "string"
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<stack>
#include<queue>
#include <map>

using namespace std;
stack<string>downstack;//下推栈
queue<string>inputstr;//输入字符串
ofstream fout;//


//非终结符
string VN[38] = {
	"PR","DL","D","D'","VD","VD'","P","PS","PL","PL'",
	"CS","LD","SL","S","ES","SS","SS'","IS","RS","RS'"
	,"E","V","V'","SE","SE'","AE","AE'","T","T'","F",
	"F'","AS","AL","AL'","ROP","AOP","MOP","TS"};

//终结符
string VT[31] = {"id","num","decimal", "int","void","float","if","else","while","return",
";",",","(",")","[","]","{","}","=","+","-","*","/","<",">","<=",">=","==","!=","&&","||" };



stack<int>VNCOUNT[69];//用于统计输出图每个节点的标号
int vcount[69] = { 0 };
void printLink(int n1, int n2, string nodeName1, string nodeName2)
{

	int t1, t2;

	t1 = VNCOUNT[n1].top();//左边用栈顶原节点值
	
	t2=++vcount[n2];//右边用新建的节点值
	VNCOUNT[n2].push(t2);

	fout << "node" << t2 << "[label=\"" << nodeName2 << "\"]" << endl;
	fout << "node" << t1 << "->" << "node" << t2 << endl;
}

bool SearchTable(string X, string a);


//判断是否为终结符
bool IsVT(string str) {
	for (int i = 0; i < 29; i++) {
		if (VT[i]==str)
			return true;
	}
	return false;
}

//判断是否为非终结符
bool ISVN(string str) {
	for (int i = 0; i < 38; i++) {
		if (VN[i] == str)
			return true;
	}
	return false;
}


//初始化输入串队列
void QueueInitialize(FILE *inf) {
	char ch;
	string str="";

	while (!feof(inf)) {
		ch = fgetc(inf);
		if (ch != '\n')
			str = str + ch;
		else {
			inputstr.push(str);
			str = "";
		}		
	}
	inputstr.push("#");

	for (int i = 0; i < 69; i++) {
		VNCOUNT[i].push( i * 100);//初始化Node VN编号
		vcount[i] = i * 100;
	}
	
}

//词法分析
bool GrammarAnalysis() {
	downstack.push("#");//
	downstack.push("PR");//

	while (inputstr.front() != "#") {//输入串没读完
		string X = downstack.top();//下推栈栈顶
		string a = inputstr.front();//输入串串首
		if (X == a){//匹配
			downstack.pop();//弹出栈顶符号X
			inputstr.pop();//读头前进一格
			cout << "***************匹配***************" << X << endl;
			continue;
		}
		if (IsVT(X) && (X != a)) {//如果X属于VT  X不等于a
			cout << "语法分析出错！" << endl;
			return false;//ERROR
		}
		if (ISVN(X)) {
			if (SearchTable(X, a)) {

			}
			else {
				cout << "语法分析出错！" << endl;
				return false;//ERROR
			}
		}
		else {
			cout << "语法分析出错！" << endl;
			return false;
		}
	}
	cout << "语法分析成功！" << endl;
	return true;
}

//查询LL1表
bool SearchTable(string X,string a) {

	//E--SE    E--V=E   //这里悄悄往后查一个
	if (X == "E" &&a == "id") {
		int len = inputstr.size();//当前输入字符串长度
		inputstr.pop();//弹出队列头
		inputstr.push(a);//队列头放入队列尾
		string a2 = inputstr.front();//队列第二个元素
		int i = 0;
		string buf = "";
		while (i < len - 1) {
			buf = inputstr.front();//队列头
			inputstr.pop();//弹出
			inputstr.push(buf);//放入队列尾
			i++;
		}//恢复原队列

		if (a2 == "=") {//E-- V = E 
			downstack.pop();
			downstack.push("E");
			downstack.push("=");
			downstack.push("V");
			printLink(20, 21, "E", "V");
			printLink(20, 56, "E", "=");
			printLink(20, 20, "E", "E");
			//VNCOUNT[20].pop();//原始值用后要释放  递归调用
		}
		else {  //E--SE 
			downstack.pop();
			downstack.push("SE");
			printLink(20, 23, "E", "SE");
			VNCOUNT[20].pop();//原始值用后要释放
		}
		return true;
	}
	
	if (X == "PR" && (a == "int" || a == "void" || a == "float" || a == "#")) {
		downstack.pop();
		downstack.push("DL");
		cout << "PR--DL" << endl;

		//输出graphviz格式的语言
		
		printLink(0, 1,"PR" , "DL" );
		VNCOUNT[0].pop();//原始值用后要释放

		return true;
	}
	if (X == "DL" && (a == "int" || a == "void" || a == "float" )) {
		downstack.pop();//DL出栈
		downstack.push("DL");//产生式自右向左压栈
		downstack.push("D");
		cout << "DL--D DL" << endl;

		//输出graphviz格式的语言	
		printLink(1, 2, "DL", "D");
		printLink(1, 1, "DL", "DL");
		// VNCOUNT[1].pop();//原始值用后要释放   递归调用
		return true;
	}
	if (X == "DL" && a == "#") {
		downstack.pop();//DL出栈 此时产生式是空串
		cout << "DL--EPS" << endl;
		printLink(1, 68, "DL", "eps");
		VNCOUNT[1].pop();//原始值用后要释放
		return true;
	}
	if (X == "D" && (a == "int" || a == "void" || a == "float")) {
		downstack.pop();//DL出栈
		downstack.push("D'");//产生式自右向左压栈
		downstack.push("id");
		downstack.push("TS");
		cout << "D--TS id D'" << endl;


		//输出graphviz格式的语言

		printLink(2, 37, "D", "TS");
		printLink(2, 38, "D", "id");
		printLink(2, 3, "D", "D'");
		VNCOUNT[2].pop();//原始值用后要释放
		return true;
	}
	if (X == "D'" && (a == "int" || a == "void" || a == "float")) {//D'--EPS
		downstack.pop();//D'出栈
		cout << "D'--EPS" << endl;
		printLink(3, 68, "D'", "eps");
		VNCOUNT[3].pop();//原始值用后要释放
		return true;
	}
	if (X == "D'" && (a == "[")) {//D'--[ NUM ]
		downstack.pop();//D'出栈
		downstack.push("]");//产生式自右向左压栈
		downstack.push("num");
		downstack.push("[");
		cout << "D'--[ num ]" << endl;

		//输出graphviz格式的语言	
		printLink(3, 50, "D'", "[");
		printLink(3, 39, "D'", "num");
		printLink(3, 51, "D'", "]'");
		VNCOUNT[3].pop();//原始值用后要释放
		return true;
	}
	
	if (X == "D'" && (a == "(")) {//D'--(PS)CS
		downstack.pop();//D'出栈
		downstack.push("CS");//产生式自右向左压栈
		downstack.push(")");
		downstack.push("PS");
		downstack.push("(");

		cout << "D'--( PS ) CS" << endl;

		//输出graphviz格式的语言
		printLink(3, 52, "D'", "(");
		printLink(3, 7, "D'", "PS");
		printLink(3, 53, "D'", ")");
		printLink(3, 10, "D'", "CS");
		VNCOUNT[3].pop();//原始值用后要释放
		return true;
	}

	if (X == "VD" && (a == "int" || a == "void" || a == "float")) {//VD--TS ID VD' 
		downstack.pop();//D'出栈
		downstack.push(";");
		downstack.push("VD'");//产生式自右向左压栈
		downstack.push("id");
		downstack.push("TS");
		cout << " VD--TS id VD';" << endl;

		printLink(4, 37, "VD", "TS");
		printLink(4, 38, "VD", "id");
		printLink(4, 5, "VD", "VD'");
		printLink(4, 48, "VD", ";");


		VNCOUNT[4].pop();//原始值用后要释放
		return true;
	}
	if (X == "VD'" && (a == "[")) {//VD'--[ NUM ]
		downstack.pop();//VD'出栈
		downstack.push("]");//产生式自右向左压栈
		downstack.push("num");
		downstack.push("[");
		cout << " VD'--[ num ]" << endl;

		//输出graphviz格式的语言
		printLink(5, 50, "VD'", "[");
		printLink(5, 39, "VD'", "num");
		printLink(5, 51, "VD'", "]");
		VNCOUNT[5].pop();//原始值用后要释放
		return true;
	}
	if (X == "VD'" && (a == "int" || a == "void" || a == "float"  
		||a=="if"||a=="while" ||a=="return"   
		||a=="id" ||a=="num"||a=="decimal"
		||a==";")) {
		downstack.pop();//VD'出栈	
		cout << " VD'--EPS" << endl;
		printLink(5, 68, "VD'", "eps");
		VNCOUNT[5].pop();//原始值用后要释放

		return true;
	}
	//P--TS ID
	if (X == "P" && (a == "int" || a == "void" || a == "float")) {
		downstack.pop();//VD'出栈
		downstack.push("id");//产生式自右向左压栈
		downstack.push("TS");
		cout << " P--TS id" << endl;
		//输出graphviz格式的语言

		printLink(6, 37, "P", "TS");
		printLink(6, 38, "P", "id");
		VNCOUNT[6].pop();//原始值用后要释放

		return true;
	}
	//PS--PL
	if (X == "PS" && (a == "int" || a == "void" || a == "float")) {
		downstack.pop();//VD'出栈
		downstack.push("PL");//产生式自右向左压栈
		cout << "PS--PL" << endl;

		//输出graphviz格式的语言

		printLink(7, 8, "PS", "PL");
		VNCOUNT[7].pop();//原始值用后要释放
		return true;
	}
	if (X == "PS" && (a ==")")) {//Ps--EPS
		downstack.pop();//VD'出栈
		cout << "PS--EPS" << endl;
		printLink(7, 68, "PS", "eps");
		VNCOUNT[7].pop();//原始值用后要释放
		return true;
	}
	//PL--P PL'
	if (X == "PL" && (a == "int" || a == "void" || a == "float")) {
		downstack.pop();//VD'出栈
		downstack.push("PL'");//产生式自右向左压栈
		downstack.push("P");
		cout << "PL--P PL'" << endl;

		//输出graphviz格式的语言

		printLink(8, 6, "PL", "P");
		printLink(8, 9, "PL", "PL'");
		VNCOUNT[8].pop();//原始值用后要释放
		return true;
	}
	//PL'--, P PL'
	if (X == "PL'" && (a == ",")) {
		downstack.pop();//VD'出栈
		downstack.push("PL'");//产生式自右向左压栈
		downstack.push("P");
		downstack.push(",");
		cout << "PL'--, P PL'" << endl;
		//输出graphviz格式的语言

		printLink(9, 49, "PL'", ",");
		printLink(9, 6, "PL'", "P");
		printLink(9, 9, "PL'", "PL'");
		//VNCOUNT[9].pop();//原始值用后要释放 递归调用
		return true;
	}
	if (X == "PL'" && (a == ")")) {//PL'--EPS
		downstack.pop();//VD'出栈
		cout << "PL'--EPS'" << endl;
		printLink(9, 68, "PL'", "eps");
		VNCOUNT[9].pop();//原始值用后要释放
		return true;
	}
	
	if (X == "CS" && (a == "{")) {//CS--{LD SL}
		downstack.pop();//VD'出栈
		downstack.push("}");
		downstack.push("SL");
		downstack.push("LD");
		downstack.push("{"); 
		cout << "CS--{LD SL}" << endl;
		//输出graphviz格式的语言

		printLink(10, 54, "CS", "{");
		printLink(10, 11, "CS", "LD");
		printLink(10, 12, "CS", "SL");
		printLink(10, 55, "CS", "}");
		VNCOUNT[10].pop();//原始值用后要释放
		return true;
	}

	//LD--VD LD
	if (X == "LD" && (a == "int" || a == "void" || a == "float")) {
		downstack.pop();//VD'出栈
		downstack.push("LD");//产生式自右向左压栈
		downstack.push("VD");
		cout << "LD--VD LD" << endl;
		printLink(11, 4, "LD", "VD");
		printLink(11, 11, "LD", "LD");
		//VNCOUNT[11].pop();//原始值用后要释放 递归调用
		return true;
	}
	//LD--EPS
	if (X == "LD" && (a == ";" || a == "("||a=="{"
		|| a == "if" || a == "while" || a == "return"
		|| a == "id" || a == "num" || a == "decimal"
		)) {
		downstack.pop();//VD'出栈
		cout << "LD--EPS" << endl;
		printLink(11, 68, "LD", "eps");
		VNCOUNT[11].pop();//原始值用后要释放
		return true;
	}
	//SL--S SL
	if (X == "SL" && (a == ";" || a == "(" || a == "{"
		|| a == "if" || a == "while" || a == "return"
		|| a == "id" || a == "num" || a == "decimal"
		)) {
		downstack.pop();
		downstack.push("SL");
		downstack.push("S");
		cout << "SL--S SL" << endl;
		printLink(12, 13, "SL", "S");
		printLink(12, 12, "SL", "SL");
		//VNCOUNT[12].pop();//原始值用后要释放
		return true;
	}
	//SL--EPS
	if (X == "SL" && (a =="}")) {
		downstack.pop();//VD'出栈
		cout << "SL--EPS" << endl;
		printLink(12, 68, "SL", "eps");
		VNCOUNT[12].pop();//原始值用后要释放
		return true;
	}
	//S--ES      FIRST(ES)={    ;    (  ID NUM  DECIMAL  }
	if (X == "S" && (a == "id" || a == "num" || a == "decimal"
		|| a == ";" || a == "(" )) {
		downstack.pop();
		downstack.push("ES");
		cout << "S--ES" << endl;
		printLink(13, 14, "S", "ES");
		VNCOUNT[13].pop();//原始值用后要释放
		return true;
	}
	//S--SS
	if (X == "S" && (a == "if")) {
		downstack.pop();
		downstack.push("SS");
		cout << "S--SS" << endl;
		printLink(13, 15, "S", "SS");VNCOUNT[13].pop();//原始值用后要释放
		return true;
	}

	//S--IS
	if (X == "S" && (a == "while")) {
		downstack.pop();
		downstack.push("IS");
		cout << "S--IS" << endl;
		printLink(13, 17, "S", "IS"); VNCOUNT[13].pop();//原始值用后要释放
		return true;
	}
	//S--RS
	if (X == "S" && (a == "return")) {
		downstack.pop();
		downstack.push("RS");
		cout << "S--RS" << endl;
		printLink(13, 18, "S", "RS"); VNCOUNT[13].pop();//原始值用后要释放
		return true;
	}
	//S--CS
	if (X == "S" && (a == "{")) {
		downstack.pop();
		downstack.push("CS");
		cout << "S--CS" << endl;
		printLink(13, 10, "S", "CS"); VNCOUNT[13].pop();//原始值用后要释放
		return true;
	}

	//ES--E;    FIRST(E)= {  (  ID NUM  DECIMAL }
	if (X == "ES" && (a == "{" || a == "id" || a == "num" || a == "decimal") ) {
		downstack.pop();
		downstack.push(";");
		downstack.push("E");
		cout << "ES--E;" << endl;
		printLink(14, 20, "ES", "E"); 
		printLink(14, 48, "ES", ";");
		VNCOUNT[14].pop();//原始值用后要释放
		return true;
	}
	//ES--;
	if (X == "ES" && (a == ";")) {
		downstack.pop();
		downstack.push(";");
		cout << "ES--;" << endl;
		printLink(14, 48, "ES", ";"); VNCOUNT[14].pop();//原始值用后要释放
		return true;
	}
	//SS--if ( E )  S   SS'
	if (X == "SS" && (a == "if")) {
		downstack.pop();
		downstack.push("SS'");
		downstack.push("S");
		downstack.push(")");
		downstack.push("E");
		downstack.push("(");
		downstack.push("if");
		cout << "SS-- if ( E )  S   SS'" << endl;
		printLink(15, 44, "SS", "if");
		printLink(15, 52, "SS", "(");
		printLink(15, 20, "SS", "E");
		printLink(15, 53, "SS", ")");
		printLink(15, 13, "SS", "S");
		printLink(15, 16, "SS", "SS'"); VNCOUNT[15].pop();//原始值用后要释放
		return true;
	}
	//SS' -- else S
	if (X == "SS'" && (a == "else")) {
		downstack.pop();
		downstack.push("S");
		downstack.push("else");
		cout << "SS' -- else S " << endl;
		printLink(16, 45, "SS'", "else");
		printLink(16, 13, "SS'", "S"); VNCOUNT[16].pop();//原始值用后要释放
		return true;
	}
	//SS'--EPS      FOLLOW(SS’)= {  else     } ; {  (        if while  return        ID NUM  DECIMAL  }
	if (X == "SS'" && (a == ";" || a == "{" ||a=="}"|| a=="("
		|| a == "if" || a == "while" || a == "return"
		|| a == "id" || a == "num" || a == "decimal"
		)) {
		downstack.pop();
		cout << "SS'--EPS" << endl;
		printLink(16, 68, "SS'", "eps");
		VNCOUNT[16].pop();//原始值用后要释放
		return true;
	}
	//IS--while( E ) S
	if (X == "IS" && (a == "while")) {
		downstack.pop();
		downstack.push("S");
		downstack.push(")");
		downstack.push("E");
		downstack.push("("); 
		downstack.push("while");
		cout << "IS-- while (  E ) S" << endl;
		printLink(17, 46, "IS", "while");
		printLink(17, 52, "IS", "(");
		printLink(17, 20, "IS", "E");
		printLink(17, 53, "IS", ")");
		printLink(17, 13, "IS", "S"); VNCOUNT[17].pop();//原始值用后要释放
		return true;
	}
	//RS--return RS'
	if (X == "RS" && (a == "return")) {
		downstack.pop();
		downstack.push("RS'");
		downstack.push("return");
		cout << "RS--return RS'" << endl;
		printLink(18, 47, "RS", "return");
		printLink(18, 19, "RS", "RS'"); VNCOUNT[18].pop();//原始值用后要释放
		return true;
	}
	//RS'--E  FIRST(E)= {  (  ID NUM  DECIMAL }
	if (X == "RS'" && (a == "{" || a == "id" || a == "num" || a == "decimal")) {
		downstack.pop();
		downstack.push("E");
		cout << "RS'--E" << endl;
		printLink(19,20 , "RS'", "E"); VNCOUNT[19].pop();//原始值用后要释放
		return true;
	}
	//RS'--;
	if (X == "RS'" && (a ==";")) {
		downstack.pop();
		downstack.push(";");
		cout << "RS'--;" << endl;
		printLink(19, 48, "RS'", ";"); VNCOUNT[19].pop();//原始值用后要释放
		return true;
	}


	 //E--SE   FIRST(simple-E)= {   (  ID NUM  DECIMAL }
	if (X == "E" && (a == "(" || a == "num" || a == "decimal")) {
		downstack.pop();
		downstack.push("SE");
		cout << "E--SE" << endl;
		printLink(20, 23, "E", "SE"); VNCOUNT[20].pop();//原始值用后要释放
		return true;
	}
	//V--id V'
	if (X == "V" && (a == "id")) {
		downstack.pop();
		downstack.push("V'");
		downstack.push("id");
		cout << "V--id V'" << endl;
		printLink(21, 38, "V", "id");
		printLink(21, 22, "V", "V'"); VNCOUNT[21].pop();//原始值用后要释放
		return true;
	}

	//V'--[E]
	if (X == "V'" && (a == "[")) {
		downstack.pop();
		downstack.push("]");
		downstack.push("E");
		downstack.push("[");
		cout << "V'--[E]" << endl;
		printLink(22,50 , "V'", "[");
		printLink(22, 20, "V'", "E");
		printLink(22, 51, "V'", "]"); VNCOUNT[22].pop();//原始值用后要释放
		return true;
	}

	//V'--EPS
	if (X == "V'" && (a == "=")) {
		downstack.pop();
		cout << "V'--EPS" << endl;
		printLink(22, 68, "V'", "eps");
		VNCOUNT[22].pop();//原始值用后要释放
		return true;
	}

	//SE--AE SE' FIRST(additive-E)= {  (  ID NUM  DECIMAL }
	if (X == "SE" && (a == "(" || a == "id" || a == "num" || a == "decimal")) {
		downstack.pop();
		downstack.push("SE'");
		downstack.push("AE");
		cout << "SE--AE SE'" << endl;
		printLink(23, 25, "SE", "AE");
		printLink(23, 24, "SE", "SE'"); VNCOUNT[23].pop();//原始值用后要释放
		return true;
	}

	//SE'--EPS   FOLLOW(simple-E’)={  ;   )  ]  ,   }
	if (X == "SE'" && (a == ";" || a == ")" || a == "]" || a == ",")) {
		downstack.pop();
		cout << "SE'--EPS" << endl;
		printLink(24, 68, "SE'", "eps");
		VNCOUNT[24].pop();//原始值用后要释放
		return true;
	}

	//SE'--ROP AE  FIRST(ROP)= {  <   >   <=  >=    ==   !=    &&    ||    }
	if (X == "SE'" && (a == "<" || a == ">" || a == "<=" || a == ">="
		|| a == "==" || a == "!=" || a == "&&"|| a == "||" 
		)) {
		downstack.pop();
		downstack.push("AE");
		downstack.push("ROP"); 
		cout << "SE'--ROP AE" << endl;
		printLink(24, 34, "SE'", "ROP");
		printLink(24, 25, "SE'", "AE"); VNCOUNT[24].pop();//原始值用后要释放
		return true;
	}  
	//AE--T AE'  FIRST(T)= {  (  ID NUM  DECIMAL }
	if (X == "AE" && (a == "(" || a == "id" || a == "num" || a == "decimal")) {
		downstack.pop();
		downstack.push("AE'");
		downstack.push("T");
		cout << "AE--T AE'" << endl;
		printLink(25, 27, "AE", "T");
		printLink(25, 26, "AE", "AE'"); VNCOUNT[25].pop();//原始值用后要释放
		return true;
	}

	//AE'--EPS     FOLLOW(AE’)={ ;   )  ]  ,          <   >   <=   >=       ==   !=    &&    ||     }
	if (X == "AE'" && (a == "<" || a == ">" || a == "<=" || a == ">="
		|| a == "==" || a == "!=" || a == "&&" || a == "||"
		|| a == ";" || a == ")" || a == "]" || a == ","
		)) {
		downstack.pop();
		cout << "AE'--EPS" << endl;
		printLink(26, 68, "AE'", "eps");
		VNCOUNT[26].pop();//原始值用后要释放
		return true;
	}

	//AE'--AOP T AE'
	if (X == "AE'" && (a == "+" || a == "-" )) {
		downstack.pop();
		downstack.push("AE'");
		downstack.push("T");
		downstack.push("AOP");
		cout << "AE'--AOP T AE'" << endl;
		printLink(26, 35, "AE'", "AOP");
		printLink(26, 27, "AE'", "T");
		printLink(26, 26, "AE'", "AE'");
		//VNCOUNT[26].pop();//原始值用后要释放 递归调用
		return true;
	}

	//T--FT'     FIRST(F)= {  (  ID NUM  DECIMAL }
	if (X == "T" && (a == "(" || a == "id" || a == "num" || a == "decimal")  ) {
		downstack.pop();
		downstack.push("T'");
		downstack.push("F");
		cout << "T-- F T'" << endl;
		printLink(27, 29, "T", "F");
		printLink(27, 28, "T", "T'"); VNCOUNT[27].pop();//原始值用后要释放
		return true;
	}

	//T'--MOP FT'
	if (X == "T'" && (a == "*" || a == "/" )) {
		downstack.pop();
		downstack.push("T'");
		downstack.push("F");
		downstack.push("MOP");
		cout << "T'--MOP F T'" << endl;
		printLink(28, 36, "T'", "MOP");
		printLink(28, 29, "T'", "F");
		printLink(28, 28, "T'", "T'"); 
		//VNCOUNT[28].pop();//原始值用后要释放
		return true;
	}

	//T'--EPS   FOLLOW(T’)={ + -        ;   )  ]  ,          <   >   <=   >=               ==  !=    &&    ||   }
	if (X == "T'" && (a == "<" || a == ">" || a == "<=" || a == ">="
		|| a == "==" || a == "!=" || a == "&&" || a == "||"
		|| a == ";" || a == ")" || a == "]" || a == ","
		|| a == "+" || a == "-" 
		)) {
		downstack.pop();
		cout << "T'--EPS" << endl;
		printLink(28, 68, "T'", "eps");
		VNCOUNT[28].pop();//原始值用后要释放
		return true;
	}
	//F--id F'
	if (X == "F" && (a == "id")) {
		downstack.pop();
		downstack.push("F'");
		downstack.push("id");
		cout << "F--id F'" << endl;
		printLink(29, 38, "F", "id");
		printLink(29, 30, "F", "F'"); VNCOUNT[29].pop();//原始值用后要释放
		return true;
	}
	//F--NUM
	if (X == "F" && (a == "num")) {
		downstack.pop();
		downstack.push("num");
		cout << "F--num" << endl;
		printLink(29, 39, "F", "num"); VNCOUNT[29].pop();//原始值用后要释放
		return true;
	}
	//F--DECIMAL
	if (X == "F" && (a == "decimal")) {
		downstack.pop();
		downstack.push("decimal");
		cout << "F--decimal" << endl;
		printLink(29, 30, "F", "decimal"); VNCOUNT[29].pop();//原始值用后要释放
		return true;
	}
	//F--( E )
	if (X == "F" && (a == "(")) {
		downstack.pop();
		downstack.push(")");
		downstack.push("E");
		downstack.push("(");
		cout << "F--( E )" << endl;
		printLink(29, 52, "F", "(");
		printLink(29, 20, "F", "E");
		printLink(29, 53, "F", ")"); VNCOUNT[29].pop();//原始值用后要释放
		return true;
	}

	//  F'--EPS    FOLLOW(F’)={ * /  + - ;   )  ]  ,    <   >   <=   >=     ==   !=    &&    ||  } 
	if (X == "F'" && (a == "<" || a == ">" || a == "<=" || a == ">="
		|| a == "==" || a == "!=" || a == "&&" || a == "||"
		|| a == ";" || a == ")" || a == "]" || a == ","
		|| a == "+" || a == "-" || a=="*"   || a=="/"
		)) {
		downstack.pop();
		cout << "F'--EPS" << endl;
		printLink(30, 68, "F'", "eps");
		VNCOUNT[30].pop();//原始值用后要释放
		return true;
	}
	//F'-- [  E  ]
	if (X == "F'" && (a == "[")) {
		downstack.pop();
		downstack.push("]");
		downstack.push("E");
		downstack.push("[");
		cout << "F'-- [  E  ]" << endl;
		printLink(30, 50, "F'", "[");
		printLink(30, 20, "F'", "E");
		printLink(30, 51, "F'", "]"); VNCOUNT[30].pop();//原始值用后要释放
		return true;
	}

	//F'--(As)
	if (X == "F'" && (a == "(")) {
		downstack.pop();
		downstack.push(")");
		downstack.push("AS");
		downstack.push("(");
		cout << "F'-- ( AS )" << endl;
		printLink(30, 52, "F'", "(");
		printLink(30, 31, "F'", "AS");
		printLink(30, 53, "F'", ")"); VNCOUNT[30].pop();//原始值用后要释放
		return true;
	}
	//As--AL  FIRST(A-L)= {  (  ID NUM  DECIMAL }
	if (X == "AS" && (a == "(" || a == "id" || a == "num" || a == "decimal")) {
		downstack.pop();
		downstack.push("AL");
		cout << "AS--AL" << endl;
		printLink(31, 32, "AS", "AL"); VNCOUNT[31].pop();//原始值用后要释放
		return true;
	}
	//As--EPS
	if (X == "AS" && (a == ")")) {
		downstack.pop();
		cout << "AS--EPS" << endl;
		printLink(31, 68, "AS", "eps");
		VNCOUNT[31].pop();//原始值用后要释放
		return true;
	}
	//AL--E AL'  FIRST(E)= {  (  ID NUM  DECIMAL }
	if (X == "AL" && (a == "(" || a == "id" || a == "num" || a == "decimal")) {
		downstack.pop();
		downstack.push("AL'");
		downstack.push("E");
		cout << "AL--E AL'" << endl;
		printLink(32, 20, "AL", "E");
		printLink(32, 33, "AL", "AL'"); VNCOUNT[32].pop();//原始值用后要释放

		return true;
	}

	//AL'-- , E AL'
	if (X == "AL'" && (a == ",")) {
		downstack.pop();
		downstack.push("AL'");
		downstack.push("E");
		downstack.push(",");
		cout << "AL'--  ,  E  AL' " << endl;
		printLink(33, 49, "AL'", ",");
		printLink(33, 20, "AL'", "E");
		printLink(33, 33, "AL'", "AL'"); 
		//VNCOUNT[33].pop();//原始值用后要释放 递归调用

		return true;
	}
	//AL'--EPS
	if (X == "AL'" && (a == ")")) {
		downstack.pop();
		cout << "AL'-- EPS" << endl;
		printLink(33, 68, "AL'", "eps");
		VNCOUNT[33].pop();//原始值用后要释放
		//fout << "AL'-- EPS" << endl;
		return true;
	}

	//ROP
	if (X == "ROP") {
		downstack.pop();
		if(a=="<")
			downstack.push("<");
		if (a == ">")
			downstack.push(">");
		if (a == "<=")
			downstack.push("<=");
		if (a == ">=")
			downstack.push(">=");
		if (a == "==") {
			downstack.push("==");
		}
			
		if (a == "!=")
			downstack.push("!=");
		if (a == "&&")
			downstack.push("&&");
		if (a == "||")
			downstack.push("||");
		cout << "ROP" << endl;
		//fout << "ROP" << endl;
		return true;
	}

	//MOP
	if (X == "MOP") {
		downstack.pop();
		if (a == "*") {
			downstack.push("*");
			cout << "MOP -- *" << endl;
			//fout << "MOP -- *" << endl;
		}
			
		if (a == "/") {
			downstack.push("/");
			cout << "MOP -- /" << endl;
			//fout << "MOP -- /" << endl;
		}
		return true;
	}

	//AOP
	if (X == "AOP") {
		downstack.pop();
		if (a == "+") {
			downstack.push("+");
			cout << "AOP -- +" << endl;
			//fout << "AOP -- +" << endl;
		}		
		if (a == "-") {
			downstack.push("-");
			cout << "AOP -- -" << endl;
			//fout << "AOP -- -" << endl;
		}
		return true;
	}

	//TS
	if (X == "TS") {
		downstack.pop();
		if (a == "int"){
			downstack.push("int");
			cout << "TS -- int" << endl;
			printLink(37, 41, "TS", "int"); VNCOUNT[37].pop();//原始值用后要释放
			//fout << "TS -- int" << endl;
		}
			
		if (a == "void") {
			downstack.push("void");
			cout << "TS -- void" << endl;
			printLink(37, 42, "TS", "void"); VNCOUNT[37].pop();//原始值用后要释放
			//fout << "TS -- void" << endl;
		}
			
		if (a == "float") {
			downstack.push("float");
			cout << "TS -- float" << endl;
			printLink(37, 43, "TS", "float"); VNCOUNT[37].pop();//原始值用后要释放
			//fout << "TS -- float" << endl;
		}
		return true;
	}
	return false;
}


int main()
{
	FILE * inputfile;
	if ((inputfile = fopen("LA.txt", "r")) == NULL) {
		cout << "File doesn't exist!" << endl;
		system("pause");
		return 0;
	}
	else {		
		fout.open("Expression.txt");
		fout << "digraph g{" << endl;
		fout << "node" << 0 << "[label=\"" << "PR" << "\"]" << endl;
		QueueInitialize(inputfile);//初始化输入串队列
		GrammarAnalysis();//词法分析
		fout << "}" << endl;
		fclose(inputfile);
		fout.close();		
	}	
	system("pause");
	return 0;
}