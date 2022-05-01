/**
 * @file main.cpp
 * @author Joshua Calzadillas
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

#define BUAD_RATE       115200

// This defines the mode of communication
// #define LORA_SENDER     1 // Defines a particular mode to allow the device to choose its particular role

String SMSG;
uint8_t * secretMessage;
uint8_t * padding;
char * msg;
String outputText;
int packetSize = 0;

/**
 * @brief 
 * Setup function runs the code once
 * 
 */
void setup() 
{
  // put your setup code here, to run once:
  delay(100);
  Serial.begin(BUAD_RATE);
  delay(50);
  if (!Serial)
  {
    Serial.begin(BUAD_RATE);
    delay(100);
    while(!Serial);
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
  if (!LoRa.begin(USA_915))
  {
    Serial.println("LoRa Module Not working...");
    delay(100);
    while(1);
  }
  Serial.println("LoRa Module Success");
  // dumpLoRaRegisters();

  // LoRa Senderr
  #ifdef LORA_SENDER
  SMSG = SECRET_MESSAGE;
  delay(100);
  Serial.println("\n\n");
  // sendLoRaMessage("Sending Encrypted Text:");
  for (int i = 0; i < strlen(SECRET_MESSAGE); i++)
  {
    Serial.print(SECRET_MESSAGE[i], HEX);
  }
  Serial.println("\n");
  secretMessage = toUINT(SECRET_MESSAGE, strlen(SECRET_MESSAGE));
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
  Serial.println("LoRa Communication Mode Sender");
  #else
  Serial.println("LoRa Communication Mode Reciever");
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
  packetSize = parseLoRaPacket();
  if (packetSize > 0)
  {
    Serial.println("GOT A PACKET!");
    uint32_t i = 0;
    while (LoRa.available())
    {
        SMSG += (char) LoRa.read();
        Serial.print(SMSG[i], HEX);
        i++;
    }
    Serial.println("\n\n\n\n");
    msg = (char *) SMSG.c_str();
    for (i = 0; i < SMSG.length(); i++)
    {
      Serial.print(msg[i], HEX);
    }
    delay(100);
    Serial.println("\n\nDECRYPTING MESSAGE...");
    secretMessage = (uint8_t *) msg;
    for (i = 0; i < SMSG.length(); i++)
    {
      Serial.print(secretMessage[i], HEX);
    }
    Serial.println("\n\n");
    Serial.println("CONVERTING MESSAGE");
    AES_CBC_decrypt_buffer(&ctx, secretMessage, SMSG.length());
    for (i = 0; i < SMSG.length(); i++)
    {
      Serial.print(secretMessage[i], HEX);
    }
    Serial.println("\n\n");

    // Encrypted String
    Serial.println("REMOVING PADDING");
    removePad(secretMessage, SMSG.length());
    for (i = 0; i < SMSG.length(); i++)
    {
      Serial.print(secretMessage[i], HEX);
    }
    Serial.println("\n\n");

    //
    Serial.println("CONVERTING TO STRING");
    outputText = String((const char *) secretMessage);
    Serial.print("Message Length: ");
    Serial.println(outputText.length());
    Serial.println("Recieved This Message: \n\n");
    Serial.println(outputText);
    Serial.println("\n\n\n");

    // Clean buffers
  }

  #else
  // Serial.print("Encrypted Text Length: ");
  // Serial.print(SMSG.length());
  // Serial.println("\n\n\n");
  // Serial.println("Sending Message:");
  // Serial.println(SMSG);
  // sendLoRaMessage(SMSG);
  // delay(2000);
  #endif
}