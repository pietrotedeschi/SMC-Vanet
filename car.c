clock x; 

int cur_speed; 

bool dir; 
int cur_seg;  
int Vx,Vy;    
int VxA=-1;   
int VyA=-1;   
int tCross;  
int crossed; 
int posIn;    
int nextI;    
int prevI;    


bool isAnomaly;         
bool anomaly_rcv=false; 




void setParameters(int seg, int d)
{   
	cur_seg=seg;
	prevI=nInters[seg][!d]; 
	nextI=nInters[seg][d];
	isAnomaly = (idcar==9) ? true:false;
	Vx=I[prevI].Px;
	Vy=I[prevI].Py;
	dir=d;
	if(dir==0) 
		S[cur_seg].nc_r++;
	else 
		S[cur_seg].nc_l++;

	settings++; 
}

void setSpeed()
{
	if (dir==0 && S[cur_seg].nc_r!=0)
	{    
		cur_speed=high_speed/S[cur_seg].nc_r;
		if(S[cur_seg].expressway==1 && cur_speed>speedmax_way)
		{
			cur_speed=speedmax_way/S[cur_seg].nc_r;
		} else if (S[cur_seg].expressway==0 && cur_speed>slowmax_way)
		{
			cur_speed=slowmax_way/S[cur_seg].nc_r;
		}
	}else if (dir==1 && S[cur_seg].nc_l!=0)
	{
		cur_speed=high_speed/S[cur_seg].nc_l;
		if(S[cur_seg].expressway==1 && cur_speed>speedmax_way)
		{
			cur_speed=speedmax_way/S[cur_seg].nc_l;
		} else if (S[cur_seg].expressway==0 && cur_speed>slowmax_way)
		{
			cur_speed=slowmax_way/S[cur_seg].nc_l;
		}
	}
	if(cur_speed<slow_speed)
	{
		cur_speed=slow_speed;
	}
}

void initSpeed()
{

	setSpeed();
	
	posIn = S[cur_seg].length/(idcar+1);
	
	if(I[prevI].Px!=I[nextI].Px)
	{
		if(I[prevI].Px<I[nextI].Px)
		{
			Vx=Vx+posIn;
		}else {Vx=Vx-posIn;}
	}
	if(I[prevI].Py!=I[nextI].Py)
	{
		if(I[prevI].Py<I[nextI].Py)
		{
			Vy=Vy+posIn;
		}else {Vy=Vy-posIn;}
	}

	tCross=(S[cur_seg].length/cur_speed)-(posIn/cur_speed); 
	
	Mgen[idcar].Vx=Vx;
	Mgen[idcar].Vy=Vy;
	Mgen[idcar].cur_seg=cur_seg;
	Mgen[idcar].dir=dir;
	
	if(isAnomaly==true)
	{
		Mgen[idcar].anomaly=true; 
		Mgen[idcar].VxA=Vx; 
		Mgen[idcar].VyA=Vy;
	}else
	{
		Mgen[idcar].anomaly=false; 
		Mgen[idcar].VxA=-1;    
		Mgen[idcar].VxA=-1;
	}
	
	Mrec[idcar].VxA = -1;
	Mrec[idcar].VyA = -1;   
}


void update(int id,int ns)
{

	meta int temp=cur_seg;

	crossed=0;   
	prevI=nextI; 
	cur_seg=ns;  

	if(dir==0)
		S[temp].nc_r--;
	else 
		S[temp].nc_l--;

	if(nInters[ns][0]==prevI)
	{
		nextI=nInters[ns][1];  
		dir=1;
		S[cur_seg].nc_l++; 
	} 
	else
	{
		nextI=nInters[ns][0]; 
		dir=0; 
		S[cur_seg].nc_r++;
	}

	Vx=I[prevI].Px; 
	Vy=I[prevI].Py;

	x=0; 

	tCross=S[cur_seg].length/cur_speed; 

}

void send(int id)
{
	if(isAnomaly==true)
	{
		tCross = BP;

	}
	else{

		if(I[prevI].Px!=I[nextI].Px)
		{
			if(I[prevI].Px<I[nextI].Px)
			{
				Vx=Vx+BP*cur_speed;
			}else {Vx=Vx-BP*cur_speed;}
		}
		if(I[prevI].Py!=I[nextI].Py)
		{
			if(I[prevI].Py<I[nextI].Py)
			{
				Vy=Vy+BP*cur_speed;
			}else {Vy=Vy-BP*cur_speed;}
		}

		Mgen[id].cur_seg=cur_seg;
		Mgen[id].Vx=Vx;
		Mgen[id].Vy=Vy;
		Mgen[id].dir=dir;
		Mgen[id].cur_seg=GEN; 

		if(anomaly_rcv==true)
		{
			Mgen[id].anomaly=true;
			Mgen[id].VxA=VxA;
			Mgen[id].VyA=VyA;
		}

		crossed=crossed+BP*cur_speed; 
		setSpeed();

		if (crossed>=(S[cur_seg].length-100)) {cur_speed=slow_speed;} 
		tCross = ((S[cur_seg].length-crossed)/cur_speed);
	}
	x=0;
}

void sendRelay(int id)
{
	Mgen[id].type=REL;
}

bool inRange(int id_tx)
{
	bool ricevuto=false;

	if(cur_seg==Mgen[id_tx].cur_seg && Mgen[id_tx].anomaly==true and anomaly_rcv==false)
	{
		if(dir==Mgen[id_tx].dir) 
		{
			if(Mgen[id_tx].type==GEN) 
			{
				if(Vx==Mgen[id_tx].Vx) 
				{
					
					if((Vy-Mgen[id_tx].Vy)<0 && abs(Vy-Mgen[id_tx].Vy<=r)) ricevuto=true; 
				}
				if(Vy==Mgen[id_tx].Vy) 
				{

					if((Vx-Mgen[id_tx].Vx)<0 && abs(Vx-Mgen[id_tx].Vx<=r)) ricevuto=true; 
				}
			} 
			else if (Mgen[id_tx].type==REL) 
			{
				if(Vx==Mgen[id_tx].Vx) 
				{
					if((Vy-Mgen[id_tx].Vy)>0 && abs(Vy-Mgen[id_tx].Vy<=r)) ricevuto=true; 
				}
				if(Vy==Mgen[id_tx].Vy) 
				{
					
					if((Vx-Mgen[id_tx].Vx)>0 && abs(Vx-Mgen[id_tx].Vx<=r)) ricevuto=true; 
				}
			}

		}
		if (dir!=Mgen[id_tx].dir)
		{
			if(Vx==Mgen[id_tx].Vx) 
			{
				if(abs(Vy-Mgen[id_tx].Vy)<=r) ricevuto=true; 
			}  
			if(Vy==Mgen[id_tx].Vy)
			{
				if(abs(Vx-Mgen[id_tx].Vx)<=r) ricevuto=true;
			}     
		}

	}
	return ricevuto;
}

void receive(int id_tx)
{
	anomaly_rcv=true;   
	VxA=Mgen[id_tx].VxA;
	VyA=Mgen[id_tx].VyA;

	if(distance(Vx,Vy,VxA,VyA) >= dist) count++; 

	if((Mgen[id_tx].type==GEN and dir !=Mgen[id_tx].dir) || (Mgen[id_tx].type==REL and dir==Mgen[id_tx].dir))
	{
		Mrec[idcar].VxA=VxA; 
		Mrec[idcar].VyA=VyA;
	}

}

bool inRangeRSU(int id_r)
{
	bool ricevuto=false;

	if(anomaly_rcv==false and distance(Vx,Vy,RSUAnomaly[id_r].Vx,RSUAnomaly[id_r].Vy) <= RR)
	{
		ricevuto=true;
	}
	return ricevuto;
}

void receiveRSU(int id_r)
{
	anomaly_rcv=true; 

	VxA=RSUAnomaly[id_r].VxA;
	VyA=RSUAnomaly[id_r].VyA;

	Mrec[idcar].VxA=VxA;
	Mrec[idcar].VyA=VyA;

	if(distance(Vx,Vy,VxA,VyA) >= dist) count++;

}

bool relayReady() {return Mrec[idcar].VxA == -1 and Mrec[idcar].VyA == -1 ? false:true;}
bool segEnabled(int id_seg) { return Map[nextI][id_seg]!=-1 and Map[nextI][id_seg]!=cur_seg ? true:false;}
