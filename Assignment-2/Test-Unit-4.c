#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <sys/time.h>
#include <math.h>
#define N 10
#define M 10
#define default 1000000
void initialization(int n_s, int n_z);
void assign_dimensions(int index, int i);
void assign_state(int n_s, int n_z);
void movement(int index, int i);
void print_individuals(int n_s, int n_z);
typedef struct PTnode {
  int x_dim;
  int y_dim;
  int state;
  int index;
} PTnode;

PTnode P[default];
int main() {
  srand(time(NULL));
  int i,j,k,t;
  int n_s=3, n_z=2;
  P[0].index=5;
  P[1].index=21;
  P[2].index=100;
  P[3].index=92;
  P[4].index=60;
  for(i=0;i<n_s+n_z;i++)
    {
      assign_dimensions(P[i].index,i);
    }
  assign_state(n_s,n_z);
  printf("***********INITIAL STATE*********** \n");
  print_individuals(n_s,n_z);
  for(i=0;i<n_s+n_z;i++)
   {
      movement(P[i].index ,i);
   }
  for(k=0;k<n_s+n_z;k++)
   {
     assign_dimensions(P[k].index,k);
   }
  printf("***********FINAL STATUS*********** \n");
  print_individuals(n_s,n_z);
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
void movement(int index, int i){
  int m, s=1;
  while(s==1)
    {
    m=rand()%4;
    if(m==0)
      {
      if(index%N!=1)
        {
        P[i].index=index-1;
        s=0;
        break;
        }
      }
    if(m==1)
      {
      if(index%N!=0)
        {
        P[i].index=index+1;
        s=0;
        break;
        }
      }
    if(m==2)
      {
      if(index/N!=0 && index!=N)
        {
        P[i].index=index-N;
        s=0;
        break;
        }
      }
    if(m==3)
      {
      if(index/N!=M-1 && index!=M*N)
        {
        P[i].index=index+N;
        s=0;
        break;
        }
      }
    }
}
void assign_dimensions(int index, int i){
    if(index%M!=0)
      {
      P[i].x_dim=1+(index/M);
      P[i].y_dim=index%M;
      }
    else
      {
      P[i].x_dim=index/M;
      P[i].y_dim=M;
      }
}
void assign_state(int n_s, int n_z){
  int i, k;
  for(i=0;i<n_s+n_z;i++)
      P[i].state=0;
  for(i=0;i<n_s;i++)
    {
      k=rand()%(n_s+n_z);
      if(P[k].state==0)
        P[k].state=1;
      else
        i--;
    }
  for(i=0;i<n_z+n_s;i++)
    {
      if(P[i].state==0)
        P[i].state=2;
    }
}
void print_individuals(int n_s, int n_z){
    for(int j=0;j<n_s+n_z;j++)
      {
      printf("x=%d-" , P[j].x_dim);
      printf("y=%d-" , P[j].y_dim);
      printf("state=%d" , P[j].state);
      printf("\n");
      }
}

