#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

int RST = 9, CE = 10, DC = 11, DIN = 12, CLK = 13;
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CE, RST);
const int PIN_UP = A4;
const int PIN_DOWN = A3;
const int PIN_FIRE = 2;
const int PIN_POTEN = A0;

const unsigned char white[] PROGMEM = {
  0x38, 0x38, 0x38, 0x38, 0xc1, 0x94, 0xe1, 0xcc, 0xe1, 0xcc, 0xc1, 0x94, 0x38, 0x38, 0x38, 0x38
};

const unsigned char black[] PROGMEM = {
  0x3b, 0x38, 0x3b, 0xb8, 0xfe, 0x74, 0xfe, 0x3c, 0xfe, 0x3c, 0xfe, 0x74, 0x3b, 0xb8, 0x3b, 0x38
};

// 'lanes', 84x19px
const unsigned char lanes [] PROGMEM = {
	0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 
	0x00
};

// 'finishLine', 6x35px
const unsigned char finishLine [] PROGMEM = {
	0x30, 0x30, 0xcc, 0xcc, 0x30, 0x30, 0xcc, 0xcc, 0x30, 0x30, 0xcc, 0xcc, 0x30, 0x30, 0xcc, 0xcc, 
	0x30, 0x30, 0xcc, 0xcc, 0x30, 0x30, 0xcc, 0xcc, 0x30, 0x30, 0xcc, 0xcc, 0x30, 0x30, 0xcc, 0xcc, 
	0x30, 0x30, 0xcc
};

class AutomatedCar {
  public:
    const int W = 14;
    const int H = 8;
    int X = 0;
    int Y = 0;
    const int Y_MIN = 2;
    const int Y_MAX = 29;
    const unsigned char* bitmap;

    AutomatedCar(const unsigned char* bmp, int x, int y) {
      bitmap = bmp;
      X = x;
      Y = y;
    }

    void draw() {
      display.drawBitmap(X, Y, bitmap, W, H, BLACK);
    }
    
    void moveUp() {
      Y = max(Y - 9, Y_MIN);
    }
    
    void moveDown() {
      Y = min(Y + 9, Y_MAX);
    }

    void accelerate() {
      int randomX = random(1, 4);
      int movement = (randomX*8);
        for(int i = 0; i <= movement; i += 2){
          if(X >= 32) break; // Si la posición X supera el límite de 24, detener el ciclo
          X += 2;
        }
    }

    void changeLane(int otherCar, int playerCarX, int playerCarY) {
      int newY;
      do {
        newY = (otherCar == (Y - 9)) ? Y + 9 : (otherCar == (Y + 9)) ? Y - 9 : Y + random(0, 2) * 18 - 9;
      } while (abs(playerCarX - X) <= W && abs(newY - playerCarY) <= H);
      Y = max(Y_MIN, min(Y_MAX, newY));
    }

};

class ControlledCar : public AutomatedCar {
  public:
    int sensorValue = 0;
    float convertedValue = 0.00;
    float ratio = 0.00;
    int accelerationValue = 0;
    int previousAccelerationValue = 0;
    float currentSpeed = 2;
    const int MAX_DISPLACAMENT = 15;

    ControlledCar(const unsigned char* bmp, int x, int y) : AutomatedCar(bmp, x, y) {}

    void draw() {
      //initialDraw();
      readAcceleration();
      readMovements();
      display.drawBitmap(X, Y, bitmap, W, H, BLACK);
    }

    void initialDraw(){
      if(X>=0 && X<=6){
        X++;
      }
    }

    void accelerate() {
      float accelerationFactor = 0.05;
      float maxSpeed = 3.0;
        if(accelerationValue>5 && accelerationValue<10 && accelerationValue>previousAccelerationValue){
          if(X<MAX_DISPLACAMENT){
            X+=2;
          }else{
            currentSpeed = min(maxSpeed, currentSpeed * (1 + accelerationFactor));
          }
        }
        if(accelerationValue<5 && accelerationValue>0 && accelerationValue<previousAccelerationValue){
          if (X > 0) {X-=2;}
          if(X<MAX_DISPLACAMENT){
            currentSpeed = min(maxSpeed, currentSpeed * (1 - accelerationFactor));
          }
        }
        previousAccelerationValue = accelerationValue;
      }
      
      void readMovements() {
        int bUp = digitalRead(PIN_UP);
        int bDown = digitalRead(PIN_DOWN);
        if (bUp == LOW) {
          moveUp();
        } else if (bDown == LOW) {
          moveDown();
        }
      }

      void readAcceleration(){ 
        sensorValue = analogRead(PIN_POTEN);
        ratio = (10.0) / (1023.0);
        convertedValue = (sensorValue) * ratio;
        accelerationValue = ceil(convertedValue);
        accelerate();
      }
};

class Obstacle {
  public:
    int X = 0;
    int Y = 0;
    int W = 5;
    int H = 8;
    bool isActive = false;

    void activate(int x, int yValues[4]) {
      X = x-W;
      Y = yValues[random(0, 4)];
      isActive = true;
    }

    void deactivate() {
      isActive = false;
    }
    
    void draw(){
      display.setCursor(X,Y);
      display.write(0x04);
    }

    void move() {
      X-=2;
    }
};

class Game {
  public:
    const int DISPLAY_WIDTH = 84;
    const int ROAD_HEIGHT = 35;
    static const int MAX_OBSTACLES = 3;
    const long OBSTACLE_INTERVAL = 2000;
    const long PCCAR_INTERVAL = 3000;
    unsigned long previousMillisObstacle = 0;
    unsigned long previousMillisPCCars = 0;
    unsigned long currentMillis = 0;
    bool menuSelected = false;
    unsigned long cx = 0;
    int yValues[4] = {2, 11, 20, 29};
    ControlledCar* playerCar = new ControlledCar(white,0,11);
    AutomatedCar* pcCarL = new ControlledCar(black,0,2);
    AutomatedCar* pcCarR = new ControlledCar(black,0,20); 
    Obstacle obstacles[MAX_OBSTACLES];
    int runCounter = 0;
    bool hasCrossedFinishLine = false;
    //unsigned long temp = 0;
    unsigned long startTime = 0; // Variable para guardar el tiempo de inicio del temporizador
    unsigned long elapsedTime = 0; // Variable para guardar el tiempo transcurrido
    const unsigned long TIMER_DURATION = 60000; // Duración del temporizador en milisegundos (60 segundos)


    void showMenu(){
      display.fillRect(0,0,DISPLAY_WIDTH,48,BLACK);
      display.setCursor(10, 20);
      display.fillRect(0,20,DISPLAY_WIDTH,8,WHITE);
      display.setTextColor(BLACK);
      display.print("Start Game");
      display.display();
    }

    void readButton() {
      int bFire = digitalRead(PIN_FIRE);
      
      if (bFire == LOW) {
        menuSelected = true;
        startTime = millis();
      } 
    }

    void play(){
      currentMillis = millis();
      updateScrolling();
      if(runCounter>=9){
        drawFinishLine();

      }else{
        drawLanes();
      }
      playerCar->draw();
      pcCarL->draw();
      pcCarR->draw();
      drawObstacles();
      MovePCcars();
      showTimer();
      display.display();
      detectCollision();
      detectCarCollision(pcCarL, playerCar);
      detectCarCollision(pcCarR, playerCar);
    }

    void showTimer() {
    display.fillRect(5,39,DISPLAY_WIDTH,13,BLACK);
    display.setTextColor(WHITE);
    display.setCursor(5, 39);
    elapsedTime = millis() - startTime;
    int remainingTime = (TIMER_DURATION - elapsedTime) / 1000; // Calcular tiempo restante en segundos
    display.print("Time left: ");
    if (remainingTime >= 0) { // Si todavía queda tiempo
      display.print(remainingTime);
    } else { // Si el tiempo ha finalizado
      showResult("Time Is Over");
      restartValues();
    }
  }

    void drawLanes(){
      display.fillRect(0,2,DISPLAY_WIDTH,ROAD_HEIGHT,WHITE);
      display.drawBitmap(cx, 10, lanes, DISPLAY_WIDTH, 19, BLACK);
      display.drawBitmap(cx+DISPLAY_WIDTH, 10,  lanes, DISPLAY_WIDTH, 19, BLACK);
    }

    void drawFinishLine(){
      display.fillRect(0,2,DISPLAY_WIDTH,ROAD_HEIGHT,WHITE);
      display.drawBitmap(cx, 10, lanes, DISPLAY_WIDTH, 19, BLACK);
      display.drawBitmap(cx+DISPLAY_WIDTH, 10,  lanes, DISPLAY_WIDTH, 19, BLACK);
      display.drawBitmap(cx+DISPLAY_WIDTH, 2,  finishLine, 6, ROAD_HEIGHT, BLACK);
      if ((playerCar->X+playerCar->W) >= (cx+DISPLAY_WIDTH+12) && !hasCrossedFinishLine) {
          hasCrossedFinishLine = true;
      }
    }

    void drawObstacles(){
      display.setTextColor(BLACK);
      if (currentMillis - previousMillisObstacle >= OBSTACLE_INTERVAL) {
        previousMillisObstacle = currentMillis;
        // Buscar la siguiente posición disponible en el arreglo de obstáculos
        for (int i = 0; i < MAX_OBSTACLES; i++) {
          if (!obstacles[i].isActive) {
            obstacles[i].activate(DISPLAY_WIDTH,yValues);
            break;
          }
        }
      }

      // Iterar sobre el arreglo de obstáculos y dibujar los activos
      for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].isActive) {
          obstacles[i].move();
          obstacles[i].draw();
          // Verificar si el obstáculo ha llegado al final de la pantalla y desactivarlo
          if (obstacles[i].X <= 0) {
            obstacles[i].deactivate();
          }
        }
      }
    }

    void updateScrolling() {
      if (playerCar->currentSpeed > 2) { // Si la velocidad de desplazamiento es mayor que 1, entonces ajusta las posiciones en X de los elementos
        cx -= playerCar->currentSpeed <= 2.35 ? 3 : playerCar->currentSpeed <= 2.70 ? 4 : 5;
        if (pcCarR->X > 0) {
          pcCarR->X -= (playerCar->currentSpeed);
        }
        if (pcCarL->X > 0) {
          pcCarL->X -= (playerCar->currentSpeed);
        }
        for (int i = 0; i < MAX_OBSTACLES; i++) {
          if (obstacles[i].isActive) {
            obstacles[i].X -= (playerCar->currentSpeed);
          }
        }
      } else{
         cx -= 2;
      }
      
      if (cx <= -DISPLAY_WIDTH) {
        cx = 0;
        runCounter++;
      }
    }

    void detectCollision() {
      if(!hasCrossedFinishLine){
        for (int i = 0; i < MAX_OBSTACLES; i++) {
          if (obstacles[i].isActive) {
            // Compara la coordenada X del carro con la coordenada X del obstáculo
            if (playerCar->X + playerCar->W >= obstacles[i].X && playerCar->X <= obstacles[i].X + obstacles[i].W) {
              // Verifica si están en la misma posición vertical
              if (playerCar->Y + playerCar->H >= obstacles[i].Y && playerCar->Y <= obstacles[i].Y + obstacles[i].H) {
                // Hay colisión, detener el juego o mostrar un mensaje de "Game Over"
                showResult("Game Over :(");
                restartValues();
                break;
              }
            }
          }
        }
      }
    }

    void detectCarCollision(AutomatedCar* car1, AutomatedCar* car2) {
      // Compara la coordenada X de ambos carros
      if (car1->X + car1->W >= car2->X && car1->X <= car2->X + car2->W) {
        // Verifica si están en la misma posición vertical
        if (car1->Y + car1->H >= car2->Y && car1->Y <= car2->Y + car2->H) {
          // Hay colisión, detener el juego o mostrar un mensaje de "Game Over"
          showResult("Game Over :(");
          restartValues();
        }
      }
    }

    void MovePCcars(){
      const int MAX_SPEED_FOR_PC_CARS = 2;
      bool isPlayerCarSpeedLow = playerCar->currentSpeed <= MAX_SPEED_FOR_PC_CARS;
      bool isChangeLaneTime = currentMillis - previousMillisPCCars >= PCCAR_INTERVAL;
        
        if (isChangeLaneTime) {
          previousMillisPCCars = currentMillis;
          if (playerCar->Y <= pcCarR->Y && isPlayerCarSpeedLow) {
            pcCarL->accelerate();
          } else { 
            pcCarL->changeLane(pcCarR->Y, playerCar->X, playerCar->Y);
          }
          if (playerCar->Y <= pcCarL->Y && isPlayerCarSpeedLow) {
            pcCarR->accelerate();
          } else {
            pcCarR->changeLane(pcCarL->Y, playerCar->X, playerCar->Y);
          }
        }
    }

    void showResult(String message){
      display.fillRect(0,0,DISPLAY_WIDTH,48,BLACK);
      display.setCursor(5, 20);
      display.fillRect(0,20,DISPLAY_WIDTH,8,WHITE);
      display.setTextColor(BLACK);
      display.print(message);
      delay(1000);
      display.display();
      delay(1500);
    }

    finish(){
      if( (playerCar->X+14 > pcCarR->X+14) && (playerCar->X+14 > pcCarL->X+14) ){
        showResult("You Win :)");
      }else{
        showResult("You Lose :(");
      }
      restartValues();
    }

    void restartValues(){
      previousMillisObstacle = millis();
      previousMillisPCCars = millis();
      currentMillis = millis();
      menuSelected = false;
      hasCrossedFinishLine = false;
      cx = 0; 
      playerCar->currentSpeed = 2;
      playerCar->X = 0;
      playerCar->Y = 11;
      pcCarR->X = 0;
      pcCarR->Y = 20;
      pcCarL->X = 0;
      pcCarL->Y = 2;
      runCounter = 0;
      for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].deactivate();
      }
    }
};

Game game;

void setup() {
  Serial.begin(9600);
  _display_initialization();
  _buttons_initialization();
}

void loop() {
  while(!game.menuSelected){
    game.showMenu();
    game.readButton();
  }
  if(game.hasCrossedFinishLine){
    game.finish();
  }else{
    game.play();
  }
}
