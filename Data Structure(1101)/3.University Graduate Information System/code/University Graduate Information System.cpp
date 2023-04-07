// 蒲品憶 10927207   吳凱鈺 10927234 
#include <iostream>
#include <vector> // vector 
#include <fstream> // 讀檔 
#include <time.h> // 計算時間 
#include <conio.h> // 按任意見繼續 
#include <sstream> // istringstream stream用到 
#include <cstdlib> // atoi
#include <string>
#include <iomanip> // 使用小數點後兩位 
using namespace std ;

bool sfile = true ;

struct SchoolData {
	string sName = "" ; // 學校名稱
	string dName = "" ; // 科系名稱 
	string cRef = "" ; // 進修 
	string level = "" ; // 等級別  
	int nStud = 0 ; // 學生數
	int nTeacher = 0 ; // 教師數
	int nGrad = 0 ; // 畢業生數 
};	  

vector<SchoolData> sDataList ;
SchoolData oneR ;
int schTH = 0, graTH = 0 ;
void ReadTxt() ; // 讀檔
void Print() ;
bool isNumber(string& str) ;

class BinaryT {
	private: 
		struct TreeNode {
			SchoolData item;
			TreeNode * left;
			TreeNode * right;
		};
		
		typedef TreeNode * TreePtr ;
		TreePtr gradPtr = NULL ;
		TreePtr schoolPtr = NULL ;
		
	public: 
		void reset( TreePtr & walk ) ;
		void PrintT( TreePtr walk ) ;
		void gradTree() ;
		void schTree() ;
		void searchG() ;
		void searchS() ;
		void recurG( TreePtr walk, int & i, int num );
		void recurS( TreePtr walk, int & i, string name );
		void deleteTree();
		void recurDeleteG( TreePtr & walk, TreePtr pre, int & i, int num );
		void recurDeleteS( TreePtr & walk, TreePtr pre, int num );
		void deleteNode( TreePtr & walk, TreePtr pre );
		void deleteData( int num ) ;
		int countH( TreePtr walk );
};

int main() {
	
	BinaryT tree;
	char command ;
	
	while ( true ) {
		sfile = true ;
		cout << endl << "*** University Graduate Information System ***" ;
		cout << endl << "* 0. Quit                                    *" ;
		cout << endl << "* 1. Create Two Binary Search Trees          *" ;
		cout << endl << "* 2. Search by Number of Graduates           *" ;
		cout << endl << "* 3. Search by School Name                   *" ;
		cout << endl << "* 4. Removal by Number of Graduates          *" ;
		cout << endl << "**********************************************" ;
		cout << endl << "Input a command(0, 1-4):" ;
		cin >> command ;
		sfile = true ;
		 
		if ( command == '1' ) {
			ReadTxt() ; 
			if ( sfile == true ) {
				schTH = 0, graTH = 0 ;
				tree.gradTree() ;
				tree.schTree() ;
				cout << "Tree height {School name} = " << schTH << endl;
				cout << "Tree height {Number of graduates} = " << graTH << endl;				
			} // if
		} // if
		
		else if ( command == '2' ) {
			tree.searchG() ;
		} // eise if
		
		else if ( command == '3' ) {
			tree.searchS() ;
		} // eise if
		
		else if ( command == '4' ) {
			tree.deleteTree();
		} // else if
		
		else if ( command == '0' ) return 0 ;
		
		else cout << endl << "Command does not exist!" << endl ;
	} // while 
	
} // main()

void ReadTxt() { // 讀檔
	sDataList.clear() ;
	string num, name ;
	cout << endl << "Input a file number (e.g., 501, 502, ...): " ;
	cin >> num ;
	cout << endl ;	

	name = "input" + num + ".txt" ;  
	ifstream file ;
	file.open(name, ios_base::in) ; // 文件打開 

    if (!file.is_open()) {
        cout << "### " << name << " does not exist! ###\n" ;
        cout << endl << "There is no data!" << endl ;
        num = "" ;
        sfile = false ;
    } // if
	
    else {
		string line, temp ;
 		getline( file, line ) ;
 		getline( file, line ) ;
 		getline( file, line ) ;
		 		
		while (getline( file, line )) {
			istringstream stream(line) ;
			int count = 0 ;
    		while( stream >> temp ) {  
				if ( count == 1 ) oneR.sName = temp ;
				else if ( count == 3 ) oneR.dName = temp ;
				else if ( count == 6 ) oneR.nStud = atoi(temp.c_str()) ;
				else if ( count == 7 ) oneR.nTeacher = atoi(temp.c_str()) ;
				else if ( count == 8 ) oneR.nGrad = atoi(temp.c_str()) ;
				else if ( count == 4 ) {
					oneR.cRef = temp ;
					stream >> temp;
					oneR.cRef = oneR.cRef + " " + temp ;
				} // else if
				else if ( count == 5 ) {
					oneR.level = temp ;	
					stream >> temp;
					oneR.level = oneR.level + " " + temp ;
				} // else if	
				count++ ;
				
			} // while
			
			sDataList.push_back(oneR) ; 
			stream.clear() ;
						
		} // while
		
		file.close() ;
		
		if ( !sDataList.size()) {
			cout << "### Get nothing from the file " << name << "!###" << endl ;
			cout << endl << "There is no data!" << endl ;
			sfile = false ;
		} // end if		
	} // else	
	
	Print() ;
} // ReadTxt()

void Print() { // 印資料 
	for ( int i = 0; i < sDataList.size(); i++ ) 
		cout << "["<< i+1 << "]" << "\t" << sDataList[i].sName << " \t" << sDataList[i].dName << "\t" << sDataList[i].cRef << "\t" 
		<< sDataList[i].level << "\t" << sDataList[i].nStud << "\t" << sDataList[i].nTeacher << " \t" << sDataList[i].nGrad << endl;
} // Print()

bool isNumber(string& str) { // 字串是否為數字 
    for (char &c : str) {
        if (isdigit(c) == 0) return false;
    } // for
    return true;
} // isNumber()

void BinaryT::reset( TreePtr & walk ) { // 初始化 
	if ( walk == NULL ) return ;	
	if ( walk->left != NULL ) reset( walk->left ) ;	
   	if ( walk->right != NULL ) reset( walk->right ) ;	
   	if ( walk != NULL ) {
   		delete walk ;
		walk = NULL ;	
	} // if 
} // BinaryT::reset()

void BinaryT::PrintT( TreePtr walk ) { // 初始化 
	if ( walk == NULL ) return ;
	PrintT(walk->left);
   	PrintT(walk->right);	
   	return ;
} // BinaryT::reset()

void BinaryT::gradTree() { // 建樹 畢業生 
	int currH = 0 ;
	if ( gradPtr != NULL ) reset( gradPtr ) ;
	for ( long long int i = 0; i < sDataList.size() ; i++ ) { 
		TreePtr temp = new TreeNode;
		temp->item = sDataList[i] ;
		temp->right = NULL ;
		temp->left = NULL ;
		
		if ( gradPtr == NULL ) gradPtr = temp ;
		else {
			currH = 1;
			int k = 0 ;
			TreePtr walk = gradPtr ;
			while ( k == 0 ) {
				if ( sDataList[i].nGrad >= walk->item.nGrad ) {
					if ( walk->right == NULL ) {
						walk->right = temp;
						walk = walk->right ;
						k = 1;
					} // if
					else walk = walk->right;
				} // if
				else {
					if ( walk->left == NULL ) {
						walk->left = temp;
						walk = walk->left ;
						k = 1;
					} // if
					else walk = walk->left;
				} // else
				
				currH++ ;
			} // while				
		} // else
		
		if ( currH > graTH ) graTH = currH ; 
	} // for

} // BinaryT::gradTree()

void BinaryT::schTree() { // 建樹 學校名 
	int currH = 0;
	if ( schoolPtr != NULL ) reset( schoolPtr ) ;	
	
	for ( long long int i = 0; i < sDataList.size(); i++ ) {  

		TreePtr temp = new TreeNode;
		temp->item = sDataList[i] ;
		temp->right = NULL ;
		temp->left = NULL ;
		
		if ( schoolPtr == NULL ) schoolPtr = temp ;
		else {
			currH = 1;
			int k = 0;
			TreePtr walk = schoolPtr ;
			while ( k == 0 ) {
				if ( sDataList[i].sName.compare( walk->item.sName ) >= 0 ) {
					if ( walk->right == NULL ) {
						walk->right = temp;
						k = 1;
					} // if
					else walk = walk->right;
				} // if
				else {
					if ( walk->left == NULL ) {
						walk->left = temp;
						k = 1;
					} // if
					else walk = walk->left;
				} // else
				
				currH++ ;
			} // while	
			
		} // else
		
		if ( currH > schTH )  schTH = currH ;
	} // for
} // BinaryT::schTree()

void BinaryT::searchG() { // 搜尋 畢業生 
	if ( gradPtr == NULL ) cout << "Please choose command 1 first!" << endl;
	else {
		string num;
		cout << "Input the number of graduates: " ;
		cin >> num ;
		cout << endl ;
		int i = 0;
		TreePtr walk = gradPtr ;
		if ( isNumber(num) && atoi(num.c_str()) >= 0 ) recurG( walk, i, atoi(num.c_str()) ) ;
		else cout << "### the input string " << num <<" is not a decimal number! ###" << endl ;
		if ( i == 0 ) cout << "There is no match!" << endl ;
	} // else 
} // BinaryT::searchG()

void BinaryT::recurG( TreePtr walk, int & i, int num ) { // 遞迴搜尋畢業生 

	if ( walk == NULL ) return;
	else if ( walk->item.nGrad >= num ) {
		i++;
		if ( i == 1 ) cout << "Search results:" << endl ;
		cout << "[" << i << "]\t" << walk->item.sName << "\t" << walk->item.dName << "\t" << walk->item.cRef 
		<< "\t" << walk->item.level << "\t" << walk->item.nStud << "\t" << walk->item.nTeacher << "\t" << walk->item.nGrad << endl ;
		if ( walk->left != NULL ) recurG( walk->left, i, num ) ;
		if ( walk->right != NULL ) recurG( walk->right, i, num ) ;
		
		return;
	} // else if 
	else {
		if ( walk->right != NULL ) recurG( walk->right, i, num ) ;
		return;
	} // else
} // BinaryT::recurG()

void BinaryT::searchS() { // 搜尋 學校名 
	if ( schoolPtr == NULL ) cout << "Please choose command 1 first!" << endl;
	else {
		string name;
		cout << "Input a school name: " ;
		cin >> name ;
		cout << endl ;
		int i = 0 ;
		TreePtr walk = schoolPtr ;
		recurS( walk, i, name ) ;	
		if ( i == 0 ) cout << "There is no match!" << endl ;
	} // else
} // BinaryT::searchS()

void BinaryT::recurS( TreePtr walk, int & i, string name ) { // 遞迴搜尋學校名 
	if ( walk == NULL ) return;
	else if ( walk->item.sName.compare( name ) == 0 ) {
		i++;
		if ( i == 1 ) cout << "Search results:" << endl ;
		cout << "[" << i << "]\t" << walk->item.sName << "\t" << walk->item.dName << "\t" << walk->item.cRef 
		<< "\t" << walk->item.level << "\t" << walk->item.nStud << "\t" << walk->item.nTeacher << "\t" << walk->item.nGrad << endl ;
		if ( walk->right != NULL ) recurS( walk->right, i, name ) ;
		
		return;
	} // else if 
	else {
		if ( walk->left != NULL ) recurS( walk->left, i, name ) ;
		if ( walk->right != NULL ) recurS( walk->right, i, name ) ;
		return;
	} // else 
	
	return;
} // BinaryT::recurS()

void BinaryT::deleteTree() { // 任務4 
	if ( gradPtr == NULL ) cout << "Please choose command 1 first!" << endl;
	else {
		string num;
		cout << "Input the number of graduates: " ;
		cin >> num ;
		cout << endl ;
		int i = 0;		
		if ( isNumber(num) && atoi(num.c_str()) >= 0 ) {
			TreePtr pre = gradPtr ;
			recurDeleteG( gradPtr, pre, i, atoi(num.c_str()) ) ; // 刪畢業生二元樹 
			pre = schoolPtr ;
			recurDeleteS( schoolPtr, pre, atoi(num.c_str()) ) ; // 刪學校二元樹
			deleteData( atoi(num.c_str()) ) ; // 刪資料
			schTH = countH( schoolPtr ) ; // 算樹高 
			graTH = countH( gradPtr ) ;
			if ( i == 0 ) cout << "There is no match!" << endl ; 
			cout << "Tree height {School name} = " << schTH << endl;
			cout << "Tree height {Number of graduates} = " << graTH << endl;
			if ( schTH == 1 ) cout << schoolPtr->item.nGrad << endl ;
		} // if
		else cout << "### the input string " << num <<" is not a decimal number! ###" << endl ;
	} // else 
} // BinaryT::deleteTree() 

void BinaryT::recurDeleteG( TreePtr & walk, TreePtr pre, int & i, int num ) { // 刪除 畢業生二元樹 
	if ( walk == NULL ) return;
	else if ( walk->item.nGrad <= num ) {
		i++;
		if ( i == 1 ) cout << "Search results:" << endl ;
		cout << "[" << i << "]\t" << walk->item.sName << "\t" << walk->item.dName << "\t" << walk->item.cRef 
		<< "\t" << walk->item.level << "\t" << walk->item.nStud << "\t" << walk->item.nTeacher << "\t" << walk->item.nGrad << endl ;
		if ( walk != NULL && walk->right != NULL ) recurDeleteG( walk->right, walk, i, num ) ;
		if ( walk != NULL && walk->left != NULL ) recurDeleteG( walk->left, walk, i, num ) ;
		deleteNode( walk, pre ) ; // 刪節點 
		return;
	} // else if 
	else {
		if ( walk != NULL && walk->left != NULL ) recurDeleteG( walk->left, walk, i, num ) ;
		return;
	} // else 
} // BinaryT::recurDeleteG()

void BinaryT::recurDeleteS( TreePtr & walk, TreePtr pre, int num ) { // 刪除 學校二元樹 
	if ( walk == NULL ) return ;
	else if ( walk->item.nGrad <= num ) {
		if ( walk != NULL && walk->right != NULL ) recurDeleteS( walk->right, walk, num ) ;
		if ( walk != NULL && walk->left != NULL ) recurDeleteS( walk->left, walk, num ) ;
		deleteNode( walk, pre ); // 刪節點 		
		return;
	} // else if 
	else {
		if ( walk != NULL && walk->right != NULL ) recurDeleteS( walk->right, walk, num ) ;
		if ( walk != NULL && walk->left != NULL ) recurDeleteS( walk->left, walk, num ) ;
		return;
	} // else 
} // BinaryT::recurDeleteS()

void BinaryT::deleteNode( TreePtr & walk, TreePtr pre ) { // 刪節點 
	if ( pre == walk && walk->left == NULL && walk->right == NULL ) {
		delete walk ;
		walk = NULL ;
	} // if
	else if ( walk->left == NULL && walk->right == NULL ) {
		delete walk ;
		walk = NULL ;
	} // else if
	else if ( walk->right != NULL ) { // 右小 min
		TreePtr temp = walk->right ;
		if ( temp->left == NULL ) { // temp是最小 
			walk->item = temp->item ;
			if ( temp->right == NULL ) walk->right = NULL ;
			else walk->right = temp->right ;
			delete temp ;
		} // if
		else {  
			while ( temp->left != NULL && temp->left->left != NULL ) temp = temp->left ;	
			TreePtr min = temp->left ;
			walk->item = min->item ;
			if ( min->right != NULL ) temp->left = min->right ;
			else temp->left = NULL ;
			delete min ;
		} // else 
		
	} // else if
	else if ( walk->right == NULL ) { // 左大 max
		TreePtr temp = walk->left ;
		delete walk ; 
		walk = temp ;
	} // else if
	
} // BinaryT::deleteNodeG()

void BinaryT::deleteData( int num ) { // 刪vector 
	int i = 0 ; 
	vector<SchoolData>::iterator it ;
	for ( it = sDataList.begin() ; it < sDataList.end() ;  ) {
		if ( sDataList[i].nGrad <= num ) sDataList.erase(it) ;
		else ++it, ++i ;
	} // for
} // BinaryT::deleteData()

int BinaryT::countH( TreePtr walk ) { // 計算樹高 
	 if( walk == NULL ) return 0;
     else return max(countH(walk->left),countH(walk->right)) + 1;
} // BinaryT::countH()
