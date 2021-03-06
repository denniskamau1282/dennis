#include <AES.h> //calling AES.h library file, which is in AES_libfiles folder

AES aes ; //module name declaration, AES algorithm is also called Rinjadael by the name of its makers

             //In order for AES to perform its functions, a Fixed key is required to encrypt data,
             //And same key is used for decryption of data. The key is more like A password used in encrypting and decrypting data using AES ALgorithm
byte key[] = //declaration of variable byte Key[]. This is an array variable, which will be used for key generation and storing key.
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

byte plain[] = //This plain here is plain data which need to be encrypted with AES
{
  // 0xf3, 0x44, 0x81, 0xec, 0x3c, 0xc6, 0x27, 0xba, 0xcd, 0x5d, 0xc3, 0xfb, 0x08, 0xf2, 0x73, 0xe6
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

byte my_iv[] = //this variable provides necessary space for storing data during AES process
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
} ;

byte cipher [4*N_BLOCK] ; // Encrypted data is usually called cipher. 
byte check [4*N_BLOCK] ; // This mean cipher block will hold encrypted data coming byte by byte from AES encryption engine.

void loop ()  //Avoid this loop
{}


void setup () 
{
  Serial.begin (57600) ; //Serial communication declaration stages, this is declaration Baude rate of UART for communication with Arduino Board.
  Serial.print ("testng mode") ; //when arduino connected this message will be printed on command line, showing the successful working of Algorithm

  prekey_test () ; //calling function prekey_test().
  
//  otfly_test () ;
//  otfly_test256 () ;
}

void prekey (int bits, int blocks) //declaration of function
{
  byte iv [N_BLOCK] ; //variable declaration
  
  long t0 = micros () ; // t0 data declaration and value of micros() function is assigned to it. These function can be found in library files.
  byte succ = aes.set_key (key, bits) ; //succ datatype declaration and aes.set_key() function value is being assigned to it.These function can be found in library files.
  long t1 = micros()-t0 ;// value of micro-t0 is being assigned to t1, a new long variable
  Serial.print ("set_key ") ; Serial.print (bits) ; Serial.print (" ->") ; Serial.print ((int) succ) ;// data is being sent to command line for printing
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ; //data is being printed.
  t0 = micros () ; //new value of micros is being assigned to t0
  if (blocks == 1) //if condition
    succ = aes.encrypt (plain, cipher) ;// if bloco==1, then assign value of aes.encrypt(plain, cipher) to succ. this function will give us encrypted data
  else
  {
    for (byte i = 0 ; i < 16 ; i++)
      iv[i] = my_iv[i] ;
    succ = aes.cbc_encrypt (plain, cipher, blocks, iv) ; //if block is not equal to 1, the assign plain text to encryption engine, to perform encryption.
  }
  t1 = micros () - t0 ; //micros()-t0 value is being assigned to t1
  Serial.print ("encrypt ") ; Serial.print ((int) succ) ; //the encrypted data will be printed
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ; //on the command lines
  
  t0 = micros () ; //micros() is being assigned to t0 again.
  if (blocks == 1) //if block is one the assign decrypt function value to succ
    succ = aes.decrypt (cipher, plain) ;
  else   //otherwise
  {
    for (byte i = 0 ; i < 16 ; i++) //play a loop
      iv[i] = my_iv[i] ; 
    succ = aes.cbc_decrypt (cipher, check, blocks, iv) ; //and assigned encypted text to decrypt function
  }
  t1 = micros () - t0 ; //then, assign value of micros()-t0 to t1
  Serial.print ("decrypt ") ; Serial.print ((int) succ) ; //Now, print the value
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ;//of decrypted data on command lines

  for (byte ph = 0 ; ph < (blocks == 1 ? 3 : 4) ; ph++) //play a loop for 3 times if block==1 else 4 times.
  {
    for (byte i = 0 ; i < (ph < 3 ? blocks*N_BLOCK : N_BLOCK) ; i++) //Loop for N_Block
    {
      byte val = ph == 0 ? plain[i] : ph == 1 ? cipher[i] : ph == 2 ? check[i] : iv[i] ; //Assign the value of Plain and
      Serial.print (val>>4, HEX) ; Serial.print (val&15, HEX) ; Serial.print (" ") ;    // cipher text to variables to be printed finally of command lines.
    }
    Serial.println () ; // Value print function
  }

}

void prekey_test () //voind prekey_test is function which is passing value given below, whenever called upon. It has been called in above lones of code.
{
  prekey (128, 4) ;
  prekey (192, 3) ;
  prekey (256, 2) ;
  prekey (128, 1) ;
  prekey (192, 1) ;
  prekey (256, 1) ;
}
