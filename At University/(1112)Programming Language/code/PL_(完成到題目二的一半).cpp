# include <iostream>
# include <stdio.h>
# include <vector> // vector 
# include <sstream> // istringstream stream用到 
# include <cstdlib> // atoi
# include <string>
# include <iomanip> // 使用小數點後兩位 
# include <cmath>
# include <cstring>

using namespace std ;

# define NUEXPECTED_TOKEN 1
# define NUEXPECTED_RIGJT_PAREN 2
# define NO_CLOSING_QUOTE 3
# define NO_MORE_INPUT 4

/*  
2023/5/26 4:00
Project 1 end
*/

struct Error {
  string msg ;
  int typ ;  
} ; 

struct Tree {
  string token ;
  Tree * left  ;
  Tree * right  ;
  Tree * pre ;
};
typedef Tree * Treenodes ;

int gRow ;
int gCol ;
string gTestNum ;
bool gNewExpr ;
Error gError ;
vector<string> gTable ;
vector<string> gList ;

void Treeinitial( Treenodes & tmp ) {
  tmp->token = "" ;
  tmp->left = NULL ;
  tmp->right = NULL ;
  tmp->pre = NULL ;  
} // Treeinitial()

bool Read_Line( string & line ) {
  char ch[200] ;
  if ( cin.getline( ch, 200 ) ) {
    for ( int i = 0 ; i < strlen( ch ) ; i++ ) {
      line = line + ch[i] ;
    } // for
     
    return true ;
  } // if
  else {
    return false ;
  } // else
} // Read_Line()

bool Get_Token( string & token, string & line ) {
  token = "" ;
  if ( line.size() == 0 ) {
    bool isRead = true ;
    while ( isRead && line.size() == 0 ) {
      gCol = 0 ; // get new line initial col
      isRead = Read_Line( line ) ;
      gRow++ ;
      if ( gRow == 2 && gNewExpr ) {
        gRow = 1 ;
        gNewExpr = false ;
      } // if
      
    } // while
    
    if ( ! isRead ) return false ;
  } // if
  
  for ( int i = 0 ; i < line.size() ; i++ ) {
    gCol++ ;
    if ( token.size() != 0 ) {
      // is string 
      if ( line[i] == '\"' ) {  
        if ( token[0] != '\"' ) {
          line.erase( 0, i ) ;
          gCol-- ;
          return true ;          
        } // if
        else {
          token = token + line[i] ;
          // string done
          if ( token[token.size()-2] != '\\' && line[i] == '\"' ) {
            line.erase( 0, i+1 ) ;
            return true ;
          } // if      
        } // else
      } // if             
      else if ( token[0] == '\"' ) { 
        token = token + line[i] ;
      } // else if  

      else if ( line[i] == ';' ) {
        line = "" ;
        return true ;
      } // else if        
      // is space
      else if ( line[i] == ' ' ) {
        line.erase( 0, i ) ;
        gCol-- ;
        return true ;
      } // else if
      // is (, ), '     
      else if ( line[i] == '(' || line[i] == ')' || line[i] == '\'' ) {
        line.erase( 0, i ) ;
        gCol-- ;
        return true ;
      } // else if
      else if ( i == line.size() - 1 ) {
        token = token + line[i] ;
        line = "" ;
        return true ;
      } // else if
      else {
        token = token + line[i] ;
      } // else 
    } // if
    else {
      if ( line[i] == ' ' ) {
        line.erase( 0, 1 ) ;
        return true ;
      } // if
      else if ( line[i] == ';' ) {
        line = "" ;
        return true ;
      } // else if
      else {
        token = token + line[i] ;
        if ( token[0] == '(' || token[0] == ')' || token[0] == '\'' ) {
          line.erase( 0, i+1 ) ;
          return true ;
        } // if
      } // else 
    } // else 
  } // for
  
  line = "" ;
  return true ;
} // Get_Token()

string To_string( int num ) {
  string result;  
  bool isNegative = false;
  
  if ( num == 0 ) {
    return "0";  
  } // if
  else if ( num < 0 ) {
    isNegative = true ;
    num *= -1 ; 
  } // else if
  
  while ( num > 0 ) {
    char digit = '0' + ( num % 10 ) ;
    result = digit + result ;
    num /= 10 ;
  } // while 
  
  if ( isNegative ) {
    result = "-" + result ;
  } // if
      
  return result;
} // To_string()

void SeterrMsg( int errTyp, string token ) {
  if ( gError.typ == 0 || ( gError.typ != NO_CLOSING_QUOTE && gError.typ != NO_MORE_INPUT ) ) {
    gError.typ = errTyp ;
    string strRow = To_string( gRow ) ;
    string strCol1 = To_string( gCol - ( token.size() - 1 ) ) ;
    string strCol2 = To_string( gCol + 1 ) ;
    
    if ( gError.typ == NUEXPECTED_TOKEN ) {
      gError.msg = "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
      gError.msg =  gError.msg + strRow + " Column " + strCol1 + " is >>" + token + "<<" ;
    } // if
    
    if ( gError.typ == NUEXPECTED_RIGJT_PAREN ) {
      gError.msg = "ERROR (unexpected token) : ')' expected when token at Line " ; 
      gError.msg = gError.msg + strRow + " Column " + strCol1 + " is >>" + token + "<<" ;
    } // if
    
    if ( gError.typ == NO_CLOSING_QUOTE ) {
      gError.msg = "ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
      gError.msg = gError.msg + strRow + " Column " + strCol2  ;
    } // if
    
    if ( gError.typ == NO_MORE_INPUT ) {
      gError.msg = "ERROR (no more input) : END-OF-FILE encountered" ; 
    } // if    
  } // if 
} // SeterrMsg()

bool IsINT( string token ) {
  bool isINT = false ;
  
  if ( token[0] == '+' || token[0] == '-' ) ;
  else if ( token[0] <= '9' && token[0] >= '0' ) isINT = true ;
  else return false ; 
  
  for ( int i = 1 ; i < token.size() ; i++ ) {
    if ( token[i] <= '9' && token[i] >= '0' ) isINT = true ;
    else return false ; 
  } // for
  
  if ( isINT ) return true ;
  else return false ;
} // IsINT()

bool IsFLOAT( string token ) {
  bool isdot = false ;
  bool isNum = false ;
  
  if ( token[0] == '+' || token[0] == '-' ) ;
  else if ( token[0] <= '9' && token[0] >= '0' ) isNum = true ;
  else if ( token[0] == '.' && ! isdot ) isdot = true ; // a dot 
  else return false ; 
  
  for ( int i = 1 ; i < token.size() ; i++ ) {
    if ( ( token[i] <= '9' && token[i] >= '0' ) || token[i] == '.' ) {
      if ( token[i] <= '9' && token[i] >= '0' ) isNum = true ;
      
      if ( token[i] == '.' && ! isdot ) isdot = true ; // a dot 
      else if ( token[i] == '.' && isdot ) return false ; // dots
      
    } // if 
    else return false ;
    
  } // for
 
  if ( isNum ) return true ;
  return false ;
} // IsFLOAT()

bool IsSTRING( string token, bool & mark ) {
  int backslash = 0 ;
  if ( token.size() >= 2 && token[0] == '\"' && token[token.size()-1] == '\"' ) {
    for ( int i = token.size()-2 ; i >= 1 ; i-- ) {
      if ( token[i] == '\\' ) backslash++ ; 
      else {    
        if ( backslash % 2 == 0 ) {
          return true ;
        } // if
        else {
          SeterrMsg( NO_CLOSING_QUOTE, token ) ; 
          mark = true ;
          return false ;
        } // else
      } // else if
      
      if ( i == 1 ) {    
        if ( backslash % 2 == 0 ) {
          return true ;
        } // if
        else {
          SeterrMsg( NO_CLOSING_QUOTE, token ) ; 
          mark = true ;
          return false ;
        } // else
      } // if     
    } // for
  } // if

  else if ( token.size() > 0 && token[0] == '\"' ) {
    SeterrMsg( NO_CLOSING_QUOTE, token ) ; 
    mark = true ;
    return false ;
  } // else if
  
  else return false ;
  
  return false ;
  
} // IsSTRING()

bool IsNIL( string token ) {
  if ( token.compare( "nil" ) == 0 || token.compare( "#f" ) == 0 ) return true ;
  else return false ; 
} // IsNIL()

bool IsT( string token ) {
  if ( token.compare( "t" ) == 0 || token.compare( "#t" ) == 0 ) return true ;
  else return false ; 
} // IsT()

bool IsLR( string & token, string & line ) {
  if ( token.compare( "(" ) == 0 && line.size() > 0 && line[0] == ')' ) {
    token = token + line[0] ;
    line.erase( 0, 1 ) ;
    gCol++ ;
    return true ;
  } // if
  else return false ; 
} // IsLR()

bool IsSYMBOL( string token ) {
  if ( token.size() == 1 && token[0] == '.' ) return false ; // 2.a is symbol had change
  for ( int i = 0 ; i < token.size() ; i++ ) {
    if ( token[i] != '(' && token[i] != ')' && token[i] != '\'' && token[i] != '\"' ) ;
    else return false ;
  } // for

  if ( token.size() > 0 ) return true ;
  else return false ;
} // IsSYMBOL()

bool Atom( string & token, string & line, bool & mark ) {
  if ( IsINT( token ) ) return true ;
  else if ( IsFLOAT( token ) ) return true ;
  else if ( IsSTRING( token, mark ) ) return true ;
  else if ( IsNIL( token ) ) return true ;
  else if ( IsT( token ) ) return true ;
  else if ( IsLR( token, line ) ) return true ;
  else if ( IsSYMBOL( token ) ) return true ;
  
  return false ; 
} // Atom()

bool S_exp( string & token, string & line, bool & mark ) {
  bool isRead = true ;
  isRead = Get_Token( token, line ) ;
  while ( isRead && token.size() == 0 ) { 
    isRead = Get_Token( token, line ) ; 
  } // while
  
  gNewExpr = false ;
  
  if ( !isRead ) {
    SeterrMsg( NO_MORE_INPUT, token ) ;
    return false ;
  } // if
  
  else if ( Atom( token, line, mark ) ) {
    gTable.push_back( token ) ;
    return true ;
  } // end if
  else if ( token.compare( "(" ) == 0 ) {
    gTable.push_back( token ) ;
    mark = true ; 
    if ( S_exp( token, line, mark ) ) {
      
      bool s_Exp = true ; 
      while ( s_Exp ) {
        mark = false ;
        s_Exp = S_exp( token, line, mark ) ;
      } // while 
      
      if ( mark == true ) return false ;   
      else { // mark == flase
        mark = true ; 
        if ( token.compare( "." ) == 0 ) {
          gTable.push_back( token ) ;
          if ( S_exp( token, line, mark ) ) {
            bool isRead = true ; 
            isRead = Get_Token( token, line ) ;
            while ( isRead && token.size() == 0 ) { 
              isRead = Get_Token( token, line ) ;
            } // while 
            
            if ( !isRead ) {
              SeterrMsg( NO_MORE_INPUT, token ) ;
              return false ;
            } // if
          } // if
          else {
            return false ; 
          } // else 
        } // if
        
        if ( token.compare( ")" ) == 0 ) {
          gTable.push_back( token ) ;
          return true ;
        } // if
        else {
          SeterrMsg( NUEXPECTED_RIGJT_PAREN, token ) ;
          return false ;
        } // else      
      } // else   
    } // if
    else {   
      if ( token.compare( ")" ) == 0 ) {
        gTable[gTable.size()-1] = gTable[gTable.size()-1] + token ;
        return true ;
      } // if
      else return false ;
    } // else
    
  } // end else if
  else if ( token.compare( "\'" ) == 0 ) {
    gTable.push_back( token ) ;
    mark = true ; 
    if ( S_exp( token, line, mark ) ) {
      return true ;
    } // if
    else {
      return false ;
    } // else
    
  } // end else if
  else {
    SeterrMsg( NUEXPECTED_TOKEN, token ) ;
    return false ;
  } // end else
  
} // S_exp()

void PrintT( Treenodes walk ) { // 初始化 
  if ( walk->token == "" ) cout << "space\n" ;
  else {
    cout << walk->token << endl ;
  } // else 
  
  if ( walk == NULL ) return ;
  
  if ( walk->left != NULL ) {
    cout << "left\n" ;
    PrintT( walk->left ) ;
  } // if
  
  if ( walk->right != NULL ) {
    cout << "right\n" ;
    PrintT( walk->right ) ;
  } // if

  return ;
} // PrintT()

void Print( string token, string & space ) {
  if ( token[0] == '\"' && token[token.size() - 1] == '\"' ) {
    for ( int i = 0 ; i < token.size() ; i++ ) {
      if ( token[i] == '\\' ) {
        if ( token[i+1] == '\\' || token[i+1] == '\"' ) {
          i++ ;
          cout << token[i] ;
        } // if
        else if ( token[i+1] == 'n' ) {
          cout << "\n" ;
          i += 1 ;
        } // else if
        else if ( token[i+1] == 't' ) {
          cout << "\t" ;
          i += 1 ;
        } // else if
        else {
          cout << token[i] ;
        } // else
      } // if
      else {
        cout << token[i] ;
      } // else
      
    } // for
    
    cout << endl ;
  } // if
  else if ( token.compare( "\'" ) == 0 ) {
    cout << "( quote\n" ;
    space = space + "  " ;
  } // else if 
  else cout << token << endl ;
  
} // Print()

void Print_Tree( Treenodes walk, string & space ) { // 初始化 
  if ( walk->left != NULL ) {
    if ( walk->left->token.compare( "" ) == 0 ) {
      gList.push_back( "(" ) ;
      space = space + "  " ;
    } // if
    else {
      gList.push_back( walk->left->token ) ;
    } // else  
  } // if
  
  if ( walk->left != NULL && walk->left->token.compare( "" ) == 0 ) {
    Print_Tree( walk->left, space );
  } // if
  
  if ( walk->right != NULL && walk->right->token.compare( "" ) == 0 ) {
    Print_Tree( walk->right, space );
  } // if
  
  if ( walk->right != NULL && walk->right->token.compare( "" ) != 0 && 
       walk->right->token.compare( "nil" ) != 0 ) {
    gList.push_back( "." ) ;
    gList.push_back( walk->right->token ) ;
  } // if
  
  if ( space.size() > 0 && ( ( walk->right != NULL && walk->right->token.compare( "" ) != 0 ) ||
                             ( walk->right == NULL ) ) ) {
    space.erase( 0, 2 ) ;
    gList.push_back( ")" ) ;
  } // if
  
  return ;
} // Print_Tree()

bool Print_List() {
  /*
  for ( int i = 0 ; i < gList.size() ; i++ ) {
    cout << gList[i] << endl ;
  } // for
  */
  
  if ( gList.size() == 3 && gList[0] == "(" && gList[1] == "exit" && gList[2] == ")" ) {
    cout << endl ;
    gList.clear() ;
    return false ;
  } // if
  else {
    bool isParn = false ;
    string space = "" ;
    for ( int i = 0 ; i < gList.size() ; i++ ) {
      if ( gList[i] == "(" ) {
        if ( i-1 >= 0 && gList[i-1] == "(" ) ;
        else cout << space ;
        
        isParn = true ;
        cout << "( " ;
        space = space + "  " ;
      } // if
      else if ( gList[i] == ")" ) {
        space.erase( 0, 2 ) ;
        cout << space << ")\n" ;
      } // else if
      else {
        if ( isParn ) {
          isParn = false ;
        } // if
        else {
          cout << space ; 
        } // else
        
        Print( gList[i], space ) ;
      } // else 
      
    } // for
    
    while ( space.size() > 0 ) {
      space.erase( 0, 2 ) ;
      cout << space << ")\n" ;
    } // while
    
    return true ;
  } // else 
} // Print_List()

void NormalizeFloat( string & str ) {
  if ( str[0] == '+' ) str.erase( 0, 1 ) ;   
  if ( str[0] == '.' ) str.insert( 0, 1, '0' ) ;
  if ( str[0] == '-'  && str[1] == '.' ) str.insert( 1, 1, '0' ) ; 
    
  for ( int j = 0; j < str.size() ; j++ ) { 
    if ( str[j] == '.' ) {
      if ( str.size() - j == 4 ) ;
      else if ( str.size() - j < 4 ) {
        while ( str.size() - j < 4 ) {  
          str = str + '0' ;
        } // while       
      } // else if   
      else if ( str.size() - j > 4 ) { 
        if (  str[j+4] < '5' ) {  
          str.resize( j+4 ) ; 
        } // if       
        else { 
          str.resize( j+4 ) ; 
          int k = str.size() - 1 ; 
          int zero = 0 ; 
          for (  k = k ; k >= 0 ; k-- ) {
            if ( str[k] == '.' || str[k] == '-' ) ;
            else {
              if ( str[k] == '9' ) {
                str[k] = '0' ;
                zero = zero + 1 ;
              } // if
              else {
                str[k] = str[k] + 1 ;
                k = -1 ;
              } // else  
            } // else
          } // for       
          
          if ( str[0] == '-' ) {
            if ( zero == str.size()-2 ) {
              str.insert( 1, 1, '1' ) ; 
            } // if
          } // if
          else {
            if ( zero == str.size()-1 ) {
              str.insert( 0, 1, '1' ) ; 
            } // if           
          } // else        
        } // else        
      } // else if        
          
      j = str.size() ;
    } // if
  } // for
} // NormalizeFloat()

void Quote_Add_Right( int i ) {
  int count = 1 ;
  for ( int j = i+1 ; j < gTable.size() ; j++ ) {
    if ( gTable[j].compare( "(" ) == 0 ) {
      count += 1 ;
    } // if
    else if ( gTable[j].compare( ")" ) == 0 ) {
      count -= 1 ;
      if ( count == 0 ) {
        if ( j + 1 < gTable.size() ) gTable.insert( gTable.begin()+j+1, ")" ) ;
        else gTable.push_back( ")" ) ;
        j = gTable.size() ;
      } // if
      
    } // else if
    
  } // for
  
} // Quote_Add_Right()

void Quote( int i, int q_num ) {
  gTable[i] = "(" ;
  gTable.insert( gTable.begin()+i+1, "quote" ) ;
  i += 2 ;
  if ( gTable[i].compare( "(" ) == 0 ) {
    Quote_Add_Right( i ) ;
    q_num-- ;
  } // if
  else if ( gTable[i].compare( "\'" ) == 0 ) {
    q_num += 1 ;
    Quote( i, q_num ) ;
    Quote_Add_Right( i ) ;
  } // else if 
  else {
    if ( i + 1 < gTable.size() ) gTable.insert( gTable.begin()+i+1, ")" ) ;
    else gTable.push_back( ")" ) ;
    i-- ;
  } // else
    
} // Quote()

void Standardization() {
  for ( int i = 0 ; i < gTable.size() ; i++ ) {
    if ( IsINT( gTable[i] ) ) { // +123 -> 123
      if ( gTable[i][0] == '+' ) gTable[i].erase( 0, 1 ) ;
    } // if
    else if ( IsFLOAT( gTable[i] ) ) { // .1 -> 0.100
      NormalizeFloat( gTable[i] ) ;
    } // else if
    else if ( IsNIL( gTable[i] ) || gTable[i].compare( "()" ) == 0 ) {
      gTable[i] = "nil" ;
    } // else if
    else if ( IsT( gTable[i] ) ) {
      gTable[i] = "#t" ;
    } // else if
    else if ( gTable[i].compare( "\'" ) == 0 ) {
      int q_num = 1 ;
      Quote( i, q_num ) ;
    } // else if
    
  } // for
} // Standardization()

bool Build_tree() {
  /*
  for ( int i = 0 ; i < gTable.size() ; i++ ) {
    cout << gTable[i] << endl ;
  } // for
  */
  
  Standardization() ;
  Treenodes tree, temp ;
  tree = new Tree ;
  Treeinitial( tree ) ;
  int isDot = 0 ;
  int paren_Num = 0 ;
  tree->pre = tree ;
  Treenodes now = tree ;
  for ( int i = 0 ; i < gTable.size() ; i++ ) {
    if ( gTable[i].compare( "(" ) == 0 ) {
      if ( isDot > 0 ) paren_Num++ ;
      temp = new Tree ;
      Treeinitial( temp ) ; 
      
      if ( now->left != NULL ) {
        now->right = temp ;
        temp->pre = now ;
        now = temp ;
        temp = new Tree ;
        Treeinitial( temp ) ;
      } // if
      
      now->left = temp ;
      temp->pre = now ;
      now = temp ;
    } // if
    else if ( gTable[i].compare( "." ) == 0 ) {
      i++ ;
      temp = new Tree ;
      Treeinitial( temp ) ; 
      if ( gTable[i].compare( "(" ) != 0 ) {
        temp->token = gTable[i] ;
      } // if
      else {
        isDot++ ;        
        paren_Num++ ;
      } // else
      
      now->right = temp ;
      temp->pre = now ;
      now = temp ;
    } // else if
    else if ( gTable[i].compare( ")" ) == 0 ) {    
      if ( isDot > 0 ) paren_Num-- ;
      
      if ( isDot > 0 && ( isDot - paren_Num ) == 1 ) {     
        isDot-- ;
        // cout << "I " << i+1 << endl; 
      } // if
      else {
        now = now->pre ;
        while ( ( now->left->token != "" || now->right != NULL ) && now != now->pre ) {
          now = now->pre ;
        } // while
      } // else      
    } // else if
    else {
      temp = new Tree ;
      Treeinitial( temp ) ;
      if ( now->left == NULL ) {
        temp->token = gTable[i] ;
        // cout << "gTable[i]: " << gTable[i] << endl ;
        now->left = temp ;
        temp->pre = now ;
      } // if
      else if ( now->right == NULL ) {
        now->right = temp ;
        temp->pre = now ;
        now = temp ;
        i-- ;
      } // else if
    } // else
  } // for
  
  Treenodes walk = tree ;
  string space = "" ;
  gList.clear() ;
  // PrintT( walk ) ;
  Print_Tree( walk, space ) ;
  return Print_List() ;
} // Build_tree()

bool Grammar( string & line ) {
  bool mark = false ;
  gTable.clear() ;  
  gError.msg = "" ;
  gError.typ = 0 ;    
  string token = "" ; 
  cout << endl << "> " ;
  
  if ( S_exp( token, line, mark ) ) {
    if ( line.size() != 0 ) { // one line have over one S-exp
      // cout << "true 1 " << endl ;
      // for ( int i = 0 ; i < gTable.size() ; i++ ) cout << gTable[i] << endl ; // print test
      if ( !Build_tree() ) return false ;
      gCol = 0 ;
      gRow = 1 ;
      gNewExpr = true ;
      // cout << "line : " << line << endl ;
      if ( ! Grammar( line ) ) {
        return false ;
      } // if
    } // if
    else {
      // cout << "true 2 " << endl ; 
      // for ( int i = 0 ; i < gTable.size() ; i++ ) cout << gTable[i] << endl ; // print test
      if ( !Build_tree() ) return false ;
      return true ;
    } // else 
  } // if
  else if ( gError.msg.size() != 0 ) {
    cout << gError.msg << endl ; 
    if ( gError.typ == NO_MORE_INPUT ) {
      return false ;
    } // if
    else {
      return true ;     
    } // else
  } // else if
  
  return true ;
} // Grammar()


int main() {
  bool haveInput = true ;  
  Read_Line( gTestNum ) ;
  cout << "Welcome to OurScheme!" << endl ;

  while ( haveInput ) {
    gCol = 0, gRow = 0 ;
    string line = "" ;
    gNewExpr = false ;
    haveInput = Grammar( line ) ;
  } // while 
  
  cout << "Thanks for using OurScheme!" << endl ; 
  
} // main() 


