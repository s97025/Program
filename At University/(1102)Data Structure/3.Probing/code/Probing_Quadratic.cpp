// 10927248 連翊安 10927207 蒲品憶

# include <iostream>
# include <vector>
# include <string>
# include <algorithm>
# include <fstream>
# include <math.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>
# include <iomanip> // 控制小數輸出

using namespace std ;

struct Info {
    char sid[10] = {'\0'};
    char sname[10] = {'\0'};
    unsigned char score[6] = {'\0' } ;  // 6個分數
    float average = 0.0 ;
} ;

struct key {
    Info data ;
    int hvalue ;
} ;

vector<Info> Data ;
string filenum ;

class Hash {
    protected :
        vector<key> table ;
        int size ;
        double search = 0.0 ;  

        int getPrime( int a ) {
            // 找比a大的最小質數
            int i = 0 ;
            
            while( i != 1 ) {
                a ++ ;
                i = sqrt( a ) ;

                while ( a % i != 0 ) {
                    i -- ;
                } // while()   

            } // while()

            return a ;
        } // getPrime()

        void SetSize() {
            // 設定Table 大小
            size = getPrime( Data.size() * 1.2 ) ;
            table.resize( size ) ;
        } // SetSize()  

        int HashFunct( char sid[10] ) {
            // 計算hash的key 
            long long int hvalue = 1 ;

            for ( int i = 0 ; i < 10 && sid[i] != '\0' ; i ++ ) {
                hvalue = hvalue * (int)sid[i] ;
            } // for()

            hvalue = hvalue % size ;

            return hvalue ;
        } // HashFunct()
    
        void Copy( Info & b, Info a ) {
            strcpy( b.sid, a.sid ) ;
            strcpy( b.sname, a.sname ) ;

            for ( int i = 0 ; i < 6 ; i ++ ) {
                b.score[i] = a.score[i] ;
            } // for()  

            b.average = a.average ; 
        } // Copy()

    public :
        void Initial() {
            table.clear() ;
            search = 0.0 ;
        } // Initial()

        void Print() {
            for ( int i = 0 ; i < table.size() ; i ++ ) {
                cout << '[' << i << "]\t" << table[i].hvalue << '\t' << table[i].data.sid << '\t' 
                    << table[i].data.sname << '\t' << table[i].data.average << endl  ;
            } // for()
        } // Print()

        void Write( string filename ) {
            fstream file( filename.c_str(), ios :: out | fstream :: app ) ;

            for ( int i = 0 ; i < table.size() ; i ++ ) {
                file << '[' << setw(3) << i << "] "  ;
                if ( strcmp( table[i].data.sid, ""  ) != 0 ) {
                    file << setw(10) << table[i].hvalue << ',' << setw(11) << table[i].data.sid << ',' ;
                    file << setw(11) << table[i].data.sname << ','<< setw(11) ;
                    file << setw(11) << table[i].data.average ;
                } // if()
                file << endl ;
            } // for()

            file.close() ;
        } // Write()
} ;

class Linear : public Hash {

    public :
        void Write( string num ) {
            string name = "linear" + num + ".txt" ;
            fstream file( name.c_str(), ios :: out ) ;

            file << " --- Hash Table X --- (linear probing)" << endl ;
            file.close() ;

            Hash ::Write( name ) ;
        } // Write()
            
        double Unsussece() {
            // 查詢失敗 
            double step = 0.0 ;
            for ( int i = 0 ; i < table.size() ; i ++ ) {

                int j = i ;
                while ( strcmp( table[j].data.sid, "" ) != 0  ) {
                    // 一直找 直到找到沒有放東西的地方
                    step ++ ;
                    j ++ ;
                    if ( j == table.size() ) {
                        // 超過table大小 從0再繼續找
                        j = 0 ;
                    } // if()

                } // while()

            } // for()

            return step / table.size() ;

        } // Unsussece()

        void Build( string num ) {
            
            SetSize() ;
            key temp ;
            
            for ( int i = 0 ; i < Data.size() ; i ++ ) {
                Copy( temp.data, Data[i] ) ;
                temp.hvalue = HashFunct( Data[i].sid ) ;
                Place( temp ) ;
            } // for()

            Write( num ) ;
            cout << endl << "Hash Table X has been created." << endl ;
            cout << "Unsuccessful search: " << fixed << setprecision(4) << Unsussece() << " comparisons on average." << endl ;
            cout << "successful search: " << search / Data.size() << " comparisons on average." << endl ;

        } // Build()

        void Place( key temp ) {

            int i = temp.hvalue ;
            
            while( strcmp( table[i].data.sid, "" ) != 0 ) {
                // 找還沒被放過的位置
                i ++, search ++ ; 

                if ( i == table.size() ) {
                    // 超過table大小 從頭繼續找
                    i = 0 ;
                } // if()

            } // while()

            search ++ ;
            Copy( table[i].data, temp.data ) ;
            table[i].hvalue = temp.hvalue ;

        } // Place()
} ;

class DoubleHash : public Hash {

    int steph ;     // 最高步階

    public :

        int StepFunct( char sid[10] ) {
            // 計算step(key)
            long long int hvalue = 1 ;

            for ( int i = 0 ; i < 10 && sid[i] != '\0' ; i ++ ) {
                hvalue = hvalue * (int)sid[i] ;
            } // for()

            hvalue = steph - ( hvalue % steph ) ;
            return hvalue ;
        } // StepFunct()

        void Write( string num ) {
            string name = "double" + num + ".txt" ;
            fstream file( name.c_str(), ios :: out ) ;

            file << " --- Hash Table Y --- (double hashing)" << endl ;
            file.close() ;

            Hash:: Write( name ) ;
        } // Write()
 
        void Build( string num ) {

            SetSize() ;
            // 設定最高步階
            steph = getPrime( Data.size() / 3 ) ;
            key temp ;

            for ( int i = 0 ; i < Data.size() ; i ++ ) {
                Copy( temp.data, Data[i] ) ;
                temp.hvalue = HashFunct( Data[i].sid ) ;
                Place( temp ) ;
            } // for()

            Write( num ) ;
            cout << fixed << setprecision(4) << endl << "Hash Table Y has been created." << endl ;
            cout << "successful search: "  << search / Data.size() << " comparisons on average." << endl ;
        } // Build()

        void Place( key temp ) {
            int i = temp.hvalue ;

            // 算步階
            int step = StepFunct( temp.data.sid ) ;

            while( strcmp( table[i].data.sid, "" ) != 0 ) {

                i += step, search ++ ; 

                if ( i >= table.size() ) {
                    i = i % table.size() ;
                } // if()

            } // while()

            search ++ ;
            Copy( table[i].data, temp.data ) ;
            table[i].hvalue = temp.hvalue ;
        } // Place()

} ;

class QuadHash : public Hash {

    public : 
    
        void Write( string num ) {
            string name = "quadratic" + num + ".txt" ;
            fstream file( name.c_str(), ios :: out ) ;

            file << " --- Hash Table Z --- (Quadratic probing)" << endl ;
            file.close() ;

            Hash:: Write( name ) ;
        } // Write()

        double Unsussece() {
            // 查詢失敗 
            double step = 0.0, temp = 0.0 ;
            int k, j ;
            bool test = false, first = true ;     // 紀錄是否開始循環
            vector<int> path ;
            for ( int i = 0 ; i < table.size() ; i ++ ) {
                
                test = false ;
                path.clear() ;
                j = i, k = 1, temp = 0.0 ;
                first = true ;
                while ( strcmp( table[j].data.sid, "" ) != 0 && ! test ) {
                    // 一直找 直到找到沒有放東西的地方

                    if ( ! first && j == i ) {
                        // 開始循環
                        test = true ;
                    } // if()

                    else {
                    	first = false ;
                        path.push_back( j ) ;
                        j = i + k * k ;
                        temp ++, k ++ ;
                        if ( j >= table.size() ) {
                            // 超過table大小 
                            j = j % table.size() ;
                        } // if()
                    } // else 

                } // while()

                step += temp ;
            } // for()
            
            return step / table.size() ;

        } // Unsussece()

        void Build( string num ) {
                SetSize() ;
                key temp ;

                for ( int i = 0 ; i < Data.size() ; i ++ ) {
                    Copy( temp.data, Data[i] ) ;
                    temp.hvalue = HashFunct( Data[i].sid ) ;
                    Place( temp, i ) ;
                } // for()

                Write( num ) ;
                cout << endl << "Hash Table Z has been created." << endl ;
                cout << "Unsuccessful search: " << fixed << setprecision( 4 ) <<  Unsussece() << " comparisons on average." << endl ;
                cout << "successful search: " << search / Data.size() << " comparisons on average." << endl ;        
        } // Build()

        void Place( key temp, int index ) {
            int i = temp.hvalue, j = 1 ;             // 計算碰撞次數
            vector<int> path ;
            double k = 0.0 ;          // 紀錄次數
            bool test = false, first = true ;
			
			
            while( strcmp( table[i].data.sid, "" ) != 0 && ! test ) {

                if ( !first && i == temp.hvalue ) {
                    cout << "### Failed at [" << index << "].###" << endl ;
                    test = true ; 
                } // if()

                else {
                	first = false ;
                    path.push_back( i ) ;
                    i = temp.hvalue + j * j ;
                    j ++, k ++ ; 

                    if ( i >= table.size() ) {
                        i = i % table.size() ;
                    } // if()
                } // else
            } // while()

            search = search + k + 1 ;
            
            if ( ! test ) {
                // 沒有放不下
                Copy( table[i].data, temp.data ) ;
                table[i].hvalue = temp.hvalue ;
            } // if()

        } // Place()
} ;

void txt2bin( string filenum ) {
    
    fstream in, out ;
    string binname = "input" + filenum + ".bin", txtname = "input" + filenum + ".txt" ;
    in.open( txtname.c_str(), ios:: in ) ;
    out.open( binname.c_str(), ios:: out | ios_base:: binary ) ;

    Info t ;
    while ( in.getline( t.sid, 10, '\t') ) {
        
        in.getline( t.sname, 10, '\t') ;
        char s[10] ;
        for ( int i = 0 ; i < 6 ; i ++ ) {
            in.getline( s, 10, '\t');
            t.score[i] = atoi( s ) ;
        } // for()

        in.getline( s, 10 );
        t.average = atof( s ) ;
        out.write( (char*)&t, sizeof(t) ) ;

    } // while()

    out.close() ;
    in.close() ;
} // txt2bin()

bool Read() {

    fstream bin, txt ;
    bool success = false ;
    Info t ; 
    int len ;

    cout << endl << "Input a file number ([0] Quit): " ;
    cin >> filenum ;
    string binname = "input" + filenum + ".bin", txtname = "input" + filenum + ".txt" ;
    bin.open( binname.c_str(), ios:: in | ios_base:: binary ) ;
    txt.open( txtname.c_str(), ios:: in ) ;

    while( ! success ) {
        
        if ( bin.is_open() ) {
            // has binary file 
            // 計算總共有多少筆資料
            bin.seekg( 0, bin.end ) ;
            len = bin.tellg() / sizeof( t ) ;
            bin.seekg( 0, bin.beg ) ;
            
            for( int i = 0 ; i < len ; i ++ ) {
                bin.read( (char*)&t, sizeof(t) ) ;
                Data.push_back( t ) ;
            } // for()

            success = true ;
        } // if()

        else if ( txt.is_open() ) {
            // has txt file 
            // create a binary file
            txt2bin( filenum ) ;

            // open the binary file 
            bin.open( binname.c_str(), ios:: in | ios_base:: binary ) ;
 
            // 計算總共有多少筆資料            
            bin.seekg( 0, bin.end ) ;
            len = bin.tellg() / sizeof( t ) ;
            bin.seekg( 0, bin.beg ) ;
            
            for( int i = 0 ; i < len ; i ++ ) {
                bin.read( (char*)&t, sizeof(t) ) ;
                Data.push_back( t ) ;
            } // for()

            success = true ;
        } // else if()

        else if ( filenum.compare( "0" ) == 0 ) {
            return false ;
        } // else if 

        else {
            cout << endl << "### The file does not exist ! ###" << endl ;
            cout << endl << "Input a file number ([0] Quit): " ;
            cin >> filenum ;
            binname = "input" + filenum + ".bin", txtname = "input" + filenum + ".txt" ;
            bin.open( binname.c_str(), ios:: in | ios_base:: binary ) ;
            txt.open( txtname.c_str(), ios:: in ) ;  
        } // else 

    } // while()

    bin.close() ;
    txt.close() ;

    return true ;
} // Read()

bool Check( string temp, int & command ) {
    
	command = 0 ;

	for ( int i = 0 ; i < temp.size() ; i ++ ) {
		if ( temp[i] < '0' || temp[i] > '9' ) {
			return false ;
		} //if()

		command = command * 10 + temp[i] - '0' ;

	} // for()

	return true ;

} // Check()

void Preface() {
    cout << endl << "****    Heap Construction   ****" ;
    cout << endl << "* 0. QUIT                      *" ;
    cout << endl << "* 1. Linear Probing            *" ;
    cout << endl << "* 2. Double Hashing            *" ;
    cout << endl << "* 3. Quadratic Probing         *" ;
    cout << endl << "********************************" ;
    cout << endl << "Input a choice(0, 1, 2, 3):" ;
} // Preface()

int main() {
 	
    Linear linear ;
    DoubleHash dhash ;
    QuadHash qhash ;
	string temp ;
 	int command ;
 	bool test = true ;
 	do {
 		
 		Preface() ;
		cin >> temp ;
		if ( ! Check( temp, command ) ) {
			cout << endl << "Command does not exist!" << endl ;
		} // if()

 		else if ( command == 0 ) test = false ;
 		
 		else if ( command == 1 ) {
            Data.clear() ;
            linear.Initial() ;
            if ( Read() ) {
                linear.Build( filenum ) ;
            } // if () 
        } // else if() 

        else if ( command == 2 ) {
            dhash.Initial() ;
            if ( ! filenum.empty() ) {
                dhash.Build( filenum ) ;
            } // if () 

            else {
                cout << endl << "### Choose 1 first. ###" << endl ;
            } // else

        } // else if()

        else if ( command == 3 ) {
            qhash.Initial() ;
            if ( ! filenum.empty() ) {
                qhash.Build( filenum ) ;
            } // if()

            else {
                cout << endl << "### Choose 1 first. ###" << endl ;
            } // else

        } // else if()

        else cout << endl << "Command does not exist!" << endl ;

	 } while ( test ) ;

} // main()

