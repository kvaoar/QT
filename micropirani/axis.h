#ifndef AXIS_H
#define AXIS_H
#include"axislabel.h"
#include <QObject>

class Axis : public QObject
{
    Q_OBJECT
public:
    explicit Axis(QObject *parent = nullptr,float min = 0.0, float max=0.0, float step=0.0, bool is_log=false);

    void set_min(float val);
    void set_max(float val);
    void offset_range(float val);
    void set_step(float val);
    void set_lin(bool is_lin);

    float min();
    float max();
    float width();
    float step();
    int steps();

    float linmin();
    float linmax();
    float linwidth();
    float linstep();
    int linsteps();

    float logmin();
    float logmax();
    float logwidth();
    float logstep();
    int logsteps();

    bool is_ovf(float val);
    bool is_unf(float val);

    float val_to_scale(float val) ;
    float scale_to_val(float scal);
    const QList<AxeLabel>* get_labels();

    bool is_log();

    void range_reset();
    void range_start();
    void range(float val);
    void range_end();
    float get_range_min();
    float get_range_max();

    void generate_labels();
private:

    bool  _in_range = false;
    float _signal_min_curr;
    float _signal_max_curr;

    float _signal_min;
    float _signal_max;

    float _min;
    float _max;
    float _step;
    bool _is_log;
    QList<AxeLabel> _labels;

signals:

public slots:
};

#endif // AXIS_H
