#include "soundgen.h"
#include <driver/gpio.h>
#include <driver/dac.h>
//writen with help from pratchaya
int startSound(int freq) {
    if(freq < 136) {
        freq = 136;
    } else if(freq > 5000) {
        freq = 5000;
    }
    freq = freq - (freq%136);

    dac_cw_config_t config = {
        .en_ch  = DAC_CHANNEL_1,
        .scale  = DAC_CW_SCALE_1,
        .phase  = DAC_CW_PHASE_0,
        .freq   = freq,
        .offset = 1
    };
    esp_err_t err = dac_cw_generator_config(&config);
    if (err) return freq;

    dac_cw_generator_enable();
    dac_output_enable(DAC_CHANNEL_1);
    return freq;
}
void stopSound() {
    dac_cw_generator_disable();
}

