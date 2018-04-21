
/* Enviroment : C++ */

#include<string.h>
#include<stdio.h>     
#include<stdlib.h>   
#include<time.h>     

class network   
{
private:
	float *neu, *con, *val, *err;
	int *act;
	int lv1, lv2, lv3;
	int level;
public:
	void setnet(int lv_1, int lv_2, int lv_3);
	int recall(int *input, int *output);
	int learn(int *input, int *output);
	void print(int *output);
	network(int lv1, int lv2, int lv3); 
	network(); 
	virtual ~network();
};
#define Multi 1  
#define Single 0 
network::network(int lv_1, int lv_2, int lv_3)
{
	setnet(lv_1, lv_2, lv_3);
}
network::network()
{
	setnet(0, 0, 0);
}
network::~network()
{
	delete[] neu, val, err, con, act; 
}


void network::setnet(int lv_1, int lv_2, int lv_3) 
{
	this->~network(); 
	int i;
	srand((unsigned)time(NULL));
	lv1 = lv_1; 
	lv2 = lv_2;
	lv3 = lv_3;

	if (lv2 == 0) 
	{
		level = Single;      
		con = new float[lv1*lv3];	  
	}
	else
	{
		level = Multi;	  
		con = new float[lv1*lv2 + lv2 * lv3]; 
	}

	neu = new float[lv2 + lv3];
	val = new float[lv2 + lv3];
	err = new float[lv2 + lv3];
	
	act = new int[lv1 + lv2 + lv3];
	
	for (i = 0; i<lv2 + lv3; i++)
		neu[i] = 0.5;

	if (level)
		for (i = 0; i<lv1*lv2 + lv2 * lv3; i++)
			con[i] = (float)(rand() % 100) / 100;
	else
		for (i = 0; i<lv1*lv3; i++)
			con[i] = (float)(rand() % 100) / 100;
}

#define Recall_succeed 1  
#define Recall_failed 0   

int network::recall(int *input, int *output)  
{
	int i, j, st = Recall_succeed;

	for (i = 0; i<lv2 + lv3; i++)  
		val[i] = 0;


	if (level) 
	{
		for (i = 0; i<lv1; i++)  
		{
			act[i] = input[i];

			for (j = 0; j<lv2; j++)
				val[j] += con[i*lv2 + j] * (float)act[i];
		}

		for (i = 0; i<lv2; i++)  
		{
			act[lv1 + i] = (val[i]>neu[i]);
			for (j = 0; j<lv3; j++)
				val[lv2 + j] += con[lv1*lv2 + i * lv3 + j] * (float)act[lv1 + i];
		}
	}
	else    
	{
		for (i = 0; i<lv1; i++)  
		{
			act[i] = input[i];
			for (j = 0; j<lv3; j++)
				val[j] += con[i*lv3 + j] * (float)act[i];
		}
	}


	for (i = 0; i<lv3; i++)
	{
		act[lv1 + lv2 + i] = (val[lv2 + i]>neu[lv2 + i]);
		if (act[lv1 + lv2 + i] != output[i]) st = Recall_failed;
	}

	return st;
}

const float LrnV = (float)0.07;

int network::learn(int *input, int *output)
{
	int i, j, st = recall(input, output);
	if (level) 
	{
		for (i = 0; i<lv2; i++) err[i] = 0;

		for (i = 0; i<lv3; i++)
		{
			err[lv2 + i] = (float)(output[i] - act[lv1 + lv2 + i]);

			for (j = 0; j<lv2; j++)
			{
				
				err[j] += con[lv1*lv2 + j * lv3 + i] * err[lv2 + i];

			
				con[lv1*lv2 + j * lv3 + i] += LrnV * act[lv1 + j] * err[lv2 + i];
			}
		}
		for (i = 0; i<lv2; i++)
			for (j = 0; j<lv1; j++)
				con[j*lv2 + i] += LrnV * act[j] * err[i];
	}
	else  
	{
		for (i = 0; i<lv3; i++)
		{
			err[i] = (float)(output[i] - act[lv1 + i]);

			for (j = 0; j<lv1; j++)
				con[j*lv3 + i] += LrnV * act[j] * err[i];
		}
	}

	return st; 
}


void network::print(int *output)
{
	int i;

	printf("입력 :");
	
	for (i = 0; i<lv1 + lv2 + lv3; i++) 
	{
		if (i == lv1 || i == lv1 + lv2) printf(" ");
		printf("%d", act[i]);
	}
	printf(" ");
	printf("목표값 : ");
	for (i = 0; i<lv3; i++)
		printf("%d", output[i]);

	printf("\n");
}
int lev1, lev2, lev3, num_stu;
int *input, *output;
int input_file_data();

int main()
{
	int i, j, k;

	
	network net;

	while (1) {
	
		i = input_file_data();

		
		if (i == -1) break;
		else if (i == 0) { printf("input error! \n\n"); continue; }

		
		net.setnet(lev1, lev2, lev3);


		
		for (j = 0; j<10000; j++)  
		{
		
			k = 0;

			for (i = 0; i<num_stu; i++)
				k += net.learn(input + i * lev1, output + i * lev3);
			if (k == num_stu) { 
				break; }
		}
		printf("입력층 : %d  / 은닉층 : %d  / 출력층 : %d  / 패턴갯수 : %d \n", lev1, lev2, lev3,num_stu);
		printf("학습횟수 : %d 회 \n", j);
		printf("***************************** 학습결과 ***************************** \n");
		for (i = 0; i<num_stu; i++)
		{
			net.recall(input + i * lev1, output + i * lev3);
			net.print(output + i * lev3);
		
		}

	}

	delete[] input, output;
}



int input_file_data()
{
	int i, j;
	FILE *fp;
	char filename[20]; 
	printf("파일명을 입력해주세요 :");
	scanf("%s", filename);
	if (!strcmp(filename, "exit")) return -1;
	if ((fp = fopen(filename, "r")) == NULL) return 0;
	fscanf(fp, "%d %d %d %d", &lev1, &lev2, &lev3, &num_stu);
	input = new int[lev1*num_stu];
	output = new int[lev3*num_stu];
	for (i = 0; i<num_stu; i++)
	{
		for (j = 0; j < lev1; j++) {
			fscanf(fp, "%d", &input[i*lev1 + j]);
			
			
		}
		for (j = 0; j < lev3; j++) {
			fscanf(fp, "%d", &output[i*lev3 + j]);
			
		}
	}

	fclose(fp);

	return 1;
}
