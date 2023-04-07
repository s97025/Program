// 蒲品憶 10927207   吳凱鈺 10927234 
// 如果無法執行 請在上面 點工具>編譯器選項>編譯器設定>程式碼產生>語言標準>改ISO C++ 11 
#include <iostream>
#include <vector> // vector 
#include <fstream> // 讀檔 
#include <time.h> // 計算時間 
#include <conio.h> // 按任意見繼續 
#include <sstream> // istringstream stream用到 
#include <iomanip> // 使用小數點後兩位 
using namespace std ;
 

static bool sfile = true ; // 是否有檔案  
static string num = "" ; // 檔名 數字部分 
static int command = 0 ;

struct Person { 	
	int oid = 0 ; // 工作編號 
	int arrival = 0 ; // 進入時刻 
	int duration = 0 ; // 工作耗時 
	int timeOut = 0 ; // 逾時時刻  
};	 

struct Done { // 完成清單 
	int cid = 0 ; // CPU編號 
	int oid = 0 ; // 工作編號 
	int departure = 0 ; // 完成時間 
	int delay = 0 ;  // 延遲時間
};

struct Abort { // 取消清單 
	int cid = 0 ; // CPU編號 
	int oid = 0 ; // 工作編號 
	int abort = 0 ; // 取消時刻
	int delay = 0 ;  // 延遲時間
}; 

class Queue {
	private:
		struct QueueNode {
			Person item ;
			QueueNode * next ; // 指下一個 
		};	 
		
		QueueNode * backPtr = NULL ; // 佇列	
	public:		
		bool IsEmpty() ; // 佇列為空 
		bool IsFull() ; // 佇列為滿 
		void Enqueue( Person newitem ) ; // 後端新增 vector<Person> data
		void Dequeue( Person & newitem ) ; // 前端擷取刪除 
		int Size() ; // 算長度 
};

struct Cpu {
	int curtime = 0 ; // CPU現在時間 
	int cid = 0 ; // CPU編號	
	Person temp ; // CPU執行區段 
	Queue que ; // 佇列 
};

class Sort {				
	public:	
		// Mission1 
		vector<string> header ; // 項目名稱 old，arrival，duration， timeOut
		vector<Person> data ; // 資料，讀入後存於此 
		Person persNew ; // struct Person型別 名稱 	
		double readT = 0 , sortT = 0 , writeT = 0 ; // 讀檔，排序，寫檔時間 
		void ReadTxt() ; // 讀檔 
		void ShellSort() ; // 希爾排序 
		void Writefile() ; // 寫檔 	
		void Printdata() ; // 印出資料 		
		void Printtime() ; // 印出時間 
		
		// Mission2 
		vector<Done> donelist ; // 完成清單 
		Done done ;
		vector<Abort> abortlist ; // 取消清單 
		Abort abort ; 		
		void Add_done( Person & temp, int & curtime, int cid  ) ; // 新增至完成清單 
		void Add_abort( Person & temp, int curtime, int error, int cid  ) ; // 新增至取消清單 
		void RunQue( Queue & que, Person & temp , int & curtime, int error, int cid ) ; // 進CPU執行 
		void OnlyOneCpuDone( Cpu & cpu, int & i, int error ) ;
		void WritefileOutput() ; // 寫檔
		void WritefileOutput3() ; // 寫檔3 
		void Que() ; // 佇列模擬
		void Que3() ; // 佇列模擬3 
		void ChooseCpu( Cpu & cpu1, Cpu & cpu2, int & i ) ;
		void DataArrivalOrNot( Cpu & cpu, int error, int i );
		void PopQueToCpu( Cpu & cpu, int error );	
		bool Compare( Queue que1, Queue que2 ) ;
		int BoolChooseCpu( Queue que1, Queue que2 );	
};  

int main() {
	Sort sort ;
	
	while ( true ) {
	
		cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
		cout << endl << "* 0. Quit                            *" ;
		cout << endl << "* 1. Sort a file                     *" ;
		cout << endl << "* 2. Simulate one FIFO queue         *" ;
		cout << endl << "* 3. Simulate two queues by SQF      *" ;		
		cout << endl << "**************************************" ;
		cout << endl << "Input a command(0, 1, 2, 3): " ;
		cin >> command ;
		sfile = true ; 
		
		if ( command == 1 ) {
			sort.ReadTxt() ; // 讀檔 
			if ( sfile == true ) {
				sort.Printdata() ; // 印出資料 (無排序)				
				sort.ShellSort() ; // 希爾排序
				sort.Writefile() ; // 寫檔 
				system("pause"); // 請按任意鍵繼續 . . .
				sort.Printtime() ; // 印出時間 
				cout << "See sorted" << num << ".txt" << endl;	
			} // if			
		} // if
		
		else if ( command == 2 ) {
			sort.ReadTxt() ; // 讀檔 
			if ( sfile == true ) {
				sort.Que() ; // 佇列模擬
				sort.donelist.clear() ;
				sort.abortlist.clear() ;
			} // if	
			num = "" ;		     
		} // eise if
		
		else if ( command == 3 ) {
			sort.ReadTxt() ; // 讀檔 
			if ( sfile == true ) {
				sort.Que3() ; // 佇列模擬
				sort.donelist.clear() ;
				sort.abortlist.clear() ;
			} // if	
			num = "" ;		     
		} // eise if		
		
				
		else if ( command == 0 ) return 0;
		
		else cout << endl << "Command does not exist!" << endl ;
		
		if ( command == 1 || command == 2 || command == 3 ) {
			sort.header.clear() ; // 釋放記憶體空間 
			sort.data.clear() ; // 釋放記憶體空間
		} // if
	} // while 
} // main()

// 讀檔 
void Sort::ReadTxt() {
	double start, end ;
	if ( num == "" || command == 1 ) {
		cout << "Input a file number (e.g., 401, 402, 403, ...): " ;
		cin >> num ;
		cout << endl ;			
	} // if	
						
	string name ;
	if ( command == 1 ) name = "input" + num + ".txt" ; 
	if ( command == 2 || command == 3 ) name = "sorted" + num + ".txt" ; 
	ifstream file ;
	file.open(name, ios_base::in) ; // 文件打開 

    if (!file.is_open()) {
        cout << "### " << name << " does not exist! ###\n" ;
        num = "" ;
        sfile = false ;
    } // if
	
    else {
    	start = clock() ;
    	int temp, count ;
    	string line ;
    	
    	getline( file, line ) ;
    	header.push_back(line) ; // 讀入項目名
    	
    	while( getline( file, line ) ) { // 讀入資料 
    		istringstream stream(line) ;
    		count = 0 ;
    		while( stream >> temp ) {
    			if ( count == 0 ) persNew.oid = temp ;
    			else if ( count == 1 ) persNew.arrival = temp ;
    			else if ( count == 2 ) persNew.duration = temp ;
    			else if ( count == 3 ) persNew.timeOut = temp ;
    			count++ ;
			} // while
			
			data.push_back(persNew) ; 
			stream.clear() ;
		} // while
				
		file.close() ; // 文件關閉 	
			
		end = clock() ;
		readT = end - start ;
	} // else	
} // Sort::ReadTxt()

// 希爾排序
void Sort::ShellSort() { 
	double start = clock() ;
	Person temp ;
	for ( int h = data.size() / 2 ; h > 0 ; h = h / 2 ) { // h代表間隔數 
		for ( int unsorted = h ; unsorted < data.size() ; unsorted++ ) {  
			for ( int loc = unsorted ; ( loc >= h ) ; loc = loc - h ) { 
				if ( data[loc-h].arrival > data[loc].arrival ) 
					swap( data[loc-h], data[loc] ) ;
					
				else if ( data[loc-h].arrival == data[loc].arrival ) 
					if ( data[loc-h].oid > data[loc].oid )
						swap( data[loc-h], data[loc] ) ;
			} // for
		} // for
	} // for
	
	double end = clock() ;
	sortT = end - start ;		
} // Sort::ShellSort()

// 寫檔 
void Sort::Writefile() { 
	double start = clock() ;
	ofstream newFile ; // Create a new file
	string name = "sorted" + num + ".txt" ;
    newFile.open(name, ios_base::out) ; // 文件打開 
  	for ( int i = 0 ; i < header.size() ; i++ )  // 項目名
  		if ( i == 0 ) newFile << header[i] ;
 		else newFile << '\t' << header[i] ;
			
	newFile << endl ;
   	for ( int i = 0 ; i < data.size() ; i++ )   
  	    newFile << data[i].oid << '\t' << data[i].arrival << '\t' << data[i].duration << '\t' << data[i].timeOut << endl;

	double end = clock() ;
	writeT = end - start ;    
} // Sort::Writefile()

void Sort::Printdata() { // 印出資料 		
    for ( int i = 0 ; i < header.size() ; i++ ) { // 項目名
    	cout << '\t' << header[i] ;
	} // for
	cout << endl ;
		
	for ( int i = 0 ; i < data.size() ; i++ ) // 資料  
		cout << "(" << i+1 << ")" << '\t' << data[i].oid << '\t' << data[i].arrival << '\t' << data[i].duration << '\t' << data[i].timeOut << endl;	
} // Sort::Printdata()

void Sort::Printtime() { // 印出時間 
	cout << endl ;
	cout << "Reading data:\t" << readT << " clocks(" << readT << ".00 ms).\n" ;
	cout << "Sorting data:\t" << sortT << " clocks(" << sortT << ".00 ms).\n" ;
	cout << "Writing data:\t" << writeT << " clocks(" << writeT << ".00 ms).\n" ;	
} // Sort::Printtime() 

// 佇列模擬3
void Sort::Que3() {
	
	int i = 0, error = 0 ;
	Cpu cpu1, cpu2 ;
	cpu1.cid = 1;
	cpu2.cid = 2;
	
	while ( i < data.size() ) { 
		error = 0 ;
		if ( data[i].arrival >= cpu1.curtime && data[i].arrival >= cpu2.curtime ) { // 抵達時間大於等於現在時間  做佇列 
			if ( cpu1.que.IsEmpty() ) { // que1 size = 0 -> 1
				DataArrivalOrNot( cpu2, error, i );
				ChooseCpu( cpu1, cpu2, i ) ;	
			} // if
			else if ( cpu2.que.IsEmpty() ) { // que2 size = 0 -> 2
				DataArrivalOrNot( cpu1, error, i );
				ChooseCpu( cpu1, cpu2, i ) ;
			} // else if
			else { // 佇列皆有東西 
				while ( (!cpu1.que.IsEmpty() && data[i].arrival >= cpu1.curtime) || (!cpu2.que.IsEmpty() && data[i].arrival >= cpu2.curtime) ) {
 					if ( (cpu1.curtime <= cpu2.curtime && !cpu1.que.IsEmpty()) || cpu2.que.IsEmpty() ) PopQueToCpu( cpu1, error );
					else if ( (cpu1.curtime > cpu2.curtime && !cpu1.que.IsEmpty()) || cpu1.que.IsEmpty() ) PopQueToCpu( cpu2, error );
				} // while
			} // else
		} // if
		
		else if ( data[i].arrival < cpu1.curtime && data[i].arrival >= cpu2.curtime ) OnlyOneCpuDone( cpu2, i, error ) ;
		
		else if ( data[i].arrival < cpu2.curtime && data[i].arrival >= cpu1.curtime ) OnlyOneCpuDone( cpu1, i, error ) ;
		
		else { // 抵達達時間皆小於現在時間 
			if ( !cpu1.que.IsFull() && !cpu2.que.IsFull() ) {
				if ( Compare( cpu1.que, cpu2.que ) ) cpu1.que.Enqueue( data[i] );
				else cpu2.que.Enqueue( data[i] );
			} // else if
			else if ( !cpu1.que.IsFull() ) cpu1.que.Enqueue( data[i] );
			else if ( !cpu2.que.IsFull() ) cpu2.que.Enqueue( data[i] );
			else if ( cpu1.que.IsFull() && cpu2.que.IsFull() ) Add_abort( data[i], cpu1.curtime, 1, 0 ) ; // 新增至取消清單 
			
			i++;
		} // else
		
	} // while
	
	// 做剩下佇列 
	while ( !cpu1.que.IsEmpty() || !cpu2.que.IsEmpty() ) { // que1 || que2 size=1~3 
	
		if ( !cpu1.que.IsEmpty() && !cpu2.que.IsEmpty() ) { // que1 && que2 size=1~3 
			if( cpu1.curtime <= cpu2.curtime ) PopQueToCpu( cpu1, error ); // 現在時間:1<=2 -> 1 
			else PopQueToCpu( cpu2, error );	// 現在時間:1>2 -> 2			
		} // if
		else if( !cpu1.que.IsEmpty() ) PopQueToCpu( cpu1, error ); // que1 size=1~3
		else if( !cpu2.que.IsEmpty() ) PopQueToCpu( cpu2, error ); // que2 size=1~3 
		
	} // while
	
	cout << "The simulation is running..." << endl;
	WritefileOutput3() ; 
	cout << "See double" << num << ".txt" << endl;
	
} // Sort::Que()

// 佇列模擬
void Sort::Que() {
	int i = 0 ;
	int error = 0 ;
	Cpu cpu ;
	
	while ( i < data.size() ) { 
		error = 0 ;
		if ( data[i].arrival < cpu.curtime ) { // 抵達時間小於現在時間 
			if ( !cpu.que.IsFull() ) // 佇列未滿  
				cpu.que.Enqueue( data[i] ) ;
				
			else { // 錯誤4.1進入佇列時發現佇列已滿 
				error = 1 ;
				Add_abort( data[i], cpu.curtime, error, 1 ) ; // 新增至取消清單 
			} // else 
			
			i++;
		} // if
		
		else { // 抵達時間大於或等於現在時間 先做佇列 
			if ( !cpu.que.IsEmpty() ) { // 佇列未空 
				cpu.que.Dequeue( cpu.temp ) ;
				if ( data[i].arrival == cpu.curtime ) { // 如果 抵達時間=現在時間 
					cpu.que.Enqueue( data[i] ) ;
					i++ ;
				} // if 			
				
				RunQue( cpu.que, cpu.temp , cpu.curtime, error, 1 ) ; // 進CPU執行 
			} // if
			else { // 佇列已空 
				cpu.curtime = data[i].arrival ;
				Add_done( data[i], cpu.curtime, 1 ) ; // 新增至完成清單 
				i++ ;
			} // else 
		} // else 	
	} // while
	
	while ( !cpu.que.IsEmpty() ) {
		cpu.que.Dequeue( cpu.temp ) ;
		RunQue( cpu.que, cpu.temp , cpu.curtime, error, 1 ) ;
	} // if	
	
	cout << "The simulation is running..." << endl;
	WritefileOutput() ; 
	cout << "See output" << num << ".txt" << endl;
} // Sort::Que()

// 新增至完成清單 
void Sort::Add_done( Person & temp, int & curtime, int cid ) {
	done.cid = cid ;
	done.oid = temp.oid;
	done.departure = curtime + temp.duration ;
	done.delay = curtime - temp.arrival ;
	donelist.push_back( done ) ;
	curtime = done.departure ;
} // Sort::Add_done()

// 新增至取消清單 
void Sort::Add_abort( Person & temp, int curtime, int error, int cid ) {
	abort.cid = cid ;
	abort.oid = temp.oid ;
	if ( error == 1 ) {
		abort.abort = temp.arrival ;
		abort.delay = 0 ;
	} // if
	
	else if ( error == 2 ) {
		abort.abort = curtime ;
		abort.delay = abort.abort - temp.arrival ;	
	} // else if
	
	else if ( error == 3 ) {
		abort.abort = temp.timeOut ;
		abort.delay = abort.abort - temp.arrival ;			
	} // else if 
		
	abortlist.push_back( abort ) ;
} // Sort::Add_abort()

// 進CPU執行 
void Sort::RunQue( Queue & que, Person & temp , int & curtime, int error, int cid  ) { // 做佇列 

	if ( curtime >= temp.timeOut ) { // 現在時間大於等於逾時時間，4.2進入CPU時已逾時 
		error = 2 ;
		Add_abort( temp, curtime, error, cid ) ;	// 新增至取消清單 	
	} // if
		 
	else { // 現在時間小於逾時時間，沒已逾時 
		if ( curtime + temp.duration > temp.timeOut ) { // 4.3進入CPU執行中發生逾時 
			error = 3 ;
			Add_abort( temp, curtime, error, cid ) ; // 新增至取消清單 
			curtime = temp.timeOut ;					
		} // if 
				
		else { // 沒逾時，完成清單 
			Add_done( temp, curtime, cid ) ; // 新增至完成清單 
		} // else
	} // else
} // Sort::RunQue() 

void Sort::OnlyOneCpuDone( Cpu & cpu, int & i, int error ) {
	if ( !cpu.que.IsEmpty() ) DataArrivalOrNot( cpu, error, i );
	else {
		cpu.curtime = data[i].arrival;
		Add_done( data[i], cpu.curtime, 2 ) ;
		i++;
	} // else 
} // Sort::OnlyOneCpuDone()
 
// 選擇CPU 
int Sort::BoolChooseCpu( Queue que1, Queue que2 ) {
	if ( que1.IsEmpty() ) return 1;
	else if ( que2.IsEmpty() ) return 2;
	else if ( !que1.IsFull() && !que2.IsFull() ) {
		if ( Compare( que1, que2 ) ) return 1;
		else return 2;
	} // else if
	else if ( !que1.IsFull() ) return 1;
	else if ( !que2.IsFull() ) return 2;
	else if ( que1.IsFull() && que2.IsFull() ) return 0; // 新增至取消清單 
} // Sort::BoolChooseCpu()
  
void Sort::DataArrivalOrNot( Cpu & cpu, int error, int i ) {
	while ( !cpu.que.IsEmpty() && data[i].arrival >= cpu.curtime ) {
		cpu.que.Dequeue( cpu.temp ) ;
		RunQue( cpu.que, cpu.temp , cpu.curtime, error, cpu.cid ) ;
	} // while
} // Sort::DataArrivalOrNot()
  
// 寫檔
void Sort::WritefileOutput() {
	
	double total = 0.0 ; 
	double averageDelay = 0.0, successRate = 0.0 ; 
	ofstream newFile ; // Create a new file
	string name = "output" + num + ".txt" ;
    newFile.open(name, ios_base::out) ; // 文件打開
	 
	newFile << '\t' << "[Abort Jobs]" << endl ;
	newFile << '\t' << "OID" << '\t' << "Abort" << '\t' << "Delay" << endl ;
   	for ( int k = 0 ; k < abortlist.size() ; k++ ) {
   		total = total + abortlist[k].delay;
  	    newFile << "[" << k+1 << "]" << '\t' << abortlist[k].oid << '\t' << abortlist[k].abort << '\t' << abortlist[k].delay << endl;
  	}
  	    
  	newFile << '\t' << "[Jobs Done]" << endl ;
  	newFile << '\t' << "OID" << '\t' << "Departure" << '\t' << "Delay" << endl ;
	for ( int k = 0 ; k < donelist.size() ; k++ ) {
		total = total + donelist[k].delay;
		newFile << "[" << k+1 << "]" << '\t' << donelist[k].oid << '\t' << donelist[k].departure << '\t' << donelist[k].delay << endl;
	}
		
	averageDelay = total/(double)data.size() ;
	successRate = 100 * (double)donelist.size()/(double)data.size() ;
	newFile << "[Average Delay]" << '\t' << setprecision(2) << fixed << averageDelay <<  "ms" << endl ;
	newFile << "[Success Rate ]" << '\t' << setprecision(2) << fixed << successRate << "%" << endl ;
	
	newFile.close()	;
} // Sort::WritefileOutput()

// 寫檔3 
void Sort::WritefileOutput3() {
	
	double total = 0.0 ; 
	double averageDelay = 0.0, successRate = 0.0 ; 
	ofstream newFile ; // Create a new file
	string name = "double" + num + ".txt" ;
    newFile.open(name, ios_base::out) ; // 文件打開
	 
	newFile << '\t' << "[Abort Jobs]" << endl ;
	newFile << '\t' << "OID" << '\t' << "CID" << '\t' << "Abort" << '\t' << "Delay" << endl ;
   	for ( int k = 0 ; k < abortlist.size() ; k++ ) {
   		total = total + abortlist[k].delay;
  	    newFile << "[" << k+1 << "]" << '\t' << abortlist[k].oid << '\t' << abortlist[k].cid << '\t' << abortlist[k].abort << '\t' << abortlist[k].delay << endl;
  	}
  	    
  	newFile << '\t' << "[Jobs Done]" << endl ;
  	newFile << '\t' << "OID" << '\t' << "CID" << '\t' << "Departure" << '\t' << "Delay" << endl ;
	for ( int k = 0 ; k < donelist.size() ; k++ ) {
		total = total + donelist[k].delay;
		newFile << "[" << k+1 << "]" << '\t' << donelist[k].oid << '\t' << donelist[k].cid << '\t' << donelist[k].departure << '\t' << donelist[k].delay << endl;
	}
		
	averageDelay = total/(double)data.size() ;
	successRate = 100 * (double)donelist.size()/(double)data.size() ;
	newFile << "[Average Delay]" << '\t' << setprecision(2) << fixed << averageDelay <<  "ms" << endl ;
	newFile << "[Success Rate ]" << '\t' << setprecision(2) << fixed << successRate << "%" << endl ;
	
	newFile.close()	;
} // Sort::WritefileOutput3()

void Sort::ChooseCpu( Cpu & cpu1, Cpu & cpu2, int & i ) {
	if ( BoolChooseCpu( cpu1.que, cpu2.que ) == 1 ) {
		cpu1.curtime = data[i].arrival;
		Add_done( data[i], cpu1.curtime, 1 ) ; // 新增至完成清單  
	}
	else if ( BoolChooseCpu( cpu1.que, cpu2.que ) == 2 ) {
		cpu2.curtime = data[i].arrival;
		Add_done( data[i], cpu2.curtime, 2 ) ; // 新增至完成清單  
	}
	else ;
				
	i++;
} // Sort::ChooseCpu()

bool Sort::Compare( Queue que1, Queue que2 ) {
	if ( que1.Size() > que2.Size() ) return false ;
	return true ;	
} // Sort::Compare()

void Sort::PopQueToCpu( Cpu & cpu, int error ) {
	cpu.que.Dequeue( cpu.temp ) ;
	RunQue( cpu.que, cpu.temp , cpu.curtime, error, cpu.cid ) ;
} // Sort::PopQueToCpu()



int Queue::Size() {
	int i = 0;
	if ( IsEmpty() ) return i;
	else {
		i = 1 ;
		QueueNode * walk = backPtr->next ;
		if ( walk == backPtr ) return i;
		while ( walk != backPtr ) { 
			i++ ;
			walk = walk->next ;
		} // while	
		return i ;	
	} // else 
} // Queue::Size()

// 佇列為空 
bool Queue::IsEmpty() {
	if ( backPtr == NULL ) return true ;
	return false ;
} // Queue::IsEmpty()

// 佇列為滿
bool Queue::IsFull() {
	if ( IsEmpty() ) return false ;
	else {
		int i = 1 ;
		QueueNode * walk = backPtr->next ;
		while ( walk != backPtr ) { 
			i++ ;
			walk = walk->next ;
		} // while	
		if ( i == 3 ) return true ; // 放滿三個 
		return false ;	
	} // else
} // Queue::IsFull()

// 後端新增 
void Queue::Enqueue( Person newitem ) { 
	QueueNode * newPtr = new QueueNode ;
	newPtr->item = newitem ;
	if (IsEmpty())
		newPtr->next = newPtr ;
	else { 
		newPtr->next = backPtr->next ;
		backPtr->next = newPtr ;
	} // else 
	
	backPtr = newPtr ;
	newPtr = NULL;
	delete newPtr;
} // Queue::Enqueue

// 前端擷取刪除 
void Queue::Dequeue( Person & newitem ) { 

	if(IsEmpty()) 
		return ;
	else {
		QueueNode * tempPtr = backPtr->next ;		
		newitem = tempPtr->item ;
		if ( backPtr == backPtr->next )	backPtr = NULL ;
		else backPtr->next = tempPtr->next ;
		
		tempPtr->next = NULL;
		delete tempPtr ;	
	} // else
	
} // Queue::Dequeue

