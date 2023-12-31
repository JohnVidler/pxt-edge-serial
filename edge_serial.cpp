#include "pxt.h"
#if MICROBIT_CODAL
    #include "NRF52Serial.h"
#endif

#define MICROBIT_SERIAL_READ_BUFFER_LENGTH 64

enum EdgeSerialPin {
    P0 = MICROBIT_ID_IO_P0,
    P1 = MICROBIT_ID_IO_P1,
    P2 = MICROBIT_ID_IO_P2,
    P8 = MICROBIT_ID_IO_P8,
    P12 = MICROBIT_ID_IO_P12,
    P13 = MICROBIT_ID_IO_P13,
    P14 = MICROBIT_ID_IO_P14,
    P15 = MICROBIT_ID_IO_P15,
    P16 = MICROBIT_ID_IO_P16
};

enum EdgeBaudRate {
  //% block=115200
  BaudRate115200 = 115200,
  //% block=57600
  BaudRate57600 = 57600,
  //% block=38400
  BaudRate38400 = 38400,
  //% block=31250
  BaudRate31250 = 31250,
  //% block=28800
  BaudRate28800 = 28800,
  //% block=19200
  BaudRate19200 = 19200,
  //% block=14400
  BaudRate14400 = 14400,
  //% block=9600
  BaudRate9600 = 9600,
  //% block=4800
  BaudRate4800 = 4800,
  //% block=2400
  BaudRate2400 = 2400,
  //% block=1200
  BaudRate1200 = 1200
};

//% weight=2 color=#ff2050 icon="\uf287" block="Edge Serial"
namespace edgeserial {
#if MICROBIT_CODAL
    NRF52Serial * edgeSerial = NULL;
    void checkSerialPort() {
        if( edgeSerial != NULL )
            return;
        
        edgeSerial = new NRF52Serial( uBit.io.P0, uBit.io.P1, NRF_UARTE1 );
        edgeSerial->setBaud( 9600 );
    }
#endif

    // note that at least one // followed by % is needed per declaration!

    /**
     * Read a line of text from the serial port and return the buffer when the delimiter is met.
     * @param delimiter text delimiter that separates each text chunk
     */
    //% help=serial/read-until
    //% blockId=edgeserial_read_until block="serial|read until %delimiter=edgeserial_delimiter_conv"
    //% weight=19
    String readUntil(String delimiter) {
        #if MICROBIT_CODAL
            checkSerialPort();
            return PSTR(edgeSerial->readUntil(MSTR(delimiter)));
        #else
            return mkString("", 0);
        #endif
    }

    /**
    * Read the buffered received data as a string
    */
    //% help=serial/read-string
    //% blockId=edgeserial_read_buffer block="serial|read string"
    //% weight=18
    String readString() {
        #if MICROBIT_CODAL
            checkSerialPort();
            int n = edgeSerial->getRxBufferSize();
            if (n == 0)
                return mkString("", 0);
            return PSTR(edgeSerial->read(n, MicroBitSerialMode::ASYNC));
        #else
            return mkString("", 0);
        #endif
    }

    /**
    * Register an event to be fired when one of the delimiter is matched.
    * @param delimiters the characters to match received characters against.
    */
    //% help=serial/on-data-received
    //% weight=18 blockId=edgeserial_on_data_received block="serial|on data received %delimiters=edgeserial_delimiter_conv"
    void onDataReceived(String delimiters, Action body) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->eventOn(MSTR(delimiters));
            registerWithDal(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH, body);
            // lazy initialization of serial buffers
            edgeSerial->read(MicroBitSerialMode::ASYNC);
        #endif
    }

    /**
     * Send a piece of text through the serial connection.
     */
    //% help=serial/write-string
    //% weight=87 blockGap=8
    //% blockId=edgeserial_writestring block="serial|write string %text"
    //% text.shadowOptions.toString=true
    void writeString(String text) {
        #if MICROBIT_CODAL
            checkSerialPort();
            if (!text)
                return;

            edgeSerial->send(MSTR(text));
        #endif
    }

    /**
    * Send a buffer through serial connection
    */
    //% blockId=edgeserial_writebuffer block="serial|write buffer %buffer=edgeserial_readbuffer"
    //% help=serial/write-buffer advanced=true weight=6
    void writeBuffer(Buffer buffer) {
        #if MICROBIT_CODAL
            checkSerialPort();
            if (!buffer)
                return;

            edgeSerial->send(buffer->data, buffer->length);
        #endif
    }

    /**
    * Read multiple characters from the receive buffer. 
    * If length is positive, pauses until enough characters are present.
    * @param length default buffer length
    */
    //% blockId=edgeserial_readbuffer block="serial|read buffer %length"
    //% help=serial/read-buffer advanced=true weight=5
    Buffer readBuffer(int length) {
        #if MICROBIT_CODAL
            checkSerialPort();
            auto mode = SYNC_SLEEP;
            if (length <= 0) {
                length = edgeSerial->getRxBufferSize();
                mode = ASYNC;
            }

            auto buf = mkBuffer(NULL, length);
            auto res = buf;
            registerGCObj(buf); // make sure buffer is pinned, while we wait for data
            int read = edgeSerial->read(buf->data, buf->length, mode);
            if (read != length) {
                res = mkBuffer(buf->data, read);
            }
            unregisterGCObj(buf);

            return res;
        #else
            return mkBuffer(NULL, length);
        #endif
    }

    bool tryResolvePin(EdgeSerialPin p, PinName& name) {
        auto pin = getPin(p); 
        if (NULL != pin) {
            name = (PinName)pin->name;
            return true;
        }
        return false;
    }

    /**
    * Set the serial input and output to use pins instead of the USB connection.
    * @param tx the new transmission pin, eg: EdgeSerialPin.P0
    * @param rx the new reception pin, eg: EdgeSerialPin.P1
    * @param rate the new baud rate. eg: 115200
    */
    //% weight=10
    //% help=serial/connect
    //% blockId=edgeserial_redirect block="serial|connect to|TX %tx|RX %rx|at baud rate %rate"
    //% blockExternalInputs=1
    //% tx.fieldEditor="gridpicker" tx.fieldOptions.columns=3
    //% tx.fieldOptions.tooltips="false"
    //% rx.fieldEditor="gridpicker" rx.fieldOptions.columns=3
    //% rx.fieldOptions.tooltips="false"
    //% blockGap=8
    void redirect(EdgeSerialPin tx, EdgeSerialPin rx, EdgeBaudRate rate) {
        #if MICROBIT_CODAL
            checkSerialPort();
            if (getPin(tx) && getPin(rx)) {
                edgeSerial->redirect(*getPin(tx), *getPin(rx));
            }
            edgeSerial->setBaud(rate);
        #endif
    }

    /**
    Set the baud rate of the serial port
    */
    //% weight=10
    //% blockId=edgeserial_setbaudrate block="serial|set baud rate %rate"
    //% blockGap=8 inlineInputMode=inline
    //% help=serial/set-baud-rate
    //% group="Configuration" advanced=true
    void setBaudRate(EdgeBaudRate rate) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->setBaud(rate);
        #endif
    }

    /**
    * Sets the size of the RX buffer in bytes
    * @param size length of the rx buffer in bytes, eg: 32
    */
    //% help=serial/set-rx-buffer-size
    //% blockId=edgeserialSetRxBufferSize block="serial set rx buffer size to $size"
    //% advanced=true
    void setRxBufferSize(uint8_t size) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->setRxBufferSize(size);
        #endif
    }

    /**
    * Sets the size of the TX buffer in bytes
    * @param size length of the tx buffer in bytes, eg: 32
    */
    //% help=serial/set-tx-buffer-size
    //% blockId=edgeserialSetTxBufferSize block="serial set tx buffer size to $size"
    //% advanced=true
    void setTxBufferSize(uint8_t size) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->setTxBufferSize(size);
        #endif
    }
}