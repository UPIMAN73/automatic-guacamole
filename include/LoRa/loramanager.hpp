#ifndef LORAMANAGER_HPP_
#define LORAMANAGER_HPP_

#include <LoRa.h>

/**
 * @brief 
 * List of available LoRa Operating Frequencies
 * 
 */
#define USA_915 915E6
#define EUA_866 866E6

/**
 * @brief 
 * 
 * @param frequency 
 */
void beginLoRa(const uint32_t frequency)
{
    // Serial Start
    if (Serial)
    {
        Serial.println("Starting LoRa Module...");
        Serial.print("Operating on Frequency (Hz): ");
        Serial.println(frequency);
        delay(100);
    }

    // LoRa Begin Module Library
    while (!LoRa.begin(frequency))
    {
        delay(10);
    }

    if (Serial)
    {
        delay(100);
        Serial.println("LoRa Module Initilized.");
    }
}

/**
 * @brief 
 * 
 */
void endLoRa()
{
    if (Serial)
    {
        Serial.println("Clossing LoRa Connection");
    }
}

/**
 * @brief 
 * Dump all of the information in the registers
 * 
 */
void dumpLoRaRegisters()
{
    if (Serial)
    {
        LoRa.dumpRegisters(Serial);
    }
}

/**
 * @brief Set the Frequency object
 * 
 * @param frequency 
 */
void setLoRaFrequency(const uint32_t frequency)
{
    LoRa.setFrequency(frequency);
}

/**
 * @brief Set the LoRa Bandwidth object
 * 
 * @param bw 
 */
void setLoRaBandwidth(const uint32_t bw)
{
    LoRa.setSignalBandwidth(bw);
}

/**
 * @brief Set the LoRa TX Power object
 * 
 * @param level 
 * @param outputPin 
 */
void setLoRaTXPower(const int level, const int outputPin = PA_OUTPUT_PA_BOOST_PIN)
{
    // No Secure checks Yet
    if (outputPin != PA_OUTPUT_PA_BOOST_PIN)
    {
        LoRa.setTxPower(level, outputPin);
    }

    else
    {
        LoRa.setTxPower(level);
    }
}

/**
 * @brief Set the LoRa SF object
 * 
 * @param sf 
 */
void setLoRaSF(const uint32_t sf)
{
    LoRa.setSpreadingFactor(sf);
}

/**
 * @brief Set the LoRa CRC object
 * 
 * @param enable 
 */
void setLoRaCRC(const bool enable)
{
    if (enable)
    {
        LoRa.enableCrc();
    }

    else
    {
        LoRa.disableCrc();
    }
}

/**
 * @brief 
 * 
 * @param message 
 */
void sendLoRaMessage(String message)
{
    LoRa.beginPacket();
    LoRa.println(message);
    LoRa.endPacket();
}

/**
 * @brief 
 * 
 * @param message 
 */
void sendLoRaMessage(const char * message)
{
    LoRa.beginPacket();
    LoRa.println(message);
    LoRa.endPacket();
}


/**
 * @brief 
 * 
 * @return int 
 */
int parseLoRaPacket()
{
    return LoRa.parsePacket();
}

/**
 * @brief 
 * 
 * @return int 
 */
int LoRaPacketAvailable()
{
    return LoRa.available();
}

/**
 * @brief 
 * 
 * @return int 
 */
int readLoRaValue()
{
    return LoRa.read();
}

/**
 * @brief 
 * 
 * @return String 
 */
String readLoRaPacket()
{
    String message = "";
    while (LoRa.available())
    {
        message += (char) LoRa.read();
    }
    return message;
}
#endif