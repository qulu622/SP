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
string lasttoken ; // ��W��token�� , �ΨӧP�_�O�_���r��
string last2token; // ��W�W��token��, �ΨӧP�_X''�����D
int tmp[100]; // ��x,y��
typedef char str[15] ; // �Ψө��ɦW talbe1,talbe2,table3,table4
int getToken(string input); // ��token
bool isDelimiter(char temp); // �P�_�O�_��Delimiter
bool isInTable(string token, int & x, int & y); // �P�_�O�_�btable�X�{�L�F
int isWhat(string token); // �P�_�Osymbol , integer, string? return 5,6,7
void hashing(string token, int & x, int & y, int type); // ��token��itable��

int main(){
  str table[4]; // ��4��table���ɦW
  strncpy(table[0], "Table1.table", 15) ;
  strncpy(table[1], "Table2.table", 15) ;
  strncpy(table[2], "Table3.table", 15) ;
  strncpy(table[3], "Table4.table", 15) ;

  string input[100]; // ��input Line

  for ( int i = 0 ; i < 4 ; i++ ) { // �qtable1Ū��table4
    ifstream fin(table[i]); // Ū��
    if (!fin)
      cout << "fin fail" << endl ; // �O�_Ū�ɥ���, �Y���ѴN��X"fail"

    switch( i ) {
      case 0: // Ūtable1
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t1[j] ;
        for ( int k = 1 ; k < 100 ; k++ ) { // �N��table��������ର�j�g
          for ( int l = 0 ; l < t1[k].size() ; l++)
            t1[k][l] = toupper(t1[k][l]);
        }
      case 1: // Ūtable2
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t2[j] ;
      case 2: // Ūtable3
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t3[j] ;
      case 3: // Ūtable4
        for ( int j = 1 ; !fin.eof() ; j++ )
          fin >> t4[j] ;
    }

    fin.close(); // ����
  }

  for ( int i = 0 ; i < 100 ; i++) { // ��t5,t6,t7����Ƴ���l��,�]���Ŧr��
    t5[i] = "";
    t6[i] = "";
    t7[i] = "";
  }

  str inputname ; // input�ɪ��W�r
  str outputname ; // output�ɪ��W�r
  cout << "input filename(+.txt): " ;
  cin >> inputname ;
  cout << "output filename(+.txt): " ;
  cin >> outputname ;


  // strcpy(inputname, "SIC_input.txt") ; // ���ե�
  ifstream fin(inputname); // Ū��
  ofstream fout(outputname); // ��X�ɮ�
  if (!fin)
    cout << "fin fail" << endl ; // �O�_Ū�ɥ���, �Y���ѴN��X"fail"
  else {
    // cout << "fin success" << endl ;
    int n = 0 ; // �Oinput���X��
    for ( int i = 0 ; !fin.eof() ; i++ ) {
      getline(fin,input[i]); // Ū�@��
      n = i ; // �@n��input
    }

    for ( int i = 0 ; i < n ; i++ ) {
      cout << input[i] << endl ; // �L�X�@��input
      fout << input[i] << endl ; // ��X�ɮ�
      int num = getToken(input[i]); // getToken�æL�X���G
      for ( int j = 0 ; j < num ; j = j+2 )
        fout << "(" << tmp[j] << "," << tmp[j+1] << ")" ;
      cout << endl ;
      fout << endl ;
      lasttoken = "" ;
      last2token = "" ;
    }

  }

  fin.close(); // ����
  return 0 ;
}

int getToken(string input){
  int len = input.size(); // �p��input������, ���Fchar array���j�p�ҳ]�p
  char ary[len] ; // �ŧi�@��char array �ө�input
  strcpy(ary,input.c_str()); // �Ninput�����ecopy��ary
  // cout << ary << endl ;
  int x = 0 ; // (x,y)
  int y = 0 ; // (x,y)
  bool get = false ; // �O�_�o��@��token�F?
  string token = "" ; // ��token
  int i = 0 ; // �ΨӶ]while
  int n = 0 ; // tmp index
  while( i < len ) {
    while ( !get ) {
      if ( ary[i] != ' ' && ary[i] != '\t' && ary[i] != '\n' ) {
        if ( isDelimiter(ary[i]) && token == "") { // ��WDelimiter
          if (ary[i] == '.') // �B�z����
            return n ;
          get = true ;
          token = token + ary[i];
          i++;
        }
        else if ( isDelimiter(ary[i]) && token != "" ) // �J��Delimiter, �e�������@��token
          get = true ;
        else if ( !isDelimiter(ary[i])) { // �DDelimiter, ��Jtoken��
          token = token + ary[i] ;
          i++;
        }
      }
      else if (  (ary[i] == ' ' || ary[i] == '\t' || ary[i] == '\n') && token != "") { // �J��ť�, �����@��token
        get = true ;
        i++;
      }
      else if ( (ary[i] == ' ' || ary[i] == '\t' || ary[i] == '\n') && token == "" ) // �J��ť�, �~���U�@��
        i++;
      // cout << i-1 << ": " << token << endl ;
      if ( i == len )
        get = true ;
    }

    if ( get && ( token == "C" || token == "c" ) && ary[i] == '\'') { // �B�zC'�����D
      token = "" ;
      get = false ;
    }


    if ( get ) {
      if ( isInTable(token,x,y) ) { // �ݬO�_�btable��
        cout << "(" << x << "," << y << ")" ;
        tmp[n] = x ;
        tmp[n+1] = y ;
        n = n + 2 ;
      }

      else { // ���btable��,�h�Q��hashing
        int type = isWhat(token) ; // ���ݥ��ݩ����table
        hashing(token,x,y,type); // hashing
        cout << "(" << x << "," << y << ")" ;
        tmp[n] = x ;
        tmp[n+1] = y ;
        n = n + 2 ;
      }
      x = 0 ;
      y = 0 ;
      last2token = lasttoken ; // �����W�W��token, �B�zX��
      lasttoken = token ; // �����W�@��token, �B�z�r���
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
    if ( token == t5[i] && lasttoken != "\'") { // �p�G�W��token�O'����, ���ݩ�table7
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
  int len = token.size(); // �p��token������, ���Fchar array���j�p�ҳ]�p
  char ary[len] ; // �ŧi�@��char array �ө�token
  strcpy(ary,token.c_str()); // �Ntoken�����ecopy��ary
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
  int len = token.size(); // �p��token������, ���Fchar array���j�p�ҳ]�p
  char ary[len] ; // �ŧi�@��char array �ө�token
  strcpy(ary,token.c_str()); // �Ntoken�����ecopy��ary
  int hash = 0 ;
  bool success = false ;

  if ( type == 5 ) {
    for ( int i = 0 ; i < len ; i++ ) // �N��token�ର�j�g
      ary[i] = toupper(ary[i]) ;
  }

  token.assign(ary);

  for ( int i = 0 ; i < len ; i++)
    hash = hash + (int)ary[i] ; // ��r����ascii�ۥ[

  hash = hash % 100 ; // ��100���l��

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
      for ( int k = 0 ; k < 100 ; k++ ) { // �N��table��������ର�j�g
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
