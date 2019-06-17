#ifndef JTAG_CMD_H
#define JTAG_CMD_H
#include<QMap>

typedef enum{TCK = 0, TMS = 1, TDI = 2, TDO = 3, NONVALID = 4}JTAGPin;
typedef enum{OP = 0, DAT = 1}JType;
typedef enum{READ = 0, WRITE = 1}JDir;
typedef struct{JTAGPin pin; bool st;}JTAGOp;
typedef struct{JDir dir; uint8_t len; uint8_t b_arr[256];}JTAGDat;
typedef struct{
  JType type = JType::OP;
  union{
    JTAGOp op;
    JTAGDat dat;
  };
}JTAGcmd;
//typedef QMap<QString,QList<JTAGOp>> JTAGcmd;

#endif // JTAG_CMD_H
