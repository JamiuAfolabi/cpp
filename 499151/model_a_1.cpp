#include <iostream>
#include <iomanip>

using namespace std;

// Constants for CO2
const double R = 0.0821; // L·atm/(K·mol)
const double a = 3.59;   // L²·atm/(mol²)
const double b = 0.0427; // L/mol

double calculatePressure(double n, double V, double T) {
    // Applying the Van der Waals equation
    double pressure = (n * R * T) / (V - n * b) - (a * n * n) / (V * V);
    return pressure;
}

int main() {
    double n;  // number of moles
    double V;  // volume in liters
    double T;  // temperature in Kelvin

    // User input
    cout << "Enter the number of moles of CO2: ";
    cin >> n;
    cout << "Enter the volume in liters: ";
    cin >> V;
    cout << "Enter the temperature in Kelvin: ";
    cin >> T;

    // Calculate pressure
    double pressure = calculatePressure(n, V, T);

    // Display result
    cout << fixed << setprecision(2);
    cout << "The pressure of CO2 is: " << pressure << " atm" << endl;

    return 0;
}