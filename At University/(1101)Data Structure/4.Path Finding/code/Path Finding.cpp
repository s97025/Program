// 蒲品憶 10927207   吳凱鈺 10927234 
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

class Maze {
private:
	char **maze = NULL ; // 迷宮
	char **minmap = NULL ; // 紀錄著最短路徑的迷宮
	int **loc = NULL ; // x0 y0 n
	int x = 0 ; // x軸 
	int y = 0 ; // y軸 
public:
	int step = 0; // 紀錄步數 
	int minstep ; // 最短步數	
	void Initialization () ; // 初始化		  
	void Inminstep () ; // 初始化 minstep
	void Setn( int & n ) ; // 設定要幾個G 限制1~100的數字  
	void ReadTxt( string & num ) ; // 讀入初始地圖 及x y軸 
	void Print(); 
	void PrintMin(); 
	void FindWay( int n, int x0, int y0 ) ; // 找路線 
	void FindGoal( int x0, int y0, int & n ) ; // 老鼠全部走過的路 找n個目標  
	void FindGoalRoad( int x0, int y0, int & n, bool & Gornot ) ; // 老鼠找到的正確路徑 
	void FindShortest( int x0, int y0 ) ; // 全部走過的路
	void ShortestRoad( int x0, int y0 ) ; // 找最短路徑	
};

static bool yn = false ; // 若找到n個G 則為true 
static bool sfile = false ; // 是否有檔案 
static int loc1 = 0; 

int main() {	

	int commond = 0 ;
	Maze map ; // 地圖	
	string num ; // 檔案名稱 
	int x0 = 0 , y0 = 0 ; // 老鼠的初始座標(0,0) 
	int n = 0; // 找n個目標 
	int command = 0 ; 
	
	while ( true ) {
	
		cout << endl << "*** Path Finding ***" ;
		cout << endl << "* 0. Quit          *" ;
		cout << endl << "* 1. One goal      *" ;
		cout << endl << "* 2. More goals    *" ;
		cout << endl << "* 3. Shortest path *" ;
		cout << endl << "********************" ;
		cout << endl << "Input a command(0, 1, 2, 3): " ;
		cin >> command ;
		
		if ( command == 1 || command == 2 || command == 3 ) {
			sfile = false;
			map.Initialization () ; // 初始化maze minmap
			map.ReadTxt( num ) ; // 讀入地圖 
			if ( command == 1 && sfile == true  ) {
				n = 1;
				map.FindWay( n, x0, y0 ) ; 
			} // if
			
			else if ( command == 2 && sfile == true  ) {	
				loc1 = 0 ;
				map.Setn( n ) ;
				cout << endl ;
				map.FindWay( n, x0, y0 ) ; 
			} // else if
			
			else if ( command == 3 && sfile == true  ) {
				map.step = 0;
				yn = false;
				map.FindShortest( x0, y0 ) ;
				map.Print();
				if ( yn == false ) cout << endl << endl << "### There is no path to find a goal! ###" << endl ;
				if ( yn == true ) {
					map.step = 0;
					map.Inminstep();
					map.ShortestRoad( x0, y0 ) ;
					map.PrintMin();
					cout << endl << endl << "Shortest path length = " << map.minstep + 1 << endl ; // +1加到G 
				}
			}
			
		} // if
		
		else if ( command == 0 ) return 0;
		
		else cout << endl << "Command does not exist!" << endl ;
	} // while 
} // main()


// 初始化
void Maze::Initialization() {
	
	if ( maze != NULL ) {	
		for( int line = 0 ; line < y ; line++ ) 
				delete [] maze[line] ;
		delete [] maze ;
		maze = NULL ;
	}

	if ( minmap != NULL ) {
		for( int line = 0 ; line < y ; line++ ) 
			delete [] minmap[line] ;
		delete [] minmap ;
		minmap = NULL ;
	}

	if ( loc != NULL ) {
		for( int line = 0 ; line < y ; line++ ) 
			delete [] loc[line] ;
		delete [] loc ;
		loc = NULL ;
	}

} // Initialization()


 // 初始化 minstep
void Maze::Inminstep () {
	minstep = x * y;
} // Inminstep ()


// 設定要幾個G 限制1~100的數字 
void Maze::Setn( int & n ) {
	cout << "Number of G (goals): " ;
	cin >> n ;
	while ( cin.fail() ) {
    	cin.clear() ;
     	cin.ignore( 30000,'\n' ) ;
    	cout << endl << "Number of G (goals): " ;
		cin >> n ;
 	}
	while ( n < 1 || n > 100 ) {
		cout << endl << "### The number must be in [1,100] ###" << endl ;
		cout << endl ;
		cout << "Number of G (goals): " ;
		cin >> n ;
					
		while ( cin.fail() ) {
    		cin.clear() ;
     		cin.ignore( 30000,'\n' ) ;
    		cout << endl << "Number of G (goals): " ;
			cin >> n ;
 	 	}	
	}	
} // Setn()

// 印出地圖 
void Maze::Print() {
	
	for( int j = 0 ; j < y ; j++ ) {
		for( int i = 0 ; i < x ; i++ ) {
			if ( maze[j][i] == 'A' ) maze[j][i] = 'G' ;
			cout << maze[j][i] ; 
		} // for	 
		cout << endl ;
	} // for
		
} // Print() 

void Maze::PrintMin() {
	
	cout << endl ;
	for( int j = 0 ; j < y ; j++ ) {
		for( int i = 0 ; i < x ; i++ ) {
			if ( minmap[j][i] == 'V' ) minmap[j][i] = 'E' ;
			if ( minmap[j][i] == 'A' ) minmap[j][i] = 'G' ;
			cout << minmap[j][i] ; 
		} // for	 
		cout << endl ;
	} // for
		
} // PrintMin() 

void Maze::FindWay( int n, int x0, int y0 ) {
	
	int m = n;
	yn = false ;
	FindGoal( x0, y0, n ) ;		
	if ( yn == false ) Print(); // 沒有找到目標
	else { // 有找到目標 
		cout << endl ;
		bool Gornot = false ;
		loc1 = 0;
		yn = false ;
		loc = new int*[x*y];
		for ( int i = 0; i < x*y ; i++ ) loc[i] = new int[3]; 
		FindGoalRoad( x0, y0, m, Gornot ) ;	
	} // else
	
} // FindWay()

// 讀入初始地圖 及x y軸 
void Maze::ReadTxt( string & num ) {	
	cout << "Input a file number (e.g., 201, 202, ...): " ;
	cin >> num ;
	cout << endl ;
	num = "input" + num + ".txt" ; 
	ifstream file ;
	file.open(num, ios_base::in) ; // 文件打開 

    if (!file.is_open()) {
        cout << num << " does not exist!\n" ;
    } // if

    else {
    	char temp ;
    	int i = 0, j = 0 ;
        file >> x ;
        file >> y ;
        minstep = x * y;
        sfile = true;
        if ( x > 0 && y > 0 ) {
        	int line = 0 ; 
        	char *temp = new char[x+1] ;
        	maze = new char*[y] ;
        	while ( file >> temp ) {
        		maze[line] = new char[x+1] ; 
        		strcpy( maze[line++], temp ) ;
			}
		}
        
        file.close() ; // 文件關閉 
    }
} // ReadTxt()

 // 老鼠全部走過的路 找n個目標 
void Maze::FindGoal( int x0, int y0, int & n ) {
	
	if ( maze[y0][x0] == 'G' ) maze[y0][x0] = 'A';
	if ( maze[y0][x0] == 'A' && n > 0 ) n--;
	
	if ( maze[y0][x0] == 'A' && n == 0 ) {
		yn = true ; 
		for( int j = 0 ; j < y ; j++ ) {
			for( int i = 0 ; i < x ; i++ ) {
				if ( maze[j][i] == 'A' ) maze[j][i] = 'G' ;
				cout << maze[j][i] ; 
			} 
			cout << endl ;
		} 
	}	
	else {
		if ( maze[y0][x0] == 'E' ) maze[y0][x0] = 'V' ; // 防止多換一個
		if ( x0+1 < x && ( maze[y0][x0+1] == 'E' || maze[y0][x0+1] == 'G' ) && maze[y0][x0+1] != 'A' && yn == false ) FindGoal( x0+1, y0, n ) ;
		if ( y0+1 < y && ( maze[y0+1][x0] == 'E' || maze[y0+1][x0] == 'G' ) && maze[y0+1][x0] != 'A' && yn == false ) FindGoal( x0, y0+1, n ) ;
		if ( x0-1 >= 0 && ( maze[y0][x0-1] == 'E' || maze[y0][x0-1] == 'G' ) && maze[y0][x0-1] != 'A' && yn == false ) FindGoal( x0-1, y0, n ) ;
		if ( y0-1 >= 0 && ( maze[y0-1][x0] == 'E' || maze[y0-1][x0] == 'G' ) && maze[y0-1][x0] != 'A' && yn == false ) FindGoal( x0, y0-1, n ) ;			
	}				
} // FindGoal()

void Maze::FindGoalRoad( int x0, int y0, int & n, bool & Gornot ) {
	int a = 0;
	// 判斷此點有幾條路可走 
	if ( x0+1 < x && maze[y0][x0+1] == 'V' ) a++;
	if ( y0+1 < y && maze[y0+1][x0] == 'V' ) a++;
	if ( x0-1 >= 0 && maze[y0][x0-1] == 'V' ) a++;
	if ( y0-1 >= 0 && maze[y0-1][x0] == 'V' ) a++;
	if ( a >= 2 ) { // 路至少有三條 
		loc[loc1][0] = x0 ; // 記錄此點座標 
		loc[loc1][1] = y0 ;
		loc[loc1][2] = n ; // 紀錄此時找到幾個G 
		loc1++;
	} // if
	
	// 若找到G 便將其標記並記錄G數量 
	if ( maze[y0][x0] == 'G' ) maze[y0][x0] = 'A';  
	if ( maze[y0][x0] == 'A' && n > 0 ) {
		n--;
		Gornot = true;
	} // if
	
	//  
	if ( maze[y0][x0] == 'A' && n == 0 ) {
		yn = true ; 
		for( int j = 0 ; j < y ; j++ ) {
			for( int i = 0 ; i < x ; i++ ) {
				// 將標記換回G 並印出 
				if ( maze[j][i] == 'A' ) maze[j][i] = 'G' ;
				cout << maze[j][i] ; 
			} // for 
			
			cout << endl ;
		} // for
		
		return ;
	} // if
	
	else {
		// 將V換成R 
		if ( maze[y0][x0] == 'V' ) maze[y0][x0] = 'R' ;
		// 往右走 
		if ( x0+1 < x && ( maze[y0][x0+1] == 'V' || maze[y0][x0+1] == 'G' ) && maze[y0][x0+1] != 'A' && yn == false ) {
			Gornot = false; 
			FindGoalRoad( x0+1, y0, n, Gornot ) ;
		} // if
		// 往下走 
		if ( y0+1 < y && ( maze[y0+1][x0] == 'V' || maze[y0+1][x0] == 'G' ) && maze[y0+1][x0] != 'A' && yn == false ) {
			Gornot = false;
			FindGoalRoad( x0, y0+1, n, Gornot ) ;
		} // if
		//往左走 
		if ( x0-1 >= 0 && ( maze[y0][x0-1] == 'V' || maze[y0][x0-1] == 'G' ) && maze[y0][x0-1] != 'A' && yn == false ) {
			Gornot = false;
			FindGoalRoad( x0-1, y0, n, Gornot ) ;
		} // if
		// 往上走 
		if ( y0-1 >= 0 && ( maze[y0-1][x0] == 'V' || maze[y0-1][x0] == 'G' ) && maze[y0-1][x0] != 'A' && yn == false ) {
			Gornot = false;
			FindGoalRoad( x0, y0-1, n, Gornot ) ;
		} //if 
				
		// 沒找到G 將R換回E 
		if ( maze[y0][x0] == 'R' && Gornot == false ) maze[y0][x0] = 'E' ;
	} // else
	
	// 尋找此點是否有三條路以上並判斷是否找到G 
	for ( int i = 0 ; i < 100 ; i++ ) {
		if ( loc[i][0] == x0 && loc[i][1] == y0 && loc[i][2] > n ) {	
			Gornot = true ;
			maze[y0][x0] = 'R' ;
		} // if
	} // for
	
} // FindGoalRoad()

void Maze::FindShortest( int x0, int y0 ) {
	if ( maze[y0][x0] == 'G' ) {
		//maze[y0][x0] = 'A';
		if ( step < minstep ) {
			minstep = step;
			yn = true ;
		} // if
	} // if
		
	if ( x0 == x || x0 < 0 || y0 == y || y0 < 0 || maze[y0][x0] != 'E' || step == minstep )
		return;
		
	maze[y0][x0] = 'V' ; 
	step++;
	if ( x0 + 1 < x ) FindShortest( x0+1, y0 ) ;
	if ( y0 + 1 < y ) FindShortest( x0, y0+1 ) ;
	if ( x0 - 1 >= 0 ) FindShortest( x0-1, y0 ) ;
	if ( y0 - 1 >= 0 ) FindShortest( x0, y0-1 ) ; 
	step--;
} // FindShortest()

void Maze::ShortestRoad( int x0, int y0 ) {
	if ( maze[y0][x0] == 'G' ) {
		if ( step < minstep ) {
			minstep = step ;
			minmap = new char*[y] ;
        	for ( int line = 0 ; line < y ; line++ ) { 
        		minmap[line] = new char[x+1] ; 
        		strcpy( minmap[line], maze[line] ) ;
			}
			
		} // if
	} // if

	if ( x0 == x || x0 < 0 || y0 == y || y0 < 0 || maze[y0][x0] != 'V' || step == minstep )
		return;
		
	maze[y0][x0] = 'R' ; 
	step++;
	if ( x0 + 1 < x ) ShortestRoad( x0+1, y0 ) ;
	if ( y0 + 1 < y ) ShortestRoad( x0, y0+1 ) ;
	if ( x0 - 1 >= 0 ) ShortestRoad( x0-1, y0 ) ;
	if ( y0 - 1 >= 0 ) ShortestRoad( x0, y0-1 ) ;
	maze[y0][x0] = 'V' ; 
	step--;
	
} // ShortestRoad()
