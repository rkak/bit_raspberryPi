#define NUM_OF_STU      10
#define NAME_LEN        20

struct my_msg{
        long int msgType;
        char name[NAME_LEN];
        int id;
        int kor;
        int eng;
	int math;
#ifdef MSG_RX
	int total;
	double avg;
#endif
};
