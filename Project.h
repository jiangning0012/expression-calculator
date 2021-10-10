#pragma once
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<cstring>
#include<string>
#include"Expression.h"

using namespace std;

const int MaxVarNum=100;
const int MaxFuncNum=100;
int f[4][1<<7];
#define ORI 0
#define NUM 1
#define OPR 2
#define YKH 3
#define FAIL -1
// void Delete_Blank(string line,int len)
// {
// 	int i,j;
// 	for(i=0,j=0;i<len;i++)
// 	{
// 		if(line[i]!=' ' && line[i]!='\t')
// 			line[j++]=line[i];
// 	}
// 	line[j]=0;
// }
void Init()
{
	memset(f,-1,sizeof(f));
	f[ORI]['+']=f[ORI]['-']=OPR;
	f[NUM]['+']=f[NUM]['-']=f[NUM]['*']=f[NUM]['/']=OPR;
	for(int i='0';i<='9';i++)
		f[ORI][i]=f[NUM][i]=f[OPR][i]=NUM;
    for(int i='a';i<='z';i++)
		f[ORI][i]=f[NUM][i]=f[OPR][i]=NUM;
    for(int i='A';i<='Z';i++)
		f[ORI][i]=f[NUM][i]=f[OPR][i]=NUM;
    f[ORI]['.']=f[NUM]['.']=f[OPR]['.']=NUM;
	f[ORI]['(']=f[OPR]['(']=YKH;
}
int Find_Next_Pos(string line,int x,int y)
{
	int num=1;
	for(int i=x;i<y;i++)
	{
		if(line[i]=='(')
			num++;
		else if(line[i]==')')
			num--;
		if(num==0)
			return i;
	}
	return -1;
}
bool IsExpression(string line,int x,int y)
{
	int state=ORI;
	for(int i=x;i<y;i++)
	{
		state=f[state][line[i]];
		if(state==YKH)
		{
			int next=Find_Next_Pos(line,i+1,y);
			if(next!=-1 && IsExpression(line,i+1,next))
				state=NUM,i=next;
			else return false;
		}
		else if(state==FAIL)
			return false;
	}
    for (int i=0;i<line.length()-1;i++){
        if (((line[i]>='A'&&line[i]<='Z')||(line[i]>='a'&&line[i]<='z'))&&(line[i+1]=='.'||(line[i+1]>='0'&&line[i+1]<='9'))){
            return false;
        }
    }
    for (int i=1;i<line.length();i++){
        if (((line[i+1]>='A'&&line[i+1]<='Z')||(line[i+1]>='a'&&line[i+1]<='z'))&&(line[i]=='.'||(line[i]>='0'&&line[i]<='9'))){
            return false;
        }
    }
    for (int i=0;i<line.length();i++){
        if (line[i]=='.'){
            if (!(line[i-1]>='0'&&line[i-1]<='9'&&line[i+1]>='0'&&line[i+1]<='9')) return false;
        }
    }
	return state==NUM;
}

struct Var{
    string name;
    string value;
};

struct Func{
    string name;
    Var varlist[MaxVarNum];
    int varlistlen;
    string expr;
};

class Project{
private:
    Var varlist[MaxVarNum];
    Func funclist[MaxFuncNum];
    int pvar,pfunc;
public:
    Project();
    void Welcome();
    void Judge(string command);
    void create_var(string command);
    void create_func(string command);
    void Assigning(string command);
    void Calculate(string command);
    void show_vars();
    void show_funcs();
    bool var_repeated_define(string var);
    bool func_repeated_define(string func);
    bool check_illegal_ch(string command);
    //bool check_undefined_var(string command);
};

// bool Project::check_undefined_var(string command){
//     for (int i=0;i<pvar;i++){
//         if (command.find(varlist[i].name)!=string::npos){
//             return false;
//         }
//     }
//     for (int i=0;i<command.length();i++){
//         if ((command[i]>='a'&&command[i]<='z')||(command[i]>='A'&&command[i]<='Z')){
//             return true;
//         }
//     }
//     return false;
// }

void Project::show_vars(){
    cout<<"All variables are shown below:"<<endl;
    for (int i=0;i<pvar;i++){
        cout<<varlist[i].name<<"="<<varlist[i].value<<endl;
    }
}

void Project::show_funcs(){
    cout<<"All functions are shown below:"<<endl;
    for (int i=0;i<pfunc;i++){
        cout<<funclist[i].name<<"(";
        cout<<funclist[i].varlist[0].name;
        for (int j=1;j<funclist[i].varlistlen;j++){
            cout<<","<<funclist[i].varlist[j].name;
        }
        cout<<")="<<funclist[i].expr<<endl;
    }
}

Project::Project(){
    memset(varlist,0,sizeof(varlist));
    memset(funclist,0,sizeof(funclist));
    pvar=pfunc=0;
}

void Project::Welcome(){
    cout<<"----------------------------------------------------"<<endl;
    cout<<"--------------------Proj 4--------------------------"<<endl;
    cout<<"------------------------NJU-jiangning---------------"<<endl;
    cout<<"----------------------------------------------------"<<endl<<endl<<endl;
    string command;
    while (true){
        cout<<"---> ";
        getline(cin,command);
        if (command=="exit") return;
        if (command=="") continue;
        Judge(command);
    }
}

void Project::Judge(string command){
    if (check_illegal_ch(command)){
        cout<<"Error: illegal input"<<endl;
    }
    else if (command.find("Var ")==0){
        create_var(command);
    }
    else if (command.find("Func ")==0){
        create_func(command);
    }
    else if (command.find("SHOW VAR")==0){
        show_vars();
    }
    else if (command.find("SHOW FUNC")==0){
        show_funcs();
    }
    else if (command.find("=")!=string::npos){
        Assigning(command);
    }
    else  Calculate(command);
}

void Project::create_var(string command){
    if (command.find("=")==string::npos){
        cout<<"Error: illegal input"<<endl;
        return ;
    }
    int pos_equal=command.find("=");
    string new_var=command.substr(4,pos_equal-4);
    int len=command.length();
    string value=command.substr(pos_equal+1,len-pos_equal-1);
    if (var_repeated_define(new_var)){
        cout<<"Error: variable "<<new_var<<" defined twice"<<endl;
        return ;
    }
    //cout<<value<<endl;
    // Init();
    // if (!IsExpression(value,0,value.length())){
    //     cout<<"Error: expression is not legal"<<endl;
    //     return ;
    // }
    // if (check_undefined_var(command)){
    //     cout<<"Error: undefined variable exists"<<endl;
    //     return ;
    // }
    varlist[pvar].name=new_var;
    varlist[pvar++].value=value;
}

void Project::create_func(string command){
    if (command.find("=")==string::npos){
        cout<<"Error: illegal input"<<endl;
        return ;
    }
    int pos_left=command.find("(");
    int pos_right=command.find(")");
    string name=command.substr(5,pos_left-5);
    string list=command.substr(pos_left+1,pos_right-pos_left-1);
    string expr=command.substr(pos_right+2,command.length()-pos_right);
    if (func_repeated_define(name)){
        cout<<"Error: function "<<name<<" defined twice"<<endl;
        return ;
    }
    // Init();
    // if (!IsExpression(expr,0,expr.length())){
    //     cout<<"Error: expression is not legal"<<endl;
    //     return ;
    // }
    //cout<<name<<endl<<list<<endl<<expr<<endl;
    string _varlist[MaxVarNum];
    int cnt=0;
    list=list+",";
    int pre=0;
    for (int i=0;i<list.length();i++){
        if (list[i]==','){
            _varlist[cnt++]=list.substr(pre,i-pre);
            pre=i+1;
        }
    }
    funclist[pfunc].name=name;
    funclist[pfunc].expr=expr;
    funclist[pfunc].varlistlen=cnt;
    for (int i=0;i<cnt;i++){
        funclist[pfunc].varlist[i].name=_varlist[i];
    }
    pfunc++;
}

void Project::Assigning(string command){
    int pos_equal=command.find("=");
    if (command.find("(")!=string::npos){
        int pos_left=command.find("(");
        int pos_right=command.find(")");
        string name=command.substr(0,pos_left);
        string list=command.substr(pos_left+1,pos_right-pos_left-1);
        string expr=command.substr(pos_right+2,command.length()-pos_right);
        string _varlist[MaxVarNum];
        int cnt=0;
        list=list+",";
        int pre=0;
        for (int i=0;i<list.length();i++){
            if (list[i]==','){
                _varlist[cnt++]=list.substr(pre,i-pre);
                pre=i+1;
            }
        }
        int flag=0;
        for (int i=0;i<pfunc;i++){
            if (funclist[i].name==name){
                funclist[i].expr=expr;
                funclist[i].varlistlen=cnt;
                for (int j=0;j<cnt;j++){
                    funclist[i].varlist[j].name=_varlist[j];
                }
                flag=1;
                break;
            }
        }
        if (!flag){
            cout<<"Error: cannot find function "<<name<<endl;
        }
    }
    else {
        string name=command.substr(0,pos_equal);
        int len=command.length();
        string value=command.substr(pos_equal+1,len-pos_equal-1);
        int flag=0;
        for (int i=0;i<pvar;i++){
            if (varlist[i].name==name){
                varlist[i].value=value;
                flag=1;
                break;
            }
        }
        if (!flag){
            cout<<"Error: cannot find variable "<<name<<endl;
        }
    }
}

void Project::Calculate(string command){
    Expression s(command); s.run();
    s.print();
}

bool Project::var_repeated_define(string var){
    for (int i=0;i<pvar;i++){
        if (varlist[i].name==var) return true;
    }
    return false;
}

bool Project::func_repeated_define(string func){
    for (int i=0;i<pfunc;i++){
        if (funclist[i].name==func) return true;
    }
    return false;
}

bool Project::check_illegal_ch(string command){
    for (int i=0;i<command.length();i++){
        if (!((command[i]>='a'&&command[i]<='z')||(command[i]>='A'&&command[i]<='Z')||(command[i]>='0'&&command[i]<='9')||command[i]=='+'||command[i]=='-'||command[i]=='*'||command[i]=='/'||command[i]=='.'||command[i]=='('||command[i]==')'||command[i]=='='||command[i]==','||command[i]==' ')){
            return true;
        }
    }
    return false;
}
