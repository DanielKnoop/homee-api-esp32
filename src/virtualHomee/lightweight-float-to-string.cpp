#include "lightweight-float-to-string.hpp"

void writeFloat(Print &outputStream, double value) {
  if (isnan(value)) 
  { 
    outputStream.print("nan");
    return;
  }
  if (value < 0.0) {
    outputStream.print('-');
    value = -value;
  }

  if (isinf(value)) 
  { 
    outputStream.print("inf");
    return;
  }


  uint32_t integralPart, decimalPart;
  int16_t exponent;
  splitFloat(value, integralPart, decimalPart, exponent);

  writeInteger(outputStream, integralPart);
  if (decimalPart) writeDecimals(outputStream, decimalPart);

  if (exponent < 0) {
    outputStream.print("e-");
    writeInteger(outputStream, -exponent);
  }

  if (exponent > 0) {
    outputStream.print('e');
    writeInteger(outputStream, exponent);
  }
}

void splitFloat(double value, uint32_t &integralPart,
                uint32_t &decimalPart, int16_t &exponent) {
  exponent = normalizeFloat(value);

  integralPart = (uint32_t)value;
  double remainder = value - integralPart;

  remainder *= 1e9;
  decimalPart = (uint32_t)remainder;  

  // rounding
  remainder -= decimalPart;
  if (remainder >= 0.5) {
    decimalPart++;
    if (decimalPart >= 1000000000) {
      decimalPart = 0;
      integralPart++;
      if (exponent != 0 && integralPart >= 10) {
        exponent++;
        integralPart = 1;
      }
    }
  }
}

int normalizeFloat(double& value) {
  const double positiveExpThreshold = 1e7;
  const double negativeExpThreshold = 1e-5;
  int exponent = 0;

  if (value >= positiveExpThreshold) {
    if (value >= 1e256) {
      value /= 1e256;
      exponent += 256;
    }
    if (value >= 1e128) {
      value /= 1e128;
      exponent += 128;
    }
    if (value >= 1e64) {
      value /= 1e64;
      exponent += 64;
    }
    if (value >= 1e32) {
      value /= 1e32;
      exponent += 32;
    }
    if (value >= 1e16) {
      value /= 1e16;
      exponent += 16;
    }
    if (value >= 1e8) {
      value /= 1e8;
      exponent += 8;
    }
    if (value >= 1e4) {
      value /= 1e4;
      exponent += 4;
    }
    if (value >= 1e2) {
      value /= 1e2;
      exponent += 2;
    }
    if (value >= 1e1) {
      value /= 1e1;
      exponent += 1;
    }
  }

  if (value > 0 && value <= negativeExpThreshold) {
    if (value < 1e-255) {
      value *= 1e256;
      exponent -= 256;
    }
    if (value < 1e-127) {
      value *= 1e128;
      exponent -= 128;
    }
    if (value < 1e-63) {
      value *= 1e64;
      exponent -= 64;
    }
    if (value < 1e-31) {
      value *= 1e32;
      exponent -= 32;
    }
    if (value < 1e-15) {
      value *= 1e16;
      exponent -= 16;
    }
    if (value < 1e-7) {
      value *= 1e8;
      exponent -= 8;
    }
    if (value < 1e-3) {
      value *= 1e4;
      exponent -= 4;
    }
    if (value < 1e-1) {
      value *= 1e2;
      exponent -= 2;
    }
    if (value < 1e0) {
      value *= 1e1;
      exponent -= 1;
    }
  }

  return exponent;
}

void writeInteger(Print &outputStream, uint32_t value) {
  char buffer[16];
  char *ptr = buffer + sizeof(buffer) - 1;

  *ptr = 0;
  do {
    *--ptr = (char)(value % 10) + '0';
    value = (uint32_t)(value / 10);
  } while (value);

  outputStream.print(ptr);
}

void writeDecimals(Print &outputStream, uint32_t value) {
  int width = 9;

  // remove trailing zeros
  while (value % 10 == 0 && width > 0) {
    value /= 10;
    width--;
  }
 
  char buffer[16];
  char *ptr = buffer + sizeof(buffer) - 1;

  // write the string in reverse order
  *ptr = 0;
  while (width--) {
    *--ptr = value % 10 + '0';
    value /= 10;
  }
  *--ptr = '.';

  // and dump it in the right order
  outputStream.print(ptr);
}