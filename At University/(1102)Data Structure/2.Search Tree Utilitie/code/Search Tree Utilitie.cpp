// 10927207_蒲品憶_資訊二乙 10927248_連翊安 
#include <iostream>
#include <vector> // vector 
#include <fstream> // 讀檔 
#include <cstdlib> // atoi
#include <cstring>

using namespace std ; 

struct Data {
	int no = 0 ; // 序號 
	string schoolNo = "" ; // 學校代號 
	string schoolName = "" ; // 學校名稱 
	string deptNo = "" ; // 科系代碼 
	string deptName = "" ; // 科系名稱 
	string advancedStudy = "" ; // 日間/進修別 
	string level = "" ; // 等級別 
	string student = "" ; // 學生數 
	string teacher = "" ; // 教師數 
	string graduate = "" ; // 上年度畢業生人數 
	string country = "" ; // 縣市名稱 
	string system = "" ; // 體系別 
};

struct schData {
	string schoolName =  "" ;
	vector<int> no ;
};
 
struct deptData {
	string deptName =  "" ;
	vector<int> no ;
};

bool sfile = true ; // no file or no data 
string command ;
vector<Data> tempList ;
string TurnToNumber( string temp ) { // 將間位數字，例如 "1,199" 轉成 1199 
	string number = "" ;
	for ( int j = 0 ; j < temp.length() ; j++ ) 
		if ( temp[j] != '\"' &&  temp[j] != ',' )  
			number =  number + temp[j] ;	
	return number ;	
} // int TurnToNumber()

void ReadTxt() { // 讀檔
	sfile = true ;
	tempList.clear() ;
	string num, name ;
	ifstream file ;
	cout << endl << "Input a file number ([0] Quit): " ;
	while ( cin >> num ) {
		name = "input" + num + ".txt" ;
		file.open(name, ios_base::in) ; 
		if ( ( num.compare("0") == 0 ) ) {
			sfile = false ;
			break ;
		} // if
		
    	if (file.is_open()) break ;
		else cout << "\n### " << name << " does not exist! ###\n" ;
		cout << endl << "Input a file number(101,102,103,......): " ;	  		
	} // while 

	if (file.is_open()) {	
    	int count = 0 ;
    	Data oneData ;
		string line ;
 		getline( file, line ) ;
 		getline( file, line ) ;
 		getline( file, line ) ;
		while ( getline( file, oneData.schoolNo, '\t' ) ) {
			count++ ; // 計算序號 
			oneData.no = count ;  
			getline( file, oneData.schoolName, '\t' ) ;
			getline( file, oneData.deptNo, '\t' ) ;
			getline( file, oneData.deptName, '\t' ) ;
			getline( file, oneData.advancedStudy, '\t' ) ;			
			getline( file, oneData.level, '\t' ) ;
			getline( file, oneData.student, '\t' ) ;
			if ( oneData.student[0] == '\"' ) oneData.student = TurnToNumber( oneData.student ) ;
			
			getline( file, oneData.teacher, '\t' ) ;
			getline( file, oneData.graduate, '\t' ) ;
			getline( file, oneData.country, '\t' ) ;							
			getline( file, oneData.system, '\n') ;												  
			
			tempList.push_back(oneData) ; // oneData into tempList		
		} // while
		
		file.close() ; // 關閉檔案 
		if ( !tempList.size()) {
			cout << "### Get nothing from the file " << name << "!###" << endl ;
			cout << endl << "There is no data!" << endl ;
			sfile = false ;
		} // if		
		//Print_All_Data( tempList ) ;		
	} // if
} // ReadTxt()

void Print_All_Data( vector<Data> list ) { // 印資料 
	for ( int i = 0; i < list.size() ; i++ ) {
		cout << i+1 << ": [" << tempList[i].no << "] " << tempList[i].schoolName << "\t" << tempList[i].deptName << "\t" << tempList[i].advancedStudy << "\t" << tempList[i].level << "\t" << tempList[i].student << endl ;					
	} //for
} // Print_All_Data()

void PrintDatas( vector<int> noList, int count ) { // 印出查詢資料 
	for( int i = 0 ; i < noList.size() ; i++ ) {
		cout << count+1 << ": [" << noList[i] << "] " << tempList[noList[i]-1].schoolName << "\t" << tempList[noList[i]-1].deptName << "\t" << tempList[noList[i]-1].advancedStudy << "\t" << tempList[noList[i]-1].level << "\t" << tempList[noList[i]-1].student << endl ;					
		count++ ;			
	} // for
} // PrintData()

void compare( string college, string department, vector<int> noCollege, vector<int> noDepart ) { // 比較 學校名稱資料 與 系所名稱資料 有無相無序號 並印出 
	int count = 0 ;
	if ( department == "*" ) PrintDatas( noCollege, count ) ;
	else if ( college == "*" ) PrintDatas( noDepart, count ) ;	
	else {
		for ( int i = 0 ; i < noCollege.size() ; i++ ) {
			for ( int j = 0 ; j < noDepart.size() ; j++ ) {
				if ( noCollege[i] == noDepart[j] ) {
					cout << count+1 << ": [" << noCollege[i] << "] " << tempList[noCollege[i]-1].schoolName << "\t" << tempList[noCollege[i]-1].deptName << "\t" << tempList[noCollege[i]-1].advancedStudy << "\t" << tempList[noCollege[i]-1].level << "\t" << tempList[noCollege[i]-1].student << endl ;					
					count++ ;	
				} // if
			} // for	
		} // for
	} // else
} // compare()  


class TwoThreeTree {
	private:
		struct TreeNode {
			vector<schData> item ;
			TreeNode * left ;	
			TreeNode * middle ;
			TreeNode * right ;
			TreeNode * temp ;
		};
	
		typedef TreeNode * TreePtr ;	
	
	public:		
		TreePtr tempPtr = NULL ;					
		TreePtr headPtr = NULL ;
		float tempmiddle ; // 取出三元素中 大小位於中間的元素  
		string afterDirection = "" ; // 判斷 now 節點 是從左邊/中間/右邊 走上來的 
		bool inToTree = false ; // 新元素是否已經被加入2-3樹中
			
		void BuildTree() {		
			clear( headPtr ) ; // 清空整棵樹 
			headPtr = NULL ; 		
			float oneData ;
			TreePtr pre = NULL, now = NULL ;
			for ( int i = 0 ; i < tempList.size() ; i++ ) {
				oneData.no.push_back( tempList[i].no ) ;
				oneData.schoolName = tempList[i].schoolName ;
				if ( i == 0 ) AddNode( headPtr, oneData ) ; // first Data
				else {
					pre = headPtr ;
					now = headPtr ;
					recurAddNewElement( now, pre, oneData ) ;
					headPtr = pre ; // 因為樹根有時會被變動，所以並須將headPtr指到(pre)樹根的地方 
				} // else
				
				oneData.no.clear() ;
				tempmiddle.schoolName = "" ;
				tempmiddle.no.clear() ; 
			} // for	
			
			cout << "Tree height = " << countHeight( headPtr ) << endl ; // 印高度 
			PrintRoot( headPtr ) ; // 印出樹根 	
		} // BuildTree()
		
		void recurAddNewElement( TreePtr now, TreePtr & pre, schData oneData ) { // add new element in correct place 		
			if ( now->item.size() == 1 && oneData.schoolName == now->item[0].schoolName ) { // one element same name, add no
				now->item[0].no.push_back( oneData.no[0] ) ;	
				return ;				
			} // if					
			else if ( now->item.size() == 2 && ( oneData.schoolName == now->item[0].schoolName || oneData.schoolName == now->item[1].schoolName ) ) { 
				// two element same name, add no
				if ( oneData.schoolName == now->item[0].schoolName ) now->item[0].no.push_back( oneData.no[0] ) ;	
				if ( oneData.schoolName == now->item[1].schoolName ) now->item[1].no.push_back( oneData.no[0] ) ;
				return ;	
			} // if									
			else if ( now ->left == NULL && now->middle == NULL && now->right == NULL && now->temp == NULL ) {
				// two element different name, add new element	
				now->item.push_back( oneData ) ; 		
				if ( now->item.size() == 2 ) {
					if ( now->item[0].schoolName > now->item[1].schoolName ) swap( now->item[0], now->item[1] ) ;						
				} // if							
				else if ( now->item.size() == 3 ) { // three element different name, find middle element, and spilt			
					Findtempmiddle( now ) ; // 排序 找到三元素中 大小位於中間的元素  
					Spilt( pre, now ) ;	// 分裂節點		
				} // if
				
				return ;	
			} // if
				
			if ( now->item.size() == 1 ) { // one Node內 只有一個元素, 還未加入新節點 
				// 如果新元素小於 now節點的元素 往左下走		
				if ( oneData.schoolName < now->item[0].schoolName ) recurAddNewElement( now->left, now, oneData ) ; 
				// 如果新元素大於 now節點的元素 往右下走		
				else if ( oneData.schoolName > now->item[0].schoolName ) recurAddNewElement( now->right, now, oneData ) ; 						
			} // if			
			else if ( now->item.size() == 2 ) { // one Node內 有兩個元素, 還未加入新節點 
				//如果新元素皆小於 now節點的元素 往左下走
				if ( oneData.schoolName < now->item[0].schoolName ) recurAddNewElement( now->left, now, oneData ) ;	
				//如果新元素介於 now節點的元素之間 往中下走	
				else if ( oneData.schoolName > now->item[0].schoolName && oneData.schoolName < now->item[1].schoolName ) recurAddNewElement( now->middle, now, oneData ) ; // middle								
				// 如果新元素皆大於 now節點的元素 往右下走
				else if ( oneData.schoolName > now->item[1].schoolName ) recurAddNewElement( now->right, now, oneData ) ; // right				 				
			} // else if
			
			if ( tempmiddle.schoolName != "" ) { // tempmiddle內有資料，代表需繼續往上分裂 
				Findtempmiddle( now ) ; // 排序 找到三元素中 大小位於中間的元素   
				Spilt( pre, now ) ;	// 分裂節點	
			} // if			
		} // recurFindNode()

		void Findtempmiddle( TreePtr now ) { // 排序 找到三元素中 大小位於中間的元素  	
		    // 一開始 新加入的元素(或從下面的節點上來的中間元素) 會被存在現在節點所有元素的最尾端	
			if ( now->item[2].schoolName < now->item[0].schoolName ) { // 新加入的元素 皆小於 節點中的所有元素  
				swap( now->item[0], now->item[2] ) ; // 將原本最小的元素 跟 新加入的元素交換 
				afterDirection = "left" ; // 可以知道 now 是從左下走上來的 
			} // if		
			else if ( now->item[2].schoolName > now->item[1].schoolName ) {	// 新加入的元素 皆大於 節點中的所有元素  
				swap( now->item[1], now->item[2] ) ; // 將原本最大的元素 跟 新加入的元素交換
				afterDirection = "right" ; // 可以知道 now 是從右下走上來的
			} // else if
			else afterDirection = "middle" ; // 可以知道 now 是從中下走上來的				

			tempmiddle = now->item[2] ; // 將大小在中間的元素 存到  tempmiddle
			now->item.pop_back() ; // 刪除最末端元素 
		} // Findtempmiddle()		

		void AddNode( TreePtr & ptr, schData oneData ) { // 新增節點 
			ptr = new TreeNode ;	
			ptr->item.push_back( oneData ) ;
			ptr->right = NULL ;
			ptr->middle = NULL ;
			ptr->left = NULL ;
			ptr->temp = NULL ;			
		} // AddNode()

		void Spilt( TreePtr & pre, TreePtr now ) {
			// 此時三元素中的中間元素 已被存到 tempmiddle 所以 now中只有兩個元素(中間元素在Findtempmiddle()已從now節點中刪除) 
			tempPtr = NULL ;
			TreePtr middlePtr = NULL ;	
			AddNode( tempPtr, now->item[1] ) ; // 新增節點 節點內元素是 [now節點中 較大的元素] 
			now->item.pop_back() ;
													
			if ( now == headPtr && pre == headPtr ) { // 如果已經跑到樹根 
				AddNode( middlePtr, tempmiddle ) ; // 就 新增節點 節點內元素是 [tempmiddle(三元素的中間元素)]  
				middlePtr->left = now ;                          
				middlePtr->right = tempPtr ;                
				pre = middlePtr ; // 將pre指向樹根    
				tempmiddle.schoolName = "" ; // 不須再分裂 
			}  // if 
			else if ( pre->item.size() == 1 ) {	// 如果pre節點中 只有一個元素
				// pre->left等於now代表是pre的左邊的小孩，故將 tempPtr 連到  pre的 middle 
				if ( pre->left->item[0].schoolName == now->item[0].schoolName ) pre->middle = tempPtr ;
				else if ( pre->right->item[0].schoolName == now->item[0].schoolName ) {		
					// pre->left等於now代表是pre的右邊的小孩，需先將 now 連到 pre的 middle 	
					// 然後 tempPtr 連到 pre的 right
					pre->middle = now ;					
					pre->right = tempPtr ;																	
				} // else if
				
				pre->item.push_back( tempmiddle ) ; // 將now中大小為中間的元素  加入到 pre中 
				if ( pre->item[0].schoolName > pre->item[1].schoolName ) swap( pre->item[0] , pre->item[1] ) ; // 排序 
				tempmiddle.schoolName = "" ; // 不須再分裂 
			} // if			
			else if ( pre->item.size() == 2 ) {	// 如果pre節點中 有兩個元素				
				pre->temp = tempPtr ; // 直接將 tempPtr 接在 pre 的  temp 中 
				pre->item.push_back( tempmiddle ) ;	// 將now中大小為中間的元素  加入到 pre中 	
			} // else if	
			
			
			if ( now->left != NULL && now->right != NULL && now->middle != NULL && now->temp != NULL ) { // Spilt not leaf			
				if ( afterDirection == "left" ) { // now是由左下走上來 代表之前已經分裂過一次的節點位於 左下 
					// 須將now的 middle、right 分配給 tempPtr              
					tempPtr->left = now->middle ;                	
					tempPtr->right = now->right ;          
					now->right = now->temp ;               
				} // if
				if ( afterDirection == "middle" ) { // now是由中下走上來 代表之前已經分裂過一次的節點位於 中下 
					// 須將now的 temp、right 分配給 tempPtr   
					tempPtr->left = now->temp ;
					tempPtr->right = now->right ;	
					now->right = now->middle ;												
				} // if
				if ( afterDirection == "right" ) { // now是由右下走上來 代表之前已經分裂過一次的節點位於 右下 
					// 須將now的 right、temp 分配給 tempPtr  
					tempPtr->left = now->right ;
					tempPtr->right = now->temp ;	
					now->right = now->middle ;				 
				} // if		
				
				// 因為 now 分裂過後只會剩下，左右兩個小孩，所以須將剩下的節點設NULL 
				now->temp = NULL ;
				now->middle = NULL ;	
			} // if
		} // Spilt()
	
		int countHeight( TreePtr walk ) { // 算出樹高 
			 if( walk == NULL ) return 0;
		     else return max(countHeight(walk->left),countHeight(walk->right)) + 1;
		} // countHeight()
		
		void PrintRoot( TreePtr walk ) { // 印出樹根 
			int count = 1 ;
			if ( headPtr->item.size() == 1 ) // 如果節點中只有一個元素，就直接印 
				for ( int i = 0 ; i < headPtr->item[0].no.size() ; i++, count++ ) 
					cout << count << ": [" << headPtr->item[0].no[i] << "] " << tempList[headPtr->item[0].no[i]-1].schoolName << "\t" << tempList[headPtr->item[0].no[i]-1].deptName << "\t" << tempList[headPtr->item[0].no[i]-1].advancedStudy << "\t" << tempList[headPtr->item[0].no[i]-1].level << "\t" << tempList[headPtr->item[0].no[i]-1].student << endl ;
					
			else if ( headPtr->item.size() == 2 ) { // 若有兩個元素，先判斷誰的序號在前面先印 
				if ( headPtr->item[0].no[0] < headPtr->item[1].no[0] ) { // 大小較小的元素序號在前，先印較小元素-->在印大的 
					for ( int i = 0 ; i < headPtr->item[0].no.size() ; i++, count++ ) 
						cout << count << ": [" << headPtr->item[0].no[i] << "] " << tempList[headPtr->item[0].no[i]-1].schoolName << "\t" << tempList[headPtr->item[0].no[i]-1].deptName << "\t" << tempList[headPtr->item[0].no[i]-1].advancedStudy << "\t" << tempList[headPtr->item[0].no[i]-1].level << "\t" << tempList[headPtr->item[0].no[i]-1].student << endl ;
					for ( int i = 0 ; i < headPtr->item[1].no.size() ; i++ ) 
						cout << count << ": [" << headPtr->item[1].no[i] << "] " << tempList[headPtr->item[1].no[i]-1].schoolName << "\t" << tempList[headPtr->item[1].no[i]-1].deptName << "\t" << tempList[headPtr->item[1].no[i]-1].advancedStudy << "\t" << tempList[headPtr->item[1].no[i]-1].level << "\t" << tempList[headPtr->item[1].no[i]-1].student << endl ;			
				} // if
				
				else if ( headPtr->item[0].no[0] > headPtr->item[1].no[0] ) { // 和上面相反 
					for ( int i = 0 ; i < headPtr->item[1].no.size() ; i++, count++ ) 
						cout << count << ": [" << headPtr->item[1].no[i] << "] " << tempList[headPtr->item[1].no[i]-1].schoolName << "\t" << tempList[headPtr->item[1].no[i]-1].deptName << "\t" << tempList[headPtr->item[1].no[i]-1].advancedStudy << "\t" << tempList[headPtr->item[1].no[i]-1].level << "\t" << tempList[headPtr->item[1].no[i]-1].student << endl ;			
					for ( int i = 0 ; i < headPtr->item[0].no.size() ; i++, count++ ) 
						cout << count << ": [" << headPtr->item[0].no[i] << "] " << tempList[headPtr->item[0].no[i]-1].schoolName << "\t" << tempList[headPtr->item[0].no[i]-1].deptName << "\t" << tempList[headPtr->item[0].no[i]-1].advancedStudy << "\t" << tempList[headPtr->item[0].no[i]-1].level << "\t" << tempList[headPtr->item[0].no[i]-1].student << endl ;		
				} // else if
			} // else if 
		} // PrintRoot()
			
		void clear( TreePtr & walk ) { // 刪除整棵樹 
			if ( walk == NULL ) return ;
			
			clear( walk->left ) ;
			clear( walk->middle ) ;
			clear( walk->right ) ;
			delete walk ;
		} // clear()		
		
		void search( string schoolname, vector<int> & noCollege, TreePtr walk ) {
			if ( walk == NULL ) return ;
			else if ( walk != NULL ) {
		 		for ( int j = 0 ; j < walk->item.size() ; j++ ) {	 			
					if ( walk->item[j].schoolName == schoolname ) {
						for ( int i = 0 ; i < walk->item[j].no.size() ; i++ ) noCollege.push_back( walk->item[j].no[i] ) ;						
						return ;	 
					} // if
				} // for
			} // if
			
			if ( schoolname < walk->item[0].schoolName ) search( schoolname, noCollege, walk->left ) ;
			else if ( walk->item.size() == 2 && schoolname > walk->item[0].schoolName && schoolname < walk->item[1].schoolName  ) search( schoolname, noCollege, walk->middle ) ;
			else if ( walk->item.size() == 2 && schoolname > walk->item[1].schoolName ) search( schoolname, noCollege, walk->right ) ;		
			else if ( schoolname > walk->item[0].schoolName ) search( schoolname, noCollege, walk->right ) ;
		} // search()		
};


class AVLTree {
	private:
		struct TreeNode {
			deptData item ;
			TreeNode * left ;
			TreeNode * right ;
		};
		
		typedef TreeNode * Ptr ;	
				
	public:
		Ptr headPtr = NULL ;
		bool sameDeptName = false ;
		
		void BuildAVL() {
			clear( headPtr ) ; // 清空整棵樹   
			headPtr = NULL ;			
			deptData oneData ;
			Ptr after = NULL ;
			for ( int i = 0 ; i < tempList.size() ; i++ ) {
				oneData.no.push_back( tempList[i].no ) ;
				oneData.deptName = tempList[i].deptName ;
				if ( i == 0 ) headPtr = AddNode( oneData ) ; // first Data
				else {
					after = headPtr ;
					RecurAddNewElement( headPtr, after, oneData ) ;	
				} // else
				
				oneData.no.clear() ;
				sameDeptName = false ;
			} // for


			cout << "Tree height = " << countHeight( headPtr ) << endl ; // 印樹高 
			PrintRoot( headPtr ) ; // 印樹根				
		} // BuildAVL()
		
		void RecurAddNewElement( Ptr & now, Ptr & after, deptData oneData ) {	
			if ( now == NULL ) { // now為NULL 新增節點 
				now = AddNode( oneData ) ;
				after = now ;
				return ;
			} // if
			
			if ( oneData.deptName == now->item.deptName ) { // 新元素 等於 節點元素 add, no 
				now->item.no.push_back( oneData.no[0] ) ;	
				sameDeptName = true ;
				return ;
			} // if					
			else if ( oneData.deptName < now->item.deptName ) { // 新元素 小於 節點元素 add, no 
				RecurAddNewElement( now->left, after, oneData ) ; // 往左遞迴 
				if ( now->left == NULL ) now->left = after ;  
			} // else if			
			else if ( oneData.deptName > now->item.deptName ) {
				RecurAddNewElement( now->right, after, oneData ) ; // 往右遞迴			
				if ( now->right == NULL ) now->right = after ;
			} // else if

			if ( sameDeptName == false ) RotationToBalance( now, after ) ; // 處理AVL樹不平衡			
		} // recurAddNewElement()

		void RotationToBalance( Ptr & now, Ptr & after ) { // 處理AVL樹不平衡 
			int nowHighGap = countHeight( now->left ) - countHeight( now->right ) ;	// 算出現在節點的左右樹高差		
			if ( nowHighGap == 2 || nowHighGap == -2 ) { // 不平衡 
				int afterHighGap = countHeight( after->left ) - countHeight( after->right ) ; // 算出after節點的左右樹高差				
				if ( nowHighGap == 2 && ( afterHighGap == 1 || afterHighGap == 0 ) ) // ++ LL 
					now = rotateLL( now ) ;				
				else if ( nowHighGap == -2 && ( afterHighGap == -1 || afterHighGap == 0 ) ) // -- RR 
					now = rotateRR( now ) ;								
				else if ( nowHighGap == 2 && afterHighGap == -1 ) // +- LR 
					now = rotateLR( now ) ; 			
				else if ( nowHighGap == -2 && afterHighGap == 1 )  // -+ RL 
					now = rotateRL( now ) ; 
			} // if
			
			after = now ;					
		} // RotationToBalance()

		Ptr rotateLL( Ptr x ) {  
			Ptr y = x->left ;
			x->left = y->right ; 
			y->right = x ;
			return y ;
		} // AVLTree::rotateLL()	
			
		Ptr rotateRR( Ptr x ) {
			Ptr y = x->right ;
			x->right = y->left ; 
			y->left = x ;
			return y ;
		} // AVLTree::rotateRR()

		Ptr rotateLR( Ptr x ) {
			x->left = rotateRR( x->left ) ;
			return rotateLL( x ) ;
		} // AVLTree::rotateRR()		

		Ptr rotateRL( Ptr x ) {
			x->right = rotateLL( x->right ) ;
			return rotateRR( x ) ;
		} // AVLTree::rotateRR()
		
		Ptr AddNode( deptData oneData ) {
			Ptr tempPtr = new TreeNode ; 
			tempPtr->item = oneData ;
			tempPtr->left = NULL ;
			tempPtr->right = NULL ;
			return tempPtr ;
		} // AddNode()
		
		int countHeight( Ptr walk ) {
			if ( walk == NULL ) return 0;
    	 	else return max(countHeight(walk->left),countHeight(walk->right)) + 1 ;			
		} // countHeight()
		
		void PrintRoot( Ptr walk ) {
			for ( int i = 0 ; i < headPtr->item.no.size() ; i++ ) 
				cout << i+1 << ": [" << headPtr->item.no[i] << "] " << tempList[headPtr->item.no[i]-1].schoolName << "\t" << tempList[headPtr->item.no[i]-1].deptName << "\t" << tempList[headPtr->item.no[i]-1].advancedStudy << "\t" << tempList[headPtr->item.no[i]-1].level << "\t" << tempList[headPtr->item.no[i]-1].student << endl ;
		} // PrintRoot()					

		void clear( Ptr & walk ) {
			if ( walk == NULL ) return ;
			
			clear( walk->left ) ;
			clear( walk->right ) ;
			delete walk ;
		} // clear()
		
		void search( string deptname, vector<int> & noDepart, Ptr walk ) {	
			if ( walk == NULL ) return ;
			else if ( walk != NULL ) {
			 	if ( walk->item.deptName == deptname ) {
					for ( int i = 0 ; i < walk->item.no.size() ; i++ ) noDepart.push_back( walk->item.no[i] ) ;  								
					return ;	 
				} // if			 
			} // if
			
			if ( deptname < walk->item.deptName ) search( deptname, noDepart, walk->left ) ;
			else if ( deptname > walk->item.deptName ) search( deptname, noDepart, walk->right ) ;	
		} // search()
};


int main() {
	TwoThreeTree twoThreeTree ;
	AVLTree avlTree ;
	while ( true ) {
		cout << endl << "****** Search Tree Utilities ******" ;
		cout << endl << "* 0. QUIT                         *" ;
		cout << endl << "* 1. Build Two-Three Tree         *" ;
		cout << endl << "* 2. Build AVL Tree               *" ;	
		cout << endl << "* 3. Intersection Query           *" ;
		cout << endl << "***********************************" ;
		cout << endl << "Input a choice(0, 1, 2, 3): " ;
		cin >> command ;

		
		if ( command.compare("1") == 0 ) {
			ReadTxt() ; 
			if ( command.compare("1") == 0 && sfile == true ) twoThreeTree.BuildTree() ; // 建立2-3樹 		
		} // if
		
		else if ( command.compare("2") == 0 ) {
			if ( twoThreeTree.headPtr == NULL || sfile != true ) cout << "\n### Choose 1 first. ###\n" ;
			else avlTree.BuildAVL() ; // 建立AVL樹 				
		} // if
		
		else if ( command.compare("3") == 0 ) {
				if ( twoThreeTree.headPtr == NULL || avlTree.headPtr == NULL || sfile != true ) cout << "\n### Choose 1 and 2 first. ###\n" ;
				else {	
					vector<int> noCollege, noDepart ; // 學校名稱資料 與 系所名稱資料
					string college, department ;
					cout << "Enter a college name to search [*]: " ;
					cin >> college ;
					cout << "Enter a department name to search [*]: " ;
					cin >> department ;
					cout << endl ;	
		
					if ( college == "*" && department == "*" ) Print_All_Data( tempList ) ;
					else {
						if ( department != "*" ) avlTree.search( department, noDepart, avlTree.headPtr ) ;										
						if ( college != "*" ) twoThreeTree.search( college, noCollege, twoThreeTree.headPtr ) ;  
						compare( college, department, noCollege, noDepart ) ;												
					} // else	
					
					noCollege.clear() ;
					noDepart.clear() ;	
				} // else
		} // else if
		
		else if ( command.compare("0") == 0 ) return 0 ;
		
		else cout << endl << "Command does not exist!" << endl ;
	} // while 	
} // main()




