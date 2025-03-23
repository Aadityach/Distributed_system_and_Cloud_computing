#include <stdio.h>
#include<mpi.h>
#define NUMDATA 10000

void LoadData(int data[])
{
for(int i = 0; i < NUMDATA; i++){
data[i] = 1;
}
}
int AddUp(int data[], int start, int count)
{
int sum = 0;
for(int i = start; i < start+count; i++){
sum += data[i];
}
return sum;
}
int main(int argc, char *argv[]) {
    int rank,size;
    int local_sum, global_sum;
    int data[NUMDATA];
    int elements_count, start_index;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        LoadData(data);
    }

    MPI_Bcast(data,NUMDATA, MPI_INT,0,MPI_COMM_WORLD);

    elements_count =NUMDATA/size;
    start_index = rank *elements_count;

    //size =5
    //total count =10000

    //local count = 10000/5-2000
    //0 -> 0*2000 =0
    //1-> 1*2000 =2000

    if(rank == size-1){
        elements_count +=NUMDATA%size;
    }
    local_sum = AddUp(data,start_index,elements_count);
    printf("Process %d: sum is: %d\n",rank,local_sum);

    MPI_Reduce(&local_sum, &global_sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if (rank == 0){

        printf("The total sum of data is %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}