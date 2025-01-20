#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

struct LoanApplication {
    std::string applicantName;
    int creditScore;
    bool employed;
    double annualIncome;
    double loanAmount;

    LoanApplication(std::string name, int score, bool emp, double income, double amount)
        : applicantName(name), creditScore(score), employed(emp), annualIncome(income), loanAmount(amount) {}
};


class LoanProcessingSystem {
private:
    priority_queue<LoanApplication, vector<LoanApplication>,
                   greater<LoanApplication>> applications;

public:
    void addApplication(const LoanApplication& application) {
        applications.push(application);
    }

    void processApplications() {
        std::cout << "Processing Loan Applications...\n";

        while (!applications.empty()) {
            LoanApplication app = applications.top();
            applications.pop();
            
            if (isEligible(app)) {
                std::cout << "Applicant: " << app.applicantName << " - Approved, Loan Amount: $" << app.loanAmount << ", Ratio: " << loanAmountToIncomeRatio(app) << "\n";
            } else {
                std::cout << "Applicant: " << app.applicantName << " - Denied\n";
            }
        }
    }

private:
    double loanAmountToIncomeRatio(const LoanApplication& app) {
        if (app.annualIncome == 0) {
            return 1.0; // Handle edge case: To avoid division by 0, assign the maximum priority to applications with 0 income
        }
        return app.loanAmount / app.annualIncome;
    }

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
    
    loanSystem.addApplication(LoanApplication("Alice", 750, true, 60000, 30000));
    loanSystem.addApplication(LoanApplication("Bob", 720, true, 80000, 25000));
    loanSystem.addApplication(LoanApplication("Charlie", 780, true, 40000, 15000));
    loanSystem.addApplication(LoanApplication("Diana", 710, true, 100000, 50000));
    loanSystem.addApplication(LoanApplication("Ethan", 730, true, 30000, 20000));
    loanSystem.addApplication(LoanApplication("Frank", 800, false, 70000, 40000));

    loanSystem.processApplications();
    return 0;
}
