/**
 * Reading and writing data over a serial connection.
 */
//% weight=2 color=#ff2050 icon="\uf287" block="Edge Serial"
namespace edgeserial {
    /**
     * The string used to mark a new line, default is \r\n
     */
    export let NEW_LINE = "\r\n";
    export let NEW_LINE_DELIMITER: Delimiters = Delimiters.NewLine;
    let writeLinePadding = 32;

    /**
     * Print a line of text to the serial port
     * @param value to send over serial
     */
    //% weight=90
    //% help=serial/write-line blockGap=8
    //% blockId=edgeserial_writeline block="serial|write line %text"
    //% text.shadowOptions.toString=true
    export function writeLine(text: string): void {
        if (!text) text = "";
        edgeserial.writeString(text);
        // pad data to the 32 byte boundary
        // to ensure apps receive the packet
        if (writeLinePadding > 0) {
            let r = (writeLinePadding - (text.length + NEW_LINE.length) % writeLinePadding) % writeLinePadding;
            for (let i = 0; i < r; ++i)
                edgeserial.writeString(" ");
        }
        edgeserial.writeString(NEW_LINE);
    }

    //% help=serial/write-string
    //% weight=87 blockGap=8
    //% blockId=edgeserial_writestring block="serial|write string %text"
    //% text.shadowOptions.toString=true
    //% shim=edgeserial.writeString
    export function writeString(data: string): void {
        return; // Simulator dummy shim
    }

    /**
     * Sets the padding length for lines sent with "write line".
     * @param length the number of bytes alignment, eg: 0
     *
     */
    //% weight=1
    //% help=serial/set-write-line-padding
    //% blockId=edgeserialWriteNewLinePadding block="serial set write line padding to $length"
    //% advanced=true
    //% length.min=0 length.max=128
    export function setWriteLinePadding(length: number) {
        writeLinePadding = length | 0;
        return;
    }

    /**
     * Print a numeric value to the serial port
     */
    //% help=serial/write-number
    //% weight=89 blockGap=8
    //% blockId=edgeserial_writenumber block="serial|write number %value"
    export function writeNumber(value: number): void {
        writeString(value.toString());
    }

    /**
     * Print an array of numeric values as CSV to the serial port
     */
    //% help=serial/write-numbers
    //% weight=86
    //% blockId=edgeserial_writenumbers block="serial|write numbers %values"
    export function writeNumbers(values: number[]): void {
        if (!values) return;
        for (let i = 0; i < values.length; ++i) {
            if (i > 0) writeString(",");
            writeNumber(values[i]);
        }
        writeLine("")
    }

    /**
     * Write a name:value pair as a line to the serial port.
     * @param name name of the value stream, eg: x
     * @param value to write
     */
    //% weight=88 blockGap=8
    //% help=serial/write-value
    //% blockId=edgeserial_writevalue block="serial|write value %name|= %value"
    export function writeValue(name: string, value: number): void {
        writeLine((name ? name + ":" : "") + value);
    }

    /**
     * Read a line of text from the serial port.
     */
    //% help=serial/read-line
    //% blockId=edgeserial_read_line block="serial|read line"
    //% weight=20 blockGap=8
    export function readLine(): string {
        return edgeserial.readUntil(delimiters(NEW_LINE_DELIMITER));
    }

    /**
     * Return the corresponding delimiter string
     */
    //% blockId="edgeserial_delimiter_conv" block="%del"
    //% weight=1 blockHidden=true
    export function delimiters(del: Delimiters): string {
        return String.fromCharCode(del as number);
    }

    /**
    * Register an event to be fired when one of the delimiter is matched.
    * @param delimiters the characters to match received characters against.
    */
    //% weight=18 blockId=edgeserial_on_data_received block="serial|on data received %delimiters=edgeserial_delimiter_conv"
    //% shim=edgeserial::onDataReceived
    export function onDataReceived(delimiters: string, body: () => void): void { return; }

    /**
     * Read a line of text from the serial port and return the buffer when the delimiter is met.
     * @param delimiter text delimiter that separates each text chunk
     */
    //% help=serial/read-until
    //% blockId=edgeserial_read_until block="serial|read until %delimiter=edgeserial_delimiter_conv"
    //% weight=19 shim=edgeserial::readUntil
    //% shim=edgeserial::readUntil
    export function readUntil(delimiter: string): string { return ""; };

    /**
     * Read the buffered received data as a string
     */
    //% help=serial/read-string
    //% blockId=edgeserial_read_buffer block="serial|read string"
    //% weight=18 shim=edgeserial::readString
    //% shim=edgeserial::readString
    export function readString(): string { return ""; }

    /**
     * Read multiple characters from the receive buffer. 
     * If length is positive, pauses until enough characters are present.
     * @param length default buffer length
     */
    //% blockId=edgeserial_readbuffer block="serial|read buffer %length"
    //% help=serial/read-buffer advanced=true weight=5 shim=edgeserial::readBuffer
    //% shim=edgeserial::readBuffer
    export function readBuffer(length: number): Buffer { return Buffer.create(0); }

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
    //% blockGap=8 shim=edgeserial::redirect
    //% shim=edgeserial::redirect
    export function redirect(tx: EdgeSerialPin, rx: EdgeSerialPin, rate: EdgeBaudRate): void { return; }

    /**
    * Set the baud rate of the serial port
     */
    //% weight=10
    //% blockId=edgeserial_setbaudrate block="serial|set baud rate %rate"
    //% blockGap=8 inlineInputMode=inline
    //% help=serial/set-baud-rate
    //% group="Configuration" advanced=true shim=edgeserial::setBaudRate
    //% shim=edgeserial::setBaudRate
    export function setBaudRate(rate: EdgeBaudRate): void { return; };

    /**
     * Sets the size of the RX buffer in bytes
     * @param size length of the rx buffer in bytes, eg: 32
     */
    //% help=serial/set-rx-buffer-size
    //% blockId=edgeserialSetRxBufferSize block="serial set rx buffer size to $size"
    //% advanced=true shim=edgeserial::setRxBufferSize
    //% shim=edgeserial::setRxBufferSize
    export function setRxBufferSize(size: number): void { return; };

    /**
     * Sets the size of the TX buffer in bytes
     * @param size length of the tx buffer in bytes, eg: 32
     */
    //% help=serial/set-tx-buffer-size
    //% blockId=edgeserialSetTxBufferSize block="serial set tx buffer size to $size"
    //% advanced=true shim=edgeserial::setTxBufferSize
    //% shim=edgeserial::setTxBufferSize
    export function setTxBufferSize(size: number): void { return; };
}