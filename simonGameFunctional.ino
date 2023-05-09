const int GAME_OPTIONS_DEFAULT_LENGTH = 4;
const int GAME_SEQUENCE_DEFAULT_LENGTH = 50;
const int GAME_DEFAULT_DELAY_MS = 300;

int resetButtonPort = 27;

struct GameOption {
  int ledOutputPort;
  int buttonInputPort;
};

GameOption gameOptions[GAME_OPTIONS_DEFAULT_LENGTH] = {
  {18, 34},
  {17, 35},
  {4, 32},
  {19, 33}
};

int gameSequenceLength = 0;
GameOption* gameSequence[GAME_SEQUENCE_DEFAULT_LENGTH];

int gameAnswearIndex = 0;
int isAnswearing = false;

void setup() {
  Serial.begin(9600);
  setupGameOptions();
  blinkOptions();
}

void loop() {
  if(isAnswearing){
    verifyAnswear();
  }else{
    addRandomOptionToSequence();
    blinkSequence();
    isAnswearing = true;
  }

  verifyAndReset();
}

void verifyAndReset(){
  if(digitalRead(resetButtonPort) == HIGH){
    resetGame();
  }
}

void addGameOptionToSequence(GameOption gameOption){
  if(gameSequenceLength < GAME_SEQUENCE_DEFAULT_LENGTH){
    gameSequence[gameSequenceLength] = new GameOption(gameOption);
    gameSequenceLength++;
  }
}

void addRandomOptionToSequence(){
  addGameOptionToSequence(gameOptions[random(0, GAME_OPTIONS_DEFAULT_LENGTH)]);
}

void cleanSequence() {
  for(int i = 0; i < gameSequenceLength; i++){
    delete gameSequence[i];
  }
  gameSequenceLength = 0;
}

void cleanAnswer(){
  gameAnswearIndex = 0;
  isAnswearing = false;    
}

void setupResetButton(){
  pinMode(resetButtonPort, INPUT);
}

void setupGameOption(GameOption gameOption){
  pinMode(gameOption.ledOutputPort, OUTPUT);
  pinMode(gameOption.buttonInputPort, INPUT);
}

void setupGameOptions(){
  for(int i = 0; i < GAME_OPTIONS_DEFAULT_LENGTH; i++){
    setupGameOption(gameOptions[i]);
  }

  setupResetButton();
}

bool verifyPressedOptionButton(GameOption gameOption) {
  if(digitalRead(gameOption.buttonInputPort) == HIGH){
    return true;
  }
  return false;
}

bool verifySequenceOption(int sequenceIndex, GameOption gameOption) {
  if(gameSequence[sequenceIndex]->ledOutputPort == gameOption.ledOutputPort && gameSequence[sequenceIndex]->buttonInputPort == gameOption.buttonInputPort){
    return true;
  }

  return false;
}

void verifyAnswear(){
  if(verifySomeButtonPressed()){
    GameOption gameOption = getOptionPressed();
    blink(gameOption);
    if(verifySequenceOption(gameAnswearIndex, gameOption)){
      gameAnswearIndex++;
      if(gameAnswearIndex >= gameSequenceLength){
        addRandomOptionToSequence();
        blinkSequence();
        gameAnswearIndex = 0;
      }
    }else{
      resetGame();
    }
  }
}

void resetGame(){
  cleanSequence();
  blinkOptions();
  cleanAnswer();
}

GameOption getOptionPressed(){
  GameOption gameOption = GameOption();
  for(int i=0; i < GAME_OPTIONS_DEFAULT_LENGTH; i++){
    if(verifyPressedOptionButton(gameOptions[i])){
      gameOption = gameOptions[i];
    }
  }

  return gameOption;
}

bool verifySomeButtonPressed() {
  bool isSomeButtonPressed = false;
  for(int i=0; i < GAME_OPTIONS_DEFAULT_LENGTH; i++){
    if(verifyPressedOptionButton(gameOptions[i])){
      isSomeButtonPressed = true;
    }
  }

  return isSomeButtonPressed;
}

void blink(GameOption gameOption){
  digitalWrite(gameOption.ledOutputPort, HIGH);
  delay(GAME_DEFAULT_DELAY_MS);
  digitalWrite(gameOption.ledOutputPort, LOW);
  delay(GAME_DEFAULT_DELAY_MS);
}

void blinkOptions() {
  for(int i = 0; i < GAME_OPTIONS_DEFAULT_LENGTH; i++){
    digitalWrite(gameOptions[i].ledOutputPort, HIGH);
  }

  delay(GAME_DEFAULT_DELAY_MS);

  for(int i = 0; i < GAME_OPTIONS_DEFAULT_LENGTH; i++){
    digitalWrite(gameOptions[i].ledOutputPort, LOW);
  }

  delay(GAME_DEFAULT_DELAY_MS);
}

void blinkSequence() {
  for(int i = 0; i < gameSequenceLength; i++){
    blink(*gameSequence[i]);
  }
}


