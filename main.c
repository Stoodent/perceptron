#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	
	int size;
	int *input;
	double *weight;
	double bias;
	double sum;
	double avg;
	
	int output;

} *perceptron_t;

perceptron_t make_perceptron( int size );

void print_perceptron( perceptron_t p );
void run( perceptron_t p, int *input );
void learn( perceptron_t p, int desiredOutput,double learnRatio );
void save( perceptron_t p, char *filename );

int binary_step( double x );
void generate_OR( int *input, int *result );
void generate_AND( int *input, int *result );
void generate_NOT( int *input, int *result );
void generate_A( int *input, int *result,int A );






int main( int argc, char **argv ) {
	
	srand(time(NULL));
	FILE *out = fopen("points","w");
	
	perceptron_t p = make_perceptron(2);
	int i, input[2], desiredOutput;
	
	for( i=0; i<100 ; i++ ){
		
		//generate_NOT(input,&desiredOutput);
		generate_OR(input,&desiredOutput);		
		//generate_AND(input,&desiredOutput);	
		
		run(p,input);
		
		if(i<300)
		learn(p,desiredOutput,50);
		else
		learn(p,desiredOutput,0.1);


		if(p->output == desiredOutput)
			p->avg = (p->avg*i + 1)/(i+1);
		else
			p->avg = (p->avg*i)/(i+1);
		
		fprintf(out,"%d %g\n",i , p->avg*100 );
		
		print_perceptron(p);
		//printf("avg: %g%%\n",p->avg*100 );
	}
	fclose(out);
	
	system( "gnuplot -p -e \"plot 'points' \"" );
	
	save(p,"perceptron.txt");
	
	
	
	return 0;
}




void generate_A( int *input, int *result, int n ){
	
	static int r = 0;
	
	int A[12] = { 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1 };
	int R[12];
	
		
	int i;
	for(i=0;i<12;i++)
		R[i] = rand()%2;	
	
	switch(r++%n){
		
		case 0:		result[0] = 1;
					for(i=0;i<12;i++)
						input[i] = A[i];
					break;
		
		default:	result[0] = 0;
					for(i=0;i<12;i++)
						input[i] = R[i];
					break;		
		
					
	}
}

void generate_AND( int *input, int *result ){
	
	static int r = 0;
	switch(r++%4){
		case 0: input[0] = 1; input[1] = 1; result[0] = 1; break;
		case 1: input[0] = 0; input[1] = 1; result[0] = 0; break;
		case 2: input[0] = 1; input[1] = 0; result[0] = 0; break;
		case 3: input[0] = 0; input[1] = 0; result[0] = 0; break;
	}
	
}

void generate_OR( int *input, int *result ){
	
	static int r = 0;
	switch(r++%4){
		case 0: input[0] = 1; input[1] = 1; result[0] = 1; break;
		case 1: input[0] = 0; input[1] = 1; result[0] = 1; break;
		case 2: input[0] = 1; input[1] = 0; result[0] = 1; break;
		case 3: input[0] = 0; input[1] = 0; result[0] = 0; break;
	}
	
}

void generate_NOT( int *input, int *result ){
	
	static int r = 0;
	switch(r++%2){
		case 0: input[0] = 1;  result[0] = 0; break;
		case 1: input[0] = 0;  result[0] = 1; break;
	}
	
}

perceptron_t make_perceptron( int size ){

	perceptron_t p = malloc( sizeof(*p) );
	p->weight = malloc( size * sizeof(*p->weight) );	
	p->input = malloc( size * sizeof(*p->input) );	
	
	int i;
	p->size = size;
	p->avg = 0;
	p->bias =  rand()%10-5 + (double)rand()/RAND_MAX;
	for( i=0 ; i<size ; i++ ) {
		p->weight[i] = rand()%10-5 + (double)rand()/RAND_MAX;
	}
	
	return p;
}

void run( perceptron_t p, int *input ){

	int i;
	p->sum = 0;
	for( i=0 ; i<p->size ; i++ ){
		p->sum += p->weight[i]*input[i];
		p->input[i] = input[i];
	}
	p->output = binary_step( p->sum + p->bias );
}

void learn( perceptron_t p, int desiredOutput,double learnRatio ){
	
	int i;
	for( i=0 ; i<p->size ; i++ )
		p->weight[i] += learnRatio*(double)(desiredOutput - p->output)*(double)p->input[i];
		
	p->bias += learnRatio*(double)(desiredOutput - p->output);
	
}

int binary_step( double x ) {
	
	return x > 0;
}

void save( perceptron_t p, char *filename ){
	
	FILE *out = fopen( filename, "w" );
	int i;
	
	fprintf(out,"%d   %g   ",p->size,p->bias);
	for(i=0;i<p->size;i++)
		fprintf(out,"%g ",p->weight[i]);

}

void print_perceptron( perceptron_t p ){
	
	int i;
	printf("PERCEPTRON: %p\n\n",p);
	
	for( i=0 ; i<p->size ; i++ )
		printf("(%d)\t",p->input[i]);
		
	printf("\n");
	for( i=0 ; i<p->size ; i++ )
		printf("[%g]\t",p->weight[i]);
		
	printf("\n\n| s:%g + b:%g -> %g |\n",p->sum,p->bias,p->sum+p->bias);
		
	printf("\n\n    (%d)\n-------------------------------\n\n",p->output);

}
