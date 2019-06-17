#ifndef JTAGSTATE_H
#define JTAGSTATE_H

#include <QObject>
#include<QWidget>
#include <QDialog>
#include<QShowEvent>
#include<QCloseEvent>
#include<QResizeEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QIcon>
#include <QSvgRenderer>
#include<QSvgGenerator>
#include<QGraphicsSvgItem>
#include <QGraphicsSvgItem>
#include <QInputEvent>

#include"jtag_sm.h"
#include"jtag_cmd.h"


namespace Ui {
  class JTAGstate;
}

typedef enum {
  TEST_LOGIC_RESET =0,
  RUN_TEST_IDLE =1,
  SELECT_DR_SCAN =2,
  CAPTURE_DR =3,
  SHIFT_DR =4,
  EXIT1_DR =5,
  PAUSE_DR =6,
  EXIT2_DR =7,
  UPDATE_DR =8,
  SELECT_IR_SCAN =9,
  CAPTURE_IR =10,
  SHIFT_IR =11,
  EXIT1_IR =12,
  PAUSE_IR =13,
  EXIT2_IR =14,
  UPDATE_IR =15
             } JStates;

class JTAGstate : public QDialog
{
  Q_OBJECT

public:
  explicit JTAGstate(QWidget *parent = nullptr);
  ~JTAGstate();

 const QMap<JStates,QString> st_map = {
    {TEST_LOGIC_RESET ,"TEST_LOGIC_RESET"},
    {RUN_TEST_IDLE    ,"RUN_TEST_IDLE"   },
    {SELECT_DR_SCAN   ,"SELECT_DR_SCAN"  },
    {CAPTURE_DR       ,"CAPTURE_DR"      },
    {SHIFT_DR         ,"SHIFT_DR"        },
    {EXIT1_DR         ,"EXIT1_DR"        },
    {PAUSE_DR         ,"PAUSE_DR"        },
    {EXIT2_DR         ,"EXIT2_DR"        },
    {UPDATE_DR        ,"UPDATE_DR"       },
    {SELECT_IR_SCAN   ,"SELECT_IR_SCAN"  },
    {CAPTURE_IR       ,"CAPTURE_IR"      },
    {SHIFT_IR         ,"SHIFT_IR"        },
    {EXIT1_IR         ,"EXIT1_IR"        },
    {PAUSE_IR         ,"PAUSE_IR"        },
    {EXIT2_IR         ,"EXIT2_IR"        },
    {UPDATE_IR        ,"UPDATE_IR"       }
  };

 const QMap<JStates,QPoint> st_led ={

   {TEST_LOGIC_RESET ,QPoint(359,65)    },
   {RUN_TEST_IDLE    ,QPoint(338 ,200 ) },
   {SELECT_DR_SCAN   ,QPoint(860 ,200 ) },
   {CAPTURE_DR       ,QPoint(840 , 340) },
   {SHIFT_DR         ,QPoint(840 , 490) },
   {EXIT1_DR         ,QPoint(840 , 620) },
   {PAUSE_DR         ,QPoint(840 , 760) },
   {EXIT2_DR         ,QPoint(840 , 910) },
   {UPDATE_DR        ,QPoint(840 , 1050)},
   {SELECT_IR_SCAN   ,QPoint(1360 , 200)},
   {CAPTURE_IR       ,QPoint(1350 , 340)},
   {SHIFT_IR         ,QPoint(1350 , 490)},
   {EXIT1_IR         ,QPoint(1350 , 620)},
   {PAUSE_IR         ,QPoint(1350 , 760)},
   {EXIT2_IR         ,QPoint(1350 , 910)},
   {UPDATE_IR        ,QPoint(1350 , 1050)}

 };

signals:
  void req_tdo();
  void st_close(int res);

public slots:
  void tck(bool lvl);
  void tms(bool lvl);
  void tdi(bool lvl);
  void tdo(bool lvl);
  void ex_op(JTAGOp command);
  void ex_dat(JTAGDat command);
  void ex_cmdlist(QList<JTAGcmd> *commands);


private slots:
  void on_bHi_clicked();

  void on_bLow_clicked();

  void upd_state();
  void stlog(const QString &label, const QString &msg);

  void showEvent(QShowEvent * eve);
  void closeEvent(QCloseEvent * event);
  void resizeEvent(QResizeEvent *event);

  //void paintEvent(QPaintEvent *);


private:
  Ui::JTAGstate *ui;
  jtag_sm* machine;

  struct{
    bool tck = false;
    bool tms = false;
    bool tdi = false;
    bool tdo = false;
  } level;
  QGraphicsScene scene;
  QIcon diagram = QIcon (":/res/JTAG_TAP_Controller_State_Diagram.svg");
  QSvgRenderer svgrend;
  QGraphicsSvgItem qgsvgit = QGraphicsSvgItem(":/res/JTAG_TAP_Controller_State_Diagram.svg");
  QGraphicsEllipseItem led;
  QGraphicsTextItem dr_val;
  QGraphicsTextItem ir_val;
  bool jshow = false;
  QSizeF scale;


};

#endif // JTAGSTATE_H
