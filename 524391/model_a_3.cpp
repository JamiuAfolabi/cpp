#include <iostream>
#include <deque>
#include <string>
#include <algorithm>

struct LoanApplication {
    std::string applicantName;
    int creditScore;
    bool employed;
    double annualIncome;
    double loanAmount;

    LoanApplication(std::string name, int score, bool emp, double income, double amount)
        : applicantName(name), creditScore(score), employed(emp), annualIncome(income), loanAmount(amount) {}

    double loanToIncomeRatio() const {
        return loanAmount / annualIncome;
    }
};

class LoanProcessingSystem {
private:
    std::deque<LoanApplication> applicationQueue;

public:
    void addApplication(const LoanApplication& application) {
        // Insert the application in sorted order based on loan-to-income ratio
        auto it = std::lower_bound(applicationQueue.begin(), applicationQueue.end(), application,
            [](const LoanApplication& a, const LoanApplication& b) {
                return a.loanToIncomeRatio() < b.loanToIncomeRatio();
            });
        applicationQueue.insert(it, application);
    }

    void processApplications() {
        std::cout << "Processing Loan Applications...\n";

        while (!applicationQueue.empty()) {
            const auto& app = applicationQueue.front();
            if (isEligible(app)) {
                std::cout << "Applicant: " << app.applicantName << " - Approved\n";
            } else {
                std::cout << "Applicant: " << app.applicantName << " - Denied\n";
            }
            applicationQueue.pop_front();
        }
    }

private:
    bool isEligible(const LoanApplication& app) {
        if (app.creditScore < 700) {
            return false;
        }
        if (!app.employed) {
            return false;
        }
        if (app.loanAmount > 0.5 * app.annualIncome) {
            return false;
        }
        return true;
    }
};

int main() {
    LoanProcessingSystem loanSystem;

    loanSystem.addApplication(LoanApplication("Alice Smith", 720, true, 60000, 25000));
    loanSystem.addApplication(LoanApplication("Bob Johnson", 680, true, 50000, 30000));
    loanSystem.addApplication(LoanApplication("Charlie Brown", 750, false, 70000, 20000));
    loanSystem.addApplication(LoanApplication("Diana Prince", 710, true, 40000, 18000));
    loanSystem.addApplication(LoanApplication("Ethan Hunt", 800, true, 90000, 50000));
    
    loanSystem.processApplications();

    return 0;
}