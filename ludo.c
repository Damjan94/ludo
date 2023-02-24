#include <raylib.h>
#include <assert.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "ui_element.h"
#include "ui_layout.h"
#include "piece.h"
#include "raymath.h"
#include "defines.h"
#include "game_board.h"
#include "home.h"
#include "path.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_paragraph.h"
#include "utils.h"

#include "debug.h"

void update();
void render();

Texture2D textures[NUMBER_OF_TEAMS];

Texture2D board;
Camera2D camera = {0};
float zoom = 1.0f;
struct GameBoard gameBoard = {0};

bool exitButtonClicked = false;
bool shouldExitGame() {
  return exitButtonClicked || WindowShouldClose();
}

Vector2 homeSpots[] = {
  {.x = 1.75, .y = 1.5},
  {.x = 3.75, .y = 1.5},
  {.x = 1.75, .y = 3.5},
  {.x = 3.75, .y = 3.5},

  {.x = 11, .y = 1.5},
  {.x = 13, .y = 1.5},
  {.x = 11, .y = 3.5},
  {.x = 13, .y = 3.5},

  {.x = 11, .y = 10.5},
  {.x = 13, .y = 10.5},
  {.x = 11, .y = 12.5},
  {.x = 13, .y = 12.5},

  {.x = 1.75, .y = 10.5},
  {.x = 3.75, .y = 10.5},
  {.x = 1.75, .y = 12.5},
  {.x = 3.75, .y = 12.5},

};

const int startingSquares[] = {
  0,
  13,
  26,
  39
} ;

const int firstGoalSquares[] = {
  52,
  58,
  64,
  70
};


void assertCorrectSideRange(enum Side side) {
  assert(side >= 0);
  assert(side < NUMBER_OF_TEAMS);
}
int getFirstSquareIndex(enum Side side) {
  assertCorrectSideRange(side);
  return startingSquares[side];
}

int getFirstGoalSquareIndex(enum Side side) {
  assertCorrectSideRange(side);
  return firstGoalSquares[side];
}

const Vector2* getHomeSquarePositions(enum Side side) {
  assertCorrectSideRange(side);
  return &homeSpots[side * NUMBER_OF_TEAMS];
}


const Texture2D* getPieceTexture(enum Side side) {
  assertCorrectSideRange(side);
  return &(textures[side]);
}

struct Square *getMutableSquareByIndex(int index) {
  return &(gameBoard.squares[index]);
}

void goHome(struct Piece* piece) {
  struct Home* home = &(gameBoard.teams[piece->side].home);
  homePlacePiece(home, piece);
}

const struct Square* getSquareMouseClickedOn(Vector2);
struct Piece* getPieceOnPosition(Vector2 clickPosition) {
  const struct Square* squareMouseWasOn = getSquareMouseClickedOn(clickPosition);
  if(!squareMouseWasOn) {
    return 0;
  }  
  return getPieceOnSquare(squareMouseWasOn);
}
struct Piece* getPieceMouseClickedOnCallback(enum Side side) {
 if(!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    return 0;
  }
  Vector2 clickPosition = GetMousePosition();
  Vector2 worldClick = GetScreenToWorld2D(clickPosition, camera);
  struct Piece* pieceClickedOn = getPieceOnPosition(worldClick);
  if(pieceClickedOn && !pieceBelongsToTeam(pieceClickedOn, gameBoard.teams[side].pieces)) {
    pieceClickedOn = 0;
  }
  return pieceClickedOn;
}



struct CheckBox teamsInPlayCheckBox[NUMBER_OF_TEAMS];
bool shouldShowMainMenu = true;
void onPlayButtonClicked() {
  bool hasSingleTeamChecked = false;
  for(size_t i = 0; i < NUMBER_OF_TEAMS; ++i) {
    if (teamsInPlayCheckBox[i].isChecked) {
      hasSingleTeamChecked = true;
    }
  }
  if(!hasSingleTeamChecked) {
    return;
  }
  shouldShowMainMenu = false;
}

struct Button initPlayButton() {
  struct Text text = createText("Play", FONT_SIZE, FONT_COLOR);
  return buttonCreate(text, onPlayButtonClicked);
}


void onExitButtonClicked() {
  exitButtonClicked = true;
}
struct Button initExitButton() {
  struct Text text = createText("Exit", FONT_SIZE, FONT_COLOR);
  return buttonCreate(text, onExitButtonClicked);
}


const char* getTeamName(enum Side side) {
  switch(side) {
  case Red: return "Red";
  case Green: return "Green";
  case Yellow: return "Yellow";
  case Blue: return "Blue";
  case None: return"ERROR!";
  }
  return "ERROR!";
}
struct CheckBox initUseOwnDiceCheckBox() {
  struct Text text = createText("Use own dice", FONT_SIZE, FONT_COLOR);
  struct CheckBox checkbox = checkBoxCreate(text, false);
  return checkbox;
}
void initTeamsInPlayCheckBox(struct CheckBox teamsInPlay[NUMBER_OF_TEAMS]) {
  for(int i = 0; i < NUMBER_OF_TEAMS; ++i) {
    struct Text text = createText(getTeamName(i), FONT_SIZE, FONT_COLOR);
    teamsInPlay[i] = checkBoxCreate(text, false);
  }
}
int main() {
  const Vector2 screenSize = {.x = 700, .y = 700};

  InitWindow(screenSize.x, screenSize.y, "Ne Ljuti se covece!");


  SetTargetFPS(60);

  textures[Red] = LoadTexture("assets/red_piece_trans.png");
  textures[Green] = LoadTexture("assets/green_piece_trans.png");
  textures[Yellow] = LoadTexture("assets/yellow_piece_trans.png");
  textures[Blue] = LoadTexture("assets/blue_piece_trans.png");

  board = LoadTexture("assets/Ludo_Board_main.png");
  camera.offset = (Vector2){ screenSize.y/2.0f, screenSize.x/2.0f };
  camera.zoom = (float)screenSize.x / board.width;;
  camera.target = (Vector2) {board.width / 2.0f, board.height / 2.0f};

  initGameBoard(
    &gameBoard,
    getFirstSquareIndex,
    getFirstGoalSquareIndex,
    getPieceTexture,
    getHomeSquarePositions,
    getMutableSquareByIndex,
    goHome
    );  

  Vector2 smallOffset = {0, screenSize.y / 50};
  Vector2 bigOffset = {0, screenSize.y / 20};

  struct Layout mainMenu = createLayout(screenSize);

  struct Button playButton = initPlayButton();

  struct CheckBox useOwnDiceCheckBox = initUseOwnDiceCheckBox();
  initTeamsInPlayCheckBox(teamsInPlayCheckBox);
  teamsInPlayCheckBox[0].isChecked = true; // Make red
  teamsInPlayCheckBox[2].isChecked = true; // and yellow defalut players
  layoutAddElement(&mainMenu, &(playButton.element));
  layoutAddElement(&mainMenu, &(useOwnDiceCheckBox.element));
  uiElementAddOffset(&(useOwnDiceCheckBox.element), smallOffset);
  for(size_t i = 0; i < sizeof(teamsInPlayCheckBox) / sizeof(teamsInPlayCheckBox[0]); ++i) {
    layoutAddElement(&mainMenu, &(teamsInPlayCheckBox[i].element));
    Vector2 offset = smallOffset;
    if(i == 0) {
      offset = bigOffset;
    }
    uiElementAddOffset(&(teamsInPlayCheckBox[i].element), offset); 
  }

  struct Button exitButton = initExitButton();
  layoutAddElement(&mainMenu, &(exitButton.element));
  uiElementAddOffset(&(exitButton.element), bigOffset);
  
  struct Paragraph* paragraph = paragraphCreate();

  struct Text instructions = createText("Welcome to ludo. The rules are simple. There can only be one piece at each square at a time. Press SPACE to roll the dice(UP and DOWN arrow sets the dice if you're using own dice.). Click on a piece to move it. Enemy can't capture you if you're on a square with own color", FONT_SIZE / 2, FONT_COLOR);
  paragraphAddText(paragraph, instructions);
  layoutAddElement(&mainMenu, paragraphGetElement(paragraph));

  size_t aboutFontSize = FONT_SIZE / 3;
  struct Paragraph* aboutParagraph = paragraphCreate();
  struct Text createdBy = createText("This game was created by", aboutFontSize, FONT_COLOR);
  struct Link damjan = linkCreate(" Damjan ", aboutFontSize, BLUE, "https://www.github.com/damjan94");
  struct Text contributions = createText("The artwork was provided by the good people at", aboutFontSize, FONT_COLOR);
  struct Link openGameArt = linkCreate(" OpenGameArt ", aboutFontSize, BLUE, "https://www.opengameart.org/content/ludo");

  paragraphAddText(aboutParagraph, createdBy);
  paragraphAddLink(aboutParagraph, damjan);
  paragraphAddText(aboutParagraph, contributions);
  paragraphAddLink(aboutParagraph, openGameArt);
  
  layoutAddElement(&mainMenu, paragraphGetElement(aboutParagraph));
  
  while(!shouldExitGame()) {
    if(shouldShowMainMenu) {
      mainMenu.element.updateUiElement(&(mainMenu.element)); 
      useOwnDice(&gameBoard, useOwnDiceCheckBox.isChecked);
      for(int i = 0; i < NUMBER_OF_TEAMS; ++i) {
        bool isTeamInPlay = teamsInPlayCheckBox[i].isChecked;
        setSkipSide(i, !isTeamInPlay);
        if(isTeamInPlay) {
          gameBoard.currentPlayerTurn = i;
        }      
      } 
    }
    update();

    
    BeginDrawing();
    render();
    
    if(shouldShowMainMenu) {
      mainMenu.element.renderUiElement(&(mainMenu.element));
    }
    EndDrawing();

  }

  CloseWindow();
  paragraphDestroy(paragraph);
  paragraph = 0;
  return 0;
}

int diceRoll = 0;
int getDiceRoll() {
  if (!gameBoard.usingOwnDice) {
    diceRoll = GetRandomValue(1, 6);
  }
  return diceRoll;
}
void update() {
  if(gameBoard.usingOwnDice && gameBoard.currentState == AboutToRollDice) {
    if(IsKeyPressed(KEY_UP) && diceRoll < 6) {
      diceRoll++;
    }
    if(IsKeyPressed(KEY_DOWN) && diceRoll > 1) {
      diceRoll--;
    }
  } 
  
  updateGameBoard(&gameBoard, getPieceMouseClickedOnCallback, getDiceRoll);
}
  
bool checkIfPointInsideSquare(Vector2 point, const struct Square* square) {
    const Rectangle squareRect = {.x = square->position.x * SQUARE_WIDTH, .y = square->position.y * SQUARE_HEIGHT, .width = SQUARE_WIDTH, .height = SQUARE_HEIGHT};
    return CheckCollisionPointRec(point, squareRect);
}

const struct Square* getRegularSquareMouseClickedOn(Vector2 clickPosition) {
  const struct Square* foundSquare = 0;
  for(size_t i = 0; i < BOARD_SIZE; ++i) {
    const struct Square* square = &(gameBoard.squares[i]);
    if(checkIfPointInsideSquare(clickPosition, square)) {
      foundSquare = square;
    }
  }

  return foundSquare;
}

const struct Square* getHomeSquareMouseClickedOn(Vector2 clickPosition) {
  const struct Square* foundSquare = 0;
  for(size_t i = 0; i < NUMBER_OF_TEAMS; ++i) {
    for(size_t j = 0; j < NUMBER_OF_PIECES_PER_TEAM; ++j) {
      const struct Square* square = &(gameBoard.teams[i].home.homeSquares[j]);
      if(checkIfPointInsideSquare(clickPosition, square)) {
        foundSquare = square;
      }
    }
  }
  return foundSquare;
}
const struct Square* getSquareMouseClickedOn(Vector2 clickPosition){
  const struct Square* square = getHomeSquareMouseClickedOn(clickPosition);
  if(!square) {
    square = getRegularSquareMouseClickedOn(clickPosition);
  }
  return square;
}



void renderPieces();

void render() {
  const struct Team* currentTeam = &(gameBoard.teams[gameBoard.currentPlayerTurn]);
  BeginMode2D(camera);

  ClearBackground(RAYWHITE);
  DrawTexture(board, 0, 0, RAYWHITE);

  Vector2 centerOfBoard = {.x = (float)board.width / 2, (float)board.height / 2};
  DrawCircle(centerOfBoard.x, centerOfBoard.y, 60, currentTeam->color);
  if(gameBoard.currentState == AboutToRollDice) {
    DrawCircle(centerOfBoard.x, centerOfBoard.y, 60, ColorAlpha(RAYWHITE, 0.3));
  }
  DrawText(TextFormat("%d", diceRoll), centerOfBoard.x - 25, centerOfBoard.y - 25, 50, BLACK);

  renderPieces();


  //renderDebug(gameBoard.squares, BOARD_SIZE);
  EndMode2D();
}



void renderPieces() {
  for(int i = 0; i < NUMBER_OF_TEAMS; ++i) {
    for(int j = 0; j < NUMBER_OF_PIECES_PER_TEAM; ++j) {
      const struct Piece* piece = &(gameBoard.teams[i].pieces[j]);
      Vector2 position = offsetPositionBySquareSize(piece->path->square->position);
      DrawTextureEx(*(piece->image), position,0, 0.2,  RAYWHITE);
    }
  }
}
