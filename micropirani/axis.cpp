#include "axis.h"

Axis::Axis(QObject *parent,float min, float max, float step, bool is_log) : QObject(parent),_min(min),_max(max),_step(step),_is_log(is_log)
{
    _signal_min = _min;
    _signal_max = _max;
    generate_labels();
}

void Axis::set_min(float val)
{
 /*   if(is_log())
        _min = log10(val);
    else*/
        _min = val;
}

void Axis::set_max(float val)
{
/*    if(is_log())
        _max = log10(val);
    else*/
        _max = val;
}

void Axis::offset_range(float val)
{
    set_min(min()+val);
    set_max(max()+val);
    generate_labels();
}

void Axis::set_step(float val)
{
     _step = val;
    generate_labels();
}

void Axis::set_lin(bool is_lin)
{
    _is_log = !is_lin;
}


float Axis::min(){
    if(is_log())
        return logmin();
    else
        return linmin();
}

float Axis::max(){
    if(is_log())
        return logmax();
    else
        return linmax();
}

float Axis::width(){
    if(is_log())
        return logwidth();
    else
        return linwidth();
}

float Axis::step(){
    if(is_log())
        return logstep();
    else
        return linstep();
}

int Axis::steps(){
    if(is_log())
        return logsteps();
    else
        return linsteps();
}

float Axis::linmin(){return _min;}
float Axis::linmax(){return _max;}
float Axis::linwidth(){return (max() - min());}
float Axis::linstep(){return _step;}
int Axis::linsteps(){return width()/_step;}

float Axis::logmin(){return log10(_min);}
float Axis::logmax(){return log10(_max);}
float Axis::logwidth(){return (logmax()-logmin());}
float Axis::logstep(){return log10(_step);}
int Axis::logsteps(){return logwidth()/logstep();}

bool Axis::is_ovf(float val){return ( val >=linmax() );}
bool Axis::is_unf(float val){return ( val <= linmin() );}

float Axis::val_to_scale(float val) {
    if(is_log())
        return log10(val);
    else
        return val;

}

float Axis::scale_to_val(float scal){
    if(is_log())
        return pow(10,scal);
    else
        return scal;
}

const QList<AxeLabel>* Axis::get_labels(){return &_labels;}

bool Axis::is_log(){return _is_log;}

void Axis::range_reset()
{
    _in_range = false;
    _signal_max = _max;
    _signal_min = _min;
}

void Axis::range_start()
{
    _signal_min_curr = _max;
    _signal_max_curr = _min;
    _in_range = true;
}

void Axis::range(float val)
{
    if(_in_range){
        if(val> _signal_max_curr) _signal_max_curr = val;
        if(val< _signal_min_curr) _signal_min_curr = val;
    }
}

void Axis::range_end()
{
    _in_range = false;
    _signal_min = _signal_min_curr;
    _signal_max = _signal_max_curr;
}

float Axis::get_range_min()
{
    return _signal_min;
}

float Axis::get_range_max()
{
    return _signal_max;
}

void Axis::generate_labels(){
    QList<AxeLabel> s;
    AxeLabel a;
    for(int i = 1; i < steps(); i++) {
                a.val = min()+((i)*step());
                QString txt;
                txt.sprintf("%1.0e", scale_to_val(a.val));
                a.text = txt;
        s<< a;
    }
    _labels = s;
}
