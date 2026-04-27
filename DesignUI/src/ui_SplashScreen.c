// Splash Screen - INDELC
// LVGL version: 8.3.3
// Muestra la imagen ui_img_logo centrada durante 3 segundos,
// luego hace fade-out y carga ui_Screen1.

#include "ui.h"

// ─── Objeto global de la splash screen ───────────────────────────────────────
lv_obj_t *ui_SplashScreen;

// ─── Prototipos internos
// ──────────────────────────────────────────────────────
static void splash_timer_cb(lv_timer_t *timer) {
  lv_timer_del(timer);

  // Inicializar la pantalla principal
  ui_Screen1_screen_init();

  // Transición suave: la nueva pantalla aparece (Fade ON) sobre la splash
  // El último parámetro 'true' indica que la splash se borrará automáticamente al terminar
  lv_scr_load_anim(ui_Screen1, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
}

// ─── Inicialización de la splash screen ──────────────────────────────────────
void ui_SplashScreen_init(void) {
  // Crear la pantalla
  ui_SplashScreen = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_SplashScreen, LV_OBJ_FLAG_SCROLLABLE);

  // Fondo negro total
  lv_obj_set_style_bg_color(ui_SplashScreen, lv_color_hex(0x000000),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_SplashScreen, 255,
                          LV_PART_MAIN | LV_STATE_DEFAULT);

  // ── Imagen splash a pantalla completa ──────────────────────────────────
  // ui_img_splash: 400x250 px → zoom 512 (2x) → 800x500 px
  // Pantalla: 800x480 → se recortan 10px arriba y abajo (centrado)
  lv_obj_t *img = lv_img_create(ui_SplashScreen);
  lv_img_set_src(img, &ui_img_splash);
  lv_img_set_pivot(img, 0, 0); 
  // Centrado manual para imagen 400x250 en pantalla 800x480:
  // X = (800-400)/2 = 200 | Y = (480-250)/2 = 115
  lv_obj_set_pos(img, 200, 115); 

  // ── Barra de progreso animada en la parte inferior ───────────────────────
  lv_obj_t *bar = lv_bar_create(ui_SplashScreen);
  lv_obj_set_size(bar, 300, 6);
  lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -40);
  lv_bar_set_range(bar, 0, 100);
  lv_bar_set_value(bar, 0, LV_ANIM_OFF);

  // Estilos de la barra
  lv_obj_set_style_bg_color(bar, lv_color_hex(0x222222),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(bar, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(bar, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(bar, lv_color_hex(0xFFFFFF),
                            LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(bar, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(bar, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // Animación de la barra: 0 → 100 % en 2800 ms (deja 200 ms de margen)
  lv_anim_t ba;
  lv_anim_init(&ba);
  lv_anim_set_exec_cb(&ba, (lv_anim_exec_xcb_t)lv_bar_set_value);
  lv_anim_set_var(&ba, bar);
  lv_anim_set_time(&ba, 2800);
  lv_anim_set_values(&ba, 0, 100);
  lv_anim_set_path_cb(&ba, lv_anim_path_ease_in_out);
  lv_anim_start(&ba);

  // ── Texto "INDELC" centrado debajo del logo ───────────────────────────────
  lv_obj_t *label = lv_label_create(ui_SplashScreen);
  lv_label_set_text(label, "INDELC");
  // Color negro (0x000000) para que se vea sobre el fondo blanco de la imagen
  lv_obj_set_style_text_color(label, lv_color_hex(0x000000),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_42,
                             LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -52);

  // ── Timer: después de 3000 ms inicia el fade-out ─────────────────────────
  lv_timer_create(splash_timer_cb, 3000, NULL);
}
