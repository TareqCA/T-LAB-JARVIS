#include "wifi_board.h"
#include "codecs/no_audio_codec.h"
#include "display/oled_display.h"
#include "application.h"
#include "button.h"
#include "config.h"

#include <esp_log.h>
#include <driver/i2c_master.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_sh1107.h>

#define TAG "TLabAI"

class TLabAIBoard : public WifiBoard {
private:
    i2c_master_bus_handle_t display_i2c_bus_ = nullptr;
    esp_lcd_panel_io_handle_t panel_io_ = nullptr;
    esp_lcd_panel_handle_t panel_ = nullptr;

    Display* display_ = nullptr;
    Button boot_button_;

    void InitializeDisplayI2c() {
        i2c_master_bus_config_t bus_config = {};

        bus_config.i2c_port = I2C_NUM_0;
        bus_config.sda_io_num = DISPLAY_SDA_PIN;
        bus_config.scl_io_num = DISPLAY_SCL_PIN;
        bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
        bus_config.glitch_ignore_cnt = 7;
        bus_config.flags.enable_internal_pullup = true;

        ESP_ERROR_CHECK(
            i2c_new_master_bus(
                &bus_config, &display_i2c_bus_
            )
        );

        ESP_LOGI(TAG, "I2C initialized");
    }

    void InitializeDisplay() {
          esp_lcd_panel_io_i2c_config_t io_config = {};

io_config.dev_addr = 0x3C;
io_config.scl_speed_hz = 400000;
io_config.control_phase_bytes = 1;
io_config.dc_bit_offset = 6;
io_config.lcd_cmd_bits = 8;
io_config.lcd_param_bits = 8;
io_config.flags.disable_control_phase = 0;

        ESP_ERROR_CHECK(
            esp_lcd_new_panel_io_i2c_v2(
                display_i2c_bus_,
                &io_config,
                &panel_io_
            )
        );

        esp_lcd_panel_sh1107_config_t sh1107_config = {};
        sh1107_config.contrast = 128;
        sh1107_config.offset = 0x00;

        esp_lcd_panel_dev_config_t panel_config = {};
        panel_config.reset_gpio_num = -1;
        panel_config.bits_per_pixel = 1;
        panel_config.vendor_config = &sh1107_config;

        ESP_ERROR_CHECK(
            esp_lcd_new_panel_sh1107(
                panel_io_,
                &panel_config,
                &panel_
            )
        );

        ESP_ERROR_CHECK(
            esp_lcd_panel_reset(panel_)
        );

        ESP_ERROR_CHECK(
            esp_lcd_panel_init(panel_)
        );

        ESP_ERROR_CHECK(
            esp_lcd_panel_disp_on_off(panel_, true)
        );

        ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_, false));

        display_ = new OledDisplay(
            panel_io_,
            panel_,
            DISPLAY_WIDTH,
            DISPLAY_HEIGHT,
            DISPLAY_MIRROR_X,
            DISPLAY_MIRROR_Y
        );

        ESP_LOGI(TAG, "SH1107 OLED ready");
    }

    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();

            if (app.GetDeviceState() ==
                kDeviceStateStarting) {
                EnterWifiConfigMode();
                return;
            }

            app.ToggleChatState();
        });
    }

public:
    TLabAIBoard() :
        boot_button_(BOOT_BUTTON_GPIO) {

        ESP_LOGI(TAG, "Starting T-Lab AI");

        InitializeDisplayI2c();
        InitializeDisplay();
        InitializeButtons();
    }

    virtual AudioCodec* GetAudioCodec() override {
        static NoAudioCodecSimplexPdm audio_codec(
            AUDIO_INPUT_SAMPLE_RATE,
            AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_SPK_GPIO_BCLK,
            AUDIO_I2S_SPK_GPIO_LRCK,
            AUDIO_I2S_SPK_GPIO_DOUT,
            AUDIO_I2S_MIC_GPIO_SCK,
            AUDIO_I2S_MIC_GPIO_DIN
        );

        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }
};

DECLARE_BOARD(TLabAIBoard);
