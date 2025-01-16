#include <iostream>
#include <vector>
#include <string>

struct LoanApplication {
    std::string applicantName;
    int creditScore;
    bool employed;
    double annualIncome;
    double loanAmount;

    // Constructor for easier initialization
    LoanApplication(std::string name, int score, bool emp, double income, double amount)
        : applicantName(name), creditScore(score), employed(emp), annualIncome(income), loanAmount(amount) {}
};

class LoanProcessingSystem {
private:
    std::vector<LoanApplication> applications;

public:
    void addApplication(const LoanApplication& application) {
        applications.push_back(application);
    }

    void processApplications() {
        std::cout << "Processing Loan Applications...\n";

        for (const auto& app : applications) {
            if (isEligible(app)) {
                std::cout << "Applicant: " << app.applicantName << " - Approved\n";
            } else {
                std::cout << "Applicant: " << app.applicantName << " - Denied\n";
            }
        }
    }

private:
    bool isEligible(const LoanApplication& app) {
        // Check credit score
        if (app.creditScore < 700) {
            return false;
        }

        // Check employment status
        if (!app.employed) {
            return false;
        }

        // Check loan amount limits
        if (app.loanAmount > 0.5 * app.annualIncome) {
            return false;
        }

        return true; // All criteria met
    }
};

int main() {
    LoanProcessingSystem loanSystem;

    // Sample loan applications
    loanSystem.addApplication(LoanApplication("Alice Smith", 720, true, 60000, 25000));
    loanSystem.addApplication(LoanApplication("Bob Johnson", 680, true, 50000, 30000));
    loanSystem.addApplication(LoanApplication("Charlie Brown", 750, false, 70000, 20000));
    loanSystem.addApplication(LoanApplication("Diana Prince", 710, true, 40000, 18000));
    loanSystem.addApplication(LoanApplication("Ethan Hunt", 800, true, 90000, 50000));
    
    // Process applications
    loanSystem.processApplications();

    return 0;
}