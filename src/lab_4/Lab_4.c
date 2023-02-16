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
uint8_t read_gyro_low(uint8_t eje);
uint16_t read_gyro_high(uint8_t eje);

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

	char int_to_str[10];
	char lcd_out[10];

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
		gyr_x = read_gyro_low(GYR_OUT_X_L);
		gyr_x |= read_gyro_high(GYR_OUT_X_H);

		// Obtencion del eje Y
		gyr_y = read_gyro_low(GYR_OUT_Y_L);
		gyr_y |= read_gyro_high(GYR_OUT_Y_H);

		// Obtencion del eje Z
		gyr_z = read_gyro_low(GYR_OUT_Z_L);
		gyr_z |= read_gyro_high(GYR_OUT_Z_H);

		gyr_x = gyr_x*L3GD20_SENSITIVITY_500DPS;
        gyr_y = gyr_y*L3GD20_SENSITIVITY_500DPS;
        gyr_z = gyr_z*L3GD20_SENSITIVITY_500DPS;

		sprintf(lcd_out, "%s", "X:");
		sprintf(int_to_str, "%d", gyr_x);
		strcat(lcd_out, int_to_str);

		gfx_fillScreen(LCD_BLACK);
		gfx_setCursor(10, 25);
		gfx_puts(lcd_out);
	
		sprintf(lcd_out, "%s", "Y:");
		sprintf(int_to_str, "%d", gyr_y);
		strcat(lcd_out, int_to_str);

		gfx_setCursor(10, 75);
		gfx_puts(lcd_out);

		sprintf(lcd_out, "%s", "Z:");
		sprintf(int_to_str, "%d", gyr_z);
		strcat(lcd_out, int_to_str);

		gfx_setCursor(10, 125);
		gfx_puts(lcd_out);
	
		lcd_show_frame();

		console_puts("TEST\n");

		int i;
		for (i = 0; i < 80000; i++)    /* Wait a bit. */
			__asm__("nop");
	}

	return 0;
}

uint8_t read_gyro_low(uint8_t eje) {

	uint8_t gyr;

	gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, eje | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr = spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);
	
	return gyr;
}

uint16_t read_gyro_high(uint8_t eje) {

	uint16_t gyr;

	gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, eje | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr = spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);
	
	return gyr;
}
