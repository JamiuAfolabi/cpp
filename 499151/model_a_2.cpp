#include <iostream>
#include <cmath>

using namespace std;

// Define global constants using #define
#define R 0.08206       // Ideal gas constant (L*atm / mol*K)
#define A_CO2 3.592    // Van der Waals constant 'a' for CO2 (L^2*atm / mol^2)
#define B_CO2 0.04267   // Van der Waals constant 'b' for CO2 (L / mol)

double vanderWaalsPressure(double moles, double temperature, double volume) {
    double P = (R * temperature * moles) / volume;
    double correction = A_CO2 * moles * moles / (volume * volume);
    return P - correction / (volume - B_CO2 * moles);
}

int main() {
    double moles, temperature, volume;

    cout << "Enter the number of moles of CO2: ";
    cin >> moles;

    cout << "Enter the temperature in Kelvin: ";
    cin >> temperature;

    cout << "Enter the volume in Liters: ";
    cin >> volume;

    if (volume <= 0) {
        cout << "Volume must be a positive value." << endl;
    } else {
        double pressure = vanderWaalsPressure(moles, temperature, volume);
        cout << "Pressure of CO2 using Van der Waals law: " << pressure << " atm" << endl;
    }

    return 0;
}