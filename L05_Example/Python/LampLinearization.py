# ******************************************************************************
# @file    lamp_linearization.py
# @author  AW             Adrian.Wojcik@put.poznan.pl
# @version 1.0
# @date    10-Nov-2024
# ******************************************************************************

import numpy as np
from scipy.optimize import fsolve
import pandas as pd

# Function: lamp_brightness2angle
def lamp_brightness2angle(brightness):
    """
    LAMP_BRIGHTNESS2ANGLE Computes firing angle for TRIAC in incandescent
    light bulb control circuit for given brightness expressed in percent.
    Solution is based on numerical solution of circuit algebraic equation.

    ANGLE = LAMP_BRIGHTNESS2ANGLE(BRIGHTNESS) returns a TRIAC firing angle
    in degrees for the given brightness expressed in percent.
    """
    # Brightness normalization
    b = np.clip(brightness / 100, 0, 1)  # <0, 100> -> <0,1> with saturation at <0,1>

    # Power-brightness relationship
    p = np.sqrt(b)  # b = p^2

    # Define equation to solve
    def equation(angle_sym):
        return 2 * np.pi * (1 - p) - (2 * angle_sym - np.sin(2 * angle_sym))

    # Solve for the angle in radians
    angle_tmp = fsolve(equation, np.pi / 2)[0]  # initial guess at pi/2
    angle = np.degrees(angle_tmp)  # Convert result to degrees

    return angle

# Main function
def main():
    N = 2
    brightness = np.linspace(0, 100, N * 100 + 1)
    angle = np.array([lamp_brightness2angle(b) for b in brightness])

    # Write the results to CSV with commas as delimiters
    angle = angle.reshape(1, -1)  # Reshape to a 1-row array
    pd.DataFrame(angle).to_csv('../Components/Inc/lamp_brightness2angle_LookUpTable.csv', index=False, header=False, sep=',')


# Run main function if script is executed directly
if __name__ == "__main__":
    main()
