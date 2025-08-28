#include <stdio.h>
#include <unistd.h>
#include <raylib.h>
#include "rl_io.h"


RlTextBuffer* rlScrollUp(RlTextBuffer *buf) {
   for (int line = 0; line<RL_LINES-1; line++){
      for (int col = 0; col<RL_COLS; col++){
         (*buf)[line][col] = (*buf)[line+1][col];
      };
   };
   for (int col = 0; col<RL_COLS; col++){
      (*buf)[RL_LINES-1][col] = '\0';
   };
   return buf;
}
RlTextBuffer* rlWriteLine(RlTextBuffer *buf, char* string, int line) {
   for (int col = 0; col<RL_COLS; col++){
      (*buf)[line-1][col] = string[col];
   };
   return buf;
}

RlTextBuffer* rlWriteNewLine(RlTextBuffer *buf, char* string) {
   rlScrollUp(buf);
   rlWriteLine(buf, string, RL_LINES);
   return buf;
}

RlTextBuffer* rlClearTextBuffer(RlTextBuffer *buf) {
   for (int line = 0; line<RL_LINES; line++){
      for (int col = 0; col<RL_COLS; col++){
         (*buf)[line][col] = '\0';
      };
   };
   return buf;
}


RlTextBuffer* rlReplaceChar(RlTextBuffer *buf, char character, int line, int col) {
   (*buf)[line-1][col-1] = character;
   return buf;
}

RlTextBuffer* rlPutNewChar(RlTextBuffer *buf, char character) {
   if (character == '\n'){
      rlScrollUp(buf);
      return buf;
   }
      for (int col = 0; col<RL_COLS-1; col++){
         if ((*buf)[RL_LINES-1][col] == '\0'){
            (*buf)[RL_LINES-1][col] = character;
            return buf;
         };
   };
   rlScrollUp(buf);
   (*buf)[RL_LINES-1][0] = character;
   return buf;
}


char rlHandleInput(RlTextBuffer *buf) {
   if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
      rlClearTextBuffer(buf);
      return '\0';
   }
   if (IsKeyDown(KEY_CAPS_LOCK) && IsKeyPressed(KEY_L)) {
      rlClearTextBuffer(buf);
      return '\0';
   }

   int key = GetKeyPressed();
   if (key == KEY_ENTER) {
      return '\n';
   }
   return GetCharPressed();
}

void rlDrawTextBuffer(RlTextBuffer *buf) {
   ClearBackground(BLACK);
   for (int line = 0; line < RL_LINES; line++) {
      for (int col = 0; col < RL_COLS-1; col++) {
         if ((*buf)[line][col] == '\0') break;
         char character[2];
         character[0] = (*buf)[line][col];
         character[1] = '\0';
         DrawText(character, col * 9+40, line*12+12, 10, RAYWHITE);
      }
      // Cursor logic
      int cursor_line = RL_LINES - 1;
      int cursor_col = 0;
      while (cursor_col < RL_COLS && (*buf)[cursor_line][cursor_col] != '\0')
         cursor_col++;
      bool show_cursor = ((int)(GetTime() * 2) % 2) == 0; // Blinks every 0.5s
      if (show_cursor && cursor_col < RL_COLS) {
         DrawText("_", cursor_col * 9 + 40, cursor_line * 12 + 12, 10, WHITE);
      }
   }
}

void rlTest() {
   RlTextBuffer buf;
   rlClearTextBuffer(&buf);
   InitWindow(800, 600, "Raylib Display Buffer Example");
   SetTargetFPS(60);
   while (!WindowShouldClose()) {
      char character = rlHandleInput(&buf);
      rlPutNewChar(&buf, character);
      BeginDrawing();
      rlDrawTextBuffer(&buf);
      EndDrawing();
   }
   CloseWindow();
#ifdef DEBUG
   write(1, (char*)buf, sizeof(buf));
#endif
}
