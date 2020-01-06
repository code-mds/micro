Realizzare su BasysMX3 il programma "Semaforo pedonale" descritto di seguito. 

Descrizione generale:
Inizialmente il semaforo si trova nello stato "verde".
Una richiesta di attraversamento, sia lato destro sia lato sinistro, porta il semaforo ad accendere la luce gialla per TG secondi e, successivamente, avvia il lampeggio (ogni secondo) della luce rossa per TR secondi. Dopo l'attraversamento il semaforo torna nello stato "verde".
È possibile modificare la durata TR. Un pulsante ci permette di entrare nella modalità di configurazione e impostare il nuovo valore.
Attraverso un terminale, è possibile recuperare i parametri del semaforo come tempo TG, tempo TR e numero di richieste complessivo di attraversamento (non importa se da Dx o da Sx).

Mappa I/O:

RB8 (BTNR) pulsante di attraversamento lato Dx
RB0 (BTNL) pulsante di attraversamento lato Sx
RF0 (BTNC) pulsante di configurazione
RB1 (BTNU) pulsante conferma nuovo valore
RD2, RD12, RD3 (RGB led)
RF12 (UART4TX)
RF13 (UART4RX)
RB14 output compare per speaker

Configurazione: 
La pressione del pulsante BTNC ci porta nella modalità configurazione. Il led RGB presenta una luce blu. Sul LCD viene mostrato il il messaggio "config".
Ruotando il potenziometro presente sulla BasysMX3 viene modificato il valore analogico acquisito. Il numero acquisito [0-1023] viene convertito in secondi (per es. 1023 -> 10 sec, 512 -> 5 sec, ecc.).
Una volta che il tempo desiderato (TR) viene raggiunto (visualizzato su LCD) il pulsante BTNU, imposta il nuovo valore e permette di uscire dalla funzione di configurazione portando il semaforo nello stato "luce verde".
Durante la fase di configurazione, qualsiasi richiesta di attraversamento viene ignorata.

Richiesta di attraversamento da Dx:
Quando il semaforo è verde, la pressione del pulsante BTNR richiama una richiesta di attraversamento. Sul LCD viene mostrato il il messaggio "call Dx".
La luce diventa gialla per TG secondi prima di diventare rossa e lampeggiare per TR secondi.
Mentre la luce è gialla (TG), viene emesso un segnale acustico "beep".
Trascorso TR, il semaforo torna verde.
Il lampeggio della luce rossa viene gestito da un timer con interrupt che genera un toggle ogni secondo.
Nuove richieste di attraversamento  e/o configurazione vengono ignorate fino a quando TR non sia trascorso.

Richiesta di attraversamento da Sx:
Quando il semaforo è verde, la pressione del pulsante BTNL richiama una richiesta di attraversamento. Sul LCD viene mostrato il il messaggio "call Sx".
La luce diventa gialla per TG secondi prima di diventare rossa e lampeggiare per TR secondi.
Mentre la luce è gialla (TG), viene emesso un segnale acustico "beep".
Trascorso TR, il semaforo torna verde.
Il lampeggio della luce rossa viene gestito da un timer con interrupt che genera un toggle ogni secondo.
Nuove richieste di attraversamento  e/o configurazione vengono ignorate fino a quando TR non sia trascorso.

DataLog:
Il comando "datalog" ricevuto su UART4 da un terminale restituisce, sempre su terminale, i tempi TR, TG e il numero di richieste di attraversamento complessive.

Tempi:
Il valore di default dei tempi è:
TG = 3 secondi
TR = 5 secondi, modificabile nella funzione "Configurazione" da un minimo di 3 secondi a un massimo di 10 secondi.

Periferiche principali:
TIMER1 con interrupt, gestione lampeggio led rosso
TIMER2, delay per gestione LCD
TIMER3 per output compare speaker
ADC  per lettura valore analogico da convertire in secondi per TR
UART
OUTPUT COMPARE
PMP