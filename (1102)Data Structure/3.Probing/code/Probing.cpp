// 10927207_蒲品憶_資訊二乙 10927248_連翊安 
#include <iostream>
#include <vector> // vector 
#include <fstream> // 讀檔 
#include <cstdlib> // atoi
#include <cstring>
#include <sstream> // istringstream stream用到 
#include <math.h>
#include <iomanip>
#include <iterator>
using namespace std ; 

struct Data {
	char id[10] = {'0'} ; // 學號 
    char name[10] = {'\0'}  ; // 名字  
	unsigned char score1 = 0 ; // 分數 
	unsigned char score2 = 0 ; // 分數 
	unsigned char score3 = 0 ; // 分數 
	unsigned char score4 = 0 ; // 分數 
	unsigned char score5 = 0 ; // 分數 
	unsigned char score6 = 0 ; // 分數 				
	float average = 0.0 ; // 平均分數 
};

struct TableData {
	string Res = "\0" ; // 餘數 
	char id[10] = {'\0'} ;  // 學號 
    char name[10] = {'\0'}  ; // 名字  			
	string average = "\0" ;  // 平均分數 
};

bool sfile = true ; // 有無檔案
bool isbin = false ;  // 有無bin檔 
string num ; //檔名 
string command ;  
vector<Data> tempList ; // 暫存txt檔的資料 


string FloatToString( float n ) { // 浮點數轉字串 
    stringstream sstream;
    sstream << n;
    string str = sstream.str();
	return str ;
} // FloatToString

string IntToString( int n ) { // 整數轉字串 
	string str ;
	stringstream strtoint(str) ;
	strtoint << n ;
	return strtoint.str();
} // IntToString()

void Createbin() { //創新bin檔	 
	ofstream newFile ; // Create a new file
	string name = "input" + num + ".bin" ;
	newFile.open(name, ios::out | ios::binary ) ; // 文件打開	
	// 寫檔 
	// ostream & write(char* buffer, int count);
	// 其中，buffer 用於指定要寫入檔案的二進位制資料的起始位置；count 用於指定寫入位元組的個數			    
	for ( int k = 0 ; k < tempList.size() ; k++ ) newFile.write( (char *) &tempList[k], sizeof(Data)) ; // 一行一行讀檔		 	   		  	
	newFile.close() ;	
	cout << "build the " << name << " !" << endl << endl ; 	   
	tempList.clear() ; 		
} // Createbin()

void ReadTxt() { // 讀檔txt檔 
	ifstream file ;
	file.open("input" + num + ".txt", ios_base::in) ; 
	if (file.is_open()) {	
    	Data oneData ;
		string line, tok ;
		char temp2[10] ;
		vector<string> temp ;
		while (getline( file, line ) ) { 		
			istringstream stream(line) ; // 讀入一行 
			while ( getline(stream, tok, '\t') ) temp.push_back(tok) ; // 切tab 
    		for ( int i = 0 ; i < temp.size() ; i++ ) {  
    			for ( int j = 0 ; j < 10 ; j++ ) temp2[j] = temp[i][j] ; 	

				if ( i == 0 ) strcpy( oneData.id, temp2 ) ;
				if ( i == 1 ) strcpy( oneData.name, temp2 ) ;				
				if ( i == 2 ) oneData.score1 = atoi(temp[i].c_str()) ;
				if ( i == 3 ) oneData.score2 = atoi(temp[i].c_str()) ;
				if ( i == 4 ) oneData.score3 = atoi(temp[i].c_str()) ;
				if ( i == 5 ) oneData.score4 = atoi(temp[i].c_str()) ;					
				if ( i == 6 ) oneData.score5 = atoi(temp[i].c_str()) ;
				if ( i == 7 ) oneData.score6 = atoi(temp[i].c_str()) ;
				if ( i == 8 ) oneData.average = atof(temp[i].c_str()) ;															
			} // for
																  			
			tempList.push_back(oneData) ; // oneData into tempList	
 			temp.clear() ;
 			stream.clear() ;				
		} // while
			
		file.close() ; // 關閉檔案 
		
		if ( !tempList.size()) {
			cout << "### Get nothing from the file " << "input" << num <<  ".txt" << "!###" << endl ;
			cout << endl << "There is no data!" << endl ;
			sfile = false ;
		} // if					
		else Createbin() ; //創新bin檔					
	} // if
} // ReadTxt()

void Readbin() { // 讀bin檔 
	tempList.clear() ;
	ifstream binFile ;
	Data oneData ; 
	binFile.open("input" + num + ".bin", ios_base::in | ios::binary ) ; // 開bin檔 
	if ( binFile.is_open() ) {	
        while (!binFile.eof()) { // 一行一行讀檔	 
	    	binFile.read((char *) &oneData, sizeof(oneData) ) ;
			tempList.push_back( oneData ) ;							
			if ( binFile.fail() ) tempList.pop_back() ;	
        } // while						
		binFile.close() ; // 關閉檔案 		
	} // if	
} // Readbin()

void CompareBinTxt() { // 比較是否需創bin檔: 無bin檔  創bin檔將txt檔的資料寫入 
	sfile = true ;
	ifstream file1, file2 ;
	cout << endl << "Input a file number ([0] Quit): " ;
		
	while ( cin >> num ) {
		file1.open("input" + num + ".txt", ios_base::in) ; 
		file2.open("input" + num + ".bin", ios_base::in) ; 		
		
		if ( ( num.compare("0") == 0 ) ) { 
			sfile = false ;
			break ;	
		} // if
    	else if ( file2.is_open() ) { // 有bin檔 
    		cout << "\nHave " << "input" << num << ".bin !" << endl << endl ;
    		file2.close() ;
    		break ;
		} // else if			
 		else if ( file1.is_open() ) { // 無bin檔，有txt檔 
    		cout << "\nDon`t have " << "input " << num << ".bin."  << endl ; 			
    		ReadTxt() ;
    		file1.close() ;
    		break ;
		} // else if				
		else cout << "\n### " << "input" << num <<  ".txt" << " does not exist! ###\n" ;
		cout << endl << "Input a file number([0] Quit): " ;	  		
	} // while 		
} // CompareBinTxt()

class Hash {  
	protected:
		int range = 0 ; // 表格大小
		unsigned long long int  numSum = 0 ; // 數字總和  
		vector<TableData> dataList ; // 暫存bin檔的資料  
		string name = "" ; // 檔案加格式名稱
		string xY = "" ; // X 或 Y 的 Hash Table 
		string type = "" ; // 檔案類別 
		
			
		int GetPrime( int n ) { // 計算比n大的最小質數 
			int sqrtI ;
			bool isprime = true ;
			for ( int i = n + 1;; i++ ) {
				isprime = true ;
				sqrtI = sqrt(i);
				for ( int j = 2 ; j <= sqrtI ; ++j ) {
					if (i % j == 0) {
						isprime = false ;
						break;
					} // if
				} // for
				if ( isprime == true ) return i;
			} // for
			
			return -1;
		} // GetPrime()
	
		void CreateArray() { // 創造表格 
			dataList.clear() ;
			TableData t ;
			for ( int i = 0 ; i < range ; i++ ) dataList.push_back(t) ;
		} // array()
		
		void numberSum( Data data ) { // 計算學號的ASCII相乘的總和 
			unsigned long long int size = 1 ; 
			int count = 1 ; 
			for( int i = 0 ; i < 10 ; i++ ) {
				if ( isdigit(data.id[i]) ) size = size * (int)data.id[i] ;			
			} // for	
			numSum = size ;
		} // CalTableSize()
	
		void CreateTxt() { //創新***.txt檔 	 
			ofstream newFile ; // Create a new file
			int size = 11 ; // 平均寬度 
			string strRange = IntToString(range) ;  
			newFile.open(name, ios::out ) ; // 文件打開
						    
			newFile << " --- Hash Table " << xY << " --- (" << type << ")" << endl ; 
			for ( int k = 0 ; k < range ; k++ ) { // 一行一行寫入檔案 
				size = 11 ;
				newFile << "[" << setw(strRange.length()+1) << k << "]" ;
				if ( dataList[k].Res != "\0" ) 
					newFile << setw(11) << dataList[k].Res << "," << setw(11) << dataList[k].id << "," 
					<< setw(11)  << dataList[k].name << "," << setw(11) << dataList[k].average << endl ;		
				else 
					newFile << endl ;
			} // for
			
			newFile.close() ;		   
		} // CreateTxt()
} ;

class LinearHash: protected Hash {
	protected:		
		void average( float & unsuccAverage, float succAverage ) { // 計算unsuccessfuil、successfuil平均	，印出		
			// unsuccessful
		 	int count = 0 ; // 搜尋次數 
			float sum = 0.0 ; // 搜尋次數總和
			for ( int i = 0 ; i < range ; i++ ) {
				int j = i ;
				count = 1 ;
				while ( dataList[j].Res != "\0" ) { // 餘數為\0,未放資料
					count++, j++ ; 	
					if ( j >= range ) j = j % range ;
				} // while 		 
				if ( count != 0 ) count = count - 1 ; 
			 	sum = sum + count ; 
			} // for
			
			unsuccAverage = sum / range ; 	// unsuccessful average
				
			cout << "Hash table X has been created." << endl ;
			cout << "unsuccessful search: " <<  fixed  <<  setprecision(4) << unsuccAverage << " comparisons on average" << endl ;
			cout << "successful search: " <<  fixed  <<  setprecision(4) << succAverage << " comparisons on average" << endl ;			
		} // average() 
	
	public:			
		void BuildLinearProbing() {
	 		int count = 1 ; // 搜尋次數 
			float sum = 0.0 ; // 搜尋次數總和 
			float unsuccAverage = 0.0, succAverage = 0.0 ; // 平均次數 
			range = 1.2 * tempList.size() ; // 資料總比數*1.2  
			range = GetPrime( range ) ; // 表格大小
			CreateArray() ; // 創表格
			for ( int i = 0 ; i < tempList.size() ; i++ ) {
				numberSum( tempList[i] ) ;
				int reside = numSum  % range ; 
				int resTemp = reside ;
				count = 1 ;
				while ( dataList[resTemp].Res != "\0" ) { // 餘數為\0,未放資料
					count++, resTemp++ ; 	
					if ( resTemp >= range ) resTemp = resTemp % range ;
				} // while 
				
				if ( dataList[resTemp].Res == "\0" ) { // 新增資料至無碰撞位置 
					dataList[resTemp].Res = IntToString( reside ) ;
					strcpy(dataList[resTemp].id, tempList[i].id ) ;
					strcpy( dataList[resTemp].name, tempList[i].name ) ;
					dataList[resTemp].average = FloatToString( tempList[i].average ) ;			
				} // if		  			  		
				
				sum = sum + count ; 
			} // for
			
			name = "linear" + num + ".txt" ; // 檔案加格式名稱
			xY = "X" ; // X 或 Y 的 Hash Table 
			type = "linear probing" ; // 檔案類別 
			CreateTxt() ;
			succAverage = sum / tempList.size() ; // successful average	
			average( unsuccAverage, succAverage ) ; // 計算unsuccessfuil、successfuil平均，印出	
			tempList.clear() ;
		} // BuildLinearProbing()	
};

class DoubleHash: protected Hash { 
	protected:
		int stepsize = 0 ; // hash2 位移距離 			
		
	public:
		void BuildDoubleHash() {
			dataList.clear() ;
	 		int count = 1 ; // 執行次數 
			float sum = 0.0 ; // 執行總和 
			float unsuccAverage = 0.0, succAverage = 0.0 ; // 平均次數 
			range = 1.2 * tempList.size() ; // 資料總比數*1.2 
			range = GetPrime( range ) ; // 表格大小 
			CreateArray() ;	// 創表格					
			for ( int i = 0 ; i < tempList.size() ; i++ ) {							
				numberSum( tempList[i] ) ;
				int reside = numSum  % range ; 
				int resTemp = reside ;
				count = 1 ;
				while ( dataList[resTemp].Res != "\0" ) { // 餘數為\0,未放資料
					count++ ;
					if ( count == 2 )  stepsize = GetPrime( tempList.size() / 3 ) - numSum % ( GetPrime( tempList.size() / 3 ) ) ;						
					resTemp = resTemp + stepsize ;
					if ( resTemp >= range ) resTemp = resTemp % range ;		
				} // while 
				
				if ( dataList[resTemp].Res == "\0" ) { // 新增資料至無碰撞位置 
					dataList[resTemp].Res = IntToString( reside ) ;
					strcpy(dataList[resTemp].id, tempList[i].id ) ;
					strcpy( dataList[resTemp].name, tempList[i].name ) ;
					dataList[resTemp].average = FloatToString( tempList[i].average ) ;			
				} // if		  			  		
				
				sum = sum + count ; 
			} // for
			
			name = "double" + num + ".txt" ; // 檔案加格式名稱
			xY = "Y" ; // X 或 Y 的 Hash Table 
 			type = "double hashing" ; // 檔案類別 			
			CreateTxt() ; // 創檔 
			succAverage = sum / tempList.size() ; // successful average	
			cout << "Hash table Y has been created." << endl ;
			cout << "successful search: " <<  fixed  <<  setprecision(4) << succAverage << " comparisons on average" << endl ;				
			tempList.clear() ;
		} // BuildLinearProbing()		
};


int main() {
	LinearHash lh ; 
	DoubleHash bh ; 	
	while ( true ) {
		cout << endl << "****** Heap Construction ******" ;
		cout << endl << "* 0. QUIT                     *" ;
		cout << endl << "* 1. Build linear probing     *" ;
		cout << endl << "* 2. Build double hash        *" ;	
		cout << endl << "*******************************" ;
		cout << endl << "Input a choice(0, 1, 2): " ;
		cin >> command ;

		
		if ( command.compare("1") == 0 ) { 
			CompareBinTxt()	; // 比較是否需轉換: 無bin檔  創bin檔將txt檔的資料寫入 
			if (sfile == true ) {
				Readbin() ;
				lh.BuildLinearProbing();				
			}
		} // if
		
		else if ( command.compare("2") == 0 ) {	
			CompareBinTxt() ;
			if (sfile == true ) {			
				Readbin() ;
				bh.BuildDoubleHash() ;
			}
		} // if
		
		else if ( command.compare("0") == 0 ) return 0 ;
		
		else cout << endl << "Command does not exist!" << endl ;
	} // while 	
} // main()




