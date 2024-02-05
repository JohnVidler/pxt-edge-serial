// Auto-generated. Do not edit.



    //% weight=2 color=#ff2050 icon="\uf287" block="Edge Serial"
declare namespace edgeserial {

    /**
     * Read a line of text from the serial port and return the buffer when the delimiter is met.
     * @param delimiter text delimiter that separates each text chunk
     */
    //% shim=edgeserial::readUntil
    function readUntil(delimiter: string): string;

    /**
     * Read the buffered received data as a string
     */
    //% shim=edgeserial::readString
    function readString(): string;

    /**
     * Register an event to be fired when one of the delimiter is matched.
     * @param delimiters the characters to match received characters against.
     */
    //% shim=edgeserial::onDataReceived
    function onDataReceived(delimiters: string, body: () => void): void;

    /**
     * Send a piece of text through the serial connection.
     */
    //% shim=edgeserial::writeString
    function writeString(text: string): void;

    /**
     * Send a buffer through serial connection
     */
    //% shim=edgeserial::writeBuffer
    function writeBuffer(buffer: Buffer): void;

    /**
     * Read multiple characters from the receive buffer. 
     * If length is positive, pauses until enough characters are present.
     * @param length default buffer length
     */
    //% shim=edgeserial::readBuffer
    function readBuffer(length: int32): Buffer;

    /**
     * Set the serial input and output to use pins instead of the USB connection.
     * @param tx the new transmission pin, eg: EdgeSerialPin.P0
     * @param rx the new reception pin, eg: EdgeSerialPin.P1
     * @param rate the new baud rate. eg: 115200
     */
    //% shim=edgeserial::redirect
    function redirect(tx: EdgeSerialPin, rx: EdgeSerialPin, rate: EdgeBaudRate): void;

    /**
    Set the baud rate of the serial port
     */
    //% shim=edgeserial::setBaudRate
    function setBaudRate(rate: EdgeBaudRate): void;

    /**
     * Sets the size of the RX buffer in bytes
     * @param size length of the rx buffer in bytes, eg: 32
     */
    //% shim=edgeserial::setRxBufferSize
    function setRxBufferSize(size: uint8): void;

    /**
     * Sets the size of the TX buffer in bytes
     * @param size length of the tx buffer in bytes, eg: 32
     */
    //% shim=edgeserial::setTxBufferSize
    function setTxBufferSize(size: uint8): void;
}

// Auto-generated. Do not edit. Really.
