#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

map<string,string> mapping;
mt19937 rng;

// apni 4 markers ki row hai. And us row ke ends pr opponent ki ring hai toh us row mein bahot potential hai ki wo opponent ki remove row ban jaye
// varibale depth

// convertors
vector<int> fconvertor(int hex, int pos, int size){
	int len = 2*size;
	vector <int> vec;
	int part_hex = 0;
	int arr[2];
	arr[0] = 0;
	arr[1] = 0;

	if(hex==0){
        arr[0] = len;
        arr[1] = len;
    }else{
    	part_hex = pos/hex;
	    
	    if(part_hex==0){
	        arr[1] = len + (2*pos);
	        arr[0] = len - (2*hex) + (pos%hex);
	    }else if(part_hex==1){
	        arr[1] = len + (2*hex);
	        arr[0] = len - (2*hex) + hex + 2*(pos%hex);
	    }else if(part_hex==2){
	        arr[1] = len + (2*hex) - 2*(pos%hex);
	        arr[0] = len - (2*hex) + 3*hex + (pos%hex);
	    }else if(part_hex==3){
	        arr[1] = len - 2*(pos%hex);
	        arr[0] = len + (2*hex) - (pos%hex);
	    }else if(part_hex==4){
	        arr[1] = len - (2*hex);
	        arr[0] = len + (2*hex) - hex - 2*(pos%hex);
	    }else{
	        arr[1] = len - (2*hex) + 2*(pos%hex);
	        arr[0] = len - hex - (pos%hex);
	    }
    }

    vec.push_back(arr[0]);
    vec.push_back(arr[1]);
	return vec;
}

vector<int> rev_convertor(int row, int col){
	vector<int> vec;
	string src = to_string(row) + " " + to_string(col);
	string dest = mapping.at(src);
	int len = dest.length();
	int hex,pos;

	if(len==3){
		hex = dest.at(0)-'0';
		pos = dest.at(2)-'0';
	}else{
		hex = dest.at(0)-'0';
		pos = 10*(dest.at(2)-'0') + (dest.at(3)-'0');
	}
	vec.push_back(hex);
	vec.push_back(pos);
	return vec;	
}

int stringtoint(string s){
	int val = 0;
	int len = s.length();
	int i = 0;
	for(i = 0;i<len;i++){
		val = 10*val + (s.at(i)-'0');
	}
	return val;
}

// alignment and intersection functions
bool aligned(int r1,int c1,int r2,int c2){
	int hdiff = abs(r2-r1);
	int vdiff = abs(c2 - c1);

	if(r1==r2){
		return false;
	}else if(c1==c2){
		return true;
	}else{
		vdiff/=2;
		if(hdiff==vdiff){
			return true;
		}else{
			return false;
		}
	}
}

bool intersect(int r, int c, int r1, int c1, int r2, int c2){
	if(aligned(r,c,r1,c1) && aligned(r,c,r2,c2)){
		return true;
	}else{
		return false;
	}
}

// copy board functions from arr2 to arr1
void board_copy5(int arr1[21][21],int arr2[21][21]){
	int i = 0;
	int j = 0;
	for(i = 0;i<21;i++){
		for(j = 0;j<21;j++){
			arr1[i][j] = arr2[i][j];
		}
	}
}

void board_copy6(int arr1[25][25],int arr2[25][25]){
	int i = 0;
	int j = 0;
	for(i = 0;i<25;i++){
		for(j = 0;j<25;j++){
			arr1[i][j] = arr2[i][j];
		}
	}
}

// should i proceed further or not
bool shouldproceed5(int board[21][21],int turn){
	int ringval = 2 - turn;
	int i = 0;
	int j = 0;
	int count = 0;
	for(i = 0;i<21;i++){
		for(j = 0;j<21;j++){
			if(board[i][j]==ringval){
				count++;
			}
		}
	}
	if(count>2){
		return true;
	}else{
		return false;
	}
}

bool shouldproceed6(int board[25][25],int turn){
	int ringval = 2 - turn;
	int i = 0;
	int j = 0;
	int count = 0;
	for(i = 0;i<25;i++){
		for(j = 0;j<25;j++){
			if(board[i][j]==ringval){
				count++;
			}
		}
	}
	if(count>3){
		return true;
	}else{
		return false;
	}
}

// copy ring coordinates function from 2nd set to the first set
void ring_copy5(int x1[5],int y1[5],int x2[5],int y2[5]){
	int i = 0;
	for(i = 0;i<5;i++){
		x1[i] = x2[i];
		y1[i] = y2[i];
	}
}

void ring_copy6(int x1[6],int y1[6],int x2[6],int y2[6]){
	int i = 0;
	for(i = 0;i<6;i++){
		x1[i] = x2[i];
		y1[i] = y2[i];
	}
}

// initializations
void initialize5(int board[21][21],int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	int index = 0;
	int i1 = 0;
	int i2 = 0;
	vector<int> vec;
	int hex = 0;
	int pos = 0;
	string s1 = "";
	string s2 = "";
	
	for(index = 0;index<5;index++){
		mringrow[index] = -1;
		mringcol[index] = -1;
		oringrow[index] = -1;
		oringcol[index] = -1;
	}

	for(i1 = 0;i1<21;i1++){
		for(i2 = 0;i2<21;i2++){
			board[i1][i2] = -1;
		}
	}

	for(hex = 0;hex<=5;hex++){
		if(hex==0){
			s1 = to_string(hex) + " " + to_string(pos);
			vec = fconvertor(0,0,5);
			board[vec[0]][vec[1]] = 0;
			s2 = to_string(vec[0]) + " " + to_string(vec[1]);
			mapping.insert(pair <string,string> (s2,s1));
		}else if(hex==5){
			for(pos = 0;pos<6*hex;pos++){
				if(pos%hex!=0){
					s1 = to_string(hex) + " " + to_string(pos);
					vec = fconvertor(hex,pos,5);
					board[vec[0]][vec[1]] = 0;
					s2 = to_string(vec[0]) + " " + to_string(vec[1]);
					mapping.insert(pair <string,string> (s2,s1));
				}
			}
		}else{
			for(pos = 0;pos<6*hex;pos++){
				s1 = to_string(hex) + " " + to_string(pos);
				vec = fconvertor(hex,pos,5);
				board[vec[0]][vec[1]] = 0;
				s2 = to_string(vec[0]) + " " + to_string(vec[1]);
				mapping.insert(pair <string,string> (s2,s1));
			}
		}
	}
}

void initialize6(int board[25][25],int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6]){
	int index = 0;
	int i1 = 0;
	int i2 = 0;
	vector<int> vec;
	int hex = 0;
	int pos = 0;
	string s1 = "";
	string s2 = "";

	for(index = 0;index<6;index++){
		mringrow[index] = -1;
		mringcol[index] = -1;
		oringrow[index] = -1;
		oringcol[index] = -1;
	}
	
	for(i1 = 0;i1<25;i1++){
		for(i2 = 0;i2<25;i2++){
			board[i1][i2] = -1;
		}
	}

	for(hex = 0;hex<=6;hex++){
		if(hex==0){
			s1 = to_string(hex) + " " + to_string(pos);
			vec = fconvertor(0,0,6);
			board[vec[0]][vec[1]] = 0;
			s2 = to_string(vec[0]) + " " + to_string(vec[1]);
			mapping.insert(pair <string,string> (s2,s1));
		}else if(hex==6){
			for(pos = 0;pos<6*hex;pos++){
				if(pos%hex!=0){
					s1 = to_string(hex) + " " + to_string(pos);
					vec = fconvertor(hex,pos,6);
					board[vec[0]][vec[1]] = 0;
					s2 = to_string(vec[0]) + " " + to_string(vec[1]);
					mapping.insert(pair <string,string> (s2,s1));
				}
			}
		}else{
			for(pos = 0;pos<6*hex;pos++){
				s1 = to_string(hex) + " " + to_string(pos);
				vec = fconvertor(hex,pos,6);
				board[vec[0]][vec[1]] = 0;
				s2 = to_string(vec[0]) + " " + to_string(vec[1]);
				mapping.insert(pair <string,string> (s2,s1));
			}
		}
	}
}

// ring placement functions
void place_ring5(int board[21][21],int turn,int ringrow[5],int ringcol[5],int row,int col){
	int ringvalue = 2-turn;
	int i = 0;

	if(board[row][col]==0){
		for(i = 0;i<5;i++){
			if(ringrow[i]==-1 && ringcol[i]==-1){
				ringrow[i] = row;
				ringcol[i] = col;
				board[row][col] = ringvalue;
				break;
			}
		}
	}else{
		// do nothing
	}
}

void place_ring6(int board[25][25],int turn,int ringrow[6],int ringcol[6],int row,int col){
	int ringvalue = 2-turn;
	int i = 0;

	if(board[row][col]==0){
		for(i = 0;i<6;i++){
			if(ringrow[i]==-1 && ringcol[i]==-1){
				ringrow[i] = row;
				ringcol[i] = col;
				board[row][col] = ringvalue;
				break;
			}
		}
	}else{
		// do nothing
	}
}

// move ring functions
void move_ring5(int board[21][21],int turn,int ringrow[5],int ringcol[5],int irow,int icol,int frow,int fcol){
	int ringvalue = 2 - turn;
	int marker = 4 - turn;
	int i = 0;

	for(i = 0;i<5;i++){
		if(ringrow[i]==irow && ringcol[i]==icol){
			ringrow[i] = frow;
			ringcol[i] = fcol;
			break;
		}
	}

	if(board[irow][icol]==ringvalue){
		int hdiff = frow - irow;
		int vdiff = fcol - icol;
		int rstart = irow;
		int cstart = icol;
		board[irow][icol] = 7 - marker;

		if(vdiff==0){
			if(hdiff>0){
				while(rstart<=frow){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart+=2;
				}
			}else{
				while(rstart>=frow){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart-=2;
				}
			}
		}else if(vdiff>0){
			if(hdiff>0){
				while(rstart<=frow && cstart<=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart+=1;
					cstart+=2;
				}
			}else{
				while(rstart>=frow && cstart<=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart-=1;
					cstart+=2;
				}
			}
		}else{
			if(hdiff>0){
				while(rstart<=frow && cstart>=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart+=1;
					cstart-=2;
				}
			}else{
				while(rstart>=frow && cstart>=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart-=1;
					cstart-=2;
				}
			}
		}

		board[frow][fcol] = ringvalue;
	}else{
		// do nothing
	}
}

void move_ring6(int board[25][25],int turn,int ringrow[6],int ringcol[6],int irow,int icol,int frow,int fcol){
	int ringvalue = 2 - turn;
	int marker = 4 - turn;
	int i = 0;

	for(i = 0;i<6;i++){
		if(ringrow[i]==irow && ringcol[i]==icol){
			ringrow[i] = frow;
			ringcol[i] = fcol;
			break;
		}
	}

	if(board[irow][icol]==ringvalue){
		int hdiff = frow - irow;
		int vdiff = fcol - icol;
		int rstart = irow;
		int cstart = icol;
		board[irow][icol] = 7 - marker;

		if(vdiff==0){
			if(hdiff>0){
				while(rstart<=frow){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart+=2;
				}
			}else{
				while(rstart>=frow){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart-=2;
				}
			}
		}else if(vdiff>0){
			if(hdiff>0){
				while(rstart<=frow && cstart<=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart+=1;
					cstart+=2;
				}
			}else{
				while(rstart>=frow && cstart<=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart-=1;
					cstart+=2;
				}
			}
		}else{
			if(hdiff>0){
				while(rstart<=frow && cstart>=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart+=1;
					cstart-=2;
				}
			}else{
				while(rstart>=frow && cstart>=fcol){
					if(board[rstart][cstart]==3 || board[rstart][cstart]==4){
						board[rstart][cstart] = 7 - board[rstart][cstart];
					}
					rstart-=1;
					cstart-=2;
				}
			}
		}

		board[frow][fcol] = ringvalue;
	}else{
		// do nothing
	}
}

// ring removal functions
void remove_ring5(int board[21][21],int turn,int ringrow[5],int ringcol[5],int row,int col){
	int ringvalue = 2-turn;
	int i = 0;

	if(board[row][col]==ringvalue){
		for(i = 0;i<5;i++){
			if(ringrow[i]==row && ringcol[i]==col){
				ringrow[i] = -1;
				ringcol[i] = -1;
				board[row][col] = 0;
				break;
			}
		}
	}else{
		// do nothing
	}
}

void remove_ring6(int board[25][25],int turn,int ringrow[6],int ringcol[6],int row,int col){
	int ringvalue = 2-turn;
	int i = 0;

	if(board[row][col]==ringvalue){
		for(i = 0;i<6;i++){
			if(ringrow[i]==row && ringcol[i]==col){
				ringrow[i] = -1;
				ringcol[i] = -1;
				board[row][col] = 0;
				break;
			}
		}
	}else{
		// do nothing
	}
}

// row removal functions
void remove_row5(int board[21][21],int rstart,int cstart,int rend,int cend){
	int hdiff = rend - rstart;
	int vdiff = cend - cstart;
	int row = rstart;
	int col = cstart;

	if(vdiff==0){
		if(hdiff>0){
			while(row<=rend){
				board[row][col] = 0;
				row+=2;
			}
		}else{
			while(row>=rend){
				board[row][col] = 0;
				row-=2;
			}
		}
	}else if(vdiff>0){
		if(hdiff>0){
			while(row<=rend && col<=cend){
				board[row][col] = 0;
				row+=1;
				col+=2;
			}
		}else{
			while(row>=rend && col<=cend){
				board[row][col] = 0;
				row-=1;
				col+=2;
			}
		}
	}else{
		if(hdiff>0){
			while(row<=rend && col>=cend){
				board[row][col] = 0;
				row+=1;
				col-=2;
			}
		}else{
			while(row>=rend && col>=cend){
				board[row][col] = 0;
				row-=1;
				col-=2;
			}
		}
	}
}

void remove_row6(int board[25][25],int rstart,int cstart,int rend,int cend){
	int hdiff = rend - rstart;
	int vdiff = cend - cstart;
	int row = rstart;
	int col = cstart;

	if(vdiff==0){
		if(hdiff>0){
			while(row<=rend){
				board[row][col] = 0;
				row+=2;
			}
		}else{
			while(row>=rend){
				board[row][col] = 0;
				row-=2;
			}
		}
	}else if(vdiff>0){
		if(hdiff>0){
			while(row<=rend && col<=cend){
				board[row][col] = 0;
				row+=1;
				col+=2;
			}
		}else{
			while(row>=rend && col<=cend){
				board[row][col] = 0;
				row-=1;
				col+=2;
			}
		}
	}else{
		if(hdiff>0){
			while(row<=rend && col>=cend){
				board[row][col] = 0;
				row+=1;
				col-=2;
			}
		}else{
			while(row>=rend && col>=cend){
				board[row][col] = 0;
				row-=1;
				col-=2;
			}
		}
	}
}

// move from string helpers
void movomstring15(string s,int turn,int board[21][21],int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	int len = s.length();
	int hex1 = 0;
	int pos1 = 0;
	int hex2 = 0;
	int pos2 = 0;
	vector<int> vec;
	vector<int> vec2;

	if(len==5){
		hex1 = s.at(2) - '0';
		pos1 = s.at(4) - '0';
		vec = fconvertor(hex1,pos1,5);
		if(s.at(0)=='P'){
			if(turn==1){
				place_ring5(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				place_ring5(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}else{
			if(turn==1){
				remove_ring5(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				remove_ring5(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}
	}else if(len==6){
		hex1 = s.at(2) - '0';
		pos1 = 10*(s.at(4) - '0') + (s.at(5)-'0');
		vec = fconvertor(hex1,pos1,5);
		if(s.at(0)=='P'){
			if(turn==1){
				place_ring5(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				place_ring5(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}else{
			if(turn==1){
				remove_ring5(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				remove_ring5(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}
	}else if(len==11){
		hex1 = s.at(2) - '0';
		pos1 = s.at(4) - '0';
		hex2 = s.at(8) - '0';
		pos2 = s.at(10) - '0';
		vec = fconvertor(hex1,pos1,5);
		vec2 = fconvertor(hex2,pos2,5);
		if(turn==1){
			move_ring5(board,turn,mringrow,mringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}else{
			move_ring5(board,turn,oringrow,oringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}
	}else if(len==12){
		if(s.at(5)==' '){
			hex1 = s.at(2) - '0';
			pos1 = s.at(4) - '0';
			hex2 = s.at(8) - '0';
			pos2 = 10*(s.at(10) - '0') + (s.at(11) - '0');
		}else{
			hex1 = s.at(2) - '0';
			pos1 = 10*(s.at(4) - '0') + (s.at(5) - '0');
			hex2 = s.at(9) - '0';
			pos2 = s.at(11) - '0';
		}
		vec = fconvertor(hex1,pos1,5);
		vec2 = fconvertor(hex2,pos2,5);
		if(turn==1){
			move_ring5(board,turn,mringrow,mringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}else{
			move_ring5(board,turn,oringrow,oringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}
	}else if(len==13){
		if(s.at(0)=='S'){
			hex1 = s.at(2) - '0';
			pos1 = 10*(s.at(4)-'0') + (s.at(5) - '0');
			hex2 = s.at(9) - '0';
			pos2 = 10*(s.at(11) - '0') + (s.at(12) - '0');
			vec = fconvertor(hex1,pos1,5);
			vec2 = fconvertor(hex2,pos2,5);
			if(turn==1){
				move_ring5(board,turn,mringrow,mringcol,vec[0],vec[1],vec2[0],vec2[1]);
			}else{
				move_ring5(board,turn,oringrow,oringcol,vec[0],vec[1],vec2[0],vec2[1]);
			}
		}else{
			hex1 = s.at(3) - '0';
			pos1 = s.at(5) - '0';
			hex2 = s.at(10) - '0';
			pos2 = s.at(12) - '0';
			vec = fconvertor(hex1,pos1,5);
			vec2 = fconvertor(hex2,pos2,5);
			remove_row5(board,vec[0],vec[1],vec2[0],vec2[1]);
		}
	}else if(len==14){
		if(s.at(6)==' '){
			hex1 = s.at(3) - '0';
			pos1 = s.at(5) - '0';
			hex2 = s.at(10) - '0';
			pos2 = 10*(s.at(12) - '0') + (s.at(13) - '0');
			vec = fconvertor(hex1,pos1,5);
			vec2 = fconvertor(hex2,pos2,5);
		}else{
			hex1 = s.at(3) - '0';
			pos1 = 10*(s.at(5)-'0') + (s.at(6) - '0');
			hex2 = s.at(11) - '0';
			pos2 = s.at(13) - '0';
			vec = fconvertor(hex1,pos1,5);
			vec2 = fconvertor(hex2,pos2,5);
		}
		remove_row5(board,vec[0],vec[1],vec2[0],vec2[1]);
	}else if(len==15){
		hex1 = s.at(3) - '0';
		pos1 = 10*(s.at(5)-'0') + (s.at(6) - '0');
		hex2 = s.at(11) - '0';
		pos2 = 10*(s.at(13)-'0') + (s.at(14) - '0');
		vec = fconvertor(hex1,pos1,5);
		vec2 = fconvertor(hex2,pos2,5);
		remove_row5(board,vec[0],vec[1],vec2[0],vec2[1]);
	}else{
		// do nothing
	}
}

void movomstring16(string s,int turn,int board[25][25],int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6]){
	int len = s.length();
	int hex1 = 0;
	int pos1 = 0;
	int hex2 = 0;
	int pos2 = 0;
	vector<int> vec;
	vector<int> vec2;

	if(len==5){
		hex1 = s.at(2) - '0';
		pos1 = s.at(4) - '0';
		vec = fconvertor(hex1,pos1,6);
		if(s.at(0)=='P'){
			if(turn==1){
				place_ring6(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				place_ring6(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}else{
			if(turn==1){
				remove_ring6(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				remove_ring6(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}
	}else if(len==6){
		hex1 = s.at(2) - '0';
		pos1 = 10*(s.at(4) - '0') + (s.at(5)-'0');
		vec = fconvertor(hex1,pos1,6);
		if(s.at(0)=='P'){
			if(turn==1){
				place_ring6(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				place_ring6(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}else{
			if(turn==1){
				remove_ring6(board,turn,mringrow,mringcol,vec[0],vec[1]);
			}else{
				remove_ring6(board,turn,oringrow,oringcol,vec[0],vec[1]);
			}
		}
	}else if(len==11){
		hex1 = s.at(2) - '0';
		pos1 = s.at(4) - '0';
		hex2 = s.at(8) - '0';
		pos2 = s.at(10) - '0';
		vec = fconvertor(hex1,pos1,6);
		vec2 = fconvertor(hex2,pos2,6);
		if(turn==1){
			move_ring6(board,turn,mringrow,mringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}else{
			move_ring6(board,turn,oringrow,oringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}
	}else if(len==12){
		if(s.at(5)==' '){
			hex1 = s.at(2) - '0';
			pos1 = s.at(4) - '0';
			hex2 = s.at(8) - '0';
			pos2 = 10*(s.at(10) - '0') + (s.at(11) - '0');
		}else{
			hex1 = s.at(2) - '0';
			pos1 = 10*(s.at(4) - '0') + (s.at(5) - '0');
			hex2 = s.at(9) - '0';
			pos2 = s.at(11) - '0';
		}
		vec = fconvertor(hex1,pos1,6);
		vec2 = fconvertor(hex2,pos2,6);
		if(turn==1){
			move_ring6(board,turn,mringrow,mringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}else{
			move_ring6(board,turn,oringrow,oringcol,vec[0],vec[1],vec2[0],vec2[1]);
		}
	}else if(len==13){
		if(s.at(0)=='S'){
			hex1 = s.at(2) - '0';
			pos1 = 10*(s.at(4)-'0') + (s.at(5) - '0');
			hex2 = s.at(9) - '0';
			pos2 = 10*(s.at(11) - '0') + (s.at(12) - '0');
			vec = fconvertor(hex1,pos1,6);
			vec2 = fconvertor(hex2,pos2,6);
			if(turn==1){
				move_ring6(board,turn,mringrow,mringcol,vec[0],vec[1],vec2[0],vec2[1]);
			}else{
				move_ring6(board,turn,oringrow,oringcol,vec[0],vec[1],vec2[0],vec2[1]);
			}
		}else{
			hex1 = s.at(3) - '0';
			pos1 = s.at(5) - '0';
			hex2 = s.at(10) - '0';
			pos2 = s.at(12) - '0';
			vec = fconvertor(hex1,pos1,6);
			vec2 = fconvertor(hex2,pos2,6);
			remove_row6(board,vec[0],vec[1],vec2[0],vec2[1]);
		}
	}else if(len==14){
		if(s.at(6)==' '){
			hex1 = s.at(3) - '0';
			pos1 = s.at(5) - '0';
			hex2 = s.at(10) - '0';
			pos2 = 10*(s.at(12) - '0') + (s.at(13) - '0');
			vec = fconvertor(hex1,pos1,6);
			vec2 = fconvertor(hex2,pos2,6);
		}else{
			hex1 = s.at(3) - '0';
			pos1 = 10*(s.at(5)-'0') + (s.at(6) - '0');
			hex2 = s.at(11) - '0';
			pos2 = s.at(13) - '0';
			vec = fconvertor(hex1,pos1,6);
			vec2 = fconvertor(hex2,pos2,6);
		}
		remove_row6(board,vec[0],vec[1],vec2[0],vec2[1]);
	}else if(len==15){
		hex1 = s.at(3) - '0';
		pos1 = 10*(s.at(5)-'0') + (s.at(6) - '0');
		hex2 = s.at(11) - '0';
		pos2 = 10*(s.at(13)-'0') + (s.at(14) - '0');
		vec = fconvertor(hex1,pos1,6);
		vec2 = fconvertor(hex2,pos2,6);
		remove_row6(board,vec[0],vec[1],vec2[0],vec2[1]);
	}else{
		// do nothing
	}
}

// move from string functions
void movromstring5(string s,int turn,int board[21][21],int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	if(s.length()<=4 || s.at(0)==' '){
		// do nothing
	}else{
		if(s.at(0)=='S'){
			if(s.at(5)==' '){
				if(s.length()<=13){
					movomstring15(s,turn,board,mringrow,mringcol,oringrow,oringcol);
				}else{
					if(s.at(11)==' '){
						movomstring15(s.substr(0,11),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(12),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring15(s.substr(0,12),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(13),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}
			}else{
				if(s.length()<=13){
					movomstring15(s,turn,board,mringrow,mringcol,oringrow,oringcol);
				}else{
					if(s.at(12)==' '){
						movomstring15(s.substr(0,12),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(13),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring15(s.substr(0,13),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(14),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}
			}
		}else if(s.at(0)=='R'){
			if(s.length()<=15){
				movomstring15(s,turn,board,mringrow,mringcol,oringrow,oringcol);
			}else{
				if(s.at(6)==' '){
					if(s.at(13)==' '){
						movomstring15(s.substr(0,13),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(14),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring15(s.substr(0,14),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(15),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}else{
					if(s.at(14)==' '){
						movomstring15(s.substr(0,14),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(15),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring15(s.substr(0,15),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring5(s.substr(16),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}
			}
		}else{
			if(s.length()==5 || s.length()==6){
				movomstring15(s,turn,board,mringrow,mringcol,oringrow,oringcol);
			}else{
				if(s.at(5)==' '){
					movomstring15(s.substr(0,5),turn,board,mringrow,mringcol,oringrow,oringcol);
					movromstring5(s.substr(6),turn,board,mringrow,mringcol,oringrow,oringcol);
				}else{
					movomstring15(s.substr(0,6),turn,board,mringrow,mringcol,oringrow,oringcol);
					movromstring5(s.substr(7),turn,board,mringrow,mringcol,oringrow,oringcol);
				}
			}
		}
	}
}

void movromstring6(string s,int turn,int board[25][25],int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6]){
	if(s.length()<=4){
		// do nothing
	}else{
		if(s.at(0)=='S'){
			if(s.at(5)==' '){
				if(s.length()<=13){
					movomstring16(s,turn,board,mringrow,mringcol,oringrow,oringcol);
				}else{
					if(s.at(11)==' '){
						movomstring16(s.substr(0,11),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(12),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring16(s.substr(0,12),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(13),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}
			}else{
				if(s.length()<=13){
					movomstring16(s,turn,board,mringrow,mringcol,oringrow,oringcol);
				}else{
					if(s.at(12)==' '){
						movomstring16(s.substr(0,12),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(13),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring16(s.substr(0,13),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(14),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}
			}
		}else if(s.at(0)=='R'){
			if(s.length()<=15){
				movomstring16(s,turn,board,mringrow,mringcol,oringrow,oringcol);
			}else{
				if(s.at(6)==' '){
					if(s.at(13)==' '){
						movomstring16(s.substr(0,13),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(14),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring16(s.substr(0,14),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(15),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}else{
					if(s.at(14)==' '){
						movomstring16(s.substr(0,14),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(15),turn,board,mringrow,mringcol,oringrow,oringcol);
					}else{
						movomstring16(s.substr(0,15),turn,board,mringrow,mringcol,oringrow,oringcol);
						movromstring6(s.substr(16),turn,board,mringrow,mringcol,oringrow,oringcol);
					}
				}
			}
		}else{
			if(s.length()==5 || s.length()==6){
				movomstring16(s,turn,board,mringrow,mringcol,oringrow,oringcol);
			}else{
				if(s.at(5)==' '){
					movomstring16(s.substr(0,5),turn,board,mringrow,mringcol,oringrow,oringcol);
					movromstring6(s.substr(6),turn,board,mringrow,mringcol,oringrow,oringcol);
				}else{
					movomstring16(s.substr(0,6),turn,board,mringrow,mringcol,oringrow,oringcol);
					movromstring6(s.substr(7),turn,board,mringrow,mringcol,oringrow,oringcol);
				}
			}
		}
	}
}

// ring placement heuristics and company
vector<int> placement5heuristic(int board[21][21],int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	vector<int> vec;
	int mrings = 0;
	int orings = 0;
	int index = 0;
	rng.seed(random_device()());
    // uniform_int_distribution<mt19937::result_type> dist(6,14);
    uniform_int_distribution<mt19937::result_type> dist(1,1000);

	for(index = 0;index<5;index++){
		if(mringrow[index]!=-1 && mringcol[index]!=-1){
			mrings++;
		}
		if(oringrow[index]!=-1 && oringcol[index]!=-1){
			orings++;
		}
	}
	vec.push_back(10);
	vec.push_back(10);

	while(board[vec[0]][vec[1]]!=0){
		vec.clear();
		vec.push_back(dist(rng)%21);
		vec.push_back(dist(rng)%21);
		// vec.push_back(dist(rng));
		// vec.push_back(dist(rng));
	}
	return vec;
}

vector<int> placement6heuristic(int board[25][25],int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6]){
	vector<int> vec;
	int mrings = 0;
	int orings = 0;
	int index = 0;
	rng.seed(random_device()());
    uniform_int_distribution<mt19937::result_type> dist(8,16);

	for(index = 0;index<6;index++){
		if(mringrow[index]!=-1 && mringcol[index]!=-1){
			mrings++;
		}
		if(oringrow[index]!=-1 && oringcol[index]!=-1){
			orings++;
		}
	}
	vec.push_back(12);
	vec.push_back(12);

	while(board[vec[0]][vec[1]]!=0){
		vec.clear();
		vec.push_back(dist(rng));
		vec.push_back(dist(rng));
	}

	return vec;
}

string placement5(int board[21][21],int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	string str = "";
	vector<int> vec;
	vector<int> vec2;

	if(board[10][10]==0){
		str = "P 0 0";
	}else{
		vec = placement5heuristic(board,mringrow,mringcol,oringrow,oringcol);
		vec2 = rev_convertor(vec[0],vec[1]);
		str = "P " + to_string(vec2[0]) + " " + to_string(vec2[1]);
	}
	return str;
}

string placement6(int board[25][25],int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6]){
	string str = "";
	vector<int> vec;
	vector<int> vec2;

	if(board[12][12]==0){
		str = "P 0 0";
	}else{
		vec = placement6heuristic(board,mringrow,mringcol,oringrow,oringcol);
		vec2 = rev_convertor(vec[0],vec[1]);
		str = "P " + to_string(vec2[0]) + " " + to_string(vec2[1]);
	}
	return str;
}

// ring importance function and heuristics
int ringimpt5(int board[21][21],int row,int col){
	int value = 0;
	int numMR = 0;
	int numOR = 0;
	int numMM = 0;
	int numOM = 0;
	int myr = board[row][col];
	int mm = myr+2;
	int opr = 3-myr;
	int om = 5-myr;
	int x = 0;
	int y = 0;

	// vertically upwards
	x = row;
	y = col;
	x-=2;
	while(x>=0){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x-=2;
	}

	// vertically downwards
	x = row;
	y = col;
	x+=2;
	while(x<=20){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x+=2;
	}

	// +45 degrees
	x = row;
	y = col;
	x-=1;
	y+=2;
	while(x>=0 && y<=20){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x-=1;
		y+=2;
	}

	// +135 degrees
	x = row;
	y = col;
	x-=1;
	y-=2;
	while(x>=0 && y>=0){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x-=1;
		y-=2;
	}

	// +225 degrees
	x = row;
	y = col;
	x+=1;
	y-=2;
	while(x<=20 && y>=0){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x+=1;
		y-=2;
	}

	// +315 degrees
	x = row;
	y = col;
	x+=1;
	y+=2;
	while(x<=20 && y<=20){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x+=1;
		y+=2;
	}
	value = 5*numMM - 12*numMR + 3*numOM - 16*numOR;
	return value;
}

int ringimpt6(int board[25][25],int row,int col){
	int value = 0;
	int numMR = 0;
	int numOR = 0;
	int numMM = 0;
	int numOM = 0;
	int myr = board[row][col];
	int mm = myr+2;
	int opr = 3-myr;
	int om = 5-myr;
	int x = 0;
	int y = 0;

	// vertically upwards
	x = row;
	y = col;
	x-=2;
	while(x>=0){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x-=2;
	}

	// vertically downwards
	x = row;
	y = col;
	x+=2;
	while(x<=24){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x+=2;
	}

	// +45 degrees
	x = row;
	y = col;
	x-=1;
	y+=2;
	while(x>=0 && y<=24){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x-=1;
		y+=2;
	}

	// +135 degrees
	x = row;
	y = col;
	x-=1;
	y-=2;
	while(x>=0 && y>=0){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x-=1;
		y-=2;
	}

	// +225 degrees
	x = row;
	y = col;
	x+=1;
	y-=2;
	while(x<=24 && y>=0){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x+=1;
		y-=2;
	}

	// +315 degrees
	x = row;
	y = col;
	x+=1;
	y+=2;
	while(x<=24 && y<=24){
		if(board[x][y]==myr){
			numMR++;
		}else if(board[x][y]==mm){
			numMM++;
		}else if(board[x][y]==opr){
			numOR++;
		}else if(board[x][y]==om){
			numOM++;
		}else if(board[x][y]==-1){
			break;
		}else{
			// do nothing
		}
		x+=1;
		y+=2;
	}
	value = 5*numMM - 12*numMR + 3*numOM - 16*numOR;
	return value;
}

string remove_ringmove5(int turn,int board[21][21],int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	string str = "";
	int min = INT_MAX;
	int i = 0;
	int val = 0;
	int index = 0;
	vector<int> vec;

	if(turn==1){
		for(i = 0;i<5;i++){
			if(mringrow[i]!=-1 && mringcol[i]!=-1){
				val = ringimpt5(board,mringrow[i],mringcol[i]);
				if(val<min){
					min = val;
					index = i;
				}
			}	
		}
		vec = rev_convertor(mringrow[index],mringcol[index]);
		str = "X " + to_string(vec[0]) + " " + to_string(vec[1]);
	}else{
		for(i = 0;i<5;i++){
			if(oringrow[i]!=-1 && oringcol[i]!=-1){
				val = ringimpt5(board,oringrow[i],oringcol[i]);
				if(val<min){
					min = val;
					index = i;
				}
			}	
		}
		vec = rev_convertor(oringrow[index],oringcol[index]);
		str = "X " + to_string(vec[0]) + " " + to_string(vec[1]);
	}
	return str;
}

string remove_ringmove6(int turn,int board[25][25],int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6]){
	string str = "";
	int min = INT_MAX;
	int i = 0;
	int val = 0;
	int index = 0;
	vector<int> vec;

	if(turn==1){
		for(i = 0;i<6;i++){
			if(mringrow[i]!=-1 && mringcol[i]!=-1){
				val = ringimpt6(board,mringrow[i],mringcol[i]);
				if(val<min){
					min = val;
					index = i;
				}
			}	
		}
		vec = rev_convertor(mringrow[index],mringcol[index]);
		str = "X " + to_string(vec[0]) + " " + to_string(vec[1]);
	}else{
		for(i = 0;i<6;i++){
			if(oringrow[i]!=-1 && oringcol[i]!=-1){
				val = ringimpt6(board,oringrow[i],oringcol[i]);
				if(val<min){
					min = val;
					index = i;
				}
			}	
		}
		vec = rev_convertor(oringrow[index],oringcol[index]);
		str = "X " + to_string(vec[0]) + " " + to_string(vec[1]);
	}
	return str;
}

// row importance function
// direction sense => 0 means || 1 means // and -1 means the 135 degrees waala
int rowpotential5(int board[21][21],int rstart,int cstart,int length,int dirn){
	int marker = board[rstart][cstart];
	int turn = 4 - marker;
	int mring = 2 - turn;
	int omarker = 7 - marker;
	int val = 1;
	if(length==5){
		val = 1000;
	}else if(length==2){
		val = 1;
	}else if(length==4){
		// check ki kisi node par opposite favourable ring ya opposite marker hai ya nahi. Agar hai toh add more weight on it else keep it less
		if(dirn==0){
			if(rstart<=12 && (board[rstart+8][cstart]==omarker || board[rstart+8][cstart]==mring)){
				val+=40;
			}
			if(rstart>=2){
				if(board[rstart-2][cstart]==omarker || board[rstart-2][cstart]==mring){
					val+=40;
				}
			}
			if(val==0){
				val = 10;
			}
		}else if(dirn==1){
			if(rstart>=1 && cstart<=18 && (board[rstart-1][cstart+2]==omarker || board[rstart-1][cstart+2]==mring)){
				val+=40;
			}
			if(rstart<=19 && cstart>=2){
				if(board[rstart+1][cstart-2]==omarker || board[rstart+1][cstart-2]==mring){
					val+=40;
				}
			}
			if(val==0){
				val = 10;
			}
		}else{
			if(rstart<=19 && cstart<=18){
				if(board[rstart+1][cstart+2]==omarker || board[rstart+1][cstart+2]==mring){
					val+=40;
				}
			}
			if(rstart>=1 && cstart>=2 && (board[rstart-1][cstart-2]==omarker || board[rstart-1][cstart-2]==mring)){
				val+=40;
			}
			if(val==0){
				val = 10;
			}
		}
	}else{
		// check ki kisi end par ek position chhodkar favourable marker ho and beech waale par opposite marker ya favourable ring. Agar hai toh add more weight on it else keep it less
		if(dirn==0){
			if(rstart<=12 && (board[rstart+6][cstart]==omarker || board[rstart+6][cstart]==mring) && board[rstart+8][cstart]==marker){
				val+=40;
			}
			if(rstart>=4){
				if(board[rstart-2][cstart]==omarker || board[rstart-2][cstart]==mring){
					if(board[rstart-4][cstart]==marker){
						val+=40;
					}
				}
			}
			if(val==0){
				val = 5;
			}
		}else if(dirn==1){
			if(rstart>=2 && cstart<=16 && (board[rstart-1][cstart+2]==omarker || board[rstart-1][cstart+2]==mring) && board[rstart-2][cstart+4]==marker){
				val+=40;
			}
			if(rstart<=18 && cstart>=4){
				if(board[rstart+1][cstart-2]==omarker || board[rstart+1][cstart-2]==mring){
					if(board[rstart+2][cstart-4]==marker){
						val+=40;
					}
				}
			}
			if(val==0){
				val = 5;
			}
		}else{
			if(rstart<=18 && cstart<=16){
				if(board[rstart+1][cstart+2]==omarker || board[rstart+1][cstart+2]==mring){
					if(board[rstart+2][cstart+4]==marker){
						val+=40;
					}
				}
			}
			if(rstart>=2 && cstart>=4 && (board[rstart-1][cstart-2]==omarker || board[rstart-1][cstart-2]==mring) && board[rstart-2][cstart-4]==marker){
				val+=40;
			}
			if(val==0){
				val = 5;
			}
		}
	}
	return val;
}

int rowpotential6(int board[25][25],int rstart,int cstart,int length,int dirn){
	int marker = board[rstart][cstart];
	int turn = 4 - marker;
	int mring = 2 - turn;
	int omarker = 7 - marker;
	int val = 1;
	if(length==6){
		val = 1000;
	}else if(length==5){
		val = 800;
	}else if(length==4){
		if(dirn==0){
			if(rstart<=16 && (board[rstart+8][cstart]==omarker || board[rstart+8][cstart]==mring)){
				val+=40;
			}
			if(rstart>=2){
				if(board[rstart-2][cstart]==omarker || board[rstart-2][cstart]==mring){
					val+=40;
				}
			}
			if(val==0){
				val = 10;
			}
		}else if(dirn==1){
			if(rstart>=1 && cstart<=22 && (board[rstart-1][cstart+2]==omarker || board[rstart-1][cstart+2]==mring)){
				val+=40;
			}
			if(rstart<=23 && cstart>=2){
				if(board[rstart+1][cstart-2]==omarker || board[rstart+1][cstart-2]==mring){
					val+=40;
				}
			}
			if(val==0){
				val = 10;
			}
		}else{
			if(rstart<=23 && cstart<=22){
				if(board[rstart+1][cstart+2]==omarker || board[rstart+1][cstart+2]==mring){
					val+=40;
				}
			}
			if(rstart>=1 && cstart>=2 && (board[rstart-1][cstart-2]==omarker || board[rstart-1][cstart-2]==mring)){
				val+=40;
			}
			if(val==0){
				val = 10;
			}
		}
	}else if(length==3){
		if(dirn==0){
			if(rstart<=16 && (board[rstart+6][cstart]==omarker || board[rstart+6][cstart]==mring) && board[rstart+8][cstart]==marker){
				val+=40;
			}
			if(rstart>=4){
				if(board[rstart-2][cstart]==omarker || board[rstart-2][cstart]==mring){
					if(board[rstart-4][cstart]==marker){
						val+=40;
					}
				}
			}
			if(val==0){
				val = 5;
			}
		}else if(dirn==1){
			if(rstart>=2 && cstart<=20 && (board[rstart-1][cstart+2]==omarker || board[rstart-1][cstart+2]==mring) && board[rstart-2][cstart+4]==marker){
				val+=40;
			}
			if(rstart<=22 && cstart>=4){
				if(board[rstart+1][cstart-2]==omarker || board[rstart+1][cstart-2]==mring){
					if(board[rstart+2][cstart-4]==marker){
						val+=40;
					}
				}
			}
			if(val==0){
				val = 5;
			}
		}else{
			if(rstart<=22 && cstart<=20){
				if(board[rstart+1][cstart+2]==omarker || board[rstart+1][cstart+2]==mring){
					if(board[rstart+2][cstart+4]==marker){
						val+=40;
					}
				}
			}
			if(rstart>=2 && cstart>=4 && (board[rstart-1][cstart-2]==omarker || board[rstart-1][cstart-2]==mring) && board[rstart-2][cstart-4]==marker){
				val+=40;
			}
			if(val==0){
				val = 5;
			}
		}
	}else{
		val = 2;
	}
	return val;
}

// row formed functions of some desired continuous strand of markers
string rowformed5(int board[21][21],int turn,int reqd_markers,bool movenotvalue){
	string str = "";
	int marker = 4 - turn;
	int counter = 0;
	int rstart = 0;
	int cstart = 0;
	int rend = 0;
	int cend = 0;
	int x = 0;
	int y = 0;
	int num = 0;
	bool isRow = false;
	vector<int> vec;
	int val = 0;

	// for -45 degree kind of movement
	counter = 1;
	while(counter<=11){
		if(counter==1){
			rstart = 16;
			cstart = 2;
			rend = 20 - reqd_markers;
			cend = 10 - 2*reqd_markers;
		}else if(counter<=5){
			rstart = 17 - 2*counter;
			cstart = 0;
			rend = 22 - counter - reqd_markers;
			cend = 10 + 2*counter - 2*reqd_markers;
		}else if(counter==6){
			rstart = 6;
			cstart = 2;
			rend = 15 - reqd_markers;
			cend = 20 - 2*reqd_markers;
		}else if(counter<=10){
			rstart = 11 - counter;
			cstart = 2*counter - 12;
			rend = 28 - 2*counter - reqd_markers;
			cend = 22 - 2*reqd_markers;
		}else{
			rstart = 1;
			cstart = 12;
			rend = 5 - reqd_markers;
			cend = 20 - 2*reqd_markers;
		}

		x = rstart;
		y = cstart;
		while(x<=rend && y<=cend){
			num = 0;
			while(num<reqd_markers){
				if(board[x+num][y+2*num]==marker){
					isRow = true;
				}else{
					isRow = false;
					break;
				}
				num++;
			}

			if(isRow){
				if(movenotvalue){
					vec = rev_convertor(x,y);
					str = "RS " + to_string(vec[0]) + " " + to_string(vec[1]);
					str+=" RE ";
					vec = rev_convertor(x+reqd_markers-1,y+2*(reqd_markers-1));
					str+=(to_string(vec[0]) + " " + to_string(vec[1]));
					return str;
				}else{
					val+=rowpotential5(board,x,y,reqd_markers,-1);
				}
			}
			x+=1;
			y+=2;
		}
		counter+=1;
	}

	// for 0 degree kind of movement that is vertical movement
	counter = 1;
	while(counter<=11){
		if(counter==1){
			rstart = 7;
			cstart = 0;
			rend = 15 - 2*reqd_markers;
			cend = 0;
		}else if(counter<=5){
			rstart = 6 - counter;
			cstart = 2*counter - 2;
			rend = 16 + counter - 2*reqd_markers;
			cend = 2*counter - 2;
		}else if(counter==6){
			rstart = 2;
			cstart = 10;
			rend = 20 - 2*reqd_markers;
			cend = 10;
		}else if(counter<=10){
			rstart = counter - 6;
			cstart = 2*counter - 2;
			rend = 28 - counter - 2*reqd_markers;
			cend = 2*counter - 2;
		}else{
			rstart = 7;
			cstart = 20;
			rend = 15 - 2*reqd_markers;
			cend = 20;
		}

		x = rstart;
		y = cstart;
		while(x<=rend && y<=cend){
			num = 0;
			while(num<reqd_markers){
				if(board[x+2*num][y]==marker){
					isRow = true;
				}else{
					isRow = false;
					break;
				}
				num++;
			}

			if(isRow){
				if(movenotvalue){
					vec = rev_convertor(x,y);
					str = "RS " + to_string(vec[0]) + " " + to_string(vec[1]);
					str+=" RE ";
					vec = rev_convertor(x+2*(reqd_markers-1),y);
					str+=(to_string(vec[0]) + " " + to_string(vec[1]));
					return str;
				}else{
					val+=rowpotential5(board,x,y,reqd_markers,0);
				}
			}
			x+=2;
		}
		counter+=1;
	}

	// for +45 degree kind of movement
	counter = 1;
	while(counter<=11){
		if(counter==1){
			rstart = 4;
			cstart = 2;
			rend = reqd_markers;
			cend = 10 - 2*reqd_markers;
		}else if(counter<=5){
			rstart = 2*counter + 3;
			cstart = 0;
			rend = counter + reqd_markers - 2;
			cend = 10 + 2*counter - 2*reqd_markers;
		}else if(counter==6){
			rstart = 14;
			cstart = 2;
			rend = 5 + reqd_markers;
			cend = 20 - 2*reqd_markers;
		}else if(counter<=10){
			rstart = 9 + counter;
			cstart = 2*counter - 12;
			rend = 2*counter + reqd_markers - 8;
			cend = 22 - 2*reqd_markers;
		}else{
			rstart = 19;
			cstart = 12;
			rend = 15 + reqd_markers;
			cend = 20 - 2*reqd_markers;
		}

		x = rstart;
		y = cstart;
		while(x>=rend && y<=cend){
			num = 0;
			while(num<reqd_markers){
				if(board[x-num][y+2*num]==marker){
					isRow = true;
				}else{
					isRow = false;
					break;
				}
				num++;
			}

			if(isRow){
				if(movenotvalue){
					vec = rev_convertor(x,y);
					str = "RS " + to_string(vec[0]) + " " + to_string(vec[1]);
					str+=" RE ";
					vec = rev_convertor(x-reqd_markers+1,y+2*(reqd_markers-1));
					str+=(to_string(vec[0]) + " " + to_string(vec[1]));
					return str;
				}else{
					val+=rowpotential5(board,x,y,reqd_markers,1);
				}
			}
			x-=1;
			y+=2;
		}
		counter+=1;
	}
	if(movenotvalue){
		return str;
	}else{
		return to_string(val);
	}
}

string rowformed6(int board[25][25],int turn,int reqd_markers,bool movenotvalue){
	string str = "";
	int marker = 4 - turn;
	int counter = 0;
	int rstart = 0;
	int cstart = 0;
	int rend = 0;
	int cend = 0;
	int x = 0;
	int y = 0;
	int num = 0;
	bool isRow = false;
	vector<int> vec;
	int val = 0;

	// for -45 degree kind of movement
	counter = 1;
	while(counter<=13){
		if(counter==1){
			rstart = 19;
			cstart = 2;
			rend = 24 - reqd_markers;
			cend = 12 - 2*reqd_markers;
		}else if(counter<=6){
			rstart = 20 - 2*counter;
			cstart = 0;
			rend = 26 - counter - reqd_markers;
			cend = 12 + 2*counter - 2*reqd_markers;
		}else if(counter==7){
			rstart = 7;
			cstart = 2;
			rend = 18 - reqd_markers;
			cend = 24 - 2*reqd_markers;
		}else if(counter<=12){
			rstart = 13 - counter;
			cstart = 2*counter - 14;
			rend = 33 - 2*counter - reqd_markers;
			cend = 26 - 2*reqd_markers;
		}else{
			rstart = 1;
			cstart = 14;
			rend = 6 - reqd_markers;
			cend = 24 - 2*reqd_markers;
		}

		x = rstart;
		y = cstart;
		while(x<=rend && y<=cend){
			num = 0;
			while(num<reqd_markers){
				if(board[x+num][y+2*num]==marker){
					isRow = true;
				}else{
					isRow = false;
					break;
				}
				num++;
			}

			if(isRow){
				if(movenotvalue){
					vec = rev_convertor(x,y);
					str = "RS " + to_string(vec[0]) + " " + to_string(vec[1]);
					str+=" RE ";
					vec = rev_convertor(x+reqd_markers-1,y+2*(reqd_markers-1));
					str+=(to_string(vec[0]) + " " + to_string(vec[1]));
					return str;
				}else{
					val+=rowpotential6(board,x,y,reqd_markers,-1);
				}
			}
			x+=1;
			y+=2;
		}
		counter+=1;
	}

	// for 0 degree kind of movement that is vertical movement
	counter = 1;
	while(counter<=13){
		if(counter==1){
			rstart = 8;
			cstart = 0;
			rend = 18 - 2*reqd_markers;
			cend = 0;
		}else if(counter<=6){
			rstart = 7 - counter;
			cstart = 2*counter - 2;
			rend = 19 + counter - 2*reqd_markers;
			cend = 2*counter - 2;
		}else if(counter==7){
			rstart = 2;
			cstart = 12;
			rend = 24 - 2*reqd_markers;
			cend = 12;
		}else if(counter<=12){
			rstart = counter - 7;
			cstart = 2*counter - 2;
			rend = 33 - counter - 2*reqd_markers;
			cend = 2*counter - 2;
		}else{
			rstart = 8;
			cstart = 24;
			rend = 18 - 2*reqd_markers;
			cend = 24;
		}

		x = rstart;
		y = cstart;
		while(x<=rend && y<=cend){
			num = 0;
			while(num<reqd_markers){
				if(board[x+2*num][y]==marker){
					isRow = true;
				}else{
					isRow = false;
					break;
				}
				num++;
			}

			if(isRow){
				if(movenotvalue){
					vec = rev_convertor(x,y);
					str = "RS " + to_string(vec[0]) + " " + to_string(vec[1]);
					str+=" RE ";
					vec = rev_convertor(x+2*(reqd_markers-1),y);
					str+=(to_string(vec[0]) + " " + to_string(vec[1]));
					return str;
				}else{
					val+=rowpotential6(board,x,y,reqd_markers,0);
				}
			}
			x+=2;
		}
		counter+=1;
	}

	// for +45 degree kind of movement
	counter = 1;
	while(counter<=13){
		if(counter==1){
			rstart = 5;
			cstart = 2;
			rend = reqd_markers;
			cend = 12 - 2*reqd_markers;
		}else if(counter<=6){
			rstart = 2*counter + 4;
			cstart = 0;
			rend = counter + reqd_markers - 2;
			cend = 12 + 2*counter - 2*reqd_markers;
		}else if(counter==7){
			rstart = 17;
			cstart = 2;
			rend = 6 + reqd_markers;
			cend = 24 - 2*reqd_markers;
		}else if(counter<=12){
			rstart = 11 + counter;
			cstart = 2*counter - 14;
			rend = 2*counter + reqd_markers - 9;
			cend = 26 - 2*reqd_markers;
		}else{
			rstart = 23;
			cstart = 14;
			rend = 18 + reqd_markers;
			cend = 24 - 2*reqd_markers;
		}

		x = rstart;
		y = cstart;
		while(x>=rend && y<=cend){
			num = 0;
			while(num<reqd_markers){
				if(board[x-num][y+2*num]==marker){
					isRow = true;
				}else{
					isRow = false;
					break;
				}
				num++;
			}

			if(isRow){
				if(movenotvalue){
					vec = rev_convertor(x,y);
					str = "RS " + to_string(vec[0]) + " " + to_string(vec[1]);
					str+=" RE ";
					vec = rev_convertor(x-reqd_markers+1,y+2*(reqd_markers-1));
					str+=(to_string(vec[0]) + " " + to_string(vec[1]));
					return str;
				}else{
					val+=rowpotential6(board,x,y,reqd_markers,1);
				}
			}
			x-=1;
			y+=2;
		}
		counter+=1;
	}
	if(movenotvalue){
		return str;
	}else{
		return to_string(val);
	}
}

// checks and returns the premove after the opponent plays its moves
string rowformed35(int board[21][21],int turn,int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5],int reqd_markers){
	string str = "";
	string str2 = "";
	int cycle = 0;

	while(cycle<3){
		str2 = rowformed5(board,turn,reqd_markers,true);
		if(str2.length()!=0){
			if(str.length()==0){
				str+=str2;
				str+=" ";
			}else{
				str+=" ";
				str+=str2;
				str+=" ";
			}
			movomstring15(str2,turn,board,mringrow,mringcol,oringrow,oringcol);
			str2 = "";
			str2 = remove_ringmove5(turn,board,mringrow,mringcol,oringrow,oringcol);
			str+=str2;
			movomstring15(str2,turn,board,mringrow,mringcol,oringrow,oringcol);
			str2 = "";
			if(!shouldproceed5(board,turn)){
				return str;
			}
		}else{
			break;
		}
		cycle+=1;
	}

	return str;
}

string rowformed36(int board[25][25],int turn,int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6],int reqd_markers){
	string str = "";
	string str2 = "";
	int cycle = 0;

	while(cycle<3){
		str2 = rowformed6(board,turn,reqd_markers,true);
		if(str2.length()!=0){
			if(str.length()==0){
				str+=str2;
				str+=" ";
			}else{
				str+=" ";
				str+=str2;
				str+=" ";
			}
			movomstring16(str2,turn,board,mringrow,mringcol,oringrow,oringcol);
			str2 = "";
			str2 = remove_ringmove6(turn,board,mringrow,mringcol,oringrow,oringcol);
			str+=str2;
			movomstring16(str2,turn,board,mringrow,mringcol,oringrow,oringcol);
			str2 = "";
			if(!shouldproceed6(board,turn)){
				return str;
			}
		}else{
			break;
		}
		cycle+=1;
	}

	return str;
}

// finding friends functions
vector<string> FF5(int board[21][21],int turn,int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5],int reqd_markers){
	int ringnum = 0;
	int row = 0;
	int col = 0;
	int xc[5];
	int yc[5];
	int tempxc[5];
	int tempyc[5];
	vector<string> vec;
	vector<int> v;
	string str = "";
	string select = "";
	string l3 = "";
	string base = "";
	int tempBoard[21][21];
	int tempBoard2[21][21];
	
	board_copy5(tempBoard,board);
	if(turn==1){
		ring_copy5(xc,yc,mringrow,mringcol);
		str = rowformed35(tempBoard,turn,xc,yc,oringrow,oringcol,reqd_markers);
	}else{
		ring_copy5(xc,yc,oringrow,oringcol);
		str = rowformed35(tempBoard,turn,mringrow,mringcol,xc,yc,reqd_markers);
	}

	if(str.length()!=0){
		if(!shouldproceed5(tempBoard,turn)){
			vec.push_back(str);
			return vec;
		}
		str+=" ";
	}
	board_copy5(tempBoard2,tempBoard);

	for(ringnum = 0;ringnum<5;ringnum++){
		if(xc[ringnum]!=-1 && yc[ringnum]!=-1){

			// vertically up movements
			row = xc[ringnum];
			col = yc[ringnum];
			v = rev_convertor(row,col);
			base = "S " + to_string(v[0]) + " " + to_string(v[1]) + " M ";
			select = base;
			row-=2;

			while(row>=1 && tempBoard2[row][col]==0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row-=2;
			}

			if(row>=1){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row>=0 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row-=2;
					}
					if(row>=1 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// vertically down movements
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row+=2;

			while(row<=19 && tempBoard2[row][col]==0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row+=2;
			}

			if(row<=19){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row<=19 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row+=2;
					}
					if(row<=19 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +45 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row-=1;
			col+=2;
			while(row>=1 && col<=20 && tempBoard2[row][col]==0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row-=1;
				col+=2;
			}

			if(row>=1 && col<=20){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row>=0 && col<=20 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row-=1;
						col+=2;
					}
					if(row>=1 && col<=20 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +135 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row-=1;
			col-=2;
			while(row>=1 && col>=0 && tempBoard2[row][col]==0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row-=1;
				col-=2;
			}

			if(row>=1 && col>=0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row>=0 && col>=0 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row-=1;
						col-=2;
					}
					if(row>=1 && col>=0 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +225 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row+=1;
			col-=2;
			while(row<=19 && col>=0 && tempBoard2[row][col]==0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row+=1;
				col-=2;
			}

			if(row<=19 && col>=0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row<=19 && col>=0 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row+=1;
						col-=2;
					}
					if(row<=19 && col>=0 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +315 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row+=1;
			col+=2;
			while(row<=19 && col<=20 && tempBoard2[row][col]==0){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row+=1;
				col+=2;
			}

			if(row<=19 && col<=20){
				board_copy5(tempBoard,tempBoard2);
				ring_copy5(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row<=19 && col<=20 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row+=1;
						col+=2;
					}
					if(row<=19 && col<=20 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring15(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed35(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}
		}
	}
	return vec;
}

vector<string> FF6(int board[25][25],int turn,int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6],int reqd_markers){
	int ringnum = 0;
	int row = 0;
	int col = 0;
	int xc[6];
	int yc[6];
	int tempxc[6];
	int tempyc[6];
	vector<string> vec;
	vector<int> v;
	string str = "";
	string select = "";
	string l3 = "";
	string base = "";
	int tempBoard[25][25];
	int tempBoard2[25][25];

	board_copy6(tempBoard,board);
	if(turn==1){
		ring_copy6(xc,yc,mringrow,mringcol);
		str = rowformed36(tempBoard,turn,xc,yc,oringrow,oringcol,reqd_markers);
	}else{
		ring_copy6(xc,yc,oringrow,oringcol);
		str = rowformed36(tempBoard,turn,mringrow,mringcol,xc,yc,reqd_markers);
	}
	if(str.length()!=0){
		if(!shouldproceed6(tempBoard,turn)){
			vec.push_back(str);
			return vec;
		}
		str+=" ";
	}
	board_copy6(tempBoard2,tempBoard);

	for(ringnum=0;ringnum<6;ringnum++){
		if(xc[ringnum]!=-1 && yc[ringnum]!=-1){

			// vertically up movements
			row = xc[ringnum];
			col = yc[ringnum];
			v = rev_convertor(row,col);
			base = "S " + to_string(v[0]) + " " + to_string(v[1]) + " M ";
			select = base;
			row-=2;

			while(row>=1 && tempBoard2[row][col]==0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row-=2;
			}

			if(row>=1){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row>=0 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row-=2;
					}
					if(row>=1 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// vertically down movements
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row+=2;

			while(row<=23 && tempBoard2[row][col]==0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row+=2;
			}

			if(row<=23){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row<=23 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row+=2;
					}
					if(row<=23 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +45 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row-=1;
			col+=2;
			while(row>=1 && col<=24 && tempBoard2[row][col]==0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row-=1;
				col+=2;
			}

			if(row>=1 && col<=24){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row>=0 && col<=24 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row-=1;
						col+=2;
					}
					if(row>=1 && col<=24 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +135 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row-=1;
			col-=2;
			while(row>=1 && col>=0 && tempBoard2[row][col]==0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row-=1;
				col-=2;
			}

			if(row>=1 && col>=0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row>=0 && col>=0 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row-=1;
						col-=2;
					}
					if(row>=1 && col>=0 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +225 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row+=1;
			col-=2;
			while(row<=23 && col>=0 && tempBoard2[row][col]==0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row+=1;
				col-=2;
			}

			if(row<=23 && col>=0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row<=23 && col>=0 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row+=1;
						col-=2;
					}
					if(row<=23 && col>=0 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}

			// +315 degree kinda movement
			row = xc[ringnum];
			col = yc[ringnum];
			select = base;
			row+=1;
			col+=2;
			while(row<=23 && col<=24 && tempBoard2[row][col]==0){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);
				v = rev_convertor(row,col);
				select+=(to_string(v[0]) + " " + to_string(v[1]));
				movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
				l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
				if(l3.length()!=0){
					select+=" ";
					select+=l3;
				}
				vec.push_back(str+select);
				select = "";
				select = base;
				row+=1;
				col+=2;
			}

			if(row<=23 && col<=24){
				board_copy6(tempBoard,tempBoard2);
				ring_copy6(tempxc,tempyc,xc,yc);

				if(tempBoard[row][col]==3 || tempBoard[row][col]==4){
					while(row<=23 && col<=24 && (tempBoard[row][col]==3 || tempBoard[row][col]==4)){
						row+=1;
						col+=2;
					}
					if(row<=23 && col<=24 && tempBoard[row][col]==0){
						v = rev_convertor(row,col);
						select = base;
						select+=(to_string(v[0]) + " " + to_string(v[1]));
						movomstring16(select,turn,tempBoard,tempxc,tempyc,tempxc,tempyc);
						l3 = rowformed36(tempBoard,turn,tempxc,tempyc,tempxc,tempyc,reqd_markers);
						if(l3.length()!=0){
							select+=" ";
							select+=l3;
						}
						vec.push_back(str+select);
					}
				}
			}			
		}
	}

	return vec;
}

// evaluation functions
int eval5(int board[21][21],int turn,int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	int val = 0;
	int val2 = 0;
	int mring = 2 - turn;
	int oring = 1 + turn;
	int mmark = 4 - turn;
	int omark = 3 + turn;
	int mrc = 0;
	int orc = 0;
	int mmc = 0;
	int omc = 0;
	int i = 0;
	int j = 0;
	int counter = 0;
	int weight = 2;
	string rows = "";

	for(i = 0;i<21;i++){
		for(j = 0;j<21;j++){
			if(board[i][j]==mring){
				mrc++;
			}else if(board[i][j]==oring){
				orc++;
			}else if(board[i][j]==mmark){
				mmc++;
			}else{
				// do nothing
			}
		}
	}

	mrc = 5 - mrc;
	orc = 5 - orc;
	if(mrc==3){
		val = 10 - orc;
	}else if(mrc==2){
		if(orc==3){
			val = 2;
		}else{
			val = 7 - orc;
		}
	}else if(mrc==1){
		if(orc==3){
			val = 1;
		}else{
			val = 6 - orc;
		}
	}else{
		if(orc==3){
			val = 0;
		}else{
			val = 5 - orc;
		}
	}
	val = 1000*val + 10*mmc;

	for(counter=5;counter>=2;counter--){
		rows = rowformed5(board,turn,counter,false);
		val2 = stringtoint(rows);
		val+=(3*val2);
	}

	for(counter=5;counter>=4;counter--){
		rows = rowformed5(board,1-turn,counter,false);
		val2 = stringtoint(rows);
		val-=(2*val2);
	}

	return val;
}

int eval6(int board[25][25],int turn,int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6],int num_markers){
	int val = 0;
	int mring = 2 - turn;
	int mrc = 0;
	int oring = 1 + turn;
	int orc = 0;
	int mmark = 4 - turn;
	int mmc = 0;
	int omark = 3 + turn;
	int omc = 0;
	int counter = num_markers-1;
	int val2 = 0;
	int i = 0;
	int j = 0;
	string rows;

	for(i = 0;i<25;i++){
		for(j = 0;j<25;j++){
			if(board[i][j]==mring){
				mrc++;
			}else if(board[i][j]==oring){
				orc++;
			}else if(board[i][j]==mmark){
				mmc++;
			}else{
				// do nothing
			}
		}
	}

	mrc = 5 - mrc;
	orc = 5 - orc;
	if(mrc==3){
		val = 10 - orc;
	}else if(mrc==2){
		if(orc==3){
			val = 2;
		}else{
			val = 7 - orc;
		}
	}else if(mrc==1){
		if(orc==3){
			val = 1;
		}else{
			val = 6 - orc;
		}
	}else{
		if(orc==3){
			val = 0;
		}else{
			val = 5 - orc;
		}
	}
	val = 1000*val + 10*mmc;

	for(counter=num_markers;counter>=num_markers-3;counter--){
		rows = rowformed6(board,turn,counter,false);
		val2 = stringtoint(rows);
		val+=(3*val2);
	}

	for(counter=num_markers;counter>=num_markers-2;counter--){
		rows = rowformed6(board,1-turn,counter,false);
		val2 = stringtoint(rows);
		val-=(2*val2);
	}
	return val;
}

// move ordering functions
vector<string> move_ordering5(vector<string> v,int board[21][21],int turn,int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5]){
	vector<string> vec;
	map<int,string> storage;
	int i = 0;
	int tempBoard[21][21];
	int mr[5];
	int mc[5];
	int opr[5];
	int opc[5];
	int val = 0;
	int idx = 0;

	for(i = 0;i<v.size();i++){
		board_copy5(tempBoard,board);
		ring_copy5(mr,mc,mringrow,mringcol);
		ring_copy5(opr,opc,oringrow,oringcol);
		movromstring5(v[i],turn,tempBoard,mr,mc,opr,opc);
		val = eval5(tempBoard,turn,mr,mc,opr,opc);
		storage.insert(pair <int,string> (val,v[i]));
	}

	map<int,string>::reverse_iterator rit;
  	for(rit=storage.rbegin();rit!=storage.rend();++rit){
  		vec.push_back(rit->second);
  	}
	return vec;
}

vector<string> move_ordering6(vector<string> v,int board[25][25],int turn,int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6],int num_markers){
	vector<string> vec;
	map<int,string> storage;
	int i = 0;
	int tempBoard[25][25];
	int mr[6];
	int mc[6];
	int opr[6];
	int opc[6];
	int val = 0;
	int idx = 0;

	for(i = 0;i<v.size();i++){
		board_copy6(tempBoard,board);
		ring_copy6(mr,mc,mringrow,mringcol);
		ring_copy6(opr,opc,oringrow,oringcol);
		movromstring6(v[i],turn,tempBoard,mr,mc,opr,opc);
		val = eval6(tempBoard,turn,mr,mc,opr,opc,num_markers);
		storage.insert(pair <int,string> (val,v[i]));
	}

	map<int,string>::reverse_iterator rit;
  	for(rit=storage.rbegin();rit!=storage.rend();++rit){
  		vec.push_back(rit->second);
  	}
	return vec;
}

// alpha beta pruning functions
int ABP5(int board[21][21],int turn,int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5],int alpha,int beta,int depth,bool maxormin,int num_markers){
	int val = 0;
	int idx = 0;
	int tempBoard[21][21];
	int mxc[5];
	int myc[5];
	int oxc[5];
	int oyc[5];
	vector<string> vec;

	if(depth==0){
		val = eval5(board,turn,mringrow,mringcol,oringrow,oringcol);
	}else{
		if(maxormin){
			val = INT_MIN;
		}else{
			val = INT_MAX;
		}
		vec = FF5(board,turn,mringrow,mringcol,oringrow,oringcol,num_markers);
		// vec = move_ordering5(vec,board,turn,mringrow,mringcol,oringrow,oringcol);
		for(idx = 0;idx<vec.size();idx++){
			board_copy5(tempBoard,board);
			ring_copy5(mxc,myc,mringrow,mringcol);
			ring_copy5(oxc,oyc,oringrow,oringcol);
			movromstring5(vec[idx],turn,tempBoard,mxc,myc,oxc,oyc);
			
			if(maxormin){
				val = max(val,ABP5(tempBoard,1-turn,mxc,myc,oxc,oyc,alpha,beta,depth-1,false,num_markers));
				if(val>=beta){
					return val;
				}
				alpha = max(alpha,val);
			}else{
				val = min(val,ABP5(tempBoard,1-turn,mxc,myc,oxc,oyc,alpha,beta,depth-1,true,num_markers));
				if(val<=alpha){
					return val;
				}
				beta = min(beta,val);
			}
		}
	}
	return val;
}

int ABP6(int board[25][25],int turn,int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6],int alpha,int beta,int depth,bool maxormin,int num_markers){
	int val = 0;
	int idx = 0;
	int tempBoard[25][25];
	int mxc[6];
	int myc[6];
	int oxc[6];
	int oyc[6];
	vector<string> vec;

	if(depth==0){
		val = eval6(board,turn,mringrow,mringcol,oringrow,oringcol,num_markers);
	}else{
		if(maxormin){
			val = INT_MIN;
		}else{
			val = INT_MAX;
		}
		vec = FF6(board,turn,mringrow,mringcol,oringrow,oringcol,num_markers);
		// vec = move_ordering6(vec,board,turn,mringrow,mringcol,oringrow,oringcol,num_markers);
		for(idx = 0;idx<vec.size();idx++){
			board_copy6(tempBoard,board);
			ring_copy6(mxc,myc,mringrow,mringcol);
			ring_copy6(oxc,oyc,oringrow,oringcol);
			movromstring6(vec[idx],turn,tempBoard,mxc,myc,oxc,oyc);
			
			if(maxormin){
				val = max(val,ABP6(tempBoard,1-turn,mxc,myc,oxc,oyc,alpha,beta,depth-1,false,num_markers));
				if(val>=beta){
					return val;
				}
				alpha = max(alpha,val);
			}else{
				val = min(val,ABP6(tempBoard,1-turn,mxc,myc,oxc,oyc,alpha,beta,depth-1,true,num_markers));
				if(val<=alpha){
					return val;
				}
				beta = min(beta,val);
			}
		}
	}
	return val;
}

// cut off search functions
string play5(int board[21][21],int turn,int mringrow[5],int mringcol[5],int oringrow[5],int oringcol[5],int num_markers){
	string str = "";
	vector<string> vec;
	int max = INT_MIN;
	int idx = 0;
	int val = 0;
	int tempBoard[21][21];
	int mxc[5];
	int myc[5];
	int oxc[5];
	int oyc[5];
	int alpha = 0;
	int beta = 0;
	int depth_to_search = 3;
	int depth = 0;
	vec = FF5(board,1,mringrow,mringcol,oringrow,oringcol,num_markers);
	// vec = move_ordering5(vec,board,1,mringrow,mringcol,oringrow,oringcol);
	
	for(idx = 0;idx<vec.size();idx++){
		depth = depth_to_search-1;
		alpha = INT_MIN;
		beta = INT_MAX;
		board_copy5(tempBoard,board);
		ring_copy5(mxc,myc,mringrow,mringcol);
		ring_copy5(oxc,oyc,oringrow,oringcol);
		movromstring5(vec[idx],turn,tempBoard,mxc,myc,oxc,oyc);
		
		val = ABP5(tempBoard,1-turn,mxc,myc,oxc,oyc,alpha,beta,depth,false,num_markers);
		if(val>max){
			max = val;
			str = vec[idx];
		}
	}
	return str;
}

string play6(int board[25][25],int turn,int mringrow[6],int mringcol[6],int oringrow[6],int oringcol[6],int num_markers){
	string str = "";
	vector<string> vec;
	int max = INT_MIN;
	int idx = 0;
	int val = 0;
	int tempBoard[25][25];
	int mxc[6];
	int myc[6];
	int oxc[6];
	int oyc[6];
	int alpha = 0;
	int beta = 0;
	int depth_to_search = 2;
	int depth = 0;
	vec = FF6(board,1,mringrow,mringcol,oringrow,oringcol,num_markers);
	// vec = move_ordering6(vec,board,1,mringrow,mringcol,oringrow,oringcol,num_markers);
	
	for(idx = 0;idx<vec.size();idx++){
		depth = depth_to_search-1;
		alpha = INT_MIN;
		beta = INT_MAX;
		board_copy6(tempBoard,board);
		ring_copy6(mxc,myc,mringrow,mringcol);
		ring_copy6(oxc,oyc,oringrow,oringcol);
		movromstring6(vec[idx],turn,tempBoard,mxc,myc,oxc,oyc);
		
		val = ABP6(tempBoard,1-turn,mxc,myc,oxc,oyc,alpha,beta,depth,false,num_markers);
		if(val>max){
			max = val;
			str = vec[idx];
		}
	}
	return str;
}

// main function
int main(){
	int num_rings = 0;
	int num_markers = 0;
	int player_id = 0;
	int time_limit = 0;
	int board_size = 0;
	int counter = 0;
	string s = "";
	string s2 = "";
	vector<string> vec;

	cin >> player_id;
	cin >> board_size;
	cin >> time_limit;
	cin >> num_markers;
	cin.ignore();
	num_rings = board_size;
	
	if(board_size==5){
		int board[21][21];
		int mringrow[5];
		int mringcol[5];
		int oringrow[5];
		int oringcol[5];
		initialize5(board,mringrow,mringcol,oringrow,oringcol);
		if(player_id==2){
			// get other player's move
			while(true){
				getline(cin,s);
				movromstring5(s,0,board,mringrow,mringcol,oringrow,oringcol);
				if(counter<5){
					// getline(cin,s);
					s = placement5(board,mringrow,mringcol,oringrow,oringcol);
				}else{
					s = play5(board,1,mringrow,mringcol,oringrow,oringcol,num_markers);
				}
				counter+=1;
				cout << s << endl;
				movromstring5(s,1,board,mringrow,mringcol,oringrow,oringcol);
			}
		}else{
			// you play first
			while(true){
				if(counter<5){
					s = placement5(board,mringrow,mringcol,oringrow,oringcol);
				}else{
					s = play5(board,1,mringrow,mringcol,oringrow,oringcol,num_markers);
				}
				movromstring5(s,1,board,mringrow,mringcol,oringrow,oringcol);
				cout << s << endl;
				getline(cin,s);
				movromstring5(s,0,board,mringrow,mringcol,oringrow,oringcol);
				counter+=1;
			}
		}
	}else{
		int board[25][25];
		int mringrow[6];
		int mringcol[6];
		int oringrow[6];
		int oringcol[6];
		initialize6(board,mringrow,mringcol,oringrow,oringcol);
		if(player_id==2){
			// get other player's move
			while(true){
				getline(cin,s);
				movromstring6(s,0,board,mringrow,mringcol,oringrow,oringcol);
				if(counter<6){
					s = placement6(board,mringrow,mringcol,oringrow,oringcol);
				}else{
					s = play6(board,1,mringrow,mringcol,oringrow,oringcol,num_markers);
				}
				counter+=1;
				cout << s << endl;
				movromstring6(s,1,board,mringrow,mringcol,oringrow,oringcol);
			}
		}else{
			// you play first
			while(true){
				if(counter<6){
					s = placement6(board,mringrow,mringcol,oringrow,oringcol);
				}else{
					s = play6(board,1,mringrow,mringcol,oringrow,oringcol,num_markers);
				}
				movromstring6(s,1,board,mringrow,mringcol,oringrow,oringcol);
				cout << s << endl;
				getline(cin,s);
				movromstring6(s,0,board,mringrow,mringcol,oringrow,oringcol);
				counter+=1;
			}
		}
	}
	return 0;
}


