import serial
import struct
import numpy as np
import matplotlib.pyplot as plt

# Serial port configuration
SERIAL_PORT = 'COM8'  # Update this with your serial port name
BAUD_RATE = 115200    # Update this if needed
DATA_LENGTH = 4 * 3 * 900  # Total bytes to read in one iteration

def send_message(ser, current_mA):
    """
    Sends a formatted message to the serial device.
    """
    message = f"R{current_mA:04.1f}mA\r"  # Format the current in the required format
    ser.write(message.encode())  # Send the message
    print(f"Sent: {message}")

def read_serial_data(ser):
    """
    Reads DATA_LENGTH bytes from the serial port.
    """
    print("Waiting for data...")
    raw_data = ser.read(DATA_LENGTH)  # Read the required number of bytes
    if len(raw_data) != DATA_LENGTH:
        print(len(raw_data))
        raise ValueError("Incomplete data received.")
    print("Data received.")
    return raw_data

def interpret_data(raw_data):
    """
    Converts raw binary data into three float arrays: measurement, reference, control.
    """
    # Convert binary data to float values
    float_values = struct.unpack('<' + 'f' * (len(raw_data) // 4), raw_data)
    if len(float_values) != 3 * 900:
        raise ValueError("Unexpected data length after conversion.")

    # Split data into 'measurement', 'reference', and 'control'
    measurement = float_values[0::3]
    reference = float_values[1::3]
    control = float_values[2::3]

    return measurement, reference, control

def plot_data(all_measurements, all_references, all_controls):
    """
    Plots all accumulated data with additional lines for reference +/- 1.0.
    """
    # Create time array for the entire dataset
    total_samples = len(all_measurements)
    sample_time = 1 / 900  # seconds
    time = np.linspace(0, total_samples * sample_time, total_samples, endpoint=False)

    # Compute reference +/- 1.0
    ref_plus_1 = np.array(all_references) + 0.75
    ref_minus_1 = np.array(all_references) - 0.75

    # Plot 'measurement' and 'reference' on one plot with additional lines
    plt.figure(figsize=(12, 6))
    plt.subplot(2, 1, 1)
    plt.plot(time, all_measurements, label='Measurement', color='blue')
    plt.plot(time, all_references, label='Reference', color='green')
    plt.plot(time, ref_plus_1, label='Reference + Max Error', color='red', linestyle='--')
    plt.plot(time, ref_minus_1, label='Reference - Max Error', color='red', linestyle='--')
    plt.title('Measurement and Reference')
    plt.ylabel('LED current [mA]')
    plt.legend()
    plt.grid()
    plt.xlim(0,16)

    # Plot 'control' on another plot
    plt.subplot(2, 1, 2)
    plt.plot(time, all_controls, label='Control', color='red')
    plt.title('Control')
    plt.xlabel('Time [s]')
    plt.ylabel('PWM duty cycle [%]')
    plt.legend()
    plt.grid()
    plt.xlim(0,16)

    # Show the plots
    plt.tight_layout()
    plt.show()

def main():
    try:
        # Open the serial port
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=5) as ser:
            # Initialize lists to accumulate data
            all_measurements = []
            all_references = []
            all_controls = []

            # Loop to send commands, read, and process data 10 times
            for i, current_mA in enumerate(np.arange(1.0, 16.1, 1.0)):
                print(f"Iteration {i + 1}/10, Sending current: {current_mA:.1f} mA")
                
                # Send the command
                send_message(ser, current_mA)

                # Receive and process the data
                raw_data = read_serial_data(ser)
                measurement, reference, control = interpret_data(raw_data)

                # Accumulate the results
                all_measurements.extend(measurement)
                all_references.extend(reference)
                all_controls.extend(control)

            # After all iterations, plot the accumulated data
            plot_data(all_measurements, all_references, all_controls)

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
