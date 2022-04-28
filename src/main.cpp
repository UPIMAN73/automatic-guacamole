/**
 * @file main.cpp
 * @author Joshua Calzadillas (upiman73@github.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-26
 * 
 * 
 */
#include <Arduino.h>
#include "AES/aesmanagement.hpp"
#include "LoRa/loramanager.hpp"
// #include <MKRWAN_v2.h>

#define BUAD_RATE       9600

#define LORA_SENDER     1 // Defines a particular mode to allow the device to choose its particular role

/**
 * @brief 
 * Setup function runs the code once
 * 
 */
void setup() 
{
  // put your setup code here, to run once:
  while (!Serial)
  {
    Serial.begin(BUAD_RATE);
    delay(10);
  }
  Serial.println("STARTING AES ENCRYPTION");

  // AES Settings
  KEY = toUINT(AES_ROOT_KEY, 16);
  IV  =   toUINT(AES_IV_KEY, 16);
  AES_init_ctx_iv(&ctx, KEY, IV); // initilizing the CTX environment

  // Print Key
  Serial.println("\n\nKey: ");
  for (int i = 0; i < 16; i++)
  {
    Serial.print((char) KEY[i], HEX);
  }

  // Print IV
  Serial.println("\n\nIV: ");
  for (int i = 0; i < 16; i++)
  {
    Serial.print((char) IV[i], HEX);
  }
  Serial.println("\n\n");

  // Lora Send Message
  beginLoRa(USA_915);     // Begin LoRa Module
  // dumpLoRaRegisters();

  // LoRa Senderr
  #ifdef LORA_SENDER
  String SMSG = SECRET_MESSAGE;
  delay(100);
  sendLoRaMessage("Sending Encrypted Text:");
  uint8_t * secretMessage = toUINT(SECRET_MESSAGE, strlen(SECRET_MESSAGE));
  secretMessage = padValues(secretMessage, strlen(SECRET_MESSAGE));
  AES_CBC_encrypt_buffer(&ctx, secretMessage, strlen(SECRET_MESSAGE) + padLength(strlen(SECRET_MESSAGE)));

  // Encrypted String
  Serial.print("Message Length: ");
  Serial.println(strlen(SECRET_MESSAGE) + padLength(strlen(SECRET_MESSAGE)));
  Serial.println("Sending This Message: ");
  for (uint32_t i = 0; i < strlen(SECRET_MESSAGE) + padLength(strlen(SECRET_MESSAGE)); i++)
  {
    Serial.print((char) secretMessage[i], HEX);
    if (i < SMSG.length())
    {
      SMSG[i] = (char) secretMessage[i];
    }
    else
    {
      SMSG = SMSG + (char) secretMessage[i];
    }
  }
  Serial.println("\n\n\n");


  Serial.print("Encrypted Text Length: ");
  Serial.print(SMSG.length());
  Serial.println("\n\n\n");
  Serial.println("Regular Output:");
  Serial.println(SMSG);

  // Send the Encrypted Text
  sendLoRaMessage(SMSG);

  #endif
}

/**
 * @brief 
 * Loops function loops the code
 * 
 */
void loop() {
  // Loop 

  #ifndef LORA_SENDER
  if (parseLoRaPacket())
  {
    String SMSG = readLoRaPacket();
    delay(100);
    uint8_t * secretMessage = toUINT(SMSG.c_str() , SMSG.length());
    AES_CBC_decrypt_buffer(&ctx, secretMessage, SMSG.length());

    // Encrypted String
    secretMessage = removePad(secretMessage, SMSG.length());
    const char * msg = toChar(secretMessage, strlen((char *) secretMessage));
    String outputText = String(msg);
    Serial.print("Message Length: ");
    Serial.println(outputText.length());
    Serial.println("Recieved This Message: ");
    Serial.println(outputText);
    Serial.println("\n\n\n");
  }
  #endif
}