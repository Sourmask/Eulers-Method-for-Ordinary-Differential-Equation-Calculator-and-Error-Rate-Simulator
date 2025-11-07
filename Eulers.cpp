#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stack>
#include <string>
using namespace std;

class ODESolver {
private:
    double y0, h, xLower, xUpper;
    string infix, postfix;
    static const int MAX = 1000;
    double xArr[MAX], yEuler[MAX], yModified[MAX], yRunge[MAX];
    int n;

    // Operator precedence
    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        if (op == '^') return 3;
        return 0;
    }

    // Converting infix (from user) to postfix (used below) using STL stack
    string infixToPostfix(string expr) {
        stack<char> st;
        string output = "";
        for (int i = 0; i < expr.length(); i++) {
            char c = expr[i];
            if (c == ' ') continue;

            if (isalnum(c) || c == '.' || c == 'x' || c == 'y') {
                output += c;
                output += ' ';
            }
            else if (c == '(') {
                st.push(c);
            }
            else if (c == ')') {
                while (!st.empty() && st.top() != '(') {
                    output += st.top(); output += ' ';
                    st.pop();
                }
                if (!st.empty()) st.pop();
            }
            else {
                while (!st.empty() && precedence(st.top()) >= precedence(c)) {
                    output += st.top(); output += ' ';
                    st.pop();
                }
                st.push(c);
            }
        }
        while (!st.empty()) {
            output += st.top(); output += ' ';
            st.pop();
        }
        return output;
    }

    double evalPostfix(double x, double y) {
        stack<double> st;
        for (int i = 0; i < postfix.length();) {
            if (postfix[i] == ' ') { i++; continue; }

            // Number
            if (isdigit(postfix[i]) || postfix[i] == '.') {
                string num = "";
                while (i < postfix.length() && (isdigit(postfix[i]) || postfix[i] == '.'))
                    num += postfix[i++];
                st.push(stod(num));
            }

            // Variable
            else if (postfix[i] == 'x') { st.push(x); i++; }
            else if (postfix[i] == 'y') { st.push(y); i++; }

            // Operator
            else {
                if (st.size() < 2) return 0;
                double b = st.top(); st.pop();
                double a = st.top(); st.pop();
                switch (postfix[i]) {
                    case '+': st.push(a + b); break;
                    case '-': st.push(a - b); break;
                    case '*': st.push(a * b); break;
                    case '/': st.push(a / b); break;
                    case '^': st.push(pow(a, b)); break;
                }
                i++;
            }
        }
        return st.top();
    }

    // f(x, y)
    double f(double x, double y) {
        return evalPostfix(x, y);
    }

public:
    ODESolver(double y0, double h, double xLower, double xUpper, string infix) {
        this->y0 = y0;
        this->h = h;
        this->xLower = xLower;
        this->xUpper = xUpper;
        this->infix = infix;
        this->postfix = infixToPostfix(infix);
        this->n = 0;
    }

    // Generate all results using proper formulas
    void generateResults() {
        xArr[0] = xLower;
        yEuler[0] = y0;
        yModified[0] = y0;
        yRunge[0] = y0;
        n = 1;

        while (xArr[n-1] + h <= xUpper && n < MAX) {
            xArr[n] = xArr[n-1] + h;

            // Euler: y[n] = y[n-1] + h * f(x[n-1], y[n-1])
            yEuler[n] = yEuler[n-1] + h * f(xArr[n-1], yEuler[n-1]);

            // Modified Euler: y[n] = y[n-1] + h/2 * (f(x[n-1], y[n-1]) + f(x[n], y[n-1] + h*f(x[n-1], y[n-1])))
            double k1m = f(xArr[n-1], yModified[n-1]);
            double k2m = f(xArr[n], yModified[n-1] + h * k1m);
            yModified[n] = yModified[n-1] + (h/2.0)*(k1m + k2m);

            // Runge-Kutta 4th order
            double k1r = f(xArr[n-1], yRunge[n-1]);
            double k2r = f(xArr[n-1] + h/2.0, yRunge[n-1] + h/2.0*k1r);
            double k3r = f(xArr[n-1] + h/2.0, yRunge[n-1] + h/2.0*k2r);
            double k4r = f(xArr[n], yRunge[n-1] + h*k3r);
            yRunge[n] = yRunge[n-1] + (h/6.0)*(k1r + 2*k2r + 2*k3r + k4r);

            n++;
        }
    }

    void displayResults() {
        cout << "\n--------------------------------------------------------------\n";
        cout << "  x\t\tEuler\t\tModified\tRunge-Kutta\n";
        cout << "--------------------------------------------------------------\n";

        for (int i = 0; i < n; i++) {
            cout << fixed << setprecision(6)
                 << setw(8) << xArr[i] << "\t"
                 << setw(10) << yEuler[i] << "\t"
                 << setw(10) << yModified[i] << "\t"
                 << setw(10) << yRunge[i] << "\n";
        }
    }

    void saveToFile() {
        ofstream fout("euler_results.txt");
        fout << "x\tEuler\tModified\tRungeKutta\n";
        for (int i = 0; i < n; i++) {
            fout << xArr[i] << "\t" << yEuler[i] << "\t"
                 << yModified[i] << "\t" << yRunge[i] << "\n";
        }
        fout.close();
        cout << "\nResults saved to 'euler_results.txt'\n";
    }

    void findYat(double xTarget) {
        generateResults(); // ensure arrays are filled
        int index = 0;
        while (index < n-1 && xArr[index+1] <= xTarget) index++;

        cout << fixed << setprecision(6);
        cout << "\nAt x = " << xTarget << ":\n";
        cout << "Euler Method y = " << yEuler[index] << endl;
        cout << "Modified Euler y = " << yModified[index] << endl;
        cout << "Runge-Kutta y = " << yRunge[index] << endl;
    }
};

// ---------------- MAIN PROGRAM ----------------

int main() {
    double y0, h, xLower, xUpper;
    string expr;
    int choice;

    cout << "Enter function f(x, y) (e.g., 1/x+y, x*y+1, y-x*x+1): ";
    getline(cin, expr);
    cout << "Enter y0: ";
    cin >> y0;
    cout << "Enter step size h: ";
    cin >> h;
    cout << "Enter lower and upper limits of x: ";
    cin >> xLower >> xUpper;

    ODESolver solver(y0, h, xLower, xUpper, expr);

    do {
        cout << "\n=========== ODE SOLVING METHOD MENU ===========\n";
        cout << "1. Generate and Display Table\n";
        cout << "2. Find y at particular x\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            solver.generateResults();
            solver.displayResults();
            solver.saveToFile();
            break;
        case 2: {
            double xTarget;
            cout << "Enter x value to find y: ";
            cin >> xTarget;
            solver.findYat(xTarget);
            break;
        }
        case 3:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}
