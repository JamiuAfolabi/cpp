#include <iostream>

using namespace std;

// Gas constant in atm·L/mol·K
constexpr double R = 0.08206;

// Template class for Van der Waals gas model
template <typename Gas>
class VanderWaalsGas {
public:
    constexpr static double a = Gas::a;  // 'a' constant for the gas
    constexpr static double b = Gas::b;  // 'b' constant for the gas

    constexpr static auto getName() {
        return Gas::name;  // Return gas name
    }

    // Calculate pressure using Van der Waals equation
    static double vanderWaalsPressure(double moles, double temperature, double volume) {
        double P = (R * temperature * moles) / volume;  // Ideal gas pressure
        double correction = a * moles * moles / (volume * volume);  // Correction term
        return P - correction / (volume - b * moles);  // Final pressure
    }
};

// CO2 gas parameters
struct CO2 {
    static constexpr double a = 3.592;
    static constexpr double b = 0.04267;
    static constexpr const char* name = "CO2";
};

// O2 gas parameters
struct O2 {
    static constexpr double a = 1.360;
    static constexpr double b = 0.03183;
    static constexpr const char* name = "O2";
};

int main() {
    double moles, temperature, volume;

    // Input data
    cout << "Enter the number of moles of Gas: ";
    cin >> moles;

    cout << "Enter the temperature in Kelvin: ";
    cin >> temperature;

    cout << "Enter the volume in Liters: ";
    cin >> volume;

    // Check volume
    if (volume <= 0) {
        cout << "Volume must be a positive value." << endl;
    } else {
        // Calculate and output pressure for CO2 and O2
        double pressureCO2 = VanderWaalsGas<CO2>::vanderWaalsPressure(moles, temperature, volume);
        cout << "Pressure of " << VanderWaalsGas<CO2>::getName() << " using Van der Waals law: " << pressureCO2 << " atm" << endl;

        double pressureO2 = VanderWaalsGas<O2>::vanderWaalsPressure(moles, temperature, volume);
        cout << "Pressure of " << VanderWaalsGas<O2>::getName() << " using Van der Waals law: " << pressureO2 << " atm" << endl;
    }

    return 0;
}