#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct LoanApplication {
    string name;
    int age;
    int creditScore;
    bool isEmployed;
    double annualIncome;
    double loanAmount;

    LoanApplication(string name, int age, int creditScore, bool isEmployed, double annualIncome, double loanAmount) 
        : name(name), age(age), creditScore(creditScore), isEmployed(isEmployed), annualIncome(annualIncome), loanAmount(loanAmount) {}
};

bool meetsLoanCriteria(const LoanApplication& app) {
   if (app.creditScore < 700) {
        cout << "Rejected: Credit score must be 700 or higher." << endl;
        return false;
    }

    if (!app.isEmployed) {
        cout << "Rejected: Applicant must be employed." << endl;
        return false;
    }

    double maxLoanAmount = app.annualIncome * 0.5;
    if (app.loanAmount > maxLoanAmount) {
        cout << "Rejected: Loan amount exceeds 50% of annual income." << endl;
        return false;
    }

    cout << "Approved!" << endl;
    return true;
}

void processLoanApplications(vector<LoanApplication>& applications) {
    for (const auto& application : applications) {
        meetsLoanCriteria(application);
    }
}

int main() {
    vector<LoanApplication> applications;

    LoanApplication app1 {"Alice", 30, 750, true, 60000, 25000};
    LoanApplication app2 {"Bob", 25, 680, true, 48000, 30000};
    LoanApplication app3 {"Charlie", 40, 780, false, 70000, 35000};
    LoanApplication app4 {"David", 28, 720, true, 80000, 32000};

    applications.push_back(app1);
    applications.push_back(app2);
    applications.push_back(app3);
    applications.push_back(app4);

    processLoanApplications(applications);

    return 0;
}