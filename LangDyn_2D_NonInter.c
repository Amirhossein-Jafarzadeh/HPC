#include "stdio.h"
#include "stdlib.h"
#include "math.h"

float PI=3.14159265359;

#define N 

int main(){
	
	int t=0,tmax=;
	
	int i, j;
	
	double avg_x=0, avg_x2=0;
	double avg_y=0, avg_y2=0;
	
	double r;
	
	double dt=;
	double kT=;
	double g=;
	double m=;
	
	
	double x[N],vx[N],vhx[N],ax[N];
	double y[N],vy[N],vhy[N],ay[N];
	
	double ran,ran1,ran2,ran3;
	double gr1,gr2;
	double pref1,pref2;
	
	pref1=g;
	pref2=sqrt(24*kT*g/dt);
	
	FILE *outt,*outstats;
	outt=fopen("ld_2D_NonInter_traj.xyz","w");
	outstats=fopen("ld_2D_NonInter_stats.txt","w");
	
	avg_x=0;
	avg_x2=0;
	for(i=0;i<N;i++){
		x[i]=0;
		y[i]=0;
		ran1=(float)rand()/RAND_MAX;
		ran2=(float)rand()/RAND_MAX;
		gr1=sqrt(kT/(m))*sqrt(-2*log(ran1))*cos(2*PI*ran2);
		gr2=sqrt(kT/(m))*sqrt(-2*log(ran1))*sin(2*PI*ran2);
		vx[i]=gr1;
		vy[i]=gr2;
		ran1=(float)rand()/RAND_MAX;
		ran2=(float)rand()/RAND_MAX;
		gr1=sqrt(kT/(m))*sqrt(-2*log(ran1))*cos(2*PI*ran2);
		vy[i]=gr1;		
		ax[i]=0;
		ay[i]=0;
	}
		
	fprintf(outstats,"%E %E %E %E\n",avg_x,avg_x2,avg_y,avg_y2);
	while(t<tmax){
		avg_x=0;
		avg_x2=0;
		avg_y=0;
		avg_y2=0;
		
		if(t%10==0){
			fprintf(outt,"%i\n",N);
			fprintf(outt,"title\n");
		}
		
		for(i=0;i<N;i++){
			if(t%10==0)
				fprintf(outt,"a%i %f %f 0.0\n",i,x[i],y[i]);
			
			vhx[i]=vx[i]+0.5*ax[i]*dt;
			vhy[i]=vy[i]+0.5*ay[i]*dt;
			
			x[i]=x[i]+vhx[i]*dt;
			y[i]=y[i]+vhy[i]*dt;

			ax[i]=0;
			ay[i]=0;
				
			ran1=(double)rand()/RAND_MAX-0.5;
			ran2=(double)rand()/RAND_MAX-0.5;
			
			ax[i]+=-pref1*vhx[i]+pref2*ran1;
			ay[i]+=-pref1*vhy[i]+pref2*ran2;
			
			vx[i]=vhx[i]+0.5*ax[i]*dt;
			vy[i]=vhy[i]+0.5*ay[i]*dt;
			
			avg_x+=x[i];
			avg_x2+=x[i]*x[i];
			avg_y+=y[i];
			avg_y2+=y[i]*y[i];
		}
		
		avg_x=avg_x/(double)N;
		avg_x2=avg_x2/(double)N;
		avg_y=avg_y/(double)N;
		avg_y2=avg_y2/(double)N;
		
		fprintf(outstats,"%E %E %E %E\n",avg_x,avg_x2,avg_y,avg_y2);
		
		t++;
	}
}
			
			
