// Auto-generated. Do not edit.



    //% weight=2 color=#ff2050 icon="\uf287" block="Edge Serial"
declare namespace edge_serial {

    /**
     * Read a line of text from the serial port and return the buffer when the delimiter is met.
     * @param delimiter text delimiter that separates each text chunk
     */
    //% help=serial/read-until
    //% blockId=edge_serial_read_until block="serial|read until %delimiter=edge_serial_delimiter_conv"
    //% weight=19 shim=edge_serial::readUntil
    function readUntil(delimiter: string): string;

    /**
     * Read the buffered received data as a string
     */
    //% help=serial/read-string
    //% blockId=edge_serial_read_buffer block="serial|read string"
    //% weight=18 shim=edge_serial::readString
    function readString(): string;

    /**
     * Register an event to be fired when one of the delimiter is matched.
     * @param delimiters the characters to match received characters against.
     */
    //% help=serial/on-data-received
    //% weight=18 blockId=edge_serial_on_data_received block="serial|on data received %delimiters=edge_serial_delimiter_conv" shim=edge_serial::onDataReceived
    function onDataReceived(delimiters: string, body: () => void): void;

    /**
     * Send a piece of text through the serial connection.
     */
    //% help=serial/write-string
    //% weight=87 blockGap=8
    //% blockId=edge_serial_writestring block="serial|write string %text"
    //% text.shadowOptions.toString=true shim=edge_serial::writeString
    function writeString(text: string): void;

    /**
     * Send a buffer through serial connection
     */
    //% blockId=edge_serial_writebuffer block="serial|write buffer %buffer=edge_serial_readbuffer"
    //% help=serial/write-buffer advanced=true weight=6 shim=edge_serial::writeBuffer
    function writeBuffer(buffer: Buffer): void;

    /**
     * Read multiple characters from the receive buffer. 
     * If length is positive, pauses until enough characters are present.
     * @param length default buffer length
     */
    //% blockId=edge_serial_readbuffer block="serial|read buffer %length"
    //% help=serial/read-buffer advanced=true weight=5 shim=edge_serial::readBuffer
    function readBuffer(length: int32): Buffer;

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
    //% blockGap=8 shim=edge_serial::redirect
    function redirect(tx: EdgeSerialPin, rx: EdgeSerialPin, rate: EdgeBaudRate): void;

    /**
    Set the baud rate of the serial port
     */
    //% weight=10
    //% blockId=edge_serial_setbaudrate block="serial|set baud rate %rate"
    //% blockGap=8 inlineInputMode=inline
    //% help=serial/set-baud-rate
    //% group="Configuration" advanced=true shim=edge_serial::setBaudRate
    function setBaudRate(rate: EdgeBaudRate): void;

    /**
     * Sets the size of the RX buffer in bytes
     * @param size length of the rx buffer in bytes, eg: 32
     */
    //% help=serial/set-rx-buffer-size
    //% blockId=edge_serialSetRxBufferSize block="serial set rx buffer size to $size"
    //% advanced=true shim=edge_serial::setRxBufferSize
    function setRxBufferSize(size: uint8): void;

    /**
     * Sets the size of the TX buffer in bytes
     * @param size length of the tx buffer in bytes, eg: 32
     */
    //% help=serial/set-tx-buffer-size
    //% blockId=edge_serialSetTxBufferSize block="serial set tx buffer size to $size"
    //% advanced=true shim=edge_serial::setTxBufferSize
    function setTxBufferSize(size: uint8): void;
}

// Auto-generated. Do not edit. Really.
