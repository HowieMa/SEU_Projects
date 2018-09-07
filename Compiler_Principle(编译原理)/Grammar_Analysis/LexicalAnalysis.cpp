#include "stdio.h"
#include "string.h"
#include<stdlib.h>
#include<iostream>
using namespace std;

#define MAX_STR 256
#define MAX_KEYWORDS 62
#define MAX_OPERATORS 12
#define MAX_SEPARATORS 15

#define STA_START 1		  /**/
#define STA_IDorKEYWORD 2 /*   IDENTIFIER： 标识符   */
#define STA_NUMBER 3      /*   NUMBER：     数字     */
#define STA_NOTE 4        /*   NOTE:        注释     */
#define STA_CONSTANT 5    /*   CONSTANT:    常量     */
#define STA_DONE  6       /*   DONE：       完成     */

#define TYPE_KEYWORD 1    /*   KEYWORD：    保留字   */
#define TYPE_IDENTIFIER 2 /*   IDENTIFIER： 标识符   */
#define TYPE_NUMBER 3     /*   NUMBER：     数字     */
#define TYPE_NOTE 4       /*   NOTE:        注释     */
#define TYPE_CONSTANT 5   /*   CONSTANT:    常量     */
#define TYPE_OPERATOR 6   /*   OPERATOR:    运算符   */
#define TYPE_SEPARATOR 7  /*   SEPARATOR:   分隔符   */
#define TYPE_ERROR 8      /*   ERROR:       错误     */
#define TYPE_UNKNOWN 9    /*   UNKNOWN:     未知     */
#define TYPE_ENDFILE 10   /*   ENDFILE:     文件结束 */
#define MAX_GRAMMAR 29

char *Operators[MAX_OPERATORS] = { "+","-","*","/","%","=","==","!=","<","<=",">",">=" };
char *Separators[MAX_SEPARATORS] = { ",",";",".","\'","\"","(",")","[","]","{","}","//","/*","*/","#" };
char *Keywords[MAX_KEYWORDS] = { "include","define","auto","bool","break","case","catch","char","class",
"const","const_cast","continue","default","delete","do","double",
"dynamic_cast","else","enum","explicit","extern","false","float","for",
"friend","goto","if","inline","int","long","mutable","namespace","new",
"operator","private","protected","public","register","reinterpret_cast",
"return","short","signed","sizeof","static","static_cast","struct",
"switch","template","this","throw","true","try","typedef","typeid",
"typename","union","unsigned","using","virtual","void","volatile","while" };

char *Grammar[MAX_GRAMMAR] = { "int","void","float","if","else","while","return",
";",",","(",")","[","]","{","}","=","+","-","*","/","<",">","<=",">=","==","!=","&&","||","#"};


bool IsOperator(char c);
bool IsSeparator(char c);
bool IsKeyWord(char *str);
bool IsAlpha(char c);
bool IsNum(char c);
bool IsGrammar(char* str);
void OutputOneDone(FILE*outf, int type, char * str);
void LexAnalyse(FILE *inf, FILE *outf);

int main()
{
	FILE * inputfile, *outputfile;
	if ((inputfile = fopen("test.txt", "r")) == NULL){
		cout << "File doesn't exist!" << endl;
		system("pause");
		return 0;
	}
	else{
		outputfile = fopen("LA.txt", "w");
		LexAnalyse(inputfile, outputfile);
		fclose(inputfile);
		fclose(outputfile);
		cout << "Finish lexis analysis" << endl;
	}
	system("pause");
	return 0;
}

bool IsOperator(char c)
{
	for (int i = 0; i < MAX_OPERATORS; i++)
		if (Operators[i][0] == c)
			return true;
	return false;
}

bool IsSeparator(char c)
{
	for (int i = 0; i < MAX_SEPARATORS; i++)
		if (Separators[i][0] == c)
			return true;
	return false;
}

bool IsKeyWord(char * str)
{

	for (int i = 0; i < MAX_KEYWORDS; i++){
		if (strcmp(Keywords[i], str) == 0)
			return true;
	}
	return false;
}

bool IsAlpha(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z'))
		return true;
	return false;
}

bool IsNum(char c)
{
	if (c >= '0'&&c <= '9')
		return true;
	return false;
}

bool IsGrammar(char* str)
{
	for (int i = 0; i < MAX_GRAMMAR; i++) {
		
		if (strcmp(Grammar[i], str) == 0)
			return true;
	}
	return false;

}

void OutputOneDone(FILE * outf, int type, char * str)
{
	if (strcmp(str, "=") == 0)
		cout << str << "等号" << endl;
	if (IsKeyWord(str))
		type = TYPE_KEYWORD;
	
	if(IsGrammar(str))	
		fprintf(outf, "%s\n", str);
	switch (type){
	//case TYPE_KEYWORD:    fprintf(outf, "       KEYWORD:     "); break;
	//case TYPE_IDENTIFIER: fprintf(outf, "       IDENTIFIER:  "); break;
	//case TYPE_NUMBER:     fprintf(outf, "       NUMBER:      "); break;
	case TYPE_IDENTIFIER: fprintf(outf, "ID\n"); break;
	case TYPE_NUMBER: {
		if(strchr(str,'.'))
			fprintf(outf, "DECIMAL\n"); 
		else
			fprintf(outf, "NUM\n");
		break;
	}
		
	//case TYPE_NOTE:       fprintf(outf, "       NOTE:        "); break;
	//case TYPE_CONSTANT:   fprintf(outf, "       CONSTANT:    "); break;
	//case TYPE_OPERATOR:   fprintf(outf, "       OPERATOR:    "); break;
	//case TYPE_SEPARATOR:  fprintf(outf, "       SEPARATOR:   "); break;
	//case TYPE_ERROR:      fprintf(outf, "       ERROR:       "); break;
	//case TYPE_UNKNOWN:    fprintf(outf, "       UNKNOWN:     "); break;
	default:break;
	}
	//fprintf(outf, "%s\n", str);
}

void LexAnalyse(FILE * inf, FILE * outf)
{
	char ch;
	char str[MAX_STR] = "";
	int i, line_num = 1;
	int state, type;

	char flag_noteline_one = 0;
	char flag_dot = 0;
	//fprintf(outf, "Line %d************************\n", line_num);
	while (!feof(inf))
	{

		i = 0;
		state = STA_START; //innitially state
		flag_noteline_one = 0;
		flag_dot = 0;

		while (state != STA_DONE) //still search
		{
			ch = fgetc(inf); //get current char
			switch (state)
			{

			case STA_START:
				if (ch == ' ' || ch == '\t')
				{

				}
				else if (ch == '\n')
				{
					line_num++;
					//fprintf(outf, "Line %d************************\n", line_num);

				}
				else if (IsAlpha(ch))
				{
					state = STA_IDorKEYWORD;
					type = TYPE_IDENTIFIER;
					str[i] = ch;
					++i;
				}
				else if (IsNum(ch))
				{
					state = STA_NUMBER;
					type = TYPE_NUMBER;
					str[i] = ch;
					++i;
				}
				else if (ch == '/')
				{
					str[i] = ch;
					i++;
					ch = fgetc(inf);
					if (ch == '/') //note //
					{
						flag_noteline_one = 1;
						state = STA_NOTE;
						type = TYPE_NOTE;
						str[i] = ch;
						i++;

					}
					else if (ch == '*')// note /*
					{
						state = STA_NOTE;
						type = TYPE_NOTE;
						str[i] = ch;
						i++;
					}
					else //divide operator like / in  1/3
					{
						state = STA_DONE;
						type = TYPE_OPERATOR;
						fseek(inf, -1, SEEK_CUR);
						i = 1;
						str[1] = '\0';
					}

				}
				else if (ch == '<' || ch == '>')
				{
					state = STA_DONE;
					type = TYPE_OPERATOR;
					str[0] = ch;
					ch = fgetc(inf);
					if (ch == '=')
					{
						str[1] = ch;
						str[2] = '\0';
						i = 2;
					}
					else
					{
						fseek(inf, -1, SEEK_CUR);
						i = 1;
					}
				}
				else if (ch == '!')
				{
					state = STA_DONE;
					str[0] = ch;
					ch = fgetc(inf);
					if (ch == '=')
					{
						type = TYPE_OPERATOR; //  recognize !=
						str[1] = '=';
						str[2] = '\0';
						i = 2;
					}
					else
					{
						type = TYPE_UNKNOWN;
						fseek(inf, -1, SEEK_CUR);
						i = 1;

					}
				}
				else if (ch == '\"' || ch == '\'')
				{
					state = STA_CONSTANT;
					type = TYPE_CONSTANT;
				}
				else if (IsOperator(ch))
				{
					state = STA_DONE;
					type = TYPE_OPERATOR;
					str[0] = 0;
					i = 1;
				}
				else if (IsSeparator(ch))
				{
					state = STA_DONE;
					type = TYPE_SEPARATOR;
					str[0] = ch;
					i = 1;
				}
				else if (ch == EOF)
				{
					state = STA_DONE;
					type = TYPE_ENDFILE;
				}
				else
				{
					state = STA_DONE;
					type = TYPE_UNKNOWN;
				}
				break; //end of case state STA_START


			case STA_IDorKEYWORD:
				if (IsAlpha(ch)) //start with aplha
				{
					str[i] = ch;
					i++;
				}
				else if (ch == '.')//such as a.b
				{
					str[i] = ch;
					i++;
					ch = fgetc(inf);
					if (IsAlpha(ch))
					{
						str[i] = ch;
						i++;
					}
					else if (ch == ' ' || ch == '\t' || ch == '\n' || IsOperator(ch) || IsSeparator(ch))
					{
						fseek(inf, -1, SEEK_CUR);
						state = STA_DONE;
						type = TYPE_ERROR; //like a.   a.1  
					}
					else
					{
						str[i] = ch;
						i++;
						type = TYPE_ERROR;
					}
				} //end of ch=='.'
				else if (ch == ' ' || ch == '\t' || ch == '\n' || IsOperator(ch) || IsSeparator(ch))
				{
					state = STA_DONE;
					fseek(inf, -1, SEEK_CUR);
				}
				else
				{
					state = STA_DONE;
					type = TYPE_ERROR;
				}
				break;
			case STA_NUMBER:
				if (IsNum(ch))
				{
					str[i] = ch;
					i++;
				}
				else if (ch == '.')
				{
					str[i] = ch;
					i++;
					ch = fgetc(inf); //next char
					if (flag_dot == 0)
					{
						if (IsNum(ch)){
							str[i] = ch;
							i++;
							flag_dot = 1;
						}
						else{
							fseek(inf, -1, SEEK_CUR);
							state = STA_DONE;
							type = TYPE_ERROR;
						}
					}
					else{
						type = TYPE_ERROR;
						str[i] = ch;
						i++;
					}
				}//end of ch=='.'
				else if (ch == ' ' || ch == '\t' || ch == '\n' || IsOperator(ch) || IsSeparator(ch)){
					state = STA_DONE;
					fseek(inf, -1, SEEK_CUR);
				}
				else{
					type = TYPE_ERROR;
					str[i] = ch;
					i++;
				}
				break;
			case STA_NOTE:
				if (flag_noteline_one == 1) { // note //
					if (ch == '\n'){
						state = STA_DONE;
						fseek(inf, -1, SEEK_CUR);
					}
					else{
						str[i] = ch;
						i++;
					}
				}//end of flag==1
				else {// note /*
					if (feof(inf)){
						state = STA_DONE;
						type = TYPE_ERROR;
					}
					else if (ch == '\n'){
						line_num++;
						str[i] = ch;
						i++;
					}
					else if (ch == '*'){
						str[i] = ch;
						i++;
						ch = fgetc(inf);
						if (ch == '/') {
							state = STA_DONE;
							str[i] = ch;
							i++;
						}
						else{
							if (feof(inf)){
								state = STA_DONE;
								type = TYPE_ERROR;
							}
							fseek(inf, -1, SEEK_CUR);
						}
					} //end of ch==*
					else{
						str[i] = ch;
						i++;
					}
				}
				break;
			case STA_CONSTANT:
				if (feof(inf)){
					state = STA_DONE;
					type = TYPE_ERROR;
				}
				else if (ch == '\n'){
					line_num++;
					str[i] = ch;
					i++;
				}
				else if (ch == '\"' || ch == '\''){
					state = STA_DONE;
					str[i] = ch;
					i++;
				}
				else if (ch == '\\'){
					str[i] = ch;
					i++;
					ch = fgetc(inf);
					str[i] = ch;
					i++;
				}
				else{
					str[i] = ch;
					i++;
				}
				break;
			case STA_DONE:
				break;
			default:
				break;
			}//END OF switch
		}//end of inner while
		str[i] = '\0';//end of string
		OutputOneDone(outf, type, str);

	}//end of out while
}//end of function
