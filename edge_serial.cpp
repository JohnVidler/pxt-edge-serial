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
namespace edge_serial {
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

#if MICROBIT_CODAL
    /**
     * Read a line of text from the serial port and return the buffer when the delimiter is met.
     * @param delimiter text delimiter that separates each text chunk
     */
    //% help=serial/read-until
    //% blockId=edge_serial_read_until block="serial|read until %delimiter=edge_serial_delimiter_conv"
    //% weight=19
    String readUntil(String delimiter) {
        checkSerialPort();
        return PSTR(edgeSerial->readUntil(MSTR(delimiter)));
    }
#else
    String readUntil(String delimiter) {
        return mkString("", 0);
    }
#endif

#if MICROBIT_CODAL
    /**
    * Read the buffered received data as a string
    */
    //% help=serial/read-string
    //% blockId=edge_serial_read_buffer block="serial|read string"
    //% weight=18
    String readString() {
        checkSerialPort();
        int n = edgeSerial->getRxBufferSize();
        if (n == 0)
            return mkString("", 0);
        return PSTR(edgeSerial->read(n, MicroBitSerialMode::ASYNC));
    }
#else
    String readString() {
        return mkString("", 0);;
    }
#endif

#if MICROBIT_CODAL
    /**
    * Register an event to be fired when one of the delimiter is matched.
    * @param delimiters the characters to match received characters against.
    */
    //% help=serial/on-data-received
    //% weight=18 blockId=edge_serial_on_data_received block="serial|on data received %delimiters=edge_serial_delimiter_conv"
    void onDataReceived(String delimiters, Action body) {
        checkSerialPort();
        edgeSerial->eventOn(MSTR(delimiters));
        registerWithDal(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH, body);
        // lazy initialization of serial buffers
        edgeSerial->read(MicroBitSerialMode::ASYNC);
    }
#else
    void onDataReceived(String delimiters, Action body) {
        // Stub
    }
#endif

#if MICROBIT_CODAL
    /**
     * Send a piece of text through the serial connection.
     */
    //% help=serial/write-string
    //% weight=87 blockGap=8
    //% blockId=edge_serial_writestring block="serial|write string %text"
    //% text.shadowOptions.toString=true
    void writeString(String text) {
        checkSerialPort();
        if (!text)
            return;

        edgeSerial->send(MSTR(text));
    }
#else
    void writeString(String text) {
        // Stub
    }
#endif

#if MICROBIT_CODAL
    /**
    * Send a buffer through serial connection
    */
    //% blockId=edge_serial_writebuffer block="serial|write buffer %buffer=edge_serial_readbuffer"
    //% help=serial/write-buffer advanced=true weight=6
    void writeBuffer(Buffer buffer) {
        checkSerialPort();
        if (!buffer)
            return;

        edgeSerial->send(buffer->data, buffer->length);
    }
#else
    void writeBuffer(Buffer buffer) {
        // Stub
    }
#endif

#if MICROBIT_CODAL
    /**
    * Read multiple characters from the receive buffer. 
    * If length is positive, pauses until enough characters are present.
    * @param length default buffer length
    */
    //% blockId=edge_serial_readbuffer block="serial|read buffer %length"
    //% help=serial/read-buffer advanced=true weight=5
    Buffer readBuffer(int length) {
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
    }
#else
    Buffer readBuffer(int length) {
        return mkBuffer(NULL, length);
    }
#endif

    bool tryResolvePin(EdgeSerialPin p, PinName& name) {
        auto pin = getPin(p); 
        if (NULL != pin) {
            name = (PinName)pin->name;
            return true;
        }
        return false;
    }

#if MICROBIT_CODAL
    /**
    * Set the serial input and output to use pins instead of the USB connection.
    * @param tx the new transmission pin, eg: EdgeSerialPin.P0
    * @param rx the new reception pin, eg: EdgeSerialPin.P1
    * @param rate the new baud rate. eg: 115200
    */
    //% weight=10
    //% help=serial/connect
    //% blockId=edge_serial_redirect block="serial|connect to|TX %tx|RX %rx|at baud rate %rate"
    //% blockExternalInputs=1
    //% tx.fieldEditor="gridpicker" tx.fieldOptions.columns=3
    //% tx.fieldOptions.tooltips="false"
    //% rx.fieldEditor="gridpicker" rx.fieldOptions.columns=3
    //% rx.fieldOptions.tooltips="false"
    //% blockGap=8
    void redirect(EdgeSerialPin tx, EdgeSerialPin rx, EdgeBaudRate rate) {
        checkSerialPort();
        if (getPin(tx) && getPin(rx)) {
            edgeSerial->redirect(*getPin(tx), *getPin(rx));
        }
        edgeSerial->setBaud(rate);
    }
#else
    void redirect(EdgeSerialPin tx, EdgeSerialPin rx, EdgeBaudRate rate) {
        // Stub
    }
#endif

#if MICROBIT_CODAL
    /**
    Set the baud rate of the serial port
    */
    //% weight=10
    //% blockId=edge_serial_setbaudrate block="serial|set baud rate %rate"
    //% blockGap=8 inlineInputMode=inline
    //% help=serial/set-baud-rate
    //% group="Configuration" advanced=true
    void setBaudRate(EdgeBaudRate rate) {
        checkSerialPort();
        edgeSerial->setBaud(rate);
    }
#else
    void setBaudRate(EdgeBaudRate rate) {
        // Stub
    }
#endif

#if MICROBIT_CODAL
    /**
    * Sets the size of the RX buffer in bytes
    * @param size length of the rx buffer in bytes, eg: 32
    */
    //% help=serial/set-rx-buffer-size
    //% blockId=edge_serialSetRxBufferSize block="serial set rx buffer size to $size"
    //% advanced=true
    void setRxBufferSize(uint8_t size) {
        checkSerialPort();
        edgeSerial->setRxBufferSize(size);
    }
#else
    void setRxBufferSize(uint8_t size) {
        // Stub
    }
#endif

#if MICROBIT_CODAL
    /**
    * Sets the size of the TX buffer in bytes
    * @param size length of the tx buffer in bytes, eg: 32
    */
    //% help=serial/set-tx-buffer-size
    //% blockId=edge_serialSetTxBufferSize block="serial set tx buffer size to $size"
    //% advanced=true
    void setTxBufferSize(uint8_t size) {
        checkSerialPort();
        edgeSerial->setTxBufferSize(size);
    }
#else
    void setTxBufferSize(uint8_t size) {
        // Stub
    }
#endif
}