#include "thermostat.h"
#include "Sensor.h"

const char *nameof_typecheck(const void *, const char *name) { return name; }
#define nameof(A) nameof_typecheck((void *)&A, #A)

void Thermostat::SerializeBody(Stream &client, long currentTimestamp, int indentLevel)
{
    Sensor::SerializeBody(client, currentTimestamp, indentLevel);
    client.printf(",");
    printIndent(client, indentLevel);
    client.printf("\"%s\": \"%s\",", nameof(externalTemperatureSensorId), externalTemperatureSensorId.c_str());
    printIndent(client, indentLevel);
    client.printf("\"%s\": \"%s\",", nameof(temperatureSensor), temperatureSensor.c_str());
    printIndent(client, indentLevel);
    client.printf("\"%s\": %g,", nameof(offset), offset / 10.0);
    printIndent(client, indentLevel);
    client.printf("\"%s\": %lu,", nameof(offsetTimestamp), offsetTimestamp);
    printIndent(client, indentLevel);
    client.printf("\"%s\": %g,", nameof(targetTemperature), targetTemperature / 10.0);
    printIndent(client, indentLevel);
    client.printf("\"%s\": %g,", nameof(targetTemperatureHigh), targetTemperatureHigh / 10.0);
    printIndent(client, indentLevel);
    client.printf("\"%s\": %g,", nameof(targetTemperatureLow), targetTemperatureLow / 10.0);

    // serialize reference sensor
    printIndent(client, indentLevel);
    client.printf("\"%s\": \"%s\"", nameof(referenceSensor), referenceSensor.c_str());
}

uint8_t PresetAction::hours() const
{
    return (time >> 8) & 0xFF;
}

uint8_t PresetAction::minutes() const
{
    return time & 0xFF;
}

bool PresetAction::enabled() const
{
    return (state & 0x80) != 0;
}

int PresetAction::order() const
{
    return (((int)firstDay()) << 24) | time;
}

uint8_t PresetAction::daysOfWeekMask() const
{
    return (state & 0x7F);
}

void PresetAction::addDayOfWeek(int8_t dayOfWeek)
{
    state = state | (1 << dayOfWeek);
}

uint8_t PresetAction::firstDay() const
{
    return __builtin_ctz(state);
    // return _tzcnt_u32(state);
}