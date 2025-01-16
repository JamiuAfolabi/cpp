#include <iostream>
#include <deque>
#include <vector>
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

// Template alias to select between deque or vector as the underlying container
template <typename Container = std::deque<LoanApplication>>
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
            
            // For deque, pop from the front, for vector, pop from the front manually
            if constexpr (std::is_same_v<Container, std::deque<LoanApplication>>) {
                applicationQueue.pop_front(); // works with deque
            } else {
                applicationQueue.erase(applicationQueue.begin()); // works with vector
            }
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
    // Using deque as the container type
    LoanProcessingSystem<> loanSystemDeque;

    loanSystemDeque.addApplication(LoanApplication("Alice Smith", 720, true, 60000, 25000));
    loanSystemDeque.addApplication(LoanApplication("Bob Johnson", 680, true, 50000, 30000));
    loanSystemDeque.addApplication(LoanApplication("Charlie Brown", 750, false, 70000, 20000));
    loanSystemDeque.addApplication(LoanApplication("Diana Prince", 710, true, 40000, 18000));
    loanSystemDeque.addApplication(LoanApplication("Ethan Hunt", 800, true, 90000, 50000));
    
    loanSystemDeque.processApplications();

    // Using vector as the container type
    LoanProcessingSystem<std::vector<LoanApplication>> loanSystemVector;

    loanSystemVector.addApplication(LoanApplication("Jack Daniels", 740, true, 65000, 28000));
    loanSystemVector.addApplication(LoanApplication("Nina Williams", 680, true, 55000, 25000));
    loanSystemVector.addApplication(LoanApplication("Oscar Wilde", 710, false, 75000, 21000));
    loanSystemVector.addApplication(LoanApplication("Lana Lang", 690, true, 42000, 16000));
    loanSystemVector.addApplication(LoanApplication("Tom Cruise", 810, true, 95000, 52000));
    
    loanSystemVector.processApplications();

    return 0;
}
