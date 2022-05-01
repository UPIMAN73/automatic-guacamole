/**
 * @file aesmanagement.hpp
 * @author Joshua Calzadillas
 * @brief 
 * @version 0.1
 * @date 2022-04-26
 * 
 * 
 */
#ifndef AESMANAGEMENT_HPP_
#define AESMANAGEMENT_HPP_

#include "aes.hpp"
#include "secrets.hpp"

// AES Default Values
uint8_t * KEY;
uint8_t *  IV;
struct AES_ctx ctx;

// Functions that are associated with the AES padding
uint8_t * padValues(uint8_t * address, uint32_t length);
uint8_t * padValues(char    * address, uint32_t length);
uint8_t * removePad(uint8_t * address, uint32_t length);
uint8_t * removePad(char    * address, uint32_t length);

/**
 * @brief 
 * 
 * @param address 
 * @param length 
 */
uint8_t * padValues(uint8_t * address, uint32_t length)
{
    // String length check
    if (length % 16 != 0)
    {
        // calculate padding value to append to the end of the text
        uint8_t value = 16 - (length % 16); // calculate the required padding size
        uint8_t * temp = new uint8_t[length + value];
        for (uint32_t i = 0; i < length + value; i++)
        {
            if (i < length)
            {
                temp[i] = address[i];
            }
            else
            {
                temp[i] = value;
            }
        }
        return temp;
    }

    else
    {
        return address;
    }
}

/**
 * @brief 
 * 
 * @param address 
 * @param length 
 */
uint8_t * padValues(char * address, uint32_t length)
{
    return padValues((uint8_t *) address, length);
}

/**
 * @brief 
 * 
 * @param address 
 * @param length 
 * @return uint8_t* 
 */
uint8_t * removePad(uint8_t * address, uint32_t length)
{
    // if the padding value is there
    if (address[length-1] > 0x00 && address[length-1] < 0x10)
    {
        // value of the padding
        uint8_t value = address[length-1];
        if (value == address[length - value] && value != address[length - value - 1])
        {
            // Cleaning up the address values
            for (uint32_t i = length - value; i < length; i++)
            {
                address[i] = 0x00;
            }
        }
    }
    return address;
}

/**
 * @brief 
 * 
 * @param address 
 * @param length 
 */
uint8_t * removePad(char * address, uint32_t length)
{
    // convert to the allowable function
    return removePad((uint8_t *) address, length);
}

/**
 * @brief 
 * Hexify single character values
 * 
 * @param a 
 * @return uint8_t* 
 */
uint8_t * hexify(const char * a)
{
    if (strlen(a) <= 16)
    {
        uint8_t * output = new uint8_t[16];
        uint8_t * temp = padValues((char *)a, strlen(a));
        for (int i = 0; i < 16; i++)
        {
            if (temp[i] > 0x2F && temp[i] < 0x3A)
            {
                output[i] = temp[i] - 0x30;
            }

            else if (temp[i] > 0x40 && temp[i] < 0x47)
            {
                output[i] = temp[i] - 0x37;
            }
            
            else if (temp[i] > 0x60 && temp[i] < 0x67)
            {
                output[i] = temp[i] - 0x57;
            }

            else
            {
                output[i] = 0x00;
                continue;
            }
        }
        return output;
    }

    else
    {
        return ((uint8_t *) a);
    }
}

/**
 * @brief 
 * Converts uint8_t array (pointer) into a char array (pointer)
 * 
 * @param address 
 * @param length 
 * @return char* 
 */
char * toChar(uint8_t * address, uint32_t length)
{
    char * output = new char[length];
    for (uint32_t i = 0; i < length; i++)
    {
        output[i] = (char) address[i];
    }
    return output;
}

/**
 * @brief 
 * 
 * @param text 
 * @param length 
 * @return uint8_t* 
 */
uint8_t * toUINT(const char * text, uint32_t length)
{
    uint8_t * output = new uint8_t[length];
    for (uint32_t i = 0; i < length; i++)
    {
        output[i] = (uint8_t) text[i];
    }
    return output;
}

/**
 * @brief 
 * 
 * @param length 
 * @return uint32_t 
 */
uint32_t padLength(const uint32_t length)
{
    return 16 - (length % 16);
}
#endif