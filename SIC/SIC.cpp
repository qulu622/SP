#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <ctype.h>

using namespace std;
string t1[100]; // instruction
string t2[100]; // pseudo && extra
string t3[100]; // register
string t4[100]; // delimiter
string t5[100]; // symbol
string t6[100]; // integer && real
string t7[100]; // string
string lasttoken ; // 放上個token用 , 用來判斷是否為字串
string last2token; // 放上上個token用, 用來判斷X''的問題
int tmp[100]; // 放x,y用
typedef char str[15] ; // 用來放檔名 talbe1,talbe2,table3,table4
int getToken(string input); // 切token
bool isDelimiter(char temp); // 判斷是否為Delimiter
bool isInTable(string token, int & x, int & y); // 判斷是否在table出現過了
int isWhat(string token); // 判斷是symbol , integer, string? return 5,6,7
void hashing(string token, int & x, int & y, int type); // 把token放進table中

int main(){
  str table[4]; // 放4個table的檔名
  strncpy(table[0], "Table1.table", 15) ;
  strncpy(table[1], "Table2.table", 15) ;
  strncpy(table[2], "Table3.table", 15) ;
  strncpy(table[3], "Table4.table", 15) ;

  string input[100]; // 放input Line

  for ( int i = 0 ; i < 4 ; i++ ) { // 從table1讀到table4
    ifstream fin(table[i]); // 讀檔
    if (!fin)
      cout << "fin fail" << endl ; // 是否讀檔失敗, 若失敗就輸出"fail"

    switch( i ) {
      case 0: // 讀table1
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t1[j] ;
        for ( int k = 1 ; k < 100 ; k++ ) { // 將此table內的資料轉為大寫
          for ( int l = 0 ; l < t1[k].size() ; l++)
            t1[k][l] = toupper(t1[k][l]);
        }
      case 1: // 讀table2
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t2[j] ;
      case 2: // 讀table3
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t3[j] ;
      case 3: // 讀table4
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t4[j] ;
    }

    fin.close(); // 關檔
  }

  for ( int i = 0 ; i < 100 ; i++) { // 把t5,t6,t7的資料都初始化,設為空字串
    t5[i] = "";
    t6[i] = "";
    t7[i] = "";
  }

  str inputname ; // input檔的名字
  str outputname ; // output檔的名字
  cout << "input filename(+.txt): " ;
  cin >> inputname ;
  cout << "output filename(+.txt): " ;
  cin >> outputname ;


  // strcpy(inputname, "SIC_input.txt") ; // 測試用
  ifstream fin(inputname); // 讀檔
  ofstream fout(outputname); // 輸出檔案
  if (!fin)
    cout << "fin fail" << endl ; // 是否讀檔失敗, 若失敗就輸出"fail"
  else {
    // cout << "fin success" << endl ;
    int n = 0 ; // 記input有幾行
    for ( int i = 0 ; !fin.eof() ; i++ ) {
      getline(fin,input[i]); // 讀一行
      n = i ; // 共n行input
    }

    for ( int i = 0 ; i < n ; i++ ) {
      cout << input[i] << endl ; // 印出一行input
      fout << input[i] << endl ; // 輸出檔案
      int num = getToken(input[i]); // getToken並印出結果
      for ( int j = 0 ; j < num ; j = j+2 )
        fout << "(" << tmp[j] << "," << tmp[j+1] << ")" ;
      cout << endl ;
      fout << endl ;
      lasttoken = "" ;
      last2token = "" ;
    }

  }

  fin.close(); // 關檔
  return 0 ;
}

int getToken(string input){
  int len = input.size(); // 計算input的長度, 為了char array的大小所設計
  char ary[len] ; // 宣告一個char array 來放input
  strcpy(ary,input.c_str()); // 將input的內容copy至ary
  // cout << ary << endl ;
  int x = 0 ; // (x,y)
  int y = 0 ; // (x,y)
  bool get = false ; // 是否得到一個token了?
  string token = "" ; // 放token
  int i = 0 ; // 用來跑while
  int n = 0 ; // tmp index
  while( i < len ) {
    while ( !get ) {
      if ( ary[i] != ' ' && ary[i] != '\t' && ary[i] != '\n' ) {
        if ( isDelimiter(ary[i]) && token == "") { // 單獨Delimiter
          if (ary[i] == '.') // 處理註解
            return n ;
          get = true ;
          token = token + ary[i];
          i++;
        }
        else if ( isDelimiter(ary[i]) && token != "" ) // 遇到Delimiter, 前面完成一個token
          get = true ;
        else if ( !isDelimiter(ary[i])) { // 非Delimiter, 放入token中
          token = token + ary[i] ;
          i++;
        }
      }
      else if (  (ary[i] == ' ' || ary[i] == '\t' || ary[i] == '\n') && token != "") { // 遇到空白, 完成一個token
        get = true ;
        i++;
      }
      else if ( (ary[i] == ' ' || ary[i] == '\t' || ary[i] == '\n') && token == "" ) // 遇到空白, 繼續找下一個
        i++;
      // cout << i-1 << ": " << token << endl ;
      if ( i == len )
        get = true ;
    }

    if ( get && ( token == "C" || token == "c" ) && ary[i] == '\'') { // 處理C'的問題
      token = "" ;
      get = false ;
    }


    if ( get ) {
      if ( isInTable(token,x,y) ) { // 看是否在table中
        cout << "(" << x << "," << y << ")" ;
        tmp[n] = x ;
        tmp[n+1] = y ;
        n = n + 2 ;
      }

      else { // 不在table中,則利用hashing
        int type = isWhat(token) ; // 先看它屬於哪個table
        hashing(token,x,y,type); // hashing
        cout << "(" << x << "," << y << ")" ;
        tmp[n] = x ;
        tmp[n+1] = y ;
        n = n + 2 ;
      }
      x = 0 ;
      y = 0 ;
      last2token = lasttoken ; // 紀錄上上個token, 處理X用
      lasttoken = token ; // 紀錄上一個token, 處理字串用
      token = "" ;
      get = false ;
    }
  }
  return n ;
}

bool isDelimiter(char temp) {
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( temp == t4[i][0] )
      return true ;
  }
  return false ;
}

bool isInTable(string token, int & x, int & y){
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( token == t1[i] ) {
      x = 1 ;
      y = i ;
      return true ;
    }
  }
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( token == t2[i] ) {
      x = 2 ;
      y = i ;
      return true ;
    }
  }
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( token == t3[i] ) {
      x = 3 ;
      y = i ;
      return true ;
    }
  }
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( token == t4[i] ) {
      x = 4 ;
      y = i ;
      return true ;
    }
  }
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( token == t5[i] && lasttoken != "\'") { // 如果上個token是'的話, 它屬於table7
      x = 5 ;
      y = i ;
      return true ;
    }
  }
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( token == t6[i] ) {
      x = 6 ;
      y = i ;
      return true ;
    }
  }
  for ( int i = 0 ; i < 100 ; i++ ) {
    if ( token == t7[i] ) {
      x = 7 ;
      y = i ;
      return true ;
    }
  }
  return false ;
}

int isWhat(string token) {
  int len = token.size(); // 計算token的長度, 為了char array的大小所設計
  char ary[len] ; // 宣告一個char array 來放token
  strcpy(ary,token.c_str()); // 將token的內容copy至ary
  bool isDigit = true ;
  for ( int i = 0 ; i < len ; i++) {
    if ( !isdigit(ary[i]) )
      isDigit = false ;
  }
  if ( last2token == "X" || last2token == "x")
    return 6 ;

  if ( isDigit )
    return 6 ;
  else {
    if ( lasttoken == "\'" )
      return 7 ;
    else
      return 5 ;
  }
}

void hashing(string token, int & x, int & y, int type) {
  int len = token.size(); // 計算token的長度, 為了char array的大小所設計
  char ary[len] ; // 宣告一個char array 來放token
  strcpy(ary,token.c_str()); // 將token的內容copy至ary
  int hash = 0 ;
  bool success = false ;

  if ( type == 5 ) {
    for ( int i = 0 ; i < len ; i++ ) // 將此token轉為大寫
      ary[i] = toupper(ary[i]) ;
  }

  token.assign(ary);

  for ( int i = 0 ; i < len ; i++)
    hash = hash + (int)ary[i] ; // 把字元的ascii相加

  hash = hash % 100 ; // 除100取餘數

  if ( type == 5 ) {
    x = 5 ;
    while ( !success ) {
      if ( t5[hash] == "") {
        t5[hash] = token ;
        y = hash ;
        success = true ;
      }
      else
        hash++ ;


      if ( hash >= 100)
        hash = 0 ;

      /*
      for ( int k = 0 ; k < 100 ; k++ ) { // 將此table內的資料轉為大寫
        for ( int l = 0 ; l < t5[k].size() ; l++)
          t5[k][l] = toupper(t5[k][l]);
      }
      */
    }
  }
  else if ( type == 6 ) {
    x = 6 ;
    while ( !success ) {
      if ( t6[hash] == "") {
        t6[hash] = token ;
        y = hash ;
        success = true ;
      }
      else
        hash++ ;

      if ( hash >= 100)
        hash = 0 ;
    }
  }
  else if ( type == 7 ) {
    x = 7 ;
    while ( !success ) {
      if ( t7[hash] == "") {
        t7[hash] = token ;
        y = hash ;
        success = true ;
      }
      else
        hash++ ;

      if ( hash >= 100)
        hash = 0 ;
    }
  }
}
