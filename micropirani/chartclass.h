#ifndef CHARTCLASS_H
#define CHARTCLASS_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class ChartClass: public QWidget
{
    Q_OBJECT
public:
    explicit ChartClass(QWidget *parent = 0);
    ~ChartClass();

private Q_SLOTS:
    void updateUI();

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    QComboBox *createThemeBox() const;
    QComboBox *createAnimationBox() const;
    QComboBox *createLegendBox() const;
    void connectSignals();
    QChart *createAreaChart() const;
    QChart *createBarChart(int valueCount) const;
    QChart *createPieChart() const;
    QChart *createLineChart() const;
    QChart *createSplineChart() const;
    QChart *createScatterChart() const;
    void mousePressEvent ( QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent * event);

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    QChartView* chartView;
    DataTable m_dataTable;

};


#endif // CHARTCLASS_H
