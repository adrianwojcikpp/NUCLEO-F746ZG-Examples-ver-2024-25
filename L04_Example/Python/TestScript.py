import serial
import time

# Serial port configuration
port = 'COM8'
baud_rate = 115200

# Initialize the serial port
ser = serial.Serial(port, baud_rate)
time.sleep(2)  # Give some time for the serial connection to initialize

try:
    value = 0
    while True:
        # Format the message with 'R' and the three-digit number with leading zeros
        message = f"R{value:03}B{value:03}"
        ser.write(message.encode())  # Send message over serial
        print(f"Sent: {message}")
        
        # Increment value, wrap around after reaching 100
        value = (value + 10) % 110
        
        time.sleep(0.5)  # 500 ms delay

except KeyboardInterrupt:
    print("Stopping program...")

finally:
    ser.close()  # Make sure to close the serial port when done
