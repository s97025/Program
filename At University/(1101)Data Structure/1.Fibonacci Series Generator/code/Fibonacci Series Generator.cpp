// 10927207 籞珇拘 
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <sstream> // stringstreamノ integer To int
#include<time.h> // 衡丁 

using namespace std ;


typedef unsigned long long int Ulli ;


struct Series {
	int serialnum[100] ; // 絪腹 
	Ulli num[100] ; // 计 
	Ulli factor1[100] ; // 计1	
	Ulli factor2[100] ; // 计2 
	Ulli innerloop[100] ; // ず碻吏Ω计 
	
    // [絪腹] 计  =  计1 * 计2 (Inner loop: ず碻吏Ω计 times) 
};


struct OneLine { // 患癹"逼"舱┮ㄏノ 
	char ch_Line[10] ;	
	Ulli int_temp = 0 ;	
	
};

static int N = 0 ; // 患癹"逼"舱┮ㄏノ  N: N计 


//mainㄏノfunction 
void Initialization ( int & outerLoop , Series & value ) ; // ﹍て
int SetType() ; // 砞﹚setType弄 弄 0 or 1 or 2 肚 
int InputNumber() ; // 砞﹚弄计 箂 肚

//ヴ叭1.2ノfunction 
void PrintSerialNum( int serialNum ) ; // 絪腹
string intTostring( Ulli i ) ; // // integer To int
void WhiteSpace( int spacecount, Ulli numlong, Ulli factor1long, Ulli factor2long ) ; // フ 
void PrintInnerloop( Ulli innerloop ) ; // ず癹伴ΩΩ计 
void PrintAllNumber( Ulli num, Ulli factor1, Ulli factor2 )  ; // 计计蛤フ

//ヴ叭1
void CalculateFactor( Ulli num , Ulli & factor1, Ulli & factor2, Ulli & innerloop ) ; // 癹伴 т计  
//ヴ叭2
void IterativeCalculateFib( int number, Series & value, int & outerLoop ) ; // 癹伴 璶:暗禣ん计  + Ω璶:т计+┮Τ狥﹁ 
 
 
//ヴ叭1 盡ノ function 
void RecursionCalculateFib( int i, int number, Series & value, int & outerLoop ) ;  // 患癹 禣ん计 そΑ 
//ヴ叭2 盡ノ function 
void RecurCalculateFactor( Ulli sqrtnum, Ulli num , Ulli & factor1, Ulli & factor2, Ulli & innerloop ) ; //患癹 т计 



//逼舱ノそΑ  
bool RepeatYn( OneLine num ) ; // 患癹  逼  
void Prem( OneLine num, Ulli & serialNum, Ulli temp ) ; // 耞弄计 N计 秈材if はぇ秈else 
void ScanfNum( OneLine & num, Ulli temp ); //耞 1い Τ礚狡计



int main() {
	
	int command = 0 ; // 砞﹚琌 0 Quit ┪ 1. Iterative generation ┪ 2. Recursive generation 	
	Series value ; // 癹伴┪患癹╰  
	int inputNumber = 0 ; // 块计 
	int outerLoop = 0 ; // 碻吏Ω计 
	
	Initialization ( outerLoop , value ) ; // ﹍て 
    command = SetType() ;
		
	while( command != 0 ) {
		
		inputNumber = InputNumber() ; 
		if ( command == 1 ) 
			IterativeCalculateFib( inputNumber, value, outerLoop ) ;
	
		else if ( command == 2 ) {
			RecursionCalculateFib( 1, inputNumber, value, outerLoop ) ; // 1 计 InputNumber()
		}
		
		else if ( command == 3 ) { 
			while ( inputNumber < 2 || inputNumber > 10 ) {
				cout << endl << "### The number must be in [2,10] ###" << endl;	 
				inputNumber = InputNumber() ;							
			}

			N = inputNumber ; // N计 
			OneLine num ;
			Ulli serialNum = 0 ;
			for ( int i = 0 ; i < N ; i++ ) { // ﹍砞﹚ 
				num.int_temp = i + (num.int_temp * 10) ; 
				num.ch_Line[i] = 48 + i ; // ASCLL 480 
			}	

			double start, end ;
			start = clock() ;
			Prem( num, serialNum, num.int_temp ) ;	
			end = clock() ;
			cout << "Recursion:  " << end - start << " ms" << endl ;
		}
		
		command = SetType() ;
		Initialization ( outerLoop, value ) ; // ﹍て  
	}
	return 0 ;
	
}

// ﹍て
void Initialization ( int & outerLoop , Series & value ) {
	outerLoop = 0 ;
	for ( int j = 0 ; j <= 100 ; j++ ) {
		value.serialnum[j] = 0 ;
		value.num[j] = 0 ;
		value.factor1[j] = 0 ; 	
		value.factor2[j] = 0 ; 
		value.innerloop[j] = 0 ; 
	}
}



// 砞﹚setType弄 弄 0 or 1 or 2 肚  
int SetType( ) {
	int command = 0 ; 
	
	cout << endl << "** Fibonacci Series Generator **" ;
	cout << endl << "* 0. Quit                      *" ;	
	cout << endl << "* 1. Iterative generation      *" ;	
	cout << endl << "* 2. Recursive generation      *" ;
	cout << endl << "* 3. Permutation generator     *" ;  
	cout << endl << "********************************" ;
	
	cout << endl << "Input a command(0, 1, 2, 3): " ;
	cin >> command ;
	while ( command > 3 || command < 0 ) { ///
		cout << endl << "Command does not exist!" << endl;
		
		cout << endl << "** Fibonacci Series Generator **" ;
		cout << endl << "* 0. Quit                      *" ;	
		cout << endl << "* 1. Iterative generation      *" ;	
		cout << endl << "* 2. Recursive generation      *" ;
		cout << endl << "* 3. Permutation generator     *" ;  
		cout << endl << "********************************" ;
		cout << endl << "Input a command(0, 1, 2, 3): " ;
		cin >> command ;		
	}
	
	return command ;
}

// 砞﹚弄计 箂 肚 
int InputNumber() {
	int number = 0 ;
	
	cout << endl << "Input a number: " ;
	cin >> number ;
	
	while ( cin.fail() ) {
    	cin.clear() ;
     	cin.ignore( 30000,'\n' ) ;
     	cout << endl << "Input a number: " ;
		cin >> number ;
    }
    
 	while ( number <= 0 ) {
		if ( number == 0 ) cout << endl << "### The number must be in [1,999999] ###" << endl ;
		cout << endl << "Input a number: " ;
		cin >> number ;
		
		while ( cin.fail() ) {
    		cin.clear() ;
    	 	cin.ignore( 30000,'\n' ) ;
    	 	cout << endl << "Input a number: " ;
			cin >> number ;
    	}
	}	   	

	return number ;
}

// 絪腹
void PrintSerialNum( int serialNum ) {
	cout << "[" << " " ;
	if ( serialNum > 0 && serialNum < 10 ) cout << " " ;
	cout << serialNum << "]" ;	
}


// integer To int
string intTostring( Ulli i ) {
	string str ;
	stringstream toInt(str) ;
	toInt << i ;
	
	return toInt.str() ;
}

//フ 
void WhiteSpace( int spacecount, Ulli numlong, Ulli factor1long, Ulli factor2long ) {
   // 1.计2(ぇ丁フ 2.Inner loop: Inner loopΩ计ぇ丁フ               	
                 	
	spacecount = spacecount - numlong - factor1long - factor2long ;
	while ( spacecount > 0 ) {
		cout << " " ;	
		spacecount-- ;
	}
}


// ず癹伴Ω计 
void PrintInnerloop( Ulli innerloop ) {       
	int whitespace = 12 - intTostring(innerloop).length() ;
	for ( int i = 1 ; i < whitespace ; i++ ) cout << " " ;
	cout << innerloop <<" times)" << endl ;	// 璶э程 	
}



// 计计蛤フ
void PrintAllNumber( Ulli num, Ulli factor1, Ulli factor2 ) {    	 
		              	
	Ulli numlong = intTostring( num ).length() ; // num计 
	Ulli factor1long = intTostring( factor1 ).length() ; // 计1	
	Ulli factor2long = intTostring( factor2 ).length() ; // 计2	

	cout << " " << factor1 << " * " << factor2 ;
	
	if( numlong <= 1 ) cout << " " ; 
	
	else if ( numlong <= 5 ) 
		WhiteSpace( 13, numlong, factor1long, factor2long ) ; // 13セ璶whiteSpace计秖 
	
	else if ( numlong <= 9 ) 
		WhiteSpace( 20, numlong, factor1long, factor2long ) ; // 20セ璶whiteSpace计秖 
		
	
	else if ( numlong <= 13 ) 
		WhiteSpace( 30, numlong, factor1long, factor2long ) ;	// 30セ璶whiteSpace计秖 	
	
	else if ( numlong <= 17 ) 
		WhiteSpace( 37, numlong, factor1long, factor2long ) ;	// 40セ璶whiteSpace计秖 		

	else 
		WhiteSpace( 42, numlong, factor1long, factor2long ) ;	// 45セ璶whiteSpace计秖  
	
}



// 癹伴 т计 
void CalculateFactor( Ulli num , Ulli & factor1, Ulli & factor2, Ulli & innerloop ) {
	Ulli sqrtnum = sqrt(num) ;
	int yn = 0 ;
	for ( sqrtnum = sqrtnum ; sqrtnum > 0 && yn == 0  ; sqrtnum-- ) {
		if ( num % sqrtnum == 0 ) {
			factor1 = sqrtnum ;		
			factor2 = num / sqrtnum ;			
			yn = 1 ;	
		}
		
		innerloop++ ;
	}
}

// 癹伴 璶:暗禣ん计  + Ω璶:т计+┮Τ狥﹁ 
void IterativeCalculateFib( int number, Series & value, int & outerLoop ) {
	//禣ん计
	//眖材2ぇ
	//穝计穦琌程计㎝计材计羆㎝ 
	for ( int i = 1 ; i <= number ; i++ ) {
		
		value.serialnum[i] = i ;
		
		if ( i == 1 ) value.num[i] = 1 ;			
		else if ( i == 2 ) value.num[i] = 2 ;							
		else value.num[i] = value.num[i-1] + value.num[i-2] ;  

		CalculateFactor( value.num[i] , value.factor1[i], value.factor2[i], value.innerloop[i] ) ; // т计 
		
		PrintSerialNum( value.serialnum[i] ) ; // 絪腹
		cout << " " << value.num[i] << " = " ; // 计
		PrintAllNumber( value.num[i] , value.factor1[i], value.factor2[i] ) ; // 计计蛤フ
		cout << "(Inner loop:" ; 
		PrintInnerloop( value.innerloop[i]) ; // ず癹伴Ω计 
		
		
		outerLoop++ ; 
	}

	cout << "<Outer loop:   " << outerLoop <<" times>" << endl ;
}


// 患癹 禣ん计 そΑ 
void RecursionCalculateFib( int i, int number, Series & value, int & outerLoop ) {
	if ( i <= number )  {
		value.serialnum[i] = i ;
		
		if ( i == 1 ) value.num[i] = 1 ;			
		else if ( i == 2 ) value.num[i] = 2 ;							
		else value.num[i] = value.num[i-1] + value.num[i-2] ; 
		
			
		Ulli sqrtnum = sqrt( value.num[i] ) ; // num秨腹	
	
		RecurCalculateFactor( sqrtnum, value.num[i], value.factor1[i], value.factor2[i], value.innerloop[i]) ; // т计 		
		
		PrintSerialNum( value.serialnum[i] ) ; // 絪腹
		cout << " " << value.num[i] << " = " ; // 计 
		
		if ( value.innerloop[i] > 10000000 ) 
			cout << "(Inner recursion: more than   10000000 times)" << endl ; // ず癹伴Ω计 	 
			
		else {
			PrintAllNumber( value.num[i] , value.factor1[i], value.factor2[i] ) ; // 计计蛤フ 
			cout << "(Inner recursion:" ;   
			PrintInnerloop( value.innerloop[i] ) ; // ず癹伴Ω计 
		}
		
		outerLoop++ ;
		RecursionCalculateFib( i + 1 , number, value, outerLoop ) ;	
	}
	
	else cout << "<<Outer recursion:  " << outerLoop <<" times>" << endl ;

}


//患癹 т计 
void RecurCalculateFactor( Ulli sqrtnum, Ulli num , Ulli & factor1, Ulli & factor2, Ulli & innerloop ) {
	int yn = 0 ; 
	innerloop++ ; 
	if ( innerloop <= 10000000 ) {
	
		if ( num % sqrtnum == 0 && yn == 0 ) {	
			factor1 = sqrtnum ;		
			factor2 = num / sqrtnum ;	
			yn = 1 ; // 患癹 
		}
	
		else 
			RecurCalculateFactor( sqrtnum - 1 , num , factor1, factor2, innerloop ) ;				
	}	
}






// 患癹  逼 
void Prem( OneLine num, Ulli & serialNum, Ulli temp ) {
	Ulli deline = pow( 10, N ) ; // N计 ぃN+1计 氨ゎ翴 
	if ( temp < deline ) {
		
		ScanfNum( num, temp ) ; // 耞弄计	
		
		if ( RepeatYn( num ) ) { // ぃ狡 
			serialNum++ ;
			cout << "[" << serialNum << "]" ;
			for ( int i = 0 ; i < N ; i++ ) {
				cout << " " << num.ch_Line[i] ; 			
			}
			cout << endl ; 
		}
		
		Prem( num, serialNum, temp + 1 ) ;			
	}
}

 // 耞弄计 N计 秈材if はぇ秈else 	
void ScanfNum( OneLine & num, Ulli temp ) {
	
	Ulli demarcation = pow( 10, N-1 ) ; // N计 蛤N计计 だ翴 
	if ( temp < demarcation ) { // 盢计皚 眖程秨﹍ 
		for ( int i = N-1 ; i > 0 ; i-- ) { // ぃ穦笆num.ch_Line[0]
			num.ch_Line[i] = 48 + temp % 10 ; //  // ASCLL 480 
			temp = temp / 10 ;
		}	 				
	}
		
	else sprintf(num.ch_Line, "%d", temp );
	
}


//耞 1い Τ礚狡计 
bool RepeatYn( OneLine num ) {
	for ( int i = 0 ; i < N ; i++ ) {
		for ( int j = i+1 ; j < N ; j++ ) {
			if ( num.ch_Line[i] == num.ch_Line[j] )
				return false ;
		}
	}
	
	return true ;
}








