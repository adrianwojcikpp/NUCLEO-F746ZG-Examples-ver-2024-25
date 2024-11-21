import serial
import time
import json
import matplotlib.pyplot as plt
from datetime import datetime

# Serial port configuration
port = 'COM8'
baud_rate = 115200

# Initialize the serial port
ser = serial.Serial(port, baud_rate, timeout=1)
time.sleep(2)  # Give some time for the serial connection to initialize

# Data lists to store timestamps and received values
time_data = []
value_data = []

def init_timeline_plot():
    """Initialize the timeline plot."""
    plt.ion()  # Turn on interactive mode
    plt.figure(figsize=(12, 6))
    plt.xlabel("Timestamp [ms]")
    plt.ylabel("Temperature [degC]")
    plt.title("Electric heater step response")
    plt.grid()
    plt.show()

def update_timeline_plot(timestamp, value):
    """Update the timeline plot with new data."""
    time_data.append(timestamp)
    value_data.append(value)
    plt.plot(time_data, value_data, 'r-', marker='o', markersize=4)  # Red line with circle markers
    plt.gcf().autofmt_xdate()  # Auto-format the timestamps for better readability
    plt.draw()
    plt.pause(0.01)  # Small pause to allow the plot to update

# Initialize the plot
init_timeline_plot()

# Send the initial message
initial_message = "W100\r"
ser.write(initial_message.encode())
print(f"Sent: {initial_message}")

# Record the start time
start_time = time.time()

try:
    while True:
        # Receive and parse JSON response
        response = ser.readline().decode().strip()  # Read a line from the serial port
        if response:
            try:
                # Attempt to parse JSON
                data = json.loads(response)
                
                # Check if 'id' and 'value' fields exist
                if 'id' in data and 'temp' in data:
                    received_value = data['temp']
                    print(f"Received - ID: {data['id']}, Value: {received_value}")
                    
                    # Compute elapsed time in milliseconds
                    elapsed_time = int((time.time() - start_time) * 1000)  # Convert to ms
                    
                    # Update the plot with elapsed time and received value
                    update_timeline_plot(elapsed_time, received_value)
                else:
                    print("Invalid JSON format: Missing 'id' or 'value'")
            except json.JSONDecodeError:
                print("Failed to decode JSON:", response)

except KeyboardInterrupt:
    print("Stopping program...")

finally:
    ser.close()  # Close the serial port when done
    plt.ioff()   # Turn off interactive mode
    plt.show()   # Display the final plot
