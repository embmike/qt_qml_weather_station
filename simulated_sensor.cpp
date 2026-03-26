#include "simulated_sensor.h"

SimulatedSensor::SimulatedSensor(double minimum_value, double maximum_value, unsigned resulution)
    : _minimum_value{minimum_value}, _maximum_value{maximum_value}, _resulution{resulution}
{}

double SimulatedSensor::measure()
{
    /* Todo: Implementierung mittels Zufallsgenerator erstellen.
     * Skalierte zufallsgenerierter Zahl = (_minimum_value <= zufallsgenerierte Zahl <= _maximum_value) / _resulution
     */

    return .0;
}
