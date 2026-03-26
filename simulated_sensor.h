#ifndef SIMULATED_SENSOR_H
#define SIMULATED_SENSOR_H

#include "sensor.h"

class SimulatedSensor : public Sensor
{
public:
    SimulatedSensor(double manimum_value, double maximum_value, unsigned resulution);

    SimulatedSensor()                                            = default; //< Standard constructor
    SimulatedSensor &operator=(const SimulatedSensor &other)     = default; ///< Assignment operator (=)
    SimulatedSensor &operator=(SimulatedSensor &&other) noexcept = default; ///< Move operator (=)
    SimulatedSensor(const SimulatedSensor &other)                = default; ///< Copy constuctor
    SimulatedSensor(SimulatedSensor &&other) noexcept            = default; ///< Move constuctor
    virtual ~SimulatedSensor()                                   = default; ///< Destructor

    double measure() override;

private:
    double _minimum_value{.0};
    double _maximum_value{.0};
    unsigned _resulution{1};
};

#endif // SIMULATED_SENSOR_H
