#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Heap MAX配列
#define MAX_ELEMENTS 100000000
// 繰り返し回数
#define Loop 1000
// 平均化定数
#define AVE 10

// 変数宣言
long long int MAX = 0;
int *a;
int *a2;
clock_t start, end;
double data[Loop];
int Add;
int b[ MAX_ELEMENTS ];

// min ~ max までのランダムな要素を返す関数
int GetRandom( int min, int max ) {
	static int flag;
	if ( flag == 0 ) {
		srand((unsigned int) time(NULL));
		flag=1;
	}
	return min + (int)( rand() * ( max - min + 1.0 ) / ( 1.0 + RAND_MAX ) );
}

// 必要な要素数が入る配列のメモリを確保する→必要な要素数が入る配列を取得
void array_init( int **a, long long int MAX ) {
	*a = (int *)malloc(sizeof(int)*MAX);
}

// ヒープソート以外のときに、配列の各要素にランダムな要素を代入する関数
void array_random( int *a ) {
	for ( int i = 0; i < MAX; i++ ) a[i] = GetRandom( 1, 1000 );
}

// ヒープソートのときに、配列の各要素にランダムな要素を代入する関数
void h_array_random( int *a ) {
	for ( int i = 1; i <= MAX; i++ ) a[i] = GetRandom( 1, 1000 );
}

// 二つの要素を入れ替える関数
void swap(int a[],int i, int j){
	int t=a[i];
	a[i]=a[j];
	a[j]=t;
}

// バブルソート
void bubble_sort ( int a[], int n ) {
	for ( int i= 0; i < n -1; i++ ) {
		for ( int j = n - 1; j > i; j-- ) {
			if ( a[ j - 1 ] > a[ j ] ) swap( a, j, j-1 );
		}
	}
}

// 選択ソート
void selection_sort ( int a[], int n ) {
	int lowest, lowkey;
	for ( int i = 0; i < n - 1; i++ ) {
		lowest = i;
		lowkey = a[i];
		for ( int j = i + 1; j < n; j++ ) {
			if ( a[j] < lowkey ) {
				lowest = j;
				lowkey = a[j];
			}
		}
		swap( a, i, lowest );
	}
}

// 挿入ソート
void insertion_sort ( int a[], int n ) {
	for ( int i = 1; i < n; i++ ) {
		int j = i;
		while ( j >= 1 && a[ j - 1 ] > a[j] ) {
			swap( a, j, j - 1 );
			j--;
		}
	}
}

// シェルソート
void shell_sort ( int a[], int n ) {
	int h;
	for ( h = 1; h < n / 9; h = h * 3 + 1 );
	for ( ; h > 0; h /= 3 ){
		for ( int i = h; i < n; i++ ) {
			int j = i;
			while ( j >= h && a[ j - h ] > a[ j ] ) {
				swap( a, j, j - h);
				j -= h;
			}
		}
	}
}

// パーティション
int partition(int a[], int l, int r){
	int i=l-1, j=r, pivot=a[r];
	for(;;){
		while(a[++i] < pivot);
		while(i < --j && pivot < a[j]);
		if (i >= j) break;
		swap(a,i,j);
	}
	swap(a,i,r);
	return i;
}

// クイックソートの内部関数
void quick_sort_l(int a[], int l, int r){
	if (l>=r) return;
	int v = partition(a, l, r);
	quick_sort_l(a, l, v-1);
	quick_sort_l(a, v+1, r);
}

// クイックソート
void quick_sort(int a[]){
	quick_sort_l(a, 0, MAX-1);
}

// マージソート(配列版)
void merge_sort_array( int a[], int low, int high ) {
	int i, j;
	if ( low >= high ) return;
	int mid = ( low + high ) / 2;
	merge_sort_array( a, low, mid );
	merge_sort_array( a, mid + 1, high );
	for ( i = low; i <= mid; i++ ) b[ i ] = a[ i ];
	for ( i = mid + 1, j = high; i <= high; i++, j-- ) b[ i ] = a[ j ];
	i = low;
	j = high;
	for ( int k = low; k <= high; k++ ) {
		if ( b[ i ] <= b[ j ] ) a[ k ] = b[ i++ ];
		else a[ k ] = b[ j-- ];
	}
}

// 逆ヒープ
void downheap( int from, int to ) {
	int val = a2[ from ];
	int i = from;
	while ( i <= to / 2 ) {
		int j = i * 2;
		if ( j + 1 <= to && a2[ j ] > a2[ j + 1 ] ) j++;
		if ( val <= a2[ j ] ) break;
		a2[ i ] = a2[ j ];
		i = j;
	}
	a2[ i ] = val;
}

// ヒープソート
void heapsort() {
	int n = MAX;
	for ( int i = n / 2; i >= 1; i-- ) downheap( i, n );
	for ( int i = n; i >= 2; i-- ) {
		swap( a2, 1, i );
		downheap( 1, i - 1 );
	}
}

// 出力関数
void print( char *s,int *a ){
	puts("");
	puts( s );
	for ( int i = 0; i < MAX; i++ ) printf( "%d\t", a[i] );
	puts("");
}

// ヒープ用出力関数
void h_print( char *s,int *a ){
	puts("");
	puts( s );
	for ( int i = MAX; i >= 1; i--) printf( "%d\t", a[i] );
	puts("");
}

// gnuplot用プロット関数
void plot(double *data, int count){
	FILE *gp;
	int x=Loop*Add;
	int loop=Loop;

	gp = popen("gnuplot -persist", "w");
	fprintf(gp, "set terminal png\n");
	fprintf(gp, "set output '%d.png'\n",count);
	fprintf(gp, "set multiplot\n");
	fprintf(gp, "set ylabel \"Processing time t [s]\"\n");
	fprintf(gp, "plot '-' with lines linetype 1\n");
	for(int i=0;i<Loop;i++){
		fprintf(gp, "%d\t%lf\n", i*Add, data[i]);
	}
	fprintf(gp, "e\n");
	fprintf(gp, "set nomultiplot\n");
	fprintf(gp, "exit\n");
	fflush(gp);
	pclose(gp);
}

void time_count(double *data, int count){
	double *data2;
	int *tmp;
	
	data2 = (double *)malloc(sizeof(double)*AVE);

	for (int i = 0; i < Loop; i++, MAX+=Add){
		array_init( &a , MAX );
		array_init( &tmp, MAX );

		array_random( tmp );

		for(int j=0; j < AVE; j++){
			memcpy(a, tmp, sizeof(tmp));

			start = clock();
			switch(count){
				case 0:
					bubble_sort ( a, MAX );
					break;
				case 1:
					selection_sort ( a, MAX );
					break;
				case 2:
					insertion_sort ( a, MAX );
					break;
				case 3:
					shell_sort ( a, MAX );
					break;
				case 4:
					quick_sort( a );
					break;
				case 5:
					merge_sort_array( a, 0, MAX - 1 );
					break;
				case 6:
					break;
			}
			end = clock();
			data2[j] = (double)(end-start)/CLOCKS_PER_SEC;
		}
		
		for (int j=0; j < AVE; j++){
			data[i] += data2[j];
		}
		
		data[i] /= AVE;

		if(i)printf("\033[0G\033[2K%d : %lld\t%3.1lf %%\t%lf s\t", count, MAX, ((double)i/(double)Loop)*100, data[i]);
		free(tmp);
		free(a);
	}
}

// バブルソートのプロット関数
void bubble_plot(){
	int count = 0;
	
	time_count(data, count);

	plot(data, count);
}

// 選択ソートのプロット関数
void selection_plot(){
	int count = 1;
	
	time_count(data, count);

	plot(data, count);
}

// 挿入ソートのプロット関数
void insertion_plot(){
	int count = 2;
	
	time_count(data, count);

	plot(data, count);
}

// シェルソートのプロット関数
void shell_plot(){
	int count = 3;
	
	time_count(data, count);

	plot(data, count);
}

// クイックソートのプロット関数
void quick_plot(){
	int count = 4;
	
	time_count(data, count);

	plot(data, count);
}

// マージソートのプロット関数
void merge_plot(){
	int count = 5;
	
	time_count(data, count);

	plot(data, count);
}

// ヒープソートのプロット関数
void heap_plot(){
	int count = 6;
	for (int i = 0; i< Loop; i++, MAX+=Add){
		array_init( &a2 , MAX + 1);

		h_array_random( a2 );
		start = clock();
		heapsort();
		end = clock();

		data[i] = (double)(end-start)/CLOCKS_PER_SEC;
		if(i)printf("\033[0G\033[2K%d : %lld\t%3.1lf %%\t%lf s\t", count, MAX, ((double)i/(double)Loop)*100, data[i]);
		free(a2);
	}
	plot(data, count);
}

int main(int argc, char *argv[]){
	int element, arg;
	

	if(argc != 3) {
		fprintf(stderr, "Error\n");
		exit(1);
	}

	arg = atoi(argv[1]);
	element = atoi(argv[2]);

	Add = element / Loop;

	printf("\033[2J");
	switch(arg){
		case 0:
			bubble_plot();
			break;
		case 1:
			selection_plot();
			break;
		case 2:
			insertion_plot();
			break;
		case 3:
			shell_plot();
			break;
		case 4:
			quick_plot();
			break;
		case 5:
			merge_plot();
			break;
		case 6:
			heap_plot();
			break;
		default:
			bubble_plot();
			insertion_plot();
			selection_plot();
			shell_plot();
			quick_plot();
			merge_plot();
			heap_plot();
	}

	return 0;
}
