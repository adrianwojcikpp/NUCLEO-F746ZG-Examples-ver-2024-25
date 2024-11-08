import serial
import time
import json
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Serial port configuration
port = 'COM8'
baud_rate = 115200

# Initialize the serial port
ser = serial.Serial(port, baud_rate, timeout=1)
time.sleep(2)  # Give some time for the serial connection to initialize

# Data lists to store points for plotting
x_data = []
y_data = []

def init_plot():
    """Initialize the plot."""
    plt.ion()  # Turn on interactive mode
    plt.figure(figsize=(10, 5))
    plt.xlabel("PWM Duty Cycle [%]")
    plt.ylabel("Illuminance [lx]")
    plt.title("LED-BH1750 sensor characteristic")
    plt.grid()
    plt.xlim(0, 100)  # Adjust this as needed
    plt.ylim(0, 7000)  # Adjust this based on expected range of received values
    plt.show()

def update_plot(x, y):
    """Update the plot with new data."""
    x_data.append(x)
    y_data.append(y)
    plt.plot(x_data, y_data, 'bo-', markersize=4)  # 'bo-' means blue color, circle markers, solid line
    plt.draw()
    plt.pause(0.01)  # Small pause to allow the plot to update

# Initialize plot
init_plot()

try:
    value = 0
    for value in range(0,101):
        # Send "W" message
        message = f"W{value:03}\r"
        ser.write(message.encode())
        print(f"Sent: {message}")
        
        time.sleep(0.5)  # 500 ms delay
        
        # Send "R" message
        message = f"R000\r"
        ser.write(message.encode())
        print(f"Sent: {message}")

        # Receive and parse JSON response
        response = ser.readline().decode().strip()  # Read a line from the serial port
        if response:
            try:
                # Attempt to parse JSON
                data = json.loads(response)
                
                # Check if 'id' and 'value' fields exist
                if 'id' in data and 'value' in data:
                    received_value = data['value']
                    print(f"Received - ID: {data['id']}, Value: {received_value}")
                    
                    # Update the plot with sent and received values
                    update_plot(value, received_value)
                else:
                    print("Invalid JSON format: Missing 'id' or 'value'")
            except json.JSONDecodeError:
                print("Failed to decode JSON:", response)

except KeyboardInterrupt:
    print("Stopping program...")

finally:
    ser.close()  # Make sure to close the serial port when done
    plt.ioff()   # Turn off interactive mode
    plt.show()   # Display the final plot
