#include "pxt.h"
#if MICROBIT_CODAL
    #ifndef NRFX_UARTE_ENABLED
        #define NRFX_UARTE_ENABLED 1
    #endif
    #ifndef NRFX_UARTE1_ENABLED
        #define NRFX_UARTE1_ENABLED 1
    #endif

    #include "NRF52Serial.h"
    #include "MicroBitPowerManager.h"
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

    //%
    String readUntil(String delimiter) {
        #if MICROBIT_CODAL
            checkSerialPort();
            return PSTR(edgeSerial->readUntil(MSTR(delimiter)));
        #else
            return mkString("", 0);
        #endif
    }

    //%
    void onDataReceived(String delimiters, Action body) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->eventOn(MSTR(delimiters));
            //registerWithDal(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH, body);
            // lazy initialization of serial buffers
            edgeSerial->read(MicroBitSerialMode::ASYNC);
        #endif
    }

    //%
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
     * Send a piece of text through the serial connection.
     */
    void writeString(String text) {
        #if MICROBIT_CODAL
            checkSerialPort();
            if (!text)
                return;

            edgeSerial->send(MSTR(text));
        #endif
    }

    //%
    void writeBuffer(Buffer buffer) {
        #if MICROBIT_CODAL
            checkSerialPort();
            if (!buffer)
                return;

            edgeSerial->send(buffer->data, buffer->length);
        #endif
    }

    //%
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

    //% 
    bool tryResolvePin(EdgeSerialPin p, PinName& name) {
        auto pin = getPin(p); 
        if (NULL != pin) {
            name = (PinName)pin->name;
            return true;
        }
        return false;
    }

    //%
    void redirect(EdgeSerialPin tx, EdgeSerialPin rx, EdgeBaudRate rate) {
        #if MICROBIT_CODAL
            checkSerialPort();
            if (getPin(tx) && getPin(rx))
                edgeSerial->redirect( *getPin(tx), *getPin(rx) );
            edgeSerial->setBaud(rate);
        #endif
    }

    //%
    void setBaudRate(EdgeBaudRate rate) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->setBaud(rate);
        #endif
    }

    //%
    void setRxBufferSize(uint8_t size) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->setRxBufferSize(size);
        #endif
    }

    //%
    void setTxBufferSize(uint8_t size) {
        #if MICROBIT_CODAL
            checkSerialPort();
            edgeSerial->setTxBufferSize(size);
        #endif
    }
}