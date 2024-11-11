# Example SHT3X with ESP32 (ESP-IDF)

## What it does

The example configures a `sht3x` device.

When `CONFIG_EXAMPLE_SHT3X_DEMO_HL` is defined, the task that triggers a
measurement every 5 seconds. Due to power efficiency reasons it uses *single
shot* mode. In this example it uses the high level function *sht3x_measure()* to
perform one measurement in each cycle.

When `CONFIG_EXAMPLE_SHT3X_DEMO_LL` is defined, the task that triggers a
measurement every 5 seconds. Due to power efficiency reasons it uses *single
shot* mode. In this example it starts the measurement, waits for the results
and fetches the results using separate functions.

Choose either `CONFIG_EXAMPLE_SHT3X_DEMO_HL` or `CONFIG_EXAMPLE_SHT3X_DEMO_LL`
in `make menuconfig` under `Example configuration`. The default is
`CONFIG_EXAMPLE_SHT3X_DEMO_HL`.

## Wiring


Connect `SCL` and `SDA` pins to the following pins with appropriate pull-up
resistors.

| Name | Description | Defaults |
|------|-------------|----------|
| `CONFIG_EXAMPLE_I2C_MASTER_SCL` | GPIO number for `SCL` | "5" for `esp8266`, "6" for `esp32c3`, "19" for `esp32`, `esp32s2`, and `esp32s3` |
| `CONFIG_EXAMPLE_I2C_MASTER_SDA` | GPIO number for `SDA` | "4" for `esp8266`, "5" for `esp32c3`, "18" for `esp32`, `esp32s2`, and `esp32s3` |

## Don't forget to edit the CMakeList section

set(EXTRA_COMPONENT_DIRS "C:/Users/Haydar/Desktop/components/sht3x" 
"C:/Users/Haydar/Desktop/components/i2cdev" 
"C:/Users/Haydar/ Desktop/components/esp_idf_lib_helpers" ) #replace with the location where the libraries are stored

## Example Output
```bash
I (265) app_start: Starting scheduler on CPU0
I (269) main_task: Started on CPU0
I (269) main_task: Calling app_main()
I (269) main_task: Returned from app_main()
SHT3x Sensör: 22.89 °C, 36.20 %
SHT3x Sensör: 22.80 °C, 36.19 %
SHT3x Sensör: 22.83 °C, 36.18 %
SHT3x Sensör: 22.82 °C, 36.16 %
SHT3x Sensör: 22.82 °C, 36.20 %
```
