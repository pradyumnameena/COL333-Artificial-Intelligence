#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

int arr[21][21];
map<string,string> rev_eng;
int mringrow[5];
int mringcol[5];
int oringrow[5];
int oringcol[5];

void cpV1(int b1[21][21],int b2[21][21]){
	int i,j;
	for(i = 0;i<21;i++){
		for(j = 0;j<21;j++){
			b1[i][j] = b2[i][j];
		}
	}
}

void cpV2(int x1[5],int y1[5],int x2[5],int y2[5]){
	int i = 0;
	for(i = 0;i<5;i++){
		x1[i] = x2[i];
		y1[i] = y2[i];
	}
}

vector<int> fconvertor(int hex,int pos){
    int n = 5;
    int mid_idx = 4*n + 1;
    int len = (mid_idx-1)/2;
    int part_hex;
    vector <int> vec;
    int arr2[2];

    if(hex==0){
        arr2[0] = len;
        arr2[1] = len;
    }else{
    	part_hex = (int)(pos/hex);
	    
	    if(part_hex==0){
	        arr2[1] = len + (2*pos);
	        arr2[0] = len - (2*hex) + (pos%hex);
	    }else if(part_hex==1){
	        arr2[1] = len + (2*hex);
	        arr2[0] = len - (2*hex) + hex + 2*(pos%hex);
	    }else if(part_hex==2){
	        arr2[1] = len + (2*hex) - 2*(pos%hex);
	        arr2[0] = len - (2*hex) + 3*hex + (pos%hex);
	    }else if(part_hex==3){
	        arr2[1] = len - 2*(pos%hex);
	        arr2[0] = len + (2*hex) - (pos%hex);
	    }else if(part_hex==4){
	        arr2[1] = len - (2*hex);
	        arr2[0] = len + (2*hex) - hex - 2*(pos%hex);
	    }else{
	        arr2[1] = len - (2*hex) + 2*(pos%hex);
	        arr2[0] = len - hex - (pos%hex);
	    }
    }

    vec.push_back(arr2[0]);
    vec.push_back(arr2[1]);
    return vec;
}

vector<int> rev_convertor(int row,int col){
    int hex,pos;
    vector<int> vec;
    string s = to_string(row) + " " + to_string(col);
    string str = rev_eng.at(s);
    int len = str.length();

    if(len==3){
        hex = str.at(0)-'0';
        pos = str.at(2)-'0';
    }else{
        hex = str.at(0)-'0';
        pos = 10*(str.at(2)-'0') + (str.at(3)-'0');
    }

    vec.push_back(hex);
    vec.push_back(pos);
    return vec;
}

void initial_state(int n){
	vector<int> vec;
	string s1,s2;
	int i,j,k,i1,i2;

	for(k = 0;k<5;k++){
    	mringrow[k] = -1;
        mringcol[k] = -1;
        oringrow[k] = -1;
        oringcol[k] = -1;
    }

    for(i1 = 0;i1<=4*n;i1++){
        for(i2 = 0;i2<=4*n;i2++){
            arr[i1][i2] = -1;
        }
    }
    arr[2*n][2*n] = 0;

    vec = fconvertor(0,0);
    s1 = to_string(vec[0]) + " " + to_string(vec[1]);
    s2 = to_string(0) + " " + to_string(0);
    rev_eng.insert(pair <string,string> (s1,s2));


    for(i = 1;i<=n;i++){
        for(j = 0;j<6*i;j++){
            if(i==n && j%i==0){
                // avoid these positions
            }else{
                vec = fconvertor(i,j);
                s1 = to_string(vec[0]) + " " + to_string(vec[1]);
                s2 = to_string(i) + " " + to_string(j);
                rev_eng.insert(pair <string,string> (s1,s2));
                arr[vec[0]][vec[1]] = 0;
            }
        }
    }
}

void boardPR(int row,int col,int turn){
	int i = 0;
    if(arr[row][col]==0){
		if(turn==1){
			for(i = 0;i<5;i++){
				if(mringrow[i]==-1){
					mringrow[i] = row;
					mringcol[i] = col;
					arr[row][col] = 1;
					break;
				}
			}
		}else{
			for(i = 0;i<5;i++){
				if(oringrow[i]==-1){
					oringrow[i] = row;
					oringcol[i] = col;
					arr[row][col] = 2;
					break;
				}
			}
		}
	}else{
		// invalid move
	}
}

void boardOR(int rows,int rowe,int cols, int cole,int turn){
    int vdiff = rowe-rows;
    int hdiff = cole-cols;
    int s1,s2;
    s1 = rows;
    s2 = cols; 
    if(hdiff==0){
        if(vdiff>0){
            for(s1 = rows;s1<=rowe;s1+=2){
                arr[s1][s2] = 0;
            }
        }else{
            for(s1 = rows;s1>=rowe;s1-=2){
                arr[s1][s2] = 0;
            }
        }
    }else if(hdiff>0){
        if(vdiff>0){
            for(s1 = rows;s1<=rowe;s1+=1){
                arr[s1][s2] = 0;
                s2+=2;
            }
        }else{
            for(s1 = rows;s1>=rowe;s1-=1){
                arr[s1][s2] = 0;
                s2+=2;
            }
        }
    }else{
        if(vdiff>0){
            for(s1 = rows;s1<=rowe;s1+=1){
                arr[s1][s2] = 0;
                s2-=2;
            }
        }else{
            for(s1 = rows;s1>=rowe;s1-=1){
                arr[s1][s2] = 0;
                s2-=2;
            }
        }
    }
}

void boardORV2(int rows,int rowe,int cols, int cole,int turn,int board5[21][21],int xc[5],int yc[5]){
    int vdiff = rowe-rows;
    int hdiff = cole-cols;
    int s1,s2;
    s1 = rows;
    s2 = cols; 
    if(hdiff==0){
        if(vdiff>0){
            for(s1 = rows;s1<=rowe;s1+=2){
                board5[s1][s2] = 0;
            }
        }else{
            for(s1 = rows;s1>=rowe;s1-=2){
                board5[s1][s2] = 0;
            }
        }
    }else if(hdiff>0){
        if(vdiff>0){
            for(s1 = rows;s1<=rowe;s1+=1){
                board5[s1][s2] = 0;
                s2+=2;
            }
        }else{
            for(s1 = rows;s1>=rowe;s1-=1){
                board5[s1][s2] = 0;
                s2+=2;
            }
        }
    }else{
        if(vdiff>0){
            for(s1 = rows;s1<=rowe;s1+=1){
                board5[s1][s2] = 0;
                s2-=2;
            }
        }else{
            for(s1 = rows;s1>=rowe;s1-=1){
                board5[s1][s2] = 0;
                s2-=2;
            }
        }
    }
}

void boardRR(int row,int col,int turn){
    if(arr[row][col]==(2-turn)){
        arr[row][col] = 0;
        int idx;
        if(turn==1){
            for(idx=0;idx<5;idx++){
                if(mringrow[idx]==row && mringcol[idx]==col){
                    mringrow[idx]=-1;
                    mringcol[idx]=-1;
                    break;
                }
            }
        }else{
            for(idx=0;idx<5;idx++){
                if(oringrow[idx]==row && oringcol[idx]==col){
                    oringrow[idx]=-1;
                    oringcol[idx]=-1;
                    break;
                }
            }
        }
    }else{
        // give error
    }
}

void boardRRV2(int row,int col,int turn,int board5[21][21],int xc[5],int yc[5]){
    if(board5[row][col]==(2-turn)){
        board5[row][col] = 0;
        int idx;
        for(idx=0;idx<5;idx++){
            if(xc[idx]==row && yc[idx]==col){
                xc[idx]=-1;
                yc[idx]=-1;
                break;
            }
        }
    }else{
        // give error
    }
}

void boardMR(int irow,int icol,int frow,int fcol,int turn){
    int ring_val = 2 - turn;
    int mark = ring_val+2;
    int count = 0;
    
    if(turn==1){
        for(int idx2 = 0;idx2<5;idx2++){
            if(mringrow[idx2]==irow && mringcol[idx2]==icol){
                mringrow[idx2] = frow;
                mringcol[idx2] = fcol;
                break;
            }
        }
    }else{
        for(int idx2 = 0;idx2<5;idx2++){
            if(oringrow[idx2]==irow && oringcol[idx2]==icol){
            	oringrow[idx2] = frow;
                oringcol[idx2] = fcol;
                break;
            }
        }
    }

    if(arr[irow][icol]==ring_val && arr[frow][fcol]==0){
        int hdiff = frow - irow;
        int vdiff = fcol - icol;
        int start = 0;
        int start2 = icol;
        arr[irow][icol] = 7-mark;
        if(vdiff==0){
            if(hdiff>0){
                for(start = irow;start<=frow;start+=2){
                    if(arr[start][icol]==3 || arr[start][icol]==4){
                        arr[start][icol] = 7 - arr[start][icol];
                    } 
                }
            }else{
                for(start = irow;start>=frow;start-=2){
                    if(arr[start][icol]==3 || arr[start][icol]==4){
                        arr[start][icol] = 7 - arr[start][icol];
                    } 
                }
            }
        }else if(vdiff>0){
            if(hdiff>0){
                for(start = irow;start<=frow;start++){
                    if(arr[start][start2]==3 || arr[start][start2]==4){
                        arr[start][start2] = 7 - arr[start][start2];
                    }
                    start2+=2;
                }
            }else{
                for(start = irow;start>=frow;start--){
                    if(arr[start][start2]==3 || arr[start][start2]==4){
                        arr[start][start2] = 7 - arr[start][start2];
                    }
                    start2+=2;
                }
            }
        }else{
            if(hdiff>0){
                for(start = irow;start<=frow;start++){
                    if(arr[start][start2]==3 || arr[start][start2]==4){
                        arr[start][start2] = 7 - arr[start][start2];
                    }
                    start2-=2;
                }
            }else{
                for(start = irow;start>=frow;start--){
                    if(arr[start][start2]==3 || arr[start][start2]==4){
                        arr[start][start2] = 7 - arr[start][start2];
                    }
                    start2-=2;
                }
            }
        }

        arr[frow][fcol] = ring_val;
    }else{
        // move not allowed here
    }
}

void boardMRV2(int irow,int icol,int frow,int fcol,int turn,int board5[21][21],int xc[5],int yc[5]){
    int ring_val = 2 - turn;
    int mark = ring_val+2;
    int count,idx2;
    
    for(int idx2 = 0;idx2<5;idx2++){
        if(xc[idx2]==irow && yc[idx2]==icol){
            xc[idx2] = frow;
            yc[idx2] = fcol;
            break;
        }
    }

    if(board5[irow][icol]==ring_val && board5[frow][fcol]==0){
        int hdiff = frow - irow;
        int vdiff = fcol - icol;
        int start = 0;
        int start2 = icol;
        board5[irow][icol] = 7-mark;
        if(vdiff==0){
            if(hdiff>0){
                for(start = irow;start<=frow;start+=2){
                    if(board5[start][icol]==3 || board5[start][icol]==4){
                        board5[start][icol] = 7 - board5[start][icol];
                    } 
                }
            }else{
                for(start = irow;start>=frow;start-=2){
                    if(board5[start][icol]==3 || board5[start][icol]==4){
                        board5[start][icol] = 7 - board5[start][icol];
                    } 
                }
            }
        }else if(vdiff>0){
            if(hdiff>0){
                for(start = irow;start<=frow;start++){
                    if(board5[start][start2]==3 || board5[start][start2]==4){
                        board5[start][start2] = 7 - board5[start][start2];
                    }
                    start2+=2;
                }
            }else{
                for(start = irow;start>=frow;start--){
                    if(board5[start][start2]==3 || board5[start][start2]==4){
                        board5[start][start2] = 7 - board5[start][start2];
                    }
                    start2+=2;
                }
            }
        }else{
            if(hdiff>0){
                for(start = irow;start<=frow;start++){
                    if(board5[start][start2]==3 || board5[start][start2]==4){
                        board5[start][start2] = 7 - board5[start][start2];
                    }
                    start2-=2;
                }
            }else{
                for(start = irow;start>=frow;start--){
                    if(board5[start][start2]==3 || board5[start][start2]==4){
                        board5[start][start2] = 7 - board5[start][start2];
                    }
                    start2-=2;
                }
            }
        }

        board5[frow][fcol] = ring_val;
    }else{
        // move not allowed here
    }
}

void move_from_string(string s,int turn){
	int len = s.length();
	vector <int> vec1;
	vector <int> vec2;

	if(len==5 || len==6){
        int hex = 0;
        int pos = 0;
        hex = s.at(2) - '0';
        
        if(len==5){
            pos = s.at(4)-'0';
        }else{
            pos = 10*(s.at(4)-'0') + (s.at(5)-'0');
        }
        vec1 = fconvertor(hex,pos);
        if(s.at(0)=='P'){
         	boardPR(vec1[0],vec1[1],turn);
        }else{
            boardRR(vec1[0],vec1[1],turn);
        }
    }else if(len==11 || len==12 || len==13){
        int hex1,pos1,hex2,pos2;

        hex1 = s.at(2)-'0';
        if(len==11){
            hex2 = s.at(8)-'0';
            pos1 = s.at(4)-'0';
            pos2 = s.at(10)-'0';
        }else if(len==13){
            if(s.at(0)=='S'){
                hex2 = s.at(9)-'0';
                pos1 = 10*(s.at(4)-'0') + (s.at(5)-'0');
                pos2 = 10*(s.at(11)-'0') + (s.at(12)-'0');
            }else{
                hex1 = s.at(3)-'0';
                hex2 = s.at(10)-'0';
                pos1 = s.at(5)-'0';
                pos2 = s.at(12)-'0';
            }   
        }else{
            if(s.at(5)==' '){
                hex2 = s.at(8)-'0';
                pos1 = s.at(4)-'0';
                pos2 = 10*(s.at(10)-'0') + (s.at(11)-'0');
            }else{
                hex2 = s.at(9)-'0';
                pos1 = 10*(s.at(4)-'0') + (s.at(5)-'0');
                pos2 = s.at(11)-'0';
            }
        }
        vec1.clear();
        vec2.clear();
        vec1 = fconvertor(hex1,pos1);
        vec2 = fconvertor(hex2,pos2);

        if(s.at(0)=='S'){
        	boardMR(vec1[0],vec1[1],vec2[0],vec2[1],turn);
        }else{
            boardOR(vec1[0],vec2[0],vec1[1],vec2[1],turn);
        }
    }else{
    	int hex1,pos1,hex2,pos2;

    	if(len==15){
    		hex1 = s.at(3)-'0';
    		hex2 = s.at(11)-'0';
    		pos1 = 10*(s.at(5)-'0') + (s.at(6)-'0');
    		pos2 = 10*(s.at(13)-'0') + (s.at(14)-'0');
    	}else{
    		if(s.at(6)==' '){
				hex2 = s.at(10)-'0';
	    		pos1 = s.at(5)-'0';
	    		pos2 = 10*(s.at(12)-'0') + (s.at(13)-'0');
    		}else{
    			hex2 = s.at(11)-'0';
	    		pos1 = 10*(s.at(5)-'0') + (s.at(6)-'0');
	    		pos2 = s.at(13)-'0';
    		}
    	}
    	vec1 = fconvertor(hex1,pos1);
    	vec2 = fconvertor(hex2,pos2);
    	boardOR(vec1[0],vec2[0],vec1[1],vec2[1],turn);
    }
}

void move_from_stringV2(string s,int turn,int board5[21][21],int xc[5],int yc[5]){
	int len = s.length();
	vector <int> vec1;
	vector <int> vec2;

	if(len==5 || len==6){
        int hex = 0;
        int pos = 0;
        hex = s.at(2) - '0';
        
        if(len==5){
            pos = s.at(4)-'0';
        }else{
            pos = 10*(s.at(4)-'0') + (s.at(5)-'0');
        }
        
        vec1 = fconvertor(hex,pos);
        if(s.at(0)=='X'){
        	boardRRV2(vec1[0],vec1[1],turn,board5,xc,yc);
        	vec1.clear();
        }
    }else if(len==11 || len==12 || len==13){
        int hex1,pos1,hex2,pos2;

        hex1 = s.at(2)-'0';
        if(len==11){
            hex2 = s.at(8)-'0';
            pos1 = s.at(4)-'0';
            pos2 = s.at(10)-'0';
        }else if(len==13){
            if(s.at(0)=='S'){
                hex2 = s.at(9)-'0';
                pos1 = 10*(s.at(4)-'0') + (s.at(5)-'0');
                pos2 = 10*(s.at(11)-'0') + (s.at(12)-'0');
            }else{
                hex1 = s.at(3)-'0';
                hex2 = s.at(10)-'0';
                pos1 = s.at(5)-'0';
                pos2 = s.at(12)-'0';
            }   
        }else{
            if(s.at(5)==' '){
                hex2 = s.at(8)-'0';
                pos1 = s.at(4)-'0';
                pos2 = 10*(s.at(10)-'0') + (s.at(11)-'0');
            }else{
                hex2 = s.at(9)-'0';
                pos1 = 10*(s.at(4)-'0') + (s.at(5)-'0');
                pos2 = s.at(11)-'0';
            }
        }
        vec1 = fconvertor(hex1,pos1);
        vec2 = fconvertor(hex2,pos2);

        if(s.at(0)=='S'){
        	boardMRV2(vec1[0],vec1[1],vec2[0],vec2[1],turn,board5,xc,yc);
        }else{
            boardORV2(vec1[0],vec2[0],vec1[1],vec2[1],turn,board5,xc,yc);
        }
        vec1.clear();
        vec2.clear();
    }else{
    	int hex1,pos1,hex2,pos2;

    	if(len==15){
    		hex1 = s.at(3)-'0';
    		hex2 = s.at(11)-'0';
    		pos1 = 10*(s.at(5)-'0') + (s.at(6)-'0');
    		pos2 = 10*(s.at(13)-'0') + (s.at(14)-'0');
    	}else{
    		if(s.at(6)==' '){
				hex2 = s.at(10)-'0';
	    		pos1 = s.at(5)-'0';
	    		pos2 = 10*(s.at(12)-'0') + (s.at(13)-'0');
    		}else{
    			hex2 = s.at(11)-'0';
	    		pos1 = 10*(s.at(5)-'0') + (s.at(6)-'0');
	    		pos2 = s.at(13)-'0';
    		}
    	}
    	vec1 = fconvertor(hex1,pos1);
    	vec2 = fconvertor(hex2,pos2);
    	boardORV2(vec1[0],vec2[0],vec1[1],vec2[1],turn,board5,xc,yc);
    	vec1.clear();
    	vec2.clear();
    }
}

void movromring(string s,int turn){
    if(s.length()==0){
        // do nothing
    }else{
        if(s.at(0)=='S'){
            if(s.at(5)==' '){
                if(s.length()<=13){
                    move_from_string(s,turn);
                }else{
                    if(s.at(11)==' '){
                        // 10 tak ki return krni h string
                        move_from_string(s.substr(0,11),turn);
                        movromring(s.substr(12),turn);
                    }else{
                        // 11 tak ki return krni h
                        move_from_string(s.substr(0,12),turn);
                        movromring(s.substr(13),turn);
                    }
                }
            }else{
                if(s.length()<=13){
                    move_from_string(s,turn);
                }else{
                    if(s.at(12)==' '){
                        // 12 tak ki return krni h string
                        move_from_string(s.substr(0,13),turn);
                        movromring(s.substr(14),turn);
                    }else{
                        // 13 tak ki return krni h
                        move_from_string(s.substr(0,14),turn);
                        movromring(s.substr(15),turn);
                    }
                }
            }
        }else if(s.at(0)=='R'){
            if(s.length()<=15){
                move_from_string(s,turn);
            }else{
                if(s.at(6)==' '){
                    if(s.at(13)==' '){
                        move_from_string(s.substr(0,13),turn);
                        movromring(s.substr(14),turn);
                    }else{
                        move_from_string(s.substr(0,14),turn);
                        movromring(s.substr(15),turn);
                    }
                }else{
                    if(s.at(14)==' '){
                        move_from_string(s.substr(0,14),turn);
                        movromring(s.substr(15),turn);
                    }else{
                        move_from_string(s.substr(0,15),turn);
                        movromring(s.substr(16),turn);
                    }
                }
            }
        }else{
        	if(s.length()<=6){
                move_from_string(s,turn);
            }else{
                if(s.at(5)==' '){
                    move_from_string(s.substr(0,5),turn);
                    movromring(s.substr(6),turn);
                }else{
                    move_from_string(s.substr(0,6),turn);
                    movromring(s.substr(7),turn);
                }
            }
        }
    }
}

void arrversion(string s,int turn,int board5[21][21],int xc[5],int yc[5]){
    if(s.length()==0){
        
    }else{
        if(s.at(0)=='S'){
            if(s.at(5)==' '){
                if(s.length()<=13){
                    move_from_stringV2(s,turn,board5,xc,yc);
                }else{
                    if(s.at(11)==' '){
                        move_from_stringV2(s.substr(0,11),turn,board5,xc,yc);
                        arrversion(s.substr(12),turn,board5,xc,yc);
                    }else{
                        // 11 tak ki return krni h
                        move_from_stringV2(s.substr(0,12),turn,board5,xc,yc);
                        arrversion(s.substr(13),turn,board5,xc,yc);
                    }
                }
            }else{
                if(s.length()<=13){
                    move_from_stringV2(s,turn,board5,xc,yc);
                }else{
                    if(s.at(12)==' '){
                        move_from_stringV2(s.substr(0,13),turn,board5,xc,yc);
                        arrversion(s.substr(14),turn,board5,xc,yc);
                    }else{
                        move_from_stringV2(s.substr(0,14),turn,board5,xc,yc);
                        arrversion(s.substr(15),turn,board5,xc,yc);
                    }
                }
            }
        }else if(s.at(0)=='R'){
            if(s.length()<=15){
                move_from_stringV2(s,turn,board5,xc,yc);
            }else{
                if(s.at(6)==' '){
                    if(s.at(13)==' '){
                        move_from_stringV2(s.substr(0,13),turn,board5,xc,yc);
                        arrversion(s.substr(14),turn,board5,xc,yc);
                    }else{
                        move_from_stringV2(s.substr(0,14),turn,board5,xc,yc);
                        arrversion(s.substr(15),turn,board5,xc,yc);
                    }
                }else{
                    if(s.at(14)==' '){
                        move_from_stringV2(s.substr(0,14),turn,board5,xc,yc);
                        arrversion(s.substr(15),turn,board5,xc,yc);
                    }else{
                        move_from_stringV2(s.substr(0,15),turn,board5,xc,yc);
                        arrversion(s.substr(16),turn,board5,xc,yc);
                    }
                }
            }
        }else{
        	if(s.length()<=6){
                move_from_stringV2(s,turn,board5,xc,yc);
            }else{
                if(s.at(5)==' '){
                    move_from_stringV2(s.substr(0,5),turn,board5,xc,yc);
                    arrversion(s.substr(6),turn,board5,xc,yc);
                }else{
                    move_from_stringV2(s.substr(0,6),turn,board5,xc,yc);
                    arrversion(s.substr(7),turn,board5,xc,yc);
                }
            }
        }
    }
}

void boardD(){
	int n = 21;
	int i = 0;
	int j = 0;

    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            if(arr[i][j]==-1){
                cout << " " << " ";
            }else{
                cout << arr[i][j] << " ";
            }
        }
        cout << "" << endl;
    }
    cout << "************************************" << endl;
}

bool ifalign(int r1,int c1,int r2,int c2){
	if(c1==c2 && r1!=r2){
		return true;
	}
	if(c1==c2 && r1==r2){
		return false;
	}
	int hdiff = abs(c2-c1);
	int vdiff = abs(r2-r1);
	hdiff = hdiff/2;
	if(hdiff==vdiff){
		return true;
	}else{
		return false;
	}
}

vector<int> getSect(int r,int c,int r1,int c1,int r2,int c2){
	vector<int> vec;
	if(ifalign(r,c,r1,c1) && ifalign(r,c,r2,c2)){
		vec.push_back(r);
		vec.push_back(c);
	}else{
		vec.push_back(-1);
		vec.push_back(-1);
	}
	return vec;
}

vector<int> boardPH(int num1,int num2){
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;

	if(num2>=2){
		int i = 1;
		int j = 0;
		int i1,i2,j1,score;
		int max = 0;
		while(j<6*i){
			v2 = fconvertor(i,j);
			for(i1=0;i1<num2;i1++){
				for(i2=i1+1;i2<num2;i2++){
					v1 = getSect(v2[0],v2[1],oringrow[i1],oringcol[i1],oringrow[i2],oringcol[i2]);
					if(v1[0]==-1 && v1[1]==-1){
						continue;
					}else{
						if(arr[v1[0]][v1[1]]!=0){
							continue;
						}else{
							if(arr[v1[0]-2][v1[1]]==1){
								score++;
							}
							if(arr[v1[0]+2][v1[1]]==1){
								score++;
							}
							if(arr[v1[0]-1][v1[1]-2]==1){
								score++;
							}
							if(arr[v1[0]-1][v1[1]+2]==1){
								score++;
							}
							if(arr[v1[0]+1][v1[1]-2]==1){
								score++;
							}
							if(arr[v1[0]+1][v1[1]+2]==1){
								score++;
							}
							if(score>max && arr[v1[0]][v1[1]]==0){
								max = score;
								v3.clear();
								v3.push_back(v1[0]);
								v3.push_back(v1[1]);
							}
							score = 0;
						}
					}
					v1.clear();
				}
			}
			v2.clear();
			j++;
		}

		i = 2;
		j = 0;
		while(j<6*i){
			v2 = fconvertor(i,j);
			for(i1=0;i1<num2;i1++){
				for(i2=i1+1;i2<num2;i2++){
					v1 = getSect(v2[0],v2[1],oringrow[i1],oringcol[i1],oringrow[i2],oringcol[i2]);
					if(v1[0]==-1 && v1[1]==-1){
						continue;
					}else{
						if(arr[v1[0]][v1[1]]!=0){
							continue;
						}else{
							if(arr[v1[0]-2][v1[1]]==1){
								score++;
							}
							if(arr[v1[0]+2][v1[1]]==1){
								score++;
							}
							if(arr[v1[0]-1][v1[1]-2]==1){
								score++;
							}
							if(arr[v1[0]-1][v1[1]+2]==1){
								score++;
							}
							if(arr[v1[0]+1][v1[1]-2]==1){
								score++;
							}
							if(arr[v1[0]+1][v1[1]+2]==1){
								score++;
							}
							if(score>max && arr[v1[0]][v1[1]]==0){
								max = score;
								v3.clear();
								v3.push_back(v1[0]);
								v3.push_back(v1[1]);
							}
							score = 0;
						}
					}
					v1.clear();
				}
			}
			v2.clear();
			j++;
		}

		i = 1;
		j = 0;
		while(j<6*i){
			v1 = fconvertor(i,j);
			for(i1=num2-1;i1>=0;i1--){
				if(ifalign(v2[0],v2[1],oringrow[i1],oringcol[i1])){
					if(arr[v1[0]-2][v1[1]]==1){
						score++;
					}
					if(arr[v1[0]+2][v1[1]]==1){
						score++;
					}
					if(arr[v1[0]-1][v1[1]-2]==1){
						score++;
					}
					if(arr[v1[0]-1][v1[1]+2]==1){
						score++;
					}
					if(arr[v1[0]+1][v1[1]-2]==1){
						score++;
					}
					if(arr[v1[0]+1][v1[1]+2]==1){
						score++;
					}
					if(score>max && arr[v1[0]][v1[1]]==0){
						max = score;
						v3.clear();
						v3.push_back(v1[0]);
						v3.push_back(v1[1]);
					}
					score = 0;
				}
				v1.clear();
			}
			j++;
		}

		v1.clear();
		i = 2;
		j = 0;
		while(j<6*i){
			v1 = fconvertor(i,j);
			for(i1=num2-1;i1>=0;i1--){
				if(ifalign(v2[0],v2[1],oringrow[i1],oringcol[i1])){
					if(arr[v1[0]-2][v1[1]]==1){
						score++;
					}
					if(arr[v1[0]+2][v1[1]]==1){
						score++;
					}
					if(arr[v1[0]-1][v1[1]-2]==1){
						score++;
					}
					if(arr[v1[0]-1][v1[1]+2]==1){
						score++;
					}
					if(arr[v1[0]+1][v1[1]-2]==1){
						score++;
					}
					if(arr[v1[0]+1][v1[1]+2]==1){
						score++;
					}
					if(score>max && arr[v1[0]][v1[1]]==0){
						max = score;
						v3.clear();
						v3.push_back(v1[0]);
						v3.push_back(v1[1]);
					}
					score = 0;
				}
				v1.clear();
			}
			j++;
		}
	}else{
		int i = 1;
		int j = 0;
		for(j = 0;j<6;j++){
			v1.clear();
			v1 = fconvertor(i,j);
			if(ifalign(v1[0],v1[1],oringrow[num2-1],oringcol[num2-1])){
				if(arr[v1[0]][v1[1]]==0){
					v3.clear();
					v3.push_back(v1[0]);
					v3.push_back(v1[1]);
					break;
				}
			}
		}
	}
	if(v3.size()==0){
        int hexa = 1;
        int posi = 0;
        for(posi = 0;posi<6;posi++){
            v3 = fconvertor(hexa,posi);
            if(arr[v3[0]][v3[1]]==0){
                break;
            }else{
                v3.clear();
            }
        }

        if(v3.size()==0){
            hexa = 2;
            posi = 0;
            for(posi = 0;posi<12;posi++){
                v3 = fconvertor(hexa,posi);
                if(arr[v3[0]][v3[1]]==0){
                    break;
                }else{
                    v3.clear();
                }
            }
        }
	}
	v1.clear();
    v1 = rev_convertor(v3[0],v3[1]);
	v3.clear();
	v3.push_back(v1[0]);
	v3.push_back(v1[1]);
	return v3;
}

string placement(int counter){
	string s = "P ";
	vector <int> vec;
	int n = 5;
	int idx = -1;
	int i;
	int mine = 0;
	int other = 0;

	for(i = 0;i<n;i++){
		if(mringrow[i]!=-1){
			mine++;
		}else{
            break;
        }
	}

	for(i = 0;i<n;i++){
		if(oringrow[i]!=-1){
			other++;
		}else{
            break;
        }
	}

	if(arr[2*n][2*n]==0){
		s+=to_string(0)+" "+to_string(0);
		return s;
	}else if(counter==0){
		s+=to_string(1)+" "+to_string(0);
		return s;
	}else{
		vec = boardPH(mine,other);
		s+=to_string(vec[0])+" "+to_string(vec[1]);
		return s;
	}
	return s;
}

string boardRRH(int board[21][21],int xc[5],int yc[5],int turn){
	int o2 = 1+turn;
	int mark1 = 4-turn;
	int mark2 = 3+turn;
	int val = 1000;
	int m1,m2,r2,rv,i,rdx,i1,i2;

	for(i = 0;i<5;i++){
		m1 = 0;
		m2 = 0;
		r2 = 0;
		if(xc[i]!=-1 && yc[i]!=-1){
			i1 = xc[i];
			i2 = yc[i];
			while(i1>=0 && arr[i1][i2]!=-1){
				if(arr[i1][i2]==o2){
					r2++;
				}else if(arr[i1][i2]==mark1){
					m1++;
				}else if(arr[i1][i2]==mark2){
					m2++;
				}else{

				}
				i1-=2;
			}

			i1 = xc[i];
			i2 = yc[i];
			while(i1<=20 && arr[i1][i2]!=-1){
				if(arr[i1][i2]==o2){
					r2++;
				}else if(arr[i1][i2]==mark1){
					m1++;
				}else if(arr[i1][i2]==mark2){
					m2++;
				}else{

				}
				i1+=2;
			}

			i1 = xc[i];
			i2 = yc[i];
			while(i1<=20 && i2<=20 && arr[i1][i2]!=-1){
				if(arr[i1][i2]==o2){
					r2++;
				}else if(arr[i1][i2]==mark1){
					m1++;
				}else if(arr[i1][i2]==mark2){
					m2++;
				}else{

				}
				i1+=1;
				i2+=2;
			}

			i1 = xc[i];
			i2 = yc[i];
			while(i1>=0 && i2<=20 && arr[i1][i2]!=-1){
				if(arr[i1][i2]==o2){
					r2++;
				}else if(arr[i1][i2]==mark1){
					m1++;
				}else if(arr[i1][i2]==mark2){
					m2++;
				}else{

				}
				i1-=1;
				i2+=2;
			}

			i1 = xc[i];
			i2 = yc[i];
			while(i1>=0 && i2>=0 && arr[i1][i2]!=-1){
				if(arr[i1][i2]==o2){
					r2++;
				}else if(arr[i1][i2]==mark1){
					m1++;
				}else if(arr[i1][i2]==mark2){
					m2++;
				}else{

				}
				i1-=1;
				i2-=2;
			}

			i1 = xc[i];
			i2 = yc[i];
			while(i1<=20 && i2>=0 && arr[i1][i2]!=-1){
				if(arr[i1][i2]==o2){
					r2++;
				}else if(arr[i1][i2]==mark1){
					m1++;
				}else if(arr[i1][i2]==mark2){
					m2++;
				}else{
					
				}
				i1+=1;
				i2-=2;
			}
		}else{

		}
		rv = (m1+2*m2+r2);
		if(rv<val && xc[i]!=-1 && yc[i]!= 1){
			val = rv;
			rdx = i;
		}
	}
    vector<int> v = rev_convertor(xc[rdx],yc[rdx]);
	string s = "";
	s = "X " + to_string(v[0]) + " " + to_string(v[1]);
	return s;
}

void stringboard(string s,int board5[21][21],int turn){
	int ring = 2 - turn;
	int xc[5];
	int yc[5];
	int i,j,idx;
    idx = 0;
	for(i = 0;i<21;i++){
		for(j = 0;j<21;j++){
			if(board5[i][j]==ring){
				xc[idx] = i;
				yc[idx] = j;
				idx++;
			}
		}
	}

    if(idx<5){
        while(idx<=4){
            xc[idx] = -1;
            yc[idx] = -1;
            idx++;
        }
    }
	arrversion(s,turn,board5,xc,yc);
}

string rowformed(int board6[21][21],int turn){
	string s = "";
	int mark = 4 - turn;
	int counter = 1;
	int i1,i2,i3,j1,j2,j3,i,j,count;
	vector<int> v1;

	i1 = 13;
	j1 = 0;
    v1.clear();

	while(counter<=9){
        if(counter<=4){
            i1 = 15 - 2*counter;
            j1 = 0;
            i3 = 16 - counter;
            j3 = 2*(counter+1);
        }else if(counter==5){
            i1 = 6;
            j1 = 2;
            i3 = 10;
            j3 = 10;
        }else{
            i1 = 10 - counter;
            j1 = 2*counter - 10;
            i3 = 21 - 2*counter;
            j3 = 12;
        }
        
        i = i1;
        j = j1;
        while(i<=i3 && j<=j3){
            if(board6[i][j]==mark && board6[i+1][j+2]==mark && board6[i+2][j+4]==mark && board6[i+3][j+6]==mark && board6[i+4][j+8]==mark){
                v1 = rev_convertor(i,j);
                i2 = v1[0];
                j2 = v1[1];
                v1.clear();
                v1 = rev_convertor(i+4,j+8);
                s = "";
                s = "RS " + to_string(i2) + " " + to_string(j2) + " RE " + to_string(v1[0]) + " " + to_string(v1[1]);
                v1.clear();
                return s;
            }
            i+=1;
            j+=2;
        }
        counter++;
    }

    // loop 2 left top corner is starting point ///
    counter = 1;
    i1 = 7;
    j1 = 0;
    while(counter<=9){
        if(counter<=4){
            i1 = 5 + 2*counter;
            j1 = 0;
            i3 = 4 + counter;
            j3 = 2*(counter+1);
        }else if(counter==5){
            i1 = 14;
            j1 = 2;
            i3 = 10;
            j3 = 10;
        }else{
            i1 = 10 + counter;
            j1 = 2*counter - 10;
            i3 = 2*counter - 1;
            j3 = 12;
        }
        
        i = i1;
        j = j1;
        while(i>=i3 && j<=j3){
            if(board6[i][j]==mark && board6[i-1][j+2]==mark && board6[i-2][j+4]==mark && board6[i-3][j+6]==mark && board6[i-4][j+8]==mark){
                v1 = rev_convertor(i,j);
                i2 = v1[0];
                j2 = v1[1];
                v1.clear();
                v1 = rev_convertor(i-4,j+8);
                s = "";
                s = "RS " + to_string(i2) + " " + to_string(j2) + " RE " + to_string(v1[0]) + " " + to_string(v1[1]);
                v1.clear();
                return s;
            }
            i-=1;
            j+=2;
        }
        counter++;
    }

    // loop 3 left top corner is starting point |||
    counter = 1;
    i1 = 4;
    j1 = 2;
    while(counter<=9){
        if(counter<=4){
            i1 = 5-counter;
            j1 = 2*counter;
            i3 = counter+7;
            j3 = 2*counter;
        }else if(counter==5){
            i1 = 2;
            j1 = 10;
            i3 = 10;
            j3 = 10;
        }else{
            i1 = counter-5;
            j1 = 2*counter;
            i3 = 17 - counter;
            j3 = 2*counter;
        }
        
        i = i1;
        j = j1;
        while(i<=i3){
            if(board6[i][j]==mark && board6[i+2][j]==mark && board6[i+4][j]==mark && board6[i+6][j]==mark && board6[i+8][j]==mark){
                v1 = rev_convertor(i,j);
                i2 = v1[0];
                j2 = v1[1];
                v1.clear();
                v1 = rev_convertor(i+8,j);
                s = "";
                s = "RS " + to_string(i2) + " " + to_string(j2) + " RE " + to_string(v1[0]) + " " + to_string(v1[1]);
                v1.clear();
                return s;
            }
            i+=2;
        }
        counter++;
    }
	return s;
}

string rowformedlevel3(int board[21][21],int turn,int xc[5],int yc[5]){
	string s = "";
	string str = "";
	int num;

	for(num = 0;num<3;num++){
		s = rowformed(board,turn);
		if(s.length()!=0){
			if(str.length()==0){
				str+=s;
				str+=" ";
			}else{
				str+=" ";
				str+=s;
				str+=" ";
			}
			arrversion(s,turn,board,xc,yc);
			s = "";
			s = boardRRH(board,xc,yc,turn);
			str+=s;
			arrversion(s,turn,board,xc,yc);
			s = "";
		}else{
			break;
		}
	}
	return str;
}

vector<string> getFriends(int board[21][21],int turn){
	vector<string> vec;
	int ring = 2 - turn;
	int xc[5];
	int yc[5];
	int x[5];
	int y[5];
	int secBoard[21][21];
	int i,j,idx,num,row,col,i1,i2;
	idx = 0;
	string str = "";
	string s = "";
	vector<int> v;
	
	for(i = 0;i<21;i++){
		for(j = 0;j<21;j++){
			if(board[i][j]==ring){
				xc[idx] = i;
				yc[idx] = j;
				idx++;
			}
		}
	}

    if(idx<5){
        while(idx<5){
            xc[idx] = -1;
            yc[idx] = -1;
            idx++;
        }
    }

	str = rowformedlevel3(board,turn,xc,yc);
	for(num = 0;num<5;num++){
		string s = "S ";
		string s2 = "";
		string s3 = "";
		row = xc[num];
		col = yc[num];
		if(row!=-1 && col!=-1){
            v = rev_convertor(row,col);
			s+= (to_string(v[0]) + " " + to_string(v[1])) + " M ";
			s2+=s;
			v.clear();
            
            row = xc[num];
			row-=2;
			while(row>=1 && arr[row][col]==0){
				cpV1(secBoard,board);
				cpV2(x,y,xc,yc);
                v = rev_convertor(row,col);
				s+=(to_string(v[0])+" "+to_string(v[1]));
				v.clear();
				arrversion(s,turn,board,xc,yc);
				s3 = rowformedlevel3(board,turn,xc,yc);
				if(s3.length()!=0){
					s+=" ";
					s+=s3;
				}
				vec.push_back(s);
				s = "";
				s3 = "";
				s+=s2;
				cpV1(board,secBoard);
				cpV2(xc,yc,x,y);
				row-=2;
			}
			if(row<1){

			}else{
				if(board[row][col]==3 || board[row][col]==4){
                   while(row>=0 && (board[row][col]==3 || board[row][col]==4)){
                        row-=2;
                   }

                   if(row<1){

                   }else if(board[row][col]==0){
                        cpV1(secBoard,board);
                        cpV2(x,y,xc,yc);
                        v = rev_convertor(row,col);
                        s+=(to_string(v[0])+" "+to_string(v[1]));
                        v.clear();
                        arrversion(s,turn,board,xc,yc);
                        s3 = rowformedlevel3(board,turn,xc,yc);
                        if(s3.length()!=0){
                            s+=" ";
                            s+=s3;
                        }
                        vec.push_back(s);
                        s = "";
                        s3 = "";
                        s+=s2;
                        cpV1(board,secBoard);
                        cpV2(xc,yc,x,y);
                   }else{

                   }
				}else{

				}
			}

            row = xc[num];
            col = yc[num];
            v.clear();
            row+=2;
            s = "";
            s+=s2;
            s3 = "";
            while(row<=19 && arr[row][col]==0){
                cpV1(secBoard,board);
                cpV2(x,y,xc,yc);
                v = rev_convertor(row,col);
                s+=(to_string(v[0])+" "+to_string(v[1]));
                v.clear();
                arrversion(s,turn,board,xc,yc);
                s3 = rowformedlevel3(board,turn,xc,yc);
                if(s3.length()!=0){
                    s+=" ";
                    s+=s3;
                }
                vec.push_back(s);
                s = "";
                s3 = "";
                s+=s2;
                cpV1(board,secBoard);
                cpV2(xc,yc,x,y);
                row+=2;
            }
            if(row>=20){

            }else{
                if(board[row][col]==3 || board[row][col]==4){
                   while(row<=19 && (board[row][col]==3 || board[row][col]==4)){
                        row+=2;
                   }

                   if(row>=20){

                   }else if(board[row][col]==0){
                        cpV1(secBoard,board);
                        cpV2(x,y,xc,yc);
                        v = rev_convertor(row,col);
                        s+=(to_string(v[0])+" "+to_string(v[1]));
                        v.clear();
                        arrversion(s,turn,board,xc,yc);
                        s3 = rowformedlevel3(board,turn,xc,yc);
                        if(s3.length()!=0){
                            s+=" ";
                            s+=s3;
                        }
                        vec.push_back(s);
                        s = "";
                        s3 = "";
                        s+=s2;
                        cpV1(board,secBoard);
                        cpV2(xc,yc,x,y);
                   }else{

                   }
                }else{

                }
            }

            // row = xc[num];
            // col = yc[num];
            // v.clear();
            // s = "";
            // s+=s2;
            // s3 = "";
            // row+=1;
            // col+=2;
            // while(row<=19 && col<=19 && arr[row][col]==0){
            //     cpV1(secBoard,board);
            //     cpV2(x,y,xc,yc);
            //     v = rev_convertor(row,col);
            //     s+=(to_string(v[0])+" "+to_string(v[1]));
            //     v.clear();
            //     arrversion(s,turn,board,xc,yc);
            //     s3 = rowformedlevel3(board,turn,xc,yc);
            //     if(s3.length()!=0){
            //         s+=" ";
            //         s+=s3;
            //     }
            //     vec.push_back(s);
            //     s = "";
            //     s3 = "";
            //     s+=s2;
            //     cpV1(board,secBoard);
            //     cpV2(xc,yc,x,y);
            //     row+=1;
            //     col+=2;
            // }
            // if(row>19 || col>19){

            // }else{
            //     if(board[row][col]==3 || board[row][col]==4){
            //        while(row<20 && col<20 && (board[row][col]==3 || board[row][col]==4)){
            //             row+=1;
            //             col+=2;
            //        }

            //        if(row>19 || col>19){

            //        }else if(board[row][col]==0){
            //             cpV1(secBoard,board);
            //             cpV2(x,y,xc,yc);
            //             v = rev_convertor(row,col);
            //             s+=(to_string(v[0])+" "+to_string(v[1]));
            //             v.clear();
            //             arrversion(s,turn,board,xc,yc);
            //             s3 = rowformedlevel3(board,turn,xc,yc);
            //             if(s3.length()!=0){
            //                 s+=" ";
            //                 s+=s3;
            //             }
            //             vec.push_back(s);
            //             s = "";
            //             s3 = "";
            //             s+=s2;
            //             cpV1(board,secBoard);
            //             cpV2(xc,yc,x,y);
            //        }else{

            //        }
            //     }else{

            //     }
            // }

            // row = xc[num];
            // col = yc[num];
            // v.clear();
            // s = "";
            // s+=s2;
            // s3 = "";
            // row-=1;
            // col+=2;
            // while(row>=1 && col<=19 && arr[row][col]==0){
            //     cpV1(secBoard,board);
            //     cpV2(x,y,xc,yc);
            //     v = rev_convertor(row,col);
            //     s+=(to_string(v[0])+" "+to_string(v[1]));
            //     v.clear();
            //     arrversion(s,turn,board,xc,yc);
            //     s3 = rowformedlevel3(board,turn,xc,yc);
            //     if(s3.length()!=0){
            //         s+=" ";
            //         s+=s3;
            //     }
            //     vec.push_back(s);
            //     s = "";
            //     s3 = "";
            //     s+=s2;
            //     cpV1(board,secBoard);
            //     cpV2(xc,yc,x,y);
            //     row-=1;
            //     col+=2;
            // }
            // if(row<1 || col>19){

            // }else{
            //     if(board[row][col]==3 || board[row][col]==4){
            //        while(row>0 && col<20 && (board[row][col]==3 || board[row][col]==4)){
            //             row-=1;
            //             col+=2;
            //        }

            //        if(row>19 || col>19){

            //        }else if(board[row][col]==0){
            //             cpV1(secBoard,board);
            //             cpV2(x,y,xc,yc);
            //             v = rev_convertor(row,col);
            //             s+=(to_string(v[0])+" "+to_string(v[1]));
            //             v.clear();
            //             arrversion(s,turn,board,xc,yc);
            //             s3 = rowformedlevel3(board,turn,xc,yc);
            //             if(s3.length()!=0){
            //                 s+=" ";
            //                 s+=s3;
            //             }
            //             vec.push_back(s);
            //             s = "";
            //             s3 = "";
            //             s+=s2;
            //             cpV1(board,secBoard);
            //             cpV2(xc,yc,x,y);
            //        }else{

            //        }
            //     }else{

            //     }
            // }

            // row = xc[num];
            // col = yc[num];
            // v.clear();
            // s = "";
            // s+=s2;
            // s3 = "";
            // row-=1;
            // col-=2;
            // while(row>=1 && col>=0 && arr[row][col]==0){
            //     cpV1(secBoard,board);
            //     cpV2(x,y,xc,yc);
            //     v = rev_convertor(row,col);
            //     s+=(to_string(v[0])+" "+to_string(v[1]));
            //     v.clear();
            //     arrversion(s,turn,board,xc,yc);
            //     s3 = rowformedlevel3(board,turn,xc,yc);
            //     if(s3.length()!=0){
            //         s+=" ";
            //         s+=s3;
            //     }
            //     vec.push_back(s);
            //     s = "";
            //     s3 = "";
            //     s+=s2;
            //     cpV1(board,secBoard);
            //     cpV2(xc,yc,x,y);
            //     row-=1;
            //     col-=2;
            // }
            // if(row<1 || col<0){

            // }else{
            //     if(board[row][col]==3 || board[row][col]==4){
            //        while(row>0 && col<20 && (board[row][col]==3 || board[row][col]==4)){
            //             row-=1;
            //             col-=2;
            //        }

            //        if(row>19 || col<0){

            //        }else if(board[row][col]==0){
            //             cpV1(secBoard,board);
            //             cpV2(x,y,xc,yc);
            //             v = rev_convertor(row,col);
            //             s+=(to_string(v[0])+" "+to_string(v[1]));
            //             v.clear();
            //             arrversion(s,turn,board,xc,yc);
            //             s3 = rowformedlevel3(board,turn,xc,yc);
            //             if(s3.length()!=0){
            //                 s+=" ";
            //                 s+=s3;
            //             }
            //             vec.push_back(s);
            //             s = "";
            //             s3 = "";
            //             s+=s2;
            //             cpV1(board,secBoard);
            //             cpV2(xc,yc,x,y);
            //        }else{

            //        }
            //     }else{

            //     }
            // }
            
            // row = xc[num];
            // col = yc[num];
            // v.clear();
            // s = "";
            // s+=s2;
            // s3 = "";
            // row+=1;
            // col-=2;
            // while(row<=19 && col>=0 && arr[row][col]==0){
            //     cpV1(secBoard,board);
            //     cpV2(x,y,xc,yc);
            //     v = rev_convertor(row,col);
            //     s+=(to_string(v[0])+" "+to_string(v[1]));
            //     v.clear();
            //     arrversion(s,turn,board,xc,yc);
            //     s3 = rowformedlevel3(board,turn,xc,yc);
            //     if(s3.length()!=0){
            //         s+=" ";
            //         s+=s3;
            //     }
            //     vec.push_back(s);
            //     s = "";
            //     s3 = "";
            //     s+=s2;
            //     cpV1(board,secBoard);
            //     cpV2(xc,yc,x,y);
            //     row+=1;
            //     col-=2;
            // }
            // if(row>19 || col<0){

            // }else{
            //     if(board[row][col]==3 || board[row][col]==4){
            //        while(row<20 && col<20 && (board[row][col]==3 || board[row][col]==4)){
            //             row+=1;
            //             col-=2;
            //        }

            //        if(row>19 || col<0){

            //        }else if(board[row][col]==0){
            //             cpV1(secBoard,board);
            //             cpV2(x,y,xc,yc);
            //             v = rev_convertor(row,col);
            //             s+=(to_string(v[0])+" "+to_string(v[1]));
            //             v.clear();
            //             arrversion(s,turn,board,xc,yc);
            //             s3 = rowformedlevel3(board,turn,xc,yc);
            //             if(s3.length()!=0){
            //                 s+=" ";
            //                 s+=s3;
            //             }
            //             vec.push_back(s);
            //             s = "";
            //             s3 = "";
            //             s+=s2;
            //             cpV1(board,secBoard);
            //             cpV2(xc,yc,x,y);
            //        }else{

            //        }
            //     }else{

            //     }
            // }            
		}
	}
	return vec;
}

int getnumFriends(int board[21][21],int x,int y){
        int rv = 0;
        int row,col;
        row = x;
        col = y;

            row-=2;
            while(row>=1 && arr[row][col]==0){
                rv+=1;
                row-=2;
            }
            if(row<1){

            }else{
                if(board[row][col]==3 || board[row][col]==4){
                   while(row>=0 && (board[row][col]==3 || board[row][col]==4)){
                        row-=2;
                   }

                   if(row<1){

                   }else if(board[row][col]==0){
                        rv+=1;
                   }else{

                   }
                }else{

                }
            }

            row = x;
            col = y;
            row+=2;
            while(row<=19 && arr[row][col]==0){
                rv+=1;
                row+=2;
            }
            if(row>=20){

            }else{
                if(board[row][col]==3 || board[row][col]==4){
                   while(row<=19 && (board[row][col]==3 || board[row][col]==4)){
                        row+=2;
                   }

                   if(row>=20){

                   }else if(board[row][col]==0){
                        rv+=1;
                   }else{

                   }
                }else{

                }
            }

            row = x;
            col = y;
            row+=1;
            col+=2;
            while(row<=19 && col<=19 && arr[row][col]==0){
                rv+=1;
                row+=1;
                col+=2;
            }
            if(row>19 || col>19){

            }else{
                if(board[row][col]==3 || board[row][col]==4){
                   while(row<20 && col<20 && (board[row][col]==3 || board[row][col]==4)){
                        row+=1;
                        col+=2;
                   }

                   if(row>19 || col>19){

                   }else if(board[row][col]==0){
                        rv+=1;
                   }else{

                   }
                }else{

                }
            }

            row = x;
            col = y;
            row-=1;
            col+=2;
            while(row>=1 && col<=19 && arr[row][col]==0){
                rv+=1;
                row-=1;
                col+=2;
            }
            if(row<1 || col>19){

            }else{
                if(board[row][col]==3 || board[row][col]==4){
                   while(row>0 && col<20 && (board[row][col]==3 || board[row][col]==4)){
                        row-=1;
                        col+=2;
                   }

                   if(row>19 || col>19){

                   }else if(board[row][col]==0){
                        rv+=1;
                   }else{

                   }
                }else{

                }
            }

            row = x;
            col = y;
            row-=1;
            col-=2;
            while(row>=1 && col>=0 && arr[row][col]==0){
                rv+=1;
                row-=1;
                col-=2;
            }
            if(row<1 || col<0){

            }else{
                if(board[row][col]==3 || board[row][col]==4){
                   while(row>0 && col<20 && (board[row][col]==3 || board[row][col]==4)){
                        row-=1;
                        col-=2;
                   }

                   if(row>19 || col<0){

                   }else if(board[row][col]==0){
                        rv+=1;
                   }else{

                   }
                }else{

                }
            }
            
            row = x;
            col = y;
            row+=1;
            col-=2;
            while(row<=19 && col>=0 && arr[row][col]==0){
                rv+=1;
                row+=1;
                col-=2;
            }
            if(row>19 || col<0){

            }else{
                if(board[row][col]==3 || board[row][col]==4){
                   while(row<20 && col<20 && (board[row][col]==3 || board[row][col]==4)){
                        row+=1;
                        col-=2;
                   }

                   if(row>19 || col<0){

                   }else if(board[row][col]==0){
                        rv+=1;
                   }else{

                   }
                }else{

                }
            }
        return rv;
}

int ring_potential(int board[21][21],int x,int y){
    int rv = 0;
    // int turn = 2 - board[x][y];
    rv = getnumFriends(board,x,y);
    return rv;
}

int row_potential(int board[21][21],int x,int y,int move,int level){
    // move = 0 means vertical | 1 means +45degrees / and -1 means -45degree \ kinda movement
    // level = 3 or 4 i.e. number of contiguous markers
    // for number of rows introduced constt
    int rv = 0;
    if(level==4){
        rv = 12;
    }else{
        rv = 7;
    }
    // int turn = 4 - board[x][y];
    // if(level==3){
    //     if(move==0){

    //     }else if(move==1){

    //     }else{

    //     }
    // }else{
    //     if(move==0){

    //     }else if(move==1){

    //     }else{
            
    //     }
    // }
    return rv;
}

int rowof3(int board[21][21],int turn){
    int rv = 0;
    int mark = 4 - turn;
    int counter = 1;
    int i1,j1,i3,j3,i,j;

    i1 = 13;
    j1 = 0;
    while(counter<=9){
        if(counter<=4){
            i1 = 15 - 2*counter;
            j1 = 0;
            i3 = i1+(counter+2);
            j3 = 2*(counter+2);
        }else if(counter==5){
            i1 = 6;
            j1 = 2;
            i3 = 11;
            j3 = 12;
        }else{
            i1 = 10 - counter;
            j1 = 2*counter - 10;
            i3 = i1 + 12 - counter;
            j3 = 14;
        }
        i = i1;
        j = j1;
        while(i<=i3 && j<=j3){
            if(board[i][j]==mark && board[i+1][j+2]==mark && board[i+2][j+4]==mark && board[i+3][j+6]!=mark){
                rv+=(row_potential(board,i,j,-1,3));
            }
            i+=1;
            j+=2;
        }
        counter++;
    }

    // loop 2 left top corner is starting point ///
    counter = 1;
    i1 = 7;
    j1 = 0;
    while(counter<=9){
        if(counter<=4){
            i1 = 5 + 2*counter;
            j1 = 0;
            i3 = i1-(counter+2);
            j3 = 2*(counter+2);
        }else if(counter==5){
            i1 = 14;
            j1 = 2;
            i3 = 9;
            j3 = 12;
        }else{
            i1 = 10 + counter;
            j1 = 2*counter - 10;
            i3 = i1 - 12 + counter;
            j3 = 14;
        }
        i = i1;
        j = j1;
        while(i>=i3 && j<=j3){
            if(board[i][j]==mark && board[i-1][j+2]==mark && board[i-2][j+4]==mark && board[i-3][j+6]!=mark){
                rv+=(row_potential(board,i,j,1,3));
            }
            i-=1;
            j+=2;
        }
        counter++;
    }

    // loop 3 left top corner is starting point |||
    counter = 1;
    i1 = 4;
    j1 = 2;
    while(counter<=9){
        if(counter<=4){
            i1 = 5-counter;
            j1 = 2*counter;
            i3 = i1+2*(counter+2);
            j3 = 2*counter;
        }else if(counter==5){
            i1 = 2;
            j1 = 10;
            i3 = 12;
            j3 = 10;
        }else{
            i1 = counter-5;
            j1 = 2*counter;
            i3 = i1 + 2*(12-counter);
            j3 = 2*counter;
        }
        i = i1;
        j = j1;
        while(i<=i3){
            if(board[i][j]==mark && board[i+2][j]==mark && board[i+4][j]==mark && board[i+6][j]!=mark){
                rv+=(row_potential(board,i,j,0,3));
            }
            i+=2;
        }
        counter++;
    }
    return rv;
}

int rowof4(int board[21][21],int turn){
    int rv = 0;
    int mark = 4 - turn;
    int counter = 1;
    int i1,i3,j1,j3,i,j,count;

    i1 = 13;
    j1 = 0;
    while(counter<=9){
        if(counter<=4){
            i1 = 15 - 2*counter;
            j1 = 0;
            i3 = i1+(counter+1);
            j3 = 2*(counter+1);
        }else if(counter==5){
            i1 = 6;
            j1 = 2;
            i3 = 10;
            j3 = 10;
        }else{
            i1 = 10 - counter;
            j1 = 2*counter - 10;
            i3 = i1 + 11 - counter;
            j3 = 12;
        }
        
        i = i1;
        j = j1;
        while(i<=i3 && j<=j3){
            if(board[i][j]==mark && board[i+1][j+2]==mark && board[i+2][j+4]==mark && board[i+3][j+6]==mark && board[i+4][j+8]!=mark){
                rv+=(row_potential(board,i,j,-1,4));
            }
            i+=1;
            j+=2;
        }
        counter++;
    }

    // loop 2 left top corner is starting point ///
    counter = 1;
    i1 = 7;
    j1 = 0;
    while(counter<=9){
        if(counter<=4){
            i1 = 5 + 2*counter;
            j1 = 0;
            i3 = i1-(counter+1);
            j3 = 2*(counter+1);
        }else if(counter==5){
            i1 = 14;
            j1 = 2;
            i3 = 10;
            j3 = 10;
        }else{
            i1 = 10 + counter;
            j1 = 2*counter - 10;
            i3 = i1 - 11 + counter;
            j3 = 12;
        }
        
        i = i1;
        j = j1;
        while(i>=i3 && j<=j3){
            if(board[i][j]==mark && board[i-1][j+2]==mark && board[i-2][j+4]==mark && board[i-3][j+6]==mark && board[i-4][j+8]!=mark){
                rv+=(row_potential(board,i,j,1,4));
            }
            i-=1;
            j+=2;
        }
        counter++;
    }

    // loop 3 left top corner is starting point |||
    counter = 1;
    i1 = 4;
    j1 = 2;
    while(counter<=9){
        if(counter<=4){
            i1 = 5-counter;
            j1 = 2*counter;
            i3 = i1+2*(counter+1);
            j3 = 2*counter;
        }else if(counter==5){
            i1 = 2;
            j1 = 10;
            i3 = 10;
            j3 = 10;
        }else{
            i1 = counter-5;
            j1 = 2*counter;
            i3 = i1 + 2*(11-counter);
            j3 = 2*counter;
        }
        
        i = i1;
        j = j1;
        while(i<=i3){
            if(board[i][j]==mark && board[i+2][j]==mark && board[i+4][j]==mark && board[i+6][j]==mark && board[i+8][j]!=mark){
                rv+=(row_potential(board,i,j,0,4));
            }
            i+=2;
        }
        counter++;
    }
    return rv;
}

int state_eval(int board2[21][21],int turn){
    int rv = 0;
    int r1 = 2-turn;
    int r2 = 1+turn;
    int mxc[5];
    int myc[5];
    int oxc[5];
    int oyc[5];
    int idx1 = 0;
    int idx2 = 0;
    int i1,i2,numrings,myvalue,oppvalue,ridx;

    for(i1 = 0;i1<21;i1++){
        for(i2 = 0;i2<21;i2++){
            if(board2[i1][i2]==r1){
                mxc[idx1] = i1;
                myc[idx1] = i2;
                idx1++;
            }
            if(board2[i1][i2]==r2){
                oxc[idx2] = i1;
                oyc[idx2] = i2;
                idx2++;
            }
        }
    }
    if(idx1<5){
        while(idx1<5){
            mxc[idx1] = -1;
            myc[idx1] = -1;
            idx1++;
        }
    }
    if(idx2<5){
        while(idx2<5){
            oxc[idx2] = -1;
            oyc[idx2] = -1;
            idx2++;
        }
    }

    string rowf = rowformedlevel3(board2,turn,mxc,myc);
    if(rowf.length()!=0){
        numrings = 0;
        for(int k = 0;k<5;k++){
            if(mxc[k]==-1 && myc[k]==-1){
                numrings++;
            }
        }
        // add some
        myvalue+=0;
    }

    myvalue = 0;
    for(ridx = 0;ridx<5;ridx++){
        if(mxc[ridx]!=-1 && myc[ridx]!=-1){
            myvalue+=ring_potential(board2,mxc[ridx],myc[ridx]);
        }
    }
    myvalue+=rowof3(board2,turn);
    myvalue+=rowof4(board2,turn);
    rv = myvalue;
    rowf = rowformedlevel3(board2,1-turn,oxc,oyc);
    if(rowf.length()!=0){
        numrings = 0;
        for(int k = 0;k<5;k++){
            if(oxc[k]==-1 && oyc[k]==-1){
                numrings++;
            }
        }
        // subtract some
        rv-=0;
    }
    // int oppvalue = 0;
    // for(ridx = 0;ridx<5;ridx++){
    //     if(oxc[ridx]!=-1 && oyc[ridx]!=-1){
    //         oppvalue+=ring_potential(board,oxc[ridx],oyc[ridx]);
    //     }
    // }
    // oppvalue+=rowof3(board,1-turn);
    // oppvalue+=rowof4(board,1-turn);
    return rv;
}

int maxvalue(int board2[21][21],int alpha,int beta,int depth,bool mom,vector<string> moves){
	if(depth==0){
		if(mom){
			return state_eval(board2,1);
		}else{
			return state_eval(board2,0);
		}
	}

	int i,i1,i2,val;
	string s;
	vector<string> vec;
	int board3[21][21];

	for(i1 = 0;i1<21;i1++){
		for(i2 = 0;i2<21;i2++){
			board3[i1][i2] = board2[i1][i2];
		}
	}

	if(mom){
		val = -2000;
	}else{
		val = 2000;
	}
	
	for(i = 0;i<moves.size();i++){
		s = moves[i];
		
		if(mom){
            stringboard(s,board3,1);
            vec = getFriends(board3,1);
            val = max(val,maxvalue(board3,alpha,beta,depth+1,false,vec));
            
            if(val>=beta){
                return val;
            }
            
            alpha = max(alpha,val);
        }else{
            stringboard(s,board3,0);
            vec = getFriends(board3,0);
            val = min(val,maxvalue(board3,alpha,beta,depth+1,true,vec));
            
            if(val<=alpha){
                return val;
            }
            beta = min(beta,val);
        }

		vec.clear();
		for(i1 = 0;i1<21;i1++){
			for(i2 = 0;i2<21;i2++){
				board3[i1][i2] = board2[i1][i2];
			}
		}
	}
	return val;
}

// cutoff search
string cos(){
    string s = "";
    string s2 = "";
    int val,i1,i2;
    int tempboard[21][21];
    int max = -1000;
    int i = 0;

    for(i1 = 0;i1<21;i1++){
    	for(i2 = 0;i2<21;i2++){
    		tempboard[i1][i2] = arr[i1][i2];
    	}
    }

    vector<string> vec = getFriends(tempboard,1);

    for(i1 = 0;i1<21;i1++){
    	for(i2 = 0;i2<21;i2++){
    		tempboard[i1][i2] = arr[i1][i2];
    	}
    }
    
    for(i = 0;i<vec.size();i++){
    	s2 = vec[i];
    	stringboard(s2,tempboard,1);
    	val = maxvalue(tempboard,-1000,1000,0,true,vec);
    	if(val>max){
    		max = val;
    		s = s2;
    	}
    	for(i1 = 0;i1<21;i1++){
	    	for(i2 = 0;i2<21;i2++){
	    		tempboard[i1][i2] = arr[i1][i2];
	    	}
	    }
    }
    return s;
}

int main(int argc, char** argv) {
    int player_id, board_size, time_limit;
    cin >> player_id;
    cin >> board_size;
    cin >> time_limit;
    cin.ignore();
    string s;
    string s2;
    int counter = 0;
    vector<string> v;
    int sec_arr[21][21];
    initial_state(board_size);

    
    if(player_id==2){
        // Get other player's move
        while(true){
        	getline(cin,s);
        	movromring(s,0);
        	if(counter<5){
        		s2 = placement(counter);
        	}else{
                for(int i = 0;i<21;i++){
                    for(int j = 0;j<21;j++){
                        sec_arr[i][j] = arr[i][j];
                    }
                }
                v = getFriends(sec_arr,1);
                // s2 = cos();
                s2 = v[0];
        	}
        	counter++;
        	cout << s2 << endl;
        	movromring(s2,1);
        }
        
    }else if(player_id==1){
        while(true){
        	if(counter<5){
        		s2 = placement(counter);
        		cout << s2 << endl;
        		movromring(s2,1);
        		getline(cin,s);
        		movromring(s,0);
        	}else{
                for(int i = 0;i<21;i++){
                    for(int j = 0;j<21;j++){
                        sec_arr[i][j] = arr[i][j];
                    }
                }
                v = getFriends(sec_arr,1);
        		// s2 = cos();
                s2 = v[0];
        		cout << s2 << endl;
        		movromring(s2,1);
        		getline(cin,s);
        		movromring(s,0);
        	}
        	counter++;   
        }
    }else{

    }
    return 0;
}