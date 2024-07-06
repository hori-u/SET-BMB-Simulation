#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MOVIE_TIME 60                           /*“®‰æŠÔ */
#define MOVIE_BITRATE 2 * 1000000               /*Ä¶ƒŒ[ƒg2Mbps */
double MOVIE_SIZE = MOVIE_TIME * MOVIE_BITRATE; /*“®‰æƒTƒCƒY=“®‰æŠÔ*Ä¶ƒŒ[ƒg*/
double SIZE_PER_SGM = 1000096;                  /*ƒuƒƒbƒNƒTƒCƒY*/
#define NUM_SGM (MOVIE_TIME*2)+1                /*ƒuƒƒbƒN”=“®‰æƒTƒCƒY/ƒuƒƒbƒNƒTƒCƒY@<“®‰æŠÔ*2>*/
//double TIME_PER_SGM = MOVIE_TIME/NUM_SGM;         
double TIME_PER_SGM = 0.5;                      /*‚PƒuƒƒbƒN‚ÌÄ¶ŠÔ“®‰æŠÔ/ƒuƒƒbƒN”*/
//double MULTICAST_PER_SGM = 0.125;             /*1ƒuƒƒbƒN‚Ì•ú‘—ŠÔƒuƒƒbƒNƒTƒCƒY/•ú‘—‘Ñˆæ*/ 
int MAX_JOIN_USER = 200;						/*Q‰Áƒ†[ƒU” ‰Û‹àƒ†[ƒU10% ”p‰Û‹àƒ†[ƒU10%*/
int MAX_BREAK_USER = 500;						/*—£’Eƒ†[ƒU”*/
int ter_n = 10;									/*ƒ†[ƒU‚Ì‰Šú’lCÅ‘å’l‚ÍMAX_JOIN_USER) */
int G = 8;                                      /*ƒXƒPƒWƒ…[ƒ‹‚·‚éƒZƒOƒƒ“ƒg”*/
int SN = 4;
int MULTICAST_BANDWIDTH = 8 * 1000000;          /*8Mbps@  •ú‘—‘Ñˆæ•*/
int UNICAST_BANDWIDTH = 1 * 1000000;            /*1Mbps    ’ÊM‘Ñˆæ•*/
int SUPER_NODE_BANDWIDTH = 8 * 1000000;			/*8Mbps    ƒX[ƒp[ƒm[ƒh‘Ñˆæ•*/
int DIFFERENCE_BANDWIDTH = 8 * 1000000; 		/*8Mbps    ’Êí‰æ¿‚Æ‚‰æ¿‚Ì·•ª*/
int calcUNICAST_BANDWIDTH = 1 * 1000000;
int SEVER_BANDWIDTH = 30 * 1000000;             /*60Mbps   ƒT[ƒo‚Ì‘Ñˆæ*/

double SWICH_TIME = 0.1;						/*ƒ`ƒƒƒ“ƒlƒ‹Ø‚è‘Ö‚¦ŠÔ(‰¼)*/



typedef struct terminal_t {
	double  play_t;	          /* ’[––‚ªÄ¶‚µ‚Ä‚¢‚é“®‰æ‚ÌÄ¶ŠÔ */
	double  test_play_t;      /*ƒVƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“—pÄ¶ŠÔ*/
	double  wait_t;           /* Ä¶‘Ò‚¿ŠÔ */
	double  test_wait_t;      /*ƒVƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“—p‘Ò‚¿ŠÔ*/
	char    sgm[NUM_SGM];	  /* óMŠ®—¹‚µ‚Ä‚¢‚éƒZƒOƒƒ“ƒg */	
	char    test_sgm[NUM_SGM];
	int     break_flag;       /*—£’Eƒtƒ‰ƒO*/
	int     finish_flag;	  /*óMŠ®—¹ƒtƒ‰ƒO*/
	int		billing_flag;	  /*‰Û‹àƒtƒ‰ƒO0-2*/
}terminal_t;                  /* “®‰æÄ¶’[––’[––‚Ì•Ûî•ñ */

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
		 return true;//1‚È‚ç‰Û‹àÒ
	}
	else return false;//”ñ‰Û‹àÒ
	
}

int init_terminal(terminal_t *p) {/*‚·‚×‚Ä‚Ì“®‰æÄ¶’[––‚Ì‰Šú‰»*/
	int i, m;
	for (i = 0; i < MAX_JOIN_USER; i++, p++) {/* ‰Šú’l0‚ğ‘ã“ü*/
		(*p).play_t = 0;
		(*p).test_play_t = 0;
		(*p).wait_t = 0;
		(*p).test_wait_t = 0;
		(*p).break_flag = 0;
		(*p).finish_flag = 0;
		
		for (m = 1; m < NUM_SGM; m++) { /*ƒZƒOƒƒ“ƒg”‚Í(NUM_SGM-1)‚Ü‚Å*/
			(*p).sgm[m] = 0;
			(*p).test_sgm[m] = 0;
		}
		(*p).billing_flag = billing_check();
	}
	return 0;
}

int init_terminal2(terminal_t *p,int *bill2) {/*‚·‚×‚Ä‚Ì“®‰æÄ¶’[––‚Ì‰Šú‰»*/
	int i, m;
	for (i = 0; i < MAX_JOIN_USER; i++, p++) {/* ‰Šú’l0‚ğ‘ã“ü*/
		(*p).play_t = 0;
		(*p).test_play_t = 0;
		(*p).wait_t = 0;
		(*p).test_wait_t = 0;
		(*p).break_flag = 0;
		(*p).finish_flag = 0;
		
		for (m = 1; m < NUM_SGM; m++) { /*ƒZƒOƒƒ“ƒg”‚Í(NUM_SGM-1)‚Ü‚Å*/
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


int init_terminal3(terminal_t *p, int *bill3) {/*‚·‚×‚Ä‚Ì“®‰æÄ¶’[––‚Ì‰Šú‰»*/
	int i, m;
	for (i = 0; i < MAX_JOIN_USER; i++, p++) {/* ‰Šú’l0‚ğ‘ã“ü*/
		(*p).play_t = 0;
		(*p).test_play_t = 0;
		(*p).wait_t = 0;
		(*p).test_wait_t = 0;
		(*p).break_flag = 0;
		(*p).finish_flag = 0;
		
		for (m = 1; m < NUM_SGM; m++) { /*ƒZƒOƒƒ“ƒg”‚Í(NUM_SGM-1)‚Ü‚Å*/
			(*p).sgm[m] = 0;
			(*p).test_sgm[m] = 0;
		}
		// (*p).billing_flag = billing_check();
		(*p).billing_flag = *bill3; /*‰Û‹àó‹µ*/
		if (i < MAX_JOIN_USER - 1)
		++bill3;
	}
	return 0;
}

int print_terminal_info(terminal_t *p, int num) {/* num‚Ü‚Å‚Ì“®‰æÄ¶’[––‚Ìî•ñ‚ğ•\¦‚·‚é*/
	int i, m;
	for (i = 0; i < num; i++, p++) {

		if ((*p).break_flag == 1) {
			printf("\n\nUSER:%dbreak\n\n", i);
		}
		else {
			printf("USER:%d\nplay_t:%.3f\nwait_t:%.3f\nPPT:%.3f\nBILLING_FLAG:%d\n"
				, i, (*p).play_t, (*p).wait_t, play_postponement_time(p),(*p).billing_flag);
			//printf("test_play_t:%.3f\ntest_wait_t:%.3f\n", (*p).test_play_t, (*p).test_wait_t);
			for (m = 1; m < NUM_SGM; m++) { /*ƒZƒOƒƒ“ƒg”(NUM_SGM-1)‚Ü‚Å*/
				if ((*p).sgm[m] == 1) {			/*•ú‘—Œ^”zM(Broadcast type distribution)*/
					printf("X");
				}
				else if ((*p).sgm[m] == 2) {	/*’[––“`‘—Œ^”zM(terminal transmission type distribution)*/
					printf("Z");
				}
				else if ((*p).sgm[m] == 3) {	/*ƒIƒ“ƒfƒ}ƒ“ƒhŒ^(On-demand delivery)*/
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

int send_sgm(terminal_t *p, int sgm_n, int ter_n) {/*Œ»İÚ‘±‚µ‚Ä‚¢‚é’[––‚ÖƒZƒOƒƒ“ƒgsgm‚ğ•ú‘—‚·‚é */
	printf("\nsend_sgm_by_multicast:%d USER:%d\n", sgm_n,ter_n);
	int i;
	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).break_flag != 1) {
			if((*p).sgm[sgm_n] == 0){
				(*p).sgm[sgm_n] = 1;	/*•ú‘—Œ^”zM(Broadcast type distribution)*/
			}
		}
	}
	return 0;
}

int send_sgm_p2p(terminal_t *p, int min_req_sgm, int req_user){ /*Ú‘±ƒ†[ƒU“à‚ÅÅ‚àŒÃ‚¢ƒZƒOƒƒ“ƒg‚ğ—v‹‚µ‚Ä‚¢‚éƒ†[ƒU‚ÉP2P‚ÅƒZƒOƒƒ“ƒg‘—M*/
	int i;
	for (i = 0; i <= req_user; i++, p++) {
		if (i = req_user) {
			(*p).sgm[min_req_sgm] = 2;
		}
	}
	printf("\nsend_sgm_by_p2p:%d USER:%d\n", min_req_sgm,req_user);
	return 0;
}

int send_sgm_p2p2(terminal_t *p, int num){ /*Ú‘±ƒ†[ƒU“à‚ÅÅ‚àŒÃ‚¢ƒZƒOƒƒ“ƒg‚ğ—v‹‚µ‚Ä‚¢‚éƒ†[ƒU‚ÉP2P‚ÅƒZƒOƒƒ“ƒg‘—M*/
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
		if ((*p).billing_flag == true) {	//‰Û‹àƒtƒ‰ƒO‚ª1‚È‚ç
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
int send_sgm_p2p3(terminal_t *p, int num){ /*Ú‘±ƒ†[ƒU“à‚ÅÅ‚àŒÃ‚¢ƒZƒOƒƒ“ƒg‚ğ—v‹‚µ‚Ä‚¢‚éƒ†[ƒU‚ÉP2P‚ÅƒZƒOƒƒ“ƒg‘—M*/
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
		if ((*p).billing_flag != 0) {	//‰Û‹àƒtƒ‰ƒO‚ª0‚Å‚È‚¢‚È‚ç
			if ((*p).break_flag != 1) {
				seeder_req_sgm = continuing_playable_sgm(p)+1;  /*Å‰‚©‚ç˜A‘±‚ÅÄ¶‰Â”\‚ÈƒZƒOƒƒ“ƒg”‚ğ‹‚ß‚é*/
				break;
			}
		}
	}

	for (j = 0; j < ter_n; j++, tmp2++) {
			// if((*p).billing_flag != true){
			// 	continue;
			// }
		if ((*tmp2).billing_flag != 0){	//‰Û‹àƒtƒ‰ƒO‚ª0‚Å‚È‚¢‚È‚ç
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
				(*tmp).sgm[min_req_sgm] = 2;	/*Ú‘±ƒ†[ƒU“à‚ÅÅ‚àŒÃ‚¢ƒZƒOƒƒ“ƒg‚ğ—v‹‚µ‚Ä‚¢‚éƒ†[ƒU‚ÉP2P‚ÅƒZƒOƒƒ“ƒg‘—M*/
				printf("send_sgm_by_p2p:%d\nreq_USER=%d\n", min_req_sgm, req_user);
				return 0;
			}
		}
	// }
	// print_terminal_info(p, req_user);
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

int continuing_playable_sgm(terminal_t *p) {       /*Å‰‚©‚ç˜A‘±‚ÅÄ¶‰Â”\‚ÈƒZƒOƒƒ“ƒg”‚ğ‹‚ß‚é*/
	int i;
	for (i = 1; i < NUM_SGM; i++) {
		if ((*p).sgm[i] == 0) {	/* óMŠ®—¹‚µ‚Ä‚¢‚È‚¢ƒZƒOƒƒ“ƒg‚ğ•Ô‚· */
			return (i - 1);
		}
	}
	return NUM_SGM - 1;/*Å‘å’l*/
}


double play_postponement_time(terminal_t *p) {         /*—]—TŠÔ‚ğŒvZ‚·‚é*/
	if ((*p).break_flag == 1 || (*p).finish_flag == 1) {
		return 10000;
	}
	return continuing_playable_sgm(p) * TIME_PER_SGM - (*p).play_t;
}


double p_i_play_t(terminal_t *p, int i) {
	int m;
	for (m = 1; m < i; m++, p++);
	return (*p).play_t;
}


double set_c_return_sgm_num(terminal_t *p, int ter_n) {/*Å¬—]—TŠÔ‚ğ‚Âƒ†[ƒU‚ğ’T‚µC‚»‚Ìƒ†[ƒU‚Ì—v‹‚µ‚Ä‚¢‚éƒZƒOƒƒ“ƒg‚ğ•Ô‚·*/
	int i;
	int min_PPT_sgm = 0;
	double eps = 1e-10;
	double p_i;
	double Movie_t = MOVIE_TIME;




	for (i = 0; i < ter_n; i++) {
		p_i = p_i_play_t(p, i);

		if (p[i].break_flag != 1 || p[i].finish_flag != 1) {

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



int add_play_t_or_wait_t(terminal_t *p, int ter_n, double before_wait_time[]) {/*Šeƒ†[ƒU‚ÉÄ¶ŠÔ‚Ü‚½‚Í‘Ò‚¿ŠÔ‚ğ‰ÁZ*/
	int i;
	int k;
	int flag = 0;
	/*1ƒ†[ƒU‚¸‚Âó‹µ‚ğæ“¾*/
	for (i = 0; i < ter_n; i++, p++) {
		if ((*p).break_flag != 1) {/*Ú‘±ó‹µ‚ÌŠm”F*/

			if ((*p).finish_flag == 1 && (*p).play_t <= MOVIE_TIME) {/*óMŠ®—¹‚©‚ÂÄ¶ŠÔ‚ª“®‰æŠÔ‚ğ‰º‰ñ‚Á‚Ä‚¢‚éê‡*/
				(*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
			}
			else if (play_postponement_time(p) >= SIZE_PER_SGM / MULTICAST_BANDWIDTH) {/*—]—TŠÔ‚ª\•ª‚É‚ ‚éê‡*/
				(*p).play_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH;
			}
			else if ((*p).play_t != MOVIE_TIME) {/*—]—TŠÔ‚ª\•ª‚É‚È‚¢‚©‚ÂÄ¶“r’†‚Å‚ ‚éê‡*/
				(*p).wait_t += SIZE_PER_SGM / MULTICAST_BANDWIDTH - play_postponement_time(p);
				(*p).play_t += play_postponement_time(p);
					if (before_wait_time[i] < 0) {  /*G‰ñƒ‹[ƒv“r’†‚Éƒ†[ƒU’Ç‰Á‚³‚ê‚½‚çC‚»‚Ìƒ†[ƒU‚Ì‘Ò‚¿ŠÔ‚ğ0‚É‚·‚éiƒ}ƒCƒiƒX‚Ì”’l‚ª“ü‚Á‚Ä‚¢‚é‚©‚çj*/
					before_wait_time[i] = 0;
					}
				if (flag != 1) {
					if ((*p).wait_t - before_wait_time[i] >= SIZE_PER_SGM / UNICAST_BANDWIDTH) {/*•ú‘—‚ğ‘Ò‚Â‚æ‚è’ÊM‚ÅóM‚µ‚½•û‚ª‘‚¢ƒ†[ƒU‚ğŒ©‚Â‚¯‚é*/
						//printf("(%d).wait_t:%.3f\n",i, (*p).wait_t);
						//printf("before_wait_time[%d]:%.3f\n", i, before_wait_time[i]);
						//printf("SIZE_PER_SGM / UNICAST_BANDWIDTH=%f\n", SIZE_PER_SGM / UNICAST_BANDWIDTH);
						printf("USER%dwaittimeover!============================================================================================\n", i);
						for (k = 1; k < NUM_SGM; k++) {                /*•ú‘—‚ğ‘Ò‚Â‚æ‚è’ÊM‚ÅóM‚µ‚½•û‚ª‘‚¢ƒ†[ƒU‚ÉƒZƒOƒƒ“ƒg‚ğ’ÊM‚Å‘—M‚·‚é*/
							if (flag != 1) {
								if ((*p).sgm[k] == 0) {
									(*p).sgm[k] = 3;	/*ƒIƒ“ƒfƒ}ƒ“ƒhŒ^*/
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


int before_wait_t_save(terminal_t *p, int ter_n, double before_wait_time[]) {  /*Œ»İ‚Ì‘Ò‚¿ŠÔ‚ğ”z—ñ‚É•Û‘¶‚µ‚Ä‚¨‚­*/
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

bool conplete_sgm(terminal_t *p, int ter_n) {   /*‚·‚×‚Ä‚Ìƒ†[ƒU‚ªƒZƒOƒƒ“ƒg‚ğóMŠ®—¹‚Å‚«‚½‚©”»’è‚·‚é*/
	int i, m;
	for (i = 1; i <= ter_n; i++, p++) {

		if ((*p).break_flag != 1) {

			for (m = 1; m < NUM_SGM; m++)
				if ((*p).sgm[m] == 0) {
					return false;
				}
			(*p).finish_flag = 1;
		}
	}

	return true;
}


double randam_break(terminal_t *p, int ter_n) {/*ƒ‰ƒ“ƒ_ƒ€‚Èƒ†[ƒU‚ğ—£’E‚³‚¹‚éi—£’Eƒtƒ‰ƒO‚ğ—§‚Ä‚éj*/
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

//double caluclate_wait_t(terminal_t *p, int ter_n, double before_wait_time[]) {/*•½‹Ï’†’fŠÔ‚ÌŒvZ*/
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

double caluclate_wait_t(terminal_t *p, int ter_n, double before_wait_time[]) {/*•½‹Ï’†’fŠÔ‚ÌŒvZ*/
	int i,j,m,cnt,q_cnt;
	int user=0;
	int k_user = 0;
	int n_user = 0;
	int b_band = 0;
	int c_band = 0;
	int s_band = 0;
	int all_band = 0;
	int q1_user = 0;
	int q2_user = 0;
	int q3_user = 0;
	int q4_user = 0;
	double all_wait_time = 0;
	double kakin_wait_time = 0;
	double not_wait_time = 0;
	double q1_wait_time = 0;
	double q2_wait_time = 0;
	double q3_wait_time = 0;
	double q4_wait_time = 0;
	double avarage,k_avarage,n_avarage,q1_avarage,q2_avarage,q3_avarage,q4_avarage;

	terminal_t *tmp = p;
	q_cnt= ter_n / 4;
	for (i = 0; i < ter_n; i++, p++) {/*1ƒ†[ƒU‚¸‚Âó‹µ‚ğæ“¾*/
		cnt = i;
		if ((*p).break_flag != 1) {
			user++;
			all_wait_time = all_wait_time + before_wait_time[i];
			if ((*p).billing_flag == 1){
				kakin_wait_time = kakin_wait_time + before_wait_time[i];	/*‰Û‹àƒ†[ƒU‚Ìê‡*/
				k_user = k_user + 1;
			}
			else{
				not_wait_time = not_wait_time + before_wait_time[i];		/*”ñ‰Û‹àƒ†[ƒU‚Ìê‡*/
				n_user = n_user + 1;
			}
			if (cnt/q_cnt==0){												/*0/4`1/4‚ğƒ†[ƒU1*/
				q1_wait_time = q1_wait_time + before_wait_time[i];
				q1_user = q1_user + 1;
			}
			else if (cnt/q_cnt==1){											/*1/4`2/4‚ğƒ†[ƒU2*/
				q2_wait_time = q2_wait_time + before_wait_time[i];
				q2_user = q2_user + 1;
			}
			else if (cnt/q_cnt==2){											/*2/4`3/4‚ğƒ†[ƒU3*/
				q3_wait_time = q3_wait_time + before_wait_time[i];
				q3_user = q3_user + 1;
			}
			else if (cnt/q_cnt==3){											/*3/4`4/4‚ğƒ†[ƒU4*/
				q4_wait_time = q4_wait_time + before_wait_time[i];
				q4_user = q4_user + 1;
			}
			// ”zM•û–@‚ÌŒvZ
			for (m = 1; m < NUM_SGM ; m++){
				if ((*p).sgm[m] == 1) {			/*•ú‘—Œ^”zM(Broadcast type distribution)*/
					b_band = b_band + 1;
					all_band = all_band + 1;
				}
				else if ((*p).sgm[m] == 3) {	/*ƒIƒ“ƒfƒ}ƒ“ƒhŒ^(On-demand delivery)*/
					c_band = c_band + 1;
					all_band = all_band + 1;
				}
				else if ((*p).sgm[m] == 2) {	/*’[––“`‘—Œ^”zM(terminal transmission type distribution)*/
					s_band = s_band + 1;
					all_band = all_band + 1;
				}
			}
			

		}
	}

	avarage = all_wait_time / user;			/*•½‹Ï‘Ò‚¿ŠÔ*/
	k_avarage = kakin_wait_time / k_user;	/*‰Û‹àƒ†[ƒU‚Ì•½‹Ï‘Ò‚¿ŠÔ*/
	n_avarage = not_wait_time / n_user;		/*”ñ‰Û‹àƒ†[ƒU‚Ì•½‹Ï‘Ò‚¿ŠÔ*/
	q1_avarage = q1_wait_time / q1_user;	/*ƒ†[ƒU1‚Ì•½‹Ï‘Ò‚¿ŠÔ*/
	q2_avarage = q2_wait_time / q2_user;	/*2‚Ì•½‹Ï‘Ò‚¿ŠÔ*/
	q3_avarage = q3_wait_time / q3_user;	/*3‚Ì•½‹Ï‘Ò‚¿ŠÔ*/
	q4_avarage = q4_wait_time / q4_user;	/*4‚Ì•½‹Ï‘Ò‚¿ŠÔ*/
	printf("b_band total : %d\nc_band total : %d\ns_band total : %d\ntotal_band total : %d\n",b_band,c_band,s_band,all_band);
	printf("all_wait_time:%.3f  avarage_wait_time:%.3f  user:%d\n", all_wait_time,avarage,user);
	printf("kakin_wait_time:%.3f  k_avarege_wait_time:%.3f  k_user:%d\n", kakin_wait_time,k_avarage,k_user);
	printf("not_wait_time:%.3f  n_avarege_wait_time:%.3f  n_user:%d\n", not_wait_time,n_avarage,n_user);
	printf("q1:%.3f q2:%.3f q3:%.3f q4:%.3f\n",q1_avarage,q2_avarage,q3_avarage,q4_avarage);
	printf("q1:%d q2:%d q3:%d q4:%d\n",q1_user,q2_user,q3_user,q4_user);

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
			seeder_req_sgm = continuing_playable_sgm(p)+1;  //?¿½?½¿?½½?¿½?½¿?½½?¾”æœ€æ“¾?½¿?½½?¿½?½¿?½½?¾Œï¿½?¿½?½¿?½½?¿½?½¿?½½[?¿½?½¿?½½U?¿½?½¿?½½i?¿½?½¿?½½V?¿½?½¿?½½[?¿½?½¿?½½_?¿½?½¿?½½j?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½v?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¾?ã‚‘ï½¿?½½?¿½?½¿?½½?¿½?½¿?½½Z?¿½?½¿?½½O?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½g?¿½?½¿?½½?¿½?½¿?½½\?¿½?½¿?½½?¿½?½¿?½½
			break;
		}
	}
	printf("max_req_sgm=%d\n", seeder_req_sgm);

	for (j = 0; j < ter_n; j++, p++) {
		if ((*p).break_flag != 1) {
			if (continuing_playable_sgm(p) < min_req_sgm) {
				min_req_sgm = continuing_playable_sgm(p)+1;//?¿½?½¿?½½S?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½[?¿½?½¿?½½U?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½v?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¾?ã‚‘ï½¿?½½?¿½?½¿?½½?¿½?½¿?½½Z?¿½?½¿?½½O?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½g?¿½?½¿?½½?¾ŒæŠµ?½¿?½½?¿½?½¿?½½?¾?æœ€ã‚‘ï½¿?½½?¿½?½¿?½½?¾?æ“¾?½¿?½½?¿½?½¿?½½?¾Œã‚»?¿½?½¿?½½O?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½g?¿½?½¿?½½?¿½?½¿?½½\?¿½?½¿?½½?¿½?½¿?½½
				req_user = j;
			}
		}
	}
	printf("min_req_sgm=%d\nreq_USER=%d\n", min_req_sgm, req_user);

	if (min_req_sgm < seeder_req_sgm) {	

		send_sgm_p2p(p, min_req_sgm, req_user);/*?¿½?½¿?½½S?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½[?¿½?½¿?½½U?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½v?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¾?ã‚‘ï½¿?½½?¿½?½¿?½½?¿½?½¿?½½Z?¿½?½¿?½½O?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½g?¿½?½¿?½½?¾ŒæŠµ?½¿?½½?¿½?½¿?½½?¾?æœ€ã‚‘ï½¿?½½?¿½?½¿?½½?¾?æ“¾?½¿?½½?¿½?½¿?½½?¾Œã‚»?¿½?½¿?½½O?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½g?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½V?¿½?½¿?½½[?¿½?½¿?½½_?¿½?½¿?½½?¾Œè¶£?½¿?½½M?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¿½?½¿?½½?¾ˆã‚‘?½¿?½½?¿½?½¿?½½é¼¾?¿½*/

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
	//int frequent[2];	//2í—Ş‚Ìƒ`ƒƒƒ“ƒlƒ‹‚Ì•p“x‚ğŠi”[
	srand(time(NULL));

	FILE *fp; // FILEŒ^\‘¢‘Ì
	FILE *fp2;
	char fname[] = "testpoason—£’E1•ª.txt";  //—£’E—p
	char fname2[] = "p-p200-2.txt"; //Q‰Á—p
	static double str[500];    //—£’Eƒ†[ƒU‚ğ“ü—Í
	static double str2[200];   //Q‰Áƒ†[ƒU‚ğ“ü—Í
	static int bill[200];	   //‰Û‹àó‹µ‚Ì“ü—Í
	static int query[200];	   //—v‹ƒ`ƒƒƒlƒ‹‚Ì“ü—Í

	char line[11];
	char *check, *check2;

	printf("%s\n", fname);

	print_define_data();
	

	//-------------------------------------------------------------------------------------
	//errno_t err;
	//err = fopen_s(&fp, fname, "r"); // ƒtƒ@ƒCƒ‹‚ğŠJ‚­B¸”s‚·‚é‚ÆNULL‚ğ•Ô‚·B—£’E—p
	fp = fopen(fname, "r");
	//if (err != 0) {
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		return -1;                           //–‘O‚É—pˆÓ‚µ‚½ƒ|ƒAƒ\ƒ“—£’Eƒf[ƒ^ƒZƒbƒg‚ğ”z—ñ‚ÉŠi”[
	}
	printf("%s file open!\n", fname);
	while (fgets(line, 10, fp) != NULL) {
		str[m] = atof(line);
		//printf("%.3lf\n", str[m]);
		m++;

	}
	fclose(fp);

	//err = fopen_s(&fp2, fname2, "r"); // ƒtƒ@ƒCƒ‹‚ğŠJ‚­B¸”s‚·‚é‚ÆNULL‚ğ•Ô‚·BQ‰Á—p
	fp2 = fopen(fname2, "r");
	//if (err != 0) {
	if(fp2 == NULL) {
		printf("%s file not open!\n", fname2);
		return -1;                           //–‘O‚É—pˆÓ‚µ‚½ƒ|ƒAƒ\ƒ“—£’Eƒf[ƒ^ƒZƒbƒg‚ğ”z—ñ‚ÉŠi”[
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

	for (i = 1; i < 5000; i++) {                    /*ƒ‹[ƒvãŒÀ*/
		printf("\n\n\n\nLOOP:%d-----------------------------------------------------\n", i);



		if ((ter_n - breakcount)*calcUNICAST_BANDWIDTH > SEVER_BANDWIDTH) {  /*ƒ†[ƒU‚Ì’ÊM‘Ñˆæ‚Ì‡Œv‚ªƒT[ƒo‚Ì‘Ñˆæ‚ğ’´‚¦‚é‚ÆCÄ¶’[––”‚É‰‚¶‚Ä’ÊM‘Ñˆæ‚ğ–â‚¤•ªŠ„*/
			printf("SEVER_BANDWIDTHisOVER!\n");
			UNICAST_BANDWIDTH = SEVER_BANDWIDTH / (ter_n - breakcount);
			printf("UNICAST_BANDWIDTH:%d\n", UNICAST_BANDWIDTH);
		}

		// print_terminal_info(&p, ter_n);                 /*‘Sƒ†[ƒUî•ñ•\¦*/

		before_wait_t_save(&p, ter_n, before_wait_time);/*Œ»İ‚Ì‘Ò‚¿ŠÔ‚ğ”z—ñ‚É•Û‘¶*/


		for (j = 1; j <= G; j++) {
			add_play_t_or_wait_t(&p, ter_n, before_wait_time); //Ä¶ŠÔC‘Ò‚¿ŠÔ‰ÁZ
			send_sgm(&p, set_c_return_sgm_num(p, ter_n), ter_n);   //ƒZƒOƒƒ“ƒg•ú‘—
			for (k = 1; k <= SN; k++){//ƒZƒOƒƒ“ƒgP2P”zM
				send_sgm_p2p2(&p,ter_n);
				// sgm_num_find(&p,ter_n);                              
			}
			
			// print_terminal_info(&p, ter_n);
			TIME = TIME + SIZE_PER_SGM / MULTICAST_BANDWIDTH;
			TIME2 = TIME2 + SIZE_PER_SGM / MULTICAST_BANDWIDTH;


			// if (TIME2 >= str[n] && breakcount < MAX_BREAK_USER) {	/*ƒ†[ƒU—£’E”»’è*/
			// 	randam_break(&p, ter_n);
			// 	breakcount++;
			// 	printf("USER BREAK\n");
			// 	n++;
			// 	TIME2 = 0;
			// }

			if (TIME >= str2[n2] && ter_n < MAX_JOIN_USER) {			/*ƒ†[ƒUQ‰Á”»’è*/
				printf("USER JOIN\n");
				ter_n++;
				n2++;
				TIME = 0;
			}
			if (conplete_sgm(p, ter_n)) {
				break;
			}
		}



		//////////////////////////‚·‚×‚Äƒf[ƒ^‚ğóM‚Å‚«‚½‚©/////////////////////////////////////////////////////////////////////
		if (conplete_sgm(p, ter_n)) {
			printf("\n\n\n\n\n\nFINISHLOOP:%d\n", i);
			// print_terminal_info(&p, ter_n);
			before_wait_t_save(&p, ter_n, before_wait_time);
			caluclate_wait_t(&p,ter_n, before_wait_time);
			printf("FINISHLOOP:%d", i);
			break;
		}
	}
	return 0;
}