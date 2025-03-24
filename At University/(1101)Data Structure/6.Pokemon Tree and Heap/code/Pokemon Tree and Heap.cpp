// 蒲品憶 10927207   吳凱鈺 10927234 
#include <iostream>
#include <vector> // vector 
#include <fstream> // 讀檔 
#include <time.h> // 計算時間 
#include <conio.h> // 按任意見繼續 
#include <sstream> // istringstream stream用到 
#include <cstdlib> // atoi
#include <cstring>
#include <iomanip> // 使用小數點後兩位 
#include <cmath>
using namespace std ;

struct PokemonData {
	int i = 0 ;
	string name = "" ;
	string type = "" ;
	int num = 0 ;
	int HP = 0 ;
	int attack = 0 ;
	int defense = 0 ;	
};

bool sfile = true ;
string command ;
vector<PokemonData> PokemonList ;
PokemonData oneR ;
void ReadTxt() ; // 讀檔
void Print() ; // 印資料 
bool isNumber(string& str) ; // 字串是否為數字 
	
		
class PokeBalls {
	private: 
		struct TreeNode {
			PokemonData item;
			TreeNode * left;
			TreeNode * right;
		};
		
		typedef TreeNode * TreePtr ;
		TreePtr HpPtr = NULL ;
		
	public:	
		int HpTH = 0 ;
		void reset( TreePtr & walk ) ; // 初始化 
		void HpTree() ; // 建樹
		void searchLeft( TreePtr HpPtr ) ; // 搜尋 樹最小值 
		void searchRight( TreePtr HpPtr ) ; // 搜尋 樹最大值 
};

class Heap {
	public:	
		vector<PokemonData> HeapList ;
		void arrayBST( int i ) ; // 任務二 用動態陣列建樹  
		void countH() ; // 算樹高 和最左、最底項 
		void Delete() ;	// 刪除ROOT 
		void CopyList() ; // 複製 PokemonList
		void PrintHeap() ; // 印 HeapList
};

int main() {
	PokeBalls pokeBalls ;
	Heap heap ;
	while ( true ) {
		sfile = true ;
		cout << endl << "***** Pokemon Tree and Heap *****" ;
		cout << endl << "* 0. QUIT                       *" ;
		cout << endl << "* 1. Read a file to build BST   *" ;
		cout << endl << "* 2. Transform it into Max Heap *" ;
		cout << endl << "*********************************" ;
		cout << endl << "* 3. Delete max from Max Heap   *" ;
		cout << endl << "*********************************" ;
		cout << endl << "Input a choice(0, 1, 2, 3): " ;
		cin >> command ;
		sfile = true ;
		
		if ( command.compare("1") == 0 ) {
			ReadTxt() ; 
			if ( sfile == true ) {
				pokeBalls.HpTree() ;
			} // if
		} // if
		
		else if ( command.compare("2") == 0 ) {
			if ( PokemonList.size() != 0 ) {
				heap.CopyList() ; // 複製PokemonList至HeapList
				int i = ( heap.HeapList.size() - 1 ) / 2 ; // 父節點個數 
				heap.arrayBST( i ) ; // 用動態陣列建樹 
				heap.PrintHeap() ; // 印HeapList
				heap.countH() ;	// 算樹高 和最左、最底項
			} // if
			
			else cout << endl << "----- Execute Mission 1 first! -----" << endl ; 
		} // eise if
		
		else if ( command.compare("3") == 0 ) {
			if ( heap.HeapList.size() == 0 ) cout << "----- Execute Mission 2 first! -----\n" ;
			else {
				heap.Delete();	
				if ( heap.HeapList.size() != 0 ) { // 有資料 
					int i = ( heap.HeapList.size() - 1 ) / 2 ; // 父節點個數 
					heap.arrayBST( i ) ; // 用動態陣列建樹  
					heap.PrintHeap() ; // 印HeapList 
					heap.countH() ;	// 算樹高 和最左、最底項
				}
			} // else
		} // else if
		
		else if ( command.compare("0") == 0 ) return 0 ;
		
		else cout << endl << "Command does not exist!" << endl ;
	} // while 	
} // main()

void ReadTxt() { // 讀檔
	PokemonList.clear() ;
	string num, name ;
	cout << endl << "Input a file number [0: quit]: " ;
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
		string line, tok ;
		vector<string> temp ;
 		getline( file, line ) ;
		while (getline( file, line )) {
			int count = 0 ;
			istringstream stream(line) ; // 讀入一行 
			temp.clear() ;
			while ( getline(stream, tok, '\t') ) temp.push_back(tok) ; // 切tab 

    		for ( int i = 0 ; i < temp.size() ; i++ ) {  
				if ( i == 0 ) oneR.num = atoi(temp[i].c_str()) ;
				else if ( i == 1 ) oneR.name = temp[i] ;
				else if ( i == 2 ) oneR.type = temp[i] ;
				else if ( i == 5 ) oneR.HP = atoi(temp[i].c_str()) ;
				else if ( i == 6 ) oneR.attack = atoi(temp[i].c_str()) ;
				else if ( i == 7 ) oneR.defense = atoi(temp[i].c_str());
							
				else if ( i == 3 ) // 排除無Type2的情況 
					if ( isNumber(temp[i]) ) i++ ;
			} // for
			
			PokemonList.push_back(oneR) ; // 將oneR加入PokemonList
			stream.clear() ; // 釋放記憶體				
		} // while
		
		file.close() ; // 關閉檔案 
		
		if ( !PokemonList.size()) {
			cout << "### Get nothing from the file " << name << "!###" << endl ;
			cout << endl << "There is no data!" << endl ;
			sfile = false ;
		} // if		
		
		else Print() ;
	} // else	
	
} // ReadTxt()

void Print() { // 印資料 
	cout << "	#	Name				Type 1		HP	Attack	Defense" << endl ;
	for ( int i = 0; i < PokemonList.size() ; i++ ) {
		PokemonList[i].i = i+1 ;
		cout << "["<< PokemonList[i].i << "]" << "\t" << PokemonList[i].num << "\t" << PokemonList[i].name ; 
		if ( PokemonList[i].name.length() <= 4 ) cout << setw(25) ;
		else if ( PokemonList[i].name.length() <= 15 )  cout << setw(20) ;		
		else cout << setw(8) ;	
		cout << "\t" << PokemonList[i].type << setw(6) << "\t" << PokemonList[i].HP << "\t" << PokemonList[i].attack << "\t" << PokemonList[i].defense << endl;	
	} // for
} // Print()

bool isNumber(string& str) { // 字串是否為數字 
    for (char &c : str) {
        if (isdigit(c) == 0) return false;
    } // for
    return true;
} // isNumber()

void PokeBalls::reset( TreePtr & walk ) { // 初始化 
	if ( walk == NULL ) return ;	
	if ( walk->left != NULL ) reset( walk->left ) ;	
   	if ( walk->right != NULL ) reset( walk->right ) ;	
   	if ( walk != NULL ) {
   		delete walk ;
		walk = NULL ;	
	} // if 
} // PokeBalls::reset()

void PokeBalls::HpTree() { // 建樹
	int currH = 0 ;
	HpTH = 0 ;
	if ( HpPtr != NULL ) reset( HpPtr ) ;
	for ( int i = 0; i < PokemonList.size() ; i++ ) { 
		// 建新節點 
		TreePtr temp = new TreeNode;
		temp->item = PokemonList[i] ;
		temp->right = NULL ;
		temp->left = NULL ;
		
		if ( HpPtr == NULL ) HpPtr = temp ; // 插入節點  
		else {
			currH = 1;
			int k = 0 ;
			TreePtr walk = HpPtr ;  
			while ( k == 0 ) {
				if ( PokemonList[i].HP >= walk->item.HP ) { // HP:資料>=節點 
					if ( walk->right == NULL ) { // 最右 
						walk->right = temp; // 插入節點 
						k = 1; // 出迴圈 
					} // if
					walk = walk->right; // 往右 
				} // if
				else { // HP:資料<節點 
					if ( walk->left == NULL ) { // 最左 
						walk->left = temp; // 插入節點 
						k = 1; // 出迴圈 
					} // if
					walk = walk->left; // 往左  
				} // else
				
				currH++ ; // 算樹高 
			} // while				
		} // else
		
		if ( currH > HpTH ) HpTH = currH ; // 紀錄最大樹高 
	} // for

	cout << endl << "HP tree height = " << HpTH << endl ;
	
	searchLeft( HpPtr )	;
	searchRight( HpPtr ) ;
} // PokeBalls::HpTree()

void PokeBalls::searchLeft( TreePtr HpPtr ) { // 搜尋 樹最小值 
	// 最左節點為最小值->往左跑直到最後一項
	if ( HpPtr->left == NULL ) {
		cout << "Leftmost node:\n" ;
		cout << "	#	Name				Type 1		HP	Attack	Defense" << endl ;
		cout << "["<< HpPtr->item.i << "]" << "\t" << HpPtr->item.num << "\t" << HpPtr->item.name ;
		if ( HpPtr->item.name.length() <= 4 ) cout << setw(25) ;
		else if ( HpPtr->item.name.length() <= 15 )  cout << setw(20) ;		
		else cout << setw(8) ;		
		cout << "\t" <<  HpPtr->item.type << setw(6) << "\t" << HpPtr->item.HP << "\t" << HpPtr->item.attack << "\t" << HpPtr->item.defense << endl;	
	} // if 
	else searchLeft( HpPtr->left ) ; 
} // PokeBalls::searchLeft()

void PokeBalls::searchRight( TreePtr HpPtr ) { // 搜尋 樹最大值 
	// 最右節點為最大值->往右跑直到最後一項 
	if ( HpPtr->right == NULL ) {
		cout << "Rightmost node:\n" ;
		cout << "	#	Name				Type 1		HP	Attack	Defense" << endl ;
		cout << "["<< HpPtr->item.i << "]" << "\t" << HpPtr->item.num << "\t" << HpPtr->item.name ;
		if ( HpPtr->item.name.length() <= 4 ) cout << setw(25) ;
		else if ( HpPtr->item.name.length() <= 15 )  cout << setw(20) ;		
		else cout << setw(8) ;		
		cout << "\t" << HpPtr->item.type << setw(6) << "\t" << HpPtr->item.HP << "\t" << HpPtr->item.attack << "\t" << HpPtr->item.defense << endl;	
	} // if 
	else searchRight( HpPtr->right ) ; 
} // PokeBalls::searchRight()

void Heap::arrayBST( int i ) { // 任務二 用動態陣列建樹 
	while ( i >= 0 ) {
		// 子節點皆大於父節點 
		if ( i*2+2 < HeapList.size() && HeapList[i].HP < HeapList[i*2+1].HP && HeapList[i].HP < HeapList[i*2+2].HP ) {
			if ( HeapList[i*2+1].HP >= HeapList[i*2+2].HP ) { // 左大於右子節點 
				swap( HeapList[i] , HeapList[i*2+1] ) ;	
				if ( i*2+1 < HeapList.size() ) arrayBST( i*2+1 ) ;
			} // if
			else { // 右大於左子節點 
				swap( HeapList[i] , HeapList[i*2+2] ) ;
				if ( i*2+2 < HeapList.size() ) arrayBST( i*2+2 ) ;	
			}
		} // if
		// 左子節點大於父節點  
		else if ( i*2+1 < HeapList.size() && HeapList[i].HP < HeapList[i*2+1].HP ) {
			swap( HeapList[i] , HeapList[i*2+1] ) ;
			if ( i*2+1 < HeapList.size() ) arrayBST( i*2+1 ) ;
		} // else if 
		// 右子節點大於父節點  
		else if ( i*2+2 < HeapList.size() && HeapList[i].HP < HeapList[i*2+2].HP ) {
			swap( HeapList[i] , HeapList[i*2+2] ) ;
			if ( i*2+2 < HeapList.size() ) arrayBST( i*2+2 ) ;		
		} // else if 
		i-- ;
	} // while 

} // Heap::arrayBST()

void Heap::PrintHeap() { // 印 HeapList 

	cout << "	#	Name				Type 1		HP	Attack	Defense" << endl ;
	for ( int i = 0; i < HeapList.size() ; i++ ) {
		HeapList[i].i = i ;
		cout << "["<< HeapList[i].i << "]" << "\t" << HeapList[i].num << "\t" << HeapList[i].name ; 
		if ( HeapList[i].name.length() <= 4 ) cout << setw(25) ;
		else if ( HeapList[i].name.length() <= 15 )  cout << setw(20) ;		
		else cout << setw(8) ;	
		cout << "\t" << HeapList[i].type << setw(6) << "\t" << HeapList[i].HP << "\t" << HeapList[i].attack << "\t" << HeapList[i].defense << endl;	
	} // for
	
} // Heap::PrintHeap()

void Heap::countH() { // 算樹高 和最左、最底項 
	int i = HeapList.size() ;
	// 算樹高 
	int count = 0 ;
	while ( i > 0 ) {
		i = i / 2 ;
		count++ ;
	} // for 
	// 印樹高 
	cout << "HP heap height = " << count << endl ;
	
	// 印最左項 
	cout << "Leftmost node:" << endl ;
	i = pow( 2,(count-1) ) -1 ;
	cout << "	#	Name				Type 1		HP	Attack	Defense" << endl ;
	cout << "["<< HeapList[i].i << "]" << "\t" << HeapList[i].num << "\t" << HeapList[i].name ; 
	if ( HeapList[i].name.length() <= 4 ) cout << setw(25) ;
	else if ( HeapList[i].name.length() <= 15 )  cout << setw(20) ;		
	else cout << setw(8) ;	
	cout << "\t" << HeapList[i].type << setw(6) << "\t" << HeapList[i].HP << "\t" << HeapList[i].attack << "\t" << HeapList[i].defense << endl ;	
			
	// 印最底部 
	cout << "Bottom:" << endl ;
	i = HeapList.size() - 1 ;
	cout << "	#	Name				Type 1		HP	Attack	Defense" << endl ;
	cout << "["<< HeapList[i].i << "]" << "\t" << HeapList[i].num << "\t" << HeapList[i].name ; 
	if ( HeapList[i].name.length() <= 4 ) cout << setw(25) ;
	else if ( HeapList[i].name.length() <= 15 )  cout << setw(20) ;		
	else cout << setw(8) ;	
	cout << "\t" << HeapList[i].type << setw(6) << "\t" << HeapList[i].HP << "\t" << HeapList[i].attack << "\t" << HeapList[i].defense << endl;	
} // Heap::countH() 

void Heap::CopyList() { // 複製 PokemonList 
	HeapList.clear() ;
	for ( int i = 0 ; i < PokemonList.size() ; i++ ) {
		oneR = PokemonList[i] ;
		HeapList.push_back(oneR) ;
	} // for
	
} // Heap::CopyList() 

void Heap::Delete() { // 刪除ROOT  
	// 最後項與ROOT互換 
	swap( HeapList[0], HeapList[HeapList.size()-1] ) ;
	vector<PokemonData>::iterator it ;
	it = HeapList.end() - 1;
	// 印被刪除項 
	cout << "The removed root:\n" ;
	cout << "	#	Name				Type 1		HP	Attack	Defense" << endl ;
	int i = HeapList.size()-1 ;
	cout << "\t" << HeapList[i].num << "\t" << HeapList[i].name ; 
	if ( HeapList[i].name.length() <= 4 ) cout << setw(25) ;
	else if ( HeapList[i].name.length() <= 15 )  cout << setw(20) ;		
	else cout << setw(8) ;	
	cout << "\t" << HeapList[i].type << setw(6) << "\t" << HeapList[i].HP << "\t" << HeapList[i].attack 
		<< "\t" << HeapList[i].defense << endl << endl;	
	// 刪除ROOT 
	HeapList.erase(it) ;

} // Heap::Delete()

