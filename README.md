# micro
Programmazione Microcontrollori

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

#### Example: Toggle pin 0 of Port D
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
| `UxTXREG`  | trasmettere dati |
| `UxRXREG`  | ricevere dati |
| `UxBRG`  | baud rate generator |

Where `x` is [1..6] to identify UART1 ... UART6

#### Example: config
```
//setto il bit (15) per UART ON/OFF, 1=ON
U1MODEbits.ON = 1;

// setto i 2 bit (8 e 9) per TX e RX, 00=TX+RX usati
U1MODEbits.UEN = 0b00; // 0b=binario, 0x esadecimale

// setto i 2 bit (1 e 2) per parita',  01=parita' pari
U1MODEbits.PDSEL = 0b01; 

// setto il bit (0) per stop bit, 0=1 bit stop   (1=2 bit stops)
U1MODEbits.STSEL = 0; 
```

