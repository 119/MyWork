#ifndef _YX_TTS_H_
#define _YX_TTS_H_ 

/* constant for TTS heap size */
//#define ivTTS_HEAP_SIZE		50000 /* �������Ч */
#define ivTTS_HEAP_SIZE		70000 /* �������Ч */

#define RESOUSE_SIZE 3459714

int InitTTS();
int ReleaseTTS();
void MakeTTS(char* v_pText);
void *PlayTTS(void *arg);
int StartTTS();
int StopTTS();

int TTSTest(char* v_pText);

#endif
