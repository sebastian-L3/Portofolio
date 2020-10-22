#include<iostream>
#include<windows.h>
#include<cstdlib>
#include<string>
#include<ctime>
#include<vector>
#include<cmath>

using namespace std;


class pixel{
		public:
	char icon;
	string type;
	bool bits;
	int u,l,r,d=-1;
		
	void update(char icon1,string type1,bool bits1,int u1,int l1,int r1,int d1){
		icon=icon1;
		type=type1;
		bits=bits1;
		u=u1;
		l=l1;
		d=d1;
		r=r1;
	}
};

class node{
		public:
	int curr;
	int posX;
	int posY;
	float heuristic;
	string direction;
	
	
	void update(int curr1,int posX1,int posY1,string direction1){
		curr=curr1;
		posX=posX1;
		posY=posY1;
		direction=direction1;
	}
	
	void insertHeuristic(int gx,int gy,int px,int py){
		int height=gy-py;
		if(height<0)height=height*-1;
		int base=gx-px;
		if(base<0)base=base*-1;
		
		heuristic=pow(height,2)+pow(base,2);
		if(heuristic<0)heuristic=heuristic*-1;
		heuristic=sqrt(heuristic);
 //  if(heuristic<1)heuristic=1;
	}
};

//global
pixel map[31][28];
char gamemode=1;
char debug=1;

class pac{
		public:
	bool spawned=false;
	int x;
	int y;
	int eat;
	string direction;
	char tempIcon;
	
	
	void update(int x1,int y1,int eat1,string direction1){
		x=x1;
		y=y1;
		eat=eat1;
		direction=direction1;
	}
	
	void positionUpdate(){
	if(direction.compare("up")==0){
			
			if(map[(y)-1][x].type.compare("wall")!=0){
				y--;		
			}
		}
		else if(direction.compare("down")==0){
		
			if(map[(y)+1][x].type.compare("wall")!=0){
				y++;		
			}
		}
		else if(direction.compare("left")==0){
		
			if(map[y][(x)-1].type.compare("wall")!=0){
				x--;		
			}
		}
		else if(direction.compare("right")==0){
			
			if(map[y][(x)+1].type.compare("wall")!=0){
				x++;		
			}
		}
	
	}
	
};

	pac pacman;
		

class ghost{
		public:
	bool spawned=false;	
	bool already=false;	
	int x;
	int y;
	string direction;
	char tempIcon;
	int sleep=0;
	
	void update(int x1,int y1,string direction1){
		x=x1;
		y=y1;
		direction=direction1;
	}
	
	void positionUpdate(){
	if(direction.compare("up")==0){
			
			if(map[(y)-1][x].type.compare("wall")!=0){
				y--;		
			}
		}
		else if(direction.compare("down")==0){
		
			if(map[(y)+1][x].type.compare("wall")!=0){
				y++;		
			}
		}
		else if(direction.compare("left")==0){
		
			if(map[y][(x)-1].type.compare("wall")!=0){
				x--;		
			}
		}
		else if(direction.compare("right")==0){
			
			if(map[y][(x)+1].type.compare("wall")!=0){
				x++;		
			}
		}
	
	}
	
	void algorithmicDirectionChange(int starOrGreed ,bool eduMode){
		vector<node> Vnode;
		int dToInter1=0,dToInter2=0;
		node temp;

		if(map[y][x].type.compare("intersection")!=0){
			if(map[y+1][x].type.compare("wall")==0 && map[y-1][x].type.compare("wall")==0){
				while(map[y][x-dToInter1].type.compare("intersection")!=0 && !(y==pacman.y && x-dToInter1==pacman.x)){
					dToInter1++;
				}
				while(map[y][x+dToInter2].type.compare("intersection")!=0 && !(y==pacman.y && x+dToInter2==pacman.x)){
					dToInter2++;
				}
			
				temp.update(dToInter1,x-dToInter1,y,"left");
				temp.insertHeuristic(x-dToInter1,y,pacman.x,pacman.y);

				Vnode.insert(Vnode.begin(),temp);
				temp.update(dToInter2,x+dToInter2,y,"right");
				temp.insertHeuristic(x+dToInter2,y,pacman.x,pacman.y); 
				}
			else if(map[y][x+1].type.compare("wall")==0 && map[y][x-1].type.compare("wall")==0){
				while(map[y-dToInter1][x].type.compare("intersection")!=0 && !(y-dToInter1==pacman.y && x==pacman.x)){
					dToInter1++;
				}
				while(map[y+dToInter2][x].type.compare("intersection")!=0 && !(y+dToInter2==pacman.y && x==pacman.x)){
					dToInter2++;
				}
			
				cout << dToInter1 << " " << dToInter2 << endl;
				
				temp.update(dToInter1,x,y-dToInter1,"up");
				temp.insertHeuristic(x,y-dToInter1,pacman.x,pacman.y);

				Vnode.insert(Vnode.begin(),temp);
				temp.update(dToInter2,x,y+dToInter2,"down");
				temp.insertHeuristic(x,y+dToInter2,pacman.x,pacman.y); 
				}
			
			if((Vnode.at(0).heuristic>temp.heuristic && starOrGreed==0) || ((Vnode.at(0).heuristic+Vnode.at(0).curr)>(temp.heuristic+temp.curr) && starOrGreed==1)){
				Vnode.insert(Vnode.begin() + 0,temp);
			}else{
				Vnode.insert(Vnode.begin() + 1,temp);
				
			}
		
			
			
		}else{// if inter
			int ixs=0;
			if(map[y][x].u!=-1){
				temp.update(map[y][x].u,x,y-map[y][x].u,"up");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				Vnode.insert(Vnode.begin() + 0,temp);
			}
		
			if(map[y][x].d!=-1){
				temp.update(map[y][x].d,x,y+map[y][x].d,"down");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				ixs=0;
				if(Vnode.size()>0){
				while(ixs<Vnode.size()){
							if((Vnode.at(ixs).heuristic>temp.heuristic && starOrGreed==0) || (((Vnode.at(ixs).heuristic+Vnode.at(ixs).curr)>(temp.heuristic+temp.curr) && starOrGreed==1))){
								Vnode.insert(Vnode.begin()+ixs,temp);
								break;
							}
							if(ixs==Vnode.size()-1){
								Vnode.insert(Vnode.begin()+ixs+1,temp);
								break;
							}
							ixs++;
				}
			}else{
				Vnode.insert(Vnode.begin() + 0,temp);
			}
			}
			if(map[y][x].l!=-1){
				temp.update(map[y][x].l,x-map[y][x].l,y,"left");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				ixs=0;
				if(Vnode.size()>0){
				while(ixs<Vnode.size()){
								if((Vnode.at(ixs).heuristic>temp.heuristic && starOrGreed==0) || (((Vnode.at(ixs).heuristic+Vnode.at(ixs).curr)>(temp.heuristic+temp.curr) && starOrGreed==1))){
								Vnode.insert(Vnode.begin()+ixs,temp);
								break;
							}
							if(ixs==Vnode.size()-1){
								Vnode.insert(Vnode.begin()+ixs+1,temp);
								break;
							}
							ixs++;
					}
			}else{
				Vnode.insert(Vnode.begin() + 0,temp);
			}
			}
			
			if(map[y][x].r!=-1){
				temp.update(map[y][x].r,x+map[y][x].r,y,"right");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				ixs=0;
				if(Vnode.size()>0){
				while(ixs<Vnode.size()){
								if((Vnode.at(ixs).heuristic>temp.heuristic && starOrGreed==0) || ((Vnode.at(ixs).heuristic+Vnode.at(ixs).curr>temp.heuristic+temp.curr && starOrGreed==1))){
								Vnode.insert(Vnode.begin()+ixs,temp);
								break;
							}
							if(ixs==Vnode.size()-1){
								Vnode.insert(Vnode.begin()+ixs+1,temp);
								break;
							}
							ixs++;
					}
			}else{
				Vnode.insert(Vnode.begin() + 0,temp);
			}
			}
		
		}
		//print size vector
//		if(eduMode!=true){
//		for(int ix=0;ix<Vnode.size();ix++){
//			if(debug=='Y')cout << ix << " # H:" << Vnode.at(ix).heuristic << " C:"<<Vnode.at(ix).curr << " " << Vnode.at(ix).posX << " " << Vnode.at(ix).posY << "  " << Vnode.at(ix).direction << "| ";
//			}
//		}
		
		int dToInterPac1=0;
		int dToInterPac2=0;
		node goal1,goal2;
		
		if(map[pacman.y][pacman.x].type.compare("intersection")!=0){
			
			if(map[pacman.y+1][pacman.x].type.compare("wall")==0 && map[pacman.y-1][pacman.x].type.compare("wall")==0){
				while(map[pacman.y][pacman.x-dToInterPac1].type.compare("intersection")!=0){
					dToInterPac1++;
				}
				while(map[pacman.y][pacman.x+dToInterPac2].type.compare("intersection")!=0){
					dToInterPac2++;
				}
			
				node temp;
				goal1.update(dToInterPac1,pacman.x-dToInterPac1,pacman.y,"right");
				goal2.update(dToInterPac2,pacman.x+dToInterPac2,pacman.y,"left");
			
				}
			else if(map[pacman.y][pacman.x+1].type.compare("wall")==0 && map[pacman.y][pacman.x-1].type.compare("wall")==0){
				while(map[pacman.y-dToInterPac1][pacman.x].type.compare("intersection")!=0){
					dToInterPac1++;
				}
				while(map[pacman.y+dToInterPac2][pacman.x].type.compare("intersection")!=0){
					dToInterPac2++;
				}
			
				goal1.update(dToInterPac1,pacman.x,pacman.y-dToInterPac1,"down");
				goal2.update(dToInterPac2,pacman.x,pacman.y+dToInterPac2,"up");
				
				}
		}else if(map[pacman.y][pacman.x].type.compare("intersection")==0){
			goal1.update(0,pacman.x,pacman.y,"none");
			goal2.update(0,pacman.x,pacman.y,"none");
		}
				vector<node> popped;
				//////////////Print DEBUGMODE///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
				if(debug=='Y'){
					printf("==========X==Y==Dirrection\n");
					printf("1st Goal: %-2d %-2d ",goal1.posX,goal1.posY); cout<< goal1.direction << endl;
					printf("2nd Goal: %-2d %-2d ",goal2.posX,goal2.posY); cout<< goal2.direction << endl;
					printf("X |Y |Current|Heuristic|Dirrection\n");
				}
	
				while((Vnode.at(0).posX!=goal1.posX || Vnode.at(0).posY!=goal1.posY)&& (Vnode.at(0).posX!=goal2.posX || Vnode.at(0).posY!=goal2.posY)){
					node current=Vnode.at(0);
					if(debug=='Y'){
					printf("%-2d|%-2d|%-7d|%-9f|",current.posX,current.posY,current.curr,current.heuristic);
					cout << current.direction << endl;
					}
					
					//PRINNT EDUMODE////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					if(eduMode && already==false){
						system("CLS");
						map[pacman.y][pacman.x].icon=='O';
						char iconic;
						if(starOrGreed==0){map[y][x].icon='$';iconic=':';}
						if(starOrGreed==1){map[y][x].icon='X';iconic='|';}
						for (int i = 0; i < 31; i++) {
							for (int j = 0; j < 28; j++) {
							if(map[i][j].icon!='*'){cout << map[i][j].icon;}
							else{
							cout << " ";
							}
						}
							cout << "\n";
						}
						if(starOrGreed==0)cout<<"[GREED]\n";
						if(starOrGreed==1)cout<<"[A STAR]\n";
						node printemp=Vnode.at(0);
						if(map[printemp.posY][printemp.posX].u!=-1){
							int up1=map[printemp.posY][printemp.posX].u;
							if(map[printemp.posY-up1][printemp.posX].icon==iconic || (x==printemp.posX && y==printemp.posY-up1)){
								for(int i=0;i<up1;i++){
									map[printemp.posY-i][printemp.posX].icon=iconic ;
								}
							}
						}
						if(map[printemp.posY][printemp.posX].d!=-1){
							int down1=map[printemp.posY][printemp.posX].d;
							if(map[printemp.posY+down1][printemp.posX].icon==iconic  || (x==printemp.posX && y==printemp.posY+down1)){
								for(int i=0;i<down1;i++){
									map[printemp.posY+i][printemp.posX].icon=iconic ;
								}
							}
						}
						if(map[printemp.posY][printemp.posX].l!=-1){
							int left1=map[printemp.posY][printemp.posX].l;
							if(map[printemp.posY][printemp.posX-left1].icon==iconic  || (x==printemp.posX-left1 && y==printemp.posY)){
								for(int i=0;i<left1;i++){
									map[printemp.posY][printemp.posX-i].icon=iconic ;
								}
							}
						}
						if(map[printemp.posY][printemp.posX].r!=-1){
							int right1=map[printemp.posY][printemp.posX].r;
							if(map[printemp.posY][printemp.posX+right1].icon==iconic  || (x==printemp.posX+right1 && y==printemp.posY)){
								for(int i=0;i<right1;i++){
									map[printemp.posY][printemp.posX+i].icon=iconic ;
								}
							}
						}
						cout<<"press Enter.";
						getchar();
						fflush(stdin);
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					popped.insert(popped.begin(),Vnode.at(0));
					Vnode.erase(Vnode.begin());
					node tempo;
					//UP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					if(map[current.posY][current.posX].u!=-1){
					
						tempo.update(current.curr+map[current.posY][current.posX].u,current.posX,current.posY-map[current.posY][current.posX].u,current.direction);
						tempo.insertHeuristic(tempo.posX,tempo.posY,pacman.x,pacman.y);
						int i=0;
						bool bye=false;
						for(int ixx=0;ixx<popped.size();ixx++){
							if(popped.at(ixx).posX==tempo.posX && popped.at(ixx).posY==tempo.posY && popped.at(ixx).direction.compare(tempo.direction)==0){
								bye=true;
								break;
							}
						}
						while(i<Vnode.size()){
							if(bye)break;
								if((Vnode.at(i).heuristic>tempo.heuristic && starOrGreed==0) || ((Vnode.at(i).heuristic+Vnode.at(i).curr>tempo.heuristic+tempo.curr && starOrGreed==1))){
								Vnode.insert(Vnode.begin()+i,tempo);
								break;
							}
							if(i==Vnode.size()-1){
								Vnode.insert(Vnode.begin()+i+1,tempo);
								break;
							}
							i++;
						}
					}
				//DOWN//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if(map[current.posY][current.posX].d!=-1){
				
						tempo.update(current.curr+map[current.posY][current.posX].d,current.posX,current.posY+map[current.posY][current.posX].d,current.direction);
						tempo.insertHeuristic(tempo.posX,tempo.posY,pacman.x,pacman.y);
						int i=0;
						bool bye=false;
					
						for(int ixx=0;ixx<popped.size();ixx++){
							if(popped.at(ixx).posX==tempo.posX && popped.at(ixx).posY==tempo.posY && popped.at(ixx).direction.compare(tempo.direction)==0){
								bye=true;
								break;
							}
						}
					
						while(i<Vnode.size()){
							if(bye)break;
							if((Vnode.at(i).heuristic>tempo.heuristic && starOrGreed==0) || ((Vnode.at(i).heuristic+Vnode.at(i).curr>tempo.heuristic+tempo.curr && starOrGreed==1))){
								Vnode.insert(Vnode.begin()+i,tempo);
								break;
							}
							if(i==Vnode.size()-1){
								Vnode.insert(Vnode.begin()+i+1,tempo);
								break;
							}
							i++;
						}
					
					}
				//LEFT//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
				if(map[current.posY][current.posX].l!=-1){
				
						tempo.update(current.curr+map[current.posY][current.posX].l,current.posX-map[current.posY][current.posX].l,current.posY,current.direction);
						tempo.insertHeuristic(tempo.posX,tempo.posY,pacman.x,pacman.y);
						int i=0;
						bool bye=false;
						for(int ixx=0;ixx<popped.size();ixx++){
							if(popped.at(ixx).posX==tempo.posX && popped.at(ixx).posY==tempo.posY && popped.at(ixx).direction.compare(tempo.direction)==0){
								bye=true;
								break;
							}
						}
						while(i<Vnode.size()){
							if(bye)break;
							if((Vnode.at(i).heuristic>tempo.heuristic && starOrGreed==0) || ((Vnode.at(i).heuristic+Vnode.at(i).curr>tempo.heuristic+tempo.curr && starOrGreed==1))){
								Vnode.insert(Vnode.begin()+i,tempo);
								break;
							}
							if(i==Vnode.size()-1){
								Vnode.insert(Vnode.begin()+i+1,tempo);
								break;
							}
							i++;
						}
					}
				//RIGHT//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if(map[current.posY][current.posX].r!=-1){
			
						tempo.update(current.curr+map[current.posY][current.posX].r,current.posX+map[current.posY][current.posX].r,current.posY,current.direction);
						tempo.insertHeuristic(tempo.posX,tempo.posY,pacman.x,pacman.y);
						int i=0;
						bool bye=false;
						for(int ixx=0;ixx<popped.size();ixx++){
							if(popped.at(ixx).posX==tempo.posX && popped.at(ixx).posY==tempo.posY && popped.at(ixx).direction.compare(tempo.direction)==0){
								bye=true;
								break;
							}
						}
						while(i<Vnode.size() && !bye){
							if(bye)break;
							if((Vnode.at(i).heuristic>tempo.heuristic && starOrGreed==0) || ((Vnode.at(i).heuristic+Vnode.at(i).curr>tempo.heuristic+tempo.curr && starOrGreed==1))){
								Vnode.insert(Vnode.begin()+i,tempo);
								break;
							}
							if(i==Vnode.size()-1){
								Vnode.insert(Vnode.begin()+i+1,tempo);
								break;
							}
							i++;
						}
					}	
			}
			/////////////////////////////////////////////////////////////////////////
			if(eduMode && already==false){
						system("CLS");
						map[pacman.y][pacman.x].icon=='O';
						char iconic;
						if(starOrGreed==0){map[y][x].icon='$';iconic=':';}
						if(starOrGreed==1){map[y][x].icon='X';iconic='|';}
						for (int i = 0; i < 31; i++) {
							for (int j = 0; j < 28; j++) {
							if(map[i][j].icon!='*'){cout << map[i][j].icon;}
							else{
							cout << " ";
							}
						}
							cout << "\n";
						}
						if(starOrGreed==0)cout<<"[GREED]\n";
						if(starOrGreed==1)cout<<"[A STAR]\n";
						node printemp=Vnode.at(0);
						if(map[printemp.posY][printemp.posX].u!=-1){
							int up1=map[printemp.posY][printemp.posX].u;
							if(map[printemp.posY-up1][printemp.posX].icon==iconic || (x==printemp.posX && y==printemp.posY-up1)){
								for(int i=0;i<up1;i++){
									map[printemp.posY-i][printemp.posX].icon=iconic ;
								}
							}
						}
						if(map[printemp.posY][printemp.posX].d!=-1){
							int down1=map[printemp.posY][printemp.posX].d;
							if(map[printemp.posY+down1][printemp.posX].icon==iconic  || (x==printemp.posX && y==printemp.posY+down1)){
								for(int i=0;i<down1;i++){
									map[printemp.posY+i][printemp.posX].icon=iconic ;
								}
							}
						}
						if(map[printemp.posY][printemp.posX].l!=-1){
							int left1=map[printemp.posY][printemp.posX].l;
							if(map[printemp.posY][printemp.posX-left1].icon==iconic  || (x==printemp.posX-left1 && y==printemp.posY)){
								for(int i=0;i<left1;i++){
									map[printemp.posY][printemp.posX-i].icon=iconic ;
								}
							}
						}
						if(map[printemp.posY][printemp.posX].r!=-1){
							int right1=map[printemp.posY][printemp.posX].r;
							if(map[printemp.posY][printemp.posX+right1].icon==iconic  || (x==printemp.posX+right1 && y==printemp.posY)){
								for(int i=0;i<right1;i++){
									map[printemp.posY][printemp.posX+i].icon=iconic ;
								}
							}
						}
						cout<<"press Enter.";
						getchar();
						fflush(stdin);
					}
					
				//////////////////////////////////////////////////////////////////////
				if(map[pacman.y][pacman.x].type.compare("road")==0){
                popped.insert(popped.begin(),Vnode.at(0));
            }
				if(map[pacman.y][pacman.x].type.compare("road")==0 && ((x==goal1.posX && y==goal1.posY) || (x==goal2.posX && y==goal2.posY))){
					if(x==goal1.posX && y==goal1.posY){
						direction=goal1.direction;
					}
					else if(x==goal2.posX && y==goal2.posY){
						direction=goal2.direction;
					}
				}
				else{
				direction=Vnode.at(0).direction;
				}
				already=true;
	}
};
		ghost rando;
		ghost greed;
		ghost star;

void createMap(pixel map[31][28]){
	for (int i = 0; i < 31; i++) {//BORDER
		for (int j = 0; j < 28; j++) {
			if (i == 0 || i == 30 || j == 0 || j == 27) {
			map[i][j].icon = 176;
			map[i][j].type = "wall";
			map[i][j].bits=false;
			}
			else {
				map[i][j].icon = ' ';
				map[i][j].bits=false;
			}
		}
	}
	
	
	
	//road wall dalam
	for(int i=1;i<=29;i++){
		for(int j=1;j<=26;j++){
			if(i==1){
				if(j>12 && j<15){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i>1 && i<5){
				if(j==1 || j==6 || j==12 || j==26 || j==21 || j==15){
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
				else {
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
			}
			
			if(i==5){
				map[i][j].update('*',"road",true,-1,-1,-1,-1);
			}
			
			if(i==6 || i==7){
				if(!(j==1 || j==6 || j==9 || j==26 || j==21 || j==18)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i==8){
				if((j>6 && j<9) || (j>18 && j<21)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i>8 && i<11){
				if(!(j==6 || j==12 || j==21 || j==15)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i==11){
				if((j>0 && j<6) || (j>6 && j<9) || (j>12 && j<15) || (j>18 && j<21) || (j>21 && j<27)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i>11 && i<14){
				if(!(j==6 || j==9 || j==21 || j==18)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i==14){
				if((j>0 && j<6) || (j>9 && j<18) || (j>21 && j<27)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i>14 && i<20){
				if(!(j==6 || j==9 || j==21 || j==18)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i==20){
				if((j>12 && j<15)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i>20 && i<23){
				if(!(j==1 || j==6 || j==12 || j==26 || j==21 || j==15)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i==23){
				if((j>4 && j<6) || (j>12 && j<15) || (j>21 && j<23)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i>23 && i<26){
				if(!(j==4 || j==6 || j==9 || j==18 || j==21 || j==23)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i==26){
				if((j>6 && j<9) || (j>12 && j<15) || (j>18 && j<21) ){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i>26 && i<29){
				if(!(j==1 || j==12 || j==26 || j==15)){
					map[i][j].update(176,"wall",false,-1,-1,-1,-1);
					}
				else {
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
					}
			}
			
			if(i==29){
					map[i][j].update('*',"road",true,-1,-1,-1,-1);
			}
		}
	}	

	//intersection
	if(true){
	map[1][1].update('*',"intersection",true,-1,-1,5,4);
	map[1][6].update('*',"intersection",true,-1,5,6,4);
	map[1][12].update('*',"intersection",true,-1,6,-1,4);
	map[1][26].update('*',"intersection",true,-1,5,-1,4);
	map[1][21].update('*',"intersection",true,-1,6,5,4);
	map[1][15].update('*',"intersection",true,-1,-1,6,4);
	
	map[5][1].update('*',"intersection",true,4,-1,5,3);
	map[5][6].update('*',"intersection",true,4,5,3,3);
	map[5][9].update('*',"intersection",true,-1,3,3,3);
	map[5][12].update('*',"intersection",true,4,3,3,-1);
	map[5][26].update('*',"intersection",true,4,5,-1,3);
	map[5][21].update('*',"intersection",true,4,3,5,3);
	map[5][18].update('*',"intersection",true,-1,3,3,3);
	map[5][15].update('*',"intersection",true,4,3,3,-1);
	
	map[8][1].update('*',"intersection",true,3,-1,5,-1);
	map[8][6].update('*',"intersection",true,3,5,-1,6);
	map[8][9].update('*',"intersection",true,3,-1,3,-1);
	map[8][12].update('*',"intersection",true,-1,3,-1,3);
	map[8][26].update('*',"intersection",true,3,5,-1,-1);
	map[8][21].update('*',"intersection",true,3,-1,5,6);
	map[8][18].update('*',"intersection",true,3,3,-1,-1);
	map[8][15].update('*',"intersection",true,-1,-1,3,3);
	
	map[11][9].update('*',"intersection",true,-1,-1,3,3);
	map[11][12].update('*',"intersection",true,3,3,-1,-1);
	map[11][18].update('*',"intersection",true,-1,3,-1,3);
	map[11][15].update('*',"intersection",true,3,-1,3,-1);
	
	map[14][6].update('*',"intersection",true,6,-1,3,6);
	map[14][9].update('*',"intersection",true,3,3,-1,6);
	map[14][21].update('*',"intersection",true,6,3,-1,6);
	map[14][18].update('*',"intersection",true,3,-1,3,6);
	
	map[20][1].update('*',"intersection",true,-1,-1,5,3);
	map[20][6].update('*',"intersection",true,6,5,3,3);
	map[20][9].update('*',"intersection",true,6,3,3,-1);
	map[20][12].update('*',"intersection",true,-1,3,-1,3);
	map[20][26].update('*',"intersection",true,-1,5,-1,3);
	map[20][21].update('*',"intersection",true,6,3,5,3);
	map[20][18].update('*',"intersection",true,6,3,3,-1);
	map[20][15].update('*',"intersection",true,-1,-1,3,3);
	
	map[23][1].update('*',"intersection",true,3,-1,3,-1);
	map[23][4].update('*',"intersection",true,-1,3,-1,3);
	map[23][6].update('*',"intersection",true,3,-1,3,3);
	map[23][9].update('*',"intersection",true,-1,3,3,3);
	map[23][12].update('*',"intersection",true,3,3,-1,-1);
	map[23][26].update('*',"intersection",true,3,3,-1,-1);
	map[23][23].update('*',"intersection",true,-1,-1,3,3);
	map[23][21].update('*',"intersection",true,3,3,-1,3);
	map[23][18].update('*',"intersection",true,-1,3,3,3);
	map[23][15].update('*',"intersection",true,3,-1,3,-1);
	
	map[26][1].update('*',"intersection",true,-1,-1,3,3);
	map[26][4].update('*',"intersection",true,3,3,2,-1);
	map[26][6].update('*',"intersection",true,3,2,-1,-1);
	map[26][9].update('*',"intersection",true,3,-1,3,-1);
	map[26][12].update('*',"intersection",true,-1,3,-1,3);
	map[26][26].update('*',"intersection",true,-1,3,-1,3);
	map[26][23].update('*',"intersection",true,3,2,3,-1);
	map[26][21].update('*',"intersection",true,3,-1,2,-1);
	map[26][18].update('*',"intersection",true,3,3,-1,-1);
	map[26][15].update('*',"intersection",true,-1,-1,3,3);
	
	map[29][1].update('*',"intersection",true,3,-1,11,-1);
	map[29][12].update('*',"intersection",true,3,11,3,-1);
	map[29][26].update('*',"intersection",true,3,11,-1,-1);
	map[29][15].update('*',"intersection",true,3,3,11,-1);
	}
}

void Game(int mode){
	//SCREEN1//////////////////////////////////////////////////////////////////////////////////
		if(mode == 1){
			while(gamemode!='f' && gamemode!='s'){
			system("CLS");
			cout << "fast or slow gamemode [f|s]: ";
			gamemode=getchar();
			fflush(stdin);
			}
			debug='N';
			
		}else if(mode == 2){
			gamemode='s';
			debug='Y';
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		
		rando.update(1,29,"down");
		pacman.update(1,1,0,"none");
		greed.update(26,1,"down");
		star.update(26,29,"left");
		
		int turncounter=0;
		int Plive=3;
		system("CLS");
		
		cout << "===Tutorial===" << endl;
		cout << "you are C(Pacman), starts at top left\nmove with W,A,S,D (Hold the button)[fast mode],(and press enter to finalize input)[slow mode]\n\n";
		cout << "eat * to get points, reach 280/all * and you win the game\n";
		cout << "dodge ghosts($,?,X),ghosts need rest after a while and start a countdown before walking again (X->1)\n";
		cout << "Press Enter...";
		fflush(stdin);
		getchar();
		//count down
		system("CLS");
		cout << "3 ";
		Sleep(300);
		cout << "2 ";
		Sleep(300);
		cout << "1 ";
		Sleep(300);
		cout << "GO! ";
		Sleep(300);
		
			int oldrandox=rando.x;int oldrandoy=rando.y;
			int oldgreedx=greed.x;int oldgreedy=greed.y;
			int oldstarx=star.x;int oldstary=star.y;
			
		while(pacman.eat<280 && Plive>=1){////////////////////////////////////////////////GAMELOOP///////////////////////////////////////////////////////////////////////////
			system("CLS");
			
			if((pacman.y==rando.y && pacman.x==rando.x) || (pacman.y==greed.y && pacman.x==greed.x) || (pacman.y==star.y && pacman.x==star.x)
				|| (pacman.y==oldrandoy && pacman.x==oldrandox) || (pacman.y==oldgreedy && pacman.x==oldgreedx) || (pacman.y==oldstary && pacman.x==oldstarx)){
				cout << "3 ";
				Sleep(60);
				cout << "2 ";
				Sleep(60);
				cout << "1 ";
				Sleep(60);

				system("CLS");
				Plive--;
				pacman.update(1,1,pacman.eat,"none");
				if(rando.y<5 && rando.x<5){
					rando.update(1,29,"down");
				}
				if(greed.y<5 && greed.x<5){
				greed.update(26,1,"down");
				}
				if(star.y<5 && star.x<5){
				star.update(26,29,"down");
				}
				
			}
			oldrandox=rando.x;	oldrandoy=rando.y;
			oldgreedx=greed.x;	oldgreedy=greed.y;
			oldstarx=star.x;	oldstary=star.y;
			
			rando.tempIcon=map[rando.y][rando.x].icon;
			greed.tempIcon=map[greed.y][greed.x].icon;
			star.tempIcon=map[star.y][star.x].icon;
			map[pacman.y][pacman.x].icon='C';
			if(rando.sleep<1){map[rando.y][rando.x].icon='?';}else{map[rando.y][rando.x].icon=48+rando.sleep;}
			if(greed.sleep<1){map[greed.y][greed.x].icon='$';}else{map[greed.y][greed.x].icon=48+greed.sleep;}
			if(star.sleep<1){map[star.y][star.x].icon='X';}else{map[star.y][star.x].icon=48+star.sleep;}
			
			
			//printmap
			for (int i = 0; i < 31; i++) {
				for (int j = 0; j < 28; j++) {
				cout << map[i][j].icon;
				}
			cout << "\n";
			
			}
	      	map[rando.y][rando.x].icon=rando.tempIcon;
	    	map[greed.y][greed.x].icon=greed.tempIcon;
	    	map[star.y][star.x].icon=star.tempIcon;
	       
		
		//print meta data///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(true){
		cout << "LIFES:" << Plive << "		SCORE:"<< pacman.eat << endl;
		cout << "PACMAN|X:" << pacman.x << "Y:" << pacman.y << "	Greed|X:" << greed.x << "Y:" << greed.y << "	Rando|X:" << rando.x << "Y:" << rando.y <<  "	Star|X:" << star.x << "Y:" << star.y <<endl; 
		cout << "Input[W|A|S|D]";
		if(gamemode=='f') cout << "(Hold):";
		else cout << "(+Enter):";
		}
		
        //////sleepcounter///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        turncounter++;
        if(greed.sleep>0)greed.sleep--;
        if(rando.sleep>0)rando.sleep--;
        if(star.sleep>0)star.sleep--;
        if(turncounter%70==0 && greed.sleep<1)greed.sleep=9;
        if(turncounter%50==0 && star.sleep<1)star.sleep=7;
        int randomvar=rand()%9+4;
        if (randomvar<0)randomvar*=-1;
        if(turncounter%(randomvar*randomvar)==0 && rando.sleep<1)rando.sleep=randomvar;
        
        
             
		///////////////////////////////RANDO////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		int varRand=1;
		if(map[rando.y][rando.x].u!=-1 && rando.direction.compare("down")!=0){
				varRand++;
		}
		if(map[rando.y][rando.x].d!=-1 && rando.direction.compare("up")!=0){
				varRand++;
		}
		if(map[rando.y][rando.x].l!=-1 && rando.direction.compare("right")!=0){
				varRand++;
		}
		if(map[rando.y][rando.x].r!=-1 && rando.direction.compare("left")!=0){
				varRand++;
		}
		
		int randoDir = rand() % varRand;
		
		if(debug=='Y')cout << "\n==Rando==\nrandom variable :" << randoDir << "	routes:" << varRand-1 << "\n";
		bool cont=false;
		
		if(map[rando.y][rando.x].type.compare("road")!=0){
		if(map[rando.y][rando.x].u!=-1 && !cont && rando.direction.compare("down")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="up";
					cont=true;
				}
		}
		if(map[rando.y][rando.x].d!=-1 && !cont && rando.direction.compare("up")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="down";
					cont=true;
				}
		}
		if(map[rando.y][rando.x].l!=-1 && !cont && rando.direction.compare("right")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="left";
					cont=true;
				}
		}
		if(map[rando.y][rando.x].r!=-1 && !cont && rando.direction.compare("left")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="right";
					cont=true;
				}
		}
		}
		
		
		if(rando.sleep<1)rando.positionUpdate();
		
		///////////////////////////////GREED////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		if(debug=='Y')cout << "==Greed==\nCurrent Direction:" <<greed.direction << endl;
				
				greed.algorithmicDirectionChange(0,false);
				if(greed.sleep<1)greed.positionUpdate();
				if(debug=='Y')cout << "\nDirection after greedy:" <<greed.direction << endl;
		
		///////////////////////////////STAR////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		if(debug=='Y')cout << "==Star==\nCurrent Direction:" <<star.direction << endl;
				
				//if(map[star.y][star.x].type.compare("intersection")==0)
					star.algorithmicDirectionChange(1,false);
				if(star.sleep<1)star.positionUpdate();
				if(debug=='Y')cout << "\nDirection after a*:" <<star.direction << endl;
		
		
	
		//input///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(gamemode=='f'){
		if( GetKeyState('W') & 0x8000 )
		{
    	pacman.direction="up";
		}
		if( GetKeyState('A') & 0x8000 )
		{
    	pacman.direction="left";
		}
		if( GetKeyState('S') & 0x8000 )
		{
    	pacman.direction="down";
		}
		if( GetKeyState('D') & 0x8000 )
		{
    	pacman.direction="right";
		}
		}
		
   		//input getchar
        if(gamemode=='s'){
        	char input=getchar();
		if(input=='w')
		{
    	pacman.direction="up";
		}
		if(input=='a')
		{
    	pacman.direction="left";
		}
		if(input=='s')
		{
    	pacman.direction="down";
		}
		if(input=='d')
		{
    	pacman.direction="right";
		}
		}
	
		if(map[pacman.y][pacman.x].type.compare("intersection")!=0){Sleep(180);}
		else{Sleep(480);}
		//getchar();//nyalakan untuk debug
	
		///////////////////////////////PACMAN////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(map[pacman.y][pacman.x].bits==true){
				pacman.eat++;
				map[pacman.y][pacman.x].bits=false;
				map[pacman.y][pacman.x].icon=' ';
			}else{
				map[pacman.y][pacman.x].icon=' ';
			}
		
		pacman.positionUpdate();	

	
	
	if(pacman.eat==280){
		system("CLS");
		cout << "Pacman DINNER!\n";
	}
	fflush(stdin);
	Sleep(180);
//////////////////////////////////////////////////////ENDLOOP//////////////////////////////////////////////////////
}
}

printmap(){
				rando.tempIcon=map[rando.y][rando.x].icon;
				greed.tempIcon=map[greed.y][greed.x].icon;
				star.tempIcon=map[star.y][star.x].icon;
				char pacmantemp=map[pacman.y][pacman.x].icon;
				
				if(pacman.y!=0 && pacman.x!=0)map[pacman.y][pacman.x].icon='O';
				if(rando.y!=0 && rando.x!=0)map[rando.y][rando.x].icon='?';
				if(greed.y!=0 && greed.x!=0)map[greed.y][greed.x].icon='$';
				if(star.y!=0 && star.x!=0)map[star.y][star.x].icon='X';

	for (int i = 0; i < 31; i++) {
				for (int j = 0; j < 28; j++) {
				if(i==0)cout<<j%10;
				else if(j==0)cout<<i%10;
				else if(map[i][j].icon!='*'){cout << map[i][j].icon;}
				else{
					cout << " ";
				}
				}
				cout << "\n";
				}
				
				map[rando.y][rando.x].icon=rando.tempIcon;
	    		map[greed.y][greed.x].icon=greed.tempIcon;
	    		map[star.y][star.x].icon=star.tempIcon;
	    		map[pacman.y][pacman.x].icon=pacmantemp;
				
				
}

void Place(){
	char inputPlace=NULL;
	int posX=0; int posY=0;
	while(inputPlace!='1' && inputPlace!='2' && inputPlace!='3' && inputPlace!='4'){
		system("CLS");
		printmap();
		cout << "1.Pacman \n2.Greedy \n3.Random \n4.A*\nInput:";
		inputPlace=getchar();
		fflush(stdin);
		if(inputPlace=='1' ||inputPlace=='2' || inputPlace=='3'  || inputPlace=='4' ){
			do{
			posX=0;
			posY=0;
			while(posX<1 || posX>26){
				cout << "input position X[1-26]:";
				cin >> posX;
			}
			while(posY<1 || posY>29){
				cout << "input position Y[1-29]:";
				cin >> posY;
			}
			if(map[posY][posX].type=="wall"){
				cout<< "position is a wall\n";
			}
			}while(map[posY][posX].type=="wall");
		}
		switch(inputPlace){
			case '1':
				pacman.update(posX,posY,0,"None");
				pacman.spawned=true;
				break;
			case '2':
				greed.update(posX,posY,"None");
				greed.spawned=true;
				break;
			case '3':
				rando.update(posX,posY,"Right");
				rando.spawned=true;
				break;
			case '4':
				star.update(posX,posY,"None");
				star.spawned=true;
				break;
		}
	}
}

void Simulate(){
	while(!(pacman.y==rando.y && pacman.x==rando.x) && !(pacman.y==greed.y && pacman.x==greed.x) && !(pacman.y==star.y && pacman.x==star.x)){
		printmap();
		if(!rando.spawned && !greed.spawned && !star.spawned)break;
		if(rando.spawned==true){
		int varRand=1;
		if(map[rando.y][rando.x].u!=-1 && rando.direction.compare("down")!=0){
				varRand++;
		}
		if(map[rando.y][rando.x].d!=-1 && rando.direction.compare("up")!=0){
				varRand++;
		}
		if(map[rando.y][rando.x].l!=-1 && rando.direction.compare("right")!=0){
				varRand++;
		}
		if(map[rando.y][rando.x].r!=-1 && rando.direction.compare("left")!=0){
				varRand++;
		}
		
		int randoDir = rand() % varRand;
		
		bool cont=false;
		
		if(map[rando.y][rando.x].type.compare("road")!=0){
		if(map[rando.y][rando.x].u!=-1 && !cont && rando.direction.compare("down")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="up";
					cont=true;
				}
		}
		if(map[rando.y][rando.x].d!=-1 && !cont && rando.direction.compare("up")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="down";
					cont=true;
				}
		}
		if(map[rando.y][rando.x].l!=-1 && !cont && rando.direction.compare("right")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="left";
					cont=true;
				}
		}
		if(map[rando.y][rando.x].r!=-1 && !cont && rando.direction.compare("left")!=0){
				if(randoDir>0){
					randoDir--;
				}else{
					rando.direction="right";
					cont=true;
				}
		}
		}
		rando.positionUpdate();
		}
		
		
		
		///////////////////////////////GREED////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		
				if(greed.spawned==true){
				greed.algorithmicDirectionChange(0,true);
				greed.positionUpdate();
				}
		
		///////////////////////////////STAR////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		
				
					if(star.spawned==true){
					star.algorithmicDirectionChange(1,true);
					star.positionUpdate();
					}
		Sleep(60);
		system("CLS");
	}
	createMap(map);
}

void Remove(){
	char inputPlace=NULL;
	while(inputPlace!='1' && inputPlace!='2' && inputPlace!='3' && inputPlace!='4'){
		system("CLS");
		printmap();
		cout << "1.Pacman \n2.Greedy \n3.Random \n4.A*\nInput:";
		inputPlace=getchar();
		fflush(stdin);
		
		switch(inputPlace){
			case '1':
				pacman.update(0,0,0,"None");
				pacman.spawned=false;
				break;
			case '2':
				greed.update(0,0,"None");
				greed.spawned=false;
				break;
			case '3':
				rando.update(0,0,"None");
				rando.spawned=false;
				break;
			case '4':
				star.update(0,0,"None");
				star.spawned=false;
				break;
		}
	}
}

void Free(){
			char inputPointer=NULL;
			while(inputPointer!='4'){
				system("CLS");
				printmap();
				
				cout << "1.Place character\n" << "2.Simulate\n" << "3.remove character\n" << "4.Exit\n" << "Input:";
				inputPointer=getchar();
				fflush(stdin);
				system("CLS");
				switch(inputPointer){
					case '1':Place();break;
					case '2':Simulate();break;
					case '3':Remove();break;
					case '4':return;break;
				}
			}
			
	      	
}

int main() {
		srand (time(NULL));
		createMap(map);
		int s1choice=0;
		while(!(s1choice>=1 && s1choice<=3)){
		cout<<"1.play game\n" << "2.debug game\n" <<"3.algorithm learning\n" <<"input:";
		cin >> s1choice;
		system("CLS");
		getchar();
		}
		switch(s1choice){
		case 1: Game(1);break;
		case 2: Game(2);break;
		case 3: Free();break;
		}
		

}
