/*Amirhossein Jafarzadeh Ghazi
Student number: 100745718
HPC assignment:
This code is for zombie land problem
considering intractions between individuals and
partitioning the rectangular field in order to parallelization*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <sys/time.h>
#define M 10
#define N 10
int n_s=20, n_z=2, n_f_s=20, n_f_z=2; /*fixing the number of
zombies and susceptible individuals*/
#define default 1000000
void initialization(int n_s, int n_z);
void assign_dimensions(int index, int i);
void assign_state(int n_s, int n_z);
void movement(int index, int i, int n_s, int n_z);
void print_individuals(int n_s, int n_z);
void interaction(int n_s, int n_z);
void partition(int i, int n_thread, int n_s, int n_z);
typedef struct PTnode {
  int i_dim;
  int j_dim;
  char state;
  int index;
  int partition;
} PTnode;
PTnode P[default];

int main() {
  srand(time(NULL));
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  int n_thread=4;/*fixing the number of threads, but it can be easily changed(it must be 1,2, or 4*/
  int i,j,r,k,t;
  int T_max=10;/*fixing the number of steps, but it can be easily changed*/
  initialization(n_s, n_z);
  for(i=0;i<n_s+n_z;i++)
    {
      partition(i, n_thread, n_s, n_z);
    }
  for(i=0;i<n_s+n_z;i++)
    {
      assign_dimensions(P[i].index,i);
    }
  assign_state(n_s,n_z);
  printf("\n********************************|FIRST STEP|********************************\n\n");
  printf("At the first step, there are %d Zombie(s)and %d Susceptible individual(s).\n\nYou can see characteristics of each individual below:\n\n", n_z, n_s);
  print_individuals(n_s,n_z);
  for(t=0;t<T_max;t++) /*We move individuals T times in order of partition, from partition one to four depending on number of threds*/
    {
    for(i=0;i<n_s+n_z;i++)
      {
        /*first if finds individuals in the first partition and moves them*/
        if(P[i].partition==1 && (n_thread==1 || n_thread==2 || n_thread==4))
          {
          movement(P[i].index, i, n_s, n_z);
          partition(i, n_thread , n_s, n_z);
          }
        /*second if finds individuals in the second partition and moves them*/
        if(P[i].partition==2 && (n_thread==2 || n_thread==4))
          {
          movement(P[i].index, i, n_s, n_z);
          partition(i, n_thread , n_s, n_z);
          }
        /*third if finds individuals in the third partition and moves them*/
        if(P[i].partition==3 && n_thread==4)
          {
          movement(P[i].index, i, n_s, n_z);
          partition(i, n_thread , n_s, n_z);
          }
        /*fourth if finds individuals in the fourth partition and moves them*/
        if(P[i].partition==4 && n_thread==4)
          {
          movement(P[i].index, i, n_s, n_z);
          partition(i, n_thread , n_s, n_z);
          }
      }
      interaction(n_s, n_z);/*after each step we should consider interactions between individuals*/
    }
  for(i=0;i<n_s+n_z;i++)
    {
    assign_dimensions(P[i].index,i);
    }
  printf("\n********************************|FINAL STEP|********************************\n\n");
   printf("At the final step, there are %d Zombie(s) and %d Susceptible individual(s).\n\nYou can see characteristics of each individual below:\n\n", n_f_z, n_f_s);
  print_individuals(n_s,n_z);
  gettimeofday(&stop, NULL);
  printf("\n                *********************************************\n                |It took %lu micro seconds to run this code|\n                *********************************************\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
return 0;
}
void initialization(int n_s, int n_z){
  int i, j, k, r, l;
  P[0].index=(rand()%(M*N))+1;
  for(i=1;i<n_s+n_z;i++)
    {
    int s=0;
    r=(rand()%(M*N-i))+1;
    for(j=0;j<i;j++)
      {
      if(r<P[j].index)
        {s=1;
        break;}
      else
        r=r+1;
      }
    if(s==1)
      {for(k=i;k>j;k--)
        P[k].index=P[k-1].index;}
    P[j].index=r;
    }
}
void movement(int index, int i, int n_s, int n_z){
  int m, s=0, L=1, R=1, D=1, U=1, v, L_s=0, R_s=0, U_s=0, D_s=0;
  while(L_s==0 || R_s==0 || U_s==0 || D_s==0)/*If there is a place to go for individual number i, it must move and cannot stay. This movement is randomly*/
    {
    m=rand()%4; /*choose one direction(left, right, up, or down) randomly*/
    if(m==0)/*move to left*/
      {
      L_s=1;
      if(index%N!=1)
        {
          for(v=0;v<n_s+n_z;v++)
          {
            if(P[v].index==index-1)/*check if the left lattice site is empty*/
            {
              L=0;
              break;
            }
          }
           if(L==1)
            {
            P[i].index=index-1;/*move*/
            break;
            }
        }
      }
    if(m==1)/*move to right*/
      {
      R_s=1;
      if(index%N!=0)
        {
          for(v=0;v<n_s+n_z;v++)
          {
            if(P[v].index==index+1)/*check if the right lattice site is empty*/
            {
              R=0;
              break;
            }
          }
          if(R==1)
          {
            P[i].index=index+1;/*move*/
            break;
          }
        }
      }
    if(m==2)/*move upwards*/
      {
      U_s=1;
      if(index/N!=0 && index!=N)
        {
        for(v=0;v<n_s+n_z;v++)
          {
            if(P[v].index==index-N)/*check if the upper lattice site is empty*/
            {
              U=0;
              break;
            }
          }
         if(U==1)
          {
            P[i].index=index-N;/*move*/
            break;
          }
        }
      }
    if(m==3)/*move downwards*/
      {
      D_s=1;
      if(index/N!=M-1 && index!=M*N)
        {
        for(v=0;v<n_s+n_z;v++)
          {
            if(P[v].index==index+N)/*check if the lower lattice site is empty*/
            {
              D=0;
              break;
            }
          }
         if(D==1)
          {
            P[i].index=index+N;/*move*/
            break;
          }
      }
    }
  }
}
void assign_dimensions(int index, int i){
    if(index%N!=0)
      {
      P[i].i_dim=1+(index/N);
      P[i].j_dim=index%N;
      }
    else
      {
      P[i].i_dim=index/N;
      P[i].j_dim=N;
      }
}
void assign_state(int n_s, int n_z){
  int i, k;
  for(i=0;i<n_s+n_z;i++)
      P[i].state='U';
  for(i=0;i<n_s;i++)
    {
      k=rand()%(n_s+n_z);
      if(P[k].state=='U')
        P[k].state='S';
      else
        i--;
    }
  for(i=0;i<n_s+n_z;i++)
    {
      if(P[i].state=='U')
        P[i].state='Z';
    }
}
void interaction(int n_s, int n_z){
int k ,v;
for(k=0;k<n_s+n_z;k++)
  {
  for(v=0;v<n_s+n_z;v++)
        {
          if(P[k].index%N!=0 && P[k].state=='S')
          {
            if(P[k].index+1==P[v].index)/*check if the right lattice site is occupied*/
            {
              if(P[v].state=='Z')/*check if the right lattice site is zombie*/
              {
              P[k].state='Z';
              n_f_z++;
              n_f_s--;
              break;   
              }   
            }
          }
          if(P[k].index%N!=1 && P[k].state=='S')
          {
            if(P[k].index-1==P[v].index)/*check if the left lattice site is occupied*/
            {
              if(P[v].state=='Z')/*check if the left lattice site is zombie*/
              {
              P[k].state='Z';
              n_f_z++;
              n_f_s--;
              break;   
              }   
            }
          }
          if(P[k].index/N!=M-1 && P[k].index!=M*N && P[k].state=='S')
          {
            if(P[k].index+N==P[v].index)/*check if the lower lattice site is occupied*/
            {
              if(P[v].state=='Z')/*check if the lower lattice site is zombie*/
              {
              P[k].state='Z';
              n_f_z++;
              n_f_s--;
              break;   
              }   
            }
          }
          if(P[k].index/N!=0 && P[k].index!=N && P[k].state=='S')
          {
            if(P[k].index-N==P[v].index)/*check if the upper lattice site is occupied*/
            {
              if(P[v].state=='Z')/*check if the upper lattice site is zombie*/
              {
              P[k].state='Z';
              n_f_z++;
              n_f_s--;
              break;   
              }   
            }
          }
        }
  }       
}
void partition(int i, int threads, int n_s, int n_z){
int t;
  /*partitioning for 1 number of threads*/
  if(threads==1)
    P[i].partition=1;
  /*partitioning for 2 number of threads*/
  if(threads==2 && M%2==0)
    {
    if(M<2)
    {
    printf("Your rectangular field is too small to use this number of threads\n");
    }
    if(P[i].index>0 && P[i].index<=M*N/2 && M>1)
      P[i].partition=1;
    if(P[i].index>M*N/2 && P[i].index<=M*N && M>1)
      P[i].partition=2;
    }
  if(threads==2 && M%2!=0)
    {
    if(M<2)
    {
    printf("Your rectangular field is too small to use this number of threads\n");
    }
    if(P[i].index>0 && P[i].index<=(1+M/2)*N && M>1)
      P[i].partition=1;
    if(P[i].index>(1+M/2)*N && P[i].index<=M*N && M>1)
      P[i].partition=2;
    }
  /*partitioning for 4 number of threads*/
  if(threads==4 && M%4==0)
   {
    if(M<4)
    {
    printf("Your rectangular field is too small to use this number of threads\n");
    }
    if(P[i].index>0 && P[i].index<=M*N/4 && M>3)
      P[i].partition=1;
    if(P[i].index>M*N/4 && P[i].index<=2*(M*N/4) && M>3)
      P[i].partition=2;
    if(P[i].index>2*(M*N/4) && P[i].index<=3*(M*N/4) && M>3)
      P[i].partition=3;
    if(P[i].index>3*(M*N/4) && P[i].index<=M*N && M>3)
      P[i].partition=4;
    } 
  if(threads==4 && M%4==1)
    {
    if(M<4)
    {
    printf("Your rectangular field is too small to use this number of threads\n");
    }
    if(P[i].index>0 && P[i].index<=(1+M/4)*N && M>3)
      P[i].partition=1;
    if(P[i].index>(1+M/4)*N && P[i].index<=(1+M/4)*N+(M/4)*N && M>3)
      P[i].partition=2;
    if(P[i].index>(1+M/4)*N+M/4*N && P[i].index<=(1+M/4)*N+(2*M/4)*N && M>3)
      P[i].partition=3;
    if(P[i].index>(1+M/4)*N+(2*M/4)*N && P[i].index<=M*N && M>3)
      P[i].partition=4;
    }
  if(threads==4 && M%4==2)
    {
    if(M<4)
    {
    printf("Your rectangular field is too small to use this number of threads\n");
    }
    if(P[i].index>0 && P[i].index<=(1+M/4)*N && M>3)
      P[i].partition=1;
    if(P[i].index>(1+M/4)*N && P[i].index<=2*(1+M/4)*N && M>3)
      P[i].partition=2;
    if(P[i].index>2*(1+M/4)*N && P[i].index<=2*(1+M/4)*N+(M/4)*N && M>3)
      P[i].partition=3;
    if(P[i].index>2*(1+M/4)*N+(M/4)*N && P[i].index<=M*N && M>3)
      P[i].partition=4;
    }
  if(threads==4 && M%4==3)
    {
    if(M<4)
    {
    printf("Your rectangular field is too small to use this number of threads\n");
    }
    if(P[i].index>0 && P[i].index<=(1+M/4)*N && M>3)
      P[i].partition=1;
    if(P[i].index>(1+M/4)*N && P[i].index<=2*(1+M/4)*N && M>3)
      P[i].partition=2;
    if(P[i].index>2*(1+M/4)*N && P[i].index<=3*(1+M/4)*N && M>3)
      P[i].partition=3;
    if(P[i].index>3*(1+M/4)*N && P[i].index<=M*N && M>3)
      P[i].partition=4;
    }
}
void print_individuals(int n_s, int n_z){
    for(int j=0;j<n_s+n_z;j++)
      {
      printf("                         i=%d-" , P[j].i_dim);
      printf("j=%d-" , P[j].j_dim);
      printf("State=%c-" , P[j].state);
      printf("Thread=%d\n" , P[j].partition);
      }
}






