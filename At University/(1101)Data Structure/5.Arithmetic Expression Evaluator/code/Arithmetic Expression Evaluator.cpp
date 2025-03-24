//10927207 蒲品憶 10927234 吳凱鈺
// 如果無法執行 請在上面 點工具>編譯器選項>編譯器設定>程式碼產生>語言標準>改ISO C++ 11 
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <sstream>
 
using namespace std;

class Stack {
	public:
	struct Formula {
		string fml;
		Formula*next;
	};
	typedef Formula*ForPtr ;
	ForPtr formula, opera, walk, walkopera , insert, takeout;
	
	void TakeOutOpera() ; // 印出優先權較大的符號並刪除 
	void AddInformula() ; // 新增一塊記憶體 
	void Run() ; // 原本放main裡的東西 
	int Postfix( string temp ) ; // (任務二) 中序轉後序 
	int FindError( string temp ) ; // (任務一) 找運算式錯誤 
	bool Compare( string before, string after ); // 找後進優先前是否比先進小 
	void CalulateTotal() ; // (任務三) 算出答案  
};


int main() {
	Stack stack ;
	stack.Run() ; 
} // main() 


// (任務一) 找運算式錯誤 
int Stack::FindError( string temp ) {
	int left = 0, right = 0, sum = 0 ;
	for ( int i = 0; i < temp.size() ; i++ )  {
		if ( !isdigit(temp[i]) && temp[i] != '+' && temp[i] != '-' 
			&& temp[i] != '*' && temp[i] != '/' && temp[i] != '(' && temp[i] != ')' ) {
			cout << "Error 1: "<< temp[i] << " is not a legitimate character." << endl ; 
			return 1 ; // Error1: temp[i] is not a legitimate character.
		} // if
		
		if ( temp[i] == '(' || temp[i] == ')' ) { // 判斷括號成雙 
			if ( temp[i] == '(' ) left++ ;
			else right++ ; 
			if ( right > left ) return 21 ; // 判斷是否有多 右括號
			 // Error 2: there is one extra close parenthesis.
		} // if
		if ( temp[0] == '+' || temp[0] == '-' || temp[0] == '*' || temp[0] == '/' )	
			return 31 ; // Error 3: there is one extra operator.
		if ( i+1 < temp.size() ) {
			if ( temp[i] == '(' && !isdigit(temp[i+1]) && temp[i+1] != '(' ) // 左括號右邊不為數字 
				return 32 ; // Error 3: it is not infix in the parentheses.
			if ( temp[i] == ')' && isdigit(temp[i+1]) )	// 右括號右邊為數字 
				return 33 ; // Error 3: there is one extra operand.			
			if ( temp[i] == ')' && temp[i+1] == '(' )	// 右括號右邊為左括號 
				return 33 ; // Error 3: there is one extra operand.				
			if ( temp[i+1] == '(' && isdigit(temp[i]) )	// 數字右邊為左括號 
				return 33 ; // Error 3: there is one extra operand.
			if ( ( temp[i] == '+' || temp[i] == '-' || temp[i] == '*' 
				|| temp[i] == '/' ) && temp[i+1] == ')' )	// *+-/ 右邊為括號 
				return 32 ; // Error 3: it is not infix in the parentheses.
			if ( ( temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' ) 
				&& (temp[i+1] == '+' || temp[i+1] == '-'|| temp[i+1] == '*' || temp[i+1] == '/') )	// *+-/ 右邊為 *+-/
				return 31 ; // Error 3: there is one extra operator.
		} // if
		else if ( i == temp.size()-1 ) {
			if ( temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' ) 
				return 31; // Error 3: there is one extra operator.
		} // else if
	} // for

	
	sum = left - right ;
	if ( sum != 0 ) return 22 ; // 判斷是否有多 左括號 
	// Error 2: there is one extra open parenthesis.
} // Stack::FindError()


// 找後進優先前是否比先進小 
bool Stack::Compare( string before, string after ) {
	if ( ( before == "+" || before == "-" ) && ( after == "*" || after == "/" ) ) {
		return true; // 乘除(先進)的優先權大於加減(後進) 
	} // if
	else if ( ( before == "+" || before == "-" ) && ( after == "+" || after == "-" ) ) {
		return true; // 加減(先進)的優先權大於加減(後進) 
	} // else if
	else if ( ( before == "*" || before == "/" ) && ( after == "*" || after == "/" ) ) {
		return true; // 乘除(先進)的優先權大於乘除(後進) 
	} // else if
	else return false;
} //  Stack::Compare()


// (任務二) 中序轉後序 
int Stack::Postfix( string temp ) {
	walkopera = NULL;
	insert = NULL;
	opera = NULL ;
	walk = NULL ; // 跑 formula
	for ( int i = 0; i < temp.size() ; i++ ) {
		
		if ( isdigit(temp[i]) ) { // 是數字 
			if ( formula != NULL ) cout << ", " ;
			AddInformula() ;
			while ( isdigit(temp[i]) ) { // 讀入完整數字 
				cout << temp[i] ;
				if ( walk->fml == "\0" ) walk->fml = temp[i];
				else walk->fml = walk->fml + temp[i];
				i++;
			} // if
			
			i--;
		} // if
		else { // 是加減乘除或括號 
			insert = new Formula;
			insert->fml = temp[i];
			insert->next = opera ;
			opera = insert ;
			if ( opera->fml == "(" );
			else if ( opera->next == NULL ) ;
			else if ( opera->fml != ")" ) {
		    	while ( opera != NULL && opera->next != NULL && Compare( opera->fml, opera->next->fml ) ) {
		    		TakeOutOpera();
				} // while
			} // else if
			else if ( opera->fml == ")" ) { // 右括號 
				walkopera = opera;
				while ( walkopera->fml != "(" ) { // 印出直到遇到左括號 
					if ( walkopera->fml != ")" )  {
						cout << ", " << walkopera->fml;
						AddInformula() ; 
						walk->fml = walkopera->fml;
					} // if
					walkopera = walkopera->next;
				} // while
					
				if ( walkopera->next == NULL ) opera = NULL; // 只有一個符號 
				else {
					opera = walkopera->next ;
					walkopera->next = NULL ;
					delete walkopera ;
				} // else
			} // else if
			
		} // else 
	} // for
	walkopera = opera;
	while ( walkopera != NULL ) { // 印出最後剩下的符號 
		if ( walkopera->fml != "(" && walkopera->fml != ")" ) {
			cout << ", " << walkopera->fml;
			AddInformula() ; 
			walk->fml = walkopera->fml ;	
		} // if
		walkopera = walkopera->next;
	} // whie 
	delete opera;

} // Stack::Postfix( string temp )


// 印出優先權較大的符號並刪除 
void Stack::TakeOutOpera() { 
	cout << ", " << opera->next->fml ;	
	takeout = opera->next ;
	if ( opera->next->next == NULL ) opera->next = NULL;
	else opera->next = opera->next->next ;
	takeout->next = NULL;
	
	AddInformula() ; 
	walk->fml = takeout->fml ;
	delete takeout ;
} // Stack::TakeOutOpera()


// 新增一塊記憶體
void Stack::AddInformula() {
	if ( formula == NULL ) {
		walk = new Formula;
		walk->next = NULL ;
		formula = walk ;
	} // if
	else {
		walk->next = new Formula;
		walk = walk->next ;	
		walk->next = NULL ;
	} // else
} // Stack::AddInformula()


// 原本放main裡的東西 
void Stack::Run() {
	string num ;
	int error = 0 ; // 第幾個錯誤 
	bool exe = true;
	
	while ( exe ) {
		error = 0;
		formula = NULL;
		cout << "\n* Arithmetic Expression Evaluator *" ;
		cout << "\n* 0. QUIT                         *" ;
		cout << "\n* 1. Infix2postfix Evaluation     *" ;
		cout << "\n***********************************" ;
		cout << "\nInput a choice(0, 1): " ;
		getline( cin, num ) ;
		cout << endl ;

		if ( num == "1" ) {
			string str, temp ;
			cout << "Input an infix expression: " ;
			getline( cin, str ) ;
			for ( int i = 0; i < str.size() ; i++ ) 
				if ( str[i] != ' ' ) temp = temp + str[i] ; // 忽略空格 
			
			if ( temp[0] != '\0' ) error = FindError( temp ) ;
			else error = 31 ;
			if ( error == 21 ) cout << "Error 2: there is one extra close parenthesis." << endl;
			else if ( error == 22 ) cout << "Error 2: there is one extra open parenthesis." << endl;
			else if ( error == 31 ) cout << "Error 3: there is one extra operator." << endl;
			else if ( error == 32 ) cout << "Error 3: it is not infix in the parentheses." << endl;
			else if ( error == 33 ) cout << "Error 3: there is one extra operand." << endl;
			else if ( error == 0 ) {
				cout << "It is a legitimate infix expression." << endl;
				cout << "Postfix expression: " ;
				Postfix( temp ) ;
				CalulateTotal();	
			} // else if
			
			
			if ( formula != NULL ) delete formula ;	
		} // if
	
		else if ( num == "0" ) exe = false ;
		
	} // while
} // Stack::Run()


// (任務三) 算出答案  
void Stack::CalulateTotal() {
	walk = NULL, opera = NULL, walkopera = NULL ;
	bool divisorZore = false ; // 除數是否為零 
	walkopera = new Formula ;
	walk = formula ;
	int a = 0, b = 0;
	while ( walk != NULL ) {
		if ( isdigit(walk->fml[0]) ) {
			if ( opera == NULL ) { // 給opera一塊記憶體 
				opera = walkopera;
				opera->next = NULL ;
			} // if
			
			else { // 新增一塊記憶體在opera最前面 
				walkopera = new Formula;
				walkopera->next = opera;
				opera = walkopera;
			} //else 
			
			walkopera->fml = walk->fml;
		} // if
		else {
			a = stoi(opera->next->fml) ; // 先讀進去opera的數 
			b = stoi(opera->fml) ; // 後讀進去opera的數  
			
			if ( walk->fml == "+" ) a = a + b;
			else if ( walk->fml == "-" ) a = a - b;
			else if ( walk->fml == "*" ) a = a * b;
			else if ( walk->fml == "/" ) {
				if ( b == 0 ) { // 除數為零 
					cout << "\n### Error: Divided by ZERO! ### " << endl;
					cout << "### It cannot be successfully evaluated! ###" << endl;
					divisorZore = true ;
				} // else 
				else a = a / b;
			} // else 
			 
			if ( divisorZore == false ) {
				opera->next->fml = to_string(a);
				walkopera = opera;
				opera = opera->next;
				walkopera->next = NULL;
				delete walkopera;					
			} // if
		} // else 
		
		walk = walk->next;
		if ( divisorZore == true ) walk = NULL ; 
	} // while
	if ( divisorZore == false ) cout << "\nAnswer: " << a << endl;
	delete opera ;
} // Stack::CalulateTotal()
