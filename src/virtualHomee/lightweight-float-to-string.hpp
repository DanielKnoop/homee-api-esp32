#include <Arduino.h>

void writeFloat(Print &outputStream, double value);
void splitFloat(double value, uint32_t &integralPart,
                uint32_t &decimalPart, int16_t &exponent) ;
int normalizeFloat(double& value) ;
void writeInteger(Print &outputStream, uint32_t value);
void writeDecimals(Print &outputStream, uint32_t value) ;
