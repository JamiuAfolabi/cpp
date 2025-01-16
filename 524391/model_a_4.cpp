#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <memory>

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

template<typename Container>
class LoanProcessingSystem {
private:
    Container applicationQueue;

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
            applicationQueue.pop_front();  // For deque; pop_back() for vector
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

// Type alias for LoanProcessingSystem using std::deque or std::vector
using LoanProcessingSystemDeque = LoanProcessingSystem<std::deque<LoanApplication>>;
using LoanProcessingSystemVector = LoanProcessingSystem<std::vector<LoanApplication>>;

int main() {
    // Using std::deque as the underlying data structure
    LoanProcessingSystemDeque loanSystemDeque;
    LoanProcessingSystemVector loanSystemVector;

    // Sample loan applications
    loanSystemDeque.addApplication(LoanApplication("Alice Smith", 720, true, 60000, 25000));
    loanSystemDeque.addApplication(LoanApplication("Bob Johnson", 680, true, 50000, 30000));
    loanSystemDeque.addApplication(LoanApplication("Charlie Brown", 750, false, 70000, 20000));
    loanSystemDeque.addApplication(LoanApplication("Diana Prince", 710, true, 40000, 18000));
    loanSystemDeque.addApplication(LoanApplication("Ethan Hunt", 800, true, 90000, 50000));

    loanSystemVector.addApplication(LoanApplication("Alice Smith", 720, true, 60000, 25000));
    loanSystemVector.addApplication(LoanApplication("Bob Johnson", 680, true, 50000, 30000));
    loanSystemVector.addApplication(LoanApplication("Charlie Brown", 750, false, 70000, 20000));
    loanSystemVector.addApplication(LoanApplication("Diana Prince", 710, true, 40000, 18000));
    loanSystemVector.addApplication(LoanApplication("Ethan Hunt", 800, true, 90000, 50000));

    // Process applications with deque
    loanSystemDeque.processApplications();
    loanSystemVector.processApplications();

    return 0;
}