#include <iostream>
#include <cmath>

using namespace std;

// Define a template function for the Van der Waals pressure calculation
template <typename T, T a, T b>
constexpr T vanderWaalsPressure(T moles, T temperature, T volume) {
    // Check if volume is greater than b * moles to avoid division by zero
    if (volume <= b * moles) {
        throw invalid_argument("Volume must be greater than b * moles to avoid division by zero.");
    }
    
    T P = (R * temperature * moles) / (volume - b * moles);
    T correction = a * moles * moles / (volume * volume);
    return P - correction;
}

// Define constexpr for the ideal gas constant
constexpr double R = 0.08206;  // Ideal gas constant (L*atm / mol*K)

// Define Van der Waals constants for CO2 using constexpr
constexpr double a_CO2 = 3.592;  // Van der Waals constant 'a' for CO2 (L^2*atm / mol^2)
constexpr double b_CO2 = 0.04267; // Van der Waals constant 'b' for CO2 (L / mol)

int main() {
    double moles, temperature, volume;

    cout << "Enter the number of moles of CO2: ";
    cin >> moles;

    cout << "Enter the temperature in Kelvin: ";
    cin >> temperature;

    cout << "Enter the volume in Liters: ";
    cin >> volume;

    try {
        double pressure = vanderWaalsPressure<double, a_CO2, b_CO2>(moles, temperature, volume);
        cout << "Pressure of CO2 using Van der Waals law: " << pressure << " atm" << endl;
    } catch (const std::invalid_argument& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}