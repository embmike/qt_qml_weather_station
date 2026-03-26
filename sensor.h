#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
public:
    virtual ~Sensor() = default;

    virtual double measure() = 0;
};

#endif // SENSOR_H
