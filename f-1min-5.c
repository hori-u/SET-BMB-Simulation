#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MOVIE_TIME 60                           /*動画時間 */
#define MOVIE_BITRATE 2 * 1000000               /*再生レート2Mbps */
double MOVIE_SIZE = MOVIE_TIME * MOVIE_BITRATE; /*動画サイズ=動画時間*再生レート*/
double SIZE_PER_SGM = 1000096;                  /*ブロックサイズ*/
#define NUM_SGM (MOVIE_TIME*2)+1                /*ブロック数=動画サイズ/ブロックサイズ　<動画時間*2>*/
//double TIME_PER_SGM = MOVIE_TIME/NUM_SGM;         
double TIME_PER_SGM = 0.5;                      /*１ブロックの再生時間＝動画時間/ブロック数*/
//double MULTICAST_PER_SGM = 0.125;             /*1ブロックの放送時間＝ブロックサイズ/放送帯域*/ 
int MAX_JOIN_USER = 200;						/*参加ユーザ数 課金ユーザ10% 高課金ユーザ10%*/
int MAX_BREAK_USER = 500;						/*離脱ユーザ数*/
int ter_n = 10;									/*ユーザの初期値，最大値はMAX_JOIN_USER) */
int G = 8;                                      /*スケジュールするセグメント数*/
int SN = 4;
int MULTICAST_BANDWIDTH = 8 * 1000000;          /*8Mbps　  放送帯域幅*/
int UNICAST_BANDWIDTH = 1 * 1000000;            /*1Mbps    通信帯域幅*/
int SUPER_NODE_BANDWIDTH = 8 * 1000000;			/*8Mbps    スーパーノード帯域幅*/
int DIFFERENCE_BANDWIDTH = 8 * 1000000; 		/*8Mbps    通常画質と高画質の差分*/
int calcUNICAST_BANDWIDTH = 1 * 1000000;
int SEVER_BANDWIDTH = 30 * 1000000;             /*30Mbps   サーバの帯域*/

double SWICH_TIME = 0.1;						/*チャンネル切り替え時間(仮)*/



typedef struct terminal_t {
	double  play_t;	          /* 端末が再生している動画の再生時間 */
	double  test_play_t;      /*シミュレーション用再生時間*/
    double  high_play_t;
	double  wait_t;           /* 再生待ち時間 */
	double  test_wait_t;      /*シミュレーション用待ち時間*/
	char    sgm[NUM_SGM];	  /* 受信完了しているセグメント */
	char	high_sgm[NUM_SGM];/*受信完了している高画質セグメント*/	
	char    test_sgm[NUM_SGM];
	int     break_flag;       /*離脱フラグ*/
	int     finish_flag;	  /*受信完了フラグ0:未受信,1:通常画質受信,2:高画質受信*/
	int		billing_flag;	  /*課金フラグ0-2*/
}terminal_t;                  /* 動画再生端末端末の保持情報 */

double play_postponement_time(terminal_t *p);

int print_define_data() {
	printf("NUM_SGM       %d\n", NUM_SGM - 1);
	printf("TIME_PER_SGM  %f\n", TIME_PER_SGM);
	printf("MOVIE_TIME    %d\n", MOVIE_TIME);
	printf("MOVIE_SIZE    %f\n", MOVIE_SIZE);
	printf("MOVIE_BITRATE %d\n", MOVIE_BITRATE);
	printf("MAX_JOIN_USER %d\n", MAX_JOIN_USER);
	printf("MAX_BREAK_USER %d\n", MAX_BREAK_USER);
	printf("MULTICAST_BANDWIDTH %d\n", MULTICAST_BANDWIDTH);
	printf("UNICAST_BANDWIDTH   %d\n", UNICAST_BANDWIDTH);
	printf("\n");
}

int billing_check(){
	int i;
	i = rand() % 10;
	if (i <= 0){
		 return true;//1なら課金者
	}
	else return false;//非課金者
	
}

int init_terminal(terminal_t *p) {/*すべての動画再生端末の初期化*/
	int i, m;
	for (i = 0; i < MAX_JOIN_USER; i++, p++) {/* 初期値0を代入*/
		(*p).play_t = 0;
		(*p).test_play_t = 0;
		(*p).wait_t = 0;
		(*p).test_wait_t = 0;
		(*p).break_flag = 0;
		(*p).finish_flag = 0;
		
		for (m = 1; m < NUM_SGM; m++) { /*セグメント数は(NUM_SGM-1)まで*/
			(*p).sgm[m] = 0;
			(*p).test_sgm[m] = 0;
		}
		(*p).billing_flag = billing_check();
	}
	return 0;
}

int init_terminal2(terminal_t *p,int *bill2) {/*すべての動画再生端末の初期化*/
	int i, m;
	for (i = 0; i < MAX_JOIN_USER; i++, p++) {/* 初期値0を代入*/
		(*p).play_t = 0;
		(*p).test_play_t = 0;
		(*p).wait_t = 0;
		(*p).test_wait_t = 0;
		(*p).break_flag = 0;
		(*p).finish_flag = 0;
		
		for (m = 1; m < NUM_SGM; m++) { /*セグメント数は(NUM_SGM-1)まで*/
			(*p).sgm[m] = 0;
			(*p).test_sgm[m] = 0;
		}
		// (*p).billing_flag = billing_check();
		(*p).billing_flag = *bill2;
		if (i < MAX_JOIN_USER - 1)
		++bill2;
	}
	return 0;
}


int init_terminal3(terminal_t *p, int *bill3) {/*すべての動画再生端末の初期化*/
	int i, m;
	for (i = 0; i < MAX_JOIN_USER; i++, p++) {/* 初期値0を代入*/
		(*p).play_t = 0;
		(*p).test_play_t = 0;
		(*p).wait_t = 0;
		(*p).test_wait_t = 0;
		(*p).break_flag = 0;
		(*p).finish_flag = 0;
		
		for (m = 1; m < NUM_SGM; m++) { /*セグメント数は(NUM_SGM-1)まで*/
			(*p).sgm[m] = 0;
			(*p).test_sgm[m] = 0;
		}
		// (*p).billing_flag = billing_check();
		(*p).billing_flag = *bill3; /*課金状況*/
		if (i < MAX_JOIN_USER - 1)
		++bill3;
	}
	return 0;
}

int print_terminal_info(terminal_t *p, int num) {/* numまでの動画再生端末の情報を表示する*/
	int i, m;
	for (i = 0; i < num; i++, p++) {

		if ((*p).break_flag == 1) {
			printf("\n\nUSER:%dbreak\n\n", i);
		}
		else {
			printf("USER:%d\nplay_t:%.3f\nwait_t:%.3f\nPPT:%.3f\nBILLING_FLAG:%d\n"
				, i, (*p).play_t, (*p).wait_t, play_postponement_time(p),(*p).billing_flag);
			//printf("test_play_t:%.3f\ntest_wait_t:%.3f\n", (*p).test_play_t, (*p).test_wait_t);
			for (m = 1; m < NUM_SGM; m++) { /*セグメント数(NUM_SGM-1)まで*/
				if ((*p).sgm[m] == 1) {			/*放送型配信(Broadcast type distribution)*/
					printf("X");
				}
				else if ((*p).sgm[m] == 2) {	/*端末伝送型配信(terminal transmission type distribution)*/
					printf("Z");
				}
				else if ((*p).sgm[m] == 3) {	/*オンデマンド型(On-demand delivery)*/
					printf("B");
				}
				else {
					printf("_");
				}
			}
		}

		printf("\n");
	}


	return 0;
}

int send_sgm(terminal_t *p, int sgm_n, int ter_n) {/*現在接続している端末へセグメントsgmを放送する */
	printf("\nsend_sgm_by_multicast:%d USER:%d\n", sgm_n,ter_n);
	int i;

	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).break_flag != 1) {
			if((*p).sgm[sgm_n] == 0){
				(*p).sgm[sgm_n] = 1;	/*放送型配信(Broadcast type distribution)*/
				//(*p).high_sgm[sgm_n] = 2;	/*高画質セグメントのP2P配信*/
			}
		}
	}
	return 0;
}

int send_high_sgm(terminal_t *p, int sgm_n, int ter_n) {/*現在接続している端末へセグメントsgmを放送する */
	printf("\nsend_sgm_by_multicast:%d USER:%d\n", sgm_n,ter_n);
	int i;
	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).break_flag != 1) {
			if((*p).high_sgm[sgm_n] == 0){
				//(*p).sgm[sgm_n] = 1;	/*放送型配信(Broadcast type distribution)*/
				(*p).high_sgm[sgm_n] = 2;	/*高画質セグメントのP2P配信*/
			}
		}
	}
	return 0;
}

int send_sgm_p2p(terminal_t *p, int min_req_sgm, int req_user){ /*接続ユーザ内で最も古いセグメントを要求しているユーザにP2Pでセグメント送信*/
	int i;
	for (i = 0; i <= req_user; i++, p++) {
		if (i = req_user) {
			(*p).sgm[min_req_sgm] = 2;
		}
	}
	printf("\nsend_sgm_by_p2p:%d USER:%d\n", min_req_sgm,req_user);
	return 0;
}

int send_sgm_p2p2(terminal_t *p, int num){ /*接続ユーザ内で最も古いセグメントを要求しているユーザにP2Pでセグメント送信*/
	int i,m,j;
	int min_req_sgm=10000;
	int req_user=0;
	int seeder_req_sgm=0;
	int sgm = 10000;

	// sgm_num_find(p, &min_req_sgm, &req_user, &seeder_req_sgm, num);
	terminal_t *tmp = p;
	terminal_t *tmp2 = p;

	// printf("tmp = %p\np = %p\n",tmp,p);

	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).billing_flag == true) {	//課金フラグが1なら
			if ((*p).break_flag != 1) {
				seeder_req_sgm = continuing_playable_sgm(p)+1;  
				break;
			}
		}
	}

	for (j = 0; j < ter_n; j++, tmp2++) {
			// if((*p).billing_flag != true){
			// 	continue;
			// }
		if ((*tmp2).billing_flag == true){
			if ((*tmp2).break_flag != 1) {
				printf("user:%d req_sgm:%d\n",j,continuing_playable_sgm(tmp2) + 1);

				if (continuing_playable_sgm(tmp2) + 1 < min_req_sgm) {
					min_req_sgm = continuing_playable_sgm(tmp2) + 1;//?S???[?U???v?????????Z?O?????g????????????Z?O?????g??\??
					req_user = j;
				}
			}
		}
	}

	// if (min_req_sgm < seeder_req_sgm) {
		for (i = 0; i <= req_user; i++, tmp++) {
			if (i == req_user) {
				(*tmp).sgm[min_req_sgm] = 2;
				printf("send_sgm_by_p2p:%d\nreq_USER=%d\n", min_req_sgm, req_user);
				return 0;
			}
		}
	// }
	// print_terminal_info(p, req_user);
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int send_sgm_p2p3(terminal_t *p, int num){ /*接続ユーザ内で最も古いセグメントを要求しているユーザにP2Pでセグメント送信*/
	int i,m,j;
	int min_req_sgm=10000;
	int req_user=0;
	int seeder_req_sgm=0;
	int sgm = 10000;

	// sgm_num_find(p, &min_req_sgm, &req_user, &seeder_req_sgm, num);
	terminal_t *tmp = p;
	terminal_t *tmp2 = p;
	terminal_t *tmp3 = p;

	// printf("tmp = %p\np = %p\n",tmp,p);

	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).billing_flag != 0) {	//課金フラグが0でないなら
			if ((*p).break_flag != 1) {
				seeder_req_sgm = continuing_playable_sgm(p)+1;  /*最初から連続で再生可能なセグメント数を求める*/
				break;
			}
		}
	}

	for (j = 0; j < ter_n; j++, tmp2++) {
		//高課金ユーザで最初から連続で再生可能なセグメントを求める
		if ((*tmp2).billing_flag != 0){	//課金フラグが2なら
			if ((*tmp2).break_flag != 1) {
				printf("user:%d req_sgm:%d\n",j,continuing_playable_sgm(tmp2) + 1);

				if (continuing_playable_sgm(tmp2) + 1 < min_req_sgm) {
					min_req_sgm = continuing_playable_sgm(tmp2) + 1;
					req_user = j;
				}
			}
		}
	}

	if(req_user == 0){	//動画が配信されていない高課金ユーザがいないなら低課金ユーザにスケジューリングを行う
		for (j = 0; j < ter_n; j++, tmp3++) {
			
			//if ((*tmp3).billing_flag == 1){	//課金フラグが1なら
				if ((*tmp3).break_flag != 1) {
					printf("user:%d req_sgm:%d\n",j,continuing_playable_sgm(tmp3) + 1);

					if (continuing_playable_sgm(tmp3) + 1 < min_req_sgm) {
						min_req_sgm = continuing_playable_sgm(tmp3) + 1;
						req_user = j;
					}
				}
			//}
		}
	}

	// if (min_req_sgm < seeder_req_sgm) {
	for (i = 0; i <= req_user; i++, tmp++) {
		if (i == req_user) {
			(*tmp).sgm[min_req_sgm] = 2;	/*接続ユーザ内で最も古いセグメントを要求しているユーザにP2Pでセグメント送信*/
			printf("send_sgm_by_p2p:%d\nreq_USER=%d\n", min_req_sgm, req_user);
			return 0;
		}
	}
	// }
	// print_terminal_info(p, req_user);
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
int send_high_sgm_p2p(terminal_t *p, int num){ /*接続ユーザ内で最も古いセグメントを要求しているユーザにP2Pでセグメント送信*/
	int i,m,j;
	int min_req_sgm=10000;
	int req_user=0;
	int seeder_req_sgm=0;
	int sgm = 10000;

	// sgm_num_find(p, &min_req_sgm, &req_user, &seeder_req_sgm, num);
	terminal_t *tmp = p;
	terminal_t *tmp2 = p;
	terminal_t *tmp3 = p;

	// printf("tmp = %p\np = %p\n",tmp,p);

	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).billing_flag != 0) {	//課金フラグが0でないなら
			if ((*p).break_flag != 1) {
				seeder_req_sgm = continuing_playable_high_sgm(p)+1;  /*最初から連続で再生可能なセグメント数を求める*/
				break;
			}
		}
	}

	for (j = 0; j < ter_n; j++, tmp2++) {
		//高課金ユーザで最初から連続で再生可能なセグメントを求める
		if ((*tmp2).billing_flag == 2){	//課金フラグが2なら
			if ((*tmp2).break_flag != 1) {
				printf("user:%d req_sgm:%d\n",j,continuing_playable_high_sgm(tmp2) + 1);

				if (continuing_playable_high_sgm(tmp2) + 1 < min_req_sgm) {
				//if (continuing_playable_sgm(tmp2) + 1 < min_req_sgm) {
					min_req_sgm = continuing_playable_high_sgm(tmp2) + 1;
					//min_req_sgm = continuing_playable_sgm(tmp2) + 1;
					req_user = j;
				}
			}
		}
	}

	if(req_user == 0){	//動画が配信されていない高課金ユーザがいないなら低課金ユーザにスケジューリングを行う
		for (j = 0; j < ter_n; j++, tmp3++) {
			
			if ((*tmp3).billing_flag == 1){	//課金フラグが1なら
				if ((*tmp3).break_flag != 1) {
					printf("user:%d req_sgm:%d\n",j,continuing_playable_high_sgm(tmp3) + 1);

					if (continuing_playable_high_sgm(tmp3) + 1 < min_req_sgm) {
					//if (continuing_playable_sgm(tmp3) + 1 < min_req_sgm) {
						min_req_sgm = continuing_playable_high_sgm(tmp3) + 1;
						//min_req_sgm = continuing_playable_sgm(tmp3) + 1;
						req_user = j;
					}
				}
			}
		}
	}

	// if (min_req_sgm < seeder_req_sgm) {
	for (i = 0; i <= req_user; i++, tmp++) {
		if (i == req_user) {
			(*tmp).high_sgm[min_req_sgm] = 2;	/*接続ユーザ内で最も古いセグメントを要求しているユーザにP2Pで高画質セグメント送信*/
			printf("send_sgm_by_p2p:%d\nreq_USER=%d\n", min_req_sgm, req_user);
			return 0;
		}
	}
	// }
	// print_terminal_info(p, req_user);
	
	return 0;
}

int continuing_playable_sgm(terminal_t *p) {       /*最初から連続で再生可能なセグメント数を求める*/
	int i;
	for (i = 1; i < NUM_SGM; i++) {
		if ((*p).sgm[i] == 0) {	/* 受信完了していないセグメントを返す */
			return (i - 1);
		}
	}
	return NUM_SGM - 1;/*最大値*/
}

int continuing_playable_high_sgm(terminal_t *p) {
	int i;
	for (i = 1; i < NUM_SGM; i++) {
		if ((*p).high_sgm[i] == 0) {	/* 受信完了していないセグメントを返す */
			return (i - 1);
		}
	}
	return NUM_SGM - 1;/*最大値*/
}


double play_postponement_time(terminal_t *p) {         /*余裕時間を計算する*/
	if ((*p).break_flag == 1 || (*p).finish_flag != 0) {
		return 10000;
	}
	return continuing_playable_sgm(p) * TIME_PER_SGM - (*p).play_t;
}

double play_postponement_high_time(terminal_t *p) {         /*余裕時間を計算する*/
	if ((*p).break_flag == 1 || (*p).finish_flag != 0) {
		return 10000;
	}
	return continuing_playable_high_sgm(p) * TIME_PER_SGM - (*p).play_t;
}


double p_i_play_t(terminal_t *p, int i) {
	int m;
	for (m = 1; m < i; m++, p++);
	return (*p).play_t;
}


double set_c_return_sgm_num(terminal_t *p, int ter_n) {/*最小余裕時間を持つユーザを探し，そのユーザの要求しているセグメントを返す*/
	int i;
	int min_PPT_sgm = 0;
	double eps = 1e-10;
	double p_i;
	double Movie_t = MOVIE_TIME;

	for (i = 0; i < ter_n; i++) {
		p_i = p_i_play_t(p, i);

		if (p[i].break_flag != 1 || p[i].finish_flag == 0) {	//update

			if (play_postponement_time(p + min_PPT_sgm) >= play_postponement_time(p + i)) {
				if (fabs(p_i - Movie_t) > eps) {
					min_PPT_sgm = i;
					//printf("%d\n", i);
				}
			}

		}

	}
	if (continuing_playable_sgm(p + min_PPT_sgm) == NUM_SGM) {
		return 0;
	}
	else {
		return continuing_playable_sgm(p + min_PPT_sgm) + 1;
	}
}

double set_c_return_high_sgm_num(terminal_t *p, int ter_n) {/*最小余裕時間を持つユーザを探し，そのユーザの要求しているセグメントを返す*/
	int i;
	int min_PPT_sgm = 0;
	double eps = 1e-10;
	double p_i;
	double Movie_t = MOVIE_TIME;

	for (i = 0; i < ter_n; i++) {
		p_i = p_i_play_t(p, i);

		if (p[i].break_flag != 1 || p[i].finish_flag == 0) {	//update

			if (play_postponement_time(p + min_PPT_sgm) >= play_postponement_time(p + i)) {
				if (fabs(p_i - Movie_t) > eps) {
					min_PPT_sgm = i;
					//printf("%d\n", i);
				}
			}

		}

	}
	if (continuing_playable_high_sgm(p + min_PPT_sgm) == NUM_SGM) {
		return 0;
	}
	else {
		return continuing_playable_high_sgm(p + min_PPT_sgm) + 1;
	}
}



int add_play_t_or_wait_t(terminal_t *p, int ter_n, double before_wait_time[]) {/*各ユーザに再生時間または待ち時間を加算*/
	int i;
	int k;
	int flag = 0;
	/*1ユーザずつ状況を取得*/
	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).break_flag != 1) {/*接続状況の確認*/

			if ((*p).finish_flag != 0 && (*p).play_t <= MOVIE_TIME) {/*update受信完了かつ再生時間が動画時間を下回っている場合*/
				(*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
			}
			else if (play_postponement_time(p) >= SIZE_PER_SGM / MULTICAST_BANDWIDTH) {/*余裕時間が十分にある場合*/
				(*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
			}
			else if ((*p).play_t != MOVIE_TIME) {/*余裕時間が十分にないかつ再生途中である場合*/
				(*p).wait_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH - play_postponement_time(p);
				(*p).play_t += play_postponement_time(p);
					if (before_wait_time[i] < 0) {  /*G回ループ途中にユーザ追加されたら，そのユーザの待ち時間を0にする（マイナスの数値が入っているから）*/
					before_wait_time[i] = 0;
					}
				if (flag != 1) {
					if ((*p).wait_t - before_wait_time[i] >= SIZE_PER_SGM / UNICAST_BANDWIDTH) {/*放送を待つより通信で受信した方が早いユーザを見つける*/
						//printf("(%d).wait_t:%.3f\n",i, (*p).wait_t);
						//printf("before_wait_time[%d]:%.3f\n", i, before_wait_time[i]);
						//printf("SIZE_PER_SGM / UNICAST_BANDWIDTH=%f\n", SIZE_PER_SGM / UNICAST_BANDWIDTH);
						printf("USER%dwaittimeover!============================================================================================\n", i);
						for (k = 1; k < NUM_SGM; k++) {                /*放送を待つより通信で受信した方が早いユーザにセグメントを通信で送信する*/
							if (flag != 1) {
								if ((*p).sgm[k] == 0) {
									(*p).sgm[k] = 3;	/*オンデマンド型*/
									printf("USER%dsendsgm%d\n", i, k);
									flag = 1;
								}
							}
						}
					}
				}

			}
		}
	}
	flag = 0;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int add_play_t_or_wait_t_2(terminal_t *p, int ter_n, double before_wait_time[]) {/*各ユーザに再生時間または待ち時間を加算*/
	int i;
	int k;
	int flag = 0;
	/*1ユーザずつ状況を取得*/
	for (i = 0; i < ter_n; i++, p++) {
        //flag = 0;

		if ((*p).break_flag != 1) {/*接続状況の確認*/

			if ((*p).finish_flag != 0 && (*p).play_t <= MOVIE_TIME) {/*受信完了かつ再生時間が動画時間を下回っている場合*/
				(*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
                if ((*p).finish_flag == 2) {
                    (*p).high_play_t += SIZE_PER_SGM / DIFFERENCE_BANDWIDTH;
                }
			}
            else if (play_postponement_time(p) >= SIZE_PER_SGM / MULTICAST_BANDWIDTH + SIZE_PER_SGM / DIFFERENCE_BANDWIDTH) {/*高画質セグメントを受信する余裕時間がある場合*/
				int t = (*p).play_t / TIME_PER_SGM; //現在再生中のセグメント
                (*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
                if((*p).high_sgm[t] != 0) {
                    (*p).high_play_t += SIZE_PER_SGM / DIFFERENCE_BANDWIDTH; //高画質セグメントの再生時間加算
                }

				for (k = t+1; k < NUM_SGM; k++) {                //高画質セグメントを送信
					if (flag != 1) {
						if ((*p).high_sgm[k] == 0) {
							(*p).high_sgm[k] = 2;
							printf("USER%dsendhighsgm%d\n", i, k);
							flag = 1;
						}
					}
				}
            }
			else if (play_postponement_time(p) >= SIZE_PER_SGM / MULTICAST_BANDWIDTH) {/*余裕時間が十分にある場合*/
				int t = (*p).play_t / TIME_PER_SGM; //現在再生中のセグメント
                (*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
                if((*p).high_sgm[t] != 0) {
                    (*p).high_play_t += SIZE_PER_SGM / DIFFERENCE_BANDWIDTH; //高画質セグメントの再生時間加算
                }
			}
			else if ((*p).play_t != MOVIE_TIME) {/*余裕時間が十分にないかつ再生途中である場合*/
                int t = (*p).play_t / TIME_PER_SGM; //現在再生中のセグメント
				(*p).wait_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH - play_postponement_time(p);
				(*p).play_t += play_postponement_time(p);
                if((*p).high_sgm[t] != 0) {
                    (*p).high_play_t += play_postponement_time(p); //高画質セグメントの再生時間加算
                }

					if (before_wait_time[i] < 0) {  /*G回ループ途中にユーザ追加されたら，そのユーザの待ち時間を0にする（マイナスの数値が入っているから）*/
						before_wait_time[i] = 0;
					}
				if (flag != 1) {
					if ((*p).wait_t - before_wait_time[i] >= SIZE_PER_SGM / UNICAST_BANDWIDTH) {/*放送を待つより通信で受信した方が早いユーザを見つける*/
						//printf("(%d).wait_t:%.3f\n",i, (*p).wait_t);
						//printf("before_wait_time[%d]:%.3f\n", i, before_wait_time[i]);
						//printf("SIZE_PER_SGM / UNICAST_BANDWIDTH=%f\n", SIZE_PER_SGM / UNICAST_BANDWIDTH);
						printf("USER%dwaittimeover!============================================================================================\n", i);
						for (k = 1; k < NUM_SGM; k++) {                /*放送を待つより通信で受信した方が早いユーザにセグメントを通信で送信する*/
							if (flag != 1) {
								if ((*p).sgm[k] == 0) {
									(*p).sgm[k] = 3;	/*オンデマンド型*/
									printf("USER%dsendsgm%d\n", i, k);
									flag = 1;
								}
							}
						}
					}
				}

			}
		}
	}
	flag = 0;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int add_play_t_or_wait_t_high(terminal_t *p, int ter_n, double before_wait_time[]) {/*各ユーザに高画質動画の再生時間または待ち時間を加算*/
	int i;
	int k;
	int flag = 0;
	/*1ユーザずつ状況を取得*/
	for (i = 0; i < ter_n; i++, p++) {
        //flag = 0;

		if ((*p).break_flag != 1) {/*
		接続状況の確認*/

			if ((*p).finish_flag == 2 && (*p).play_t <= MOVIE_TIME) {/*高画質動画受信完了かつ再生時間が動画時間を下回っている場合*/
				//(*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
				/*for (k = 1; k < NUM_SGM; k++) {
					if ((*p).high_sgm[k] == 0) {
						(*p).high_sgm[k] = 2;
						printf("USER%dsendhighsgm%d\n", i, k);
					}
				}*/
			}
			else if (play_postponement_time(p) >= SIZE_PER_SGM / DIFFERENCE_BANDWIDTH) {/*余裕時間が十分にある場合*/
				int t = (*p).play_t / TIME_PER_SGM;
				//(*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
				for (k = t; k < NUM_SGM; k++) {                /*高画質セグメントを送信*/
					if (flag != 1) {
						if ((*p).high_sgm[k] == 0) {
							(*p).high_sgm[k] = 2;	/**/
							printf("USER%dsendhighsgm%d\n", i, k);
							flag = 1;
						}
					}
				}
			}
			else{	/*余裕時間が十分にないかつ再生途中である場合*/
				/*(*p).wait_t += SIZE_PER_SGM / DIFFERENCE_BANDWIDTH - play_postponement_time(p);
				int t = (*p).play_t / TIME_PER_SGM;
				for (k = t; k < NUM_SGM; k++) {                //高画質セグメントを送信
					if (flag != 1) {
						if ((*p).high_sgm[k] == 0) {
							(*p).high_sgm[k] = 2;
								printf("USER%dsendhighsgm%d\n", i, k);
								flag = 1;
						}
					}
				}
				//(*p).play_t += play_postponement_time(p);
				if (before_wait_time[i] < 0) {  //G回ループ途中にユーザ追加されたら，そのユーザの待ち時間を0にする（マイナスの数値が入っているから
					before_wait_time[i] = 0;
				}*/
			}
		}
	}
	flag = 0;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int before_wait_t_save(terminal_t *p, int ter_n, double before_wait_time[]) {  /*現在の待ち時間を配列に保存しておく*/
	int i;
	double a;
	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).break_flag == 1) {
			printf("USER:%dbreak\n", i);
			//printf("(%d)before_wait_time=%.3f\n", i, before_wait_time[i]);
			before_wait_time[i] = 0;
		}
		else {
			a = (*p).wait_t;
			before_wait_time[i] = a;
			printf("(%d)before_wait_time=%.3f\n", i, before_wait_time[i]);
		}
	}
	return 0;
}

/*update*/ 
bool conplete_sgm(terminal_t *p, int ter_n) {   /*すべてのユーザがセグメントを受信完了できたか判定する*/
	int i, j, m, n, f;
	for (i = 1; i <= ter_n; i++, p++) {

		if ((*p).break_flag != 1) {

			for (m = 1, f = 1; m < NUM_SGM; m++){	/*高画質のセグメントを受信できたかを判断する*/
				if ((*p).high_sgm[m] == 0) {	
					f = 0;					/*全ての高画質セグメントが受信できていたらf=1*/
				}
			}
			if (f == 1) {
				(*p).finish_flag = 2;
			}

			for (n = 1; n < NUM_SGM; n++){	/*通常画質のセグメントを受信できたかを判断する*/
				if ((*p).sgm[n] == 0) {
					return false;
				}
			}
			(*p).finish_flag = 1;
			
			if ((*p).billing_flag == 2) {
				if ((*p).high_sgm[NUM_SGM-1] == 0) {	//全高課金ユーザが最終的に高画質セグメントを受信できるまで終了しない
				//if ((*p).finish_flag != 2) {	//全高課金ユーザが高画質セグメントを受信できるまで終了しない
					return false;
				}
			}
		}
	}

	return true;
}


double randam_break(terminal_t *p, int ter_n) {/*ランダムなユーザを離脱させる（離脱フラグを立てる）*/
	int i;
	int j;
	for (j = 1; j < 1000; j++) {
		i = rand() % ter_n;
		if (p[i].break_flag != 1) {
			p[i].break_flag = 1;
			//printf("srand=%d\n", ter_n);
			printf("breakuser=%d\n\n\n", i);
			break;
		}
	}
	return 0;
}

//double caluclate_wait_t(terminal_t *p, int ter_n, double before_wait_time[]) {/*平均中断時間の計算*/
//	int i;
//	int user;
//	int x = 0;
//	double all_wait_time = 0;
//	double avarage;
//	for (i = 0; i < ter_n; i++,p++) {
//		if (before_wait_time[i] == 0) {
//			x++;
//		}
//		else {
//			all_wait_time = all_wait_time + before_wait_time[i];
//		}
//	}
//	user = ter_n - x;
//	avarage = all_wait_time / user;
//	printf("\nall_wait_time:%.3f\n", all_wait_time);
//	printf("remaining_ter_n:%d\n", x);
//	printf("avarage_wait_time:%.3f\n", avarage);
//}

/*update*/
double caluclate_wait_t(terminal_t *p, int ter_n, double before_wait_time[]) {/*平均中断時間の計算*/
	int i,j,m,cnt,q_cnt;
	int user=0;
	int k2_user = 0;	/*高課金ユーザ*/
	int k_user = 0;	/*課金ユーザ*/
	int n_user = 0;	/*非課金ユーザ*/
	int b_band = 0;	/*放送型配信(Broadcast type distribution)*/
	int c_band = 0;	/*オンデマンド型(On-demand delivery)*/
	int s_band = 0;	/*端末伝送型配信(terminal transmission type distribution)*/
	int d_band = 0;	/*端末伝送型配信 差分データ(terminal transmission type distribution)*/ 
	int all_band = 0;
	int q1_user = 0;
	int q2_user = 0;
	int q3_user = 0;
	int q4_user = 0;
	int n_high = 0;
	int k_high = 0;
	int k2_high = 0;
	int n_low = 0;
	int k_low = 0;
	int k2_low = 0;
	double all_wait_time = 0;
	double kakin2_wait_time = 0;
	double kakin_wait_time = 0;
	double b_k_avarage = 0;
	double not_wait_time = 0;
	double q1_wait_time = 0;
	double q2_wait_time = 0;
	double q3_wait_time = 0;
	double q4_wait_time = 0;
    double k2_high_play_time = 0;
    double k1_high_play_time = 0;
    double not_high_play_time = 0;
	double avarage,k2_avarage,k_avarage,n_avarage,q1_avarage,q2_avarage,q3_avarage,q4_avarage;

	terminal_t *tmp = p;
	q_cnt= ter_n / 4;
	for (i = 0; i < ter_n; i++, p++) {/*1ユーザずつ状況を取得*/
		cnt = i;
		if ((*p).break_flag != 1) {
			user++;
			all_wait_time = all_wait_time + before_wait_time[i];
			if ((*p).billing_flag == 2){
				kakin2_wait_time = kakin2_wait_time + before_wait_time[i];	/*高課金ユーザの場合*/
				k2_user = k2_user + 1;
			}
			else if ((*p).billing_flag == 1){
				kakin_wait_time = kakin_wait_time + before_wait_time[i];	/*課金ユーザの場合*/
				k_user = k_user + 1;
			}
			else{
				not_wait_time = not_wait_time + before_wait_time[i];		/*非課金ユーザの場合*/
				n_user = n_user + 1;
			}
			if (cnt/q_cnt==0){												/*0/4～1/4をユーザ1*/
				q1_wait_time = q1_wait_time + before_wait_time[i];
				q1_user = q1_user + 1;
			}
			else if (cnt/q_cnt==1){											/*1/4～2/4をユーザ2*/
				q2_wait_time = q2_wait_time + before_wait_time[i];
				q2_user = q2_user + 1;
			}
			else if (cnt/q_cnt==2){											/*2/4～3/4をユーザ3*/
				q3_wait_time = q3_wait_time + before_wait_time[i];
				q3_user = q3_user + 1;
			}
			else if (cnt/q_cnt==3){											/*3/4～4/4をユーザ4*/
				q4_wait_time = q4_wait_time + before_wait_time[i];
				q4_user = q4_user + 1;
			}
			// 配信方法の計算
			for (m = 1; m < NUM_SGM ; m++){
				if ((*p).sgm[m] == 1) {			/*放送型配信(Broadcast type distribution)*/
					b_band = b_band + 1;
					all_band = all_band + 1;
				}
				else if ((*p).sgm[m] == 3) {	/*オンデマンド型(On-demand delivery)*/
					c_band = c_band + 1;
					all_band = all_band + 1;
				}
				else if ((*p).sgm[m] == 2) {	/*端末伝送型配信(terminal transmission type distribution)*/
					s_band = s_band + 1;
					all_band = all_band + 1;
				}
			}
			//画質の計算
			for (int k = 0; k < NUM_SGM; k++) {
				if ((*p).high_sgm[k] != 0) {
					d_band++;
					if ((*p).billing_flag == 0) {
						n_high++;
					} else if ((*p).billing_flag == 1) {
						k_high++;
					} else {
						k2_high++;
					}
				} else {
					if ((*p).billing_flag == 0) {
						n_low++;
					} else if ((*p).billing_flag == 1) {
						k_low++;
					} else {
						k2_low++;
					}
				}
			}

			if ((*p).billing_flag == 0) {
				not_high_play_time = not_high_play_time + (*p).high_play_t;
			} else if ((*p).billing_flag == 1) {
				k1_high_play_time = k1_high_play_time + (*p).high_play_t;
			} else {
				k2_high_play_time = k2_high_play_time + (*p).high_play_t;
			}

		}
	}



	avarage = all_wait_time / user;			/*平均待ち時間*/
	k2_avarage = kakin2_wait_time / k2_user; /*高課金ユーザの平均待ち時間*/
	k_avarage = kakin_wait_time / k_user;	/*低課金ユーザの平均待ち時間*/
	n_avarage = not_wait_time / n_user;		/*非課金ユーザの平均待ち時間*/
	b_k_avarage = (kakin2_wait_time + kakin_wait_time) / (k2_user + k_user); /*課金ユーザの平均待ち時間*/
	q1_avarage = q1_wait_time / q1_user;	/*ユーザ1の平均待ち時間*/
	q2_avarage = q2_wait_time / q2_user;	/*2の平均待ち時間*/
	q3_avarage = q3_wait_time / q3_user;	/*3の平均待ち時間*/
	q4_avarage = q4_wait_time / q4_user;	/*4の平均待ち時間*/
	printf("b_band total : %d\nc_band total : %d\ns_band total : %d\nd_band total : %d\ntotal_band total : %d\n",b_band,c_band,s_band,d_band,all_band);
	printf("all_wait_time:%.3f  avarage_wait_time:%.3f  user:%d\n", all_wait_time,avarage,user);
	printf("kakin2_wait_time:%.3f  k2_avarage_wait_time:%.3f  k2_user:%d\n", kakin2_wait_time,k2_avarage,k2_user);
	printf("kakin1_wait_time:%.3f  k1_avarege_wait_time:%.3f  k1_user:%d\n", kakin_wait_time,k_avarage,k_user);
	printf("kakin_wait_time:%.3f  k_avarege_wait_time:%.3f  k_user:%d\n", kakin_wait_time+kakin2_wait_time,b_k_avarage,k_user+k2_user);
	printf("not_wait_time:%.3f  n_avarege_wait_time:%.3f  n_user:%d\n", not_wait_time,n_avarage,n_user);
	printf("q1:%.3f q2:%.3f q3:%.3f q4:%.3f\n",q1_avarage,q2_avarage,q3_avarage,q4_avarage);
	printf("q1:%d q2:%d q3:%d q4:%d\n",q1_user,q2_user,q3_user,q4_user);
	printf("not_high_resolution: %d, kakin_high_resolution: %d, kakin2_high_resolution: %d\n", n_high, k_high, k2_high);
	printf("not_low_resolution: %d, kakin_low_resolution: %d, kakin2_low_resolution: %d\n", n_low, k_low, k2_low);
    printf("k2_avarage_play_time: %.3f, k1_avarage_play_time: %.3f, n_avarage_play_time: %.3f\n", k2_high_play_time/k2_user, k1_high_play_time/k_user, not_high_play_time/n_user);

	// for (j = 0; j < ter_n; i++, tmp++){
	// 	if((*tmp).billing_flag == true){
	// 		printf("K");
	// 	}
	// 	else {
	// 		printf("N");
	// 	}
	// }
	// printf("\n");
}

int sgm_num_find(terminal_t *p, int ter_n) {
	int i, j, k,l;
	int seeder_req_sgm;
	int min_req_sgm = 10000;
	int req_user;
	int flag = 0;
	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).break_flag != 1) {
			seeder_req_sgm = continuing_playable_sgm(p)+1;  //?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ疲怙謫ｾ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ鯉ｿｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ[?ｿｽ?ｽｿ?ｽｽU?ｿｽ?ｽｿ?ｽｽi?ｿｽ?ｽｿ?ｽｽV?ｿｽ?ｽｿ?ｽｽ[?ｿｽ?ｽｿ?ｽｽ_?ｿｽ?ｽｿ?ｽｽj?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽv?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ?繧托ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽZ?ｿｽ?ｽｿ?ｽｽO?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽg?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ\?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ
			break;
		}
	}
	printf("max_req_sgm=%d\n", seeder_req_sgm);

	for (j = 0; j < ter_n; j++, p++) {
		if ((*p).break_flag != 1) {
			if (continuing_playable_sgm(p) < min_req_sgm) {
				min_req_sgm = continuing_playable_sgm(p)+1;//?ｿｽ?ｽｿ?ｽｽS?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ[?ｿｽ?ｽｿ?ｽｽU?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽv?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ?繧托ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽZ?ｿｽ?ｽｿ?ｽｽO?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽg?ｿｽ?ｽｿ?ｽｽ?ｾ梧慣?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ?譛繧托ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ?謫ｾ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ後そ?ｿｽ?ｽｿ?ｽｽO?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽg?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ\?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ
				req_user = j;
			}
		}
	}
	printf("min_req_sgm=%d\nreq_USER=%d\n", min_req_sgm, req_user);

	if (min_req_sgm < seeder_req_sgm) {	

		send_sgm_p2p(p, min_req_sgm, req_user);/*?ｿｽ?ｽｿ?ｽｽS?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ[?ｿｽ?ｽｿ?ｽｽU?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽv?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ?繧托ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽZ?ｿｽ?ｽｿ?ｽｽO?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽg?ｿｽ?ｽｿ?ｽｽ?ｾ梧慣?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ?譛繧托ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ?謫ｾ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ後そ?ｿｽ?ｽｿ?ｽｽO?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽg?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽV?ｿｽ?ｽｿ?ｽｽ[?ｿｽ?ｽｿ?ｽｽ_?ｿｽ?ｽｿ?ｽｽ?ｾ瑚ｶ｣?ｽｿ?ｽｽM?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ?ｾ医ｑ?ｽｿ?ｽｽ?ｿｽ?ｽｿ?ｽｽ鮠ｾ?ｿｽ*/

		//for (l = 0; l < req_user;l++) {
		//	p++;
		//}
		//		if (flag != 1) {
		//				(*p).sgm[min_req_sgm] = 1;
		//				printf("USER%dsendsgm%d\n", req_user, min_req_sgm);
		//				flag = 1;
		//		}
		//		flag = 0;
	}
}

// int sgm_num_find(terminal_t *p, int ter_n) {
	
	// int i, j, k,l;
	// int sgm = 10000;
	// int flag = 0;
	// // for (i = 0; i < ter_n; i++, p++) {
	// // 	if((*p).billing_flag != true){
	// // 		continue;
	// // 	}
	// // 	if ((*p).break_flag != 1) {
			

	// // 		seeder_req_sgm = continuing_playable_sgm(p)+1;  //?????????[?U?i?V?[?_?j???v?????????Z?O?????g??\??
	// // 		break;
	// // 	}
	// // }
	// // printf("\nmax_req_sgm=%d\nUSER:%d\n", seeder_req_sgm,i);

	// for (j = 0; j < ter_n; j++, p++) {
	// 	// if((*p).billing_flag != true){
	// 	// 	continue;
	// 	// }
	// 	if (p[j].billing_flag == true){
	// 		if (p[j].break_flag != 1) {
	// 			printf("flag:%d\nuser:%d\nreq_sgm:%d\n",p[j].billing_flag,*req_user,continuing_playable_sgm(p) + 1);

	// 			if (continuing_playable_sgm(p) + 1 < *min_req_sgm) {
	// 				*min_req_sgm = continuing_playable_sgm(p) + 1;//?S???[?U???v?????????Z?O?????g????????????Z?O?????g??\??
	// 				*req_user = j;
	// 			}
	// 		}
	// 	}
	// }

	// printf("min_req_sgm=%d\nreq_USER=%d\n", *min_req_sgm, *req_user);

	// if (min_req_sgm <= seeder_req_sgm) {	

	// 	send_sgm_p2p(&p, &min_req_sgm, &req_user,ter_n);/*?S???[?U???v?????????Z?O?????g????????????Z?O?????g???V?[?_???M?????????????*/
	// 	//for (l = 0; l < req_user;l++) {
	// 	//	p++;
	// 	//}
	// 	//		if (flag != 1) {
	// 	//				(*p).sgm[min_req_sgm] = 1;
	// 	//				printf("USER%dsendsgm%d\n", req_user, min_req_sgm);
	// 	//				flag = 1;
	// 	//		}
	// 	//		flag = 0;
	// }
// }

void print_high_sgm(terminal_t *p, int ter_n) {
	int i, j;
	for (i = 1; i <= ter_n; i++, p++) {
		if ((*p).billing_flag == 1){
			printf("%d: ", i);
			for (j = 1; j < NUM_SGM; j++){
				printf("%d, ", (*p).high_sgm[j]);
			}
			printf("\n");
		}
	}
}


int main() {
	int i;
	int j;
	int k;
	int l;
	int m = 0;
	int m2 = 0;
	int n = 0;
	int n2 = 0;
	int breakcount = 0;
	double TIME = 0;
	double TIME2 = 0;
	static char before_wait_time[5000];
	static terminal_t p[1000];
	//int frequent[2];	//2種類のチャンネルの頻度を格納
	srand(time(NULL));

	FILE *fp; // FILE型構造体
	FILE *fp2;
	//char fname[] = "testpoason離脱1分.txt";
	char fname[] = "testpoason300.txt";	//離脱用
	char fname2[] = "p-p200-2.txt"; //参加用
	static double str[500];    //離脱ユーザを入力
	static double str2[200];   //参加ユーザを入力
	static int bill[200];	   //課金状況の入力
	static int query[200];	   //要求チャネルの入力

	char line[11];
	char *check, *check2;

	printf("%s\n", fname);

	print_define_data();
	

	//-------------------------------------------------------------------------------------
	//errno_t err;
	//err = fopen_s(&fp, fname, "r"); // ファイルを開く。失敗するとNULLを返す。離脱用
	fp = fopen(fname, "r");
	//if (err != 0) {
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		return -1;                           //事前に用意したポアソン離脱データセットを配列に格納
	}
	printf("%s file open!\n", fname);
	while (fgets(line, 10, fp) != NULL) {
		str[m] = atof(line);
		//printf("%.3lf\n", str[m]);
		m++;

	}
	fclose(fp);

	//err = fopen_s(&fp2, fname2, "r"); // ファイルを開く。失敗するとNULLを返す。参加用
	fp2 = fopen(fname2, "r");
	//if (err != 0) {
	if(fp2 == NULL) {
		printf("%s file not open!\n", fname2);
		return -1;                           //事前に用意したポアソン離脱データセットを配列に格納
	}
	printf("%s file open!\n", fname2);
	
	// while (fgets(line, 10, fp2) != NULL) {
	while (m2 < 200) {
		fgets(line, 10, fp2); 
		check = NULL;
		// str2[m2] = atof(line);

		check = strtok(line,",");
		str2[m2] = atof(check);

		check2 = strtok(NULL,",");
		bill[m2] = atoi(check2);
		// printf("%d\n",bill[m2]);
		// printf("m2=%d %f: %d\n",m2, str2[m2], bill[m2]);
		m2++;

		// printf("loop %d\n",m2);
		// for (i = 0; i < m2; i++){
		// 	printf("i=%d %f: %d\n", i, str2[i], bill[i]);
		// }
	}

	// for (i = 0; i < 200; i++){
	// 	printf("i=%d %f: %d\n", i, str2[i], bill[i]);
	// }
	

	fclose(fp2);

	//init_terminal2(&p,bill);
	init_terminal3(&p,bill);
	// print_terminal_info(&p, 600);
	// init_terminal(&p);

	//-------------------------------------------------------------------------------------

	for (i = 1; i < 5000; i++) {                    /*ループ上限*/
		printf("\n\n\n\nLOOP:%d-----------------------------------------------------\n", i);



		if ((ter_n - breakcount)*calcUNICAST_BANDWIDTH > SEVER_BANDWIDTH) {  /*ユーザの通信帯域の合計がサーバの帯域を超えると，再生端末数に応じて通信帯域を問う分割*/
			printf("SEVER_BANDWIDTHisOVER!\n");
			UNICAST_BANDWIDTH = SEVER_BANDWIDTH / (ter_n - breakcount);
			printf("UNICAST_BANDWIDTH:%d\n", UNICAST_BANDWIDTH);
		}

		// print_terminal_info(&p, ter_n);                 /*全ユーザ情報表示*/

		before_wait_t_save(&p, ter_n, before_wait_time);/*現在の待ち時間を配列に保存*/


		for (j = 1; j <= G; j++) {
			add_play_t_or_wait_t_2(&p, ter_n, before_wait_time); //再生時間，待ち時間加算
			add_play_t_or_wait_t_high(&p, ter_n, before_wait_time); //高画質動画の待ち時間加算
			send_sgm(&p, set_c_return_sgm_num(p, ter_n), ter_n);   //セグメント放送
			if (set_c_return_sgm_num(p, ter_n) == 0) {	//通常画質セグメントを全て受信できたら高画質セグメントを配信
				send_high_sgm(&p, set_c_return_high_sgm_num(p, ter_n), ter_n);	//高画質セグメント
			}
			for (k = 1; k <= SN; k++){//セグメントP2P配信
				send_sgm_p2p3(&p,ter_n);
				send_high_sgm_p2p(&p,ter_n);	//差分セグメントP2P配信
				//send_high_sgm_p2p(&p,ter_n);
				// sgm_num_find(&p,ter_n);                              
			}
			
			// print_terminal_info(&p, ter_n);
			TIME = TIME + SIZE_PER_SGM / MULTICAST_BANDWIDTH;
			TIME2 = TIME2 + SIZE_PER_SGM / MULTICAST_BANDWIDTH;


			// if (TIME2 >= str[n] && breakcount < MAX_BREAK_USER) {	/*ユーザ離脱判定*/
			// 	randam_break(&p, ter_n);
			// 	breakcount++;
			// 	printf("USER BREAK\n");
			// 	n++;
			// 	TIME2 = 0;
			// }

			if (TIME >= str2[n2] && ter_n < MAX_JOIN_USER) {			/*ユーザ参加判定*/
				printf("USER JOIN\n");
				ter_n++;
				n2++;
				TIME = 0;
			}
			if (conplete_sgm(p, ter_n)) {
				break;
			}
		}



		//////////////////////////すべてデータを受信できたか/////////////////////////////////////////////////////////////////////
		if (conplete_sgm(p, ter_n)) {
			printf("\n\n\n\n\n\nFINISHLOOP:%d\n", i);
			// print_terminal_info(&p, ter_n);
			before_wait_t_save(&p, ter_n, before_wait_time);	/*現在の待ち時間を配列に保存しておく*/
			caluclate_wait_t(&p,ter_n, before_wait_time);		/*平均中断時間の計算*/
			printf("FINISHLOOP:%d", i);
			break;
		}
	}

	//print_high_sgm(p, ter_n);

	return 0;
}