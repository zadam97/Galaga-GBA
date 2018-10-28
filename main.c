#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"
#include "sprites.h"
#include "text.h"

int pelletdamage = 1;
int lives = 10;
int alienlives = 2;
int shipspeed = 1;//higher is slower
int alienapproachspeed = 20; //higher is slower
int aliensidespeed = 10;//higher is slower
int bulletsnum = 200;
int aliensnum = 12;
int bulletspeed = 1;//higher is slower
int pelletspeed = 1;//higher is slower

int firefreq = 15; //Don't set too low or Game Crashes

SPRITE titlescreen;
SPRITE gameOverscreen;
SPRITE ship;
SPRITE aliens[12];
SPRITE bullets[200];
SPRITE pellets[200];
SPRITE blackbullet;
SPRITE blackAlien;

int bulletcounter = 0;
int loopcounter = 0;
int pelletcounter = 0;

int main(){
      REG_DISPCNT = MODE3 | BG2_ENABLE;

      if (shipspeed > 3){
        shipspeed = 3;
      }


      titlescreen.row = 0;
      titlescreen.col = 0;
      titlescreen.width = TITLE_WIDTH;
      titlescreen.height = TITLE_HEIGHT;
      titlescreen.img = Title;

      gameOverscreen.row = 0;
      gameOverscreen.col = 0;
      gameOverscreen.width = GAMEOVER_WIDTH;
      gameOverscreen.height = GAMEOVER_HEIGHT;
      gameOverscreen.img = GameOver;

      blackbullet.width = BLACKBULLET_WIDTH;
      blackbullet.height = BLACKBULLET_HEIGHT;
      blackbullet.img = BlackBullet;

      blackAlien.width = BLACKALIEN_WIDTH;
      blackAlien.height = BLACKALIEN_HEIGHT;
      blackAlien.img = BlackAlien;

      //Run StartGame
      StartGame();



////////////////////////////////////////////////////////
//////////////////////GAME LOOP/////////////////////////
////////////////////////////////////////////////////////


      while(1)
      {
        //Ship Controls
        if(1){

          if(KEY_DOWN_NOW(BUTTON_LEFT)) {
      			ship.col = ship.col - shipspeed;
            if(ship.col<=0){
              ship.col = 0;
            }
      		}
      		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
      		  ship.col = ship.col + shipspeed;
            if(ship.col>=210){
              ship.col = 210;
            }
      		}

          if(KEY_DOWN_NOW(BUTTON_DOWN)) {
            ship.row = ship.row + shipspeed;
            if(ship.row>=130){
              ship.row = 130;
            }
          }
          if(KEY_DOWN_NOW(BUTTON_UP)) {
            ship.row = ship.row - shipspeed;
            if(ship.row<=0){
              ship.row = 0;
            }
          }

        }

        //Automated Alien Controls
        for(unsigned int i = 0; i < 12; i++){
          if(aliens[i].isHid == SHOW){
            if(loopcounter%aliensidespeed == 0){
              if(aliens[i].col <= 0){
                for(int n = 0; n < 6; n++){
                  aliens[i + n].deltac *= -1;
                }
                aliens[i].col = 0;
              }
              if(aliens[i].col >= 215){
                for(int m = 0; m < 6; m++){
                  aliens[i - m].deltac *= -1;
                }
                aliens[i].col = 215;
              }

              aliens[i].col = aliens[i].col + aliens[i].deltac;

            }
            if(loopcounter%alienapproachspeed == 0){
              aliens[i].row = aliens[i].row + 1;
            }
            //BackUp EndGame Trigger
            if(aliens[i].isHid == SHOW && aliens[i].row >= 105){
              EndGame();
            }
          }


          }
          if(loopcounter%firefreq == 0){

            int r = rand()%12;
            if(aliens[r].isHid == SHOW){
              pellets[pelletcounter].row = aliens[r].row + 27;
              pellets[pelletcounter].col = aliens[r].col + 12;
              pellets[pelletcounter].width = ALIENBULLET_WIDTH;
              pellets[pelletcounter].height = ALIENBULLET_HEIGHT;
              pellets[pelletcounter].isHid = SHOW;
              pellets[pelletcounter].deltac = 0;
              pellets[pelletcounter].deltar = 1;
              pellets[pelletcounter].img = AlienBullet;
              pelletcounter++;
            }
          }


        //SHOOT
        if(KEY_DOWN_NOW(BUTTON_A) && (bulletcounter < bulletsnum) && (loopcounter%8 == 0)){
          bullets[bulletcounter].row = ship.row - 2;
          bullets[bulletcounter].col = ship.col + 15;
          bullets[bulletcounter].width = BULLET_WIDTH;
          bullets[bulletcounter].height = BULLET_HEIGHT;
          bullets[bulletcounter].isHid = SHOW;
          bullets[bulletcounter].deltac = 0;
          bullets[bulletcounter].deltar = -1;
          bullets[bulletcounter].img = Bullet;
          bulletcounter++;
        }

        //Update Bullets Motion
        for(int b = 0; b < bulletcounter; b++){
          if(loopcounter%bulletspeed == 0){
            bullets[b].row += (bullets[b].deltar * 2);
          }

          if(bullets[b].row <= 0 && bullets[b].isHid == SHOW){
            bullets[b].deltar = 0;
            bullets[b].isHid = HID;
            drawImage3(bullets[b].row, bullets[b].col, bullets[b].width, bullets[b].height, blackbullet.img);
          }
        }

        for(int c = 0; c < pelletcounter; c++){
          if(pelletcounter%pelletspeed == 0){
            pellets[c].row += (pellets[c].deltar);
          }

          if(pellets[c].row >= 160 && pellets[c].isHid == SHOW){
            pellets[c].deltar = 0;
            pellets[c].isHid = HID;
            drawImage3(pellets[c].row, pellets[c].col, pellets[c].width, pellets[c].height, blackbullet.img);
          }
        }

        //Check for Bullet Collisions
        for(int t = 0; t < bulletcounter; t++){
          for(int s = 0; s < 12; s++){
            if(aliens[s].isHid == SHOW && bullets[t].isHid == SHOW){
              aliencollision(&bullets[t], &aliens[s]);
            }
          }
        }

        //Check for Pellet Collisions
        for(int l = 0; l < pelletcounter; l++){
          if(pellets[l].isHid == SHOW){
            shipcollision(&pellets[l], &ship);
          }
        }




        //DRAW SCREEN

        draw();
        waitForVblank();
        loopcounter++;

        char snum[5];

        // convert 123 to string [buf]
        sprintf(snum, "%d", ship.health);
        drawImage3(5,50,25,25,blackAlien.img);
        drawString(5,5,"Health: ",RED);
        drawString(5,50,snum,RED);

        //Check if you won
        int won = 1;
        for(int x = 0; x < 12; x++){
          if(aliens[x].isHid == SHOW){
            won = 0;
          }
        }
        if(won){
          drawString(75, 95, "YOU WON", RED);
        }



        //Goes Back to StartGame
        if(KEY_DOWN_NOW(BUTTON_SELECT)){
          StartGame();
        }

        //Bullet and Pellet Checks
        if(bulletcounter >= 195){
          bulletcounter = 195;
        }
        if(pelletcounter >= 195){
          pelletcounter = 195;
        }
      }
}



void StartGame(){
  drawTitle();
  delay(25);
  int wait = 0;
  while(!KEY_DOWN_NOW(BUTTON_START))
	{
		wait++;
	}
  fillScreen(BLACK);

  ship.row = 120;
  ship.col = 105;
  ship.width = SHIP_WIDTH;
  ship.height = SHIP_HEIGHT;
  ship.isHid = SHOW;
  ship.health = lives;
  ship.img = Ship;


  int x = 0;
  for (unsigned int i = 0; i < 2; i++){
    for (unsigned int j = 0; j < 6; j++){
          aliens[x].row = i*30;
          aliens[x].col = 25 + j*30;
          aliens[x].deltar = 1;
          if(i==1){
            aliens[x].deltac = -1;
          } else {
            aliens[x].deltac = 1;
          }
          aliens[x].width = ALIEN_WIDTH;
          aliens[x].height = ALIEN_HEIGHT;
          aliens[x].isHid = SHOW;
          aliens[x].health = alienlives;
          aliens[x].img = Alien;
          x++;
    }
  }
  loopcounter = 0;
  bulletcounter = 0;
  pelletcounter = 0;
}

void EndGame(){
  drawGameOver();
  int wait = 0;
  while(!KEY_DOWN_NOW(BUTTON_START))
	{
		wait++;
	}
  fillScreen(BLACK);
  StartGame();
}

void drawTitle(){
  drawImage3(titlescreen.row, titlescreen.col, titlescreen.width, titlescreen.height, titlescreen.img);
}
void drawGameOver(){
  drawImage3(gameOverscreen.row, gameOverscreen.col, gameOverscreen.width, gameOverscreen.height, gameOverscreen.img);
}

void aliencollision(SPRITE *bullet, SPRITE *alien){
  if(bullet->row < (alien->row + 25) && (bullet->row > alien->row)){
    if(bullet->col < (alien->col + 21) && (bullet->col > alien->col + 4)){
      bullet->isHid = HID;
      bullet->deltar = 0;
      alien->isHid = HID;
      drawImage3(bullet->row, bullet->col, bullets->width, bullets->height, blackbullet.img);
      drawImage3(alien->row, alien->col, alien->width, alien->height, blackAlien.img);
    }
  }
}

void shipcollision(SPRITE *pellet, SPRITE *ship){
  if(pellet->row > (ship->row) - 5){
    if(pellet->col < (ship->col + 25) && (pellet->col > ship->col + 5)){
      pellet->isHid = HID;
      pellet->deltar = 0;
      ship->health -= pelletdamage;
      if(ship->health <= 0){
        EndGame();
      }
      drawImage3(pellet->row, pellet->col, pellet->width, pellet->height, blackbullet.img);
    }
  }
}


void draw(){

  drawImage3(ship.row, ship.col, ship.width, ship.height, ship.img);


  //Draw Aliens
  for(unsigned int a = 0; a < 12; a++){
    if(aliens[a].isHid == 0){
      drawImage3(aliens[a].row, aliens[a].col, aliens[a].width, aliens[a].height, aliens[a].img);
   }
  }
//Draw Bullets
  for(int b = 0; b < bulletcounter; b++){
    if(bullets[b].isHid == SHOW){
      drawImage3(bullets[b].row, bullets[b].col, bullets[b].width, bullets[b].height, bullets[b].img);
    }
  }
//Draw Pellets
  for(int c = 0; c < pelletcounter; c++){
    if(pellets[c].isHid == SHOW){
      drawImage3(pellets[c].row, pellets[c].col, pellets[c].width, pellets[c].height, pellets[c].img);
    }
  }
}
