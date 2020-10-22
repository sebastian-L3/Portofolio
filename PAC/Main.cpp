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
		
		heuristic=pow(height,2)-pow(base,2);
		if(heuristic<0)heuristic=heuristic*-1;
		heuristic=sqrt(heuristic);
	}
};

//global
pixel map[31][28];

class pac{
		public:
	int x;
	int y;
	int eat;
	string direction;
	
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

class ghost{
		public:
	int x;
	int y;
	string direction;
	
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
};

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

int main() {
		srand (time(NULL));
		
		
		pac pacman;
		ghost rando;
		ghost greed;
		
		rando.update(26,28,"down");
		pacman.update(1,1,0,"none");
		greed.update(23,1,"down");
		createMap(map);
		
		
		int varRand;
		int randoDir;
		
		int Plive=3;
		
		//count down
		
		cout << "3 ";
		Sleep(300);
		cout << "2 ";
		Sleep(300);
		cout << "1 ";
		Sleep(300);
		cout << "GO! ";
		Sleep(300);
		
		
		while(pacman.eat<280 && Plive>=1){
			system("CLS");
			
			if((pacman.y==rando.y && pacman.x==rando.x) || (pacman.y==greed.y && pacman.x==greed.x)){
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
					rando.update(26,28,"down");
				}
				if(greed.y<5 && greed.x<5){
				greed.update(23,1,"down");
				}
			}
			
			char tempIconRando=map[rando.y][rando.x].icon;
			char tempIconGreed=map[greed.y][greed.x].icon;
			map[pacman.y][pacman.x].icon='C';
			map[rando.y][rando.x].icon='@';
			map[greed.y][greed.x].icon='$';
			
			//printmap
			for (int i = 0; i < 31; i++) {
				for (int j = 0; j < 28; j++) {
				cout << map[i][j].icon;
				}
			cout << "\n";
			
		}
		
		
		//print meta data///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(true){
		cout << "LIFES:" << Plive << "		SCORE:"<< pacman.eat << endl;
		cout << pacman.direction << "|";
		cout << "down:" <<map[(pacman.y)+1][pacman.x].type ;
		cout << "  up:" << map[(pacman.y)-1][pacman.x].type ;
		cout << "  right:" <<map[(pacman.y)][(pacman.x)+1].type ;
		cout << "  left:" << map[(pacman.y)][(pacman.x)-1].type << endl;
		cout << "PACMAN|X:" << pacman.x << "Y:" << pacman.y << "	Greed|X:" << greed.x << "Y:" << greed.y << "	Rando|X:" << rando.x << "Y:" << rando.y << endl; 
		}
		
		if(map[pacman.y][pacman.x].type.compare("intersection")!=0){Sleep(50);}
		else{Sleep(250);}
		
		
		//input
		if(true){
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
		
		///////////////////////////////RANDO////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		map[rando.y][rando.x].icon=tempIconRando;
		
		
		varRand=1;
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
		
		randoDir = rand() % varRand;
		
		cout << "Rando|random variable :" << randoDir << "	routes:" << varRand-1 << "\n";
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
		
		///////////////////////////////GREED////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		cout << "Greed| Current Direction:" <<greed.direction << endl;
		
		vector<node> vgreed;
		int dToInter1=0,dToInter2=0;
		node temp;

		if(map[greed.y][greed.x].type.compare("intersection")!=0){
			if(map[greed.y+1][greed.x].type.compare("wall")==0 && map[greed.y-1][greed.x].type.compare("wall")==0){
				while(map[greed.y][greed.x-dToInter1].type.compare("intersection")!=0){
					dToInter1++;
				}
				while(map[greed.y][greed.x+dToInter2].type.compare("intersection")!=0){
					dToInter2++;
				}
			
				temp.update(dToInter1,greed.x-dToInter1,greed.y,"left");
				temp.insertHeuristic(greed.x-dToInter1,greed.y,pacman.x,pacman.y);

				vgreed.insert(vgreed.begin(),temp);
				temp.update(dToInter2,greed.x+dToInter2,greed.y,"right");
				temp.insertHeuristic(greed.x+dToInter2,greed.y,pacman.x,pacman.y); 
				}
			else if(map[greed.y][greed.x+1].type.compare("wall")==0 && map[greed.y][greed.x-1].type.compare("wall")==0){
				while(map[greed.y-dToInter1][greed.x].type.compare("intersection")!=0){
					dToInter1++;
				}
				while(map[greed.y+dToInter2][greed.x].type.compare("intersection")!=0){
					dToInter2++;
				}
			
				
				temp.update(dToInter1,greed.x,greed.y-dToInter1,"up");
				temp.insertHeuristic(greed.x,greed.y-dToInter1,pacman.x,pacman.y);

				vgreed.insert(vgreed.begin(),temp);
				temp.update(dToInter2,greed.x,greed.y+dToInter2,"down");
				temp.insertHeuristic(greed.x,greed.y+dToInter2,pacman.x,pacman.y); 
				}
				
			if(vgreed.at(0).heuristic>temp.heuristic){
				vgreed.insert(vgreed.begin() + 0,temp);
			}else{
				vgreed.insert(vgreed.begin() + 1,temp);
			}
		}else{
			int ixs=0;
			if(map[greed.y][greed.x].u!=-1){
				temp.update(map[greed.y][greed.x].u,greed.x,greed.y-map[greed.y][greed.x].u,"up");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				vgreed.insert(vgreed.begin() + 0,temp);
			}
		
			if(map[greed.y][greed.x].d!=-1){
				temp.update(map[greed.y][greed.x].d,greed.x,greed.y+map[greed.y][greed.x].d,"down");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				ixs=0;
				if(vgreed.size()>0){
				while(ixs<vgreed.size()){
							if(vgreed.at(ixs).heuristic>temp.heuristic){
								vgreed.insert(vgreed.begin()+ixs,temp);
								break;
							}
							if(ixs==vgreed.size()-1){
								vgreed.insert(vgreed.begin()+ixs+1,temp);
								break;
							}
							ixs++;
				}
			}else{
				vgreed.insert(vgreed.begin() + 0,temp);
			}
			}
			if(map[greed.y][greed.x].l!=-1){
				temp.update(map[greed.y][greed.x].l,greed.x-map[greed.y][greed.x].l,greed.y,"left");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				ixs=0;
				if(vgreed.size()>0){
				while(ixs<vgreed.size()){
							if(vgreed.at(ixs).heuristic>temp.heuristic){
								vgreed.insert(vgreed.begin()+ixs,temp);
								break;
							}
							if(ixs==vgreed.size()-1){
								vgreed.insert(vgreed.begin()+ixs+1,temp);
								break;
							}
							ixs++;
					}
			}else{
				vgreed.insert(vgreed.begin() + 0,temp);
			}
			}
			
			if(map[greed.y][greed.x].r!=-1){
				temp.update(map[greed.y][greed.x].r,greed.x+map[greed.y][greed.x].r,greed.y,"right");
				temp.insertHeuristic(temp.posX,temp.posY,pacman.x,pacman.y);
				ixs=0;
				if(vgreed.size()>0){
				while(ixs<vgreed.size()){
							if(vgreed.at(ixs).heuristic>temp.heuristic){
								vgreed.insert(vgreed.begin()+ixs,temp);
								break;
							}
							if(ixs==vgreed.size()-1){
								vgreed.insert(vgreed.begin()+ixs+1,temp);
								break;
							}
							ixs++;
					}
			}else{
				vgreed.insert(vgreed.begin() + 0,temp);
			}
			}
		
		}
		//greed print size vector
//		cout << "greedy perform   ";
//			cout << "greed size" << vgreed.size() << "\n";
//		for(int ix=0;ix<vgreed.size();ix++){
//			cout << ix << " " << vgreed.at(ix).heuristic << "\n";
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
				vector<node> greedpopped;
				
				while((vgreed.at(0).posX!=goal1.posX || vgreed.at(0).posY!=goal1.posY)&& (vgreed.at(0).posX!=goal2.posX || vgreed.at(0).posY!=goal2.posY)){
					node current=vgreed.at(0);
					cout << "X " <<current.posX << " Y "<<current.posY << "  ";
					greedpopped.insert(greedpopped.begin(),vgreed.at(0));
					vgreed.erase(vgreed.begin());
					node tempo;
					//UP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					if(map[current.posY][current.posX].u!=-1){
					
						tempo.update(current.curr+map[current.posY][current.posX].u,current.posX,current.posY-map[current.posY][current.posX].u,current.direction);
						tempo.insertHeuristic(tempo.posX,tempo.posY,pacman.x,pacman.y);
						int i=0;
						bool bye=false;
						for(int ixx=0;ixx<greedpopped.size();ixx++){
							if(greedpopped.at(ixx).posX==tempo.posX && greedpopped.at(ixx).posY==tempo.posY){
								bye=true;
								break;
							}
						}
						while(i<vgreed.size()){
							if(bye)break;
							if(vgreed.at(i).heuristic>tempo.heuristic){
								vgreed.insert(vgreed.begin()+i,tempo);
								break;
							}
							if(i==vgreed.size()-1){
								vgreed.insert(vgreed.begin()+i+1,tempo);
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
					
						for(int ixx=0;ixx<greedpopped.size();ixx++){
							if(greedpopped.at(ixx).posX==tempo.posX && greedpopped.at(ixx).posY==tempo.posY){
								bye=true;
								break;
							}
						}
					
						while(i<vgreed.size()){
							if(bye)break;
							if(vgreed.at(i).heuristic>tempo.heuristic){
								vgreed.insert(vgreed.begin()+i,tempo);
								break;
							}
							if(i==vgreed.size()-1){
								vgreed.insert(vgreed.begin()+i+1,tempo);
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
						for(int ixx=0;ixx<greedpopped.size();ixx++){
							if(greedpopped.at(ixx).posX==tempo.posX && greedpopped.at(ixx).posY==tempo.posY){
								bye=true;
								break;
							}
						}
						while(i<vgreed.size()){
							if(bye)break;
							if(vgreed.at(i).heuristic>tempo.heuristic){
								vgreed.insert(vgreed.begin()+i,tempo);
								break;
							}
							if(i==vgreed.size()-1){
								vgreed.insert(vgreed.begin()+i+1,tempo);
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
						for(int ixx=0;ixx<greedpopped.size();ixx++){
							if(greedpopped.at(ixx).posX==tempo.posX && greedpopped.at(ixx).posY==tempo.posY){
								bye=true;
								break;
							}
						}
						while(i<vgreed.size() && !bye){
							if(bye)break;
							if(vgreed.at(i).heuristic>tempo.heuristic){
								vgreed.insert(vgreed.begin()+i,tempo);
								break;
							}
							if(i==vgreed.size()-1){
								vgreed.insert(vgreed.begin()+i+1,tempo);
								break;
							}
							i++;
						}
					}	
				
				}
				if(map[pacman.y][pacman.x].type.compare("road")==0 && greedpopped.size()<3){
					if(vgreed.at(0).posX==goal1.posX && vgreed.at(0).posY==goal1.posY){
						greed.direction=goal1.direction;
					}
					else if(vgreed.at(0).posX==goal2.posX && vgreed.at(0).posY==goal2.posY){
						greed.direction=goal2.direction;
					}
				}
				else{
				greed.direction=vgreed.at(0).direction;
				}
				map[greed.y][greed.x].icon=tempIconGreed;
				greed.positionUpdate();
				cout << "\nDirection after greedy:" <<greed.direction << endl;
				//getchar();
		
		
		
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
	getchar();
	fflush(stdin);
	}
}
