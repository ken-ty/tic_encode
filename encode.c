/*=====================================*/
/* encode.c                            */
/* 3進数の状態state3を10進数に直す。   */
/* 異なる3進数を入力しても、変換により */
/* 同様の状態とみなせる場合、同じ10進数*/
/* にする。                            */ 
/*-------------------------------------*/ 
/*version 0.1 (2019/08/08) 作成        */ 
/*version 0.2 (2019/08/09) バグを修正  */ 
/*=====================================*/ 

#include <stdio.h> 
#define STATE_MAX 19683 

int cand(int state3[9], int convert[9]); 

int main(void)
{ 
    int cands[8]; 
    int state10; 
    int i, j, k; 
    //変換行列 
    int convert[8][9] = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, //元の状態
                          { 3, 2, 1, 6, 5, 4, 9, 8, 7 }, //縦に線対称
                          { 7, 4, 1, 8, 5, 2, 9, 6, 3 }, //右90°回転
                          { 1, 4, 7, 2, 5, 8, 3, 6, 9 }, //左斜に線対称
                          { 9, 8, 7, 6, 5, 4, 3, 2, 1 }, //右に180°回転
                          { 7, 8, 9, 4, 5, 6, 1, 2, 3 }, //横に線対称
                          { 3, 6, 9, 2, 5, 8, 1, 4, 7 }, //右に270°回転
                          { 9, 6, 3, 8, 5, 2, 7, 4, 1 }, //右斜に線対称
                        };

    int state3[STATE_MAX][9];
    int power3[9] = { 6561, 2187, 729, 243, 81, 27, 9, 3, 1 }; //{ 3^8, 3^7, ... , 3^0 }

    int rem[STATE_MAX] = {0};
    int cnt = 0;

    for (i = 0/*STATE_MAX-1*/; i < STATE_MAX; i++) {

        //state3を入力
        int n = i;
        for (j = 0; j < 9; j++) {
            state3[i][j] = n / power3[j];
            n %= power3[j];
        }
        //状態を見てみる
        /*
        printf("state3[%d] = { ", i);
        for (j = 0; j < 9; j++) {
            printf("%d, ", state3[i][j]);
        }
        printf("}\n");
        */

        //変換をして10進数に直し、候補を作成する。
        for (j = 0; j < 8; j++) {
            cands[j] = cand( state3[i], convert[j] );
            //候補を見てみる
            /*
            printf("cands[%d] = %d\n", j, cands[j]);
            */
        }

        //候補から、一番小さいものをstate10とする。
        state10 = STATE_MAX;
        for ( j = 0; j < 8; j++ ) {
            if (cands[j] < state10) {
                state10 = cands[j];
            }
        }

        //結果を出力
        //printf("state10 = %d\n", state10);

        //新規ならカウントして、配列に入れる。
        if ( i == 0 ) {
            rem[cnt] = state10;
            printf("状態 = { %d, ", rem[cnt]);
            cnt++;
        }
        int flag = 1;
        for ( j = 0; j < cnt; j++) {
            if ( rem[j] == state10 ) {
                flag = 0;
                break;
            }
        }
        if ( flag ) {
            rem[cnt] = state10;
            printf("%d, ", rem[cnt]);
            cnt++;
        }
    }
    printf("\b\b }\n");
    printf("エンコード済み状態数 %d\n", cnt);

    return 0;

}

//変換行列convertがstate3から生成するstate10の候補を返す
int cand(int state3[9], int convert[9])
{
    int temp[9] = {0};
    int cand = 0;
    int i, j;
    //3進数から10進数に変換するための行列
    int power3[9] = { 6561, 2187, 729, 243, 81, 27, 9, 3, 1 }; //{ 3^8, 3^7, ... , 3^0 }

    //temp[i]に変換した値を代入
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (convert[j] == i+1) { //マスは1~9に注意
                temp[i] = state3[j];
                break;
            }
        }
        //i桁目を3進数から10進数に直して足していく。
        cand += temp[i] * power3[i];
        //printf("temp[%d] = %d\n", i, temp[i]);
    }
    //printf("\n");

    return cand;
}

