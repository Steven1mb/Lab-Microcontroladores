/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014 Chuck McManis <cmcmanis@mcmanis.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h> 

void spi_setup(void);
int main(void);
uint16_t read_gyro(uint8_t eje, int high);
char* show_info(char label[10], int16_t value, int y_axis);

char int_to_str[10], lcd_out[10], gyr_x_c[10], gyr_y_c[10], gyr_z_c[10];

// Variables para controlar los rebotes
char BTN_press = 0;
char BTN_release = 0;
char BTN_released = 1;
char Bouncevalue = 1; // Tan solo 1 bounce porque las iteraciones del programa son muy lentas

// Inicialización del SPI5 para leer giroscopio
void spi_setup(void)
{
    rcc_periph_clock_enable(RCC_SPI5);
    rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOF);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
    gpio_set(GPIOC, GPIO1);
    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE,
		GPIO7 | GPIO8 | GPIO9);   
	gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);
    spi_set_master_mode(SPI5);
	spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);
	spi_set_clock_polarity_0(SPI5);
	spi_set_clock_phase_0(SPI5);
	spi_set_full_duplex_mode(SPI5);
	spi_set_unidirectional_mode(SPI5);
	spi_enable_software_slave_management(SPI5);
	spi_send_msb_first(SPI5);
	spi_set_nss_high(SPI5);
    SPI_I2SCFGR(SPI5) &= ~SPI_I2SCFGR_I2SMOD;
	spi_enable(SPI5);
	/* Enable GPIOG clock. */
	rcc_periph_clock_enable(RCC_GPIOG);
	/* Set GPIO13 (in GPIO port G) to 'output push-pull'. */
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13 | GPIO14);
}

#define d2r(d) ((d) * 6.2831853 / 360.0) /* Convert degrees to radians */
#define GYR_RNW			(1 << 7) /* Write when zero */
#define GYR_MNS			(1 << 6) /* Multiple reads when 1 */
#define GYR_WHO_AM_I		0x0F
#define GYR_OUT_TEMP		0x26
#define GYR_STATUS_REG		0x27
#define GYR_CTRL_REG1		0x20
#define GYR_CTRL_REG1_PD	(1 << 3)
#define GYR_CTRL_REG1_XEN	(1 << 1)
#define GYR_CTRL_REG1_YEN	(1 << 0)
#define GYR_CTRL_REG1_ZEN	(1 << 2)
#define GYR_CTRL_REG1_BW_SHIFT	4
#define GYR_CTRL_REG4		0x23
#define GYR_CTRL_REG4_FS_SHIFT	4
#define GYR_OUT_X_L		0x28
#define GYR_OUT_X_H		0x29
#define GYR_OUT_Y_L		0x2A
#define GYR_OUT_Y_H		0x2B
#define GYR_OUT_Z_L		0x2C
#define GYR_OUT_Z_H		0x2D
#define L3GD20_SENSITIVITY_500DPS  (0.0175F) // Roughly 45/256

int main(void)
{
	clock_setup();
	spi_setup();

	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);

	int trans = 1;

	console_setup(115200);
	sdram_init();
	lcd_spi_init();
	console_puts("LCD Initialized\n");
	console_puts("Should have a checker pattern, press any key to proceed\n");
	msleep(2000);
	gfx_init(lcd_draw_pixel, 240, 320);
	gfx_fillScreen(LCD_GREY);
	gfx_fillRoundRect(10, 10, 220, 220, 5, LCD_WHITE);
	gfx_drawRoundRect(10, 10, 220, 220, 5, LCD_RED);
	gfx_fillCircle(20, 250, 10, LCD_RED);
	gfx_fillCircle(120, 250, 10, LCD_GREEN);
	gfx_fillCircle(220, 250, 10, LCD_BLUE);
	gfx_setTextSize(1);
	gfx_setCursor(15, 25);
	gfx_puts("STM32F429i-Disco");
	gfx_setTextSize(1);
	gfx_setCursor(15, 49);
	gfx_puts("Laboratorio 4");
	gfx_setCursor(15, 60);
	gfx_puts("Estudiantes:");
	gfx_setCursor(15, 72);
	gfx_puts("Adrian");
	gfx_setCursor(15, 84);
	gfx_puts("Steven");
	lcd_show_frame();
	msleep(2000);
	gfx_setTextColor(LCD_YELLOW, LCD_BLACK);
	gfx_setTextSize(3);
	
	int16_t gyr_x;
	int16_t gyr_y;
	int16_t gyr_z;

	// Configuracion del giroscopio mediante SPI5
	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG1); 
	spi_read(SPI5);
	spi_send(SPI5, GYR_CTRL_REG1_PD | GYR_CTRL_REG1_XEN |
			GYR_CTRL_REG1_YEN | GYR_CTRL_REG1_ZEN |
			(3 << GYR_CTRL_REG1_BW_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);

	// Configuracion del giroscopio mediante SPI5
	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG4);
	spi_read(SPI5);
	spi_send(SPI5, (1 << GYR_CTRL_REG4_FS_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);

	while (1) {
		// Obtencion del eje X
		gyr_x = read_gyro(GYR_OUT_X_L, 0);
		gyr_x |= read_gyro(GYR_OUT_X_H, 1);

		// Obtencion del eje Y
		gyr_y = read_gyro(GYR_OUT_Y_L, 0);
		gyr_y |= read_gyro(GYR_OUT_Y_H, 1);

		// Obtencion del eje Z
		gyr_z = read_gyro(GYR_OUT_Z_L, 0);
		gyr_z |= read_gyro(GYR_OUT_Z_H, 1);

		gyr_x = gyr_x*L3GD20_SENSITIVITY_500DPS;
        gyr_y = gyr_y*L3GD20_SENSITIVITY_500DPS;
        gyr_z = gyr_z*L3GD20_SENSITIVITY_500DPS;

		gfx_fillScreen(LCD_BLACK);
		sprintf(gyr_x_c, "%s", show_info("X:", gyr_x, 25));
		sprintf(gyr_y_c, "%s", show_info("Y:", gyr_y, 75));
		sprintf(gyr_z_c, "%s", show_info("Z:", gyr_z, 125));
		lcd_show_frame();

		// Checkear si GP3 es presionado
        if (gpio_get(GPIOA, GPIO0))
        {
            // Maneja el numero de veces que el boton ha sido constante
            BTN_press++;
            BTN_release = 0;

            // El boton es constante en ser presionado
            if (BTN_press > Bouncevalue)
            {
                // Habilitar el dejar de presionar el boton
                BTN_press = 0;
				BTN_released = 0;
            }
        } else {
			// Maneja el numero de veces que el boton ha sido constante
            BTN_release++;
            BTN_press = 0;

            // El boton es constante en ser soltado
            if (BTN_release > Bouncevalue)
            {
				if (BTN_released == 0)
				{
                    // Alterna la transferencia de datos
					trans = ~trans;
					BTN_released = 1;
				}
                BTN_release = 0;
            }
        }

		// Evalua si la transferencia de datos está activada
		if (trans == 1) {
			console_puts(gyr_x_c); console_puts("\t");
			console_puts(gyr_y_c); console_puts("\t");
			console_puts(gyr_z_c); console_puts("\n");
			gpio_toggle(GPIOG, GPIO14); // Parpadea el LED4
		} else gpio_clear(GPIOG, GPIO14); // Apaga LED4
	}

	return 0;
}

uint16_t read_gyro(uint8_t eje, int high) {

	uint16_t gyr;

	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, eje | GYR_RNW);
	spi_read(SPI5);
	spi_send(SPI5, 0);
	gyr = spi_read(SPI5);
	if (high) gyr = gyr << 8;
	gpio_set(GPIOC, GPIO1);
	
	return gyr;
}

// Esta funcion muestra informacion en la LCD
char* show_info(char label[10], int16_t value, int y_axis) {

	sprintf(lcd_out, "%s", label);
	sprintf(int_to_str, "%d", value);
	strcat(lcd_out, int_to_str);
	gfx_setCursor(10, y_axis);
	gfx_puts(lcd_out);

	char *val = int_to_str;
	return val;
}
