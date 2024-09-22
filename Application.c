/* 
 * File:   Application.c
 * Author: Yazan Abd-Almajeed
 * Created on July 16, 2023, 12:48 PM
 */

#include "../Application/MCAL_Layer/Application.h"


#define MAX_DIGITS 3 // Maximum number of digits in a number
#define MAX_INPUT_LENGTH (MAX_DIGITS + 1) // Maximum input length including null terminator

Std_Return_Type ret = E_OK;

char input_operand1[MAX_INPUT_LENGTH]; // Buffer to store user input for operand 1
char input_operand2[MAX_INPUT_LENGTH]; // Buffer to store user input for operand 2
int operand1 = 0, operand2 = 0;
long result = 0;
char operator;
void LCD_Welcome_Message(void);

int main() {
    application_intialize();
    LCD_Welcome_Message();

    while (1) {
        // Input for operand 1
        for (int i = 0; i < MAX_DIGITS; i++) {
            while (1) {
                ret = keypad_get_value(&keypad, &input_operand1[i]);
                if (input_operand1[i] >= '0' && input_operand1[i] <= '9') {
                    ret = lcd_4bit_send_data(&lcd_4bit, input_operand1[i]); // Display on LCD
                    operand1 = operand1 * 10 + (input_operand1[i] - '0'); // Convert ASCII character to numeric value
                    break;
                }
            }
        }

        // Input for operator
        while (1) {
            ret = keypad_get_value(&keypad, &operator);
            if (operator == '+' || operator == '-' || operator == '*' || operator == '/') {
                ret = lcd_4bit_send_data(&lcd_4bit, operator); // Display on LCD
                break;
            }
        }

        // Input for operand 2
        for (int i = 0; i < MAX_DIGITS; i++) {
            while (1) {
                ret = keypad_get_value(&keypad, &input_operand2[i]);
                if (input_operand2[i] >= '0' && input_operand2[i] <= '9') {
                    ret = lcd_4bit_send_data(&lcd_4bit, input_operand2[i]); // Display on LCD
                    operand2 = operand2 * 10 + (input_operand2[i] - '0'); // Convert ASCII character to numeric value
                    break;
                }
            }
        }

        // Calculate result based on operator
        switch (operator) {
            case '+':
                result = operand1 + operand2;
                break;
            case '-':
                result = operand1 - operand2;
                break;
            case '*':
                result = operand1 * operand2;
                break;
            case '/':
                if (operand2 != 0) {
                    result = operand1 / operand2;
                } else {
                    lcd_4bit_send_str_pos(&lcd_4bit, 4, 6, "Error !!");
                    led_turn_on(&led1);
                }
                break;
            default:
                break;
        }

        // Display result on LCD
        if (operand2) {
            char resultStr[20];
            snprintf(resultStr, sizeof (resultStr), "%d", result);
            lcd_4bit_send_str(&lcd_4bit, " = ");
            ret = lcd_4bit_send_str(&lcd_4bit, resultStr);
            led_turn_on(&led2);
        }

        // Clear input buffers and reset variables
        memset(input_operand1, 0, sizeof (input_operand1));
        memset(input_operand2, 0, sizeof (input_operand2));
        operand1 = operand2 = result = 0;
        operator = '\0';

        __delay_ms(1000);
        led_turn_off(&led1);
        led_turn_off(&led2);
        ret = lcd_4bit_send_command(&lcd_4bit, _LCD_CLEAR);
    }

    return EXIT_SUCCESS;
}

void application_intialize(void) {
    Std_Return_Type ret = E_OK;

    ecu_layer_intialize();
}

void LCD_Welcome_Message(void) {
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_CLEAR);
    ret = lcd_4bit_send_str_pos(&lcd_4bit, 1, 2, "New Calculator !");
    __delay_ms(500);
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_DISPLAY_SHIFT_RIGHT);
    __delay_ms(100);
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_DISPLAY_SHIFT_RIGHT);
    __delay_ms(100);
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_DISPLAY_SHIFT_LEFT);
    __delay_ms(100);
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_DISPLAY_SHIFT_LEFT);
    __delay_ms(100);
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_CLEAR);
    ret = lcd_4bit_send_str_pos(&lcd_4bit, 1, 5, "2 Numbers");
    ret = lcd_4bit_send_str_pos(&lcd_4bit, 2, 2, "3 Digits for Each");
    __delay_ms(1000);
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_CLEAR);
    ret = lcd_4bit_send_str_pos(&lcd_4bit, 1, 1, "Calculator is Ready");
    __delay_ms(1000);
    ret = lcd_4bit_send_command(&lcd_4bit, _LCD_CLEAR);
}
