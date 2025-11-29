#ifndef __TRANSPOSITION_H
#define __TRANSPOSITION_H

#include <unordered_map>                                            /* Data structure for the transposition table. */

#include <stdlib.h>
#include <string.h>                                                 /* Needed for memcpy(). */

#define _GAMESTATE_BYTE_SIZE         81                             /* Number of bytes needed to store a GameState structure. */
#define _MOVE_BYTE_SIZE               3                             /* Number of bytes needed to store a Move structure. */
#define _NONE                        64                             /* Don't include gamestate just to get a constant. */

#define _TRANSPO_RECORD_BYTE_SIZE    91                             /* Number of bytes needed to store a TranspoRecord object. */
#define _TRANSPO_TABLE_SIZE       65536                             /* Number of TranspoRecords, each 91 bytes. */
#define _TRANSPO_AGE_THRESHOLD       40                             /* Every time the transposition table is serialized, increment every entry's "age",
                                                                       and shed entries older than this threshold. */
#define NODE_TYPE_PV                  0                             /* Score is exact. */
#define NODE_TYPE_ALL                 1                             /* Score is an upper bound. */
#define NODE_TYPE_CUT                 2                             /* Score is a lower bound. */

/**************************************************************************************************
 Typedefs  */

typedef struct TranspoRecordType
  {
    unsigned char gs[_GAMESTATE_BYTE_SIZE];                         //  Game state as byte array (81 bytes).
    unsigned char bestMove[_MOVE_BYTE_SIZE];                        //  Move as byte array (3 bytes).
    unsigned char depth;                                            //  The depth FROM WHICH evaluation of this node is ratified (1 byte).
    float score;                                                    //  32 bits are plenty (4 bytes).
    unsigned char type;                                             //  In {NODE_TYPE_PV, NODE_TYPE_ALL, NODE_TYPE_CUT} as
                                                                    //  score is exact, an upper bound, or lower bound, respectively (1 byte).
    unsigned char age;                                              //  Used to determine when a record should be removed (1 byte).
                                                                    //  If age == 0, then this entry is free to be overwritten.
  } TranspoRecord;

/**************************************************************************************************
 Prototypes  */

bool fetchRecord(unsigned int, TranspoRecord*);
unsigned int tableSize(void);
void setTableSize(unsigned int);
unsigned int hashIndex(unsigned long long);
void serializeTranspoRecord(TranspoRecord*, unsigned char*);
void deserializeTranspoRecord(unsigned char*, TranspoRecord*);
void updateTranspoTable(void);

/**************************************************************************************************
 Globals  */
                                                                    //  6,488,068 bytes = 4 + 65536 * (8 + 91).
                                                                    //  Global array containing the serialized transposition table:
                                                                    //  4-byte uint + (8 bytes + sizeof(TranspoRecord)) * size-of-table.
                                                                    //  NOTE: using a uint for the Transpo length caps your table size at 65,535 = 0xFFFF.
unsigned char transpositionTableBuffer[4 + _TRANSPO_TABLE_SIZE * (8 + _TRANSPO_RECORD_BYTE_SIZE)];

/**************************************************************************************************
 Functions  */

/* Load whatever is at the given index and return whether its 'age' field is greater than zero. */
bool fetchRecord(unsigned int index, TranspoRecord* ttRecord)
  {
    deserializeTranspoRecord(transpositionTableBuffer + 4 + index * (8 + _TRANSPO_RECORD_BYTE_SIZE), ttRecord);
    return (ttRecord->age > 0);
  }

/* Return the number of "live" transposition table entries. */
unsigned int tableSize(void)
  {
    unsigned char i;
    unsigned int ui4 = 0;
    unsigned char buffer4[4];

    for(i = 0; i < 4; i++)                                          //  Copy 4 bytes from the serial buffer.
      buffer4[i] = transpositionTableBuffer[i];
    memcpy(&ui4, buffer4, 4);                                       //  Force the 4-byte buffer into an unsigned int.

    return ui4;
  }

/* Set the transposition table's number of "live" entries to 's'. */
void setTableSize(unsigned int s)
  {
    unsigned char i;
    unsigned char buffer4[4];
    memcpy(buffer4, (unsigned char*)(&s), 4);                       //  Force the unsigned int into a 4-byte temp buffer.
    for(i = 0; i < 4; i++)                                          //  Copy bytes to global byte array's head.
      transpositionTableBuffer[i] = buffer4[i];
    return;
  }

/* Return an index into the transposition table buffer. */
unsigned int hashIndex(unsigned long long h)
  {
    return h % _TRANSPO_TABLE_SIZE;
  }

/* Encode the given TranspoRecord to the given byte array.
   When "buffer" is the global byte array "transpositionTableBuffer", it is at an offset into that array, treated locally. */
void serializeTranspoRecord(TranspoRecord* ttRecord, unsigned char* buffer)
  {
    unsigned int i = 0, j;
    unsigned char buffer4[4];

    for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)
      buffer[i++] = ttRecord->gs[j];
    for(j = 0; j < _MOVE_BYTE_SIZE; j++)
      buffer[i++] = ttRecord->bestMove[j];
    buffer[i++] = ttRecord->depth;
    memcpy(buffer4, (unsigned char*)(&ttRecord->score), 4);         //  Force the float into a 4-byte temp buffer.
    for(j = 0; j < 4; j++)                                          //  Copy bytes to serial buffer.
      buffer[i++] = buffer4[j];
    buffer[i++] = ttRecord->type;
    buffer[i++] = ttRecord->age;

    return;
  }

/* Decode from the given byte array, a TrnaspoRecord.
   When "buffer" is the global byte array "transpositionTableBuffer", it is at an offset into that array, treated locally. */
void deserializeTranspoRecord(unsigned char* buffer, TranspoRecord* ttRecord)
  {
    unsigned int i = 0, j;
    unsigned char buffer4[4];
    float f4;

    for(j = 0; j < _GAMESTATE_BYTE_SIZE; j++)
      ttRecord->gs[j] = buffer[i++];
    for(j = 0; j < _MOVE_BYTE_SIZE; j++)
      ttRecord->bestMove[j] = buffer[i++];
    ttRecord->depth = buffer[i++];
    for(j = 0; j < 4; j++)                                          //  Copy 4 bytes from the serial buffer.
      buffer4[j] = buffer[i++];
    memcpy(&f4, buffer4, 4);                                        //  Force the 4-byte buffer into a float.
    ttRecord->score = f4;
    ttRecord->type = buffer[i++];
    ttRecord->age = buffer[i++];

    return;
  }

/* Advance the 'age' field of every "live" TranspoRecord.
   If it's past the limit, set 'age' to 0 to indicate that this slot is free.
   Write everything back to the global byte array. */
void updateTranspoTable(void)
  {
    unsigned int i;
    unsigned int s = 0;
    TranspoRecord ttRecord;

    for(i = 0; i < _TRANSPO_TABLE_SIZE; i++)
      {
        deserializeTranspoRecord(transpositionTableBuffer + 4 + i * (8 + _TRANSPO_RECORD_BYTE_SIZE), &ttRecord);
        if(ttRecord.age > 0)                                        //  "Live" entry.
          {
            if(++ttRecord.age >= _TRANSPO_AGE_THRESHOLD)            //  Too old?
              ttRecord.age = 0;                                     //  Mark as available.
            else                                                    //  Still valid: increment size counter.
              s++;
                                                                    //  Write updated record to byte array.
            serializeTranspoRecord(&ttRecord, transpositionTableBuffer + 4 + i * (8 + _TRANSPO_RECORD_BYTE_SIZE));
          }
      }

    setTableSize(s);

    return;
  }

#endif
