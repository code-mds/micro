# Programmazione Microcontrollori

* IDE: MPLABX
* Micro: PIC32MX370F512L
* Board: Digilent Basys MX3
* GPIO:	General Purpose I/O
* UART:	Universal Asynchronous Receiver Trasmitter


## PIC32MX Notes

| Registry | Description |
| -------- | ----------- |
| `ANSELx` | definisco se ANALOGICO=1 o DIGITAL=0 |
| `TRISx`  | definisco se INPUT=1 o OUTPUT=0 | 
| `PORTx`  | per leggere il valore dei bit del registro |
| `LATx`   | per scrivere bit nel registro |
| `CNCON`  | interrupt (change modification) |
| `ODCx`   | open drain control (non approfondiamo) |

Where `x` is the name of a port (A, B, C, D, ...)

#### Esempio: Toggle pin 0 of Port D
```
// operatore not: ~
LATDbits.LATD0 = ~LATDbits.LATD0; 

// uso di INV port register: 0000 0000 0000 0001
LATDINV = 0x0001
```

## UART
| Registry | Description |
| -------- | ----------- |
| `UxMODE` | configurazione: ON/OFF, parity, # data bits, stop bits, flow control |
| `UxSTA`  | stato | 
| `UxTXREG` | trasmettere dati |
| `UxRXREG` | ricevere dati |
| `UxBRG`  | baud rate generator |

Where `x` is [1..6] to identify UART1 ... UART6

#### Esempio: config UART4 (that is connected to the micro_usb)
From schematic and ref. manual, UART4 is on pin RF12 (TX) and RF13 (RX)
```
TRISFbits.TRISF12 = 0; // TX digital output
RPF12R = 0b0010;       // mapping U4TX to RPF12
TRISFbits.TRISF13 = 0; // RX digital output
U4RXR = 0b1001;        // mapping RPF13 to U4RX

// setto i 2 bit (8 e 9) per TX e RX, 00=TX+RX usati
U4MODEbits.UEN = 0b00; // 0b=binario, 0x esadecimale

// setto i 2 bit (1 e 2) per parita',  00=nesszba, 01=pari, 02=dispari
U4MODEbits.PDSEL = 0b01; 

// setto il bit (0) per stop bit, 0=1bit stop   (1=2bit stop)
U4MODEbits.STSEL = 0; 

// abilita trasmissione
U4STAbits.UTXEN = 1;    

// abilita ricezione
U4STAbits.URXEN = 1;    

// setto il bit (15) per UART ON/OFF, 1=ON
U4MODEbits.ON = 1;
```

Trasmissione Dati
```
// aspetta finche' il buffer e' pieno, poi scrivi il carattere 'a'
while(!U4STAbits.UTXBF);
U4TXREG = 'a';
```

Ricezione Dati
```
// aspetta un nuovo carattare e poi leggilo
while(!U4STAbits.URXDA);
char ch = U4RXREG;

```

