#ifndef SENSORS_H
#define SENSORS_H

/** Sensor types */
typedef enum
{
  SENSOR_TYPE_ACCELEROMETER = (1), /** < Gravity + linear acceleration */
  SENSOR_TYPE_MAGNETIC_FIELD = (2),
  SENSOR_TYPE_ORIENTATION = (3),
  SENSOR_TYPE_GYROSCOPE = (4),
  SENSOR_TYPE_LIGHT = (5), /** <- TSL2561 */
  SENSOR_TYPE_PRESSURE = (6),
  SENSOR_TYPE_PROXIMITY = (8),
  SENSOR_TYPE_GRAVITY = (9),
  SENSOR_TYPE_LINEAR_ACCELERATION = (10), /** < Acceleration not including gravity */
  SENSOR_TYPE_ROTATION_VECTOR = (11),
  SENSOR_TYPE_RELATIVE_HUMIDITY = (12),
  SENSOR_TYPE_AMBIENT_TEMPERATURE = (13),
  SENSOR_TYPE_VOLTAGE = (15),
  SENSOR_TYPE_CURRENT = (16),
  SENSOR_TYPE_COLOR = (17)
} sensors_type_t;

/** struct sensors_vec_s is used to return a vector in a common format. */
typedef struct
{
    union
    {
        float v[3];
        struct
        {
            float x;
            float y;
            float z;
        };
        /* Orientation sensors */
        struct
        {
            float azimuth;
            float pitch;
            float roll;
        };
    };
    int8_t status;
    uint8_t reserved[3];
} sensors_vec_t;

/** struct sensors_color_s is used to return color data in a common format. */
typedef struct
{
    union
    {
        float c[3];
        /* RGB color space */
        struct
        {
            float r;
            float g;
            float b;
        };
    };
    uint32_t rgba;         /* 24-bit RGB value */
} sensors_color_t;

/* Sensor event (36 bytes) */
/** struct sensor_event_s is used to provide a single sensor event in a common format. */
typedef struct
{
  int32_t version; /**< must be sizeof(struct sensors_event_t) */
  int32_t sensor_id; /**< unique sensor identifier */
  int32_t type; /**< sensor type */
  int32_t reserved0; /**< reserved */
  int32_t timestamp; /**< time is in milliseconds */
  union
  {
    float data[4];
    sensors_vec_t acceleration; /**< acceleration values are in meter per second per second (m/s^2) */
    sensors_vec_t magnetic; /**< magnetic vector values are in micro-Tesla (uT) */
    sensors_vec_t orientation; /**< orientation values are in degrees */
    sensors_vec_t gyro; /**< gyroscope values are in rad/s */
    float temperature; /**< temperature is in degrees centigrade (Celsius) */
    float distance; /**< distance in centimeters */
    float light; /**< light in SI lux units */
    float pressure; /**< pressure in hectopascal (hPa) */
    float relative_humidity; /**< relative humidity in percent */
    float current; /**< current in milliamps (mA) */
    float voltage; /**< voltage in volts (V) */
    sensors_color_t color; /**< color in RGB component values */
  };
} sensors_event_t;

/* Sensor details (40 bytes) */
/** struct sensor_s is used to describe basic information about a specific sensor. */
typedef struct
{
    char     name[12];                        /**< sensor name */
    int32_t  version;                         /**< version of the hardware + driver */
    int32_t  sensor_id;                       /**< unique sensor identifier */
    int32_t  type;                            /**< this sensor's type (ex. SENSOR_TYPE_LIGHT) */
    float    max_value;                       /**< maximum value of this sensor's value in SI units */
    float    min_value;                       /**< minimum value of this sensor's value in SI units */
    float    resolution;                      /**< smallest difference between two values reported by this sensor */
    int32_t  min_delay;                       /**< min delay in microseconds between events. zero = not a constant rate */
} sensor_t;

#endif // SENSORS_H
