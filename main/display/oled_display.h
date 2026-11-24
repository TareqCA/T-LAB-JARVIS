#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H
#include "lvgl_display.h"
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
class OledDisplay : public LvglDisplay {
private:
    esp_lcd_panel_io_handle_t panel_io_ = nullptr;
    esp_lcd_panel_handle_t panel_ = nullptr;
    lv_obj_t *top_bar_ = nullptr, *status_bar_ = nullptr, *content_ = nullptr;
    lv_obj_t *content_left_ = nullptr, *content_right_ = nullptr, *container_ = nullptr;
    lv_obj_t *side_bar_ = nullptr, *emotion_label_ = nullptr, *chat_message_label_ = nullptr;
    lv_obj_t *left_eye_ = nullptr, *right_eye_ = nullptr, *mouth_ = nullptr;
    lv_obj_t *left_glint_ = nullptr, *right_glint_ = nullptr, *brand_label_ = nullptr;
    lv_obj_t* wave_[5] = {};
    lv_timer_t* face_timer_ = nullptr;
    enum class FaceMode { Idle, Listening, Thinking, Speaking, Happy, Sleep };
    FaceMode face_mode_ = FaceMode::Idle;
    uint32_t face_tick_ = 0;
    bool happy_ = false, clock_enabled_ = false;
    void SetupUI_128x128();
    void SetupUI_128x64();
    void SetupUI_128x32();
    void UpdateFace();
    static void FaceTimerCallback(lv_timer_t* timer);
    bool Lock(int timeout_ms = 0) override;
    void Unlock() override;
public:
    OledDisplay(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel, int width, int height, bool mirror_x, bool mirror_y);
    ~OledDisplay();
    void SetStatus(const char* status) override;
    void SetPowerSaveMode(bool on) override;
    void SetChatMessage(const char* role, const char* content) override;
    void SetEmotion(const char* emotion) override;
    void SetTheme(Theme* theme) override;
};
#endif
