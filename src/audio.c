#include "../include/audio.h"
#include "../include/alarm.h"
#include "../include/cancel.h"
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
static const char *TAG = "audio";

typedef struct {
    const int16_t *data;
    size_t samples;
    uint32_t rate;
    int gain_pct;
} sound_t;

static const sound_t SND_ALARM  = { alarm_pcm,  ALARM_SAMPLES,  ALARM_SAMPLE_RATE,  200 };
static const sound_t SND_CANCEL = { cancel_pcm, CANCEL_SAMPLES, CANCEL_SAMPLE_RATE, 100 };

static i2s_chan_handle_t tx_chan;
static volatile bool playing = false;

void audio_init(void)
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_chan, NULL));

    i2s_std_config_t std_cfg = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(ALARM_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = GPIO_NUM_26,
            .ws   = GPIO_NUM_25,
            .dout = GPIO_NUM_22,
            .din  = I2S_GPIO_UNUSED,
            .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },
        },
    };
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));
}

static void audio_task(void *arg)
{
    const sound_t *s = (const sound_t *)arg;

    
    i2s_std_clk_config_t clk = I2S_STD_CLK_DEFAULT_CONFIG(s->rate);
    i2s_channel_disable(tx_chan);
    i2s_channel_reconfig_std_clock(tx_chan, &clk);
    i2s_channel_enable(tx_chan);

    int16_t buf[512 * 2];
    size_t i = 0;
    while (i < s->samples) {
        size_t n = s->samples - i;
        if (n > 512) n = 512;
        for (size_t k = 0; k < n; k++) {
            int32_t v = (int32_t)s->data[i + k] * s->gain_pct / 100;
            if (v > 32767) v = 32767;
            if (v < -32768) v = -32768;
            buf[2 * k] = buf[2 * k + 1] = (int16_t)v;
        }
        size_t written;
        i2s_channel_write(tx_chan, buf, n * 4, &written, portMAX_DELAY);
        i += n;
    }
    playing = false;
    vTaskDelete(NULL);
}

static void play(const sound_t *s)
{
    if (playing) return;
    playing = true;
    xTaskCreate(audio_task, "audio", 6144, (void *)s, 5, NULL);
}

void play_alarm(void)  { ESP_LOGI(TAG, "ALARM SOUND");  play(&SND_ALARM); }
void play_cancel(void) { ESP_LOGI(TAG, "CANCEL SOUND"); play(&SND_CANCEL); }
