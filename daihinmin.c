#include<stdio.h>
#include<string.h>
#include<strings.h>
#include"common.h"

void search_low_card(int out_cards[8][15],int my_cards[8][15],int use_joker_flag){
  /*
    低い方から探して,最初に見つけたカードを一枚out_cardsにのせる。
    use_joker_flagが1のとき,カードが見つからなければ,jokerを一枚out_cardsにのせる。
  */
  int i,j,find_flag=0;

  clear_table(out_cards);                  //テーブルをクリア
  for(j=1;j<14&&find_flag==0;j++){        //低い方からさがし
    for(i=0;i<4&&find_flag==0;i++){
      if(my_cards[i][j]==1){              //カードを見つけたら               
        find_flag=1;                      //フラグを立て
        out_cards[i][j]=my_cards[i][j];   //out_cardsにのせ,ループを抜ける。
      }
    }
  }
  if(find_flag==0&&use_joker_flag==1){       //見つからなかったとき
    out_cards[0][14]=2;                  //ジョーカーをのせる
  }
}


void make_info_table(int info_table[8][15], int my_cards[8][15]){
	int i,j;
  clear_table(info_table);
  for(j=0;j<4;j++){
    for(i = 13;i >= 1;i--){
      if(my_cards[j][i] == 1){
        if(i == 13){
          info_table[j][i] = 1;  
        }
        else{
          info_table[j][i] = info_table[j][i+1] + 1;
        }  
      }
      else{
        info_table[j][i] = 0;
      }
    }
    //2を1にする
    for(i=1;i<=13;i++){
		  if(info_table[j][i] == 2){
        info_table[j][i] = 1;
      }
	  }
  }//階段用終了

  for(i=1;i<=13;i++){
		info_table[4][i] = my_cards[0][i] + my_cards[1][i] + my_cards[2][i] + my_cards[3][i];//ペアだし
	}
}

void make_info_j_table(int info_j_table[8][15], int my_cards[8][15]){
  int i = 0;
  for(i=1;i<=13;i++){
		info_j_table[4][i] = my_cards[0][i] + my_cards[1][i] + my_cards[2][i] + my_cards[3][i] + 1;//ペアだし
	}
}

int search_low_stairs(int dst_cards[8][15], int info_table[8][15], int my_cards[8][15]){
  int i,j,k;
  int end_flag = 0;
  int count_cards = 0;
	clear_table(dst_cards);
	for(i=1;i<=13;i++){
    for(j=0;j<=3;j++){
      if(info_table[j][i]>=3){//このカードから初めて階段ができるか
        end_flag = 1;
        count_cards = info_table[j][i];
        break;
      }
    }
    if(end_flag){
      break;
    }
	}
	if(i<=13){
      for(k=0;k < count_cards;k++){
        dst_cards[j][i] = my_cards[j][i];
        i++;
      }
		return 1;
	}
	else return 0;
}

int search_count_stairs(int dst_cards[8][15], int info_table[8][15], int my_cards[8][15], int num){
  int i,j,k;
  int end_flag = 0;
  int count_cards = num;
	clear_table(dst_cards);
	for(i=1;i<=13;i++){
    for(j=0;j<=3;j++){
      if(info_table[j][i]>=num){//このカードから初めて階段ができるか
        end_flag = 1;
        break;
      }
    }
    if(end_flag){
      break;
    }
	}
	if(i<=13){
      for(k=0;k < count_cards;k++){
        dst_cards[j][i] = my_cards[j][i];
        i++;
      }
		return 1;
	}
	else return 0;
}

int search_low_pair(int dst_cards[8][15], int info_table[8][15], int my_cards[8][15]){
	int i,j;
	clear_table(dst_cards);
	for(i=1;i<=13;i++){
		if(info_table[4][i]>=2){
			break;
		}
	}
	if(i<=13){
		for(j=0;j<=3;j++){
			dst_cards[j][i] = my_cards[j][i];
		}
		return 1;
	}
	else return 0;
}
int search_count_pair(int dst_cards[8][15], int info_table[8][15], int my_cards[8][15], int num){
	//num is field pair count
	int i,j;
  int n = 0;
  clear_table(dst_cards);
  for(i=1;i<=13;i++){
    if(info_table[4][i]>=num){
      break;
    }
  }
	if(i<=13){
    for(j=0;j<=3;j++){
      dst_cards[j][i] = my_cards[j][i];
      n += my_cards[j][i];
      if(n==num){
        break;
      }
    }
    return 1;
  }
  else return 0;
}

int search_low_card_wosp(int out_cards[8][15], int info_table[8][15], int my_cards[8][15]){
  int i, j, k, end_loop_flag = 0, end_search_flag = 0;
  int count_cards;

  clear_table(out_cards);
  //ペアと階段を除いた最弱カード
  for(j=1;j<14&&end_loop_flag==0;j++){        //低い方からさがし
    for(i=0;i<4&&end_loop_flag==0;i++){
      if(my_cards[i][j]==1){              //カードを見つけたら               
        //ペアがなく階段もなければそのカードにする
        end_search_flag = 0;
        if(info_table[4][j] < 2){
          if(j > 3){
            for(k = j;k > (j - 4);k--){
              if(info_table[i][k] >= 3){
                end_search_flag = 1;
                break;
              }
            }
          }
          else{//
            for(k = j;k > 0;k--){
              if(info_table[i][k] >= 3){
                end_search_flag = 1;
                break;
              }
            }
          }
          if(!end_search_flag){
            end_loop_flag = 1;                      //フラグを立て
            out_cards[i][j]=my_cards[i][j];   //out_cardsにのせ,ループを抜ける。
          }
        }
      }
    }
  }
  if(end_loop_flag){
    return 1;
  }
  else return 0;
}

int search_low_pair_wj(int dst_cards[8][15], int info_j_table[8][15], int my_cards[8][15]){
  int i,j;
  int jFlag = 0;//ジョーカーは代入したか
	clear_table(dst_cards);
	for(i=1;i<=13;i++){
		if(info_j_table[4][i]>=2){
			break;
		}
	}
	if(i<=13){
		for(j=0;j<=3;j++){
      if(my_cards[j][i] == 0 && !jFlag){
        dst_cards[j][i] = 2;
        jFlag = 1;
      }
      else{
        dst_cards[j][i] = my_cards[j][i];
      }
		}
		return 1;
	}
	else return 0;
}