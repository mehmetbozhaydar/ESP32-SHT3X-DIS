#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <sht3x.h>
#include <string.h>
#include <esp_err.h>

/* printf() fonksiyonunda float tipini kullanmak için belirli bir yapılandırma gerekmektedir. 
 * ESP8266 için SDK yapılandırması varsayılan olarak etkinleştirilmiştir. 
 * Ayrıntılar için sdkconfig.defaults.esp8266 dosyasına bakın.
 */

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

static sht3x_t dev;

#if defined(CONFIG_EXAMPLE_SHT3X_DEMO_HL)

/*
 * Kullanıcı görevi, her 5 saniyede bir ölçüm tetikler. Güç verimliliği açısından 
 * tek seferlik ölçüm modunu kullanır. Bu örnekte, her döngüde tek ölçüm yapmak 
 * için yüksek seviyeli *sht3x_measure* fonksiyonunu kullanır.
 */
void task(void *pvParameters)
{
    float temperature;
    float humidity;

    TickType_t last_wakeup = xTaskGetTickCount();

    while (1)
    {
        // Bir ölçüm yap ve sonuçları kullan
        ESP_ERROR_CHECK(sht3x_measure(&dev, &temperature, &humidity));
        printf("SHT3x Sensör: %.2f °C, %.2f %%\n", temperature, humidity);

        // 5 saniye boyunca bekleyin
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(5000));
    }
}

#elif defined(CONFIG_EXAMPLE_SHT3X_DEMO_LL)

/*
 * Kullanıcı görevi, her 5 saniyede bir ölçüm tetikler. Güç verimliliği açısından 
 * tek seferlik ölçüm modunu kullanır. Bu örnekte, ölçümü başlatır, sonuçları 
 * bekler ve sonuçları almak için ayrı fonksiyonlar kullanır.
 */
void task(void *pvParameters)
{
    float temperature;
    float humidity;

    TickType_t last_wakeup = xTaskGetTickCount();

    // Yüksek tekrarlanabilirlik için ölçüm süresini al
    uint8_t duration = sht3x_get_measurement_duration(SHT3X_HIGH);

    while (1)
    {
        // Yüksek tekrarlanabilirlikle tek seferlik bir ölçüm başlatın
        ESP_ERROR_CHECK(sht3x_start_measurement(&dev, SHT3X_SINGLE_SHOT, SHT3X_HIGH));

        // Ölçüm hazır olana kadar bekleyin (en az 30 ms veya *sht3x_get_measurement_duration* süresi)
        vTaskDelay(duration);

        // Değerleri al ve bir şeyler yap
        ESP_ERROR_CHECK(sht3x_get_results(&dev, &temperature, &humidity));
        printf("SHT3x Sensör: %.2f °C, %.2f %%\n", temperature, humidity);

        // 5 saniye boyunca bekleyin
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(5000));
    }
}

#else // CONFIG_SHT3X_DEMO_PERIODIC
/*
 * Kullanıcı görevi, her 2 saniyede bir sensörün en son ölçüm sonuçlarını alır. 
 * SHT3x'i saniyede 1 ölçümle (SHT3X_PERIODIC_1MPS) periyodik moda başlatır.
 */
void task(void *pvParameters)
{
    float temperature;
    float humidity;
    esp_err_t res;

    // Saniyede 1 ölçümle periyodik ölçümleri başlatın
    ESP_ERROR_CHECK(sht3x_start_measurement(&dev, SHT3X_PERIODIC_1MPS, SHT3X_HIGH));

    // İlk ölçüm hazır olana kadar bekleyin (en az 30 ms veya *sht3x_get_measurement_duration* süresi)
    vTaskDelay(sht3x_get_measurement_duration(SHT3X_HIGH));

    TickType_t last_wakeup = xTaskGetTickCount();

    while (1)
    {
        // Değerleri al ve bir şeyler yap
        if ((res = sht3x_get_results(&dev, &temperature, &humidity)) == ESP_OK)
            printf("SHT3x Sensör: %.2f °C, %.2f %%\n", temperature, humidity);
        else
            printf("Sonuç alınamadı: %d (%s)", res, esp_err_to_name(res));

        // 2 saniyede bir ölçüm almaya devam et
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(2000));
    }
}

#endif

void app_main()
{
    ESP_ERROR_CHECK(i2cdev_init()); // I2C başlatılır
    memset(&dev, 0, sizeof(sht3x_t)); // SHT3x yapısı sıfırlanır

    // SHT3x sensör yapılandırması (I2C adresi ve bağlantı pinleri)
    ESP_ERROR_CHECK(sht3x_init_desc(&dev, 0x44, 0, 4, 3));
    ESP_ERROR_CHECK(sht3x_init(&dev)); // SHT3x başlatılır

    // Görev oluşturulur ve belirli bir CPU'ya sabitlenir
    xTaskCreatePinnedToCore(task, "sht3x_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}
