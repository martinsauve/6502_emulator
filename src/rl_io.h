#ifndef RL_IO
#define RL_IO

#define RL_COLS  80
#define RL_LINES 48

typedef char RlTextBuffer[RL_LINES][RL_COLS];

char rlHandleInput(RlTextBuffer *buf);
RlTextBuffer* rlPutNewChar(RlTextBuffer *buf, char character);
RlTextBuffer* rlClearTextBuffer(RlTextBuffer *buf);
void rlDrawTextBuffer(RlTextBuffer *buf);

void rlTest();
#endif
