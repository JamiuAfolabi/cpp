#include <iostream>

using namespace std;

// Constexpr variables for ideal gas constant
constexpr double R = 0.08206;  // Ideal gas constant (L*atm / mol*K)

// template class for Vander Waals gas 
template <typename Gas>
class VanderWaalsGas {
public:
    // Constexpr variables specific to each gas
    constexpr static double a = Gas::a;  // Van der Waals constant 'a'
    constexpr static double b = Gas::b;  // Van der Waals constant 'b'

    constexpr static auto getName() {
        return Gas::name;
    }

    constexpr static double vanderWaalsPressure(double moles, double temperature, double volume) {
        double P = (R * temperature * moles) / volume;
        double correction = a * moles * moles / (volume * volume);
        return P - correction / (volume - b * moles);
    }
};

// Specialization for CO2 gas
struct CO2 {
    static constexpr double a = 3.592;  // Van der Waals constant 'a' for CO2 (L^2*atm / mol^2)
    static constexpr double b = 0.04267; // Van der Waals constant 'b' for CO2 (L / mol)
    static constexpr const char* name = "CO2";
};

// Specialization for Oxygen gas (Example for flexibility)
struct O2 {
    static constexpr double a = 1.360;
    static constexpr double b = 0.03183;
    static constexpr const char* name = "O2";
};
 
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

 	// Using template function for CO2
        constexpr auto pressureCO2 = VanderWaalsGas<CO2>::vanderWaalsPressure(moles, temperature, volume);
        cout << "Pressure of " << VanderWaalsGas<CO2>::getName() << " using Van der Waals law: " << pressureCO2 << " atm" << endl;

	// Using template function for O2 (Example) 
        constexpr auto pressureO2 = VanderWaalsGas<O2>::vanderWaalsPressure(moles, temperature, volume);
        cout << "Pressure of " << VanderWaalsGas<O2>::getName() << " using Van der Waals law: " << pressureO2 << " atm" << endl;
    }

    return 0;
} 
