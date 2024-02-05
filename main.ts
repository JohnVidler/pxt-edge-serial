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

    /**
     * Sets the padding length for lines sent with "write line".
     * @param length the number of bytes alignment, eg: 0
     *
     */
    export function setWriteLinePadding(length: number) {
        writeLinePadding = length | 0;
    }

    /**
     * Print a numeric value to the serial port
     */
    export function writeNumber(value: number): void {
        writeString(value.toString());
    }

    /**
     * Print an array of numeric values as CSV to the serial port
     */
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
    export function writeValue(name: string, value: number): void {
        writeLine((name ? name + ":" : "") + value);
    }

    /**
     * Read a line of text from the serial port.
     */
    export function readLine(): string {
        return edgeserial.readUntil(delimiters(NEW_LINE_DELIMITER));
    }

    /**
     * Return the corresponding delimiter string
     */
    export function delimiters(del: Delimiters): string {
        return String.fromCharCode(del as number);
    }
}